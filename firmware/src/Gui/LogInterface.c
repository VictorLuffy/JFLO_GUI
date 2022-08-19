/** @file LogInterface.h
 *  @brief The screen provides graphic interface to update firmware
 *  @author Thanh Hiep Nguyen
 */


/* This section lists the other files that are included in this file.
 */

#include "../FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "../third_party/rtos/FreeRTOS/Source/include/semphr.h"

#include "LogInterface.h"
#include "LogMgr.h"
#include "Alarm/AlarmInterface.h"
#include "GuiInterface.h"
#include "DeviceInterface.h"
#include "FileSystemMgr.h"
#include "DisplayControl.h"

/** @brief Declare queue receive ID data */
extern QueueHandle_t g_logQueueReceiveEvent;

/** @brief Declare queue receive ID data */
extern QueueHandle_t g_logQueueReceiveAlarm;

/** @brief Declare queue receive ID data */
extern QueueHandle_t g_logQueueReceiveSpO2;

/** @brief Declare queue receive ID data */
extern QueueHandle_t g_logQueueReceiveDeviceInformation;

/** @brief Declare queue receive from Request*/
extern QueueHandle_t g_logQueueForRequest;

SemaphoreHandle_t logFileMutex = NULL;

const char *eventStringList[eNoOfLogEventLogId] = {
    [eStartOperationEventLogId]="Start Operation",
    [eStopOperationEventLogId]="Stop Operation",
    [eTreatmentModeChangedEventLogId]="Treatment Mode",
    [eTurnOnDisinfectionModeEventLogId]="Disinfection Mode ON",
    [eCompletedDisinfectionModeEventLogId]="Disinfection Mode",
    [eChangeTargetTemperatureSettingEventLogId]="Temp Setting",
    [eChangeTargetFlowSettingEventLogId]="Flow Setting",
    [eDateChangeEventLogId]="Set",
    [eChangeLanguageSettingEventLogId]="Language",
    [eChangeIlluminanceSettingEventLogId]="Illuiminance Mode",
    [eChangeIlluminanceLevelSettingEventLogId]="Illuminance Level",
    [eBtSettingChangeEventLogId]="Bluetooth",
    [eWifiSettingChangeEventLogId]="Wifi",
    [eOxygenSourceSettingChangeUpperLimitEventLogId]="Oxy Source",
    [eOxygenAlarmSettingChangeUpperLimitEventLogId]="O2Alarm U Limit",
    [eOxygenAlarmSettingChangeLowerLimitEventLogId]="O2Alarm L Limit",
    [eSpO2AlarmSettingChangeLowerLimitEventLogId]="SpO2 L Limit",
    [eChangeTouchSoundSettingsEventLogId]="Touch Sound",
    [eMuteButtonStateChangeEventLogId]="Mute State",
    [eAcModeEventLogId]="AC Mode",
    [eBatteryModeEventLogId]="Battery Mode",
    [eMainBatteryNotConnectedEventLogId]="Battery Not Connected",
    [eBatteryConnectedEventLogId]="Batt Connected",
    [eChangeSpeakerLevelSettingEventLogId]="Speaker Level"
};

