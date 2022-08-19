/* ************************************************************************** */
/** @file [PC_Monitoring.h]
 *  @brief {This file contain source code necessary to transfer data between JFLOW
 * and PC APP, include handling data come from PC APP as well as sending data to 
 * PC APP, initialize dependency components such as UART6 }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */

#ifndef _PC_MONITORING_H    /* Guard against multiple inclusion */
#define _PC_MONITORING_H


/* This section lists the other files that are included in this file.
 */
#include "system_config.h"
//#include "system_definitions.h"
#include "Setting.h"
#include "ChamberUnit.h"

//#define CONNECT             0
//#define REBOOT              1
//#define GET_DIAG            2
//#define UPDATE              3
//#define SET_SERIALNO        4
//#define GET_SERIALNO        5
//#define SET_LOGCLEAR        6
//#define SET_INIT            7
//#define SET_MOUTH_TEMP      8
//#define GET_MOUTH_TEMP      9
//#define GET_CHAMBER_TEMP    10
//#define GET_BREATHING_TEMP  11
//#define GET_ENV_TEMP        12
//#define GET_BATT_TEMP       13
//#define SET_AIR_FLOW        14
//#define GET_AIR_FLOW        15
//#define GET_O2_FLOW         16
//#define SET_OXYGEN          17
//#define GET_OXYGEN          18
//#define GET_SPO2            19
//#define GET_PULSERATE       20
//#define GET_RUNNING         21
//#define GET_OUTBATTEXT      22
//#define GET_OUTBATTREM      23
//#define GET_INBATTEXT       24
//#define GET_INBATTREM       25
//#define GET_AC              26
//#define SET_CAL1            27
//#define SET_CAL2            28
//#define GET_ARARMCNT        29
//#define GET_ARARM           30
//#define SET_LANG            31
//#define GET_LANG            32
//#define SET_WIFI            33
//#define GET_WIFI            34
//#define GET_WIFIAP          35
//#define SET_DATETIME        36
//#define GET_DATETIME        37
//#define GET_LOGCNT          38
//#define GET_LOG             39
//#define SET_BRIGHTMODE      40
//#define GET_BRIGHTMODE      41
//#define SET_BRIGHTNESS      42
//#define GET_BRIGHTNESS      43
//#define SET_BACKGROUND      44
//#define GET_BACKGROUND      45
//#define SET_MODE            46
//#define GET_MODE            47
//#define SET_SPEAKER         48
//#define GET_SPEAKER         49
//#define UNSUPPORT           255
//#define NO_COMMAND          255
#ifdef FUNCTION_DISABLE_ALARM
#define NUMBER_ALARM_OP_MODE_SETTING    (eLastAlarmOperationModeSettingId - eFirsAlarmOperationModeSettingId + 1)
#endif

enum{
    CONNECT = 0,           
    REBOOT,
    SHUTDOWN,
    GET_DIAG,            
    UPDATE,

#ifdef FUNCTION_DISABLE_ALARM
    SET_ALL_ALARM,
    SET_ALARM_DISABLE,
    SET_ALARM_ENABLE,
    GET_ALARM_DISABLED,
#endif
    
    GET_MAIN_VERSION,
    GET_CRADLE_VERSION,
    GET_CHAMBER_VERSION,
    GET_RESPIRATORY_VERSION,
    
    SET_MAIN_SERIALNO,
    SET_CRADLE_SERIALNO,
    SET_CHAMBER_SERIALNO,
    SET_RESPIRATORY_SERIALNO,
    SET_RESPIRATORY_CODE,
    SET_RESPIRATORY_FACTORY,
    SET_RESPIRATORY_DATE,
    SET_RESPIRATORY_USE_TIME,
    SET_RESPIRATORY_USE_COUNT,
    SET_RESPIRATORY_TYPE,
    
    GET_MAIN_SERIALNO,
    GET_CRADLE_SERIALNO,
    GET_CHAMBER_SERIALNO,
    GET_RESPIRATORY_SERIALNO,
    GET_RESPIRATORY_CODE,
    GET_RESPIRATORY_FACTORY,
    GET_RESPIRATORY_DATE,
    GET_RESPIRATORY_USE_TIME,
    GET_RESPIRATORY_USE_COUNT,
    GET_RESPIRATORY_TYPE,
    
    SET_LOGCLEAR,       
    SET_INIT,           
    SET_MOUTH_TEMP,      
    GET_MOUTH_TEMP,      
    GET_CHAMBER_TEMP,    
    GET_BREATHING_TEMP,
    GET_ENV_TEMP,
    GET_BATT_TEMP,
    SET_AIR_FLOW,
    GET_AIR_FLOW,
    GET_O2_FLOW,
    SET_OXYGEN,
    GET_OXYGEN,
    GET_SPO2,
    GET_PULSERATE,
    GET_RUNNING,
    GET_OUTBATTEXT,
    GET_OUTBATTREM,
    GET_INBATTEXT,
    GET_INBATTREM,
    GET_AC,
    SET_CAL1,
    SET_CAL2,
    
    GET_ALARMCNT,
    GET_ALARM,
    
    SET_LANG,
    GET_LANG,
    SET_WIFI,
    GET_WIFI,
    GET_WIFIAP,
    SET_DATETIME,
    GET_DATETIME,
    GET_LOGCNT,
    GET_LOG,
    
