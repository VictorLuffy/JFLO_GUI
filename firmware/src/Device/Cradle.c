/* ************************************************************************** */
/** @file [Cradle.c]
 *  @brief {support interface to communicate with Cradle to get 
 * information, and control, report error if occur during communication}
 *  @author {nguyen truong}
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

#include "system_config.h"
#include "system_definitions.h"
#include "Delay.h"

#include "ApplicationDefinition.h"
#include "I2C_2.h"
#include "Cradle.h"
#include "ADC.h"
#include "../system/SoftwareUpgrade.h"
#include "DeviceInformation.h"

//#define DEBUG_PRINT_CRALDE

#ifdef DEBUG_PRINT_CRALDE
    #define SYS_PRINT(fmt, ...)                 SYS_DEBUG_Print(fmt, ##__VA_ARGS__)
#else
    #undef SYS_PRINT
    #define SYS_PRINT(fmt, ...)                 
#endif


//#include "AlarmInterface.h"
//#include "ApplicationDefinition.h"

typedef enum{
    eCradleWriteRqst,
    eCradleReadRqst,
    eCradleErrorState, 
}E_CradleRequestState;

typedef enum
{
    eSetCradleSN                 = 0x01,     /**<CRADLE SN can be set from the external devices via I2Cbus*/
    eGetCradleSN                 = 0x02,     /**<The CRADLE SN can be gotten via I2Cbus*/
    eGetBatteryConnection        = 0x03,     /**<Battery connection status can be acquired via I2Cbus*/
    eGetBatteryRemainingCapacity = 0x04,     /**<The remaining battery level can be obtained via I2Cbus*/
    eGetBatteryStatus            = 0x05,     /**<Battery status can be acquired via I2Cbus*/
    eGetBatteryAverageCurrent    = 0x06,     /**<The average current of the battery for 1 minute can be obtained via I2Cbus*/
    eGetBatteryVoltage           = 0x07,     /**<Battery voltage can be obtained via I2Cbus*/
    eSetWaterSupplyOnOff         = 0x08,     /**<The ability to control the cradle water pump via I2C*/
    eSetPowerSupplyOnOff         = 0x09,     /**<The power output of the cradle can be controlled via I2C*/
    eGetBatteryLidState          = 0x0A,     /**<The state of Battery lid can be gotten via I2Cbus */
    eGetBatteryRemainingPercent  = 0x0B, /**<The percentage of Battery can be gotten via I2Cbus */   
    eGetCradleFirmwareVersion    = 0x21,     /**< Get Cradle Firmware Version*/
} E_CRADLE_CMD;


typedef enum
{
    //System do not detect any error
    eNoError = 0x00,  
    //Received command is not supported
    eUnsupportedCommand = 0x01,
    //Received command is not supported        
    eInvalidData = 0x02,
    //System detected that Error occurred in data transfer etc
    eCRCErr = 0x03,
    
    //Required to get the SN when the SN is not Set
    eSNIsNotSet = 0x10,
    
    //System detected that the battery lid has been opened        
    eBatteryLidWasOpened = 0x11,
            
    eInternalErr = 0xFF	,
                      
} E_CRADLE_ERR;

    

        
#define ADDRESS_NUM_OF_BYTE   1
#define COMMAND_NUM_OF_BYTE   1
#define CRC_NUM_OF_BYTE       1
#define CRADLE_SERIAL_NUMBER_LENGTH       13 


#define POLYNOMIAL_CRC8    (0x31)      // 1 0000 0111 x8 + x2 + x1 + x0

/** @brief I2C slave address of cradle*/
#define CRADLE_BASE_ADDR                 0x08

/** @brief I2C address while writing data to cradle
 * write address = base address << 1 + write bit (0))
 */
#define CRADLE_WRITE_ADDR                (CRADLE_BASE_ADDR << 1)

/** @brief I2C address while reading data from cradle
 * read address = base address << 1 + read bit(1))
 */
#define CRADLE_READ_ADDR                 ((CRADLE_BASE_ADDR << 1) + 1)

/** @brief max time wait for charger communication (in ms) via I2C*/
#define CRADLE_COMM_MAX_WAIT_MS          (/*40*/10 / portTICK_PERIOD_MS)

/** @brief size of buffer that store command sent to cradle */
#define     CRADLE_COMMAND_QUEUE_SIZE       (8)

/** @brief max time wait for send command to queue */
#define     QUEUE_CRADLE_COMMAND_WAITING     (2)


/** @brief a queue to store command sent to cradle */
static QueueHandle_t gs_CradleCommandQueue = NULL;

/** @brief store error state of the communication with cradle*/
static E_DeviceErrorState gs_communicationErr = eDeviceNoError;

static E_ConnectState gs_CradleConnection = eDisconnect;


static uint8_t gs_CradleSN[CRADLE_SERIAL_NUMBER_LENGTH] = {};
static uint8_t gs_CradleSNSetValue[CRADLE_SERIAL_NUMBER_LENGTH] = {};
static uint8_t gs_CradleFwVersion[3] = {0,0,0};
static E_ConnectState gs_CradleBatteryConnection = eDisconnect;
static uint16_t gs_CradleBatteryRemainPercent = 0;
static uint16_t gs_CradleBatteryRemainCapacity = 0;
static uint16_t gs_CradleBatteryStatus = 0;
static int16_t gs_CradleBatteryAverageCurrent = 0;
static uint16_t gs_CradleBatteryVoltage = 0;
static E_OnOffState gs_WaterSupplyState = eOff;
static E_OnOffState gs_PowerSupplyState = eOff;

