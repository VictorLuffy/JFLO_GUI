/** @file LogMgr.c
 *  @brief Manager of JFLO log
 *  @author Viet Le
 */

/* This section lists the other files that are included in this file.
 */
#include "system_config.h"
#include "system_definitions.h"

#include "system/debug/sys_debug.h"

#include "Alarm/AlarmInterface.h"

#include "Delay.h"
#include "Device/RTC_BQ32002.h"
#include "System/USBInterface.h"
#include "System/SQIInterface.h"

#include "Gui/File.h"
#include "Gui/LogMgr.h"
#include "Gui/memlib.h"
#include "Gui/mm.h"
#include "Gui/LogInterface.h"
#include "7z/7zFile.h"
#include "GuiDefine.h"

// Declare event file
extern SYS_FS_HANDLE g_eventLogFile;
// Declare alarm file
extern SYS_FS_HANDLE g_alarmLogFile;
// Declare setting file
extern SYS_FS_HANDLE g_settingFile;
// Declare Device infomation file
extern SYS_FS_HANDLE g_devInfoFile;
// Declare SpO2 data file
extern SYS_FS_HANDLE g_Spo2DataFile;

extern SYS_FS_HANDLE logFile;

/** @brief Declare Log Event Object */
static LogObject_Struct gs_eventLogObj = 
{
    .type = eEventLogTypeID,
    .status = eSuspendedLogStatus,
       
    .s_currentIndex = 0,
    
    .s_numLogSaved = 0, 
    
    .is_CopingFiletoUSB = 0,
    
    .fileHandle = &g_eventLogFile,
    .fileName = FILE_EVENTLOG_NAME,
    .usbFileName = "Log/Event.log",
};

/** @brief Declare Alarm Event Object */
static LogObject_Struct gs_alarmLogObj = 
{
    .type = eAlarmLogTypeID,
    .status = eSuspendedLogStatus,
       
    .s_currentIndex = 0,
    
    .s_numLogSaved = 0, 
    
    .is_CopingFiletoUSB = 0,
    
    .fileHandle = &g_alarmLogFile,
    .fileName = FILE_ALARMLOG_NAME,
    .usbFileName = "Log/Alarm.log",
};

/** @brief Declare Spo2Data Log Object */
static LogObject_Struct gs_spo2LogObj = 
{
    .type = eSpo2DataLogTypeID,
    .status = eSuspendedLogStatus,
       
    .s_currentIndex = 0,
    
    .s_numLogSaved = 0, 
    
    .is_CopingFiletoUSB = 0,
    
    .fileHandle = &g_Spo2DataFile,
    .fileName = FILE_SPO2DATA_NAME,
    .usbFileName = "Log/SpO2.log",
};

/** @brief Declare queue receive ID data */
QueueHandle_t g_logQueueReceiveEvent;

/** @brief Declare queue receive ID data */
QueueHandle_t g_logQueueReceiveAlarm;

/** @brief Declare queue receive ID data */
QueueHandle_t g_logQueueReceiveDeviceInformation;

/** @brief Declare queue receive ID data */
QueueHandle_t g_logQueueReceiveSpO2;

/** @brief Declare queue receive from Request*/
QueueHandle_t g_logQueueForRequest;

/* Get Log Object of one log type */
static LogObject_Struct* LogMgr_GetLogObj(E_LogType type);

E_LogStatus logMgr_GetLogStatus(E_LogType logtype)
{
    LogObject_Struct* tmp_LogObj;
    tmp_LogObj = LogMgr_GetLogObj(logtype);

    return tmp_LogObj->status;
}