const char *alarmStringList[eNoOfAlarmId] = {
    [eI2C1ErrorAlarm] = "I2C1 Error",
    [eI2C2ErrorAlarm] = "I2C2 Error",
    [eI2C3ErrorAlarm] = "I2C3 Error",
    [eI2C4ErrorAlarm] = "I2C4 Error",
    [eSPI3ErrorAlarm] = "SPI3 Error",
    [eUart1ErrorAlarm] = "Uart1 Error",
    [eUart2ErrorAlarm] = "Uart2 Error",
    [eUart4ErrorAlarm] = "Uart4 Error",
    [eUart6ErrorAlarm] = "Uart6 Error",
    [eAirFlowSensorErrorAlarm] = "Air Flow Sensor Error",
    [eO2FlowSensorErrorAlarm] = "O2 Flow Sensor Error",
    [eBME280ErrorAlarm] = "BME280 Error",
    [eADXL345ErrorAlarmID] = "ADXL345 Error",
    [eDRV8308ErrorAlarm] = "DRV8308 Error",
    [eAudioErrorAlarm] = "Audio Error",
    [eMotorTaskErrorAlarm] = "Motor Task Error",
    [eHeaterTaskErrorAlarm] = "Heater Task Error",
    [eHumidityTaskErrorAlarm] = "Humidity Task Error",
    [eBreathingCircuitNotConnectedAlarmId] = "E001 Breathing cir..",
    [eCheckLeakAlarmId] = "E002 Check for Leaks",
    [eCheckBlockageAlarmId] = "E003 Check for blo..",
    [eNonGenuineCircuitAlarmId] = "E004 Non-genuine ..",
    [eBreathingCircuitChangedAlarmId] = "E005 Incorrect bre..",
    [eExpiredCircuitAlarmId] = "E006 Expired circuit",
    [eCheckConnectionChamberAlarmId] = "E007 Check connect..",
    [eNoMoreWaterInChamberAlarmId] = "E008 There is no m..",
    [eRunOutOfWaterAlarmId] = "E009 run out of wa..",
    [eLowTemperatureAlarmId] = "E010 Low Temperature",
    [eHighTemperatureAlarmId] = "E011 High Temperature",
    [eHighTemperatureAbnormalityAlarmId] = "E012 High temperature",
    [eRoomTemperatureLowToAchieveTargetTemperatureAlarmId] = "E013 Room temperat..",
    [eCheckOperatingConditionsAlarmID] = "E014 Check operati..",
    [eDeviceErrorToAchieveTargetTemperatureAlarmId] = "E015 Device error, ..",
    [eOxygenHighAlarmId] = "E016 O2 too high",
    [eOxygenLowAlarmId] = "E017 O2 too low",
    [eDevicePostureAbnormalAlarmId] = "E018 The posture of..",
    [eDevicePostureBadAlarmId] = "E019 Poor posture ..",
    [eSwitchToBatteryModeAlarmId] = "E020 Switch to batt..",
    [eBatteryLowAlarmId] = "E021 Battery Low",
    [eBatteryGetsRunOutAlarmId] = "E022 Battery gets ..",
    [eStopFunctionAlarmId] = "E023 Stop function",
    [ePowerNotEnoughAlarmId] = "E024 Power is not ..",
    [eSpO2FailedAlarmId] = "E025 Spo2 Failed",
    [eSpO2LowAlarmId] = "E026 Spo2 Low",
    [eSpO2SensorProbeAlarmId] = "E027 Check the SpO2..",
    [eSpO2SignalLossAlarmId] = "E028 Spo2 signal loss",
    [eSpO2SensorUnpluggedAlarmId] = "E029 Spo2 SENSOR ..",
    [eWaterSupplyCheckAlarmId] = "E030 Water supply.." ,
    [eWaterTankConnectionCheckAlarmId] = "E031 Water tank.." ,
    [eAirFlowAbnormalAlarmId] = "E032 Air flow abnor.." ,
    [eMainUnitBatteryDisconnectedAlarmId] = "E033 Main unit batt.." ,
    [eCradleBatteryDisconnectedAlarmId] = "E034 Cradle battery.." ,
    [eFailureOxygenFlowMeasurementFlowSensorAlarmId] = "E100 Device Error",
    [eFailureAirFlowMeasurementFlowSensorAlarmId] = "E101 Device Error",
    [eFailureChamberTemperatureSensorAlarmId] = "E102 Device Error",
    [eFailureCoilTemperatureSendorAlarmId] = "E103 Device Error",
    [eFailureBreathingCircuitOutTemperatureSensorAlarmId] = "E104 Device Error",
    [eFailureEnvironmentSensorAlarmId] = "E105 Device Error",
    [eFailureCurrentSensorAlarmId] = "E106 Device Error",
    [eFailureBlowerAlarmId] = "E107 Device Error",
    [eSpeakerDisconnectedAlarmId] = "E108 Device Error",
    [eFailureWaterLevelSensorAlarmId] = "E109 Device Error",
    [eFailureExternalFlashMemoryAlarmId] = "E110 Device Error",
    [eFailureAccelerationSensorAlarmId] = "E111 Device Error",
    [eFailureLightSensorAlarmId] = "E112 Device Error",
    [eMainUnitBatteryCommunicationErrorAlarmId] = "E113 Main battery ..",
    [eCradleBatteryCommunicationErrorAlarmId] = "E114 Device Error",
    [eCradleCommunicationErrorAlarmId] = "E115 Device Error",
    [eMainMCUFailedOrOutOfControlAlarmId] = "E116 Device Error",
    [eESP32FailedAlarmId] = "E117 Device Error",
    [eBreathingCircuitHeaterWireBrokenAlarmId] = "E118 Device Error",
    [eFailureSpo2ModuleAlarmId] = "E119 Device Error",
    [eFailureRTCModuleAlarmId] = "E120 Device Error",
    [eFailureLCDTouchModuleAlarmId] = "E121 Device Error",
    [eChamberTemperatureAbnormalAlarmId] = "E124 Chamber Tem..",
    [eTooMuchWaterInTheChamberAlarmId] = "E125 Too much water.."
};

const char *alarmStatusStringList[eNoOfAlarmStatus] = {
    [eInactive] = "Cleared",
    [ePaused] = "Paused",
    [eWaitingToActive] = "Active",
    [eActive] = "Active"
};

const char languageStringList[eNoOfLanguage][3] = {
    [eJapanese] = "JP",
    [eEnglish] = "EN",
    [eVietnamese] = "VN"
};

SYS_FS_HANDLE logFile = SYS_FS_HANDLE_INVALID;

//extern bool g_isMountUSB = true;
/** @brief Write log data by logId
 *  @param [in] E_LogId id: log id
 *  @param [out] None
 *  @retval true Event was sent successful
 *  @retval false Event was sent failed
 */
