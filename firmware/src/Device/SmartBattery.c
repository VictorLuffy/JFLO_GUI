/* ************************************************************************** */
/** @file [SmartBattery.c]
 *  @brief {support interface to communicate with Smart Battery to get 
 * information, report error if occur during communication}
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

#include "FreeRTOS.h"
#include "semphr.h"

#include "system_config.h"
#include "system_definitions.h"

#include "ApplicationDefinition.h"
#include "I2C_2.h"
#include "SmartBattery.h"
#include "Charger.h"
#include "UART_1.h"

//#include "AlarmInterface.h"
//#include "ApplicationDefinition.h"


typedef enum
{
    //Remaining Capacity Alarm Threshold - mAh - read/write
    eRemainingCapacityAlarm = 0x01,
            
    //Remaining Time Alarm Threshold - minutes - read/write
    eRemainingTimeAlarm = 0x02,
            
    //Battery Operational Modes - bit flags - read/write
    eBatteryMode = 0x03,     
            
    //Returns the pack?s internal temperature - 0.1°K - read only   
    eTemperature = 0x08,
            
    //Returns the battery?s voltage (measured at the cell stack) - mV - read only
    eVoltage = 0x09,
            
    //Returns the current being supplied (or accepted) through thebattery?s terminals - mA - read only        
    eCurrent = 0x0a,
            
    //Returns the predicted remaining battery capacity expressed as a percentage of Full Charge Capacity - percent - read only
    eRemainingPercentageOfFullCharge = 0x0d,
            
    //Returns the predicted remaining battery capacity expressed as a percentage of Design Capacity - percent - read only
    eRemainingPercentageOfDesign = 0x0e,
            
    //Returns the predicted remaining battery capacity  - mAh - read only
    eRemainingCapacity = 0x0f,
            
    //Returns the predicted battery capacity when fully charged -mAh - read only
    eCapacityOfFullCharge = 0x10,
            
    //Returns the predicted remaining battery life at the present rate of discharge - minutes - read only          
    eRunTimeToEmpty = 0x11,

    //Returns the rolling average of the predicted remaining battery life - minutes - read only      
    eAverageTimeToEmpty = 0x12,            

    //Returns the rolling average of the predicted remaining time until the battery reaches full charge - minutes - read only      
    eAverageTimeToFull = 0x13,                

    //Returns the battery?s desired charging rate - mA - read only      
    eChargeCurrent = 0x14, 

    //Returns the battery?s desired charging voltage - mV - read only      
    eChargeVotage = 0x15, 

    //Returns the battery?s status word - Bit flags - read only
    eBatteryStatus = 0x16,
            
    //Returns the theoretical capacity of the new battery - mAh - read only
    eDesignCapacity = 0x18, 

    //Returns the theoretical voltage of a new battery - mV - read only
    eDesignVoltage = 0x19,             
            
} E_BATTERY_COMMAND;


//typedef enum
//{     
//    /* * * * * * Alarm Bits * * * * */
//    OVER_CHARGED_ALARM = 0x8000,
//    TERMINATE_CHARGE_ALARM = 0x4000,
//    OVER_TEMP_ALARM = 0x1000,
//    TERMINATE_DISCHARGE_ALARM = 0x0800,
//    REMAINING_CAPACITY_ALARM = 0x0200,
//    REMAINING_TIME_ALARM = 0x0100,
//
//    /* * * * * * Status Bits * * * * */       
//    INITIALIZED = 0x0080,
//    DISCHARGING = 0x0040,
//    FULLY_CHARGED = 0x0020,
//    FULLY_DISCHARGED = 0x0010,
//
//    /* * * * * * Error Code * * * * */       
//          
//} E_BATTERY_STATUS;

typedef enum
{     
    /* * * * * * Error Code * * * * */
    //The Smart Battery processed the function code without detecting any errors
    eOK = 0x0000,
    //The Smart Battery is unable to process the function code at this time
    eBusy = 0x0001,
    //The Smart Battery detected an attempt to read or write to a function code reserved by this version of the specification. The Smart Battery detected an attempt to access an unsupported optional manufacturer function code.
    eReservedCommand = 0x0002,
    //The Smart Battery does not support this function code which is defined in this version of the specification
    eUnsupportedCommand = 0x0003,
    //The Smart Battery detected an attempt to write to a read only function code.
    eAccessDenied = 0x0004,
    //The Smart Battery detected a data overflow or under flow.
    eOverflowOrUnderflow = 0x005,
    //The Smart Battery detected an attempt to write to a function code with an incorrect size data block
    eBadSize = 0x0006,
    //The Smart Battery detected an unidentifiable error.
    eUnknownError = 0x0007,
            
} E_BatteryErrStatus;



