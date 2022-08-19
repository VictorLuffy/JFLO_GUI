/* ************************************************************************** */
/** @file [ChamberUnit.c]
 *  @brief {Connecting the main board with the Chamber module (Pic18) via I2C_3}
 *  @author {Le Cuong}
 */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

#include "system_config.h"
#include "system_definitions.h"
#include "driver/i2c/drv_i2c.h"
#include "I2C_3.h"
#include "ChamberUnit.h"
#include "AlarmInterface.h"
#include "ApplicationDefinition.h"
#include "CrcChamber.h"
#include "Delay.h"
#include "../system/SoftwareUpgrade.h"
#include "DeviceInformation.h"

#define DEBUG_PRINT_CHAMBER

#ifdef DEBUG_PRINT_CHAMBER
    #define SYS_PRINT(fmt, ...)                 SYS_DEBUG_Print(fmt, ##__VA_ARGS__)
#else
    #undef SYS_PRINT
    #define SYS_PRINT(fmt, ...)                 
#endif

typedef enum{
    eChamberWriteRqst,
    eChamberReadRqst,
    eChamberErrorState, 
}E_ChamberRequestState;


/** @brief I2C slave address of the Chamber */
#define CHAMBER_BASE_ADDR           (0x08)

/** @brief I2C address while writing data to Chamber Unit
 * write address = base address << 1 + write bit (0))
 */
#define CHAMBER_WRITE_ADDR          (CHAMBER_BASE_ADDR << 1)

/** @brief I2C address while reading data from Chamber Unit
 * read address = base address << 1 + read bit(1))
 */
#define CHAMBER_READ_ADDR           ((CHAMBER_BASE_ADDR << 1) + 1)

/** @brief max time wait for Chamber Unit communication (in ms) via I2C*/
#define CHAMBER_COMM_MAX_WAIT_MS     (/*40*/10 / portTICK_PERIOD_MS)

/** @brief max time wait for MUTEX to access protection resource */
#define CHAMBER_MUTEX_MAX_WAIT_MS    (2 / portTICK_PERIOD_MS)


/** @brief size of buffer that store command sent to chamber */
#define     CHAMBER_COMMAND_QUEUE_SIZE       (8)

/** @brief max time wait for send command to queue */
#define     QUEUE_CHAMER_COMMAND_WAITING     (2)



/** @brief Flag Connection with Chamber Unit has error
 * true mean error happened
 * false mean no error */
static E_DeviceErrorState s_ChamberError = eDeviceNoError;


/** @brief a queue to store command sent to chamber */
static QueueHandle_t gs_ChamberCommandQueue = NULL;



/** @brief the CMD */
        
#define GET_CHAMBER_SN_CMD                        0x01
#define GET_CHAMBER_OUT_TEMP_CMD                  0x02
#define GET_CHAMBER_EVT_TEMP_CMD                  0x03
#define GET_CHAMBER_WATER_LEVEL_CMD               0x04
#define GET_CHAMBER_USED_TIME_CMD                 0x05
#define GET_CHAMBER_FW_VERSION_CMD                0x06
#define GET_TANK_WATER_LEVEL_CMD                  0x07
       
#define GET_BREATHING_CIRCUIT_MANUF_CODE_CMD      0x10
#define GET_BREATHING_CIRCUIT_SN_CMD              0x11
#define GET_BREATHING_CIRCUIT_MANUF_DATE_CMD      0x12
#define GET_BREATHING_CIRCUIT_FACTORY_CODE_CMD    0x13
#define GET_BREATHING_CIRCUIT_START_USED_DAY_CMD  0x14
#define GET_BREATHING_CIRCUIT_CYCLE_COUNT_CMD     0x15
#define GET_BREATHING_CIRCUIT_USED_TIME_CMD       0x16
#define GET_BREATHING_CIRCUIT_OUT_TEMP_CMD        0x17
#define GET_BREATHING_CIRCUIT_TYPE_CMD            0x18
#define GET_BREATHING_CIRCUIT_CONNECTION_CMD      0x19
        
#define SET_CHAMBER_SN_CMD                        0x20
#define SET_WATER_PUMP_FREQ_CMD                   0x21
#define RESET_CHAMBER_USED_TIME_CMD               0x22
#define RESET_CHAMBER_ALL_CMD                     0x23

#define SET_BREATHING_CIRCUIT_MANUF_CODE_CMD      0x30
#define SET_BREATHING_CIRCUIT_SN_CMD              0x31
#define SET_BREATHING_CIRCUIT_FACTORY_CODE_CMD    0x32
#define SET_BREATHING_CIRCUIT_START_USED_DAY_CMD  0x33
#define RESET_BREATHING_CIRCUIT_CYCLE_COUNT_CMD   0x34
#define RESET_BREATHING_CIRCUIT_USED_TIME_CMD     0x35
#define RESET_BREATHING_CIRCUIT_ALL_CMD           0x36

#define SET_BREATHING_CIRCUIT_MANUF_DATE_CMD      0x37
#define SET_BREATHING_CIRCUIT_CYCLE_COUNT_CMD     0x38
#define SET_BREATHING_CIRCUIT_USED_TIME_CMD       0x39
#define SET_BREATHING_CIRCUIT_TYPE_CMD            0x3A
     
static void Chamber_PrepareCommand(uint8_t command, uint8_t* data, uint8_t size);

bool tooMuchWaterFag = 0;
/** @brief current command to chamber */
static gs_currentCommand = GET_CHAMBER_SN_CMD;

static float gs_ChamberOutletTemperature = 0;
static float gs_EVTTemperature = 0;
static float gs_BreathCircuitOutletTemperature = 0;
static uint32_t gs_ChamberUsedTime = 0;
static E_TankWaterLevel gs_TankWaterlevel = eTankIsDisconnected;
static uint8_t gs_ChamberFwVersion[3] = {0,0,0};
static uint8_t gs_ChamberSN[13] = {0};
static uint8_t gs_BreathCircuitManufacturerCode[2] = {};
static uint8_t gs_BreathCircuitSN[13] = {0};
static uint8_t gs_BreathCircuitManufactureDate[4] = {0};
static uint8_t gs_BreathCircuitFactoryCode = 0;

static uint8_t gs_BreathCircuitStartUsedDate[4] = {0};//{0x00, 0x03, 0x35, 0xF3}; // 210419 : 2021/04/19
static uint8_t gs_BreathCircuitCnt[2] = {0};//{0x00,0x01}; // 1 times used 
static uint8_t gs_BreathCircuitUsedTime[2] = {0};//{0x00,0x01}; // 1 hour used
static E_BreathingCircuitType gs_BreathCircuitType = eTypeOther;
static uint8_t gs_BreathCircuitConnectStt = 0;
static E_ChamberConnectState gs_ChamberConnection = eChamberConnected;

static bool gs_IsUpdateChamberSNToGUIInfo = true;
static bool gs_IsUpdateChamberVerToGUIInfo = true;
static bool gs_IsUpdateChamberUsageTimeToGUIInfo = true;