    SET_BRIGHTMODE,
    GET_BRIGHTMODE,
    SET_BRIGHTNESS,
    GET_BRIGHTNESS,
    SET_BACKGROUND,
    GET_BACKGROUND,
    SET_MODE,
    GET_MODE,
    SET_SPEAKER,
    GET_SPEAKER,
    
    GET_BRIGHTNESS_SENSOR,
    GET_ACCELERATION_SENSOR,
    GET_WATERLEVEL_SENSOR,
    GET_BLOWER_SPEED,
    LAST_COMMAND = GET_BLOWER_SPEED,
    NUM_OF_COMMAND,
    UNSUPPORT = 255,
    NO_COMMAND = UNSUPPORT,
};

//************************************************************************
/*
  SYS CMD Command Function

  Summary:
    Identifies the command process function API.

  Description:
    Command Process Function API. This handle identifies the interface structure 
	of the command process function API.
	
  Remarks:
    None.
*/
typedef int (*PC_CMD_FNC)(void);

typedef struct
{
    const char *commandArray;           // string identifying the command
    const uint8_t commandCode;
    PC_CMD_FNC cmdFnc;                  // function to execute for this command
} Command_t;

typedef enum
{
    eMainUpdate = 0,
    eChamberUpdate,
    eCradleUpdate,
    eEsp32Update,
    eNumberDeviceUpdate,        
} E_updateDeviceType;

typedef struct
{
    const char *updateDeviceName;
    E_updateDeviceType updateDeviceCode;
} UpdateDevice_t;

typedef struct
{
    const char *alarmErrorCodeName;
    E_SettingId alarmSettingId;
} AlarmOperationModeSetting_t;


//typedef enum
//{
//    eEN,
//    eVN,
//    eJP
//} E_languageType;

//typedef struct
//{
//    const char *languageName;
//    E_languageType languageCode;
//} LanguageSetting_t;

typedef enum
{
    eReadCommandAndParse,
    eProcessCommandAndResponse,
} E_CommandHandle;


typedef enum
{
    eReadyForReceive,
    eWaitforReceiveDone,
    eParseData,
} E_ParseDataState;


//typedef enum
//{
//    eNON_EXIST,
//    eEXIST
//} E_BattType;

//typedef enum
//{
//    CONNECT_FAIL,
//    CONNECT_OK
//} E_connectState;

//typedef enum
//{
//    eOFF,
//    eON
//} E_switch;

//typedef enum
//{
//    eAUTO,
//    eMANUAL
//} E_brightnessMode;

//typedef enum
//{
//    eDARK,
//    eLIGHT
//} E_background;

//typedef enum
//{
//    eCHILD,
//    eADULT
//} E_deviceMode;
//
//typedef enum
//{
//    eSetFail,
//    eSetSuccess
//} E_setResponse;
//
//typedef struct
//{
//  uint8_t first;
//  uint8_t end;
//} Year_t;
//
//typedef struct
//{
//    Year_t year;
//    uint8_t month;
//    uint8_t day;
//    uint8_t hour;
//    uint8_t min;
//    uint8_t second;
//} Datetime_t;
//
//typedef enum
//{
//    eFail = 0,
//    ePass = 1
//} E_diag;
//
//typedef enum
//{
//    eUpdateFail,
//    eUpdateSuccess
//} E_UpdateState;
//
//typedef struct
//{
//    float gain;
//    float offset;
//} Calibration_t;
//
//typedef struct
//{
//    uint8_t gain[3];
//    uint8_t offset[3];
//} IntCalibration_t;
//
//typedef struct
//{
//    int alarmID;
//    Datetime_t alarmDatetime;
//} AlarmContent_t;
//
//typedef struct
//{
//    int logID;
//    Datetime_t logDatetime;
//} LogContent_t;

typedef struct
{
    //E_diag diagState;
    float settingTemp;
    float chamberOutletTemp;
    float breathingCircuitTemp;
    float envTemp;
    float mainBatteryTemp;
    float airFlow;
    float O2Flow;
    float O2Concentration;
    uint8_t SpO2Value;
    uint8_t pulseRate;
    //Datetime_t runningTime;
    bool outBattExt;
    uint16_t outBattRem;
    bool inBattExt;
    uint16_t inBattRem;
    bool ACState;
    uint8_t language;
    uint8_t WifiState;
    uint8_t responseAPNameSize;
    E_BrightnessMode brightnessMode;
    uint8_t brightnessValue;
    uint8_t deviceMode;
    uint8_t speakerVolume;
    E_TreatmentMode mode;
    uint16_t blowerSpeed;
    E_TankWaterLevel waterLevel;
    float xAngleDirection;
    float yAngleDirection;
} PC_MONITORING_t;

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief Initialize PC monitor use to communicate with PC app by opening a COM
     * port (UART6)
     * This function should be called 1 times when start the application
     *  @param [in] None
     *  @param [out] None
     *  @return None
     */
    void PC_Monitor_Initialize();

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
//    void PC_Monitor_Send(float* dataArray, uint32_t dataSize);

    /** @brief Maintain PC Monitor task
     *  This function should be called periodically in a Task
     *  @param [in]   None
     *  @param [out]  None
     *  @return None
     */
    void PC_Monitor_Task();
    
    void PC_Monitor_Handle();


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _PC_MONITORING_H */

/* *****************************************************************************
 End of File
 */
