/* ************************************************************************** */
/** @file [AlarmInterface.c]
 *  @brief {Contains functions which are ALarm interface}
 *  @author {Cuong Le}
 */
/* ************************************************************************** */
/* History                                                                    */
/*============================================================================*/
/* [Number]     [Date]              [Editor]                    [Explaination]*/
/*-----------+-------------------+---------------------+----------------------*/
/*  001          01/19/2021         Thanh Duong             modify            */
/* Content: Change Data type: self_check flag from E_Flag to bool
 *          mainUnitDisFlag, isACConnected, isChamberConnected,
 *          spo2Connected, isOKPress                  */
/* ************************************************************************** */
#ifndef ALARMINTERFACE_H
#define	ALARMINTERFACE_H

#include "system_config.h"
#include "system_definitions.h"
#include "FreeRTOS.h"
#include "queue.h"

#include "ChamberUnit.h"
#include "Setting.h"
#include "HeaterTask.h"

#ifdef UNIT_TEST
#include "externaltype.h"
#endif

/** @brief Declare queue alarm waiting */
#define QUEUE_ALARM_WAITING 							(2)

/** @brief Declare queue receive */
//extern QueueHandle_t g_alarmQueueReceive; // FIXME: move to c file ?

/** @brief Notice the state of the errors which are identified in the other file (true or false) */
typedef enum
{
    Flag_Off,// Flag_On if the system defines the error. 
    Flag_On// Flag_On if the system does not define the error or the user fixed it.
} E_Flag;

/** @brief list of Sp02 messages which is received */
typedef enum
{
    spo2OK,
    spo2SensorUnplugged,
    spo2NoFingerOrSensorProblem,   
    spo2Searching,
    spo2SearchingTooLong,
    spo2LostPulse,
    spo2Artifact    
} E_Spo2Signal;


/** @brief Struct of Sp02 Informations which are received to use for alarm process*/
typedef struct
{
    E_Spo2Signal spo2Message;
    float spo2Value;
    float spo2SetLimit;
    bool spo2Connected;    //Flag_On if SpO2 sensor is connected, Flag_Off if SpO2 sensor is removed from device - changed from E_Flag to bool
} SPO2_ALARM_t;

/** @brief Struct of Oxy Informations which are received */
typedef struct
{
    float o2Concentration;
    float maxO2RangeSetting;
    float minO2RangeSetting;
} O2_ALARM_t;

/** @brief Struct of Battery informations which are received*/
typedef struct
{
    float B1;   /**<main unit battery lever*/
    float B2;   /**<Cradle battery lever*/
    float L0;   /**<Battery lever which can't remain function anymore and can't supply power for MUC and LCD operation*/ 
    float L1;   /**<Battery lever which can't remain function anymore but can supply power for MUC and LCD operation*/ 
    float L2;   /**<Battery lever which can remain function in 5 min*/
    float L3;   /**<Battery lever which can remain function in 15 min*/
} BATTERY_ALARM_t;

/** @brief List of system posture*/
typedef enum
{
    Abnormal,
    Bad,
    Normal
} E_Posture;