static bool gs_IsUpdateBCSNToGUIInfo = true;
static bool gs_IsUpdateBCStartDateToGUIInfo = true;
static bool gs_IsUpdateBCTypeTGUIInfor = true;
static bool gs_IsUpdateBCUsageTimeTGUIInfor = true;
static bool gs_IsUpdateBCFacCodeTGUIInfor = true;


static uint8_t gs_chamberSerialSetValue[13];
static uint32_t gs_breathCircuitMafCodeSetValue;
static uint8_t gs_breathCircuitSerialSetValue[13];
static uint8_t gs_breathCircuitFacCodeSetValue;
static uint32_t gs_breathCircuitStartUseDateSetValue;
static uint32_t gs_breathCircuitManufactureDateSetValue;
static uint16_t gs_breathCircuitUseTimeSetValue;
static uint16_t gs_breathCircuitCycleCountSetValue;
static E_BreathingCircuitType gs_breathCircuitTypeSetValue;
static float gs_pumpFreqSetValue;
static uint8_t dataSendLen = 0;
static bool s_checkSumWait = false;
static uint8_t lineErorrCheckCounter = 0;
static int check = 0;
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

static bool s_sendCommandFlag = true;
static uint32_t byteIndex = 0;
static uint8_t sendBytes = 0;

static uint8_t rxBuf[27] = {0};
static uint8_t rxLen = 0;
static uint8_t txBuf[27] = {0};
static uint8_t txLen = 0;
static E_PIC18UpgradeFirmwareState upgradeFirmwareState = eJumpToBld; //MASTER_WAIT_BOOT_RESPONSE;
//static uint8_t PreMasterState = MASTER_SEND_PING; 
static uint8_t err;
static bool s_IsUpdateSuccess = false;

static uint8_t Chamber_CalCulateCRC8( const void *buff, size_t size );
static uint8_t Chamber_Ascii2Hex(uint8_t c);


static void Chamber_ResetChamber()
{
//    SYS_PRINT("=========Reset Chamber Board=======================================================\n");
//    CHAMBER_ENABLEOff();
//    vTaskDelay(50);
//    CHAMBER_ENABLEOn();
//    vTaskDelay(50);
}

