/* ************************************************************************** */
/** @file [PC_Monitoring.c]
*  @brief {This file contain source code necessary to transfer data between JFLOW
* and PC APP, include handling data come from PC APP as well as sending data to 
* PC APP, initialize dependency components such as UART6 }
*  @author {bui phuoc}
*/
/* ************************************************************************** */



/* This section lists the other files that are included in this file.
*/

#include "system_config.h"
#include "system_definitions.h"
#include "system/debug/sys_debug.h"

#include "stdlib.h"
#include "string.h"

#include "UART_2.h"
#include "crc.h"
#include "MotorTask.h"
#include "HeaterTask.h"
#include "PC_Monitoring.h"
#include "RTC_BQ32002.h"
#include "ChamberUnit.h"
#include "Setting.h"
#include "BME280.h"
#include "SmartBattery.h"
#include "DeviceTask.h"
#include "LogInterface.h"
#include "math.h"

#include "DeviceInformation.h"
#include "7z/7zTypes.h"
#include "Cradle.h"
#include "ADC.h"

#include "GuiInterface.h"
#include "system/devcon/sys_devcon.h"
#include "DisplayControl.h"
#include "SystemInterface.h"

//#define DEBUG_PRINT_PC_COMMAND

#ifdef DEBUG_PRINT_PC_COMMAND
    #define SYS_PRINT(fmt, ...)                 SYS_DEBUG_Print(fmt, ##__VA_ARGS__)
#else
    #undef SYS_PRINT
    #define SYS_PRINT(fmt, ...)                 
#endif


/** @brief define maximum length for a packet to send to PC app */
#define PC_MONITOR_SEND_PACKET_MAX_LEN      50

/** @brief define maximum length for a packet that received from PC app*/
#define PC_MONITOR_RECEIVE_PACKET_MAX_LEN      50

/** @brief Scaling for data transfer between JFLOW and PC app */
#define PC_MONITOR_SCALE                100.0

/** @brief Header of a packet data during communicating between JFLOW and PC app*/
#define PC_MONITOR_HEADER			0xAA

/** @brief command to start Motor From PC app to JFLOW */
#define PC_MONITOR_START_CMD        0x01

/** @brief command to stop Motor From PC app to JFLOW */
#define PC_MONITOR_STOP_CMD         0x70

/** @brief command to adjust Flow Rate Setting From PC app to JFLOW */
#define PC_MONITOR_FLOW_CMD         0x02

/** @brief command length maximum */
#define COMMAND_LENGTH_MAX         30

/** @brief command length maximum */
#define COMMAND_CONTEND_LENGTH_MAX         30

static PC_MONITORING_t gs_PCMonitoringData;
static E_CommandHandle s_CommandHandle = eReadCommandAndParse;
static uint8_t s_currentCommand = NO_COMMAND;
static char s_commandContent[30] = {};
static uint8_t s_commandContentLen = 0;

uint8_t receiveDataLength;
static uint16_t s_logIndex = 0;

/** @brief local functions */
 static int PC_Monitor_ConnectCommand (void);
 static int PC_Monitor_RebootCommand (void);
 static int PC_Monitor_ShutDownCommand (void);
 static int PC_Monitor_SelfDiagnosticCommand (void);
 static int PC_Monitor_SoftwareUpdateCommand (void);
 
#ifdef FUNCTION_DISABLE_ALARM
  static int PC_Monitor_SetAllAlarmsEnableStateCommand (void);
     static int PC_Monitor_SetAlarmDisableStateCommand (void);
     static int PC_Monitor_SetAlarmEnableStateCommand (void);
     static int PC_Monitor_GetAlarmDisabledCommand (void);
#endif
 static int PC_Monitor_GetMainSoftwareVersionCommand (void);
 static int PC_Monitor_GetCradleSoftwareVersionCommand (void);
 static int PC_Monitor_GetChamberSoftwareVersionCommand (void);
 static int PC_Monitor_GetRespiratoryVersionCommand (void);

 static int PC_Monitor_SetMainUnitSerialNumberCommand (void);
 static int PC_Monitor_SetCradleSerialNumberCommand (void);
 static int PC_Monitor_SetChamberSerialNumberCommand (void);

 static int PC_Monitor_SetRespiratorySerialNumberCommand (void);
 static int PC_Monitor_SetRespiratoryCodeCommand (void);
 static int PC_Monitor_SetRespiratoryFactoryCommand (void);
 static int PC_Monitor_SetRespiratoryDateCommand (void);
 static int PC_Monitor_SetRespiratoryUseTimeCommand (void);
 static int PC_Monitor_SetRespiratoryUseCountCommand (void);
 static int PC_Monitor_SetRespiratoryTypeCommand (void);

 static int PC_Monitor_GetMainUnitSerialNumberCommand (void);
 static int PC_Monitor_GetCradleSerialNumberCommand (void);
 static int PC_Monitor_GetChamberSerialNumberCommand (void);

 static int PC_Monitor_GetRespiratorySerialNumberCommand (void);
 static int PC_Monitor_GetRespiratoryCodeCommand (void);
 static int PC_Monitor_GetRespiratoryFactoryCommand (void);
 static int PC_Monitor_GetRespiratoryDateCommand (void);
 static int PC_Monitor_GetRespiratoryUseTimeCommand (void);
 static int PC_Monitor_GetRespiratoryUseCountCommand (void);
 static int PC_Monitor_GetRespiratoryTypeCommand (void);

 static int PC_Monitor_ClearLogCommand (void);
 static int PC_Monitor_SetInitCommand (void);
 static int PC_Monitor_SetTemperatureSettingCommand(void);
 static int PC_Monitor_GetTemperatureSettingCommand(void);
 static int PC_Monitor_GetChamberOutletTemperatureCommand(void);
 static int PC_Monitor_GetBreathCircuitOutletTemperatureCommand(void);
 static int PC_Monitor_GetEnvironmentTemperatureCommand(void);
 static int PC_Monitor_GetBatteryTemperatureCommand(void);
 static int PC_Monitor_SetAirFlowCommand(void);
 static int PC_Monitor_GetAirFlowCommand(void);
 static int PC_Monitor_GetO2FlowCommand(void);
 static int PC_Monitor_SetOxygenConcentrationSettingCommand(void);
 static int PC_Monitor_GetOxygenConcentrationCommand(void);

 static int PC_Monitor_GetSPO2Command(void);
 static int PC_Monitor_GetPulseRateCommand(void);
 static int PC_Monitor_GetRunningTimeCommand(void);
 static int PC_Monitor_GetExternalBatteryConnectionCommand(void);
 static int PC_Monitor_GetExternalBatteryLevelCommand(void);
 static int PC_Monitor_GetMainBatteryConnectionCommand(void);
 static int PC_Monitor_GetMainBatteryLevelCommand(void);
 static int PC_Monitor_GetACConnectionCommand(void);
 static int PC_Monitor_SetCalibrationData1Command(void);
 static int PC_Monitor_SetCalibrationData2Command(void);
 static int PC_Monitor_GetAlarmNumberCommand(void);
 static int PC_Monitor_GetAlarmContentCommand(void);
 static int PC_Monitor_SetLanguageCommand(void);
 static int PC_Monitor_GetLanguageCommand(void);
 static int PC_Monitor_SetWifiCommand(void);
 static int PC_Monitor_GetWifiCommand(void);
 static int PC_Monitor_GetWifiAPCommand(void);
 static int PC_Monitor_SetDateTimeCommand(void);
 static int PC_Monitor_GetTimeCommand(void);
 static int PC_Monitor_GetLogNumberCommand(void);
 static int PC_Monitor_GetLogContentCommand(void);
 static int PC_Monitor_SetBrightnessModeCommand(void);
 static int PC_Monitor_GetBrightnessModeCommand(void);
 static int PC_Monitor_SetBrightnessLevelCommand(void);
 static int PC_Monitor_GetBrightnessLevelCommand(void);
 static int PC_Monitor_SetTreatmentModeCommand(void);
 static int PC_Monitor_GetTreatmentModeCommand(void);
 static int PC_Monitor_SetSpeakerLevelCommand(void);
 static int PC_Monitor_GetSpeakerLevelCommand(void);

 static int PC_Monitor_GetBrightnessSensorValueCommand(void);
 static int PC_Monitor_GetAccelerationSensorValueCommand(void);
 static int PC_Monitor_GetWaterLevelSensorValueCommand(void);
 static int PC_Monitor_GetBlowerSpeedCommand(void);

/** @brief Initialize PC monitor use to communicate with PC app by opening a COM
* port (UART6)
* This function should be called 1 times when start the application
*  @param [in] None
*  @param [out] None
*  @return None
*/
static void PC_Monitor_SendResponse(uint8_t* dataArray, uint32_t dataSize); 