/** @brief List all alarm ID */
typedef enum {
    eFirsAlarmId = 0, 
    eFirstHardwareAlarm = eFirsAlarmId, //TODO: is neccessary ??
    eI2C1ErrorAlarm = eFirstHardwareAlarm,
    eI2C2ErrorAlarm,
    eI2C3ErrorAlarm,
    eI2C4ErrorAlarm,

    eSPI3ErrorAlarm,

    eUart1ErrorAlarm,
    eUart2ErrorAlarm,
    eUart4ErrorAlarm,
    eUart6ErrorAlarm,


    eAirFlowSensorErrorAlarm,//9
    eO2FlowSensorErrorAlarm,
    eBME280ErrorAlarm,
    eADXL345ErrorAlarmID,        
    eDRV8308ErrorAlarm,
    eAudioErrorAlarm,
            
    eMotorTaskErrorAlarm,
    eHeaterTaskErrorAlarm,//16
    eHumidityTaskErrorAlarm,
        
    //Respiratory circuit abnormality
    eBreathingCircuitNotConnectedAlarmId,  /**<The breathing circuit is not connected or not connected firmly to the device */
    eCheckLeakAlarmId, /**<Disconnected between breathing circuit and cannula, Or 80% of breathing circuit breaks */
    eCheckBlockageAlarmId, /**<Filter, breathing circuit or cannula blocked */
    eNonGenuineCircuitAlarmId, //21 /**<Non-genuine circuit*/
    eBreathingCircuitChangedAlarmId, /**<Tube check*/                // chua lam
    eExpiredCircuitAlarmId, /**<Expired circuit */
    
    //Chamber abnormality
    eCheckConnectionChamberAlarmId,//24 /**<Chamber is not connected or not connected firmly to the device */
    eNoMoreWaterInChamberAlarmId, /**<There is no more water in the chamber */
    eRunOutOfWaterAlarmId,        /**<Run out of water*/
            
    //temperature abnormality
    eLowTemperatureAlarmId, /**<Alarm when the temperature is too low (<18*C)*/
    eHighTemperatureAlarmId,/**<Alarm when the ambient temperature higher than the setting setting temperature */
    eHighTemperatureAbnormalityAlarmId,//29 /**<Alarm when the calculated mouth temperature at 1 sampling >= 42*C */
            
    //Outside the environment
    eRoomTemperatureLowToAchieveTargetTemperatureAlarmId, /**<Alarm when the mouth temperature did not reach to the target temperature after 30 minute running from started operation and the temperature of environment is lower than specification environment */
    eCheckOperatingConditionsAlarmID, /**<Alarm check operating condition */
    eDeviceErrorToAchieveTargetTemperatureAlarmId,        /**<Alarm when the mouth temperature did not reach to the target temperature after 30 minute running from started operation and the operating environment temperature is within the specified ambient temperature range*/        
    
    //Abnormal oxygen concentration
    eOxygenHighAlarmId, /**<Oxygen concentration is higher than the alarm setting upper limit (The lower and upper limits of the default setting values are 21% and 60%)*/
    eOxygenLowAlarmId,  /**<Oxygen concentration is lower than the alarm setting upper limit(The lower and upper limits of the default setting values are 21% and 60%)*/
    
    //Device position
    eDevicePostureAbnormalAlarmId, /**<Alarm when the installation orientation of the device is recognized upside down in TBD seconds (Definition of upside down will decide later)*/
    eDevicePostureBadAlarmId, /**<TBD*/
            
    //Battery
    eSwitchToBatteryModeAlarmId,//37 /**<Switch to battery Mode*/
    eBatteryLowAlarmId, /**<Battery low*/
    eBatteryGetsRunOutAlarmId, /**<Battery gets run out*/
    eStopFunctionAlarmId, /**<Stop function*/
    ePowerNotEnoughAlarmId, /**< Power is not enough: AC not connected and L0 < B1+B2 ??L1 */
            
    //SpO2
    eSpO2FailedAlarmId, /**< Alarm when receive a SEARCHING TOO LONG or ARTIFACT signal from the Spo2 device*/
    eSpO2LowAlarmId, /**< Spo2 value is lower than the alarm setting lower limit (lower limit is 85% or more)*/
    eSpO2SensorProbeAlarmId, /**< Alarm when receive a NO FINGER OR SENSOR PROBLEM signal from the Spo2 device*/
    eSpO2SignalLossAlarmId, /**< Alarm when receive a LOST PULSE signal from the Spo2 device*/
    eSpO2SensorUnpluggedAlarmId, /**< Alarm when receive a SENSOR UNPLUGGED signal from the Spo2 device*/
            
    //Water
    eWaterSupplyCheckAlarmId,//47 /**<Water supply check */
    eWaterTankConnectionCheckAlarmId,/**<Water tank connection check */
            
    eAirFlowAbnormalAlarmId, /**<Air flow abnormal */
    eMainUnitBatteryDisconnectedAlarmId,/**<Main unit battery disconnected */
    eCradleBatteryDisconnectedAlarmId,/**<Cradle battery disconnected */
                      
                     
    //Internal failure
    eFailureOxygenFlowMeasurementFlowSensorAlarmId,//52 /**<Failure of Oxygen flow measurement flow sensor */
    eFailureAirFlowMeasurementFlowSensorAlarmId, /**<Failure of Air flow measurement flow sensor */
    eFailureChamberTemperatureSensorAlarmId, /**<Failure of chamber temperature sensors */
    eFailureCoilTemperatureSendorAlarmId, /**<Failure of Coil temperature sensor */
    eFailureBreathingCircuitOutTemperatureSensorAlarmId, //56 /**<Failure of Breathing circuit out temperature sensor*/
    eFailureEnvironmentSensorAlarmId, /**<Failure of Environment sensor  (Temp, hum, pressure)*/
    eFailureCurrentSensorAlarmId, /**<Failure of Current sensor */
    eFailureBlowerAlarmId, /**<Failure of Blower */
    eFailureWaterLevelSensorAlarmId, /**<Failure of water level sensor */
    eFailureExternalFlashMemoryAlarmId, /**<extrnal flash memory failed */
    eFailureAccelerationSensorAlarmId, /**<Failure of acceleration sensor */
    eFailureLightSensorAlarmId, /**<Failure of light sensor */
    eSpeakerDisconnectedAlarmId,
    // Hiep add
    eMainUnitBatteryCommunicationErrorAlarmId, //65 /**< Main unit battery communication error */
    eCradleBatteryCommunicationErrorAlarmId, /**<Cradle battery communication error */
    eCradleCommunicationErrorAlarmId, /**<Cradle communication error */
    eMainMCUFailedOrOutOfControlAlarmId, /**<Main MCU failed or out of control */    
    eESP32FailedAlarmId, /**<ESP32 Failed */
    eBreathingCircuitHeaterWireBrokenAlarmId, /**<Breathing circuit heater wire broken */
    eFailureSpo2ModuleAlarmId, /**<Failure of spo2 module */
    eFailureRTCModuleAlarmId, /**<Failure of RTC module */
    eFailureLCDTouchModuleAlarmId, /**<Failure of LCD touch module */
    eChamberTemperatureAbnormalAlarmId, /**<Chamber temperature abnormal */
    eTooMuchWaterInTheChamberAlarmId, /**<Too much water in the chamber */
    eLastAlarmId,
    eNoneAlarmId = eLastAlarmId,
    eNoOfAlarmId = eLastAlarmId
} E_AlarmId;