/** @brief Initialize the chamber unit
 * This function should be called before RTOS is started and call 1 time only after
 * start up system
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void Chamber_Initialize() 
{
    CHAMBER_ENABLEOn();
    
    //Create a queue capable of containing 8 items of command (uint8_t) 
    gs_ChamberCommandQueue = xQueueCreate(CHAMBER_COMMAND_QUEUE_SIZE, sizeof ( uint8_t));
    if (gs_ChamberCommandQueue == NULL)
    {
        //Queue was not created and must not be used.
        //Chamber_ReportError();
    }
}

/** @brief Function to get next command write to chamber
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
uint8_t Chamber_GetNextCommand()
{
    uint8_t cmd;
    //check there are any "set data" command in the queue
    if (xQueueReceive(gs_ChamberCommandQueue, &cmd, 0) == pdTRUE) //wait 0 tick (do not wait)
    {
        return cmd;
    }
    //if there is no command in the queue, get next command get data
    switch (gs_currentCommand)
    {
        case GET_CHAMBER_SN_CMD:
            gs_currentCommand = GET_CHAMBER_OUT_TEMP_CMD;
            break;
            
        case GET_CHAMBER_OUT_TEMP_CMD:
            gs_currentCommand = GET_CHAMBER_EVT_TEMP_CMD;
            break;
            
        case GET_CHAMBER_EVT_TEMP_CMD:
            //gs_currentCommand = GET_CHAMBER_WATER_LEVEL_CMD;
            gs_currentCommand = GET_CHAMBER_USED_TIME_CMD;
            break;
            
        case GET_CHAMBER_WATER_LEVEL_CMD:
            gs_currentCommand = GET_CHAMBER_USED_TIME_CMD;
            break;
            
        case GET_CHAMBER_USED_TIME_CMD:
            gs_currentCommand = GET_CHAMBER_FW_VERSION_CMD;
            break;
            
        case GET_CHAMBER_FW_VERSION_CMD:
            gs_currentCommand = GET_TANK_WATER_LEVEL_CMD;
            break;
            
        case GET_TANK_WATER_LEVEL_CMD:
            gs_currentCommand = GET_BREATHING_CIRCUIT_MANUF_CODE_CMD;
            break;
            
        case GET_BREATHING_CIRCUIT_MANUF_CODE_CMD:
            gs_currentCommand = GET_BREATHING_CIRCUIT_SN_CMD;
            break;
            
        case GET_BREATHING_CIRCUIT_SN_CMD:
            gs_currentCommand = GET_BREATHING_CIRCUIT_MANUF_DATE_CMD;
            break;
            
        case GET_BREATHING_CIRCUIT_MANUF_DATE_CMD: 
            gs_currentCommand = GET_BREATHING_CIRCUIT_FACTORY_CODE_CMD;
            break;
            
        case GET_BREATHING_CIRCUIT_FACTORY_CODE_CMD:
            gs_currentCommand = GET_BREATHING_CIRCUIT_START_USED_DAY_CMD;
            break;
            
        case GET_BREATHING_CIRCUIT_START_USED_DAY_CMD: 
           gs_currentCommand = GET_BREATHING_CIRCUIT_CYCLE_COUNT_CMD;
            break;
            
        case GET_BREATHING_CIRCUIT_CYCLE_COUNT_CMD:  
            gs_currentCommand = GET_BREATHING_CIRCUIT_USED_TIME_CMD;
            break;
            
        case GET_BREATHING_CIRCUIT_USED_TIME_CMD:   
            gs_currentCommand = GET_BREATHING_CIRCUIT_OUT_TEMP_CMD;
            break; 
            
        case GET_BREATHING_CIRCUIT_OUT_TEMP_CMD:  
            gs_currentCommand = GET_BREATHING_CIRCUIT_TYPE_CMD;
            break;
            
        case GET_BREATHING_CIRCUIT_TYPE_CMD:    
            gs_currentCommand = GET_BREATHING_CIRCUIT_CONNECTION_CMD;
            break;
            
        case GET_BREATHING_CIRCUIT_CONNECTION_CMD:   
            gs_currentCommand = GET_CHAMBER_SN_CMD;//GET_CHAMBER_OUT_TEMP_CMD;
            break;   
               
        case SET_WATER_PUMP_FREQ_CMD:
            gs_currentCommand = GET_CHAMBER_OUT_TEMP_CMD;
            break;           
        default:
            gs_currentCommand = GET_CHAMBER_OUT_TEMP_CMD;
            break;
    }
    return gs_currentCommand;                
}

/** @brief Function to wite command to chamber
 *  @param [in]  uint8_t command: command to chamber
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_WriteCommand(uint8_t command)
{
    bool rtn = true;
    uint8_t buffWrite[20] = {};
    uint8_t size = 0;
    uint8_t idx = 0;

    switch (command)
    {
        case GET_CHAMBER_SN_CMD:
        case GET_CHAMBER_OUT_TEMP_CMD:
        case GET_CHAMBER_EVT_TEMP_CMD:
        case GET_CHAMBER_WATER_LEVEL_CMD:
        case GET_CHAMBER_USED_TIME_CMD:
        case GET_CHAMBER_FW_VERSION_CMD:
        case GET_TANK_WATER_LEVEL_CMD:
        case GET_BREATHING_CIRCUIT_MANUF_CODE_CMD:      
        case GET_BREATHING_CIRCUIT_SN_CMD:              
        case GET_BREATHING_CIRCUIT_MANUF_DATE_CMD:      
        case GET_BREATHING_CIRCUIT_FACTORY_CODE_CMD:    
        case GET_BREATHING_CIRCUIT_START_USED_DAY_CMD:  
        case GET_BREATHING_CIRCUIT_CYCLE_COUNT_CMD:     
        case GET_BREATHING_CIRCUIT_USED_TIME_CMD:      
        case GET_BREATHING_CIRCUIT_OUT_TEMP_CMD:       
        case GET_BREATHING_CIRCUIT_TYPE_CMD:           
        case GET_BREATHING_CIRCUIT_CONNECTION_CMD:
        case RESET_CHAMBER_USED_TIME_CMD:
        case RESET_CHAMBER_ALL_CMD:
        case RESET_BREATHING_CIRCUIT_CYCLE_COUNT_CMD:
        case RESET_BREATHING_CIRCUIT_USED_TIME_CMD:
        case RESET_BREATHING_CIRCUIT_ALL_CMD:
            size = 1;
            buffWrite[0] = command;
            buffWrite[1] = crcChamber_Calculate(buffWrite, size);
            if (I2C3_Write(CHAMBER_WRITE_ADDR, 
                        (void*)buffWrite, size+1, CHAMBER_COMM_MAX_WAIT_MS) == false)
            {
                  SYS_PRINT("Chamber I2C_3 Write Fail\n");
                  rtn = false;
            } 
            break;
                       
        case SET_BREATHING_CIRCUIT_MANUF_CODE_CMD:
            //data size
            size = 2;
            //byte command code
            buffWrite[0] = command;
            //bytes data
            uint32_t manufCode = gs_breathCircuitMafCodeSetValue;

            for(idx = 1; idx < (size+1); idx++)
            {
               buffWrite[size+1-idx] = (manufCode >> 8*(idx-1)) & 0xFF;
            }
            buffWrite[size+1] = crcChamber_Calculate(&buffWrite[0], size + 1);
            if (I2C3_Write(CHAMBER_WRITE_ADDR, 
                  (void*)buffWrite, (size+2) , CHAMBER_COMM_MAX_WAIT_MS) == false)
            {
                SYS_PRINT("I2C_3 Chamber SET_BREATHING_CIRCUIT_MANUF_CODE_CMD Write Fail\n");
                rtn =  false;
            }
            break;
            
        case SET_BREATHING_CIRCUIT_SN_CMD:
            //data size
            size = 13;
            //byte command code
            buffWrite[0] = command;
            //bytes data
            //copy serial number to write buffer
            for(idx = 1; idx < (size+1); idx++)
            {
               buffWrite[idx] = gs_breathCircuitSerialSetValue[idx - 1];
            }
            //calculate CRC
            buffWrite[size+1] = crcChamber_Calculate(&buffWrite[0], size + 1);
            if (I2C3_Write(CHAMBER_WRITE_ADDR, 
                  (void*)buffWrite, (size+2) , CHAMBER_COMM_MAX_WAIT_MS) == false)
            {
                SYS_PRINT("I2C_3 Chamber SET_BREATHING_CIRCUIT_SN_CMD Write Fail\n");
                rtn =  false;
            }
            break;
            
        case SET_BREATHING_CIRCUIT_FACTORY_CODE_CMD:
            //data size
            size = 1;
            //byte command code
            buffWrite[0] = command;
            //byte data
            buffWrite[1] = gs_breathCircuitFacCodeSetValue;
            //byte CRC
            buffWrite[size+1] = crcChamber_Calculate(&buffWrite[0], size + 1);
            if (I2C3_Write(CHAMBER_WRITE_ADDR, 
                  (void*)buffWrite, (size+2) , CHAMBER_COMM_MAX_WAIT_MS) == false)
            {
                SYS_PRINT("I2C_3 Chamber SET_BREATHING_CIRCUIT_FACTORY_CODE_CMD Write Fail\n");
                rtn =  false;
            }
            break;
        case SET_BREATHING_CIRCUIT_START_USED_DAY_CMD:
            //data size
            size = 4;
            //byte command code
            buffWrite[0] = command;
            //bytes data
            uint32_t date = gs_breathCircuitStartUseDateSetValue;

            for(idx = 1; idx < (size+1); idx++)
            {
               buffWrite[size+1-idx] = (date >> 8*(idx-1)) & 0xFF;
            }
            //byte CRC
            buffWrite[size+1] = crcChamber_Calculate(&buffWrite[0], size + 1);
            if (I2C3_Write(CHAMBER_WRITE_ADDR, 
                  (void*)buffWrite, (size+2) , CHAMBER_COMM_MAX_WAIT_MS) == false)
            {
                SYS_PRINT("I2C_3 Chamber SET_BREATHING_CIRCUIT_START_USED_DAY_CMD Write Fail\n");
                rtn =  false;
            }
            break;
            
        case SET_WATER_PUMP_FREQ_CMD:
            //data size
            size = 2;
            //byte command code
            buffWrite[0] = command;
            //bytes data
            uint32_t freg = (uint32_t)(gs_pumpFreqSetValue * 100.0);//scale by 100

            for(idx = 1; idx < (size+1); idx++)
            {
               buffWrite[size+1-idx] = (freg >> 8*(idx-1)) & 0xFF;
            }
            buffWrite[size+1] = crcChamber_Calculate(&buffWrite[0], size + 1);
            if (I2C3_Write(CHAMBER_WRITE_ADDR, 
                  (void*)buffWrite, (size+2) , CHAMBER_COMM_MAX_WAIT_MS) == false)
            {
                SYS_PRINT("I2C_3 Chamber SET_WATER_PUMP_FREQ_CMD Write Fail\n");
                rtn =  false;
            }
            break;
            
        case SET_CHAMBER_SN_CMD:
            //data size
            size = 13;
            //byte command code
            buffWrite[0] = command;
            //bytes data
            //copy serial number to write buffer
            for(idx = 1; idx < (size+1); idx++)
            {
               buffWrite[idx] = gs_chamberSerialSetValue[idx - 1];
            }
            //calculate CRC
            buffWrite[size+1] = crcChamber_Calculate(&buffWrite[0], size + 1);
            if (I2C3_Write(CHAMBER_WRITE_ADDR, 
                  (void*)buffWrite, (size+2) , CHAMBER_COMM_MAX_WAIT_MS) == false)
            {
                SYS_PRINT("I2C_3 Chamber SET_CHAMBER_SN_CMD Write Fail\n");
                rtn =  false;
            }
            break;
        
        case SET_BREATHING_CIRCUIT_MANUF_DATE_CMD:
            //data size
            size = 4;
            //byte command code
            buffWrite[0] = command;
            //bytes data
            uint32_t d = gs_breathCircuitManufactureDateSetValue;

            for(idx = 1; idx < (size+1); idx++)
            {
               buffWrite[size+1-idx] = (d >> 8*(idx-1)) & 0xFF;
            }
            //byte CRC
            buffWrite[size+1] = crcChamber_Calculate(&buffWrite[0], size + 1);
            if (I2C3_Write(CHAMBER_WRITE_ADDR, 
                  (void*)buffWrite, (size+2) , CHAMBER_COMM_MAX_WAIT_MS) == false)
            {
                SYS_PRINT("I2C_3 Chamber SET_BREATHING_CIRCUIT_MANUF_DATE_CMD Write Fail\n");
                rtn =  false;
            }
            break;
        case SET_BREATHING_CIRCUIT_USED_TIME_CMD:
            //data size
            size = 2;
            //byte command code
            buffWrite[0] = command;
            //bytes data
            uint16_t time = gs_breathCircuitUseTimeSetValue;

            for(idx = 1; idx < (size+1); idx++)
            {
               buffWrite[size+1-idx] = (time >> 8*(idx-1)) & 0xFF;
            }
            //byte CRC
            buffWrite[size+1] = crcChamber_Calculate(&buffWrite[0], size + 1);
            if (I2C3_Write(CHAMBER_WRITE_ADDR, 
                  (void*)buffWrite, (size+2) , CHAMBER_COMM_MAX_WAIT_MS) == false)
            {
                SYS_PRINT("I2C_3 Chamber SET_BREATHING_CIRCUIT_USED_TIME_CMD Write Fail\n");
                rtn =  false;
            }
            break;
        case SET_BREATHING_CIRCUIT_CYCLE_COUNT_CMD:
            //data size
            size = 2;
            //byte command code
            buffWrite[0] = command;
            //bytes data
            uint16_t count = gs_breathCircuitCycleCountSetValue;

            for(idx = 1; idx < (size+1); idx++)
            {
               buffWrite[size+1-idx] = (count >> 8*(idx-1)) & 0xFF;
            }
            //byte CRC
            buffWrite[size+1] = crcChamber_Calculate(&buffWrite[0], size + 1);
            if (I2C3_Write(CHAMBER_WRITE_ADDR, 
                  (void*)buffWrite, (size+2) , CHAMBER_COMM_MAX_WAIT_MS) == false)
            {
                SYS_PRINT("I2C_3 Chamber SET_BREATHING_CIRCUIT_CYCLE_COUNT_CMD Write Fail\n");
                rtn =  false;
            }
            break;
            
        case SET_BREATHING_CIRCUIT_TYPE_CMD:
            //data size
            size = 1;
            //byte command code
            buffWrite[0] = command;
            //byte data
            if(gs_breathCircuitTypeSetValue == eTypePediatric)
            {
                buffWrite[1] = 'P';
            }
            else if(gs_breathCircuitTypeSetValue == eTypeAdult)
            {
                buffWrite[1] = 'A';
            }
            else if(gs_breathCircuitTypeSetValue == eTypeDisinfection)
            {
                buffWrite[1] = 'S';
            }
            //byte CRC
            buffWrite[size+1] = crcChamber_Calculate(&buffWrite[0], size + 1);
            if (I2C3_Write(CHAMBER_WRITE_ADDR, 
                  (void*)buffWrite, (size+2) , CHAMBER_COMM_MAX_WAIT_MS) == false)
            {
                SYS_PRINT("I2C_3 Chamber SET_BREATHING_CIRCUIT_TYPE_CMD Write Fail\n");
                rtn =  false;
            }
            break;
            
        default:
            break;
            
    }
    return rtn;      
}

/** @brief Function to read data from Chamber and process it
 *  @param [in]  uint8_t command: process data response of this command
 *  @retval true read data success
 *  @retval false read data failed  
 */