bool logInterface_WriteEventLog(uint8_t dataLen, void* logData, E_EventLogId id) 
{
    if (logMgr_GetLogStatus(eEventLogTypeID) != eReadyLogStatus)
    {
        SYS_PRINT("\n EventLog havent been ready yet");
        return false;
    }
        
    Timestamp time = logMgr_getRtcTime();

    Log_Struct log = {
        .header = LOG_HEADER,
        .time.year_1 = time.year_1,//nowTime.year_1,
        .time.year_2 = time.year_2,//nowTime.year_2,
        .time.month = time.month, /*nowTime.month*/
        .time.date = time.date, /*nowTime.date*/
        .time.hour = time.hour, /*nowTime.hour*/
        .time.minute = time.minute, /*nowTime.minute*/
        .time.second = time.second, /*nowTime.second*/ 
        .logType = eEventLogTypeID,
        .eCode = id,
        .numOfDataLen = dataLen,
    };
    memcpy((void*)log.data, logData, dataLen);
//    SYS_PRINT("\n logInterface_WriteMachineLog is writing !!");

    if (xQueueSendToBack(g_logQueueReceiveEvent, &log, QUEUE_LOG_WAITING) != pdPASS) {
        SYS_PRINT("\n send failed: event to log task");
        xQueueReset(g_logQueueReceiveEvent);
        return false;
    }

    return true;
}

/** @brief Write log data by logId
 *  @param [in] E_LogId id: log id
 *  @param [out] None
 *  @retval true Alarm was sent successful
 *  @retval false Alarm was sent failed
 */
bool logInterface_WriteAlarmLog(uint8_t dataLen, void* logData, E_AlarmId id) 
{
    if (logMgr_GetLogStatus(eAlarmLogTypeID) != eReadyLogStatus)
    {
        SYS_PRINT("\n AlarmLog havent been ready yet");
        return false;
    }    
    
    Timestamp time = logMgr_getRtcTime();

    Log_Struct log = {
        .header = LOG_HEADER,
        .time.year_1 = time.year_1,  //nowTime.year_1,
        .time.year_2 = time.year_2, //nowTime.year_2,
        .time.month = time.month, /*nowTime.month*/
        .time.date =  time.date,/*nowTime.date*/
        .time.hour = time.hour, /*nowTime.hour*/
        .time.minute = time.minute, /*nowTime.minute*/
        .time.second = time.second, /*nowTime.second*/  
        .logType = eAlarmLogTypeID,
        .eCode = id,
        .numOfDataLen = dataLen,
    };

    memcpy((void*)log.data, logData, dataLen);
    
    if (xQueueSendToBack(g_logQueueReceiveAlarm, &log, QUEUE_LOG_WAITING) != pdPASS) {
        SYS_PRINT("\n send failed: event to log task");
        xQueueReset(g_logQueueReceiveAlarm);
        return false;
    }

    return true;
}

/** @brief Write SpO2 log 
 *  @param [in] uint8_t spO2data
 *  @param [out] None
 *  @retval true SpO2 Data was sent successful
 *  @retval false SpO2 Data was sent failed
 */
bool logInterface_WriteSpO2Log(uint8_t spO2data)
{   
    if (logMgr_GetLogStatus(eSpo2DataLogTypeID) != eReadyLogStatus)
    {
        SYS_PRINT("SpO2 havent been ready yet \n");
        return false;
    }    
    Timestamp time = logMgr_getRtcTime();

    Log_Struct log = {
        .header = LOG_HEADER,
        .time.year_1 = time.year_1,  //nowTime.year_1,
        .time.year_2 = time.year_2, //nowTime.year_2,
        .time.month = time.month, /*nowTime.month*/
        .time.date =  time.date,/*nowTime.date*/
        .time.hour = time.hour, /*nowTime.hour*/
        .time.minute = time.minute, /*nowTime.minute*/
        .time.second = time.second, /*nowTime.second*/  
        .logType = eSpo2DataLogTypeID,
        .numOfDataLen = 1,
    };
    log.data[0] = spO2data;
                
    if( xQueueSendToBack(g_logQueueReceiveSpO2, &log, QUEUE_LOG_WAITING) != pdPASS)
	{
        xQueueReset(g_logQueueReceiveSpO2);
		SYS_PRINT("Failed to send item to queue");
		return false;
	}
	
	return true;
}
/** @brief Write device information log data
 *  @param [in] E_LogId id: log id
 *  @param [out] None
 *  @retval true log was sent successful
 *  @retval false log was sent failed
 */
bool logInterface_WriteDeviceInformation(uint8_t dataLen, void* logData, E_DeviceInfoLogId id)
{
    DeviceInformationLog_Struct log = {
        .id = id,
        .dataLen = dataLen
    };
    memcpy((void*)log.data, logData, dataLen);
    
    if (xQueueSendToBack(g_logQueueReceiveDeviceInformation, &log, QUEUE_LOG_WAITING) != pdPASS) {
        SYS_PRINT("\n send failed: event to log task");
        xQueueReset(g_logQueueReceiveDeviceInformation);
        return false;
    }

    return true;
}