/** @brief Define alarm status */
typedef enum {
    eInactive, /**< inactive status */
    ePaused, /**< paused status */
    eWaitingToActive, /**< waiting to active status */
    eActive, /**< active status */
    eNoOfAlarmStatus /**< number of status */
} E_AlarmStatus;

/** @brief Define alarm status */
typedef enum {
    eNone = 0,
    eLow1Priority,
    eLow2Priority,
    eMediumPriority,
    eHighPriority,
} E_AlarmPriority;

/** @brief Define Water tank status */
typedef enum {
    eFullOfWater,
    eNotFullOfWater,
} E_WaterTankStatus;

/** @brief Definition of alarm checking function */
typedef void (*ALARM_CHECK_FNC)(void);

/** @brief Definition of structures */
typedef struct {
    E_AlarmId ID; /**<id of alarm */
    E_AlarmStatus previousStatus; /**<previous Status */
    E_AlarmStatus currentStatus;  /**<current Status */
    E_AlarmPriority previousPriority;/**<previous priority of alarm */
    E_AlarmPriority currentPriority;
    uint8_t data[5];
#ifdef FUNCTION_DISABLE_ALARM
    E_AlarmOperationMode mode;
    ALARM_CHECK_FNC checkAlarmFnc;
#endif
} ALARM_STAT_t;