bool Chamber_ReadAndProcessReadData(uint8_t command)
{
    uint8_t buffRead[20] = {'\0'};
    uint8_t size = 0;
    bool rtn = true;
    
    switch (command)
    {
        case GET_CHAMBER_SN_CMD: 
            size = 13;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    uint8_t i;
                    for(i = 0; i < size; i++ )
                    {
                        gs_ChamberSN[i] = buffRead[i];
                    }

                    if(gs_IsUpdateChamberSNToGUIInfo == true)
                    {
                        //update chamber serial number to display at Info screen
                        char serial[16] = {};
                        memcpy(serial, gs_ChamberSN, 13);
                        if(DeviceInformation_SetDeviceInfo(eChamberSerialNoDeviceInfoId, serial) == true){
                            //clear flag to indicate data was updated
                            gs_IsUpdateChamberSNToGUIInfo = false;
                        }
                    }
                }
                else 
                {
                    SYS_PRINT("GET_CHAMBER_SN_CMD Chamber read data CRC Fail\n");
                }            
            }
            break;
            
        case GET_CHAMBER_OUT_TEMP_CMD:
        case GET_CHAMBER_EVT_TEMP_CMD:
        case GET_BREATHING_CIRCUIT_OUT_TEMP_CMD:
            size = 3;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    int32_t temperature = 0;
                    int i;
                    for(i = 0; i < size; i++)
                    {
                        temperature = temperature + (((int32_t)buffRead[i]) << (8*(size-1-i)));
                    }
                    //clear error 
                    s_ChamberError = eDeviceNoError;
                    if(temperature != 0)
                    {
                        
                        switch (command)
                        {
                            case GET_CHAMBER_OUT_TEMP_CMD:
                                gs_ChamberOutletTemperature = (float)temperature / 100.0;
                                break;
                            case GET_CHAMBER_EVT_TEMP_CMD:
                                gs_EVTTemperature = (float)temperature / 100.0;
                                break;
                            case GET_BREATHING_CIRCUIT_OUT_TEMP_CMD:
                                gs_BreathCircuitOutletTemperature = (float)temperature / 100.0;
                                break;
                        }                                
                    }
                }
                else 
                {
                    SYS_PRINT("GET_CHAMBER_TEMP_CMD Chamber read data CRC Fail\n");
                }            
            }
            break;
            
        case GET_CHAMBER_WATER_LEVEL_CMD:
            size = 1;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    
                }
                else 
                {
                    SYS_PRINT("GET_CHAMBER_WATER_LEVEL_CMD Chamber read data CRC Fail\n");
                }            
            }
            break;
            
        case GET_CHAMBER_USED_TIME_CMD:
            size = 4;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    uint32_t usedTime = 0;
                    int i;
                    for(i = 0; i < size; i++)
                    {
                        usedTime = usedTime + (((uint32_t)buffRead[i]) << (8*(size-1-i)));
                    }
                    gs_ChamberUsedTime = usedTime;
                    if(gs_IsUpdateChamberUsageTimeToGUIInfo == true)
                    {
                        char usage[16] = {};
                        sprintf(usage, "%d", gs_ChamberUsedTime);
                        if(DeviceInformation_SetDeviceInfo(eChamberTotalRuntimeDeviceInfoId, usage) == true){
                            //clear flag to indicate data was updated
                            gs_IsUpdateChamberUsageTimeToGUIInfo = false;
                        }
                    }
                }
                else 
                {
                    SYS_PRINT("GET_CHAMBER_USED_TIME_CMD Chamber read data CRC Fail\n");
                }            
            }
            break;
            
        case GET_CHAMBER_FW_VERSION_CMD:
            size = 3;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    gs_ChamberFwVersion[0] = buffRead[0];
                    gs_ChamberFwVersion[1] = buffRead[1];
                    gs_ChamberFwVersion[2] = buffRead[2];
                    if(gs_IsUpdateChamberVerToGUIInfo == true)
                    {
                        char ver[16] = {};
                        sprintf(ver, "%d.%d.%d", gs_ChamberFwVersion[0], gs_ChamberFwVersion[1], gs_ChamberFwVersion[2]);
                        if(DeviceInformation_SetDeviceInfo(eChamberSWVersionDeviceInfoId, ver) == true){
                            //clear flag to indicate data was updated
                            gs_IsUpdateChamberVerToGUIInfo = false;
                        }
                    }
                }
                else 
                {
                    SYS_PRINT("GET_CHAMBER_FW_VERSION_CMD Chamber read data CRC Fail\n");
                }            
            }
            break;
              
        case GET_TANK_WATER_LEVEL_CMD:
            size = 1;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    E_TankWaterLevel waterLevel = 0;
                    int i;
                    for(i = 0; i < size; i++)
                    {
                        waterLevel = waterLevel + (((int32_t)buffRead[i]) << (8*(size-1-i)));
                    }
                    gs_TankWaterlevel = waterLevel;
                }
                else 
                {
                    SYS_PRINT("GET_TANK_WATER_LEVEL_CMD Chamber read data CRC Fail\n");
                }            
            }
            break;
      
      
        case GET_BREATHING_CIRCUIT_MANUF_CODE_CMD:
            size = 2;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    gs_BreathCircuitManufacturerCode[0] = buffRead[0];
                    gs_BreathCircuitManufacturerCode[1] = buffRead[1];
                }
                else 
                {
                    SYS_PRINT("GET_BREATHING_CIRCUIT_MANUF_CODE_CMD Chamber read data CRC Fail\n");
                }            
            }
            break;
               
        case GET_BREATHING_CIRCUIT_SN_CMD: 
            size = 13;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    uint8_t i;
                    for(i = 0; i < size; i++ )
                    {
                        gs_BreathCircuitSN[i] = buffRead[i];
                    }
                    if(gs_IsUpdateBCSNToGUIInfo == true)
                    {
                        //update BC serial number to display at Info screen
                        char serial[16] = {};
                        memcpy(serial, gs_BreathCircuitSN, 13);
                        if(DeviceInformation_SetDeviceInfo(eCircuitSerialNoDeviceInfoId, serial)== true){
                            //clear flag to indicate data was updated
                            gs_IsUpdateBCSNToGUIInfo = false;
                        }
                    }
                }
                else 
                {
                    SYS_PRINT("GET_BREATHING_CIRCUIT_SN_CMD Chamber read data CRC Fail\n");
                }            
            }
            break;
          
        case GET_BREATHING_CIRCUIT_MANUF_DATE_CMD: 
            size = 4;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    gs_BreathCircuitManufactureDate[0] = buffRead[0];
                    gs_BreathCircuitManufactureDate[1] = buffRead[1];
                    gs_BreathCircuitManufactureDate[2] = buffRead[2];
                    gs_BreathCircuitManufactureDate[3] = buffRead[3];
                }
                else 
                {
                    SYS_PRINT("GET_BREATHING_CIRCUIT_MANUF_DATE_CMD Chamber read data CRC Fail\n");
                }            
            }
            break;
            
        case GET_BREATHING_CIRCUIT_FACTORY_CODE_CMD: 
            size = 1;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    gs_BreathCircuitFactoryCode = buffRead[0];
                    
                    if(gs_IsUpdateBCFacCodeTGUIInfor == true)
                    {                       
                        char* str;
                        if(gs_BreathCircuitFactoryCode == '1')
                        {
                            str = "Metran Japan";
                        }
                        else if(gs_BreathCircuitFactoryCode = '2')
                        {
                            str = "Metran VietNam";
                        }
                        else
                        {
                            str = "Unknown";
                        }
                        if(DeviceInformation_SetDeviceInfo(eCircuitFactoryDeviceInfoId, str)== true){
                            //clear flag to indicate data was updated
                            gs_IsUpdateBCFacCodeTGUIInfor = false;
                        }
                    }
                }
                else 
                {
                    SYS_PRINT("GET_BREATHING_CIRCUIT_FACTORY_CODE_CMD Chamber read data CRC Fail\n");
                }            
            }
            break;
          
        case GET_BREATHING_CIRCUIT_START_USED_DAY_CMD:  
            size = 4;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    gs_BreathCircuitStartUsedDate[0] = buffRead[0];
                    gs_BreathCircuitStartUsedDate[1] = buffRead[1];
                    gs_BreathCircuitStartUsedDate[2] = buffRead[2];
                    gs_BreathCircuitStartUsedDate[3] = buffRead[3];
                    
                    if(gs_IsUpdateBCStartDateToGUIInfo == true)
                    {
                        uint32_t date = (gs_BreathCircuitStartUsedDate[0] << 24) + (gs_BreathCircuitStartUsedDate[1] << 16)
                            + (gs_BreathCircuitStartUsedDate[2] << 8)+ gs_BreathCircuitStartUsedDate[3];
                        //update BC start of use to display at Info screen
                        char str[16] = {};
                        sprintf(str, "%d/%02d/%02d", date/10000, (date/100)%100, date%100);
                        if(DeviceInformation_SetDeviceInfo(eCircuitStartOfUseNoDeviceInfoId, str)== true){
                            //clear flag to indicate data was updated
                            gs_IsUpdateBCStartDateToGUIInfo = false;
                        }
                    }
                }
                else 
                {
                    SYS_PRINT("GET_BREATHING_CIRCUIT_START_USED_DAY_CMD Chamber read data CRC Fail\n");
                }            
            }
            break;
        case GET_BREATHING_CIRCUIT_CYCLE_COUNT_CMD:
            size = 2;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    gs_BreathCircuitCnt[0] = buffRead[0];
                    gs_BreathCircuitCnt[1] = buffRead[1];
                }
                else 
                {
                    SYS_PRINT("GET_BREATHING_CIRCUIT_CYCLE_COUNT_CMD Chamber read data CRC Fail\n");
                }            
            }
            break;
        case GET_BREATHING_CIRCUIT_USED_TIME_CMD:
            size = 2;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    memcpy(gs_BreathCircuitUsedTime, buffRead, size);
                    
                    if(gs_IsUpdateBCUsageTimeTGUIInfor == true)
                    {
                        uint16_t time = (gs_BreathCircuitUsedTime[0] << 8) + gs_BreathCircuitUsedTime[1];
                        char str[16]= {};
                        sprintf(str, "%d", time);
                        if(DeviceInformation_SetDeviceInfo(eCircuitUsageTimeDeviceInfoId, str)== true){
                            //clear flag to indicate data was updated
                            gs_IsUpdateBCUsageTimeTGUIInfor = false;
                        }
                    }
                }
                else 
                {
                    SYS_PRINT("GET_BREATHING_CIRCUIT_USED_TIME_CMD Chamber read data CRC Fail\n");
                }            
            }
            break;
          
        case GET_BREATHING_CIRCUIT_TYPE_CMD:
            size = 1;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    char* str;
                    if(buffRead[0] == 'P'){
                        gs_BreathCircuitType = eTypePediatric;
                        str = "Pediatric";
                    }
                    else if (buffRead[0] == 'A'){
                        gs_BreathCircuitType = eTypeAdult;
                        str = "Adult";
                    }
                    else if (buffRead[0] == 'S'){
                        gs_BreathCircuitType = eTypeDisinfection;
                        str = "Disinfection";
                    }
                    else{
                        gs_BreathCircuitType = eTypeOther;
                        str = "Unknown";
                    }
                    if(gs_IsUpdateBCTypeTGUIInfor == true)
                    {
                        if(DeviceInformation_SetDeviceInfo(eCircuitTypeDeviceInfoId, str)== true){
                            //clear flag to indicate data was updated
                            gs_IsUpdateBCTypeTGUIInfor = false;
                        }
                    }
                    //SYS_PRINT("GET_BREATHING_CIRCUIT_TYPE_CMD type %d\n", buffRead[0]);
                }
                else 
                {                    
                    SYS_PRINT("GET_BREATHING_CIRCUIT_TYPE_CMD Chamber read data CRC Fail\n");
                }            
            }
            break;
          
        case GET_BREATHING_CIRCUIT_CONNECTION_CMD:
            size = 1;
            if(I2C3_Read(CHAMBER_READ_ADDR, 
                (void*)buffRead, (size + 1), CHAMBER_COMM_MAX_WAIT_MS) == false)
            {            
                SYS_PRINT("Chamber I2C_3 Read Fail\n");
                rtn = false;
            }
            else
            {
                //check CRC
                if (crcChamber_Check(buffRead, size, buffRead[size])) 
                {   
                    gs_BreathCircuitConnectStt = buffRead[0];
                }
                else 
                {
                    SYS_PRINT("GET_BREATHING_CIRCUIT_CONNECTION_CMD Chamber read data CRC Fail\n");
                }            
            }         
            break;
          
        default:
            break;
            
    }
    return rtn;
}