/** @brief Command log request
 *  @param [in] E_LogId id: log id
 *  @param [out] None
 *  @retval true LogRequest was sent successful
 *  @retval false LogRequest was sent failed
 */
bool logInterface_SendLogRequest(E_LogRequestID event)
{
	if( xQueueSendToBack(g_logQueueForRequest, &event, QUEUE_LOG_WAITING) != pdPASS)
	{
		SYS_PRINT("Failed to send item to queue within 500ms");
        xQueueReset(g_logQueueForRequest);
		return false;
	}
	
	return true;
}

void LogInterface_GetEventStringFromID(E_EventLogId eventId, char *strbuff)
{
    sprintf(strbuff, "%s %d", LOG_DATA_ERROR_STR, eventId);
    if (eventId < eFirstEventLogId || eventId >= eNoOfLogEventLogId)
    {
        return;
    }
    // TODO: use switch case to handle in case new event id add but not add string
    // This can remove if the event id are finalized 
    switch(eventId)
    {
        case eStartOperationEventLogId:
        case eStopOperationEventLogId:
        case eTreatmentModeChangedEventLogId:      
        case eTurnOnDisinfectionModeEventLogId:
        case eCompletedDisinfectionModeEventLogId:
        case eChangeTargetTemperatureSettingEventLogId: 
        case eChangeTargetFlowSettingEventLogId:
        case eDateChangeEventLogId:
        case eChangeLanguageSettingEventLogId: 
        case eChangeIlluminanceSettingEventLogId:
        case eChangeIlluminanceLevelSettingEventLogId:
        case eBtSettingChangeEventLogId:
        case eWifiSettingChangeEventLogId:
        case eOxygenSourceSettingChangeUpperLimitEventLogId:
        case eSpO2AlarmSettingChangeLowerLimitEventLogId:
        case eOxygenAlarmSettingChangeUpperLimitEventLogId:
        case eOxygenAlarmSettingChangeLowerLimitEventLogId:
        case eChangeTouchSoundSettingsEventLogId: 
        case eMuteButtonStateChangeEventLogId:
        case eAcModeEventLogId:
        case eBatteryModeEventLogId:
        case eMainBatteryNotConnectedEventLogId:
        case eBatteryConnectedEventLogId:
        case eChangeSpeakerLevelSettingEventLogId:
            sprintf(strbuff, "%s", eventStringList[eventId]);
            break;
        default:
            break;
    }
    return;
}

