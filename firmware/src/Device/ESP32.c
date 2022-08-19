/** @file [PowerManagement.h]
 *  @brief {Power Management module manage power, supply power for the system
 * including control the charging, consider the condition to decide the power is used is AC or
 * cradle battery or internal battery}
 *  @author {nguyen truong}
 */
/* ************************************************************************** */

#include "system_config.h"
//#include "system_definitions.h"

#include "Delay.h"
#include "system/debug/sys_debug.h"
#include "ESP32.h"

#define MSB_CRC8 (0x31)

#define BOOT_INDICATE (0x42)

#define READY_DOWNLOAD      (0x0A)

#define START_PACKET        (0x5A)

#define UART_SIZE           100
// Receive buffer for UART
uint8_t rxbuf[21] = {0xff};
// Transmit buffer for UART
uint8_t txbuf[UART_SIZE + 3] = {0};
// Length of data in rxbuf
uint8_t rxlen = 0;
// Size of firmware bin file
int bin_file_size = 0;
// Buffer to read every 100 byte from bin file
uint8_t transferbuf[UART_SIZE] ={0x00};
// Index of data byte was read from bin file
int binIndex = 0; 
// Number of byte read every data is sent <= 100
int bytesRead = 0;
// Just for test
int count = 0;

uint8_t* bin_file;



// Previous UART state
UARTState_t preUARTState = UPDATE_FIRMWARE_PING;
// Present UART state
UARTState_t UARTState = UPDATE_FIRMWARE_PING;
// Error check variable
E_UART_Error_t g_STTError;

static uint8_t esp32_SendData(uint8_t *buff, uint8_t len); 
static uint8_t esp32_SendUpdatePing(void);
static uint8_t esp32_SendEndFile(void);
static void esp32_ReceiveResponse(void);
static void esp32_HandleResponse(void);
static void esp32_Resend(void);
static int esp32_SendBinFile(void);
static int esp32_SendDataPacket(uint8_t *buff, uint8_t len);
static int esp32_ReceiveData();
static void esp32_PrintRxBuf(void);
static E_UART_Error_t esp32_checkCRC8(const uint8_t *buff, uint8_t size);
static uint8_t esp32_calCulateCRC8(const uint8_t *buff, uint8_t size);

/** @brief Init for upgrade firmware 
*    @param [I] data: data buffer
*    @param [I] fileSize: Size of firmware in bytes
*    @param [O] None
*    @return Init state
*    @retval true if initiation success or false if initiation false
*/
bool esp32_InitFirmware(void* data, uint32_t fileSize)
{  
    bin_file_size = fileSize;
    bin_file = data;
    
    preUARTState = UPDATE_FIRMWARE_PING;
    UARTState = UPDATE_FIRMWARE_PING;    
    
    
    memset((void*) txbuf, 0, 21);
    memset((void*) rxbuf, 0, 103);
    
    return true;
}

/** @brief Function to write value to register 
 *  @param [in]  uint8_t* data : The data was read from hex file 
 *  @param [out]  None
 *  @retval true update data success
 *  @retval false update data failed  
 */
bool esp32_UpgradeFirmware(uint8_t* data)
{
//     SYS_PRINT("\n esp32_UpgradeFirmware");
#define MASTER_SEND_DELAY   25
#define MASTER_RES_DELAY    5
    switch(UARTState)
    {
        case UPDATE_FIRMWARE_PING:SYS_PRINT("\n UPDATE_FIRMWARE_PING");
            if (esp32_SendUpdatePing())
            {
                delay_HardDelay(MASTER_SEND_DELAY);
                UARTState = WAIT_RESPONSE;
            }
            break;
        case SEND_DATA:SYS_PRINT("\n SEND_DATA");
            if (esp32_SendBinFile())
            {
                delay_HardDelay(MASTER_SEND_DELAY);
                UARTState = WAIT_RESPONSE;
            }
            break;
        case END_FILE:SYS_PRINT("\n END_FILE");
            if (esp32_SendEndFile())
            {
                delay_HardDelay(MASTER_SEND_DELAY);
                UARTState = WAIT_RESPONSE;
            }
            break;
        case WAIT_RESPONSE:SYS_PRINT("\n WAIT_RESPONSE");
        {
            esp32_ReceiveResponse();
            delay_HardDelay(MASTER_RES_DELAY);
            break;
        case UPDATE_DONE:SYS_PRINT("\n UPDATE_DONE");
            return true;
        }
            break;
    }
    // Clear buffer
    memset((void*) txbuf, 0, 21);
    memset((void*) rxbuf, 0, 103);
    return false;
}

/** @brief UART send data */
static uint8_t esp32_SendData(uint8_t *buff, uint8_t len)
{
    uint8_t rtn = Uart6_Send(buff, len);
    // SYS_PRINT("\n esp32_SendData rtn = %u", rtn);
    return rtn;
}