//__attribute__((section(".ddr_data"), space(prog))) 
static const Command_t commandTable[NUM_OF_COMMAND] =
{
   {"CONNECT",                      CONNECT,                    PC_Monitor_ConnectCommand},
   {"REBOOT",                       REBOOT,                     PC_Monitor_RebootCommand},
   {"SHUTDOWN",                     SHUTDOWN,                   PC_Monitor_ShutDownCommand},
   {"GET_DIAG",                     GET_DIAG,                   PC_Monitor_SelfDiagnosticCommand},
   {"UPDATE",                       UPDATE,                     PC_Monitor_SoftwareUpdateCommand},

#ifdef FUNCTION_DISABLE_ALARM
   {"SET_ALL_ALARM",                SET_ALL_ALARM,              PC_Monitor_SetAllAlarmsEnableStateCommand},
   {"SET_ALARM_DISABLE",            SET_ALARM_DISABLE,          PC_Monitor_SetAlarmDisableStateCommand},
   {"SET_ALARM_ENABLE",             SET_ALARM_ENABLE,           PC_Monitor_SetAlarmEnableStateCommand},
   {"GET_ALARM_DISABLED",           GET_ALARM_DISABLED,         PC_Monitor_GetAlarmDisabledCommand},
#endif
   {"GET_MAIN_VERSION",             GET_MAIN_VERSION,           PC_Monitor_GetMainSoftwareVersionCommand},
   {"GET_CRADLE_VERSION",           GET_CRADLE_VERSION,         PC_Monitor_GetCradleSoftwareVersionCommand},
   {"GET_CHAMBER_VERSION",          GET_CHAMBER_VERSION,        PC_Monitor_GetChamberSoftwareVersionCommand},
   {"GET_RESPIRATORY_VERSION",      GET_RESPIRATORY_VERSION,    PC_Monitor_GetRespiratoryVersionCommand},
   
   
   {"SET_MAIN_SERIALNO",            SET_MAIN_SERIALNO,          PC_Monitor_SetMainUnitSerialNumberCommand},
   {"SET_CRADLE_SERIALNO",          SET_CRADLE_SERIALNO,        PC_Monitor_SetCradleSerialNumberCommand},
   {"SET_CHAMBER_SERIALNO",         SET_CHAMBER_SERIALNO,       PC_Monitor_SetChamberSerialNumberCommand},
   {"SET_RESPIRATORY_SERIALNO",     SET_RESPIRATORY_SERIALNO,   PC_Monitor_SetRespiratorySerialNumberCommand},
   {"SET_RESPIRATORY_CODE",         SET_RESPIRATORY_CODE,       PC_Monitor_SetRespiratoryCodeCommand},
   {"SET_RESPIRATORY_FACTORY",      SET_RESPIRATORY_FACTORY,    PC_Monitor_SetRespiratoryFactoryCommand},
   {"SET_RESPIRATORY_DATE",         SET_RESPIRATORY_DATE,       PC_Monitor_SetRespiratoryDateCommand},
   {"SET_RESPIRATORY_USE_TIME",     SET_RESPIRATORY_USE_TIME,   PC_Monitor_SetRespiratoryUseTimeCommand},
   {"SET_RESPIRATORY_USE_COUNT",    SET_RESPIRATORY_USE_COUNT,  PC_Monitor_SetRespiratoryUseCountCommand},
   {"SET_RESPIRATORY_TYPE",         SET_RESPIRATORY_TYPE,       PC_Monitor_SetRespiratoryTypeCommand},
   
   {"GET_MAIN_SERIALNO",            GET_MAIN_SERIALNO,          PC_Monitor_GetMainUnitSerialNumberCommand},
   {"GET_CRADLE_SERIALNO",          GET_CRADLE_SERIALNO,        PC_Monitor_GetCradleSerialNumberCommand},
   {"GET_CHAMBER_SERIALNO",         GET_CHAMBER_SERIALNO,       PC_Monitor_GetChamberSerialNumberCommand},
   {"GET_RESPIRATORY_SERIALNO",     GET_RESPIRATORY_SERIALNO,   PC_Monitor_GetRespiratorySerialNumberCommand},
   {"GET_RESPIRATORY_CODE",         GET_RESPIRATORY_CODE,       PC_Monitor_GetRespiratoryCodeCommand},
   {"GET_RESPIRATORY_FACTORY",      GET_RESPIRATORY_FACTORY,    PC_Monitor_GetRespiratoryFactoryCommand},
   {"GET_RESPIRATORY_DATE",         GET_RESPIRATORY_DATE,       PC_Monitor_GetRespiratoryDateCommand},
   {"GET_RESPIRATORY_USE_TIME",     GET_RESPIRATORY_USE_TIME,   PC_Monitor_GetRespiratoryUseTimeCommand},
   {"GET_RESPIRATORY_USE_COUNT",    GET_RESPIRATORY_USE_COUNT,  PC_Monitor_GetRespiratoryUseCountCommand},
   {"GET_RESPIRATORY_TYPE",         GET_RESPIRATORY_TYPE,       PC_Monitor_GetRespiratoryTypeCommand},
   
   {"SET_LOGCLEAR",                 SET_LOGCLEAR,               PC_Monitor_ClearLogCommand},
   {"SET_INIT",                     SET_INIT,                   PC_Monitor_SetInitCommand},
   {"SET_MOUTH_TEMP",               SET_MOUTH_TEMP,             PC_Monitor_SetTemperatureSettingCommand},
   {"GET_MOUTH_TEMP",               GET_MOUTH_TEMP,             PC_Monitor_GetTemperatureSettingCommand},
   {"GET_CHAMBER_TEMP",             GET_CHAMBER_TEMP,           PC_Monitor_GetChamberOutletTemperatureCommand},
   {"GET_BREATHING_TEMP",           GET_BREATHING_TEMP,         PC_Monitor_GetBreathCircuitOutletTemperatureCommand},
   {"GET_ENV_TEMP",                 GET_ENV_TEMP,               PC_Monitor_GetEnvironmentTemperatureCommand},
   {"GET_BATT_TEMP",                GET_BATT_TEMP,              PC_Monitor_GetBatteryTemperatureCommand},
   {"SET_AIR_FLOW",                 SET_AIR_FLOW,               PC_Monitor_SetAirFlowCommand},
   {"GET_AIR_FLOW",                 GET_AIR_FLOW,               PC_Monitor_GetAirFlowCommand},
   {"GET_O2_FLOW",                  GET_O2_FLOW,                PC_Monitor_GetO2FlowCommand},
   {"SET_OXYGEN",                   SET_OXYGEN,                 PC_Monitor_SetOxygenConcentrationSettingCommand},
   {"GET_OXYGEN",                   GET_OXYGEN,                 PC_Monitor_GetOxygenConcentrationCommand},
   {"GET_SPO2",                     GET_SPO2,                   PC_Monitor_GetSPO2Command},
   {"GET_PULSERATE",                GET_PULSERATE,              PC_Monitor_GetPulseRateCommand},
   {"GET_RUNNING",                  GET_RUNNING,                PC_Monitor_GetRunningTimeCommand},
   {"GET_OUTBATTEXT",               GET_OUTBATTEXT,             PC_Monitor_GetExternalBatteryConnectionCommand},
   {"GET_OUTBATTREM",               GET_OUTBATTREM,             PC_Monitor_GetExternalBatteryLevelCommand},
   {"GET_INBATTEXT",                GET_INBATTEXT,              PC_Monitor_GetMainBatteryConnectionCommand},
   {"GET_INBATTREM",                GET_INBATTREM,              PC_Monitor_GetMainBatteryLevelCommand},
   {"GET_AC",                       GET_AC,                     PC_Monitor_GetACConnectionCommand},
   {"SET_CAL1",                     SET_CAL1,                   PC_Monitor_SetCalibrationData1Command},
   {"SET_CAL2",                     SET_CAL2,                   PC_Monitor_SetCalibrationData2Command},
   {"GET_ALARMCNT",                 GET_ALARMCNT,               PC_Monitor_GetAlarmNumberCommand},
   {"GET_ALARM",                    GET_ALARM,                  PC_Monitor_GetAlarmContentCommand},
   {"SET_LANG",                     SET_LANG,                   PC_Monitor_SetLanguageCommand},
   {"GET_LANG",                     GET_LANG,                   PC_Monitor_GetLanguageCommand},
   {"SET_WIFI",                     SET_WIFI,                   PC_Monitor_SetWifiCommand},
   {"GET_WIFI",                     GET_WIFI,                   PC_Monitor_GetWifiCommand},
   {"GET_WIFIAP",                   GET_WIFIAP,                 PC_Monitor_GetWifiAPCommand},
   {"SET_DATETIME",                 SET_DATETIME,               PC_Monitor_SetDateTimeCommand},
   {"GET_DATETIME",                 GET_DATETIME,               PC_Monitor_GetTimeCommand},
   {"GET_LOGCNT",                   GET_LOGCNT,                 PC_Monitor_GetLogNumberCommand},
   {"GET_LOG",                      GET_LOG,                    PC_Monitor_GetLogContentCommand},
   {"SET_BRIGHTMODE",               SET_BRIGHTMODE,             PC_Monitor_SetBrightnessModeCommand},
   {"GET_BRIGHTMODE",               GET_BRIGHTMODE,             PC_Monitor_GetBrightnessModeCommand},
   {"SET_BRIGHTNESS",               SET_BRIGHTNESS,             PC_Monitor_SetBrightnessLevelCommand},
   {"GET_BRIGHTNESS",               GET_BRIGHTNESS,             PC_Monitor_GetBrightnessLevelCommand},
   {"SET_BACKGROUND",               SET_BACKGROUND,             NULL},
   {"GET_BACKGROUND",               GET_BACKGROUND,             NULL},
   {"SET_MODE",                     SET_MODE,                   PC_Monitor_SetTreatmentModeCommand},
   {"GET_MODE",                     GET_MODE,                   PC_Monitor_GetTreatmentModeCommand},
   {"SET_SPEAKER",                  SET_SPEAKER,                PC_Monitor_SetSpeakerLevelCommand},
   {"GET_SPEAKER",                  GET_SPEAKER,                PC_Monitor_GetSpeakerLevelCommand},
   
   {"GET_BRIGHTNESS_SENSOR",        GET_BRIGHTNESS_SENSOR,      PC_Monitor_GetBrightnessSensorValueCommand},
   {"GET_ACCELERATION_SENSOR",      GET_ACCELERATION_SENSOR,    PC_Monitor_GetAccelerationSensorValueCommand},
   {"GET_WATERLEVEL_SENSOR",        GET_WATERLEVEL_SENSOR,      PC_Monitor_GetWaterLevelSensorValueCommand},
   {"GET_BLOWERSPEED",              GET_BLOWER_SPEED,           PC_Monitor_GetBlowerSpeedCommand},
   
};