static void Chamber_CheckChamberConnection(void)
{
    static uint32_t s_cnt = 0;
    if(gs_ChamberConnection == eChamberDisconnected)
    {
        if(s_cnt >= (200/10))
        {
            s_cnt = 0;
            //check connection status every 200ms when chamber disconnected

            uint8_t command = 0x00;
            if (I2C3_Write(CHAMBER_WRITE_ADDR, (void*)&command, 1, 5) == true) 
            {
                gs_ChamberConnection = eChamberConnected;
            }
            else
            {
                //SYS_PRINT("Chamber not connect\n");
            }
        }
        else{
          s_cnt++;
        }
    }
}
    
//this function should be called at least every 20ms
void Chamber_Run()
{
    static uint16_t command = 0;
    static E_ChamberRequestState state = eChamberWriteRqst;
    static uint16_t s_ErrorCounter = 0;
    
    Chamber_CheckChamberConnection();
    if(gs_ChamberConnection == eChamberConnected)
    {
        switch (state)
        {
            case eChamberWriteRqst:
                command = Chamber_GetNextCommand();
                if(Chamber_WriteCommand(command) == true)
                {
                    //check command if "get data" command, change state to read data
                    if(command < SET_CHAMBER_SN_CMD)
                    {
                        state = eChamberReadRqst;
                    }
                    s_ErrorCounter = 0;
                }
                else{
                    state = eChamberErrorState;
                }
                break;

            case eChamberReadRqst:
                if (Chamber_ReadAndProcessReadData(command) == true){
                    state = eChamberWriteRqst;
                    s_ErrorCounter = 0;
                }
                else{
                   state = eChamberErrorState;
                }
                break;

            case eChamberErrorState:
                s_ErrorCounter++;
                if(s_ErrorCounter >= (3000/20))
                {
                    s_ErrorCounter = 0;
                    gs_ChamberConnection = eChamberDisconnected;
                }
                else
                {
                    Chamber_ResetChamber();
                    state = eChamberWriteRqst;
                }
                break;

            default:
                break;
        }
    }
    else
    {
        gs_IsUpdateChamberSNToGUIInfo = true;
        gs_IsUpdateChamberVerToGUIInfo = true;
        gs_IsUpdateChamberUsageTimeToGUIInfo = true;
    }
    if(chamber_GetBreathingCircuitConnection() == false)
    {
        gs_IsUpdateBCSNToGUIInfo = true;
        gs_IsUpdateBCStartDateToGUIInfo = true;
        gs_IsUpdateBCTypeTGUIInfor = true;
        gs_IsUpdateBCUsageTimeTGUIInfor = true;
        gs_IsUpdateBCFacCodeTGUIInfor = true;
    }
}