typedef enum
{
     /* Capacity Mode || Bit Field [15] */
    // 0: Report in mA or mAh <default>
    CAPACITY_MODE_MAH           = (0 << 15),           
    // 1: Report in 10mW or 10mWh      
    CAPACITY_MODE_10MW          = (1 << 15),

            
    /* Charger Mode || Bit Field [14] */
    // 0: Enable ChargingVoltage and ChargingCurrent broadcasts to Smart Battery Charger <default>
    ENABLE_BROADCASTS_CHARGE    = (0 << 14),           
    // 1: Disable broadcasts of ChargingVoltage and ChargingCurrent to Smart Battery Charger      
    DISABLE_BROADCASTS_CHARGE   = (1 << 14),

            
    /* Alarm Mode || Bit Field [13] */
    // 0: Enable Alarm Warning broadcasts to Host and Smart Battery Charger <default>
    ENABLE_BROADCASTS_ALARM     = (0 << 13),           
    // 1: Disable Alarm Warning broadcast to Host and Smart Battery Charger      
    DISABLE_BROADCASTS_ALARM    = (1 << 13),

            
    /* PRIMARY_BATTERY || Bit Field [9] */
    // 0: Battery operating in its secondary role <default>
    SECONDARY                   = (0 << 9),           
    // 1: Battery operating in its primary role      
    PRIMARY                     = (1 << 9),

            
    /* Charge control || Bit Field [8] */
    // 0: Internal Charge Control Disabled <default>
    DISABLE_CHARGE_CONTROL      = (0 << 8),           
    // 1: Internal Charge Control Enabled     
    ENABLE_CHARGE_CONTROL       = (1 << 8),  
                                                        
} E_BatteryOption;


/** @brief define handle state machine that used to set/get mode/data for battery*/
typedef enum
{  
    eSetBatteryMode,
    eReadStatus, 
    eReadRemainPercent,
    eReadRemainCapacity,
    eReadTemperature,
    eReadVoltage,
    eReadChargeCurrent,
    eReadChargeVoltage,
    eReadCurrent,
    eReadRunTimeToEmpty,
    eReadFullChargeCapacity,
    eReadDesignCapacity,
   
}E_BATTERY_HANDLE_STATE;

/** @brief structure for storing battery data*/
typedef struct
{    
    E_BATTERY_CONNECT_STAT BattConnectState;
    E_BATTERY_HANDLE_STATE handlestate;
    uint16_t  remainPercent;
    uint16_t  remainCapacity;
    uint16_t  temperature;
    uint16_t  voltage;
    int16_t   current;
    uint16_t  runTimeToEmpty;
    uint16_t  status;
    uint16_t  chargeCurrent;
    uint16_t  chargeVoltage;
    uint16_t  fullChargeCapacity;
    uint16_t  designCapacity;
}ST_BATTERY_DATA;





#define POLYNOMIAL_CRC8    (0x07)      // 1 0000 0111 x8 + x2 + x1 + x0

/** @brief I2C slave address of smart battery*/
#define SMART_BATTERY_BASE_ADDR                 (0x16 >> 1)//0x0B

/** @brief I2C address while writing data to charger
 * write address = base address << 1 + write bit (0))
 */
#define SMART_BATTERY_WRITE_ADDR                (SMART_BATTERY_BASE_ADDR << 1)

/** @brief I2C address while reading data from charger
 * read address = base address << 1 + read bit(1))
 */
#define SMART_BATTERY_READ_ADDR                 ((SMART_BATTERY_BASE_ADDR << 1) + 1)

/** @brief max time wait for charger communication (in ms) via I2C*/
#define SMART_BATTERY_COMM_MAX_WAIT_MS          (40 / portTICK_PERIOD_MS)

/** @brief max time wait for MUTEX to access protection resource */
#define SMART_BATTERY_MUTEX_MAX_WAIT_MS         (2 / portTICK_PERIOD_MS)       