static bool gs_IsUpdateCradleSNToGUIInfo = true;
static bool gs_IsUpdateCradleVerToGUIInfo = true;
//static bool gs_IsUpdateCradleUsageTimeToGUIInfo = true;


static bool s_checkSumWait = false;
static uint8_t lineErorrCheckCounter = 0;

/** @brief current command to cradle */
static E_CRADLE_CMD gs_currentCommand = eGetCradleSN;


/** @brief Define the data packet type */
#define PING_COMMAND                    0xA0
#define ERASE_COMMAND                   0xA1
#define FLASH_DATA_COMMAND              0xA2

#define BOOT_READY_RESPONSE             0xA3
#define ACK_RESPONSE                    0xA4
#define NACK_RESPONSE                   0xA5
#define HEXFILE_CHECKSUM_ERROR_RESPONSE 0xA6
#define CRC_FAIL_RESPONSE               0xA7
#define INVALID_COMMAND_RESPONSE        0xFF
#define PACKET_TYPE_COMMAND             0xB0
#define PACKET_TYPE_RESPONSE            0xB1

#define START_PACKET                0x5A
#define MSB_CRC8                    (0x31)

/** @brief Define flag upgrade */
static bool s_sendCommandFlag = true;

/** @brief Define flag upgrade */
static uint32_t byteIndex = 0;

/** @brief Define flag upgrade */
static uint8_t sendBytes = 0;

/** @brief Define flag upgrade */
static uint8_t rxBuf[27] = {0};

/** @brief Define flag upgrade */
static uint8_t txBuf[27] = {0};

static E_PIC18UpgradeFirmwareState upgradeFirmwareState = eJumpToBld; //MASTER_WAIT_BOOT_RESPONSE;

static uint8_t dataSendLen = 0;
/** @brief Define flag upgrade */
static uint8_t err;

static bool s_IsUpdateSuccess = false;

/** @brief Define flag upgrade */
static uint8_t cradle_CalCulateCRC8( const void *buff, size_t size );

/** @brief Define flag upgrade */
static uint8_t Cradle_Ascii2Hex(uint8_t c);

static void Cradle_PrepareCommand(uint8_t command, uint8_t* data, uint8_t size);




/** @brief create CRC8 
 *  @param [in]  const void *buff   
 *  @param [out]  None
 *  @return uint8_t
 *  @retval crc8: CRC of array
 */
static uint8_t cradle_GetCRC8( const void *buff, size_t size )
{
    uint8_t *p = (uint8_t *)buff;
    uint8_t crc8;
    uint8_t i;

    for ( crc8 = 0x00 ; size != 0 ; size-- )
    {
        crc8 ^= *p++;

        for ( i = 0 ; i < 8 ; i++ )
        {
            if ( crc8 & 0x80 )
            {
                crc8 <<= 1; crc8 ^= POLYNOMIAL_CRC8;
            }
            else
            {
                crc8 <<= 1;
            }
        }
    }
    return crc8;
}

/** @brief Initialize the cradle unit
 * This function should be called before RTOS is started and call 1 time only after
 * start up system
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void cradle_Initialize()
{
      //Create a queue capable of containing 8 items of command (uint8_t) 
      gs_CradleCommandQueue = xQueueCreate(CRADLE_COMMAND_QUEUE_SIZE, sizeof (E_CRADLE_CMD));
      if (gs_CradleCommandQueue == NULL)
      {
          //Queue was not created and must not be used.
      }
}

/** @brief Function to get next command write to cradle
 *  @param [in]  None   
 *  @param [out]  None
 *  @return E_CRADLE_CMD command
 */
E_CRADLE_CMD cradle_GetNextCommand()
{
    E_CRADLE_CMD cmd;
    //check there are any "write data" command in the queue
    if (xQueueReceive(gs_CradleCommandQueue, &cmd, 0) == pdTRUE) //wait 0 tick (do not wait)
    {
        return cmd;
    }
    //if there is no command in the queue, get next command get data
    switch (gs_currentCommand)
    {  
        case eGetCradleSN:
            gs_currentCommand = eGetBatteryConnection;
            break;
            
        case eGetBatteryConnection:
            gs_currentCommand = eGetBatteryRemainingPercent;
            break;
            
        case eGetBatteryRemainingPercent:
            gs_currentCommand = eGetBatteryRemainingCapacity;
            break;
            
        case eGetBatteryRemainingCapacity:
            gs_currentCommand = eGetBatteryStatus;
            break;
            
        case eGetBatteryStatus:
            gs_currentCommand = eGetBatteryAverageCurrent;
            break;
            
        case eGetBatteryAverageCurrent:
            gs_currentCommand = eGetBatteryVoltage;
            break;
            
        case eGetBatteryVoltage:
            gs_currentCommand = eGetCradleFirmwareVersion;
            break;
        case eGetCradleFirmwareVersion:
            gs_currentCommand = eGetCradleSN;
            break;
            
        default:
            gs_currentCommand = eGetCradleSN;
            break;
    }
    return gs_currentCommand;                
}

