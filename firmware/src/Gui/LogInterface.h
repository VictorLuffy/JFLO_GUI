/** @file LogInterface.h
 *  @brief Contains functions which are Log interface
 *  @author Thanh Hiep Nguyen
 */

#ifndef LOGINTERFACE_H
#define	LOGINTERFACE_H

#include "stdint.h"

#ifndef UNIT_TEST
#include "system_config.h"
#include "system_definitions.h"
#include "system/rtcc/sys_rtcc_pic32m.h"
#endif

#include "RTC.h"
#include "Alarm/AlarmInterface.h"
#include "Setting.h"
#include "File.h"
#include "DeviceInformation.h"


/** @brief Declare queue log waiting */
#define QUEUE_LOG_WAITING 							(2)

// #define log data
#define TREATMENT_MODE_A_TO_P 0x00FF
#define TREATMENT_MODE_P_TO_A 0xFF00

#define DISFECTION_MODE_OK 0x01
#define DISFECTION_MODE_ERROR 0x02

#define ILLUMINANCE_MODE_A_TO_M 0x00FF
#define ILLUMINANCE_MODE_M_TO_A 0xFF00

#define BLUETOOTH_ON_TO_OFF 0xFF00
#define BLUETOOTH_OFF_TO_ON 0x00FF

#define WIFI_ON_TO_OFF 0xFF00
#define WIFI_OFF_TO_ON 0x00FF

#define TOUCH_SOUND_ON_TO_OFF 0xFF00
#define TOUCH_SOUND_OFF_TO_ON 0x00FF

#define MUTE_STATE_ON_TO_OFF 0xFF00
#define MUTE_STATE_OFF_TO_ON 0x00FF

#define LOG_DATA_ERROR_STR "Log data error"

/** @brief Define log header */
#define LOG_HEADER 0xAC