/** @brief Device Task delay time*/
#define DEVICE_TASK_PERIODIC_MS                 (10)//10 ms
/** @brief Period battery handle*/
#define SMART_BATTERY_TASK_PERIODIC_MS          (200/DEVICE_TASK_PERIODIC_MS)

#define INPUT_CURRENT_LIMIT                     0x1180  //mAh

/** @brief communication error times in 3 seconds continuously*/
#define SMART_BATTERY_COMM_ERR_COUNT          (3000/SMART_BATTERY_TASK_PERIODIC_MS)


/** @brief store data of internal battery*/
static ST_BATTERY_DATA g_InternalBatteryData;


/** @brief store connection state of DC power*/
static E_DC_CONNECT_STAT  g_DCConnectState;


/** @brief store number of time error communication with battery*/
static uint8_t  gs_errorTimeCount = 0;

/** @brief store error state of the communication with battery*/
static E_DeviceErrorState gs_communicationErr = eDeviceNoError;



/** @brief create CRC8 
 *  @param [in]  const void *buff   
 *  @param [out]  None
 *  @return uint8_t
 *  @retval crc8: CRC of array
 */
static uint8_t SmartBattery_GetCRC8( const void *buff, size_t size )
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



/** @brief Function to read data from smart battery
 *  @param [in]  uint8_t command : command code   
 *  @param [out]  uint16_t *readValue : pointer point to storage reading value
 *  @retval true read data success
 *  @retval false read data failed  
 */
 bool smartBattery_ReadData(uint8_t command, uint16_t *readValue)
{
    uint8_t buffWrite[3] = {'\0'};
    buffWrite[0] = SMART_BATTERY_WRITE_ADDR;
    buffWrite[1] = command;
    buffWrite[2] = SmartBattery_GetCRC8(&buffWrite[0], 2);
     
    uint8_t buffRead[4] = {'\0'};
    buffRead[0] = SMART_BATTERY_READ_ADDR;
    //write a I2C buffer containing command byte and PEC (crc8) to Smart battery 
    if (I2C2_Write(SMART_BATTERY_WRITE_ADDR, 
              (void*)(&buffWrite[1]), sizeof(buffWrite) - 1, 
              SMART_BATTERY_COMM_MAX_WAIT_MS) == false)
    {
        SYS_PRINT("\nsmartBattery_ReadData I2C_2 Write to read Fail\n");
        gs_errorTimeCount++;       
        if(gs_errorTimeCount >= SMART_BATTERY_COMM_ERR_COUNT)
        {
            gs_communicationErr = eDeviceErrorDetected;
        }
        return false;
    }
    else
    {         
        if(I2C2_Read(SMART_BATTERY_READ_ADDR, 
                (void*)&buffRead[1], sizeof(buffRead)-1, 
                SMART_BATTERY_COMM_MAX_WAIT_MS) == false)
        {            
            SYS_PRINT("I2C_2 Read Fail\n");
            gs_errorTimeCount++;
            if(gs_errorTimeCount >= SMART_BATTERY_COMM_ERR_COUNT)
            {
                gs_communicationErr = eDeviceErrorDetected;
            }
            return false;
        }
        else
        {
            gs_errorTimeCount = 0;//SYS_PRINT("I2C_2 Read OK cmmID%d\n", command);
            //calculate crc of the data read from smart battery
            uint8_t crc = SmartBattery_GetCRC8(buffRead, sizeof(buffRead)-1);
            //compare crc
            if(crc == buffRead[3])
            {
                *readValue = buffRead[1] + buffRead[2]*256;
                return true;
            }
            else
                return false;
        }
    }
   
}