/** @brief Function to Copy All log to DDR RAM for storage
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void logMgr_HandleEvent(void)
{
    Log_Struct log;
    if (gs_eventLogObj.is_CopingFiletoUSB == true)
        return;

    //wait for queue event
    if (xQueueReceive(g_logQueueReceiveEvent, &log, 0) == pdPASS ) //wait 0 tick (do not wait)
    {
//        SYS_PRINT("\n logMgr_HandleEvent-----log.eCode = %d", log.eCode);
        logMgr_WriteLogToSQI(log);
    }
    return;
}


/** @brief Function to Copy All log to DDR RAM for storage
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void logMgr_HandleAlarm(void)
{
    Log_Struct log;

    if (gs_alarmLogObj.is_CopingFiletoUSB == true)
        return;
    
    //wait for queue event
    if (xQueueReceive(g_logQueueReceiveAlarm, &log, 0) == pdTRUE) //wait 0 tick (do not wait)
        logMgr_WriteLogToSQI(log);
    
    return;
}

/** @brief Use in logTask function to handle when spo2 was received
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void logMgr_HandleSpO2(void)
{
    Log_Struct log;
    if (gs_eventLogObj.is_CopingFiletoUSB == true)
        return;
    
    //wait for queue event
    if (xQueueReceive(g_logQueueReceiveSpO2, &log, 0) == pdTRUE) //wait 0 tick (do not wait)
        logMgr_WriteLogToSQI(log);

    return;
}


/** @brief Use in logTask function to handle when logRequest was received
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void logMgr_HandleRequest(void )
{
    E_LogRequestID logRequest;

    uint32_t totalWorkingTime, sec;
    
    if (xQueueReceive(g_logQueueForRequest, &logRequest, 0) == pdTRUE) //wait 0 tick (do not wait)
    {
        switch (logRequest)
        {
            case eUSBGetSettingLogRequestId  :
            {
                logMgr_ExportLogFromSQIFlashtoUSB(eSettingsLogTypeID);
                break;
            }                 
            case eUSBGetDeviceInfoLogRequestId:
            {
                logMgr_ExportLogFromSQIFlashtoUSB(eDeviceInformationLogTypeID);
                break;
            }                 
            case eUSBGetEventLogRequestId:
            {
                logMgr_ExportLogFromSQIFlashtoUSB(eEventLogTypeID);
                break;
            }         
            case eUSBGetAlarmLogRequestId:
            {
                logMgr_ExportLogFromSQIFlashtoUSB(eAlarmLogTypeID);
                break;
            }              
            case eUSBGetSpO2LogRequestId:
            {
                logMgr_ExportLogFromSQIFlashtoUSB(eSpo2DataLogTypeID);
                break;
            } 
            case eDeleteSettingLogRequestId:
            {
                logMgr_ClearLog(eSettingsLogTypeID);
                break;
            }
            case eDeleteDeviceInfoLogRequestId:    
            {
                logMgr_ClearLog(eDeviceInformationLogTypeID);
                break;
            }                
            case eDeleteEventLogRequestId:
            {
                logMgr_ClearLog(eEventLogTypeID);
                break;
            }
            case eDeleteAlarmLogRequestId:
            {
                logMgr_ClearLog(eAlarmLogTypeID);
                break;
            }            
            case eDeleteSpO2LogRequestId: 
            {
                logMgr_ClearLog(eSpo2DataLogTypeID);
                break;
            }
            
            default:
                break;

        }
    }    
}

/* Get Log Object of one log type */
static LogObject_Struct* LogMgr_GetLogObj(E_LogType type)
{
    if (type == eEventLogTypeID) {
        return &gs_eventLogObj;
    }
    else if (type == eAlarmLogTypeID) {
        return &gs_alarmLogObj;
    } else if (type == eSpo2DataLogTypeID)
        return &gs_spo2LogObj;
    else
        return NULL;
}

/** @brief Write log to SQI Flash
 *  @param [in] Log_Struct log: log write to SQI
 *  @param [out] : None
 *  @return None
 */