/** @brief Define log id */
typedef enum {
    eFirstEventLogId = 0,                           /**< First log id */
    eStartOperationEventLogId = eFirstEventLogId,   /**< When the start sequence starts */      
    eStopOperationEventLogId,                       /**< When the stop sequence ends*/            
    eTreatmentModeChangedEventLogId,                /**< When the mode is changed*/ /* Mode before change / mode after change */              
    eTurnOnDisinfectionModeEventLogId,              /**< After the user turns ON disinfection mode*/    
    eCompletedDisinfectionModeEventLogId,           /**< After the user turns ON disinfection mode*/             
    eChangeTargetTemperatureSettingEventLogId,      /**< When the target temperature setting is changed*/ /**< Target temperature before change / target temperature after change */      
    eChangeTargetFlowSettingEventLogId,             /**< When the target flow setting is changed */ /**< Target flow before change / target flow after change */    
    eDateChangeEventLogId,                          /**< Date before change / Date after change*/     
    eChangeLanguageSettingEventLogId,               /**< Language before change /  Language after change*/   
    eChangeIlluminanceSettingEventLogId,            /**< Illuminance setting before change / Illuminance setting before change */   // Auto/ manual
    eChangeIlluminanceLevelSettingEventLogId,       /**< Illuminance level before change / Illuminance level after change */    
    eBtSettingChangeEventLogId,                     /**< BT State before change / BT State after change */     
    eWifiSettingChangeEventLogId,                   /**< Wifi State before change / Wifi State after change */            
    eOxygenSourceSettingChangeUpperLimitEventLogId,  /**< Oxygen source setting change 90 / 100 */    
    eOxygenAlarmSettingChangeUpperLimitEventLogId,  /**< Upper limit before change / Upper limit after change */    
    eOxygenAlarmSettingChangeLowerLimitEventLogId,  /**< Lower limit before change / Lower limit after change */    
    eSpO2AlarmSettingChangeLowerLimitEventLogId,  /**< Upper limit before change / Upper limit after change */    
    eChangeTouchSoundSettingsEventLogId,            /**< touch sound before change / touch sound after change */        
    eMuteButtonStateChangeEventLogId,               /**< Mute button state  / Mute button state after change */
    eAcModeEventLogId,                              /**< When power on with AC power, or when changing from battery-mode to AC mode */          
    eBatteryModeEventLogId,                         /**< When power on with battery power, or when changing from AC -mode to battery mode */   
    eMainBatteryNotConnectedEventLogId,             /**< When power on with the battery disconnected Or when the battery is removed*/            
    eBatteryConnectedEventLogId,                    /**< When power on with the battery connected Or when the battery is connected from not connected. */
    eCradleConnectedEventLogId,                     /**< When it is detected that the cradle has changed from the Disconnected state to the connected state */
    eCradleBatteryDisconnectedEventLogId,           /**< When it is detected that the cradle has changed from the connected state to the Disconnected state */
    eCradleBatteryConnectedEventLogId,              /**< When it is detected that the cradle Battery has changed from the Disconnected state to the connected state */

    eMainBatteryOverChargedAlarmEventLogId,         /**< When OVER_CHARGED_ALARM bit of main unit battery set */
    eMainBatteryTerminateChargedAlarmEventLogId,    /**< When TERMINATE_CHARGE_ALARM bit of main unit battery set */
    eMainBatteryOverTempAlarmEventLogId,            /**< When OVER_TEMP_ALARM bit of main unit battery set (main battery temp / ambient temp) */
    eMainBatteryTerminateDischargeAlarmEventLogId,          /**< When TERMINATE_DISCHARGE_ALARM bit of main unit battery set */
    eMainBatteryFullyChargedEventLogId,             /**< When FULLY_CHARGED bit of main unit battery set */
    eMainBatteryFullyDischargedEventLogId,          /**< When FULLY_DISCHARGED bit of main unit battery set */
    eMainBatteryBusyEventLogId,          /**< When Received busy error from main unit battery */
    eMainBatteryUnsupportedCommandEventLogId,          /**< When Received UnsupportedCommand error from main unit battery */
    eMainBatteryAccessDeniedEventLogId,          /**< When Received AccessDenied error from main unit battery */
    eMainBatteryOverflowUnderflowEventLogId,          /**< When Received busy Overflow/Underflow from main unit battery */
    eMainBatteryBadSizeEventLogId,          /**< When Received busy BadSize from main unit battery */
    eMainBatteryUnknownErrorEventLogId,          /**< When Received busy UnknownError from main unit battery */
            
    eCradleBatteryOverChargedAlarmEventLogId,         /**< When OVER_CHARGED_ALARM bit of Cradle battery set */
    eCradleBatteryTerminateChargedAlarmEventLogId,    /**< When TERMINATE_CHARGE_ALARM bit of Cradle  battery set */
    eCradleBatteryOverTempAlarmEventLogId,            /**< When OVER_TEMP_ALARM bit of cradle unit battery set (cradle battery temp / ambient temp) */
    eCradleBatteryTerminateDischargeAlarmEventLogId,          /**< When TERMINATE_DISCHARGE_ALARM bit of cradle unit battery set */
    eCradleBatteryFullyChargedEventLogId,             /**< When FULLY_CHARGED bit of cradle unit battery set */
    eCradleBatteryFullyDischargedEventLogId,          /**< When FULLY_DISCHARGED bit of cradle unit battery set */
    eCradleBatteryBusyEventLogId,          /**< When Received busy error from cradle unit battery */
    eCradleBatteryUnsupportedCommandEventLogId,          /**< When Received UnsupportedCommand error from Cradle unit battery */
    eCradleBatteryAccessDeniedEventLogId,          /**< When Received AccessDenied error from Cradle unit battery */
    eCradleBatteryOverflowUnderflowEventLogId,          /**< When Received busy Overflow/Underflow from Cradle unit battery */
    eCradleBatteryBadSizeEventLogId,          /**< When Received busy BadSize from Cradle unit battery */
    eCradleBatteryUnknownErrorEventLogId,          /**< When Received busy UnknownError from Cradle unit battery */

    eChangeSpeakerLevelSettingEventLogId,           /**< When speaker volume level setting change */
    eNoOfLogEventLogId,                             /**<No of log id */
} E_EventLogId;

/** @brief Definition of structures */
typedef enum
{
    eFirstLogRequestId = 0,                     /**< First Log Request*/
    eUSBGetSettingLogRequestId= eFirstLogRequestId, /**< Request for send setting log to USB*/
    eUSBGetDeviceInfoLogRequestId,              /**< Request for send device log to USB*/
    eUSBGetEventLogRequestId,                   /**< Request for send event log to USB*/     
    eUSBGetAlarmLogRequestId,                   /**< Request for send alarm log to USB*/
    eUSBGetSpO2LogRequestId,                    /**< Request for send spo2 log to USB*/
    eDeleteSettingLogRequestId,                 /**< Request for delete setting log to USB*/
    eDeleteDeviceInfoLogRequestId,              /**< Request for delete device infomation log to USB*/
    eDeleteEventLogRequestId,                   /**< Request for delete event log to USB*/
    eDeleteAlarmLogRequestId,                   /**< Request for delete alarm log to USB*/
    eDeleteSpO2LogRequestId,                    /**< Request for delete spo2 log to USB*/  

	eNoOfLogRequestId
            
} E_LogRequestID;

extern const char *eventStringList[eNoOfLogEventLogId];

extern const char *alarmStringList[eNoOfAlarmId];

extern const char *alarmStatusStringList[eNoOfAlarmStatus];

extern const char languageStringList[eNoOfLanguage][3];

/** @brief Definition of structures */
typedef enum
{
	eWait,
	eSuccess,
	eTimeout
} GetLogResult;