void LogInterface_GetEventDataString(E_EventLogId eventId, uint8_t *data, char *strbuff)
{
    if (strbuff == 0 || data == 0)
        return;

    sprintf(strbuff, "%s", "");
    if (eventId < eFirstEventLogId || eventId >= eNoOfLogEventLogId)
    {
        return;
    }

    switch(eventId)
    {
        case eStartOperationEventLogId:
        case eStopOperationEventLogId:
        case eAcModeEventLogId:
        case eMainBatteryNotConnectedEventLogId:
        case eTurnOnDisinfectionModeEventLogId:
            break;
        case eTreatmentModeChangedEventLogId:
            if ((uint16_t)(data[0] << 8 | data[1]) == TREATMENT_MODE_A_TO_P)
            {
                strcat(strbuff, "A->P");
            }
            else if ((uint16_t)(data[0] << 8 | data[1]) == TREATMENT_MODE_P_TO_A)
            {
                strcat(strbuff, "P->A");
            }
            else
            {}
            break;            
        case eCompletedDisinfectionModeEventLogId:
            if (data[0] == DISFECTION_MODE_OK)
            {
                strcat(strbuff, "OK");
            }
            else if (data[0] == DISFECTION_MODE_ERROR)
            {
                strcat(strbuff, "ERR");
            }
            else
            {}
            break;
        case eChangeTargetFlowSettingEventLogId:
            sprintf(strbuff, "%dL->%dL", data[0], data[1]);
            break;
        case eDateChangeEventLogId:
            {
                sprintf(strbuff, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d\n%.2d/%.2d/%.2d %.2d:%.2d:%.2d", 
                        data[0],
                        data[1],
                        data[2],
                        data[3],
                        data[4],
                        data[5],
                        data[8],
                        data[9],
                        data[10],
                        data[11],
                        data[12],
                        data[13]
                        );
            break;
            }
        case eChangeLanguageSettingEventLogId: 
            if (data[0]>=0 && data[0] < eNoOfLanguage)
            {
                strcat(strbuff, "");
                strcat(strbuff, languageStringList[data[0]]);
            }
            else
            {}
            if (data[1]>=0 && data[1] < eNoOfLanguage  )
            {
                strcat(strbuff, "->");
                strcat(strbuff, languageStringList[data[1]]);
            }
            else
            {}
            break;
        case eChangeIlluminanceSettingEventLogId:
            if ((uint16_t)(data[0] << 8 | data[1]) == ILLUMINANCE_MODE_A_TO_M)
            {
                strcat(strbuff, "A->M");
            }
            else if ((uint16_t)(data[0] << 8 | data[1]) == ILLUMINANCE_MODE_M_TO_A)
            {
                strcat(strbuff, "M->A");
            }
            else
            {}
            break;
        case eBtSettingChangeEventLogId:
            if ((uint16_t)(data[0] << 8 | data[1]) == BLUETOOTH_OFF_TO_ON)
            {
                strcat(strbuff, "OFF->ON");
            }
            else if ((uint16_t)(data[0] << 8 | data[1]) == BLUETOOTH_ON_TO_OFF)
            {
                strcat(strbuff, "ON->OFF");
            }
            else
            {}
            break;
        case eWifiSettingChangeEventLogId:
            if ((uint16_t)(data[0] << 8 | data[1]) == WIFI_OFF_TO_ON)
            {
                strcat(strbuff, "OFF->ON");
            }
            else if ((uint16_t)(data[0] << 8 | data[1]) == WIFI_ON_TO_OFF)
            {
                strcat(strbuff, "ON->OFF");
            }
            else
            {}
            break;
        case eChangeTouchSoundSettingsEventLogId: 
            if ((uint16_t)(data[0] << 8 | data[1]) == TOUCH_SOUND_OFF_TO_ON)
            {
                strcat(strbuff, "OFF->ON");
            }
            else if ((uint16_t)(data[0] << 8 | data[1]) == TOUCH_SOUND_ON_TO_OFF)
            {
                strcat(strbuff, "ON->OFF");
            }
            else
            {}
            break;
        case eMuteButtonStateChangeEventLogId:
            if ((uint16_t)(data[0] << 8 | data[1]) == MUTE_STATE_OFF_TO_ON)
            {
                strcat(strbuff, "OFF->ON");
            }
            else if ((uint16_t)(data[0] << 8 | data[1]) == MUTE_STATE_ON_TO_OFF)
            {
                strcat(strbuff, "ON->OFF");
            }
            else
            {}
            break;
        case eBatteryModeEventLogId:
        case eBatteryConnectedEventLogId:
            sprintf(strbuff, "(%d%%)", data[0]);
            break;
        case eChangeTargetTemperatureSettingEventLogId: 
        case eChangeIlluminanceLevelSettingEventLogId:
        case eOxygenAlarmSettingChangeUpperLimitEventLogId:
        case eOxygenAlarmSettingChangeLowerLimitEventLogId:
        case eSpO2AlarmSettingChangeLowerLimitEventLogId:
        case eChangeSpeakerLevelSettingEventLogId:
            sprintf(strbuff, "%d->%d", data[0], data[1]);
            break;
        case eOxygenSourceSettingChangeUpperLimitEventLogId:
            sprintf(strbuff, "%d->%d", data[0]*10+90, data[1]*10+90); // convert 0->1 to 90->100
            break;
        default:
            break;
    }
    return;
}

/** @brief Get Alarm Name from id
 *  @param [in] E_LogId id: log id
 *  @param [out] None
 *  @return Alarm name.
 */