/** @brief reset
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void Chamber_Reset() {
    //reset variables
    s_ChamberError = eDeviceNoError;
}


/** @brief Function to Get Chamber SN
 *  @param [out]  uint8_t *sn : pointer point to storage reading value
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_GetChamberSN(uint8_t *sn)
{
    memcpy(sn, gs_ChamberSN, 13);
    return true;
}

/** @brief Function to Get Chamber Out temp
 *  @param [out]  None
 *  @retval float chamber outlet temperature
 */
float Chamber_GetChamberOutTemp(void)
{
    return gs_ChamberOutletTemperature;
}


/** @brief Function to Get EVT temp
 *  @param [out]  None
 *  @retval float EVT temperature
 */
float Chamber_GetEVTTemp(void)
{
    return gs_EVTTemperature;
}



/** @brief Function to Get usage time of chamber
 *  @param [out] None
 *  @retval uint32_t chamber usage time
 */
uint32_t Chamber_GetChamberUsedTime(void)
{
    return  gs_ChamberUsedTime;
}

/** @brief Function to Get Chamber FW version
 *  @param [out]  float *rawFW : pointer point to storage reading value
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_GetChamberFWVer(uint8_t *fwVer)
{
    fwVer[0] = gs_ChamberFwVersion[0]; 
    fwVer[1] = gs_ChamberFwVersion[1]; 
    fwVer[2] = gs_ChamberFwVersion[2];
    return true;
}


/** @brief Function to Get Tank water level
 *  @param [out] None
 *  @retval E_TankWaterLevel
 */
E_TankWaterLevel Chamber_GetTankWaterLevel(void)
{
    return gs_TankWaterlevel;
}

/** @brief Function to Get Breathing circuit manufacture code
 *  @param [out] None
 *  @retval uint8_t manufacture code
 */
uint8_t Chamber_GetBreathingCircuitManufactureCode()
{
    return gs_BreathCircuitManufacturerCode[1];
}