void logMgr_WriteLogToSQI(Log_Struct log)
{
    LogObject_Struct* tmp_LogObj;
    tmp_LogObj = LogMgr_GetLogObj(log.logType);
    int dataIndex;
    long fileSize = file_Size(*tmp_LogObj->fileHandle);
    
    if ( fileSize == -1 )
        return;
    long max_log = 0;
    uint8_t logEvent[LOG_LEN];
    memset(&logEvent[0], 0,  LOG_LEN); // add "\n"
    
    if (tmp_LogObj->type == eEventLogTypeID) 
    {
        max_log = MAX_EVENT_LOG;
        logEvent[0] = HEADER_EVENT_LOG;
        logEvent[1] = log.time.year_1;
        logEvent[2] = log.time.year_2;
        logEvent[3] = log.time.month;
        logEvent[4] = log.time.date;
        logEvent[5] = log.time.hour;
        logEvent[6] = log.time.minute; //add hour
        logEvent[7] = log.time.second; //add hour
        logEvent[8] = log.eCode;
        
        for(dataIndex = 0; dataIndex < 16; dataIndex++)
        {
            logEvent[9 + dataIndex] = *((uint8_t*)(log.data + dataIndex));
        }
    } 
    else if (tmp_LogObj->type == eAlarmLogTypeID)
    {
        max_log = MAX_ALARM_LOG;
        logEvent[0] = HEADER_ALARM_LOG;
        logEvent[1] = log.time.year_1;
        logEvent[2] = log.time.year_2;
        logEvent[3] = log.time.month;
        logEvent[4] = log.time.date;
        logEvent[5] = log.time.hour;
        logEvent[6] = log.time.minute; //add hour
        logEvent[7] = log.time.second; //add hour
        logEvent[8] = log.eCode;
        
        for(dataIndex = 0; dataIndex < 16; dataIndex++)
        {
            logEvent[9 + dataIndex] = *((uint8_t*)(log.data + dataIndex));
        }
    }
    else if (tmp_LogObj->type == eSpo2DataLogTypeID) 
    {
        max_log = MAX_SPO2_LOG;
        logEvent[0] = HEADER_SPO2_LOG;
        logEvent[1] = log.time.year_1;
        logEvent[2] = log.time.year_2;
        logEvent[3] = log.time.month;
        logEvent[4] = log.time.date;
        logEvent[5] = log.time.hour;
        logEvent[6] = log.time.minute;
        logEvent[7] = log.time.second;
        logEvent[8] = log.data[0];
    }
    else {
        return;
    }
    
    //Move file pointer
    if (tmp_LogObj->s_numLogSaved < max_log)
    {
        file_Seek(*tmp_LogObj->fileHandle, fileSize, SYS_FS_SEEK_SET);
    }
    else
    {
        file_Seek(*tmp_LogObj->fileHandle, INFOR_LOG_SIZE + (tmp_LogObj->s_currentIndex * LOG_LEN), SYS_FS_SEEK_SET);
    }

    //Write to file
    file_Write(logEvent, LOG_LEN, *tmp_LogObj->fileHandle);

    //Update information log
    tmp_LogObj->s_numLogSaved++;
    
    if (tmp_LogObj->s_numLogSaved > max_log)
        tmp_LogObj->s_numLogSaved = max_log;

    tmp_LogObj->s_currentIndex++;
    
    if (tmp_LogObj->s_currentIndex >= max_log)
        tmp_LogObj->s_currentIndex = 0;
    
    logMgr_UpdateInforLog(log.logType);
        
    return;
}

/** @brief Act the behavior when Copy file FromSQIFlashtoUSB command was received.
 *  @param [in] type Log Type that want to clear in file
 *  @param [out] None
 *  @return None
 */
