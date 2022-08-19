/** @file Setting.h
 *  @brief Setting contains all setting for JlowMini.
 *  Setting provides the interface to read and write settings in JFlo Mini
 *  @author Viet Le
 */
#ifndef SETTING_H
#define	SETTING_H

#include "stdint.h"

#ifndef UNIT_TEST
#include "system_config.h"
#include "system_definitions.h"
#endif

#ifdef UNIT_TEST
#define language_Japanese 0
#define language_English 1
#define language_Vietnamese 2
#endif

#define FUNCTION_DISABLE_ALARM

/** @brief define step for flow setting in child mode */
#define STEP_FLOW_SETTING_IN_CHILD_MODE                 (1)

/** @brief define min for flow setting in child mode */
#define MIN_FLOW_SETTING_IN_CHILD_MODE                  (10)

/** @brief define max for flow setting in child mode */
#define MAX_FLOW_SETTING_IN_CHILD_MODE                  (25)

/** @brief define step for flow setting in adult mode */
#define STEP_FLOW_SETTING_IN_ADULT_MODE                 (5)

/** @brief define min for flow setting in adult mode */
#define MIN_FLOW_SETTING_IN_ADULT_MODE                  (15)

/** @brief define max for flow setting in adult mode */
#define MAX_FLOW_SETTING_IN_ADULT_MODE_AC_POWER         (60)

/** @brief define max for flow setting in adult mode */
#define MAX_FLOW_SETTING_IN_ADULT_MODE_BATTERY_POWER    (40)

/** @brief define step for temp setting in child mode */
#define STEP_TEMP_SETTING_IN_CHILD_MODE                 (1)

/** @brief define min for temp setting in child mode */
#define MIN_TEMP_SETTING_IN_CHILD_MODE                  (34)

/** @brief define max for temp setting in child mode */
#define MAX_TEMP_SETTING_IN_CHILD_MODE                  (34)

/** @brief define step for temp setting in adult mode */
#define STEP_TEMP_SETTING_IN_ADULT_MODE                 (1)

/** @brief define min for temp setting in adult mode */
#define MIN_TEMP_SETTING_IN_ADULT_MODE                  (31)

/** @brief define max for temp setting in adult mode */
#define MAX_TEMP_SETTING_IN_ADULT_MODE                  (37)

/** @brief define 2 discrete of digital GPIO on chip */
typedef enum {
    eSettingOff = 0,
    eSettingOn
} E_OnOff;

/** @brief define oxygen source  */
typedef enum {
    e90OxygenSource, /**< 90 percent oxygen source */
    e100OxygenSource, /**< 100 percent oxygen source*/
    eNoOfOxygenSource
} E_OxygenSource;


/** @brief define all language that Swell machine supported: here are English and Japanese */
typedef enum {
    eJapanese = language_Japanese, /**<Japanese language. */
    eEnglish = language_English, /**<English language. */
    eVietnamese = language_Vietnamese, /**<VietNam language. */
    eNoOfLanguage,
} E_Language;

/** @brief define treatment mode */
typedef enum {
    eAdultMode, /**<Adult mode */
    eChildMode,/**<Child mode */
    eDisinfectionMode,
    eDryMode,
} E_TreatmentMode;

/** @brief define brightness mode */
typedef enum {
    eAutoBrightnessMode, /**<auto mode */
    eManualBrightnessMode,/**<manual mode */
    eNoOfBrightnessMode,
} E_BrightnessMode;

/** @brief define room mode */
typedef enum {
    eHomeMode, /**<Home mode */
    eHospitalMode/**<Hospital mode */
} E_RoomMode;

/** @brief define room mode */
typedef enum {
    eDarkMode, /**<Home mode */
    eLightMode/**<Hospital mode */
} E_Background;

/** @brief define alarm disable state */
typedef enum {
    eAlarmEnable, /**<Enable mode */
    eAlarmDisable,/**<Disable mode */
} E_AlarmOperationMode;