/** @brief Function to Get Breathing circuit serial number
 *  @param [out]  float *sn : pointer point to storage reading value
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_GetBreathingCircuitSerialNumber(uint8_t* sn)
{
    memcpy(sn, gs_BreathCircuitSN, 13);
    return true;
}

/** @brief Function to Get Breathing Circuit manufacture date
 *  @param [out]  None
 *  @retval uint32_t manufacture date(format:YYYYMMDD,Ex: Date(Hex) : 0x143A52D-> Date(Dec) : 21210413-> Date(Time): 2021/4/13)
 */
//    //day format:
//    //Example: Date(Hex) : 0x143A52D
//    //-> Date(Dec) : 21210413
//    //-> Date(Time): 2021/4/13
uint32_t Chamber_GetBreathingCircuitManufactureDate()
{
    uint32_t date = (gs_BreathCircuitManufactureDate[0] << 24) + (gs_BreathCircuitManufactureDate[1] << 16)
                    + (gs_BreathCircuitManufactureDate[2] << 8)+ gs_BreathCircuitManufactureDate[3];
    return date;
}

/** @brief Function to Get Breathing Circuit factory code
 *  @param [out] None
 *  @retval uint8_t Factory code : 01 (Metran JP),  02 (Vitec VN)
 */
uint8_t Chamber_GetBreathingCircuitFactoryCode(void)
{
    return gs_BreathCircuitFactoryCode;
}

/** @brief Function to Get Breathing Circuit start used date
 *  @param [out] None
 *  @retval uint32_t start use date(format:YYYYMMDD,Ex: Date(Hex) : 0x143A52D-> Date(Dec) : 21210413-> Date(Time): 2021/4/13)
 */
//    //day format:
//    //Example: Date(Hex) : 0x13432ED
//    //-> Date(Dec) : 20210413
//    //-> Date(Time): 2021/4/13

uint32_t Chamber_GetBreathingCircuitStartUsedDay(void)
{  
    uint32_t date = (gs_BreathCircuitStartUsedDate[0] << 24) + (gs_BreathCircuitStartUsedDate[1] << 16)
                    + (gs_BreathCircuitStartUsedDate[2] << 8)+ gs_BreathCircuitStartUsedDate[3];
    return date;
}


/** @brief Function to Get Breathing Circuit cycle count
 *  @param [out] None
 *  @retval uint16_t cycle count
 */
uint16_t Chamber_GetBreathingCircuitCycleCount(void)
{
    uint16_t cnt = (gs_BreathCircuitCnt[0] << 8) + gs_BreathCircuitCnt[1];
    return cnt;
}

/** @brief Function to Get Breathing Circuit used time (in hour)
 *  @param [out] None
 *  @retval uint16_t usage time 
 */
uint16_t Chamber_GetBreathingCircuitUsedTime(void)
{
    uint16_t time = (gs_BreathCircuitUsedTime[0] << 8) + gs_BreathCircuitUsedTime[1];
    return time;
}

/** @brief Function to Get Breathing Circuit outlet temperature
 *  @param [out] None
 *  @retval float breathing circuit outlet temperature
 */
float Chamber_GetBreathingCircuitTemperature(void)
{
    return gs_BreathCircuitOutletTemperature;
}

/** @brief Function to Get breathing circuit type
 *  @param [out] None
 *  @retval E_BreathingCircuitType: Type = 'P' :For Kid breathing circuit, A' :For Adult breathing circuit, 'S' :For Sterilization circuit
 */
E_BreathingCircuitType Chamber_GetBreathingCircuitType()
{
    return gs_BreathCircuitType; 
}

/** @brief Function to get connection status of breathing circuit
 *  @param [in]  None 
 *  @param [out]  None
 *  @retval true if connect, false if disconnect
 */