/** @brief Function to write data to smart battery 
 *  @param [in]  uint8_t command :this is command code   
 *  @param [out]  uint16_t value : the value will be written
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool smartBattery_WriteData(uint8_t command, uint16_t value)
{
    uint8_t buffWrite[5];
    //byte address 
    buffWrite[0] = SMART_BATTERY_WRITE_ADDR;
    //byte command
    buffWrite[1] = command;
    //Low Byte
    buffWrite[2] = value & 0xFF;
    //High Byte
    buffWrite[3] = (value >> 8) & 0xFF;
    //crc Byte
    buffWrite[4] = SmartBattery_GetCRC8(buffWrite, 4);
    
    if (I2C2_Write(SMART_BATTERY_WRITE_ADDR, 
          (void*)&buffWrite[1], sizeof(buffWrite) -1, SMART_BATTERY_COMM_MAX_WAIT_MS) == false)
    {
        SYS_PRINT("\nsmartBattery_ReadData I2C_2 Write Fail\n");
        gs_errorTimeCount++;
        if(gs_errorTimeCount >= SMART_BATTERY_COMM_ERR_COUNT)
        {
            gs_communicationErr = eDeviceErrorDetected;
        }
        return false;
    }
    else
    {
        //SYS_PRINT("\nsmartBattery_ReadData I2C_2 Write OK\n");
        gs_errorTimeCount = 0;
        return true;
    }
     
}


/** @brief Get connection state of battery 
*  @param [in]  E_BatteryDevice device: the battery need to get (internal or cradle battery)  
*  @param [out]  None
*  @return E_BATTERY_CONNECT_STAT
*  @retval the value indicated the connection state 
*/
E_BATTERY_CONNECT_STAT smartBattery_GetBatteryConnectionState()
{
    E_BATTERY_CONNECT_STAT connnectStat = 0;
    connnectStat = g_InternalBatteryData.BattConnectState;
    return connnectStat;
}

/** @brief Get connection state of DC power 
*  @param [in]  None  
*  @param [out]  None
*  @return E_DC_CONNECT_STAT
*  @retval the value indicated the connection state 
*/
E_DC_CONNECT_STAT smartBattery_GetDCConnectionState()
{
    return g_DCConnectState;
}
    
    

/** @brief Get remaining  capacity (percentage) of battery 
 *  @param [in]  None
 *  @param [out]  None
 *  @return uint16_t
 *  @retval the value indicated the remaining percentage of battery
 */
uint16_t smartBattery_GetRemainingPercentage()
{
    uint16_t remainPercent = 0;
    remainPercent = g_InternalBatteryData.remainPercent;
    return remainPercent;
}

/** @brief Get remaining capacity (mAh) of battery 
 *  @param [in]  None
 *  @param [out]  None
 *  @return uint16_t
 *  @retval remainCapacity: remaining capacity of battery (mAh)
 */
uint16_t smartBattery_GetRemainingCapacity()
{
    uint16_t remainCapacity = 0;
    remainCapacity = g_InternalBatteryData.remainCapacity;
    return remainCapacity;  
}

/** @brief Get present voltage of battery 
*  @param [in]  None 
*  @param [out]  None
*  @return uint16_t
*  @retval voltage: present voltage of battery (mV)
*/
uint16_t smartBattery_GetVoltage()
{
    uint16_t voltage = 0;
    voltage = g_InternalBatteryData.voltage;
    return voltage;  
}

/** @brief Get current of battery 
*  @param [in]  None 
*  @param [out]  None
*  @return uint16_t
*  @retval Returns the current being supplied (or accepted) through the battery terminals - mA
*/
int16_t smartBattery_GetCurrent()
{
    uint16_t current = 0;
    current = g_InternalBatteryData.current;
    return current;
}

/** @brief Get predicted remaining battery life of battery 
*  @param [in]  None 
*  @param [out]  None
*  @return uint16_t
*  @retval Returns the predicted remaining battery life at the present rate of discharge (minutes)
*/
uint16_t smartBattery_GetRunTimeToEmpty()
{
    uint16_t time = 0;
    time = g_InternalBatteryData.runTimeToEmpty;
    return time;
}


/** @brief Get present status of battery 
 *  @param [in]  None 
 *  @param [out]  None
 *  @return uint16_t
 *  @retval status: present status of battery
 */
uint16_t smartBattery_GetStatus()
{
    uint16_t status = 0;
    status = g_InternalBatteryData.status;
    return status;  
}

/** @brief Get present temperature of battery 
*  @param [in]  None  
*  @param [out]  None
*  @return float
*  @retval temperature: present temperature of battery (°C)
*/  
float smartBattery_GetTemperature()
{
    float temperature = 0;
    temperature = g_InternalBatteryData.temperature;
    return temperature/10-273;  
}