/** @brief define setting id */
typedef enum {
    eFirstSettingId = 0, /**<First setting */  
    eLanguageSettingId = eFirstSettingId, /**<Language setting */
    eBluetoothSettingId, /**<Bluetooth setting */
    eWifiSettingId, /**<Wifi setting */
    eBrightnessLevelSettingId, /**<Brightness level setting */
    eBrightnessModeSettingId, /**<Brightness mode setting */        
    eTreatmentModeSettingId, /**<ventilation mode setting (Adult or Child) */
    eSpeakerVolumeSettingId, /**< speaker's volume */
    eRoomModeSettingId, /**<ventilation mode setting (Home or Hospital) */
    eTempSettingId, /**<Temperature setting */
    eFlowSettingId, /**<Flow setting */
    eFiO2SettingId, /**<FiO2 setting */
    eSpO2AlarmSettingLowerLimitId,/**<SPO2 alarm setting lower limit */
    eOxygenSourceSettingId,/**<Oxygen alarm setting */
    eHourSettingId,/**<Oxygen alarm setting */
    eMinuteSettingId,/**<Oxygen alarm setting */
    eSecondSettingId,/**<Oxygen alarm setting */
    eDateSettingId,/**<Oxygen alarm setting */
    eMonthSettingId,/**<Oxygen alarm setting */       
    eYear1SettingId,/**<Oxygen alarm setting */  
    eYear2SettingId,/**<Oxygen alarm setting */              
    eOxygenAlarmSettingUpperLimitId, /**< Oxygen alarm setting upper limit */
    eOxygenAlarmSettingLowerLimitId, /**< Oxygen alarm setting lower limit */
    eAdultModeTempSettingId,
    eChildModeTempSettingId,
    eAdultModeFlowSettingId,
    eChildModeFlowSettingId,
    eCalibrationGainAirSettingId, 
    eCalibrationOffsetAirSettingId,
    eCalibrationGainO2SettingId,
    eCalibrationOffsetO2SettingId,
            
#ifdef FUNCTION_DISABLE_ALARM
    eFirsAlarmOperationModeSettingId, 
    eFirstHardwareAlarmOperationModeSettingId = eFirsAlarmOperationModeSettingId, //TODO: is neccessary ??
    eI2C1ErrorAlarmOperationModeSettingId = eFirstHardwareAlarmOperationModeSettingId,
    eI2C2ErrorAlarmOperationModeSettingId,
    eI2C3ErrorAlarmOperationModeSettingId,
    eI2C4ErrorAlarmOperationModeSettingId,

    eSPI3ErrorAlarmOperationModeSettingId,

    eUart1ErrorAlarmOperationModeSettingId,
    eUart2ErrorAlarmOperationModeSettingId,
    eUart4ErrorAlarmOperationModeSettingId,
    eUart6ErrorAlarmOperationModeSettingId,


    eAirFlowSensorErrorAlarmOperationModeSettingId,//9
    eO2FlowSensorErrorAlarmOperationModeSettingId,
    eBME280ErrorAlarmOperationModeSettingId,
    eADXL345ErrorAlarmOperationModeSettingId,        
    eDRV8308ErrorAlarmOperationModeSettingId,
    eAudioErrorAlarmOperationModeSettingId,
            
    eMotorTaskErrorAlarmOperationModeSettingId,
    eHeaterTaskErrorAlarmOperationModeSettingId,//16
    eHumidityTaskErrorAlarmOperationModeSettingId,
        
    //Respiratory circuit abnormality
    eBreathingCircuitNotConnectedAlarmOperationModeSettingId,  /**<The breathing circuit is not connected or not connected firmly to the device */
    eCheckLeakAlarmOperationModeSettingId, /**<Disconnected between breathing circuit and cannula, Or 80% of breathing circuit breaks */
    eCheckBlockageAlarmOperationModeSettingId, /**<Filter, breathing circuit or cannula blocked */
    eNonGenuineCircuitAlarmOperationModeSettingId, //21 /**<Non-genuine circuit*/
    eBreathingCircuitChangedAlarmOperationModeSettingId, /**<Tube check*/                // chua lam
    eExpiredCircuitAlarmOperationModeSettingId, /**<Expired circuit */
    
    //Chamber abnormality
    eCheckConnectionChamberAlarmOperationModeSettingId,//24 /**<Chamber is not connected or not connected firmly to the device */
    eNoMoreWaterInChamberAlarmOperationModeSettingId, /**<There is no more water in the chamber */
    eRunOutOfWaterAlarmOperationModeSettingId,        /**<Run out of water*/
            
    //temperature abnormality
    eLowTemperatureAlarmOperationModeSettingId, /**<Alarm when the temperature is too low (<18*C)*/
    eHighTemperatureAlarmOperationModeSettingId,/**<Alarm when the ambient temperature higher than the setting setting temperature */
    eHighTemperatureAbnormalityAlarmOperationModeSettingId,//29 /**<Alarm when the calculated mouth temperature at 1 sampling >= 42*C */
            
    //Outside the environment
    eRoomTemperatureLowToAchieveTargetTemperatureAlarmOperationModeSettingId, /**<Alarm when the mouth temperature did not reach to the target temperature after 30 minute running from started operation and the temperature of environment is lower than specification environment */
    eCheckOperatingConditionsAlarmOperationModeSettingId, /**<Alarm check operating condition */
    eDeviceErrorToAchieveTargetTemperatureAlarmOperationModeSettingId,        /**<Alarm when the mouth temperature did not reach to the target temperature after 30 minute running from started operation and the operating environment temperature is within the specified ambient temperature range*/        
    
    //Abnormal oxygen concentration
    eOxygenHighAlarmOperationModeSettingId, /**<Oxygen concentration is higher than the alarm setting upper limit (The lower and upper limits of the default setting values are 21% and 60%)*/
    eOxygenLowAlarmOperationModeSettingId,  /**<Oxygen concentration is lower than the alarm setting upper limit(The lower and upper limits of the default setting values are 21% and 60%)*/
    
    //Device position
    eDevicePostureAbnormalAlarmOperationModeSettingId, /**<Alarm when the installation orientation of the device is recognized upside down in TBD seconds (Definition of upside down will decide later)*/
    eDevicePostureBadAlarmOperationModeSettingId, /**<TBD*/
            
    //Battery
    eSwitchToBatteryModeAlarmOperationModeSettingId,//37 /**<Switch to battery Mode*/
    eBatteryLowAlarmOperationModeSettingId, /**<Battery low*/
    eBatteryGetsRunOutAlarmOperationModeSettingId, /**<Battery gets run out*/
    eStopFunctionAlarmOperationModeSettingId, /**<Stop function*/
    ePowerNotEnoughAlarmOperationModeSettingId, /**< Power is not enough: AC not connected and L0 < B1+B2 ??L1 */
            
    //SpO2
    eSpO2FailedAlarmOperationModeSettingId, /**< Alarm when receive a SEARCHING TOO LONG or ARTIFACT signal from the Spo2 device*/
    eSpO2LowAlarmOperationModeSettingId, /**< Spo2 value is lower than the alarm setting lower limit (lower limit is 85% or more)*/
    eSpO2SensorProbeAlarmOperationModeSettingId, /**< Alarm when receive a NO FINGER OR SENSOR PROBLEM signal from the Spo2 device*/
    eSpO2SignalLossAlarmOperationModeSettingId, /**< Alarm when receive a LOST PULSE signal from the Spo2 device*/
    eSpO2SensorUnpluggedAlarmOperationModeSettingId, /**< Alarm when receive a SENSOR UNPLUGGED signal from the Spo2 device*/
            
    //Water
    eWaterSupplyCheckAlarmOperationModeSettingId,//47 /**<Water supply check */
    eWaterTankConnectionCheckAlarmOperationModeSettingId,/**<Water tank connection check */
            
    eAirFlowAbnormalAlarmOperationModeSettingId, /**<Air flow abnormal */
    eMainUnitBatteryDisconnectedAlarmOperationModeSettingId,/**<Main unit battery disconnected */
    eCradleBatteryDisconnectedAlarmOperationModeSettingId,/**<Cradle battery disconnected */
                      
                     
    //Internal failure
    eFailureOxygenFlowMeasurementFlowSensorAlarmOperationModeSettingId,//53 /**<Failure of Oxygen flow measurement flow sensor */
    eFailureAirFlowMeasurementFlowSensorAlarmOperationModeSettingId, /**<Failure of Air flow measurement flow sensor */
    eFailureChamberTemperatureSensorAlarmOperationModeSettingId, /**<Failure of chamber temperature sensors */
    eFailureCoilTemperatureSendorAlarmOperationModeSettingId, /**<Failure of Coil temperature sensor */
    eFailureBreathingCircuitOutTemperatureSensorAlarmOperationModeSettingId, //57 /**<Failure of Breathing circuit out temperature sensor*/
    eFailureEnvironmentSensorAlarmOperationModeSettingId, /**<Failure of Environment sensor  (Temp, hum, pressure)*/
    eFailureCurrentSensorAlarmOperationModeSettingId, /**<Failure of Current sensor */
    eFailureBlowerAlarmOperationModeSettingId, /**<Failure of Blower */
    eFailureWaterLevelSensorAlarmOperationModeSettingId, /**<Failure of water level sensor */
    eFailureExternalFlashMemoryAlarmOperationModeSettingId, /**<extrnal flash memory failed */
    eFailureAccelerationSensorAlarmOperationModeSettingId, /**<Failure of acceleration sensor */
    eFailureLightSensorAlarmOperationModeSettingId, /**<Failure of light sensor */
    eSpeakerDisconnectedAlarmOperationModeSettingId,
    // Hiep add
//    eIncorrectBreathingCircuitInAdultsAndChildrenAlarmOperationModeSettingId, /**<Incorrect breathing circuit in adults and children */
    eMainUnitBatteryCommunicationErrorAlarmOperationModeSettingId, //67 /**< Main unit battery communication error */
    eCradleBatteryCommunicationErrorAlarmOperationModeSettingId, /**<Cradle battery communication error */
    eCradleCommunicationErrorAlarmOperationModeSettingId, /**<Cradle communication error */
    eMainMCUFailedOrOutOfControlAlarmOperationModeSettingId, /**<Main MCU failed or out of control */    
    eESP32FailedAlarmOperationModeSettingId, /**<ESP32 Failed */
    eBreathingCircuitHeaterWireBrokenAlarmOperationModeSettingId, /**<Breathing circuit heater wire broken */
    eFailureSpo2ModuleAlarmOperationModeSettingId, /**<Failure of spo2 module */
    eFailureRTCModuleAlarmOperationModeSettingId, /**<Failure of RTC module */
    eFailureLCDTouchModuleAlarmOperationModeSettingId, /**<Failure of LCD touch module */
//    eFailureMainUnitBatteryManagementICAlarmOperationModeSettingId, /**<Failure of Main unit battery management IC */
//    eFailureCradleBatteryManagementICAlarmOperationModeSettingId, //77 /**<Failure of Cradle battery management IC */
    eChamberTemperatureAbnormalAlarmOperationModeSettingId, /**<Chamber temperature abnormal */
    eTooMuchWaterInTheChamberAlarmOperationModeSettingId, /**<Too much water in the chamber */
    eLastAlarmOperationModeSettingId = eTooMuchWaterInTheChamberAlarmOperationModeSettingId,
#endif
            
    eLastSettingId/**<Last setting */
} E_SettingId;