void LogInterface_GetAlarmStringFromID(E_AlarmId alarmId, char *strbuff)
{
    sprintf(strbuff, "%s", LOG_DATA_ERROR_STR);
    if (alarmId < eFirsAlarmId || alarmId >= eNoOfAlarmId)
    {
        return;
    }
    switch (alarmId)
    {
        case eI2C1ErrorAlarm:
        case eI2C2ErrorAlarm:
        case eI2C3ErrorAlarm:
        case eI2C4ErrorAlarm:
        case eSPI3ErrorAlarm:
        case eUart1ErrorAlarm:
        case eUart2ErrorAlarm:
        case eUart4ErrorAlarm:
        case eUart6ErrorAlarm:
        case eAirFlowSensorErrorAlarm:
        case eO2FlowSensorErrorAlarm:
        case eBME280ErrorAlarm:
        case eADXL345ErrorAlarmID:
        case eDRV8308ErrorAlarm:
        case eAudioErrorAlarm:
        case eMotorTaskErrorAlarm:
        case eHeaterTaskErrorAlarm:
        case eHumidityTaskErrorAlarm:
        case eBreathingCircuitNotConnectedAlarmId:
        case eCheckLeakAlarmId:
        case eCheckBlockageAlarmId:
        case eNonGenuineCircuitAlarmId:
        case eBreathingCircuitChangedAlarmId:
        case eExpiredCircuitAlarmId:
        case eCheckConnectionChamberAlarmId:
        case eNoMoreWaterInChamberAlarmId:
        case eRunOutOfWaterAlarmId:
        case eLowTemperatureAlarmId:
        case eHighTemperatureAlarmId:
        case eHighTemperatureAbnormalityAlarmId:
        case eRoomTemperatureLowToAchieveTargetTemperatureAlarmId:
        case eCheckOperatingConditionsAlarmID:
        case eDeviceErrorToAchieveTargetTemperatureAlarmId:
        case eOxygenHighAlarmId:
        case eOxygenLowAlarmId:
        case eDevicePostureAbnormalAlarmId:
        case eDevicePostureBadAlarmId:
        case eSwitchToBatteryModeAlarmId:
        case eBatteryLowAlarmId:
        case eBatteryGetsRunOutAlarmId:
        case eStopFunctionAlarmId:
        case ePowerNotEnoughAlarmId:
        case eSpO2FailedAlarmId:
        case eSpO2LowAlarmId:
        case eSpO2SensorProbeAlarmId:
        case eSpO2SignalLossAlarmId:
        case eSpO2SensorUnpluggedAlarmId:
        case eWaterSupplyCheckAlarmId:
        case eWaterTankConnectionCheckAlarmId:
        case eAirFlowAbnormalAlarmId:
        case eMainUnitBatteryDisconnectedAlarmId:
        case eCradleBatteryDisconnectedAlarmId:
        case eFailureOxygenFlowMeasurementFlowSensorAlarmId:
        case eFailureAirFlowMeasurementFlowSensorAlarmId:
        case eFailureChamberTemperatureSensorAlarmId:
        case eFailureCoilTemperatureSendorAlarmId:
        case eFailureBreathingCircuitOutTemperatureSensorAlarmId:
        case eFailureEnvironmentSensorAlarmId:
        case eFailureCurrentSensorAlarmId:
        case eFailureBlowerAlarmId:
        case eFailureWaterLevelSensorAlarmId:
        case eFailureExternalFlashMemoryAlarmId:
        case eFailureAccelerationSensorAlarmId:
        case eFailureLightSensorAlarmId:
        case eSpeakerDisconnectedAlarmId:
        case eMainUnitBatteryCommunicationErrorAlarmId:
        case eCradleBatteryCommunicationErrorAlarmId:
        case eCradleCommunicationErrorAlarmId:
        case eMainMCUFailedOrOutOfControlAlarmId:
        case eESP32FailedAlarmId:
        case eBreathingCircuitHeaterWireBrokenAlarmId:
        case eFailureSpo2ModuleAlarmId:
        case eFailureRTCModuleAlarmId:
        case eFailureLCDTouchModuleAlarmId:
//        case eFailureMainUnitBatteryManagementICAlarmId
//        case eFailureCradleBatteryManagementICAlarmId
        case eChamberTemperatureAbnormalAlarmId:
        case eTooMuchWaterInTheChamberAlarmId:
            sprintf(strbuff, "%s", alarmStringList[alarmId]);
            break;
        default:
            break;
    
    }
    return;
}

void LogInterface_GetAlarmStatusString(E_AlarmStatus alarmStatus, char *strbuff)
{
    sprintf(strbuff, "%s", LOG_DATA_ERROR_STR);
    if (alarmStatus < 0 || alarmStatus >= eNoOfAlarmStatus)
    {
        return;
    }
    switch (alarmStatus)
    {
        case eInactive:
        case ePaused:
        case eWaitingToActive:
        case eActive:
            sprintf(strbuff, "%s", alarmStatusStringList[alarmStatus]);
            break;
        default:
            break;
    }
    return;
}

void LogInterface_InitDebugLogFile()
{
#ifdef DEBUG_LOG_TO_FILE
    if (FileSystemMgr_IsSQIMounted() == false)
        return;
    
    if (logFile != SYS_FS_HANDLE_INVALID)
        return; 
//    SYS_PRINT("LogInterface_InitDebugLogFile \n");
    if ( logFileMutex == NULL )
    {
        logFileMutex = xSemaphoreCreateMutex();
    }
    logFile = SYS_FS_FileOpen(DEBUG_LOG_FILE, SYS_FS_FILE_OPEN_APPEND_PLUS);

#endif    
}

void LogInterface_DeInitDebugLogFile()
{
#ifdef DEBUG_LOG_TO_FILE
    if (logFile == SYS_FS_HANDLE_INVALID)
        return; 
    
    if(logFileMutex != NULL && xSemaphoreTake( logFileMutex, ( TickType_t ) 20 ) == pdTRUE )
    {
        SYS_FS_FileClose(logFile);
        logFile = SYS_FS_HANDLE_INVALID;
        xSemaphoreGive( logFileMutex );
    }
    else
    {
        SYS_PRINT("Error: Failed to take logFileMutex \n");
    }
#endif    
}