/** @brief Function to wite command to cradle
 *  @param [in]  uint8_t command: command to cradle
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool cradle_WriteCommand(uint8_t command)
{
    bool rtn = true;
    uint8_t buffWrite[20] = {};
    uint8_t size = 0;
    uint8_t idx = 0;

    switch (command)
    {
        case eGetCradleSN:
        case eGetBatteryConnection:
        case eGetBatteryRemainingPercent:
        case eGetBatteryRemainingCapacity:
        case eGetBatteryStatus:
        case eGetBatteryAverageCurrent:
        case eGetBatteryVoltage:  
        case eGetCradleFirmwareVersion:
            size = 1;
            buffWrite[0] = command;
            buffWrite[1] = cradle_GetCRC8(buffWrite, size);
            if (I2C2_Write(CRADLE_WRITE_ADDR, 
                        (void*)buffWrite, size+1, CRADLE_COMM_MAX_WAIT_MS) == false)
            {
                  SYS_PRINT("Cradle I2C_2 Write Fail\n");
                  rtn = false;
            } 
            break;
            
        case eSetCradleSN:
            //data size
            size = CRADLE_SERIAL_NUMBER_LENGTH;
            //byte command code
            buffWrite[0] = command;
            //bytes data
            //copy serial number to write buffer
            for(idx = 1; idx < (size+1); idx++)
            {
               buffWrite[idx] = gs_CradleSNSetValue[idx - 1];
            }
            //calculate CRC
            buffWrite[size+1] = cradle_GetCRC8(&buffWrite[0], size + 1);
            if (I2C2_Write(CRADLE_WRITE_ADDR, 
                  (void*)buffWrite, (size+2) , CRADLE_COMM_MAX_WAIT_MS) == false)
            {
                SYS_PRINT("Cradle I2C_2 SET_SN_CMD Write Fail\n");
                rtn =  false;
            }
            break;
        case eSetWaterSupplyOnOff:
        case eSetPowerSupplyOnOff:
            size = 2;
            buffWrite[0] = command;
            if(command == eSetWaterSupplyOnOff){
                buffWrite[1] = gs_WaterSupplyState;
            }
            else{
                buffWrite[1] = eSetPowerSupplyOnOff;
            }
            buffWrite[2] = cradle_GetCRC8(buffWrite, size);
            if (I2C2_Write(CRADLE_WRITE_ADDR, 
                        (void*)buffWrite, size+1, CRADLE_COMM_MAX_WAIT_MS) == false)
            {
                  SYS_PRINT("Cradle I2C_2 Write Fail\n");
                  rtn = false;
            } 
            break;
        default:
            break;
            
    }
    return rtn;      
}

/** @brief Function to read data from Cradle and process it
 *  @param [in]  E_CRADLE_CMD command: process data response of this command
 *  @retval true read data success
 *  @retval false read data failed  
 */