/** @brief definition of structures */
typedef struct {
    
    bool BME280SensorErr;
    bool ADXL345SensorErr;
    bool AirFlowSensorErr;
    bool O2FlowSensorErr;
	//E_ModeSetting operationMode; //current mode
    E_BreathingCircuitType BreathingCircuitType; /**<The Type of Breathing Circuit */
    float setFlow;                        /**<Setting Flow */
    float currentFlow;                    /**<Current air flow */
    float currentO2Flow;                    /**<Current O2 flow */
    float currentIHPower;                 /**<Current IH power */
    float setTargetPower;                 /**<Set target power */
    float currentBlowerRotationSpeed;     /**<blower rotation speed */
    float currentMouthTemperature;        /**<Mouth temperature */
    float setTemperature;                 /**<Setting temperature */
    float setMouthTemperature;            /**<Target mouth temperature */
    float currenAmbientTemperature;       /**<Environment temperature */
    uint8_t breathingCode;                  /**<Manufactory code of breathing Circuit device (Metrancode) */
    O2_ALARM_t o2Info_struct;                /**<Oxy Informations which are received */
    float xAngleDirection;                   /**<X Angle of device which is detected by accelerate sensor*/
    float yAngleDirection;                   /**<Y Angle of device which is detected by accelerate sensor*/
    uint32_t dateManufactureBreathingCircuit;   /**<Date of manufacture */
    uint32_t dateCurrentBreathingCircuit ;      /**<Current day */
    BATTERY_ALARM_t batteryInfo_struct;      /**<Battery informations which are received */
    bool isACConnected;                /**<Flag is raised when AC power is connected */
    bool isChamberConnected;          /**<Flag is raised when Main board couldn't communicate with Chamber 3 times in a row */
    SPO2_ALARM_t spo2Info_struct;            /**<Sp02 Informations which are received */
//    uint8_t failureO2FlowSensorCounter;      /**<the number of the trying to reconnect O2 flow sensor */
//    uint8_t failureAirFlowSensorCounter;     /**<the number of the trying to reconnect Air flow sensor */
//    uint8_t failureChamberTemperatureSensorCounter;         /**<the number of the trying to reconnect chamber temperature sensors */
    float coilTemperatureSensorTemp;                        /**<The current voltage of coil temperature sensor */
    float breathingCircuitTemperatureSensorTemp;            /**<The current voltage of breathing Circuit temperature sensor */     
    bool self_check;                      /**< self-check at startup - Changed from E_Flag to bool type*/ 
    E_BreathingCircuitType log_BreathingCircuitType;    /**< Breathing Circuit Type is saved in log */
    E_BreathingCircuitType curent_BreathingCircuitType; /**< The current Breathing Circuit Type */
    E_TankWaterLevel waterTankStatus; /**<The current water tank status */
    // Add by Thanh Duong
    bool isBreathingCircuitHeaterWireBroken; /**< Check Heater wire broken */
    bool isCradleConnected;/**<Check Cradle connection state */
//    bool isFailureCurrentSensor;/**<Check current sensor fail state */
    bool isFailureBlower;/**<Check Blower fail state */
    bool isFailureWaterLevelSensor;/**<Check water level fail state */
    bool isFailureExternalFlashMemory;/**<Check external flash memory fail state */
    bool isFailureLightSensorAlarmId;/**<Check light sensor fail state */
    bool isFailureMainUnitBatteryCommunication;/**<Check main unit battery communication  state */
    bool isFailureCradleBatteryCommunication;/**<Check Battery communication error state */
    bool isFailureMainRunOutofControl;/**<Check Main run out of control state */
    bool isESP32Failed;/**<Check ESP32 fail state */
    bool isFailureAccelerationSensor;/**<Check Acceleration sensor fail state */
    bool failureBlower;/**<Check blower fail state */
    bool isFailureCradleCommunicationError;/**<check cradle communication fail */
    float chamberTemperatureSensorTemp;/**<Check temperature state */
    float currentSensor1Volt;
    float currentSensor2Volt;
    float blowerControlValue;
    float breathingCircuitPowerControl;
    E_TreatmentMode machineMode;
    bool isIHOperating;
    bool isMainUnitBatteryConnected;
    bool isCradleBatteryConnected;
    bool isCradleBattConnected;
    int batteryRemainingTimeInMin;
    E_WarmingUpStatus warmingUpStatus;
    bool isSpeakerBrokenOrDisconnected;
    bool isBreathingCircuitConnected;
    bool isSpo2ModuleFailure; /**<Spo2 module failure */
    bool isRTCModuleFailure; /**<RTC module failure */
    bool isLCDTouchModuleFailure; /**<LCD Touch module failure */
    float piezoControlFreq; /**<Piezo control frequency */
    float piezoControlFreqUpperLimit; /**<Piezo control frequency upper limit */
}ALARM_MONITOR_t;

