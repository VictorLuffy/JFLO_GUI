/* ************************************************************************** */
/** @file [DigitalPotentiometer.h]
 *  @brief {Supply APIs to get pressed state of the 
 *  @author {nguyen truong}
 */
/* ************************************************************************** */


#ifndef _ESP32_H    /* Guard against multiple inclusion */
#define _ESP32_H
 
// Definition type for UART state
typedef enum
{
  UPDATE_FIRMWARE_PING = 0,
  SEND_DATA,
  WAIT_RESPONSE,
  UPDATE_DONE,
  END_FILE
} UARTState_t;
// Definition for UART error
typedef enum
{   
    UART_NO_ERROR = 0x00,
    UART_UN_SUPPORT_CMD = 0x01,
    UART_IS_VALID_DATA = 0x02,
    UART_CRC_FAILED = 0x03
} E_UART_Error_t;

// Packet type definition
typedef enum
{
    PACKET_TYPE_ACK = 0xA0,
    PACKET_TYPE_NACK = 0xA1,
    PACKET_TYPE_PING = 0xA2,
    PACKET_TYPE_PING_RESPONSE = 0xA3,
    PACKET_TYPE_DATA = 0xA4,
    PACKET_TYPE_DOWNLOAD_RESPONSE = 0xA5,
    END_OF_FILE = 0xA6
} Packet_t;

// OTA state in ESP when download firmware
typedef enum
{
    OTA_BEGIN_SUCCESS = 0xB0,
    OTA_BEGIN_FAIL = 0xB1,
    OTA_WRITE_FAIL = 0xB2,
    OTA_WRITE_SUCCESS = 0xB3,
    OTA_END_FAIL = 0xB4,
    OTA_END_SUCCESS = 0xB5,
    OTA_DOWNLOAD_FIRMWARE_SUCCESS = 0xB6,
    OTA_DOWNLOAD_FIRMWARE_FAIL = 0xB7
} OTAState_t;



bool esp32_InitFirmware(void* data, uint32_t fileSize);

bool esp32_UpgradeFirmware(uint8_t* data);

////uint8_t esp32_SendData(uint8_t *buff, uint8_t len);
//uint8_t esp32_SendUpdatePing(void);
//uint8_t esp32_SendEndFile(void);
//void esp32_ReceiveResponse(void);
//void esp32_HandleResponse(void);
////void UART_Resend(void);
////int UART_SendBinFile(void);
//int esp32_SendDataPacket(uint8_t *buff, uint8_t len);
////int UART_ReceiveData();
//void esp32_PrintRxBuf(void);
////void ReceiveData(uint8_t *value, uint32_t n);
//E_UART_Error_t esp32_checkCRC8(const uint8_t *buff, uint8_t size);
//uint8_t esp32_calCulateCRC8(const uint8_t *buff, uint8_t size);

#endif