/** @brief Send update ping to ESP */
static uint8_t esp32_SendUpdatePing()
{
  txbuf[0] = START_PACKET;
  txbuf[1] = PACKET_TYPE_PING;
  txbuf[2] = BOOT_INDICATE;
  txbuf[3] = esp32_calCulateCRC8(txbuf, 3);
  return esp32_SendData(txbuf, 4);
}

/** @brief Send end of file to ESP */
static uint8_t esp32_SendEndFile()
{
    // SYS_PRINT("\nSend end of file");
    txbuf[0] = START_PACKET;
    txbuf[1] = END_OF_FILE;
    txbuf[2] = 0;
    txbuf[3] = esp32_calCulateCRC8(txbuf, 3);
    return esp32_SendData(txbuf, 4);
}

/** @brief Handle response from ESP */
static void esp32_ReceiveResponse()
{
    if (esp32_ReceiveData())
    {
        g_STTError = esp32_checkCRC8(rxbuf, 4);
        if (g_STTError != UART_NO_ERROR)
        {
            UARTState = preUARTState;
            return;
        }
        else
        {
            esp32_HandleResponse();
        }
    }
}

/** @brief Handle response from ESP*/ 
static void esp32_HandleResponse(void)
{
    uint8_t startPacket = rxbuf[0];
    uint8_t packetType = rxbuf[1];
    uint8_t responseContent = rxbuf[2];
    if (startPacket == START_PACKET)
    {
        switch(packetType)
        {
            case PACKET_TYPE_ACK:
                if (responseContent == OTA_WRITE_SUCCESS)
                {
                    if (binIndex >= bin_file_size)
                    {
                        UARTState = END_FILE;
                    }
                    else
                    {
                        UARTState = SEND_DATA;
                    }
                }

                break;
            case PACKET_TYPE_NACK:
                if (UARTState == SEND_DATA)
                {
                    binIndex -= bytesRead;
                    UARTState = preUARTState;
                }
                else
                {
                    UARTState = preUARTState;
                }

                break;
            case PACKET_TYPE_PING_RESPONSE:
          
                if (responseContent == OTA_BEGIN_SUCCESS) 
                {
                  binIndex = 0;
                  UARTState = SEND_DATA;
                }
          
                break;
            case PACKET_TYPE_DOWNLOAD_RESPONSE:
                if (responseContent == OTA_DOWNLOAD_FIRMWARE_SUCCESS)
                {
                  UARTState = UPDATE_DONE;
                }
                else
                {
                  UARTState = UPDATE_DONE;
                }
            
            default:
                break;
        }
    }
    
    
    return;
}

/** @brief Send 100 bytes to ESP */
static int esp32_SendBinFile(void)
{
    int i;
    for (i = 0; i < UART_SIZE; i++)
    {
          transferbuf[i] = *(bin_file + binIndex);
          binIndex++;
          if (binIndex > bin_file_size) break;
    }
    bytesRead = i;

    return esp32_SendDataPacket(transferbuf, bytesRead);
}

/** @brief Send data to ESP*/
static int esp32_SendDataPacket(uint8_t *buff, uint8_t len)
{
    txbuf[0] = START_PACKET;
    txbuf[1] = PACKET_TYPE_DATA;
    
    int i = 0;
    
    for (i = 0; i < len; i++)
    {
      txbuf[i + 2] = buff[i];
    }
    
    txbuf[len + 2] = esp32_calCulateCRC8(txbuf, len + 2);
    
    return esp32_SendData(txbuf, len + 3);
}

/** @brief Receive Data */
static int esp32_ReceiveData()
{
    return Uart6_ReadReceiveBuffer(rxbuf,4);
}

/** @brief Check CRC8 error */
static E_UART_Error_t esp32_checkCRC8(const uint8_t *buff, uint8_t size) {
    E_UART_Error_t temp;
    
    uint8_t crcValue = buff[size - 1];
    
    if (crcValue == esp32_calCulateCRC8(buff, (size - 1))) {
        temp = UART_NO_ERROR;
    } else
        temp = UART_CRC_FAILED;

    return temp;
}

/** @brief Calculate CRC */
static uint8_t esp32_calCulateCRC8(const uint8_t *buff, uint8_t size) {
    uint8_t *p = (uint8_t *) buff;
    uint8_t crc8;

    for (crc8 = 0x00; size != 0; size--) {
        crc8 ^= *p++;
        
        uint8_t i ;
        
        for ( i = 0; i < 8; i++) {
            if (crc8 & 0x80) {
                crc8 <<= 1;
                crc8 ^= MSB_CRC8;
            } else {
                crc8 <<= 1;
            }
        }
    }
    return crc8;
}