/** @brief Definition of structures */
typedef enum {
    eFirstLogTypeID,                            
    eEventLogTypeID = eFirstLogTypeID,          /**< Event log type*/
    eAlarmLogTypeID,                            /**< Alarm log type*/
	eDeviceInformationLogTypeID ,               /**< Device infomation log type*/
	eSettingsLogTypeID,                         /**< Setting log type*/
    eSpo2DataLogTypeID,                         /**< Spo2 log type*/
	eNumberOfLogTypeID
} E_LogType;

/** @brief Definition of timestamp */
typedef struct {
    uint8_t year_1;                     /**< 2 first number of yeat value */
    uint8_t year_2;                     /**< 2 last number of yeat value */
    uint8_t month;                      /**< month value */
    uint8_t date;                       /**< date value */
    uint8_t hour;                       /**< hour value */
    uint8_t minute;                     /**< minute value */
    uint8_t second;                     /**< second value */
} Timestamp;        // 7 bytes


/** @brief Define struct for log */
typedef struct {
    uint8_t header;                     /**<byte header */  //  0
    Timestamp time;                     /**< Timestamp  */  //  1
    E_LogType logType;                  /**< Log Type  */  //  8
    uint8_t eCode;                      /**< Log ID  *///  9
    uint8_t numOfDataLen;               /**< Data Len  *///  10
    uint8_t data[16];                      /**< Data 1  *///  11
} Log_Struct; // old: JFLO_LOG_t

typedef enum
{
    eAdultTotalOperatingTimeLogId,
	ePediatricTotalOperatingTimeLogId,
    eSerialNumber,       
    eNoOfDeviceInfoLogId        
} E_DeviceInfoLogId;

typedef struct {
    E_DeviceInfoLogId id;
    uint8_t data[8];
    uint8_t dataLen;
} DeviceInformationLog_Struct;

/* Send command to write log event data to log file stored in SQI Flash */
bool logInterface_WriteEventLog(uint8_t dataLen, void* logData, E_EventLogId id);

/* Send command to write log alarm data to log file stored in SQI Flash */
bool logInterface_WriteAlarmLog(uint8_t dataLen, void* logData, E_AlarmId id);

/* Send command to write spo2 event data to log file stored in SQI Flash */
bool logInterface_WriteSpO2Log(uint8_t spO2data);

/* Send command to write device information event data to log file stored in SQI Flash */
bool logInterface_WriteDeviceInformation(uint8_t dataLen, void* logData, E_DeviceInfoLogId id);

/* Send command to handle log data */
bool logInterface_SendLogRequest(E_LogRequestID event);

/* Internal use */
/* Trigger all log event id for testing */
void LogInterface_TestLogEvent();

/* Internal use */
void LogInterface_TestAlarmEvent();

/* define debug log macro */
#define DEBUG_LOG_TO_FILE
#define DEBUG_LOG_FILENAME "debug.log"
#define DEBUG_LOG_FILE SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0"/"DEBUG_LOG_FILENAME

/* Init debug log file */
void LogInterface_InitDebugLogFile();

/* DeInit debug log file */
void LogInterface_DeInitDebugLogFile();

/* Record debug log */
void LogInterface_WriteDebugLogFile(char * str);

/* Export debug log */
void LogInterface_ExportDebugLogFileToUSB();

/* Delete debug log */
void LogInterface_ClearDebugLogFile();

uint32_t LogInterface_GetRunningTime();

uint16_t LogInterface_GetTotalLog();

void LogInterface_GetLogContent(uint8_t id, Log_Struct *log);

uint16_t LogInterface_GetTotalAlarm();

void LogInterface_GetAlarmContent(uint8_t id, Log_Struct *log);

uint32_t LogInterface_GetTotalOperationTime(void);

uint32_t LogInterface_GetOperationTimeAfterService(void);

void LogInterface_GetESPFWVersion(char *sn);

void LogInterface_GetSpO2FWVersion(char *sn);

/** @brief Get Alarm status string
 *  @param [in] E_AlarmStatus alarmStatus
 *  @param [out] char *strbuff
 *  @return Alarm name.
 */
void LogInterface_GetAlarmStatusString(E_AlarmStatus alarmStatus, char *strbuff);

/** @brief Get Alarm status string
 *  @param [in] E_AlarmStatus alarmStatus
 *  @param [out] char *strbuff
 *  @return Alarm name.
 */
void LogInterface_GetAlarmStringFromID(E_AlarmId alarmId, char *strbuff);


/** @brief Get event string name
 *  @param [in] E_EventLogId eventId
 *  @param [out] char *strbuff
 *  @return Alarm name.
 */
void LogInterface_GetEventStringFromID(E_EventLogId eventId, char *strbuff);

/** @brief Get event refer data in string
 *  @param [in] E_EventLogId eventId
 *  @param [in] uint8_t *data
 *  @param [out] char *strbuff
 *  @return Alarm name.
 */
void LogInterface_GetEventDataString(E_EventLogId eventId, uint8_t *data, char *strbuff);

#endif	/* LOGINTERFACE_H */

/* end of file */