/** @brief define setting item structure */
typedef struct {
    E_SettingId id; /**< setting id */
    uint8_t data; /**< setting data */
    uint8_t min; /**<minimum setting data */
    uint8_t max; /**<maximum setting data */
    uint8_t step; /**<step setting */
} SETTING_ITEM_t;

//Init setting
void setting_Init(void);

// Restore setting info from file
bool setting_Restore(void);

//set value for a setting
void setting_Set(E_SettingId id, uint8_t value);

//get value of a  setting id
uint8_t setting_Get(E_SettingId id);

//Save to flash when check crc sucess
void setting_Save(void);

//Get min setting
uint8_t setting_GetMin(E_SettingId id);

//Get max setting
uint8_t setting_GetMax(E_SettingId id);

//Get step setting
uint8_t setting_GetStep(E_SettingId id);

//Set default setting
void setting_SetDefault(void);

//Update setting to treatment mode
void setting_UpdateSettingByTreatmentMode(E_TreatmentMode treatmentPreMode, E_TreatmentMode treatmentMode);

//Update setting to power mode
void setting_UpdateSettingToPowerMode(bool isACPower, bool isForceUpdate);

// check if setting is init
bool setting_IsInit();


/** @brief 
 *  @param [in] 
 *  @param [out] 
 *  @return
 **/