bool cradle_ReadAndProcessReadData(E_CRADLE_CMD command)
{
    uint8_t buffRead[15] = {'\0'};
    uint8_t size = 0;
    bool rtn = true;
    
    switch (command)
    {
        case eGetCradleSN:
            size = CRADLE_SERIAL_NUMBER_LENGTH + 1;
            if(I2C2_Read(CRADLE_READ_ADDR, 
                (void*)buffRead, (size + 1), CRADLE_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Cradle I2C_2 Read Fail\n");
                rtn = false;
            }
            else
            {
                //calculate crc of the data read from cradle
                uint8_t crc = cradle_GetCRC8(buffRead, size);
                //compare crc
                if(crc == buffRead[size])
                {
                    E_CRADLE_ERR status = buffRead[0];
                    
                    if(status == eNoError)
                    {
                        memcpy(gs_CradleSN, &buffRead[1], CRADLE_SERIAL_NUMBER_LENGTH);
                        if(gs_IsUpdateCradleSNToGUIInfo == true)
                        {
                            //update cradle serial number to display at Info screen
                            char serial[16] = {};
                            memcpy(serial, gs_CradleSN, CRADLE_SERIAL_NUMBER_LENGTH);
                            if(DeviceInformation_SetDeviceInfo(eCradleSerialNoDeviceInfoId, serial) == true){
                                //clear flag to indicate data was updated
                                gs_IsUpdateCradleSNToGUIInfo = false;
                            }
                        }
                    }
                    else{
                        rtn = false;
                    }
 
                }
                else 
                {
                    rtn = false;
                    SYS_PRINT("eGetCradleSN cradle read data CRC Fail\n");
                }            
            }        
            break;
            
        case eGetBatteryConnection:
            size = 2;
            if(I2C2_Read(CRADLE_READ_ADDR, 
                (void*)buffRead, (size + 1), CRADLE_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Cradle I2C_2 Read Fail\n");
                rtn = false;
            }
            else
            {
                //calculate crc of the data read from cradle
                uint8_t crc = cradle_GetCRC8(buffRead, size);
                //compare crc
                if(crc == buffRead[size])
                {
                    E_CRADLE_ERR status = buffRead[0];
                    
                    if(status == eNoError){
                        gs_CradleBatteryConnection = (E_ConnectState)buffRead[1];                        
                    }
                    else{
                        rtn = false;
                    }
 
                }
                else 
                {
                    rtn = false;
                    SYS_PRINT("eGetBatteryConnection cradle read data CRC Fail\n");
                }            
            }        
            break;
        case eGetBatteryRemainingPercent:    
        case eGetBatteryRemainingCapacity:
        case eGetBatteryStatus:
        case eGetBatteryAverageCurrent:
        case eGetBatteryVoltage:
            size = 3;
            if(I2C2_Read(CRADLE_READ_ADDR, 
                (void*)buffRead, (size + 1), CRADLE_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Cradle I2C_2 Read Fail\n");
                rtn = false;
            }
            else
            {
                //calculate crc of the data read from cradle
                uint8_t crc = cradle_GetCRC8(buffRead, size);
                //compare crc
                if(crc == buffRead[size])
                {
                    E_CRADLE_ERR status = buffRead[0];
                    if(status == eNoError)
                    {   //SYS_PRINT("buffRead[1]= %d   buffRead[2] = %d\n", buffRead[1], buffRead[2]);
                        switch (command)
                        {
                            case eGetBatteryRemainingPercent:
                                gs_CradleBatteryRemainPercent = (buffRead[1] << 8) + buffRead[2];
                                break;
                                
                            case eGetBatteryRemainingCapacity:
                                gs_CradleBatteryRemainCapacity = (buffRead[1] << 8) + buffRead[2];
                                break;

                            case eGetBatteryStatus:
                                gs_CradleBatteryStatus = (buffRead[1] << 8) + buffRead[2];
                                break;

                            case eGetBatteryAverageCurrent:
                                gs_CradleBatteryAverageCurrent = (int16_t)((buffRead[1] << 8) + buffRead[2]);
                                break;    

                            case eGetBatteryVoltage:
                                gs_CradleBatteryVoltage = (buffRead[1] << 8) + buffRead[2];
                                break;       
                        }                        
                    }
                    else
                    {
                        rtn = false;
                    }
                }
                else 
                {
                    rtn = false;
                    SYS_PRINT("cradle read data CRC Fail  cmd code = %d\n", command);
                    SYS_PRINT("cradle read data buffRead[0..3] = %X %X %X %X\n", buffRead[0],buffRead[1],buffRead[2],buffRead[3]);
                }            
            }
            break;
        case eGetCradleFirmwareVersion:
            size = 3 + 1;//3 bytes data + 1 byte error status
            if(I2C2_Read(CRADLE_READ_ADDR, 
                (void*)buffRead, (size + 1), CRADLE_COMM_MAX_WAIT_MS) == false) // + 1 byte CRC
            {            
                SYS_PRINT("Cradle I2C_2 GetCradleFirmwareVersion Read Fail\n");
                rtn = false;
            }
            else
            {
                //SYS_PRINT("cradle read FW ver buffRead[0..3] = %X %X %X %X %X\n", buffRead[0],buffRead[1],buffRead[2],buffRead[3],buffRead[4]);
                //calculate crc of the data read from cradle
                uint8_t crc = cradle_GetCRC8(buffRead, size);
                //compare crc
                if(crc == buffRead[size])
                {
                    E_CRADLE_ERR status = buffRead[0];
                    if(status == eNoError)
                    { 
                        gs_CradleFwVersion[0] = buffRead[1];
                        gs_CradleFwVersion[1] = buffRead[2];
                        gs_CradleFwVersion[2] = buffRead[3];
                        
                        if(gs_IsUpdateCradleVerToGUIInfo == true)
                        {
                            //update cradle serial number to display at Info screen
                            char ver[16] = {};
                            sprintf(ver, "%d.%d.%d", gs_CradleFwVersion[0], gs_CradleFwVersion[1], gs_CradleFwVersion[2]);
                            if(DeviceInformation_SetDeviceInfo(eCradleSWVersionDeviceInfoId, ver) == true){
                                //clear flag to indicate data was updated
                                gs_IsUpdateCradleVerToGUIInfo = false;
                            }
                        }
                    }
                    else 
                    {
                        rtn = false;
                    }
                }
                else
                {
                    rtn = false;
                    SYS_PRINT("GetCradleFirmwareVersion read data CRC Fail\n");
                }
            }
            break;
              
        case eSetCradleSN:
        case eSetWaterSupplyOnOff:
        case eSetPowerSupplyOnOff:
            size = 2;
            if(I2C2_Read(CRADLE_READ_ADDR, 
                (void*)buffRead, (size + 1), CRADLE_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("cradle I2C_2 Read Fail\n");
                rtn = false;
            }
            else
            {
                //calculate crc of the data read from cradle
                uint8_t crc = cradle_GetCRC8(buffRead, size);
                //compare crc
                if(crc == buffRead[size])
                {
                    E_CRADLE_ERR status = buffRead[0];
                    
                    if(status == eNoError){
                        
                    }
                    else{
                        rtn = false;
                    }
                }
                else 
                {
                    rtn = false;
                    SYS_PRINT("cradle read data CRC Fail\n");
                }            
            }  
          break;
          
        default:
            break;
            
    }
    return rtn;
}

//this function should be called at least every 10ms
void cradle_Run()
{
    static E_CRADLE_CMD command = eGetBatteryConnection;
    static E_CradleRequestState state = eCradleWriteRqst;
    static uint16_t s_ErrorCounter = 0;
    
    float inputVoltage = 0;
    ADC_GetVoltage(ADC_VOLT_INPUT_MONITOR, &inputVoltage);
    //SYS_PRINT("V input %.2f \n", inputVoltage);
    //update connection state of cradle
    if(inputVoltage > 1.5)//TODO
    {
        gs_CradleConnection = eConnect;
    }
    else
    {
        gs_CradleConnection = eDisconnect;
        gs_CradleBatteryVoltage = 0;
        gs_IsUpdateCradleSNToGUIInfo = true;
        gs_IsUpdateCradleVerToGUIInfo = true;
    }
    
    if (gs_CradleConnection == eConnect)
    {
        switch (state)
        {
            case eCradleWriteRqst:
                command = cradle_GetNextCommand();
                if(cradle_WriteCommand(command) == true)
                {
                    state = eCradleReadRqst;
                    s_ErrorCounter = 0;
                }
                else{
                    state = eCradleErrorState;
                }
                break;

            case eCradleReadRqst:
                if (cradle_ReadAndProcessReadData(command) == true){
                    state = eCradleWriteRqst;
                    s_ErrorCounter = 0;
                }
                else{
                   state = eCradleErrorState;
                }
                break;

            case eCradleErrorState:
                s_ErrorCounter++;
                if(s_ErrorCounter >= 150)
                {
                    s_ErrorCounter = 0;
                    gs_communicationErr = eDeviceErrorDetected;
                }
                else
                {
                    //reset cradle
                    state = eCradleWriteRqst;
                }
                                           
                break;

            default:
                break;
        }
    }
                        
}

/** @brief Function to read error status from cradle every time a command set is send to cradle to check if error occurs 
 *  @param [in]  None  
 *  @param [out] uint8_t* status : pointer point to place error status is store if read success
 *  @retval true if read data success
 *  @retval false if read data failed  
 */
bool cradle_ReadErrorStatus(uint8_t* status)
{
    uint8_t buffRead[3];// = {'\0'};
    buffRead[0] = CRADLE_READ_ADDR;
    if(I2C2_Read(CRADLE_READ_ADDR, 
        (void*)&buffRead[1], sizeof(buffRead)-1, 
          CRADLE_COMM_MAX_WAIT_MS) == false)
    {            
        SYS_PRINT("I2C_2 Read Fail\n");
        return false;
    }
    else
    {
        //calculate crc of the data read from smart battery
        uint8_t crc = cradle_GetCRC8(buffRead, sizeof(buffRead)-1);
        //compare crc
        if(crc == buffRead[2])
        {
            *status = buffRead[1];
            return true;
        }
        else{
            return false;
        }
    }
}

/** @brief Function to get connection status of cradle
 *  @param [out]  None
 *  @retval E_ConnectState connection status of cradle
 */
E_ConnectState cradle_GetCradleConnection()
{    
    return gs_CradleConnection;
}


/** @brief Function to get battery connection status
 *  @param [out]  None
 *  @retval E_ConnectState connection status of battery
 */
E_ConnectState cradle_GetBatteryConnection()
{    
    return gs_CradleBatteryConnection;
}

/** @brief Function to get battery remaining percent(%)
 *  @param [out] None
 *  @retval uint16_t remaining percent of battery
 */
uint16_t cradle_GetBatteryRemainingPercent()
{    
    return gs_CradleBatteryRemainPercent;
}

/** @brief Function to get battery remaining capacity(mAh)
 *  @param [out] None
 *  @retval uint16_t remaining capacity of battery
 */
uint16_t cradle_GetBatteryRemainingCapacity()
{    
    return gs_CradleBatteryRemainCapacity;
}


/** @brief Function to get battery status
 *  @param [out]  None
 *  @retval uint16_t status of battery
 */
uint16_t cradle_GetBatteryStatus()
{    
    return gs_CradleBatteryStatus;
}


/** @brief Function to get battery average current
 *  @param [in]  None
 *  @param [out]  None
 *  @retval int16_t average current of battery
 */
int16_t cradle_GetBatteryAverageCurrent()
{    
    return gs_CradleBatteryAverageCurrent;
}

/** @brief Function to get battery voltage
 *  @param [in]  None
 *  @param [out]  None
 *  @retval uint16_t voltage of battery
 */
uint16_t cradle_GetBatteryVoltage()
{    
    return gs_CradleBatteryVoltage;
}


/** @brief Function to Get Cradle SN
 *  @param [out]  uint8_t *sn : pointer point to storage reading value
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool cradle_GetCradleSN(uint8_t *sn)
{
    memcpy(sn, gs_CradleSN, 13);
    return true;
}

/** @brief Function to Get Cradle firmware version
 *  @param [out]  float *fwVer : pointer point to storage reading value
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Cradle_GetCradleFirmwareVersion(uint8_t *fwVer)
{
    fwVer[0] = gs_CradleFwVersion[0]; 
    fwVer[1] = gs_CradleFwVersion[1]; 
    fwVer[2] = gs_CradleFwVersion[2];
    return true;
}

/** @brief Function to set Water Supply ON/OFF
 *  @param [in]  E_OnOffState state: state need to set
 *  @retval true if write data success
 *  @retval false if write data failed  
 */
bool cradle_SetWaterSupplyOnOff(E_OnOffState state)
{   
    uint8_t command = eSetWaterSupplyOnOff;
    gs_WaterSupplyState = state;

    if (xQueueSendToBack(gs_CradleCommandQueue, &command, QUEUE_CRADLE_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_CradleCommandQueue);
        return false;
    }
    return true;
}

/** @brief Function to set Power ON/OFF
 *  @param [in]  E_OnOffState state: state need to set
 *  @retval true if write data success
 *  @retval false if write data failed  
 */
bool cradle_SetPowerSupplyOnOff(E_OnOffState state)
{
    uint8_t command = eSetPowerSupplyOnOff;
    gs_PowerSupplyState = state;

    if (xQueueSendToBack(gs_CradleCommandQueue, &command, QUEUE_CRADLE_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_CradleCommandQueue);
        return false;
    }
    return true;
}

/** @brief Function to Set Cradle SN
 *  @param [in]  uint8_t sn : pointer to array contains serial data
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Cradle_SetCradleSN(uint8_t *sn)
{
    uint8_t command = eSetCradleSN;

    memcpy(gs_CradleSNSetValue, sn, 13);

    if (xQueueSendToBack(gs_CradleCommandQueue, &command, QUEUE_CRADLE_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_CradleCommandQueue);
        return false;
    }
    return true;   
}

/** @brief Get current status of cradle communication
*  @param [in]  None 
*  @param [out]  None
*  @return bool
*  @retval true if error, false if not error
*/
bool cradle_IsCommunicationFailure()
{
//    SYS_PRINT("\nFail check");
    if(gs_communicationErr != eDeviceNoError)
    {
        return true;
    }
    else
    {
        return false;
    }
}






/** @brief Function to write value to register 
 *  @param [in]  uint8_t* data : The data was read from hex file 
 *  @param [out]  None
 *  @retval true update data success
 *  @retval false update data failed  
 */
bool cradleFirmwareUpdate_InitFirmware(void)
{
    byteIndex = 0;
    sendBytes = 0;
    
    upgradeFirmwareState = eJumpToBld;
    
    memset((void*) txBuf, 0, 27);
    memset((void*) rxBuf, 0, 27);
    
    return true;
}


/** @brief Send command to Slave*/
static uint8_t cradle_CalCulateCRC8( const void *buff, size_t size )
{
	uint8_t *p = (uint8_t *)buff;
	uint8_t crc8;

	for ( crc8 = 0x00 ; size != 0 ; size-- )
	{
		crc8 ^= *p++;

        int i;
		for (i = 0 ; i < 8 ; i++ )
		{
			if ( crc8 & 0x80 )
			{
				crc8 <<= 1; crc8 ^= MSB_CRC8;
			}
			else
			{
				crc8 <<= 1;
			}
		}
	}
	return crc8;
}

/** @brief Send command to Slave*/
static uint8_t Cradle_Ascii2Hex(uint8_t c)
{
	if (c >= '0' && c <= '9')
	{
		return (uint8_t)(c - '0');
	}
	if (c >= 'A' && c <= 'F')
	{
		return (uint8_t)(c - 'A' + 10);
	}
	if (c >= 'a' && c <= 'f')
	{
        return (uint8_t)(c - 'a' + 10);
	}

	return 0;  // this "return" will never be reached, but some compilers give a warning if it is not present
}

/** @brief Function to copy transmit line in hex file to transmit buffer
 *  @param [in] uint8_t* data array data was read from hex file
 *  @param [out] None
 *  @return uint8_t Number of handled bytes
 */
static uint8_t Cradle_prepareFlashData(uint8_t* data)
{
    uint8_t transferBytes = 0;  // the number of data will be transfered to Cradle
    uint8_t handleBytes = 0;    // the number of data in hex file will be handled 
    uint8_t dataBytes = 0;      // Based on hex file, define number of data
    
    // Handle data in hex file
    if (*(data) == ':') 
    {
        uint8_t tmp1, tmp2;
        dataBytes &= 0x00;
        if (data[1] >= '0' && data[1] <= '9') tmp1 = data[1] - 48;
        else tmp1 = data[1] - 55;
        dataBytes |= tmp1 << 4;
        if (data[2] >= '0' && data[2] <= '9') tmp2 = data[2] - 48;
        else tmp2 = data[2] - 55;                
        dataBytes |= tmp2; 
//        SYS_PRINT("\n dataBytes = %x", dataBytes);
        handleBytes  = dataBytes * 2; //--> data bytes
        handleBytes += (1 /*Header*/ + 2/*Data digits*/ + 4/*Address*/ + 2/*Type*/ + 2 /*CRC*/  + 1 /*0x0D*/ + 1 /*0x0A*/) ;
    } 
    else 
    {
        SYS_PRINT("\n Fail data !! Stuck at line");
        return 0xFF;
    }
    
    transferBytes = dataBytes + 1/*Data digits*/ + 2/*Address*/ + 1/*Type*/ + 1 /*CRC*/;
    dataSendLen = transferBytes;
//    SYS_PRINT("\n transferBytes=%d \tData: %d\n\n", line_num++);
    
    uint8_t handledData[50], cnt;
    for (cnt = 0; cnt < transferBytes; cnt++)
    {
        uint8_t highByte = Cradle_Ascii2Hex(*(data + 1 + cnt*2));
        uint8_t lowByte = Cradle_Ascii2Hex(*(data + 1 + cnt*2 + 1));
        handledData[cnt] &= 0x00;
        handledData[cnt] = (highByte << 4) | lowByte;
//        SYS_PRINT("\t%x", handledData[cnt]);
    }
    
    // Transfer data to other device
    Cradle_PrepareCommand(FLASH_DATA_COMMAND, handledData, transferBytes);
    SYS_PRINT("\nTransfer byte: %d", transferBytes);
    return handleBytes;
}

/** @brief Function to process command such as: PING_COMMAND, ERASE_COMMAND, FLASH_DATA_COMAND
 *  @param [in]  uint8_t* data : command data need to be sent 
 *  @param [in]  uint8_t transferBytes: command data length need to be sent
 *  @param [out]  None
 *  @retval eProcessFail process command fail
 *  @retval eProcessSuccess process command success
 */
static E_processCommandState Cradle_ProcessCommand(uint8_t* data, uint8_t transferBytes)
{
#define MASTER_DELAY        20
#define MASTER_CMD_DELAY    0
#define MASTER_RES_DELAY    0
#define MASTER_WAIT_ERASE_FLASH 1000
    static uint8_t s_sendCommandCounter = 0;
    static uint8_t s_handleCommandResponseCounter = 0;
    E_PIC18ProcessCommandState processCommandState = eSendCommand;
    while (1)
    {    
        switch(processCommandState)
        {
            case eSendCommand:
            {
//                SYS_PRINT("\n\nSend cmd: [%x]", txBuf[1]);
////                SYS_PRINT("\n--Check[%d]", check);
////                if (check < 1)
////                {
////                    txBuf[2] = 0;
////                }
////                else if (check <= 2)
////                {
////                    txBuf[2] = Cradle_CalCulateCRC8(txBuf, 2);
////                }
////                check++;
//                    int j = 0;
//                SYS_PRINT("\nSend_Data - [%d]: ", transferBytes);
//                for (j = 0; j < transferBytes; j++)
//                {
//                    SYS_PRINT("%x ", txBuf[j]);
//                }
//                SYS_PRINT("\n");
                if (s_sendCommandCounter > 3)
                {
                    SYS_PRINT("\n\nResend Fail");
                    return eProcessFail;
                }
                
                s_sendCommandCounter++;
                if (I2C2_Write(CRADLE_WRITE_ADDR, (void *)data, transferBytes, CRADLE_COMM_MAX_WAIT_MS) == false)
                {
                    SYS_PRINT("\nI2C send command fail!");
                    return eProcessFail;
                }
                else
                {
                    processCommandState = eHandleCommandResponse;
                }
            }
                break;
            case eHandleCommandResponse:
            {
//                SYS_PRINT("\n\nHandle rsp");
                s_handleCommandResponseCounter++;
                if (s_handleCommandResponseCounter > 3)
                {
                    s_handleCommandResponseCounter = 0;
//                    return eProcessFail;
                    processCommandState = eSendCommand;
                    continue;
                }
                delay_HardDelay(MASTER_DELAY);
                if ((upgradeFirmwareState == eEraseFlash) || (upgradeFirmwareState == eJumpToBld) || (s_checkSumWait == true))
                    delay_HardDelay(1000);
                
                if (I2C2_Read(CRADLE_READ_ADDR, (void *)rxBuf, 3, CRADLE_COMM_MAX_WAIT_MS) == false)
                {
                    SYS_PRINT("\nWait response timeout.\n");
                    processCommandState = eSendCommand;
                    SYS_PRINT("\nrsp again.");
                    continue;
//                    return eProcessFail;
                }
                else
                {
                    SYS_PRINT("\n %u, %u, %u", rxBuf[0], rxBuf[1],rxBuf[2]);
                    if (cradle_CalCulateCRC8(rxBuf, 2) != rxBuf[2])
                    {
                        SYS_PRINT("\n\nResend command: [%x]",txBuf[1]);
                        processCommandState = eSendCommand;
                        delay_HardDelay(5);
                        break;
                    }

                    if (rxBuf[0] == 0x5A)
                    {
                        s_handleCommandResponseCounter = 0;
                        switch(rxBuf[1])
                        {
                            case ACK_RESPONSE:
                            {
                                s_sendCommandCounter = 0;
                                SYS_PRINT("\nProcess cmd done.");
                                return eProcessDone;
                            }
                                break;
                            case CRC_FAIL_RESPONSE:
                                SYS_PRINT("\nSlave check CRC fail.");
                            case NACK_RESPONSE:
                            {
                                processCommandState = eSendCommand;
                                continue;
                            }
                                break;
                            case HEXFILE_CHECKSUM_ERROR_RESPONSE:
                            {
                              SYS_PRINT("\nHex file error at line: %d", lineErorrCheckCounter);
                              return eProcessFail;
                            }
                            break;
                            case BOOT_READY_RESPONSE:
                            {
                                upgradeFirmwareState = eEraseFlash;
                                return (E_processCommandState)50;
                            }
                            break;
                            case INVALID_COMMAND_RESPONSE:
                            {
                                SYS_PRINT("\nSend Invalid Command to slave.");
                                return eProcessFail;
                            }
                            break;
                            default:
                                SYS_PRINT("Cradle Unknown Error");
                                break;
                        }
                    }
                    else
                    {
                        SYS_PRINT("\nWait response again");
                        processCommandState = eHandleCommandResponse;
                        continue;
                    }
                }
            }
                break;
            default:
                break;
        }
    }
}

/**@brief Function to copy transmit data to buffer
 * @param [in] uint8_t command: command code that wanted send to slave
 * @param [in] uint8_t* data: data that would be sent with command
 * @param [in] uint8_t size: size of data
 * @param [out] None
 * @return None
 */
static void Cradle_PrepareCommand(uint8_t command, uint8_t* data, uint8_t size)
{
    txBuf[0] = (uint8_t)START_PACKET;
    txBuf[1] = command;
    memcpy(&txBuf[2], data, size);
    txBuf[2 + size] = cradle_CalCulateCRC8(txBuf, 2 + size);
}

 /** @brief Function to upgrade firmware for Cradle
 *  @param [in]  uint8_t* data : The data was read from hex file
 *  @param [in]  uin32_t datalen: The length of data that was read from hex file 
 *  @param [out]  None
 *  @retval true update data success
 *  @retval false update data failed  
 */
bool Cradle_UpgradeFirmware(uint8_t* data, uint32_t datalen)
{
//SYS_PRINT("\nStart Upgrade firmware: [%d]", upgradeFirmwareState);
E_processCommandState s_processResult;
static uint8_t s_bootResponseCounter = 0;
switch(upgradeFirmwareState)
{
    case eIdle:
    break;
    case eJumpToBld:
    {
        SYS_PRINT("\n\nRequest to bld.");
        Cradle_PrepareCommand(PING_COMMAND, NULL, 0);
        s_processResult = Cradle_ProcessCommand(txBuf, 3);
        if (s_processResult == eProcessFail)
        {
            SYS_PRINT("\nJump to bld fail.");
            return true;
        }
        else if (s_processResult == eProcessDone)
        {
            upgradeFirmwareState = eWaitBldResponse;
        }
    }
        break;
    case eWaitBldResponse:
    {
        SYS_PRINT("\n\nWait bld response.");
        s_bootResponseCounter++;
        if (s_bootResponseCounter > 3)
        {
            SYS_PRINT("\nWait bootloader response fail!");
            return true;
        }
        delay_HardDelay(2000);
        if (I2C2_Read(CRADLE_READ_ADDR, (void *)rxBuf, 3, CRADLE_COMM_MAX_WAIT_MS) == false)
        {
            SYS_PRINT("\nI2C read command fail!");
        }
        else
        {
            SYS_PRINT("\n %x, %x, %x", rxBuf[0], rxBuf[1],rxBuf[2]);
            if (cradle_CalCulateCRC8(rxBuf, 2) == rxBuf[2]);
            //        SYS_PRINT("\nCRC8Success");
            else {
                upgradeFirmwareState = eWaitBldResponse;
            }

            if (rxBuf[0] == 0x5A)
            {
                if (rxBuf[1] == BOOT_READY_RESPONSE)
                    upgradeFirmwareState = eEraseFlash;
            }    
        }
    }
        break;
    case eEraseFlash:
    {
        SYS_PRINT("\n\nErase Flash");
        Cradle_PrepareCommand(ERASE_COMMAND, NULL, 0);
        s_processResult = Cradle_ProcessCommand(txBuf, 3);
        if (s_processResult == eProcessFail)
        {
            SYS_PRINT("\nErase fail");
            return true;
        }
        else if (s_processResult == eProcessDone)
        {
            upgradeFirmwareState = ePrepareFlashData;
        }
    }
        break;
    case ePrepareFlashData:
        SYS_PRINT("\n\nPrepare data");
        if (byteIndex >= datalen)
        {
            // End of flashing
//            s_checkSumWait = false;
            delay_HardDelay(200);
            SYS_PRINT("\nFlash Done.");
            s_IsUpdateSuccess = true;
            return true;
        }
        else
        {
            sendBytes = Cradle_prepareFlashData(data + byteIndex);
            SYS_PRINT("\nUploading status [%d%%]", (int)byteIndex * 100/datalen);
            byteIndex += sendBytes;
            if (byteIndex >= datalen)
            {
                s_checkSumWait = true;
            }
            upgradeFirmwareState = eFlashDataState;
        }
        break;
    case eFlashDataState:
    {
        SYS_PRINT("\n\nFlash data.");
        lineErorrCheckCounter++;
        s_processResult = Cradle_ProcessCommand(txBuf, 2 + dataSendLen + 1);
        if (s_processResult == eProcessFail)
        {
            SYS_PRINT("\nFlash fail");
            return true;
        }
        else if (s_processResult == eProcessDone)
        {
            upgradeFirmwareState = ePrepareFlashData;
        }
        memset((void*) txBuf, 0, 27);
        memset((void*) rxBuf, 0, 27);
    }
        break;
    default:
    
        break;
}
return false;
}

bool Hi_TestFunction()
{
    int i;
    for (i = 0; i < 10; i++)
    {
        SYS_PRINT("\nHandle upgrade cradle firmware");
    }
    return false;
}

/** @brief Function to get status update (success or failed)
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true update data success
 *  @retval false update data failed  
 */
bool Cradle_IsUpdateSuccess()
{
    return s_IsUpdateSuccess;
}