void LogInterface_WriteDebugLogFile(char * str)
{
#ifdef DEBUG_LOG_TO_FILE
    if (logFile == SYS_FS_HANDLE_INVALID)
        return; 
    if(logFileMutex != NULL && xSemaphoreTake( logFileMutex, ( TickType_t ) 20 ) == pdTRUE )
    {       
        Timestamp time = logMgr_getRtcTime();
        char buff[255];
        sprintf(buff, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d %s",
                time.year_2,
                time.month,
                time.date,
                time.hour,
                time.minute,
                time.second,
                str
                );

        if (SYS_FS_FileWrite(logFile, (void*)buff, strlen(buff)) != strlen(buff))
        {
            /* Failed to write to the file. */
            SYS_PRINT("\n Failed to write to the file \n");
            SYS_FS_FileClose(logFile);
            xSemaphoreGive( logFileMutex );
            return;
        }
        if (SYS_FS_FileSync(logFile) != SYS_FS_RES_SUCCESS)
        {
            /* Could not flush the contents of the file. Error out. */
            SYS_PRINT("\n Could not flush the contents of the file. Error out. \n");
            SYS_FS_FileClose(logFile);
            xSemaphoreGive( logFileMutex );
            return;
        }
        xSemaphoreGive( logFileMutex );
    }
    else
    {
        SYS_PRINT("Error: Failed to take logFileMutex \n");
    } 
#endif    
}
void LogInterface_ExportDebugLogFileToUSB()
{
#ifdef DEBUG_LOG_TO_FILE
    if (logFile == SYS_FS_HANDLE_INVALID)
        return; 

    if (USBInterface_CreateDir(SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0, "DebugLog") != SYS_FS_RES_SUCCESS)
    {
        SYS_PRINT("[Debug] Failed to init log dir on USB \n");
        return;
    }
    // read data on sqi flash
    void * data;
    if ( logFileMutex != NULL )
    {
        if( xSemaphoreTake( logFileMutex, ( TickType_t ) 20 ) == pdTRUE )
        {
            int32_t fileSize = SYS_FS_FileSize(logFile);
            data = (void*)mm_malloc(fileSize);
            SYS_FS_FileSeek(logFile, 0, SYS_FS_SEEK_SET);
            if (SYS_FS_FileRead(logFile, data, fileSize) == fileSize)
            {
                USBInterface_SetFileName("DebugLog/debug.log");
                USBInterface_Write(data, fileSize);
                USBInterface_FileSync();
            }       
            else
            {
                SYS_PRINT("[Debug] Error read debug file %d \n", SYS_FS_Error());
            }
            xSemaphoreGive( logFileMutex );
        }
        else
        {
            SYS_PRINT("Error: Failed to take logFileMutex \n");
        }
    }
    else
    {
        SYS_PRINT("Error: logFileMutex is NULL \n");
    }    
#endif    
}
void LogInterface_ClearDebugLogFile()
{
#ifdef DEBUG_LOG_TO_FILE
    if ( logFileMutex != NULL )
    {
        if( xSemaphoreTake( logFileMutex, ( TickType_t ) 20 ) == pdTRUE )
        {
            SYS_FS_FileSeek(logFile, 0, SYS_FS_SEEK_SET);
            SYS_FS_FileTruncate(logFile);
            SYS_PRINT("\n [Debug] Debug log file size : %d \n", SYS_FS_FileSize(logFile));
            xSemaphoreGive( logFileMutex );
        }
        else
        {
            SYS_PRINT("Error: Failed to take logFileMutex \n");
        }
    }
    else
    {
        SYS_PRINT("Error: logFileMutex is NULL \n");
    }   
#endif    
}

uint32_t LogInterface_GetRunningTime()
{
    return DisplayControl_GetRunningTime();
}

uint16_t LogInterface_GetTotalLog()
{
    logMgr_GetNumberOfLog(eEventLogTypeID);
}

void LogInterface_GetLogContent(uint8_t id, Log_Struct *log)
{
    logMgr_GetLogAtIndex(eEventLogTypeID, id, log);
}

uint16_t LogInterface_GetTotalAlarm()
{
    logMgr_GetNumberOfLog(eAlarmLogTypeID);
}

void LogInterface_GetAlarmContent(uint8_t id, Log_Struct *log)
{
    logMgr_GetLogAtIndex(eAlarmLogTypeID, id, log);
}