/** @brief Get state of health (SOH) battery, SOH is the ratio of FullChargeCapacity() and DesignCapacity()
*  @param [in]  None  
*  @param [out]  None
*  @return float
*  @retval SOH: (0-100)ratio of FullChargeCapacity() and DesignCapacity() - percentage unit
*/  
float smartBattery_GetStateOfHealth()
{
    if(g_InternalBatteryData.designCapacity != 0)
    {
        return (100 * g_InternalBatteryData.fullChargeCapacity / g_InternalBatteryData.designCapacity);
    }
    else
    {
        return 100;
    }
}

/** @brief Get current charging state of battery (charging or not)
 *  @param [in]  None  
 *  @param [out]  None
 *  @return E_BatteryChargeState
 *  @retval chargeState: current charging state of battery
 */
E_BatteryChargeState smartBattery_GetChargeState()
{
    E_BatteryChargeState chargeState = eNotCharging;
    if(g_InternalBatteryData.status & BATTERY_DISCHARGING){
        chargeState = eNotCharging;
    }
    else{
      chargeState = eCharging;
    }
    return chargeState;  
}

/** @brief handle internal battery to update data 
 *  @param [in]  None 
 *  @param [out]  None
 *  @return None
 */
static void smartBattery_InternalBatteryHandle()
{
    
    switch (g_InternalBatteryData.handlestate)
    {   
        //set battery mode
        
        case eSetBatteryMode:            
            if(smartBattery_WriteData(eBatteryMode, 
                DISABLE_BROADCASTS_CHARGE | DISABLE_BROADCASTS_ALARM | DISABLE_CHARGE_CONTROL) == true)
            {
                g_InternalBatteryData.handlestate = eReadStatus;
            }
            break;
                    //read status word
        case eReadStatus:      
            if(smartBattery_ReadData(eBatteryStatus,
                        &g_InternalBatteryData.status) == true)
            {
                  g_InternalBatteryData.handlestate = eReadRemainPercent;
            }
          break; 
            
         //read remaining percentage 
        case eReadRemainPercent:
            if(smartBattery_ReadData(eRemainingPercentageOfFullCharge,
                                     &g_InternalBatteryData.remainPercent) == true) 
            {
                g_InternalBatteryData.handlestate = eReadRemainCapacity;
                
            }
            break;
        //read remaining capacity
        case eReadRemainCapacity:
            if(smartBattery_ReadData(eRemainingCapacity,
                                     &g_InternalBatteryData.remainCapacity) == true)
            {
                g_InternalBatteryData.handlestate = eReadTemperature;
            }
            break;
        //read temperature
        case eReadTemperature:
            if(smartBattery_ReadData(eTemperature,
                                     &g_InternalBatteryData.temperature) == true){
                g_InternalBatteryData.handlestate = eReadVoltage;
            }
            break;
        //read current voltage
        case eReadVoltage:
            if(smartBattery_ReadData(eVoltage,
                               &g_InternalBatteryData.voltage) == true)
            {
                g_InternalBatteryData.handlestate = eReadCurrent;
            }
            break;
      case eReadCurrent:
            if(smartBattery_ReadData(eCurrent,
                               (int16_t*)&g_InternalBatteryData.current) == true)
            {
                g_InternalBatteryData.handlestate = eReadRunTimeToEmpty;
            }
            break; 
        case eReadRunTimeToEmpty:
            if(smartBattery_ReadData(eAverageTimeToEmpty,
                               (int16_t*)&g_InternalBatteryData.runTimeToEmpty) == true)
            {
                g_InternalBatteryData.handlestate = eReadFullChargeCapacity;
            }
            break; 
            
        case eReadFullChargeCapacity:
            if(smartBattery_ReadData(eCapacityOfFullCharge,
                               (int16_t*)&g_InternalBatteryData.fullChargeCapacity) == true)
            {
                g_InternalBatteryData.handlestate = eReadDesignCapacity;
                //SYS_PRINT ("battery fullChargeCapacity %d\n", g_InternalBatteryData.fullChargeCapacity);
            }          
            break;
            
        case eReadDesignCapacity:
            if(smartBattery_ReadData(eDesignCapacity,
                               (int16_t*)&g_InternalBatteryData.designCapacity) == true)
            {
                g_InternalBatteryData.handlestate = eSetBatteryMode;
                //SYS_PRINT ("battery designCapacity %d\n", g_InternalBatteryData.designCapacity);
            }
            break;
            
        default:
            g_InternalBatteryData.handlestate = eSetBatteryMode;
            break;
    }         
}