void logMgr_ExportLogFromSQIFlashtoUSB(E_LogType type)
{
    LogObject_Struct* tmp_LogObj;
    tmp_LogObj = LogMgr_GetLogObj(type);
    
    tmp_LogObj->is_CopingFiletoUSB = true;   
   
    // Init log dir
    if (USBInterface_CreateDir(SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0, "Log") != SYS_FS_RES_SUCCESS)
    {
        SYS_PRINT("[Debug] Failed to init log dir on USB \n");
        return;
    }    
    
    USBInterface_SetFileName(tmp_LogObj->usbFileName);
    
    uint16_t numLog, currentIndex;
    numLog = logMgr_GetNumberOfLog(type);
    currentIndex = logMgr_GetCurrentIndex(type);

    char strbuff[255] = "";
    char strbuffx[255] = LOG_DATA_ERROR_STR;
    // Write index, numlog
    sprintf(strbuff, "NUMBER OF LOG: %5d\n", numLog);
    USBInterface_Write(strbuff, strlen(strbuff));
    sprintf(strbuff, "CURRENT INDEX: %5d\n", currentIndex);
    USBInterface_Write(strbuff, strlen(strbuff));
    
    int index = 0;
    for (index = 0 ; index < numLog ; index ++)
    {
        Log_Struct logData;
        memset(&logData, 0, sizeof(Log_Struct));
        logData.eCode = 0xff;
        logMgr_GetLogAtIndex(type, index, &logData);
        if (type == eEventLogTypeID)
        {
            LogInterface_GetEventStringFromID(logData.eCode, strbuffx);
        }
        else if(type == eAlarmLogTypeID)
        {
            LogInterface_GetAlarmStringFromID(logData.eCode, strbuffx);
        }
        else if(type == eSpo2DataLogTypeID)
        {
            sprintf(strbuffx, "%d", logData.data[0]);
        }
        else
        {
            return;
        }
        sprintf(strbuff, "%.5d %.2d%.2d/%.2d/%.2d %.2d:%.2d:%.2d %s\n", 
            index +1,
            logData.time.year_1, 
            logData.time.year_2, 
            logData.time.month, 
            logData.time.date,
            logData.time.hour, 
            logData.time.minute,
            logData.time.second,
            strbuffx
            );       
        USBInterface_Write(strbuff, strlen(strbuff));
    }

    USBInterface_FileSync();

    tmp_LogObj->is_CopingFiletoUSB = false;
    SYS_PRINT("logMgr_ExportLogFromSQIFlashtoUSB %d \n", (uint8_t)type);
    return;
}

/** @brief Set current index
 *  @param [in] type Log Type that want to clear in file
 *  @param [out] None
 *  @return None
 */
void logMgr_SetCurrentIndex(E_LogType type)
{
    LogObject_Struct* tmp_LogObj;
    
    /* Information log */
    tmp_LogObj = LogMgr_GetLogObj(type);  
    
    tmp_LogObj->s_currentIndex = tmp_LogObj->s_logInfor.currentIndex;
    return;
}

/** @brief Get current index
 *  @param [in] type Log Type that want to clear in file
 *  @param [out] None
 *  @return uint16_t: current index
 */
uint16_t logMgr_GetCurrentIndex(E_LogType type)
{
    LogObject_Struct* tmp_LogObj;
    
    /* Information log */
    tmp_LogObj = LogMgr_GetLogObj(type);        
    
    return tmp_LogObj->s_currentIndex;
}

/** @brief Get number of log
 *  @param [in] type Log Type that want to clear in file
 *  @param [out] None
 *  @return uint16_t: number of log
 */
uint16_t logMgr_GetNumberOfLog(E_LogType type)
{
    LogObject_Struct* tmp_LogObj;
    
    /* Information log */
    tmp_LogObj = LogMgr_GetLogObj(type);         
    
    return tmp_LogObj->s_numLogSaved;       
}

/** @brief Get next log
 *  @param [in] type Log Type that want to clear in file
 *  @param [in] int index: index log
 *  @param [in] Log_Struct *log: log data
 *  @param [out] None
 *  @return None
 */