bool chamber_GetBreathingCircuitConnection()
{
    if (gs_BreathCircuitConnectStt != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/** @brief Function to get connection status of chamber
 *  @param [in]  None 
 *  @param [out]  None
 *  @retval E_ConnectState connection status of chamber
 */
E_ChamberConnectState chamber_GetChamberConnection()
{
    return gs_ChamberConnection;
}



/** @brief Function to Set Chamber SN
 *  @param [in]  uint8_t sn : pointer to array contains serial data
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetChamberSN(uint8_t *sn)
{
    uint8_t command = SET_CHAMBER_SN_CMD;

    memcpy(gs_chamberSerialSetValue, sn, 13);

    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;   
}

/** @brief Function to Set Water pump control Frequency
 *  @param [in]  float value : the value will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetPumpFreq(float setFreq)
{    
    uint8_t command = SET_WATER_PUMP_FREQ_CMD;
    gs_pumpFreqSetValue = setFreq;

    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;
}

/** @brief Function to Reset used time of chamber
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true if success
 *  @retval false if failed  
 */
bool Chamber_ResetChamberUsedTime(void)
{   
    uint8_t command = RESET_CHAMBER_USED_TIME_CMD;
    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;
}

/** @brief Function to Reset all chamber data
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true if success
 *  @retval false if failed  
 */
bool Chamber_ResetChamberAll(void)
{  
    uint8_t command = RESET_CHAMBER_ALL_CMD;
    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;
}


/** @brief Function to Set Breath circuit manufacture code
 *  @param [in]  uint32_t code : the value will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitManufactureCode(uint32_t code)
{
    uint8_t command = SET_BREATHING_CIRCUIT_MANUF_CODE_CMD;
    gs_breathCircuitMafCodeSetValue = code;

    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;
}


/** @brief Function to Set Breath circuit serial number
 *  @param [in]  uint8_t sn : pointer to array contains serial data
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitSerialNumber(uint8_t* sn)
{
    uint8_t command = SET_BREATHING_CIRCUIT_SN_CMD;

    memcpy(gs_breathCircuitSerialSetValue, sn, 13);

    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;
}

/** @brief Function to Set Breath circuit factory code
 *  @param [in]  uint8_t code : the value will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitFactoryCode(uint8_t code)
{
    uint8_t command = SET_BREATHING_CIRCUIT_FACTORY_CODE_CMD;
    gs_breathCircuitFacCodeSetValue = code;

    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;
}


/** @brief Function to Set Breath circuit start used day
 *  @param [in]  uint32_t date : the value will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitStartUsedDay(uint32_t date)
{
    uint8_t command = SET_BREATHING_CIRCUIT_START_USED_DAY_CMD;
    gs_breathCircuitStartUseDateSetValue = date;

    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;
}

/** @brief Function to Reset breathing circuit cycle count
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_ResetBreathingCircuitCycleCount(void)
{   
    uint8_t command = RESET_BREATHING_CIRCUIT_CYCLE_COUNT_CMD;
    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;
}

/** @brief Function to Reset breathing circuit used time
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_ResetBreathingCircuitUsedTime(void)
{     
    uint8_t command = RESET_BREATHING_CIRCUIT_USED_TIME_CMD;
    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;
}

/** @brief Function to Reset all breathing circuit data
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_ResetBreathingCircuitAll(void)
{   
    uint8_t command = RESET_BREATHING_CIRCUIT_ALL_CMD;
    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;
}

/** @brief Function to set Breath circuit manufacture date
 *  @param [in]  uint32_t date : the value will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitManufactureDate(uint32_t date)
{
    uint8_t command = SET_BREATHING_CIRCUIT_MANUF_DATE_CMD;
    gs_breathCircuitManufactureDateSetValue = date;

    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;
}

/** @brief Function to set Breath circuit use time
 *  @param [in]  uint16_t time : the time will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitUseTime(uint16_t time)
{
    uint8_t command = SET_BREATHING_CIRCUIT_USED_TIME_CMD;
    gs_breathCircuitUseTimeSetValue = time;

    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;
}

/** @brief Function to set Breath circuit type
 *  @param [in]  E_BreathingCircuitType type : type will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitType(E_BreathingCircuitType type)
{
    uint8_t command = SET_BREATHING_CIRCUIT_TYPE_CMD;
    gs_breathCircuitTypeSetValue = type;

    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;
}

/** @brief Function to set Breath circuit cycle count
 *  @param [in]  uint16_t count : count will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitCycleCount(uint16_t count)
{
    uint8_t command = SET_BREATHING_CIRCUIT_CYCLE_COUNT_CMD;
    gs_breathCircuitCycleCountSetValue = count;

    if (xQueueSendToBack(gs_ChamberCommandQueue, &command, QUEUE_CHAMER_COMMAND_WAITING) != pdPASS)
    {
        xQueueReset(gs_ChamberCommandQueue);
        return false;
    }
    return true;
}

/** @brief Function to copy transmit line in hex file to transmit buffer
 *  @param [in] uint8_t* data array data was read from hex file
 *  @param [out] None
 *  @return uint8_t Number of handled bytes
 */
static uint8_t Chamber_prepareFlashData(uint8_t* data)
{
    uint8_t transferBytes = 0;  // the number of data will be transfered to Chamber
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
        uint8_t highByte = Chamber_Ascii2Hex(*(data + 1 + cnt*2));
        uint8_t lowByte = Chamber_Ascii2Hex(*(data + 1 + cnt*2 + 1));
        handledData[cnt] &= 0x00;
        handledData[cnt] = (highByte << 4) | lowByte;
//        SYS_PRINT("\t%x", handledData[cnt]);
    }
    
    // Transfer data to other device
    Chamber_PrepareCommand(FLASH_DATA_COMMAND, handledData, transferBytes);
    SYS_PRINT("\nTransfer byte: %d", transferBytes);
    return handleBytes;
}

 /** @brief Function to initialize before upgrade firmware
 *  @param [in]  uint8_t* data : The data was read from hex file 
 *  @param [out]  None
 *  @retval true initialize success
 *  @retval false initialize failed  
 */
bool Chamber_InitFirmware(void)
{
    byteIndex = 0;
    sendBytes = 0;
    
    upgradeFirmwareState = eJumpToBld;
    
    memset((void*) txBuf, 0, 27);
    memset((void*) rxBuf, 0, 27);
    
    return true;
}

/** @brief Function to handle command response
 *  @param [in]  uint8_t* data : command length need to be sent 
 *  @param [out]  None
 *  @retval eProcessFail process command fail
 *  @retval eProcessSuccess process command success
 */
//static uint8_t Chamber_HandleCommandResponse(uint8_t *data)
//{
//
//}

 /** @brief Function to process command such as: PING_COMMAND, ERASE_COMMAND, FLASH_DATA_COMAND
 *  @param [in]  uint8_t* data : command data need to be sent 
 *  @param [in]  uint8_t transferBytes: command data length need to be sent
 *  @param [out]  None
 *  @retval eProcessFail process command fail
 *  @retval eProcessSuccess process command success
 */
static E_processCommandState Chamber_ProcessCommand(uint8_t* data, uint8_t transferBytes)
{
#define MASTER_DELAY        10
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
////                    txBuf[2] = Chamber_CalCulateCRC8(txBuf, 2);
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
                if (I2C3_Write(CHAMBER_WRITE_ADDR, (void *)data, transferBytes, CHAMBER_COMM_MAX_WAIT_MS) == false)
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
                
                if (I2C3_Read(CHAMBER_READ_ADDR, (void *)rxBuf, 3, CHAMBER_COMM_MAX_WAIT_MS) == false)
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
                    if (Chamber_CalCulateCRC8(rxBuf, 2) != rxBuf[2])
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
                                SYS_PRINT("Chamber Unknown Error");
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
static void Chamber_PrepareCommand(uint8_t command, uint8_t* data, uint8_t size)
{
    txBuf[0] = (uint8_t)START_PACKET;
    txBuf[1] = command;
    memcpy(&txBuf[2], data, size);
    txBuf[2 + size] = Chamber_CalCulateCRC8(txBuf, 2 + size);
}

 /** @brief Function to upgrade firmware for Chamber
 *  @param [in]  uint8_t* data : The data was read from hex file
 *  @param [in]  uin32_t datalen: The length of data that was read from hex file 
 *  @param [out]  None
 *  @retval true update data success
 *  @retval false update data failed  
 */
bool Chamber_UpgradeFirmware(uint8_t* data, uint32_t datalen)
{
E_processCommandState s_processResult;
static uint8_t s_bootResponseCounter = 0;
switch(upgradeFirmwareState)
{
    case eJumpToBld:
    {
        SYS_PRINT("\n\nRequest to bld.");
        Chamber_PrepareCommand(PING_COMMAND, NULL, 0);
        s_processResult = Chamber_ProcessCommand(txBuf, 3);
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
        if (I2C3_Read(CHAMBER_READ_ADDR, (void *)rxBuf, 3, CHAMBER_COMM_MAX_WAIT_MS) == false)
        {
            SYS_PRINT("\nI2C read command fail!");
        }
        else
        {
            SYS_PRINT("\n %x, %x, %x", rxBuf[0], rxBuf[1],rxBuf[2]);
            if (Chamber_CalCulateCRC8(rxBuf, 2) == rxBuf[2]);
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
        Chamber_PrepareCommand(ERASE_COMMAND, NULL, 0);
        s_processResult = Chamber_ProcessCommand(txBuf, 3);
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
            sendBytes = Chamber_prepareFlashData(data + byteIndex);
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
        s_processResult = Chamber_ProcessCommand(txBuf, 2 + dataSendLen + 1);
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

/**@brief Function to calculate CRC8
 * @param [in] const void* buff: data buffer needed to calculate CRC8
 * @param [in] size_t: data size need to calculate CRC8
 * @param [out] None
 * @return uint8_t value of calculated CRC8
 */
static uint8_t Chamber_CalCulateCRC8( const void *buff, size_t size )
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

/** @brief Function to convert ASCII from char to decimal value
 *  @param [in] uint8_t c: character need to be convert
 *  @param [out] None
 *  @return uint8_t value of converted
 *  @retval 0 if char not in range '0' -> '9' or 'A' -> 'F' or 'a' -> 'f' 
 */
static uint8_t Chamber_Ascii2Hex(uint8_t c)
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

/** @brief Function to get status update (success or failed)
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true update data success
 *  @retval false update data failed  
 */
bool Chamber_IsUpdateSuccess()
{
    return s_IsUpdateSuccess;
}
/* *****************************************************************************
 End of File
 */