void setting_SetTemp(uint8_t t);

/** @brief 
 *  @param [in] 
 *  @param [out] 
 *  @return
 **/
void setting_SetAirFlow(uint8_t t);

/** @brief 
 *  @param [in] 
 *  @param [out] 
 *  @return
 **/
void setting_SetOxygen(uint8_t t);

/** @brief 
 *  @param [in] 
 *  @param [out] 
 *  @return
 **/
void setting_SetLanguage(E_Language l);

/** @brief 
 *  @param [in] 
 *  @param [out] 
 *  @return
 **/
void setting_SetBrightnessMode(E_BrightnessMode m);

/** @brief 
 *  @param [in] 
 *  @param [out] 
 *  @return
 **/
void setting_SetBrightnessLevel(uint8_t b);

/** @brief 
 *  @param [in] 
 *  @param [out] 
 *  @return
 **/
void setting_SetMode(E_TreatmentMode m);

/** @brief 
 *  @param [in] 
 *  @param [out] 
 *  @return
 **/
void setting_SetSpeaker(uint8_t s);

/** @brief 
 *  @param [in] 
 *  @param [out] 
 *  @return
 **/
void setting_SetWifi(E_OnOff s);

/** @brief 
 *  @param [in] 
 *  @param [out] 
 *  @return
 **/
void setting_CalibrationAirSetting(uint8_t gain, uint8_t offset);

/** @brief 
 *  @param [in] 
 *  @param [out] 
 *  @return
 **/
void setting_CalibrationO2Setting(uint8_t gain, uint8_t offset);


/** @brief Check power mode change for updating flow, temp setting
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void setting_CheckPowerMode();

#endif	/* SETTING_H */

/* end of file */