void logMgr_GetLogAtIndex(E_LogType type, int index, Log_Struct *log)
{
    LogObject_Struct* tmp_LogObj;
    tmp_LogObj = LogMgr_GetLogObj(type);   
    
    if (tmp_LogObj->fileHandle == NULL)
    {
        SYS_PRINT("Error: log file handle NULL \n");
        return;
    }       
    uint8_t data[LOG_LEN];
    tmp_LogObj->fileSize = file_Size(*tmp_LogObj->fileHandle);
    
    /*  Check header    */ 
    int pos = index * LOG_LEN + INFOR_LOG_SIZE;
    if (pos > tmp_LogObj->fileSize - (LOG_LEN))
    {
        SYS_PRINT("E_LogType %d \n", type);
        SYS_PRINT("fileSize %d \n", tmp_LogObj->fileSize);
        SYS_PRINT("Error: log id error %d \n", index);
        return;
    }
    // Jump to log location
    file_Seek(*tmp_LogObj->fileHandle, pos, SYS_FS_SEEK_SET);
    if (SYS_FS_FileRead(*tmp_LogObj->fileHandle, data, LOG_LEN) != LOG_LEN)
    {
        SYS_PRINT("SYS_FS_FileRead error %d \n", SYS_FS_Error());
        return;
    }
    // Resume cursor to end of file
    file_Seek(*tmp_LogObj->fileHandle, 0, SYS_FS_SEEK_END);
    
    if (data[0] == HEADER_EVENT_LOG)
    {
        log->logType = eEventLogTypeID;
        
        log->time.year_1 = data[1];
        log->time.year_2 = data[2];
        log->time.month = data[3];
        log->time.date = data[4];
        log->time.hour = data[5];
        log->time.minute = data[6];
        log->time.second = data[7];
        log->eCode = data[8];
        memcpy(log->data, data + 9, 16);      
    } 

    else if (data[0] == HEADER_ALARM_LOG)
    {
        log->logType = eAlarmLogTypeID;
        
        log->time.year_1 = data[1];
        log->time.year_2 = data[2];
        log->time.month = data[3];
        log->time.date = data[4];
        log->time.hour = data[5];
        log->time.minute = data[6];
        log->time.second = data[7];
        log->eCode = data[8];
        memcpy(log->data, data + 9, 16);         
    }     
    
    else if (data[0] == HEADER_SPO2_LOG)
    {
        log->logType = eSpo2DataLogTypeID;      
        log->time.year_1 = data[1];
        log->time.year_2 = data[2];
        log->time.month = data[3];
        log->time.date = data[4];
        log->time.hour = data[5];
        log->time.minute = data[6];
        log->time.second = data[7];
        log->data[0] = data[8];
    }   
    
    return;
}


/** @brief Update information log
 *  @param [in] type Log Type that want to clear in file
 *  @param [out] None
 *  @return None
 */
void logMgr_UpdateInforLog(E_LogType type)
{
    LogObject_Struct* tmp_LogObj;
    
    /* Information log */
    tmp_LogObj = LogMgr_GetLogObj(type); 
    
    uint16_t logInfor[INFOR_LOG_LENGTH] = {0};

    /* Update information log */
    logInfor[0] = tmp_LogObj->s_numLogSaved;
    logInfor[1] = tmp_LogObj->s_currentIndex;


    /* Move file pointer */
    file_Seek(*tmp_LogObj->fileHandle, 0, SYS_FS_SEEK_SET);

    /* Write to log file */
    file_Write(logInfor, sizeof (logInfor), *tmp_LogObj->fileHandle);

    return;
}

/** @brief Delete all data in one log file
 *  @param [in] type Log Type that want to clear in file
 *  @param [out] None
 *  @return None
 */
void logMgr_ClearLog(E_LogType type)
{
    LogObject_Struct* tmp_LogObj;
    
    /* Information log */
    tmp_LogObj = LogMgr_GetLogObj(type); 
    
    // Action
    if (((type == eEventLogTypeID) || (type == eAlarmLogTypeID) || (type == eSpo2DataLogTypeID))
            && (tmp_LogObj->s_numLogSaved > 0)) 
    {
        /* Truncates file */
        file_Truncates(*tmp_LogObj->fileHandle);

        /* Update information log */
        tmp_LogObj->s_numLogSaved = 0;
        tmp_LogObj->s_currentIndex = 0;  

        logMgr_UpdateInforLog(type);
        
    } else if (type == eDeviceInformationLogTypeID) {
        file_Truncates(g_devInfoFile);
        
    } else if (type == eSettingsLogTypeID) {
        file_Truncates(g_settingFile);
    }
    return;
}

/** @brief Get current RTC time.
 *  @param [in] None
 *  @param [out] None
 *  @return Current Timestamp
 */
Timestamp logMgr_getRtcTime(void)
{
    RTC_TIME_t time;
    rtc_GetTime(&time);
    
    Timestamp logTime = {
        .year_1 = 20,
        .year_2 = time.YEAR,
        .month = time.MONTH,
        .date = time.DATE,
        .hour = time.HOUR,
        .minute = time.MIN,
        .second = time.SEC
    };
    
    return logTime;
}

/** @brief Write log to SQI Flash
 *  @param [in] Log_Struct log: log write to SQI
 *  @param [out] : None
 *  @return None
 */