/** @brief configure internal charger
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 *  @retval None
 */
// static void smartBattery_ConfigureInternalCharger()
//{    
//    charger_SetChargeOption(g_ChargeOption);
//    charger_SetInputCurrent(4480);//mAh
//    charger_SetChargeVoltage(17152);//168000mV
//    charger_SetChargeCurrent(1000);//2000mAh            
//}

/** @brief clear battery data on RAM
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */

static void smartBattery_ResetBatteryData()
{
    //g_InternalBatteryData.BattConnectState = eBATTNotConnected;
    g_InternalBatteryData.handlestate = eSetBatteryMode;
    g_InternalBatteryData.remainPercent = 0;
    g_InternalBatteryData.remainCapacity = 0;
    g_InternalBatteryData.temperature = 2730;//unit 0.1°Kelvin
    g_InternalBatteryData.voltage = 0;
    g_InternalBatteryData.status = BATTERY_DISCHARGING;
    g_InternalBatteryData.chargeCurrent = 0;
    g_InternalBatteryData.chargeVoltage = 0; 
}

/** @brief run task for handling battery, including
 * read data from battery and control charging
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void smartBattery_Handle()
{
    static uint32_t s_cnt = 0;
    bool handle = false;
    if(s_cnt >= SMART_BATTERY_TASK_PERIODIC_MS){
        s_cnt = 0;
        handle = true;
    }
    else{
      s_cnt++;
    }
    
    if (handle == true)
    {
        
        E_BATTERY_CONNECT_STAT currBattCnctState;
        //if BATT_CONNECTED pin goes low , battery is connected
        if(BAT_DETECTStateGet( ) == false)
        {
            //Battery connect
            currBattCnctState = eBATTConnected;
            //SYS_PRINT("BATT  Connect\n");
        }
        else
        {
          //Battery disconnect
          currBattCnctState = eBATTNotConnected;
          //SYS_PRINT("BATT not Connect\n");
        }
        
        if (g_InternalBatteryData.BattConnectState != currBattCnctState)
        {           
            g_InternalBatteryData.BattConnectState = currBattCnctState;
            if(g_InternalBatteryData.BattConnectState == eBATTNotConnected)
            {
                smartBattery_ResetBatteryData();               
            }
        }
        
        if(g_InternalBatteryData.BattConnectState == eBATTConnected)
        {
            //handle read data when battery connect
            smartBattery_InternalBatteryHandle();
        }
    }
    
//    static uint32_t cnt3 = 0;
//    if(cnt3 >= 5*60*1000/10)
//    //if(cnt3 >= 3*1000/10)
//    {
//        //SYS_PRINT("\n battery percent %d \n", smartBattery_GetRemainingPercentage(eInternalBattery));
//        //SYS_PRINT("battery capacity %d \n", smartBattery_GetRemainingCapacity(eInternalBattery));
//        //SYS_PRINT("battery temperature %.1f \n", smartBattery_GetTemperature(eInternalBattery));
//        cnt3 = 0;
//    }
//    else
//        cnt3++;
    
//    SYS_PRINT ("battery gs_errorTimeCount %d\n", gs_errorTimeCount);
}


/** @brief  initialize data
 *  @param [in]  None  
 *  @param [out]  None
 *  @return None
 *  @retval None
 */
void smartBattery_Initialize()
{
    g_DCConnectState = eDCNotConnected;
    
    g_InternalBatteryData.BattConnectState = eBATTNotConnected;
    g_InternalBatteryData.handlestate = eSetBatteryMode;
    g_InternalBatteryData.remainPercent = 0;
    g_InternalBatteryData.remainCapacity = 0;
    g_InternalBatteryData.temperature = 2730;//unit 0.1°Kelvin
    g_InternalBatteryData.voltage = 0;
    g_InternalBatteryData.status = BATTERY_DISCHARGING;
    g_InternalBatteryData.chargeCurrent = 0;
    g_InternalBatteryData.chargeVoltage = 0;
        
}

/** @brief Get current status of battery communication
*  @param [in]  None 
*  @param [out]  None
*  @return bool
*  @retval true if error, false if not error
*/
bool smartBattery_IsBatteryCommunicationFailure()
{
    if(gs_communicationErr != eDeviceNoError)
    {
        return true;
    }
    else
    {
        return false;
    }
}