#ifdef FUNCTION_DISABLE_ALARM
static const AlarmOperationModeSetting_t s_AlarmOperateSetting [NUMBER_ALARM_OP_MODE_SETTING] =
{
    {"E900", eI2C1ErrorAlarmOperationModeSettingId},
    {"E901", eI2C2ErrorAlarmOperationModeSettingId},
    {"E902", eI2C3ErrorAlarmOperationModeSettingId},
    {"E903", eI2C4ErrorAlarmOperationModeSettingId},

    {"E904", eSPI3ErrorAlarmOperationModeSettingId},

    {"E905", eUart1ErrorAlarmOperationModeSettingId},
    {"E906", eUart2ErrorAlarmOperationModeSettingId},
    {"E907", eUart4ErrorAlarmOperationModeSettingId},
    {"E908", eUart6ErrorAlarmOperationModeSettingId},


    {"E909", eAirFlowSensorErrorAlarmOperationModeSettingId},//9
    {"E910", eO2FlowSensorErrorAlarmOperationModeSettingId},
    {"E911", eBME280ErrorAlarmOperationModeSettingId},
    {"E912", eADXL345ErrorAlarmOperationModeSettingId},        
    {"E913", eDRV8308ErrorAlarmOperationModeSettingId},
    {"E914", eAudioErrorAlarmOperationModeSettingId},
            
    {"E915", eMotorTaskErrorAlarmOperationModeSettingId},
    {"E916", eHeaterTaskErrorAlarmOperationModeSettingId},//16
    {"E917", eHumidityTaskErrorAlarmOperationModeSettingId},
        
    //Respiratory circuit abnormality
    {"E001", eBreathingCircuitNotConnectedAlarmOperationModeSettingId},  /**<The breathing circuit is not connected or not connected firmly to the device */
    {"E002", eCheckLeakAlarmOperationModeSettingId}, /**<Disconnected between breathing circuit and cannula, Or 80% of breathing circuit breaks */
    {"E003", eCheckBlockageAlarmOperationModeSettingId}, /**<Filter, breathing circuit or cannula blocked */
    {"E004", eNonGenuineCircuitAlarmOperationModeSettingId}, //21 /**<Non-genuine circuit*/
    {"E005", eBreathingCircuitChangedAlarmOperationModeSettingId}, /**<Tube check*/                // chua lam
    {"E006", eExpiredCircuitAlarmOperationModeSettingId}, /**<Expired circuit */
    
    //Chamber abnormality
    {"E007", eCheckConnectionChamberAlarmOperationModeSettingId},//24 /**<Chamber is not connected or not connected firmly to the device */
    {"E008", eNoMoreWaterInChamberAlarmOperationModeSettingId}, /**<There is no more water in the chamber */
    {"E009", eRunOutOfWaterAlarmOperationModeSettingId},        /**<Run out of water*/
            
    //temperature abnormality
    {"E010", eLowTemperatureAlarmOperationModeSettingId}, /**<Alarm when the temperature is too low (<18*C)*/
    {"E011", eHighTemperatureAlarmOperationModeSettingId},/**<Alarm when the ambient temperature higher than the setting setting temperature */
    {"E012", eHighTemperatureAbnormalityAlarmOperationModeSettingId},//29 /**<Alarm when the calculated mouth temperature at 1 sampling >= 42*C */
            
    //Outside the environment
    {"E013", eRoomTemperatureLowToAchieveTargetTemperatureAlarmOperationModeSettingId}, /**<Alarm when the mouth temperature did not reach to the target temperature after 30 minute running from started operation and the temperature of environment is lower than specification environment */
    {"E014", eCheckOperatingConditionsAlarmOperationModeSettingId}, /**<Alarm check operating condition */
    {"E015", eDeviceErrorToAchieveTargetTemperatureAlarmOperationModeSettingId},        /**<Alarm when the mouth temperature did not reach to the target temperature after 30 minute running from started operation and the operating environment temperature is within the specified ambient temperature range*/        
    
    //Abnormal oxygen concentration
    {"E016", eOxygenHighAlarmOperationModeSettingId}, /**<Oxygen concentration is higher than the alarm setting upper limit (The lower and upper limits of the default setting values are 21% and 60%)*/
    {"E017", eOxygenLowAlarmOperationModeSettingId},  /**<Oxygen concentration is lower than the alarm setting upper limit(The lower and upper limits of the default setting values are 21% and 60%)*/
    
    //Device position
    {"E018", eDevicePostureAbnormalAlarmOperationModeSettingId}, /**<Alarm when the installation orientation of the device is recognized upside down in TBD seconds (Definition of upside down will decide later)*/
    {"E019", eDevicePostureBadAlarmOperationModeSettingId}, /**<TBD*/
            
    //Battery
    {"E020", eSwitchToBatteryModeAlarmOperationModeSettingId},//37 /**<Switch to battery Mode*/
    {"E021", eBatteryLowAlarmOperationModeSettingId}, /**<Battery low*/
    {"E022", eBatteryGetsRunOutAlarmOperationModeSettingId}, /**<Battery gets run out*/
    {"E023", eStopFunctionAlarmOperationModeSettingId}, /**<Stop function*/
    {"E024", ePowerNotEnoughAlarmOperationModeSettingId}, /**< Power is not enough: AC not connected and L0 < B1+B2 ??L1 */
            
    //SpO2
    {"E025", eSpO2FailedAlarmOperationModeSettingId}, /**< Alarm when receive a SEARCHING TOO LONG or ARTIFACT signal from the Spo2 device*/
    {"E026", eSpO2LowAlarmOperationModeSettingId}, /**< Spo2 value is lower than the alarm setting lower limit (lower limit is 85% or more)*/
    {"E027", eSpO2SensorProbeAlarmOperationModeSettingId}, /**< Alarm when receive a NO FINGER OR SENSOR PROBLEM signal from the Spo2 device*/
    {"E028", eSpO2SignalLossAlarmOperationModeSettingId}, /**< Alarm when receive a LOST PULSE signal from the Spo2 device*/
    {"E029", eSpO2SensorUnpluggedAlarmOperationModeSettingId}, /**< Alarm when receive a SENSOR UNPLUGGED signal from the Spo2 device*/
            
    //Water
    {"E030", eWaterSupplyCheckAlarmOperationModeSettingId},//47 /**<Water supply check */
    {"E031", eWaterTankConnectionCheckAlarmOperationModeSettingId},/**<Water tank connection check */
            
    {"E032", eAirFlowAbnormalAlarmOperationModeSettingId}, /**<Air flow abnormal */
    {"E033", eMainUnitBatteryDisconnectedAlarmOperationModeSettingId},/**<Main unit battery disconnected */
    {"E034", eCradleBatteryDisconnectedAlarmOperationModeSettingId},/**<Cradle battery disconnected */
                      
                     
    //Internal failure
    {"E100", eFailureOxygenFlowMeasurementFlowSensorAlarmOperationModeSettingId},//53 /**<Failure of Oxygen flow measurement flow sensor */
    {"E101", eFailureAirFlowMeasurementFlowSensorAlarmOperationModeSettingId}, /**<Failure of Air flow measurement flow sensor */
    {"E102", eFailureChamberTemperatureSensorAlarmOperationModeSettingId}, /**<Failure of chamber temperature sensors */
    {"E103", eFailureCoilTemperatureSendorAlarmOperationModeSettingId}, /**<Failure of Coil temperature sensor */
    {"E104", eFailureBreathingCircuitOutTemperatureSensorAlarmOperationModeSettingId}, //57 /**<Failure of Breathing circuit out temperature sensor*/
    {"E105", eFailureEnvironmentSensorAlarmOperationModeSettingId}, /**<Failure of Environment sensor  (Temp, hum, pressure)*/
    {"E106", eFailureCurrentSensorAlarmOperationModeSettingId}, /**<Failure of Current sensor */
    {"E107", eFailureBlowerAlarmOperationModeSettingId}, /**<Failure of Blower */
    {"E109", eFailureWaterLevelSensorAlarmOperationModeSettingId}, /**<Failure of water level sensor */
    {"E110", eFailureExternalFlashMemoryAlarmOperationModeSettingId}, /**<extrnal flash memory failed */
    {"E111", eFailureAccelerationSensorAlarmOperationModeSettingId}, /**<Failure of acceleration sensor */
    {"E112", eFailureLightSensorAlarmOperationModeSettingId}, /**<Failure of light sensor */
    {"E108", eSpeakerDisconnectedAlarmOperationModeSettingId},

    {"E113", eMainUnitBatteryCommunicationErrorAlarmOperationModeSettingId}, //67 /**< Main unit battery communication error */
    {"E114", eCradleBatteryCommunicationErrorAlarmOperationModeSettingId}, /**<Cradle battery communication error */
    {"E115", eCradleCommunicationErrorAlarmOperationModeSettingId}, /**<Cradle communication error */
    {"E116", eMainMCUFailedOrOutOfControlAlarmOperationModeSettingId}, /**<Main MCU failed or out of control */    
    {"E117", eESP32FailedAlarmOperationModeSettingId}, /**<ESP32 Failed */
    {"E118", eBreathingCircuitHeaterWireBrokenAlarmOperationModeSettingId}, /**<Breathing circuit heater wire broken */
    {"E119", eFailureSpo2ModuleAlarmOperationModeSettingId}, /**<Failure of spo2 module */
    {"E120", eFailureRTCModuleAlarmOperationModeSettingId}, /**<Failure of RTC module */
    {"E121", eFailureLCDTouchModuleAlarmOperationModeSettingId}, /**<Failure of LCD touch module */
//    {"E122", eFailureMainUnitBatteryManagementICAlarmOperationModeSettingId}, /**<Failure of Main unit battery management IC */
//    {"E123", eFailureCradleBatteryManagementICAlarmOperationModeSettingId}, //77 /**<Failure of Cradle battery management IC */
    {"E124", eChamberTemperatureAbnormalAlarmOperationModeSettingId}, /**<Chamber temperature abnormal */
    {"E125", eTooMuchWaterInTheChamberAlarmOperationModeSettingId}, /**<Too much water in the chamber */
};
#endif


 static int PC_Monitor_ConnectCommand (void)
{
    SYS_PRINT("Handle command CONNECT\n");
    char send[] = "CONNECT_OK\n" ;      
    PC_Monitor_SendResponse(send, strlen(send));
}

 static int PC_Monitor_RebootCommand (void)
{
    SYS_PRINT("Handle command ReBoot\n");
    char send[] = "TEMPORARILY NOT SUPPORTED\n" ;
    PC_Monitor_SendResponse(send, strlen(send));
}
 static int PC_Monitor_ShutDownCommand (void)
{
    SYS_PRINT("Handle command ShutDown\n");
    char send[] = "SHUTTING_DOWN\n" ;      
    PC_Monitor_SendResponse(send, strlen(send));
    
//        //display pop up on GUI
//    guiInterface_SendEvent(eGuiTurnOffMachineId, 0);
//    //stop heater
//    HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId};
//    HeaterTask_SendEvent(hEvent);
//    vTaskDelay(5000);
//    //stop motor
//    MOTOR_CTRL_EVENT_t mEvent = {.id = eMotorStopId};
//    MotorTask_SendEvent(mEvent);
    OperationMgr_PowerOffSystem();
                    
}

 static int PC_Monitor_SelfDiagnosticCommand (void)
{
    SYS_PRINT("Handle command Self Diagnostic\n");
    char send[] = "GET_DIAG:PASS\n" ;  //pass if all sensors, device ok    
    PC_Monitor_SendResponse(send, strlen(send));
}

 static int PC_Monitor_SoftwareUpdateCommand (void)
{
    SYS_PRINT("Handle command Update\n");
    char* strTable[4] = {"UPDATE_MAIN\n", "UPDATE_CHAMBER\n", "UPDATE_CRADLE\n", "UPDATE_ESP32\n"};
    //char send[] = "TEMPORARILY NOT SUPPORTED\n";
    
    UpdateDevice_t updateDeviceTable[] =
    {
       {"MAIN", eMainUpdate},
       {"CHAMBER", eChamberUpdate},
       {"CRADLE", eCradleUpdate},
       {"ESP32", eEsp32Update}
    };
    
    bool isValidContent = false;
    
    int i = 0; 
    for(i = 0; i < eNumberDeviceUpdate; i++)
    {
        if(s_commandContentLen == strlen(updateDeviceTable[i].updateDeviceName))
        {
            if(memcmp(s_commandContent, &updateDeviceTable[i].updateDeviceName[0], s_commandContentLen) == 0)
            {
                PC_Monitor_SendResponse(strTable[i], strlen(strTable[i]));
                //PC_Monitor_SendResponse(send, strlen(send));
                isValidContent = true;
                switch(i)
                {
                    case eMainUpdate:
                        SYS_PRINT("Handle command Update Main\n");
                        DisplayControl_SetState(eUpdateScreenIsShowingDispState);
                        vTaskDelay(500);
                        systemInterface_SendEvent(eSystemUpgradeEventId, 1);
                        break;
                    case eChamberUpdate:
                        SYS_PRINT("Handle command Update Chamber\n");
                        DisplayControl_SetState(eUpdateScreenIsShowingDispState);
                        vTaskDelay(500);
                        systemInterface_SendEvent(eChamberUpgradeEventId, 1);
                        break;
                    case eCradleUpdate:
                        SYS_PRINT("Handle command Update Cradle\n");
                        DisplayControl_SetState(eUpdateScreenIsShowingDispState);
                        vTaskDelay(500);
                        systemInterface_SendEvent(eCradleUpgradeEventId, 1);
                        break;
                    case eEsp32Update:
                        SYS_PRINT("Handle command Update ESP32\n");
                        break;
                }
                break;
            }
        }
    }
    
//    uint8_t numberStr = 4;
//    char* strTable[4] = {"MAIN", "CHAMBER", "CRADLE", "ESP32"};
//    int i= 0; 
//    for(i = 0; i < numberStr; i++)
//    {
//        if(s_commandContentLen == strlen(strTable[i]))
//        {
//            if(memcmp(s_commandContent, &strTable[i][0], s_commandContentLen) == 0)
//            {
//                isValidContent = true;
//                break;
//            }
//        }
//    }
    
    
    if(isValidContent == false)
    {
        char send[] = "UPDATE_ERR:INVALID_DEVICE\n" ;      
        PC_Monitor_SendResponse(send, strlen(send));
    }
//    else
//    {
//        char send[] = "UPDATE_OK\n" ;      
//        PC_Monitor_SendResponse(send, strlen(send));
//    }
    
}
#ifdef FUNCTION_DISABLE_ALARM
 static int PC_Monitor_SetAllAlarmsEnableStateCommand (void)
{
    SYS_PRINT("Handle command Set all Alarm\n");
    char str1[] = "ENABLE";
    char str2[] = "DISABLE";
    bool isValidContent = false;
    int i;
    if(s_commandContentLen == strlen(str1))
    {
        if(memcmp(s_commandContent, str1, s_commandContentLen) == 0)
        {
            isValidContent = true;
            SYS_PRINT("Set All Alarms Enable\n");
            for(i = eFirsAlarmOperationModeSettingId; i <= eLastAlarmOperationModeSettingId; i++)
            {
                setting_Set(i, eAlarmEnable);
            }
            setting_Save();
        }
    }
    else if(s_commandContentLen == strlen(str2))
    {
        if(memcmp(s_commandContent, str2, s_commandContentLen) == 0)
        {
            isValidContent = true;
            SYS_PRINT("Set All Alarms Disable\n");
            for(i = eFirsAlarmOperationModeSettingId; i <= eLastAlarmOperationModeSettingId; i++)
            {
                setting_Set(i, eAlarmDisable);
            }
            setting_Save();
        }
    }
    
    if(isValidContent == false)
    {
        char send[] = "SET_ALL_ALARM_ERR:INVALID_STATE\n" ;      
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        char send[] = "SET_ALL_ALARM_OK\n" ;      
        PC_Monitor_SendResponse(send, strlen(send));
    }
}


 static int PC_Monitor_SetAlarmDisableStateCommand (void)
{
    char* send;
    int n;
    //scan defined alarm list 
    for (n = 0; n < NUMBER_ALARM_OP_MODE_SETTING; n++)
    {
        //find the command from defined List Alarm that has same length with received command
        uint8_t lenDefinedAlarmErrCode = strlen(s_AlarmOperateSetting[n].alarmErrorCodeName);

        //must compare length before comparing memory
        if(lenDefinedAlarmErrCode == s_commandContentLen)
        {
            //compare memory
            int8_t result = memcmp(&s_commandContent[0], &s_AlarmOperateSetting[n].alarmErrorCodeName[0], s_commandContentLen);

            if(result == 0)//found the alarm
            {
                //SYS_PRINT("index %d\n", i);
                SYS_PRINT("Disable alarm index %d\n",n);
                setting_Set(s_AlarmOperateSetting[n].alarmSettingId, eAlarmDisable);
                setting_Save();
                send = "SET_ALARM_DISABLE_OK\n";
                PC_Monitor_SendResponse(send, strlen(send));
                return;
            }

        }
    }
    send = "INVALID_ALARM_CODE\n";
    PC_Monitor_SendResponse(send, strlen(send));
}

 static int PC_Monitor_SetAlarmEnableStateCommand (void)
{
    char* send;
    int n;
    //scan defined alarm list 
    for (n = 0; n < NUMBER_ALARM_OP_MODE_SETTING; n++)
    {
        //find the command from defined List Alarm that has same length with received command
        uint8_t lenDefinedAlarmErrCode = strlen(s_AlarmOperateSetting[n].alarmErrorCodeName);

        //must compare length before comparing memory
        if(lenDefinedAlarmErrCode == s_commandContentLen)
        {
            //compare memory
            int8_t result = memcmp(&s_commandContent[0], &s_AlarmOperateSetting[n].alarmErrorCodeName[0], s_commandContentLen);

            if(result == 0)//found the alarm
            {
                //SYS_PRINT("index %d\n", i);
                SYS_PRINT("Enable alarm index %d\n",n);
                setting_Set(s_AlarmOperateSetting[n].alarmSettingId, eAlarmEnable);
                setting_Save();
                send = "SET_ALARM_ENABLE_OK\n";
                PC_Monitor_SendResponse(send, strlen(send));
                return;
            }

        }
    }
    send = "INVALID_ALARM_CODE\n";
    PC_Monitor_SendResponse(send, strlen(send));
}


 static int PC_Monitor_GetAlarmDisabledCommand (void)
{
    //TODO
    char send[200] = "GET_ALARM_DISABLED:";
    if(setting_IsInit() == true)
    {
        int n;
        int x = 0;
        for(n = 18; n < NUMBER_ALARM_OP_MODE_SETTING; n++)
        {
            if(strlen(send) > 190)
            {
                strcat(send, "...");
                break;
            }
            E_AlarmOperationMode mode = (E_AlarmOperationMode)setting_Get(s_AlarmOperateSetting[n].alarmSettingId);
            if(mode == eAlarmDisable)
            {
                if(x != 0){
                    strcat(send, ", ");
                }
                x++;
                strcat(send, s_AlarmOperateSetting[n].alarmErrorCodeName);
                
            }
        }
        if(x == 0){
            strcat(send, "NO_ALARM_DISABLED\n");
        }
        else{
            strcat(send, "\n");
        }
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        char str[] = "GET_ALARM_DISABLED_EER:GET_FAILURE\n";
        PC_Monitor_SendResponse(str, strlen(str));
    }
}
#endif

 static int PC_Monitor_GetMainSoftwareVersionCommand (void)
{
    SYS_PRINT("Handle command Get Main software version\n");
    char send[] = "GET_VERSION:" MAIN_FW_VERSION "\n" ;
    PC_Monitor_SendResponse(send, strlen(send));
}


 static int PC_Monitor_GetCradleSoftwareVersionCommand (void)
{
    SYS_PRINT("Handle command Get Cradle software version\n");
    char ver[3] = {};
    Cradle_GetCradleFirmwareVersion(ver);
    char send[50] = {};
    sprintf(send, "GET_CRADLE_VERSION:%d.%d.%d\n", ver[0], ver[1], ver[2]);
    PC_Monitor_SendResponse(send, strlen(send));
}

 static int PC_Monitor_GetChamberSoftwareVersionCommand (void)
{
    SYS_PRINT("Handle command Get Chamber software version\n");
    char ver[3] ={};
    Chamber_GetChamberFWVer(ver);
    char send[50] = {};
    sprintf(send, "GET_CHAMBER_VERSION:%d.%d.%d\n", ver[0], ver[1], ver[2]);
    PC_Monitor_SendResponse(send, strlen(send));
}

 static int PC_Monitor_GetRespiratoryVersionCommand(void)
{
    SYS_PRINT("Handle command Get Respiratory software version\n");
    char send[] = "GET_RESPIRATORY_VERSION:0.0.0\n" ;
    PC_Monitor_SendResponse(send, strlen(send));
}

 static int PC_Monitor_SetMainUnitSerialNumberCommand (void)
{
#define MAIN_SERIAL_NUMBER_LENGTH_MAX   16
    SYS_PRINT("Handle command set main unit serial number\n");
    SYS_PRINT("Set main unit serial is %.10s\n", s_commandContent);
    char serialNum[MAIN_SERIAL_NUMBER_LENGTH_MAX] = {};
    if(s_commandContentLen <= MAIN_SERIAL_NUMBER_LENGTH_MAX)
    {
        memcpy(serialNum, s_commandContent, s_commandContentLen);
        DeviceInformation_SetDeviceInfo(eMainSerialNoDeviceInfoId, serialNum);
        DeviceInformation_Save();
        char send[] = "SET_SERIALNO_OK\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else{
        char send[] = "INVALID_SERIALNO\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
}

 static int PC_Monitor_SetCradleSerialNumberCommand (void)
{
    SYS_PRINT("Handle command set cradle serial number\n");
    SYS_PRINT("Set cradle serial is %.10s\n", s_commandContent);
    
#define CRADLE_SERIAL_NUMBER_LENGTH_MAX   16
    
    char serialNum[CRADLE_SERIAL_NUMBER_LENGTH_MAX] = {};
    if(s_commandContentLen <= CRADLE_SERIAL_NUMBER_LENGTH_MAX)
    {
        memcpy(serialNum, s_commandContent, s_commandContentLen);
        Cradle_SetCradleSN(serialNum);
        DeviceInformation_SetDeviceInfo(eCradleSerialNoDeviceInfoId, serialNum);
        DeviceInformation_Save();
        char send[] = "SET_CRADLE_SERIALNO_OK\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else{
        char send[] = "INVALID_SERIALNO\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
}

 static int PC_Monitor_SetChamberSerialNumberCommand (void)
{
    SYS_PRINT("Handle command set chamber serial number\n");
    SYS_PRINT("Set chamber serial is %.10s\n", s_commandContent);
    
#define CHAMBER_SERIAL_NUMBER_LENGTH_MAX   16

    char serialNum[CHAMBER_SERIAL_NUMBER_LENGTH_MAX] = {};
    if(s_commandContentLen <= CHAMBER_SERIAL_NUMBER_LENGTH_MAX)
    {
        memcpy(serialNum, s_commandContent, s_commandContentLen);
        Chamber_SetChamberSN(serialNum);
        DeviceInformation_SetDeviceInfo(eChamberSerialNoDeviceInfoId, serialNum);
        DeviceInformation_Save();
        char send[] = "SET_CHAMBER_SERIALNO_OK\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else{
        char send[] = "INVALID_SERIALNO\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
}

 static int PC_Monitor_SetRespiratorySerialNumberCommand (void)
{
    SYS_PRINT("Handle command set respiratory serial number\n");
    SYS_PRINT("Set respiratory serial is %.10s\n", s_commandContent);
    
#define CHAMBER_SERIAL_NUMBER_LENGTH_MAX   16
    if( chamber_GetBreathingCircuitConnection() == false)
    {
        char send[] = "BREATHING_CIRCUIT_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        char serialNum[CHAMBER_SERIAL_NUMBER_LENGTH_MAX] = {};
        if(s_commandContentLen <= CHAMBER_SERIAL_NUMBER_LENGTH_MAX)
        {
            memcpy(serialNum, s_commandContent, s_commandContentLen);
            Chamber_SetBreathCircuitSerialNumber(serialNum);
            DeviceInformation_SetDeviceInfo(eCircuitSerialNoDeviceInfoId, serialNum);
            DeviceInformation_Save();
            char send[] = "SET_RESPIRATORY_SERIALNO_OK\n" ;
            PC_Monitor_SendResponse(send, strlen(send));
        }
        else{
            char send[] = "INVALID_SERIALNO\n" ;
            PC_Monitor_SendResponse(send, strlen(send));
        }
    }
}

//manufacture code
 static int PC_Monitor_SetRespiratoryCodeCommand (void)
{
    SYS_PRINT("Handle command set BC manufacture code\n");
    SYS_PRINT("Set BC manufacture code is %.10s\n", s_commandContent);
#define BC_MANUF_CODE_LEN_MAX 3    
    bool err = false;
    if( chamber_GetBreathingCircuitConnection() == false)
    {
        char send[] = "BREATHING_CIRCUIT_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        char code[BC_MANUF_CODE_LEN_MAX + 1] = {};
        
        if(s_commandContentLen <= BC_MANUF_CODE_LEN_MAX)
        {
            memcpy(code, s_commandContent, s_commandContentLen);
            code[s_commandContentLen] = '\0';
            
            int num = atoi(code);//fixme: string character return 0;
            if((num <= ((1 << 8) -1)) && (num > 0))
            {
                Chamber_SetBreathCircuitManufactureCode(num);
                char send[] = "SET_RESPIRATORY_CODE_OK\n" ;
                PC_Monitor_SendResponse(send, strlen(send));
            }
            else
            {
                err = true;
            }
        }
        else
        {
            err = true;
        }
        
        if(err == true)
        {
            char send[] = "INVALID_CODE\n" ;
            PC_Monitor_SendResponse(send, strlen(send));
        }
    }
}
//factory code
 static int PC_Monitor_SetRespiratoryFactoryCommand (void)
{
    SYS_PRINT("Handle command set BC factory code\n");
    SYS_PRINT("Set BC factory code is %.10s\n", s_commandContent);
#define BC_FACTORY_CODE_LEN_MAX 3    
    bool err = false;
    if( chamber_GetBreathingCircuitConnection() == false)
    {
        char send[] = "BREATHING_CIRCUIT_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        if(s_commandContentLen <= BC_FACTORY_CODE_LEN_MAX)
        {
            char code[BC_FACTORY_CODE_LEN_MAX + 1] = {};
            memcpy(code, s_commandContent, s_commandContentLen);
            code[s_commandContentLen] = '\0';
            
            uint32_t num = atoi(code);

            if((num <= 2)&&(num >= 1))
            {
                Chamber_SetBreathCircuitFactoryCode(num);
                char send[] = "SET_FACTORY_CODE_OK\n" ;
                PC_Monitor_SendResponse(send, strlen(send));
            }
            else
            {
                err = true;
            }
        }
        else{
            err = true;
        }
        
        if(err == true)
        {
            char send[] = "INVALID_CODE\n" ;
            PC_Monitor_SendResponse(send, strlen(send));
        }
    }
}

//Set Breathing circuit manufacture date
 static int PC_Monitor_SetRespiratoryDateCommand (void)
{
    SYS_PRINT("Handle command set BC manufacture date\n");
    uint32_t time = 0;
    uint32_t year = 0;
    uint32_t month = 0;
    uint32_t date = 0;
    // Parse date
    char strYear[5] = {};
    char strMonth[3] = {};
    char strDate[3] = {};
    int i,j,k;
    for(i = 0; i < s_commandContentLen; i++)
    {
        if(s_commandContent[i] == '/')
        {
            break;
        }
        strYear[i] = s_commandContent[i];
    }
    i++;//skip '/' character
    
    for(j = 0; i+j < s_commandContentLen; j++)
    {
        if(s_commandContent[i+j] == '/'){
            break;
        }
        strMonth[j] = s_commandContent[i+j];
    }
    j++;//skip '/' character
    for(k = 0; i+j+k < s_commandContentLen; k++)
    {
        strDate[k] = s_commandContent[i+j+k];
    }

    // Set time
    year = atoi(strYear);
    month = atoi(strMonth);
    date = atoi(strDate);

    bool err = false;
    //check valid date TODO
    if (date > 31){
        err = true;
    }
    else if(month > 12){
        err = true;
    }
    else if(year > 2099){
        err = true;
    }
    if(err == true)
    {
        char send[40] = "SET_RESPIRATORY_DATE_ERR:INVALID_DATE\n";
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        time = year*10000 + month*100 + date;
        Chamber_SetBreathCircuitManufactureDate(time);
        char send[40] = "SET_RESPIRATORY_DATE_OK\n";
        PC_Monitor_SendResponse(send, strlen(send));
    }
}

//Set Breathing circuit use time
 static int PC_Monitor_SetRespiratoryUseTimeCommand (void)
{
    SYS_PRINT("Handle command set BC use time\n");
    
#define BC_USE_TIME_LEN_MAX 10    
    bool err = false;
    if( chamber_GetBreathingCircuitConnection() == false)
    {
        char send[] = "BREATHING_CIRCUIT_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        char str[BC_USE_TIME_LEN_MAX + 1] = {};
        
        if(s_commandContentLen <= BC_USE_TIME_LEN_MAX)
        {
            memcpy(str, s_commandContent, s_commandContentLen);
            str[s_commandContentLen] = '\0';
            
            int time = atoi(str);//fixme: string character return 0;
            if((time <= ((1 << 16) -1)) && (time > 0))
            {
                Chamber_SetBreathCircuitUseTime((uint16_t)time);
                char send[] = "SET_RESPIRATORY_USE_TIME_OK\n" ;
                PC_Monitor_SendResponse(send, strlen(send));
            }
            else
            {
                err = true;
            }
        }
        else
        {
            err = true;
        }
        
        if(err == true)
        {
            char send[] = "INVALID_TIME\n" ;
            PC_Monitor_SendResponse(send, strlen(send));
        }
    }
}
//Set Breathing circuit use count
 static int PC_Monitor_SetRespiratoryUseCountCommand (void)
{
    SYS_PRINT("Handle command set BC cycle count\n");
    
#define BC_CYCLE_COUNT_LEN_MAX 10    
    bool err = false;
    if( chamber_GetBreathingCircuitConnection() == false)
    {
        char send[] = "BREATHING_CIRCUIT_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        char str[BC_CYCLE_COUNT_LEN_MAX + 1] = {};
        
        if(s_commandContentLen <= BC_CYCLE_COUNT_LEN_MAX)
        {
            memcpy(str, s_commandContent, s_commandContentLen);
            str[s_commandContentLen] = '\0';
            
            int count = atoi(str);//fixme: string of alphabet character return 0;
            if((count <= ((1 << 16) -1)) && (count > 0))
            {
                Chamber_SetBreathCircuitCycleCount((uint16_t)count);
                char send[] = "SET_RESPIRATORY_USE_COUNT_OK\n" ;
                PC_Monitor_SendResponse(send, strlen(send));
            }
            else
            {
                err = true;
            }
        }
        else
        {
            err = true;
        }
        
        if(err == true)
        {
            char send[] = "INVALID_COUNT\n" ;
            PC_Monitor_SendResponse(send, strlen(send));
        }
    }
}
//Set Breathing circuit use type
 static int PC_Monitor_SetRespiratoryTypeCommand (void)
{
    SYS_PRINT("Handle command set BC type\n");
    
    bool err = false;
    if( chamber_GetBreathingCircuitConnection() == false)
    {
        char send[] = "BREATHING_CIRCUIT_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {              
        if(s_commandContentLen == 1)
        {
            char type = s_commandContent[0];
            E_BreathingCircuitType bcType;
            if((type == 'A')||(type == 'P')||(type == 'S'))
            {
                if(type == 'A'){
                    bcType = eTypeAdult;
                }
                else if(type == 'P'){
                    bcType = eTypePediatric;
                }
                else if(type == 'S'){
                    bcType = eTypeDisinfection;
                }
                Chamber_SetBreathCircuitType(bcType);
                char send[] = "SET_RESPIRATORY_TYPE_OK\n" ;
                PC_Monitor_SendResponse(send, strlen(send));
            }
            else
            {
                err = true;
            }
        }
        else
        {
            err = true;
        }
        
        if(err == true)
        {
            char send[] = "INVALID_TYPE\n" ;
            PC_Monitor_SendResponse(send, strlen(send));
        }
    }
}

//Get Main serial no
 static int PC_Monitor_GetMainUnitSerialNumberCommand (void)
{
    SYS_PRINT("Handle command get main unit serial number\n");
    
    char serialNum[20] = {};
    DeviceInformation_GetDeviceInfo(eMainSerialNoDeviceInfoId, serialNum);

    char send[30] = "GET_SERIALNO:";
    strcat(send, serialNum);
    strcat(send, "\n");
    PC_Monitor_SendResponse(send, strlen(send));
    
}

//Get cradle serial no
 static int PC_Monitor_GetCradleSerialNumberCommand (void)
{
    SYS_PRINT("Handle command get cradle serial number\n");
    
    if( cradle_GetCradleConnection() == eDisconnect)
    {
        char send[] = "CRADLE_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        char serialNum[20] = {};
        //DeviceInformation_GetDeviceInfo(eCradleSerialNoDeviceInfoId, serialNum);
        cradle_GetCradleSN(serialNum);
        char send[40] = "GET_CRADLE_SERIALNO:";
        strcat(send, serialNum);
        strcat(send, "\n");
        PC_Monitor_SendResponse(send, strlen(send));
    }
}

//Get chamber serial no
 static int PC_Monitor_GetChamberSerialNumberCommand (void)
{
    SYS_PRINT("Handle command get chamber serial number\n");
 
    if( chamber_GetChamberConnection() == eChamberDisconnected)
    {
        char send[] = "CHAMBER_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {    
        char serialNum[20] = {};
        //DeviceInformation_GetDeviceInfo(eChamberSerialNoDeviceInfoId, serialNum);
        Chamber_GetChamberSN(serialNum);

        char send[40] = "GET_CHAMBER_SERIALNO:";
        strcat(send, serialNum);
        strcat(send, "\n");
        PC_Monitor_SendResponse(send, strlen(send));
    }
}

//Get BC serial no
 static int PC_Monitor_GetRespiratorySerialNumberCommand (void)
{
    SYS_PRINT("Handle command get breathing circuit serial number\n");
    if( chamber_GetBreathingCircuitConnection() == false)
    {
        char send[] = "BREATHING_CIRCUIT_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        char serialNum[20] = {};
        //DeviceInformation_GetDeviceInfo(eCircuitSerialNoDeviceInfoId, serialNum);
        Chamber_GetBreathingCircuitSerialNumber(serialNum);

        char send[40] = "GET_RESPIRATORY_SERIALNO:";
        strcat(send, serialNum);
        strcat(send, "\n");
        PC_Monitor_SendResponse(send, strlen(send));
    }
}

//Get BC Manufacture code
 static int PC_Monitor_GetRespiratoryCodeCommand (void)
{
    SYS_PRINT("Handle command get breathing circuit manufacture code\n");
    
    if( chamber_GetBreathingCircuitConnection() == false)
    {
        char send[] = "BREATHING_CIRCUIT_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {    
        uint8_t code = Chamber_GetBreathingCircuitManufactureCode();
        char str[4] = {};
        sprintf(str, "%d\n", code);
        char send[40] = "GET_RESPIRATORY_CODE:";
        strcat(send, str);
        PC_Monitor_SendResponse(send, strlen(send));
    }
}
//get BC factory code
 static int PC_Monitor_GetRespiratoryFactoryCommand (void)
{
    SYS_PRINT("Handle command get breathing circuit factory code\n");
    
    if( chamber_GetBreathingCircuitConnection() == false)
    {
        char send[] = "BREATHING_CIRCUIT_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        uint8_t code = Chamber_GetBreathingCircuitFactoryCode();
        char str[4] = {};
        sprintf(str, "%d\n", code);
        char send[40] = "GET_RESPIRATORY_FACTORY:";
        strcat(send, str);
        PC_Monitor_SendResponse(send, strlen(send));
    }
}
//get BC manufacture date 
 static int PC_Monitor_GetRespiratoryDateCommand (void)
{
    SYS_PRINT("Handle command get breathing circuit manufacture date\n");

    if( chamber_GetBreathingCircuitConnection() == false)
    {
        char send[] = "BREATHING_CIRCUIT_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        uint32_t date = Chamber_GetBreathingCircuitManufactureDate();
        char str[10] = {};
        sprintf(str, "%d/%02d/%02d\n", date/10000, (date/100)%100, date%100);
        char send[40] = "GET_RESPIRATORY_DATE:";
        strcat(send, str);
        PC_Monitor_SendResponse(send, strlen(send));
    }
}
//get BC use time
 static int PC_Monitor_GetRespiratoryUseTimeCommand (void)
{
    SYS_PRINT("Handle command get breathing circuit use time\n");

    if( chamber_GetBreathingCircuitConnection() == false)
    {
        char send[] = "BREATHING_CIRCUIT_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        uint16_t time = Chamber_GetBreathingCircuitUsedTime();
        char str[10] = {};
        sprintf(str, "%d\n", time);
        char send[40] = "GET_RESPIRATORY_USE_TIME:";
        strcat(send, str);
        PC_Monitor_SendResponse(send, strlen(send));
    }
}
//get BC use count
 static int PC_Monitor_GetRespiratoryUseCountCommand (void)
{
    SYS_PRINT("Handle command get the number of times the breathing circuit has been used\n");
    
    if( chamber_GetBreathingCircuitConnection() == false)
    {
        char send[] = "BREATHING_CIRCUIT_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        uint16_t times = Chamber_GetBreathingCircuitCycleCount();
        char str[10] = {};
        sprintf(str, "%d\n", times);
        char send[40] = "GET_RESPIRATORY_USE_COUNT:";
        strcat(send, str);
        PC_Monitor_SendResponse(send, strlen(send));
    }
}
//get BC use type
 static int PC_Monitor_GetRespiratoryTypeCommand (void)
{
    SYS_PRINT("Handle command get breathing circuit type\n");
    
    if( chamber_GetBreathingCircuitConnection() == false)
    {
        char send[] = "BREATHING_CIRCUIT_NOT_CONNECT\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        E_BreathingCircuitType type = Chamber_GetBreathingCircuitType();

        if(type == eTypePediatric){
            char send[40] = "GET_RESPIRATORY_TYPE:P\n";
            PC_Monitor_SendResponse(send, strlen(send));
        }
        else if (type == eTypeAdult){
            char send[40] = "GET_RESPIRATORY_TYPE:A\n";
            PC_Monitor_SendResponse(send, strlen(send));
        }
        else if (type == eTypeDisinfection){
            char send[40] = "GET_RESPIRATORY_TYPE:S\n";
            PC_Monitor_SendResponse(send, strlen(send));
        }
    }
}
//clear log
 static int PC_Monitor_ClearLogCommand (void)
{
    SYS_PRINT("Handle clear log command\n");
    logInterface_SendLogRequest(eDeleteEventLogRequestId);
    logInterface_SendLogRequest(eDeleteAlarmLogRequestId);
    logInterface_SendLogRequest(eDeleteSpO2LogRequestId);
    char send[] = "SET_LOGCLEAR_OK\n" ;
    PC_Monitor_SendResponse(send, strlen(send));   
}

//initializer (reset default setting)
 static int PC_Monitor_SetInitCommand (void)
{
    //TODO
    SYS_PRINT("Handle command set init\n");
    char send[] = "TEMPORARILY NOT SUPPORTED\n" ;
    PC_Monitor_SendResponse(send, strlen(send));
}

//set temperature setting
 static int PC_Monitor_SetTemperatureSettingCommand(void)
{
    SYS_PRINT("Handle set temperature setting command\n");

#define TEMP_SETTING_LEN_MAX 10
    bool err = false;

    if(s_commandContentLen <= TEMP_SETTING_LEN_MAX)
    {
        char strTemp[TEMP_SETTING_LEN_MAX + 1] = {};
        memcpy(strTemp, s_commandContent, s_commandContentLen);
        strTemp[s_commandContentLen] = '\0';

        uint32_t temp = atoi(strTemp);

        if((temp <= 37)&&(temp >= 31))
        {
            setting_SetTemp(temp);
    
            char send[] = "SET_MOUTH_TEMP_OK\n" ;
            PC_Monitor_SendResponse(send, strlen(send));
        }
        else
        {
            err = true;
        }
    }
    else{
        err = true;
    }

    if(err == true)
    {
        char send[] = "INVALID_TEMPERATURE\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
}

//get temperature setting
 static int PC_Monitor_GetTemperatureSettingCommand(void)
{
    SYS_PRINT("Handle command get temperature setting \n");
    
    char str[10] = {};
    sprintf(str, "%.0f\n", gs_PCMonitoringData.settingTemp);
    char send[40] = "GET_MOUTH_TEMP:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
}

//get chamber out let temperature
 static int PC_Monitor_GetChamberOutletTemperatureCommand(void)
{
    SYS_PRINT("Handle command get chamber Outlet temperature\n");
    
    char str[10] = {};
    sprintf(str, "%.1f\n", gs_PCMonitoringData.chamberOutletTemp);
    char send[40] = "GET_CHAMBER_TEMP:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
}

// get breath circuit temperature
 static int PC_Monitor_GetBreathCircuitOutletTemperatureCommand(void)
{
    SYS_PRINT("Handle command get breathing circuit Outlet temperature\n");
    
    char str[10] = {};
    sprintf(str, "%.1f\n", gs_PCMonitoringData.breathingCircuitTemp);
    char send[40] = "GET_BREATHING_TEMP:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
}
//get environment temperature
 static int PC_Monitor_GetEnvironmentTemperatureCommand(void)
{
    SYS_PRINT("Handle command get environment temperature\n");
    
    char str[10] = {};
    sprintf(str, "%.1f\n", gs_PCMonitoringData.envTemp);
    char send[40] = "GET_ENV_TEMP:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
}
//get battery temperature
 static int PC_Monitor_GetBatteryTemperatureCommand(void)
{
    SYS_PRINT("Handle command get main battery temperature\n");
    
    char str[10] = {};
    sprintf(str, "%.1f\n", gs_PCMonitoringData.mainBatteryTemp);
    char send[40] = "GET_BATT_TEMP:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));   
}
//set flow rate setting
 static int PC_Monitor_SetAirFlowCommand(void)
{
    SYS_PRINT("Handle command set flow rate setting\n");
    
#define FLOW_SETTING_LEN_MAX 10
    bool err = false;

    if(s_commandContentLen <= FLOW_SETTING_LEN_MAX)
    {
        char str[FLOW_SETTING_LEN_MAX + 1] = {};
        memcpy(str, s_commandContent, s_commandContentLen);
        str[s_commandContentLen] = '\0';

        uint32_t flow = atoi(str);

        if((flow <= 60)&&(flow >= 10))
        {
            setting_SetAirFlow(flow);
    
            char send[] = "SET_AIR_FLOW_OK\n" ;
            PC_Monitor_SendResponse(send, strlen(send));
        }
        else
        {
            err = true;
        }
    }
    else{
        err = true;
    }

    if(err == true)
    {
        char send[] = "INVALID_FLOW\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
}

//get flow rate
 static int PC_Monitor_GetAirFlowCommand(void)
{
    SYS_PRINT("Handle command get air flow rate\n");
    
    char str[10] = {};
    sprintf(str, "%.1f\n", gs_PCMonitoringData.airFlow);
    char send[40] = "GET_AIR_FLOW:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));   
}
//get O2 flow rate
 static int PC_Monitor_GetO2FlowCommand(void)
{
    SYS_PRINT("Handle command get O2 flow rate\n");
    
    char str[10] = {};
    sprintf(str, "%.1f\n", gs_PCMonitoringData.O2Flow);
    char send[40] = "GET_O2_FLOW:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));     
}
//set  O2 concentration setting
 static int PC_Monitor_SetOxygenConcentrationSettingCommand(void)
{
    //TODO
}
//get O2 concentration
 static int PC_Monitor_GetOxygenConcentrationCommand(void)
{
    SYS_PRINT("Handle command get O2 concentration\n");
    
    char str[10] = {};
    sprintf(str, "%.0f\n", gs_PCMonitoringData.O2Concentration);
    char send[40] = "GET_OXYGEN:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
}
//get spo2
 static int PC_Monitor_GetSPO2Command(void)
{
    SYS_PRINT("Handle command get SPO2\n");
    
    char str[10] = {};
    sprintf(str, "%d\n", gs_PCMonitoringData.SpO2Value);
    char send[40] = "GET_SPO2:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
}

//get pulse rate
 static int PC_Monitor_GetPulseRateCommand(void)
{
    SYS_PRINT("Handle command get Pulse rate \n");
    
    char str[10] = {};
    sprintf(str, "%d\n", gs_PCMonitoringData.pulseRate);
    char send[40] = "GET_PULSERATE:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
}

 static int PC_Monitor_GetRunningTimeCommand(void)
{
    SYS_PRINT("Handle command get running time\n");
    
    uint32_t runTime = LogInterface_GetRunningTime();
    uint8_t str[20];
    sprintf(str, "%d:%02d:%02d\n", runTime/3600, runTime%3600/60, runTime%3600%60);
    
    char send[40] = "GET_RUNNING:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));   
}

 static int PC_Monitor_GetExternalBatteryConnectionCommand(void)
{
    SYS_PRINT("Handle command get external battery connections status\n");
    
    char str[4] = {};
    uint8_t state = (uint8_t)gs_PCMonitoringData.outBattExt;
    sprintf(str, "%d\n",state);
    char send[40] = "GET_OUTBATTEXT:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
}

 static int PC_Monitor_GetExternalBatteryLevelCommand(void)
{
    SYS_PRINT("Handle command get external battery remaining level \n");
    
    char str[10] = {};
    sprintf(str, "%d\n", gs_PCMonitoringData.outBattRem);
    char send[40] = "GET_OUTBATTREM:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));    
}

 static int PC_Monitor_GetMainBatteryConnectionCommand(void)
{
    SYS_PRINT("Handle command get main battery connections status\n");
    
    char str[4] = {};
    uint8_t state = (uint8_t)gs_PCMonitoringData.inBattExt;
    sprintf(str, "%d\n",state);
    char send[40] = "GET_INBATTEXT:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send)); 
}

 static int PC_Monitor_GetMainBatteryLevelCommand(void)
{
    SYS_PRINT("Handle command get main battery remaining level\n");
    
    char str[10] = {};
    sprintf(str, "%d\n", gs_PCMonitoringData.inBattRem);
    char send[40] = "GET_INBATTREM:";
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));       
}

 static int PC_Monitor_GetACConnectionCommand(void)
{
    SYS_PRINT("Handle command get AC connection\n");
    
    if(gs_PCMonitoringData.ACState == true)
    {
       char send[40] = "GET_AC:ON\n";
       PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
       char send[40] = "GET_AC:OFF\n";
       PC_Monitor_SendResponse(send, strlen(send));
    }
}

 static int PC_Monitor_SetCalibrationData1Command(void)
{
    SYS_PRINT("Handle command set Calibration Data 1\n");
    
    char strGain[10] = {};
    char strOffset[10] = {};
    int i,k ;
    for(i = 0; i < s_commandContentLen; i++)
    {
        if(s_commandContent[i] == ':'){
            break;
        }
        strGain[i] = s_commandContent[i];
    }
    
    for(k = 0; k < s_commandContentLen; k++)
    {
        strOffset[k] = s_commandContent[i+k+1];//+1 to skip ':'
    }
    
    float gain = atof(strGain);
    float offset = atof(strOffset);
    //TODO:fixme: type uint8_t to float
    setting_Set(eCalibrationGainAirSettingId, (uint8_t)(gain*10));//scale by 10
    setting_Set(eCalibrationOffsetAirSettingId, (uint8_t)(offset*10));//scale by 10
    
    char send[40] = "SET_CAL1_OK\n";
    PC_Monitor_SendResponse(send, strlen(send));
       
}

 static int PC_Monitor_SetCalibrationData2Command(void)
{
    SYS_PRINT("Handle command set Calibration Data 2\n");
    
    char strGain[10] = {};
    char strOffset[10] = {};
    int i,k ;
    for(i = 0; i < s_commandContentLen; i++)
    {
        if(s_commandContent[i] == ':'){
            break;
        }
        strGain[i] = s_commandContent[i];
    }
    
    for(k = 0; k < s_commandContentLen; k++)
    {
        strOffset[k] = s_commandContent[i+k+1];//+1 to skip ':'
    }
    
    float gain = atof(strGain);
    float offset = atof(strOffset);
    //TODO:fixme: type uint8_t to float
    setting_Set(eCalibrationGainO2SettingId, (uint8_t)(gain*10));//scale by 10
    setting_Set(eCalibrationOffsetO2SettingId, (uint8_t)(offset*10));//scale by 10
    
    char send[40] = "SET_CAL2_OK\n";
    PC_Monitor_SendResponse(send, strlen(send));
}

 static int PC_Monitor_GetAlarmNumberCommand(void)
{
    //TODO
    SYS_PRINT("Handle command get number of alarm log \n");
    
    char send[40] = "GET_ALARMCNT:";
    char str[10];
    uint16_t numLog = LogInterface_GetTotalAlarm();
    sprintf(str, "%d\n", numLog);
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
}

 static int PC_Monitor_GetAlarmContentCommand(void)
{
    //TODO
    SYS_PRINT("Handle command get alarm log m \n");
   
 
    #define LOG_ALARM_M_LEN_MAX 10
    bool err = false;

    if(s_commandContentLen <= LOG_ALARM_M_LEN_MAX)
    {
        char strIdx[LOG_ALARM_M_LEN_MAX + 1] = {};
        memcpy(strIdx, s_commandContent, s_commandContentLen);
        strIdx[s_commandContentLen] = '\0';

        int32_t logInx = atoi(strIdx);
        //Log alarm number starts at number 1, but index log alarm starts at number 0
        logInx = logInx - 1;
        if((logInx < LogInterface_GetTotalAlarm())&&(logInx >= 0))
        {
            char send[40] = "GET_ALARM";
            char str[80] = {};
            Log_Struct log;
            LogInterface_GetAlarmContent(logInx, &log);
            
            sprintf(str, "%d:ALARM_ID:%s, DATE_TIME:%d/%02d/%02d %02d:%02d:%02d\n",
            logInx, alarmStringList[log.eCode], log.time.year_1*100 + log.time.year_2, log.time.month, log.time.date,
            log.time.hour, log.time.minute, log.time.second);
            
            strcat(send, str);
            PC_Monitor_SendResponse(send, strlen(send));
        }
        else
        {
            err = true;
        }
    }
    else{
        err = true;
    }

    if(err == true)
    {
        char send[] = "INVALID_LOGINDEX\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
}
//set language
 static int PC_Monitor_SetLanguageCommand(void)
{
    SYS_PRINT("Handle command set language\n");
    char str1[] = "EN";
    char str2[] = "VN";
    char str3[] = "JA";
    bool isValidContent = false;
    if(s_commandContentLen == strlen(str1))
    {
        if(memcmp(s_commandContent, str1, s_commandContentLen) == 0)
        {
            isValidContent = true;
            SYS_PRINT("Set Language English\n");
            setting_SetLanguage(eEnglish);
        }
    }
    if(s_commandContentLen == strlen(str2))
    {
        if(memcmp(s_commandContent, str2, s_commandContentLen) == 0)
        {
            isValidContent = true;
            SYS_PRINT("Set Language VietNamese\n");
            setting_SetLanguage(eVietnamese);
        }
    }
    if(s_commandContentLen == strlen(str3))
    {
        if(memcmp(s_commandContent, str3, s_commandContentLen) == 0)
        {
            isValidContent = true;
            SYS_PRINT("Set Language Japanese\n");
            setting_SetLanguage(eJapanese);
        }
    }
    if(isValidContent == false)
    {
        char send[] = "SET_LANG_ERR:INVALID_LANG\n" ;      
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        char send[] = "SET_LANG_OK\n" ;      
        PC_Monitor_SendResponse(send, strlen(send));
    }
}
//get language
 static int PC_Monitor_GetLanguageCommand(void)
{
    SYS_PRINT("Handle command get language\n");
    
    if(gs_PCMonitoringData.language == eJapanese)
    {
       char send[40] = "GET_LANG:JA\n";
       PC_Monitor_SendResponse(send, strlen(send));
    }
    else if(gs_PCMonitoringData.language == eEnglish)
    {
       char send[40] = "GET_LANG:EN\n";
       PC_Monitor_SendResponse(send, strlen(send));
    }
    else if(gs_PCMonitoringData.language == eVietnamese)
    {
       char send[40] = "GET_LANG:VN\n";
       PC_Monitor_SendResponse(send, strlen(send));
    }
}
 static int PC_Monitor_SetWifiCommand(void)
{
    SYS_PRINT("Handle command set wifi\n");
    char str1[] = "ON";
    char str2[] = "OFF";

    bool isValidContent = false;
    if(s_commandContentLen == strlen(str1))
    {
        if(memcmp(s_commandContent, str1, s_commandContentLen) == 0)
        {
            isValidContent = true;
            SYS_PRINT("Set Wifi ON\n");
            setting_SetWifi(eSettingOn);
        }
    }
    else if(s_commandContentLen == strlen(str2))
    {
        if(memcmp(s_commandContent, str2, s_commandContentLen) == 0)
        {
            isValidContent = true;
            SYS_PRINT("Set Wifi Off\n");
            setting_SetWifi(eSettingOff);
        }
    }

    if(isValidContent == false)
    {
        char send[] = "SET_WIFI_ERR:INVALID_STATE\n" ;      
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        char send[] = "SET_WIFI_OK\n" ;      
        PC_Monitor_SendResponse(send, strlen(send));
    }
}
 static int PC_Monitor_GetWifiCommand(void)
{
    SYS_PRINT("Handle command get wifi\n");
    
    if(gs_PCMonitoringData.WifiState == eSettingOn)
    {
       char send[40] = "GET_WIFI:ON\n";
       PC_Monitor_SendResponse(send, strlen(send));
    }
    else if(gs_PCMonitoringData.WifiState == eSettingOff)
    {
       char send[40] = "GET_WIFI:OFF\n";
       PC_Monitor_SendResponse(send, strlen(send));
    }
}
 static int PC_Monitor_GetWifiAPCommand(void)
{
    //TODO
     SYS_PRINT("Handle command get Wifi AP\n");
     char send[] = "TEMPORARILY NOT SUPPORTED\n" ;
     PC_Monitor_SendResponse(send, strlen(send));
}
 static int PC_Monitor_SetDateTimeCommand(void)
{
    SYS_PRINT("Handle command set date time\n");
    RTC_TIME_t time;
    // Parse time
    char strYear[5] = {};
    char strMonth[3] = {};
    char strDate[3] = {};
    char strHour[3] = {};
    char strMin[3] = {};
    char strSec[3] = {};
    int i,j,k,m,n,x;
    for(i = 0; i < s_commandContentLen; i++)
    {
        if(s_commandContent[i] == '/')
        {
            break;
        }
        strYear[i] = s_commandContent[i];
    }
    i++;//skip '/' character
    
    for(j = 0; i+j < s_commandContentLen; j++)
    {
        if(s_commandContent[i+j] == '/'){
            break;
        }
        strMonth[j] = s_commandContent[i+j];
    }
    j++;//skip '/' character
    for(k = 0; i+j+k < s_commandContentLen; k++)
    {
        if(s_commandContent[i+j+k] == ' '){//space character
            break;
        }
        strDate[k] = s_commandContent[i+j+k];
    }
    k++;//skip ' ' character
    for(m = 0; i+j+k+m < s_commandContentLen; m++)
    {
        if(s_commandContent[i+j+k+m] == ':'){//space character
            break;
        }
        strHour[m] = s_commandContent[i+j+k+m];
    }
    m++;//skip ':' character
    for(n = 0; i+j+k+m+n < s_commandContentLen; n++)
    {
        if(s_commandContent[i+j+k+m+n] == ':'){//space character
            break;
        }
        strMin[n] = s_commandContent[i+j+k+m+n];
    }
    n++;//skip ':' character
    for(x = 0; i+j+k+m+n+x < s_commandContentLen; x++)
    {
        strSec[x] = s_commandContent[i+j+k+m+n+x];
    }
    // Set time
    time.YEAR = atoi(strYear)%100;
    time.MONTH = atoi(strMonth);
    time.DATE = atoi(strDate);
    time.HOUR = atoi(strHour);
    time.MIN = atoi(strMin);
    time.SEC = atoi(strSec);
    
    bool err = false;
    if(time.SEC > 59){
        err = true;
    }
    else if(time.MIN > 59){
        err = true;
    }
    else if(time.HOUR > 23){
        err = true;
    }
    else if(time.DATE > 31){
        err = true;
    }
    else if(time.MONTH > 12){
        err = true;
    }
    else if(time.YEAR > 99){
        err = true;
    }
    if(err == true)
    {
        char send[40] = "SET_DATETIME_ERR:INVALID_DATETIME\n";
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        rtc_SetTime(time);
        char send[40] = "SET_DATETIME_OK\n";
        PC_Monitor_SendResponse(send, strlen(send));
    }
}

 static int PC_Monitor_GetTimeCommand(void)
{
    SYS_PRINT("Handle command get date time\n");
    RTC_TIME_t time;
    rtc_GetTime(&time);
    char strYear[5] = {};
    char strMonth[4] = {};
    char strDate[4] = {};
    char strHour[4] = {};
    char strMin[4] = {};
    char strSec[5] = {};
    sprintf(strYear, "%d", time.YEAR+2000);
    sprintf(strMonth, "/%02d", time.MONTH);
    sprintf(strDate, "/%02d", time.DATE);
    sprintf(strHour, " %02d", time.HOUR);
    sprintf(strMin, ":%02d", time.MIN);
    sprintf(strSec, ":%02d\n", time.SEC);
    char send[40] = "GET_DATETIME:";
    strcat(send, strYear);
    strcat(send, strMonth);
    strcat(send, strDate);
    strcat(send, strHour);
    strcat(send, strMin);
    strcat(send, strSec);
    PC_Monitor_SendResponse(send, strlen(send));
}
 static int PC_Monitor_GetLogNumberCommand(void)
{
    //TODO
    SYS_PRINT("Handle command get number of log \n");
    
    char send[40] = "GET_LOGCNT:";
    char str[10];
    uint16_t numLog = LogInterface_GetTotalLog();
    sprintf(str, "%d\n", numLog);
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
}
 static int PC_Monitor_GetLogContentCommand(void)
{
    //TODO
    SYS_PRINT("Handle command get log m \n");
   
 
    #define LOG_M_LEN_MAX 10
    bool err = false;

    if(s_commandContentLen <= LOG_M_LEN_MAX)
    {
        char strIdx[LOG_M_LEN_MAX + 1] = {};
        memcpy(strIdx, s_commandContent, s_commandContentLen);
        strIdx[s_commandContentLen] = '\0';

        int32_t logInx = atoi(strIdx);
        logInx = logInx -1;
        //log number start at number 1, but index number start at number 0

        if((logInx < LogInterface_GetTotalLog())&&(logInx >= 0))
        {
            char send[40] = "GET_LOG";
            char str[80] = {};
            Log_Struct log;
            LogInterface_GetLogContent(logInx, &log);
            
            sprintf(str, "%d:EVENT_ID:%s, DATE_TIME:%d/%02d/%02d %02d:%02d:%02d\n",
            logInx, eventStringList[log.eCode], log.time.year_1*100 + log.time.year_2, log.time.month, log.time.date,
            log.time.hour, log.time.minute, log.time.second);
            
            strcat(send, str);
            PC_Monitor_SendResponse(send, strlen(send));
        }
        else
        {
            err = true;
        }
    }
    else{
        err = true;
    }

    if(err == true)
    {
        char send[] = "INVALID_INDEX\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
}
 static int PC_Monitor_SetBrightnessModeCommand(void)
{
    SYS_PRINT("Handle command set brightness mode\n");
    char str1[] = "AUTO";
    char str2[] = "MANUAL";

    bool isValidContent = false;
    if(s_commandContentLen == strlen(str1))
    {
        if(memcmp(s_commandContent, str1, s_commandContentLen) == 0)
        {
            isValidContent = true;
            SYS_PRINT("Set brightness mode AUTO\n");
            setting_SetBrightnessMode(eAutoBrightnessMode);
        }
    }
    else if(s_commandContentLen == strlen(str2))
    {
        if(memcmp(s_commandContent, str2, s_commandContentLen) == 0)
        {
            isValidContent = true;
            SYS_PRINT("Set brightness mode MANUAL\n");
            setting_SetBrightnessMode(eManualBrightnessMode);
        }
    }

    if(isValidContent == false)
    {
        char send[] = "SET_BRIGHTMODE_ERR:INVALID_MODE\n" ;      
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        char send[] = "SET_BRIGHTMODE_OK\n" ;      
        PC_Monitor_SendResponse(send, strlen(send));
    }
}
 static int PC_Monitor_GetBrightnessModeCommand(void)
{
    SYS_PRINT("Handle command get brightness mode\n");
    
    if(gs_PCMonitoringData.brightnessMode == eAutoBrightnessMode)
    {
       char send[40] = "GET_BRIGHTMODE:AUTO\n";
       PC_Monitor_SendResponse(send, strlen(send));
    }
    else if(gs_PCMonitoringData.WifiState == eSettingOff)
    {
       char send[40] = "GET_BRIGHTMODE:MANUAL\n";
       PC_Monitor_SendResponse(send, strlen(send));
    }
}
 static int PC_Monitor_SetBrightnessLevelCommand(void)
{
    SYS_PRINT("Handle set brightness level\n");

#define BRIGHNESS_SETTING_LEN_MAX 10
    bool err = false;

    if(s_commandContentLen <= BRIGHNESS_SETTING_LEN_MAX)
    {
        char str[BRIGHNESS_SETTING_LEN_MAX + 1] = {};
        memcpy(str, s_commandContent, s_commandContentLen);
        str[s_commandContentLen] = '\0';

        uint32_t brightlevel = atoi(str)/10;

        if((brightlevel <= 10)&&(brightlevel >= 1))
        {
            setting_SetBrightnessLevel(brightlevel);
    
            char send[] = "SET_BRIGHNESS_OK\n" ;
            PC_Monitor_SendResponse(send, strlen(send));
        }
        else
        {
            err = true;
        }
    }
    else{
        err = true;
    }

    if(err == true)
    {
        char send[] = "INVALID_BRIGHTNESS\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }
}
 static int PC_Monitor_GetBrightnessLevelCommand(void)
{
    SYS_PRINT("Handle command get brightness level\n");
    
    char send[40] = "GET_BRIGHTNESS:";
    char str[10];
    sprintf(str, "%d\n", gs_PCMonitoringData.brightnessValue);
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
    
}
 static int PC_Monitor_SetTreatmentModeCommand(void)
{
    SYS_PRINT("Handle command set treatment mode\n");
    char str1[] = "ADULT";
    char str2[] = "CHILD";

    bool isValidContent = false;
    if(s_commandContentLen == strlen(str1))
    {
        if(memcmp(s_commandContent, str1, s_commandContentLen) == 0)
        {
            isValidContent = true;
            SYS_PRINT("Set mode ADULT\n");
            setting_SetMode(eAdultMode);
        }
    }
    if(s_commandContentLen == strlen(str2))
    {
        if(memcmp(s_commandContent, str2, s_commandContentLen) == 0)
        {
            isValidContent = true;
            SYS_PRINT("Set mode CHILD\n");
            setting_SetMode(eChildMode);
        }
    }

    if(isValidContent == false)
    {
        char send[] = "SET_MODE_ERR:INVALID_MODE\n" ;      
        PC_Monitor_SendResponse(send, strlen(send));
    }
    else
    {
        char send[] = "SET_MODE_OK\n" ;      
        PC_Monitor_SendResponse(send, strlen(send));
    }    
}
 static int PC_Monitor_GetTreatmentModeCommand(void)
{
    SYS_PRINT("Handle command get treatment mode\n");
    
    if(gs_PCMonitoringData.mode == eAdultMode)
    {
       char send[40] = "GET_MODE:ADULT\n";
       PC_Monitor_SendResponse(send, strlen(send));
    }
    else if(gs_PCMonitoringData.mode == eChildMode)
    {
       char send[40] = "GET_MODE:CHILD\n";
       PC_Monitor_SendResponse(send, strlen(send));
    }
}
 static int PC_Monitor_SetSpeakerLevelCommand(void)
{
SYS_PRINT("Handle set speaker level\n");

#define SPEAKER_SETTING_LEN_MAX 10
    bool err = false;

    if(s_commandContentLen <= SPEAKER_SETTING_LEN_MAX)
    {
        char str[SPEAKER_SETTING_LEN_MAX + 1] = {};
        memcpy(str, s_commandContent, s_commandContentLen);
        str[s_commandContentLen] = '\0';

        uint32_t level = atoi(str);//FIXME CASE 0, DATA:FWDDF-> 0

        if((level <= 10)&&(level >= 0))
        {
            setting_SetSpeaker(level);
    
            char send[] = "SET_SPEAKER_OK\n" ;
            PC_Monitor_SendResponse(send, strlen(send));
        }
        else
        {
            err = true;
        }
    }
    else{
        err = true;
    }

    if(err == true)
    {
        char send[] = "INVALID_LEVEL\n" ;
        PC_Monitor_SendResponse(send, strlen(send));
    }    
}
 static int PC_Monitor_GetSpeakerLevelCommand(void)
{
    SYS_PRINT("Handle command get speaker level\n");
    
    char send[40] = "GET_SPEAKER:";
    char str[10];
    sprintf(str, "%d\n", gs_PCMonitoringData.speakerVolume);
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
}

 static int PC_Monitor_GetBrightnessSensorValueCommand(void)
{
    //TODO
    SYS_PRINT("Handle command get brightness sensor value\n");
    
    char send[40] = "GET_BRIGHTNESS_SENSOR:";
    char str[10];
    float sensorVolt = 0;
    ADC_GetVoltage(ADC_LIGHT_SENSOR, &sensorVolt);
    sprintf(str, "%.4f Volt\n", sensorVolt);
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
}

 static int PC_Monitor_GetAccelerationSensorValueCommand(void)
{
    //TODO
    SYS_PRINT("Handle command get acceleration sensor\n");
    
    char send[40] = "GET_ACCELERATION_SENSOR:";
    char str[30];
    sprintf(str, "xAngel:%.0f, yAngle:%.0f\n", gs_PCMonitoringData.xAngleDirection, gs_PCMonitoringData.yAngleDirection);
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
}
 static int PC_Monitor_GetWaterLevelSensorValueCommand(void)
{
    //TODO
    SYS_PRINT("Handle command get tank water level\n");
    char* send;
    switch (gs_PCMonitoringData.waterLevel)
    {
        case eTankIsDisconnected:
            send = "GET_WATERLEVEL_SENSOR:DISCONNECT\n";            
            break;
        case eTankIsNotFull:
            send = "GET_WATERLEVEL_SENSOR:NOTFULL\n"; 
            break;
        case eTankIsFull:
            send = "GET_WATERLEVEL_SENSOR:FULL\n"; 
            break;
        case eTankIsUnknow:
            send = "GET_WATERLEVEL_SENSOR:UNKNOW\n";
            break;
        default:
            return;
    }
    PC_Monitor_SendResponse(send, strlen(send));
}
 static int PC_Monitor_GetBlowerSpeedCommand(void)
{
    SYS_PRINT("Handle command get blower speed\n");
    
    char send[40] = "GET_BLOWERSPEED:";
    char str[10];
    sprintf(str, "%d\n", gs_PCMonitoringData.blowerSpeed);
    strcat(send, str);
    PC_Monitor_SendResponse(send, strlen(send));
}


/** @brief Update current  value of all alarm monitor for device task
*  @param [in] None
*  @param [out] : None
*  @return None
*/
void PC_Monitor_UpdateMonitorData(void)
{
   DeviceTask_GetPCMonitorStruct(&gs_PCMonitoringData);
}


/** @brief Initialize PC monitor use to communicate with PC app by opening a COM
* port (UART6)
* This function should be called 1 times when start the application
*  @param [in] None
*  @param [out] None
*  @return None
*/
void PC_Monitor_processCommandAndResponse()
{
    if (s_currentCommand <= LAST_COMMAND)
    {
        if (commandTable[s_currentCommand].cmdFnc)
        {
            commandTable[s_currentCommand].cmdFnc();
        }
        else
        {
            SYS_PRINT("commandTable null function pointer \n");
        }
    }
    else
    {
        char send[] = "COMMAND_UNSUPPORT\n";
        PC_Monitor_SendResponse(send, strlen(send));
    }
}

/** @brief Initialize PC monitor use to communicate with PC app by opening a COM
* port (UART6)
* This function should be called 1 times when start the application
*  @param [in] None
*  @param [out] None
*  @return None
*/


uint8_t PC_Monitor_DecodeCommand(char *commandArray, uint8_t commandLength )
{
   int i;
   //scan defined command list 
   for (i = 0; i < NUM_OF_COMMAND; i++)
   {
        //find the command from defined List Command that has same length with received command
        uint8_t lengthDefinedCMD = strlen(commandTable[i].commandArray);
        //printf("lengthDefinedCMD %d\n", lengthDefinedCMD);
                
        //must compare length before comparing memory
        if(lengthDefinedCMD == commandLength)
        {
            //compare memory
            int8_t result = memcmp(&commandArray[0], &commandTable[i].commandArray[0], commandLength);
            
            if(result == 0)//found the command
            {
               //SYS_PRINT("index %d\n", i);
               return commandTable[i].commandCode;
            }
            else //cannot find the command
            {
               //SYS_PRINT("cmp result %d\n", result);
            }
        }
   }
   //SYS_PRINT("index %d\n", i);
   return UNSUPPORT;
}


/** @brief Initialize PC monitor use to communicate with PC app by opening a COM
* port (UART6)
* This function should be called 1 times when start the application
*  @param [in] None
*  @param [out] None
*  @return None
*/
void PC_Monitor_Initialize() {
   Uart2_Initialize();
}

/** @brief Initialize PC monitor use to communicate with PC app by opening a COM
* port (UART6)
* This function should be called 1 times when start the application
*  @param [in] None
*  @param [out] None
*  @return None
*/
static void PC_Monitor_SendResponse(uint8_t* dataArray, uint32_t dataSize) {
    Uart2_Send(dataArray, dataSize);
}

/** @brief Send a packet of data through UART6 to PC app
*  Make sure the PC_Monitor_Initialize() function has called before calling 
* this function
*  @param [in]  float airFlow: air flow obtained from Air flow sensor
*               float o2Flow:  O2 flow obtained from O2 flow sensor 
*               float totalFlow: total flow, including Air + O2, after bypass a LPF
*               float percentPower: current power for Motor, from 0% to 100%
*  @param [out]  None
*  @return None
*/
//static void PC_Monitor_Send(float* dataArray, uint32_t dataSize) {
//    int8_t* ptr;
//    int8_t index = 0;
//
//    //make data
//    uint8_t data[PC_MONITOR_SEND_PACKET_MAX_LEN];
//    data[index++] = PC_MONITOR_HEADER; //header
//    data[index++] = dataSize*2; //data length, each item has 2 byte size
//    
//    int i = 0;
//    for (i = 0; i < dataSize; i++) {
//        int16_t itemData = dataArray[i] * PC_MONITOR_SCALE;
//        ptr = (int8_t*) & itemData;
//        data[index++] = (int8_t) (*ptr);
//        data[index++] = (int8_t) (*(ptr + 1));
//    }
//    
////    uint16_t crc = crc_CheckNoInit(index, &data[0]);
////    ptr = (int8_t*) & crc;
//    uint8_t crc = caculateCRC8();
//    data[index++] = (int8_t) (*ptr);
//    data[index++] = (int8_t) (*(ptr + 1));
//
//    //push data through UART 6
//    Uart2_Send(&data[0], index);
//}

/** @brief Handle data from PC APP to JFLOW via UART 6
*  This function should be called periodically in a Task
*  @param [in]   None
*  @param [out]  None
*  @return None
*/
static bool PC_Monitor_HandleReceiveData() 
{
    static E_ParseDataState currentState = eReadyForReceive;
//     static int16_t dataLength = 0;
    static int16_t packetLength = 0;
    static int16_t packetIndex = 0;
    static uint8_t receiveBuffer[PC_MONITOR_RECEIVE_PACKET_MAX_LEN] = {0};
    uint8_t data[256] = {};
    int16_t receivedByte = 0;
    int16_t i = 0;
    int16_t commandLength = 0;
    int16_t contentLength = 0;
    uint16_t contentIndex = 0;
    
    uint8_t command[COMMAND_LENGTH_MAX] = {};
//    uint8_t commandContent[COMMAND_CONTEND_LENGTH_MAX] = {};

  
    switch (currentState) 
    {
        case eReadyForReceive:
            
            packetIndex = 0;
            packetLength = 0;
            currentState = eWaitforReceiveDone;
            break;
            
        case eWaitforReceiveDone:
            
            //get data from UART buffer
            receivedByte = Uart2_ReadReceiveBuffer(&data[0], sizeof(data));
            if ((receivedByte <= 0) ||(receivedByte >= PC_MONITOR_RECEIVE_PACKET_MAX_LEN))
            {
                break;
            }
            //receive data
            SYS_PRINT("received Bytes %d\n", receivedByte);
            SYS_PRINT("received Data %.20s\n", data);
            for (i = 0; i < receivedByte; i++)
            {   
                uint8_t ch = data[i];
                //end character
                if((ch == '\r') || (ch == '\n'))
                {
                    currentState = eParseData;
                }
                else//data
                {
                    receiveBuffer[packetIndex++] = ch;
                    packetLength++;
                    if(packetIndex >= PC_MONITOR_RECEIVE_PACKET_MAX_LEN)
                    {
                        //error- received bytes is over buffer size
                        //-->reset receive state
                        currentState = eReadyForReceive;
                    }
                }
            }
            break;
            
        case eParseData:
            
            currentState = eReadyForReceive;
            //parse data
            //find ':' character to get command and content from received data
            for (i = 0; i < packetLength; i++) 
            {
                if(receiveBuffer[i] == ':')
                {
                    commandLength = i;
                    contentIndex = i+1;//skip ':' character
                    contentLength = packetLength - commandLength - 1; //minus 1 to skip ':' character   
                    break;
                }
                if((receiveBuffer[i] >= '0') && (receiveBuffer[i] <= '9'))                        
                {
                    if((memcmp(receiveBuffer, "GET_ALARM", 9) == 0)
                            ||(memcmp(&receiveBuffer, "GET_LOG", 7) == 0))//FIXME
                        /* this condition include case GET_ALARM, GET_LOG and exclude case of GET_O2_FLOW, GET_SPO2, SET_CAL1, SET_CAL2 */
                    {                    
                        commandLength = i;
                        contentIndex = i;
                        contentLength = packetLength - commandLength; 
                        break;
                    }
                }
                commandLength = i + 1;
            }
                      
            SYS_PRINT("commandLength %d\n", commandLength);
            
            if (commandLength <= COMMAND_LENGTH_MAX) // command length is valid
            {
                memcpy(&command[0], &receiveBuffer[0], (size_t)commandLength);
                SYS_PRINT("command: %s\n", command);

                //Decode received command                    
                s_currentCommand = PC_Monitor_DecodeCommand(command, commandLength);
                SYS_PRINT("Command CODE: %d\n", s_currentCommand);
                
                if(s_currentCommand == UNSUPPORT)
                {
                    SYS_PRINT("command is not supported\n");
                    s_currentCommand = UNSUPPORT;
                }
                
                   
                if(contentLength > 0)
                {
                    SYS_PRINT("contentLength %d\n", contentLength);
                    if(contentLength <= COMMAND_CONTEND_LENGTH_MAX)
                    {
                        memcpy(&s_commandContent[0], &receiveBuffer[contentIndex], contentLength);//add 1 due to ':' character  
                        SYS_PRINT("command content %s\n", s_commandContent);
                    }
                    else{
                        SYS_PRINT("command contend too long\n");
                    }
                }
                else{
                    SYS_PRINT("command has no content\n");
                }
                s_commandContentLen = contentLength;
                               
            }
            else{
                // command length is invalid
                SYS_PRINT("command is not supported due to too long\n");
                s_currentCommand = UNSUPPORT;
            }
            return true;
  
            break;
            
        default:
            currentState = eWaitforReceiveDone;
            break;
    }
    
    return false;
}

/** @brief Initialize PC monitor use to communicate with PC app by opening a COM
* port (UART6)
* This function should be called 1 times when start the application
*  @param [in] None
*  @param [out] None
*  @return None
*/
//static void PC_Monitor_HandleTransmitData() {
//      //1. get IH current power
//    HEATER_PUBLIC_DATA_t heaterData;
//    MOTOR_PUBLIC_DATA_t motorData;
//
//    //get heater data
//    if (HeaterTask_GetPublicData(&heaterData) == false) {
//        return;
//    }
//
//    //get motor data
//    if (MotorTask_GetPublicData(&motorData) == false) {
//        return;
//    }
//    
//    float dataToSend[8];
//    dataToSend[0] = heaterData.currentPower;
//    dataToSend[1] = heaterData.targetPower;
//   
//    
//    dataToSend[2] = heaterData.chamberOutTemp;
//    dataToSend[3] = heaterData.chamberOutTargetTemp;
//    
//    dataToSend[4] = heaterData.envTemp;
//    dataToSend[5] = heaterData.envHum;
//    
//    
//    dataToSend[6] = heaterData.bumperFrequency;
//    dataToSend[7] = motorData.speed;
//    
//    PC_Monitor_Send(&dataToSend[0], sizeof (dataToSend));
//}


/** @brief State machine for receive command from PC and Response to PC
* 
*  @param [in] None
*  @param [out] None
*  @return None
*/
void PC_Monitor_Handle()
{
   switch(s_CommandHandle)
   {
   case eReadCommandAndParse:
       //PC_Monitor_ReceiveAndProcessData();
       if(PC_Monitor_HandleReceiveData() == true)
       {
           s_CommandHandle = eProcessCommandAndResponse;
       }
       break;
   case eProcessCommandAndResponse:
       PC_Monitor_UpdateMonitorData();
       PC_Monitor_processCommandAndResponse();
       s_CommandHandle = eReadCommandAndParse;
       break;
   }
}

/* *****************************************************************************
End of File
*/