void logMgr_InitOperatingLog(E_LogType type)
{
    // Check type
    LogObject_Struct* tmp_LogObj;
    tmp_LogObj = LogMgr_GetLogObj(type); 
    
    // Action
    uint16_t logInfor[INFOR_LOG_LENGTH] = {0};
    int logFileSize = file_Size(*tmp_LogObj->fileHandle);

    if (logFileSize <= 0)
    {
        //Move file pointer
        file_Seek(*tmp_LogObj->fileHandle, 0, SYS_FS_SEEK_SET);

        //Write to log file
        file_Write(logInfor, sizeof (logInfor), *tmp_LogObj->fileHandle);
    }
    else
    {
        //Read data from SQI flash
        file_Read(logInfor, sizeof (logInfor), *tmp_LogObj->fileHandle);

        //Get information log
        tmp_LogObj->s_numLogSaved = logInfor[0];
        tmp_LogObj->s_currentIndex = logInfor[1];

    }

    tmp_LogObj->status = eReadyLogStatus;

    return;
}

/** @brief Write log to SQI Flash
 *  @param [in] Log_Struct log: log write to SQI
 *  @param [out] : None
 *  @return None
 */
void logMgr_Initialize(void)
{
    SYS_PRINT("logMgr_Initialize \n"); 
    
    /* Declare queue receive ID data */
    g_logQueueReceiveDeviceInformation = xQueueCreate(LOG_QUEUE_RECEIVE_LENGTH, sizeof (DeviceInformationLog_Struct));
    
    /* Declare queue receive ID data */
    g_logQueueReceiveEvent = xQueueCreate(LOG_QUEUE_RECEIVE_LENGTH, sizeof (Log_Struct));
    
    /* Declare queue receive ID data */
    g_logQueueReceiveAlarm = xQueueCreate(LOG_QUEUE_RECEIVE_LENGTH, sizeof (Log_Struct));

    /* Declare queue receive ID data */
    g_logQueueReceiveSpO2 = xQueueCreate(LOG_QUEUE_RECEIVE_LENGTH, sizeof (Log_Struct));
    
    /* Declare queue receive from Request*/
    g_logQueueForRequest = xQueueCreate(LOG_QUEUE_RECEIVE_LENGTH, sizeof (E_LogRequestID));   

    // Initialize operating log
    logMgr_InitOperatingLog(eEventLogTypeID);
    logMgr_InitOperatingLog(eAlarmLogTypeID);
    logMgr_InitOperatingLog(eSpo2DataLogTypeID);
    
    
    logMgr_ReduceLogOver30OlderDay(eSpo2DataLogTypeID);
    
    return;
}