void LogInterface_TestLogEvent()
{
   uint8_t data[16];
   //eStartOperationEventLogId
   logInterface_WriteEventLog(0, NULL , eStartOperationEventLogId);
    
   //eStopOperationEventLogId
   logInterface_WriteEventLog(0, NULL , eStopOperationEventLogId);
    
   //eTreatmentModeChangedEventLogId
    data[0] = TREATMENT_MODE_A_TO_P >> 8;
    data[1] = (uint8_t)TREATMENT_MODE_A_TO_P;
   logInterface_WriteEventLog(2, data , eTreatmentModeChangedEventLogId);
    
    data[0] = TREATMENT_MODE_P_TO_A >> 8;
    data[1] = (uint8_t)TREATMENT_MODE_P_TO_A;
   logInterface_WriteEventLog(2, data , eTreatmentModeChangedEventLogId);
    
   //eTurnOnDisinfectionModeEventLogId
   logInterface_WriteEventLog(0, NULL , eTurnOnDisinfectionModeEventLogId);
    
   //eCompletedDisinfectionModeEventLogId
   data[0] = DISFECTION_MODE_OK;
   logInterface_WriteEventLog(1, data , eCompletedDisinfectionModeEventLogId);
    
   data[0] = DISFECTION_MODE_ERROR;
   logInterface_WriteEventLog(1, data , eCompletedDisinfectionModeEventLogId);
    
   //eChangeTargetTemperatureSettingEventLogId
   data[0] = 10;
   data[1] = 25;
   logInterface_WriteEventLog(2, data , eChangeTargetTemperatureSettingEventLogId);
    
   data[0] = 50;
   data[1] = 0;
   logInterface_WriteEventLog(2, data , eChangeTargetTemperatureSettingEventLogId);
    
   //eChangeTargetFlowSettingEventLogId
   data[0] = 10;
   data[1] = 25;
   logInterface_WriteEventLog(2, data , eChangeTargetFlowSettingEventLogId);
    
   data[0] = 50;
   data[1] = 0;
   logInterface_WriteEventLog(2, data , eChangeTargetFlowSettingEventLogId);


   //eDateChangeEventLogId
   data[0] = 20;
   data[1] = 21;
   data[2] = 03;
   data[3] = 13;
   data[4] = 22;
   data[5] = 11;
   data[6] = 00;
   
   data[7] = 20;
   data[8] = 22;
   data[9] = 03;
   data[10] = 10;
   data[11] = 44;
   data[12] = 55;
   data[13] = 66;
   logInterface_WriteEventLog(14, data , eDateChangeEventLogId);

    
   //eChangeLanguageSettingEventLogId
   data[0] = eJapanese;
   data[1] = eEnglish;
   logInterface_WriteEventLog(2, data , eChangeLanguageSettingEventLogId);
   
   data[0] = eEnglish;
   data[1] = eVietnamese;
   logInterface_WriteEventLog(2, data , eChangeLanguageSettingEventLogId);

   data[0] = eVietnamese;
   data[1] = eJapanese;
   logInterface_WriteEventLog(2, data , eChangeLanguageSettingEventLogId);

   //eChangeIlluminanceSettingEventLogId
//    auto to manual
   data[0] = 0xFF; 
   data[1] = 0; 
   logInterface_WriteEventLog(2, data , eChangeIlluminanceSettingEventLogId);
   
//    manual to auto
   data[0] = 0; 
   data[1] = 0xFF; 
   logInterface_WriteEventLog(2, data , eChangeIlluminanceSettingEventLogId);
    
   //eChangeIlluminanceLevelSettingEventLogId
   data[0] = 0; 
   data[1] = 5; 
   logInterface_WriteEventLog(2, data , eChangeIlluminanceLevelSettingEventLogId);

   //eBtSettingChangeEventLogId
//    off to on
   data[0] = 0; 
   data[1] = 0xFF; 
   logInterface_WriteEventLog(2, data , eBtSettingChangeEventLogId);

//    on to off
   data[0] = 0xFF; 
   data[1] = 0; 
   logInterface_WriteEventLog(2, data , eBtSettingChangeEventLogId);

   //eWifiSettingChangeEventLogId
//    off to on
   data[0] = 0; 
   data[1] = 0xFF; 
   logInterface_WriteEventLog(2, data , eWifiSettingChangeEventLogId);

//    on to off
   data[0] = 0xFF; 
   data[1] = 0; 
   logInterface_WriteEventLog(2, data , eWifiSettingChangeEventLogId);

   //eUpperLimitOfOxygenAlarmSettingChangeEventLogId
   data[0] = 20; 
   data[1] = 30; 
   logInterface_WriteEventLog(2, data , eOxygenAlarmSettingChangeUpperLimitEventLogId);   

   //eLowerLimitOfOxygenAlarmSettingChangeEventLogId
   data[0] = 30; 
   data[1] = 10; 
   logInterface_WriteEventLog(2, data , eOxygenAlarmSettingChangeLowerLimitEventLogId);

   //eChangeTouchSoundSettingsEventLogId
   //off to on
   data[0] = 0; 
   data[1] = 0xFF; 
   logInterface_WriteEventLog(2, data , eChangeTouchSoundSettingsEventLogId);

   //on to off
   data[0] = 0xFF; 
   data[1] = 0; 
   logInterface_WriteEventLog(2, data , eChangeTouchSoundSettingsEventLogId);

   //eMuteButtonStateChangeEventLogId
   //off to on
   data[0] = 0; 
   data[1] = 0xFF; 
   logInterface_WriteEventLog(2, data , eMuteButtonStateChangeEventLogId);

   //on to off
   data[0] = 0xFF; 
   data[1] = 0; 
   logInterface_WriteEventLog(2, data , eMuteButtonStateChangeEventLogId);

   //eAcModeEventLogId
   logInterface_WriteEventLog(0, NULL , eAcModeEventLogId);
    
   //eBatteryModeEventLogId
   data[0] = 50; 
   logInterface_WriteEventLog(1, data , eBatteryModeEventLogId);
    
   //eBatteryNotConnectedEventLogId
   logInterface_WriteEventLog(0, NULL , eMainBatteryNotConnectedEventLogId);
    
   //eBatteryConnectedEventLogId
   data[0] = 70; 
   logInterface_WriteEventLog(1, data , eBatteryConnectedEventLogId);

}

/* end of file */