enum E_AlarmData_AddtionalMessage{
    eDisableAdditionalMessage = 0, /**< Disable additional message */
    eEnableAdditionalMessage = 1, /**< Enable additional message */
};
enum E_AlarmData_E005{
    eE005AdultCircuitToPediatricMode, /**< Adult circuit connected to pediatric mode */
    eE005PediatricCircuitToAdultMode, /**< Pediatric circuit connected to adult mode */
    eE005AdultCircuitToDisinfectionMode, /**< Adult circuit connected to disinfection mode */
    eE005PediatricCircuitToDisinfectionMode, /**< Pediatric circuit connected to disinfection mode */
    eE005DisinfectionCircuitToAdultMode, /**< Disinfection circuit connected to adult mode */
    eE005DisinfectionCircuitToPediatricMode, /**< Disinfection circuit connected to pediatric mode */
};
enum E_AlarmData_E010{
    eE010AmbientTempLowerThan18CelsiusDegree, /**<  Ambient temp lower than 18 celsius degree */
    eE010AmbientTempEqualOrOverThan18CelsiusDegree, /**<  Ambient temp equal or over than 18 celsius degree */
};

enum E_AlarmData_E011{
    eE011AmbientTempLowerThanSettingTemp,  /**<  Ambient temp lower than setting temp */
    eE011AmbientTempEqualOrOverThanSettingTemp, /**<  Ambient temp  equal or over than setting temp */
};

enum E_AlarmData_E014{
    eE011AmbientTempLowerThan42CelsiusDegree, /**<  Ambient temp lower than 42 celsius degree */
    eE011AmbientTempEqualOrOverThan42CelsiusDegree, /**<  Ambient temp lower than 42 celsius degree */
};

enum E_AlarmData_ResetButtonStatus{
    eResetButtonDefault, /**< ResetButton displayed as default config in UI  */
    eResetButtonHide, /**< ResetButton not displayed (override default config in UI)  */
    eResetButtonShowReset, /**< ResetButton displayed as Reset button (override default config in UI)  */
    eResetButtonShowOK, /**< ResetButton displayed as OK button (override default config in UI)  */
};

enum E_AlarmDataIndex{
    eAdditionalMessageDataIdx = 0,
    eMessageTypeDataIdx = 1,
    eResetButtonStatusDataIdx = 2,
};

/** @brief Send the alarm message to GUI task  
 *  @param [in] ALARM_STAT_t alarm: alarm struct
 *  @param [out] None
 *  @return bool : true if event was sent successful, false is event was sent failed
 */

/*
static inline bool alarmInterface_SendEvent(ALARM_STAT_t alarm) 
{
    //Return value
    bool rtn = true;

    if (xQueueSendToBack(g_alarmQueueReceive, &alarm, QUEUE_ALARM_WAITING) != pdPASS) {
        SYS_PRINT("\n send failed: event to log task");
        xQueueReset(g_alarmQueueReceive);
        rtn = false;
    }

    return rtn;
}
 */
#ifndef UNIT_TEST
inline bool alarmInterface_SendEvent(E_AlarmId id, E_AlarmStatus status, E_AlarmPriority priority, uint8_t *data);
#else
bool alarmInterface_SendEvent(E_AlarmId id, E_AlarmStatus status, E_AlarmPriority priority, uint8_t *data);
#endif


//static inline bool alarmInterface_SendEventFromISR(E_AlarmId id, E_AlarmStatus status, int32_t data) 
//{
//    //Return value
//    bool rtn = true;
///*
//    if (xQueueSendToBack(g_alarmQueueReceive, &alarm, QUEUE_ALARM_WAITING) != pdPASS) {
//        SYS_PRINT("\n send failed: event to log task");
//        xQueueReset(g_alarmQueueReceive);
//        rtn = false;
//    }
//*/
//    return rtn;
//}
#endif	/* ALARMINTERFACE_H */

/* *****************************************************************************
 End of File
 */