/** @brief Log manager working 
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void logMgr_Task(void)
{
    if(gs_eventLogObj.status == eReadyLogStatus 
            && gs_alarmLogObj.status == eReadyLogStatus
            && gs_spo2LogObj.status == eReadyLogStatus )
    {       
        logMgr_HandleEvent();
        
        logMgr_HandleAlarm();
        
        logMgr_HandleSpO2();
        
        logMgr_HandleRequest();
    }
    
    return;
}

uint16_t logMgr_ReadLastestLog(E_LogType type, uint16_t noOfLog, uint8_t *data)
{   
    LogObject_Struct* tmp_LogObj;
    tmp_LogObj = LogMgr_GetLogObj(type);   
    uint32_t totalLogNumber = 0;
    uint32_t currentLogIndex = 0;
    if (tmp_LogObj->fileHandle == NULL)
    {
        SYS_PRINT("Error: log file handle NULL \n");
        return 0;
    }       
    totalLogNumber = tmp_LogObj->s_numLogSaved;
    currentLogIndex = tmp_LogObj->s_currentIndex;
    if ( totalLogNumber == 0)
    {
        SYS_PRINT("Error: log is NULL \n");
        return 0;
    }

    if (noOfLog > totalLogNumber )
    {
        noOfLog = totalLogNumber;
    }

    int i = 0;
    int index = 0;
    for (i = 0; i < noOfLog; i ++ )
    {
        index = (currentLogIndex - 1) - i;
        if (index < 0)
        {
            long max_log = (tmp_LogObj->type == eAlarmLogTypeID) ? MAX_ALARM_LOG : MAX_EVENT_LOG;
            index += max_log;
        }
//        SYS_PRINT("logMgr_ReadLastestLog i %d index %d \n", i , index);
        
        int pos = index * LOG_LEN + INFOR_LOG_SIZE;
        file_Seek(*tmp_LogObj->fileHandle, pos, SYS_FS_SEEK_SET);
        if (SYS_FS_FileRead(*tmp_LogObj->fileHandle, &data[i*LOG_LEN], LOG_LEN) != LOG_LEN)
        {
            SYS_PRINT("SYS_FS_FileRead error %d \n", SYS_FS_Error());
            return i;
        }
    }
    // Resume cursor to end of file
    file_Seek(*tmp_LogObj->fileHandle, 0, SYS_FS_SEEK_END); 
    return noOfLog;
}

void logMgr_GetLogFromDataArray(E_LogType type, int index, uint8_t *data, Log_Struct *log)
{
    uint8_t log_record[LOG_LEN];

    int pos = index * LOG_LEN;
    memcpy(log_record, &data[index * LOG_LEN], LOG_LEN);

    if (log_record[0] == HEADER_EVENT_LOG)
    {
        log->logType = eEventLogTypeID;
        
        log->time.year_1 = log_record[1];
        log->time.year_2 = log_record[2];
        log->time.month = log_record[3];
        log->time.date = log_record[4];
        log->time.hour = log_record[5];
        log->time.minute = log_record[6];
        log->time.second = log_record[7];
        log->eCode = log_record[8];
        memcpy(log->data, log_record + 9, 16);      
    } 

    else if (data[0] == HEADER_ALARM_LOG)
    {
        log->logType = eAlarmLogTypeID;
        
        log->time.year_1 = log_record[1];
        log->time.year_2 = log_record[2];
        log->time.month = log_record[3];
        log->time.date = log_record[4];
        log->time.hour = log_record[5];
        log->time.minute = log_record[6];
        log->time.second = log_record[7];
        log->eCode = log_record[8];
        memcpy(log->data, log_record + 9, 16);         
    }     
    
    else if (data[0] == HEADER_SPO2_LOG)
    {
        log->logType = eSpo2DataLogTypeID;      
    }   
    
    return;
}

void logMgr_BackupFileToUSB(SYS_FS_HANDLE fileHandle, const char* fileName)
{
    if (fileHandle == SYS_FS_HANDLE_INVALID)
        return;
 
    if (USBInterface_CreateDir(SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0, "Backup") != SYS_FS_RES_SUCCESS)
    {
        SYS_PRINT("[Debug] Failed to init log dir on USB \n");
        return;
    }
    char filePath[255];
    strcpy(filePath, SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0);
    strcat(filePath, "/Backup/");
    strcat(filePath, fileName);
    
    SYS_PRINT("Backing up %s -> %s \n", fileName, filePath);
    SYS_FS_HANDLE fileHandleDest = SYS_FS_FileOpen(filePath, SYS_FS_FILE_OPEN_WRITE);
    if (fileHandleDest == SYS_FS_HANDLE_INVALID)
    {
        SYS_PRINT("file_BackupToUSB error %d \n" , SYS_FS_Error());
        return;
    }
   
    uint32_t fileSize = file_Size(fileHandle);
    uint8_t *data = (uint8_t *)mm_malloc(fileSize);
    file_Seek(fileHandle, 0, SYS_FS_SEEK_SET);
    file_Read(data, fileSize, fileHandle);
    
    file_Write(data, fileSize, fileHandleDest);
    SYS_FS_FileClose(fileHandleDest);
    mm_free(data);
}

void logMgr_RestoreFileFromUSB(SYS_FS_HANDLE fileHandle, const char* fileName)
{     
    if (fileHandle == SYS_FS_HANDLE_INVALID)
        return;
    char filePath[255];
    strcpy(filePath, SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0);
    strcat(filePath, "/Backup/");
    strcat(filePath, fileName);
    SYS_PRINT("Restoring %s -> %s \n", filePath, fileName);
    SYS_FS_HANDLE fileHandleSrc = SYS_FS_FileOpen(filePath, SYS_FS_FILE_OPEN_READ);
    if (fileHandleSrc == SYS_FS_HANDLE_INVALID)
    {
        SYS_PRINT("file_RestoreFromUSB error %d \n" , SYS_FS_Error());
        return;
    }
   
    uint32_t fileSize = file_Size(fileHandleSrc);
    uint8_t *data = (uint8_t *)mm_malloc(fileSize);
    file_Read(data, fileSize, fileHandleSrc);
    SYS_FS_FileClose(fileHandleSrc);
    
    file_Seek(fileHandle, 0, SYS_FS_SEEK_SET);    
    file_Write(data, fileSize, fileHandle);
    mm_free(data);    
}

void logMgr_BackupToUSB(void)
{
    logMgr_BackupFileToUSB(g_devInfoFile, FILE_DEVICE_INFORMATION);
    logMgr_BackupFileToUSB(g_settingFile, FILE_SETTING_NAME);
    logMgr_BackupFileToUSB(g_eventLogFile, FILE_EVENTLOG_NAME);
    logMgr_BackupFileToUSB(g_alarmLogFile, FILE_ALARMLOG_NAME);
    logMgr_BackupFileToUSB(g_Spo2DataFile, FILE_SPO2DATA_NAME);
#ifdef DEBUG_LOG_TO_FILE
    logMgr_BackupFileToUSB(logFile, DEBUG_LOG_FILENAME);
#endif    
}
void logMgr_RestoreFromUSB(void)
{
    logMgr_RestoreFileFromUSB(g_devInfoFile, FILE_DEVICE_INFORMATION);
    logMgr_RestoreFileFromUSB(g_settingFile, FILE_SETTING_NAME);
    logMgr_RestoreFileFromUSB(g_eventLogFile, FILE_EVENTLOG_NAME);
    logMgr_RestoreFileFromUSB(g_alarmLogFile, FILE_ALARMLOG_NAME);    
    logMgr_RestoreFileFromUSB(g_Spo2DataFile, FILE_SPO2DATA_NAME);
#ifdef DEBUG_LOG_TO_FILE
    logMgr_RestoreFileFromUSB(logFile, DEBUG_LOG_FILENAME);
#endif 
}


void logMgr_ReduceLogOver30OlderDay(E_LogType type)
{
    SYS_PRINT("logMgr_ReduceLogOver30OlderDay %d \n", type);
    uint16_t numLog;
    uint8_t* logDataArray;
    
    Timestamp time = logMgr_getRtcTime();
    
    if (type == eEventLogTypeID)
    {
    }
    else if (type == eAlarmLogTypeID) 
    {       
    }
    else if (type == eSpo2DataLogTypeID)
    {
        logDataArray = mm_malloc(MAX_SPO2_LOG*LOG_LEN);
        numLog = logMgr_ReadLastestLog(eSpo2DataLogTypeID, MAX_SPO2_LOG, logDataArray);
        
        // clear log in file
        logMgr_ClearLog(eSpo2DataLogTypeID);
    }
    else
    {
        return;
    }   
    
    int index = 0;
    for (index = 0 ; index < numLog ; index++ )
    {
        Log_Struct logData;
        memset(&logData, 0, sizeof(Log_Struct));
        logData.eCode = 0xff;
        logMgr_GetLogFromDataArray(eSpo2DataLogTypeID, index, logDataArray, &logData);
                
        // FIXME: this estimate each month is 30 days, not check 28/31 days/ not check time
//        SYS_PRINT("CheckLogDate %d/%d current date %d/%d/%d, log date %d/%d/%d \n", index, numLog, time.year_2, time.month, time.date, logData.time.year_2, logData.time.month, logData.time.date);
        if ((logData.time.year_2 * 365 + logData.time.month + 30 + logData.time.date) <= (time.year_2 * 365 + time.month * 30 + time.date) + 30
                && (logData.time.year_2 * 365 + logData.time.month + 30 + logData.time.date) >= (time.year_2 * 365 + time.month * 30 + time.date))
        {
            logMgr_WriteLogToSQI(logData);
        }
        else
        {
            SYS_PRINT("Log date over 30 days \n");
        }
    }
    
    if (logDataArray)
        mm_free(logDataArray);     
}


/* end of file */
