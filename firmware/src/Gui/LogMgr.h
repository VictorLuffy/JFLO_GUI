/** @file LogMgr.h
 *  @brief Manager of JFLO log
 *  @author Viet Le
 */

#ifndef LOGMGR_H
#define	LOGMGR_H

/* This section lists the other files that are included in this file.
 */
#include "LogInterface.h"

#define JFLO_DEBUG_LOG   1  

/** @brief Define queue receive length */
#define LOG_QUEUE_RECEIVE_LENGTH            (16)

/** @brief Define max no of event log */
#define MAX_EVENT_LOG                       (1000)

/** @brief Define max no of alarm log */
#define MAX_ALARM_LOG                       (1000)

/** @brief Define max no of alarm log */
#define MAX_SPO2_LOG                        (100)

/** @brief Define log info length */
#define LOG_LEN                        (26)

/** @brief Define device info length */
#define LOG_DEVICEINFO_LEN              (32 + 2)

/** @brief Define the last character of log string */
#define LOG_TAIL_LEN               1    // "\n"

/** @brief Define infor log length */
#define INFOR_LOG_LENGTH                    2

/** @brief Define infor log size*/
#define INFOR_LOG_SIZE                      4

/** @brief Define maximum number of year when the log file has not been deleted */
#define MAX_SAVELOGFILE_TIME                       (1000000)

/** @brief Define maximum number of year when the log file has not been deleted */
#define HEADER_EVENT_LOG                     0x20

/** @brief Define maximum number of year when the log file has not been deleted */
#define HEADER_ALARM_LOG                     0x21  

/** @brief Define maximum number of year when the log file has not been deleted */
#define HEADER_SPO2_LOG                      0x22    
/** @brief Define struct log information */
typedef struct {
    uint32_t currentIndex; /**< The position of last written log in log file */
    uint32_t numLogSaved; /**< The number of logs which were saved in log file */

    uint32_t firstIndex; 		
    uint32_t lastIndex;		
	uint32_t maxLog;	
} LogInfo_Struct;

typedef enum {
    eInvalidLogStatus,
    eSuspendedLogStatus,        
    eRunningLogStatus,
    eReadyLogStatus,
    ePendingLogStatus,     
} E_LogStatus;

/** @brief Define struct which manage each type of log */
typedef struct {
    int type;
    E_LogStatus status;       
    /** @brief Declare log data information */
    LogInfo_Struct s_logInfor;
   
    /* Declare current index */
    uint32_t s_currentIndex;

    /* Declare number log saved */
    uint32_t s_numLogSaved;

    bool is_CopingFiletoUSB;
    
    SYS_FS_HANDLE* fileHandle;
    char* fileName;
    char* usbFileName;
    uint32_t fileSize;
} LogObject_Struct;

/***/
E_LogStatus logMgr_GetLogStatus(E_LogType logtype);

/* Handle event log to flash*/
void logMgr_HandleEvent(void);

/* Handle alarm log to flash*/
void logMgr_HandleAlarm(void);

/* Handle spo2 log to flash*/
void logMgr_HandleSpO2(void);

/*Write log to SQI*/
void logMgr_WriteLogToSQI(Log_Struct log);

/*Set current index*/
void logMgr_SetCurrentIndex(E_LogType type);

/*Get current index*/
uint16_t logMgr_GetCurrentIndex(E_LogType type);

/* Get number of log */
uint16_t logMgr_GetNumberOfLog(E_LogType type);

/* Get next log */
void logMgr_GetLogAtIndex(E_LogType type, int index, Log_Struct *log);

/*Update information log*/
void logMgr_UpdateInforLog(E_LogType type);

/* Delete all data in one log file */
void logMgr_ClearLog(E_LogType logType);

/* Use in logTask function to handle when spo2 was received */
void logMgr_HandleSpO2(void);

/* Use in logTask function to handle when logRequest was received */
void logMgr_HandleDeviceInformation(void);

/* Use in logTask function to handle when logRequest was received */
void logMgr_HandleRequest(void);

/* Not used**/
//static bool logMgr_SendLogToUSB(const Log_Struct *log);

/* Get current RTC time. */
Timestamp logMgr_getRtcTime(void);

/* Init Device Infomation to SQI Flash */
void logMgr_InitDeviceInfo(void);

/* Update Device Infomation to SQI Flash */
void logMgr_UpdateDeviceInfo(E_LogRequestID id);

/*Write device info log to SQI*/
void logMgr_WriteDeviceInfoToSQI(void);

/* Act the behavior when Copy file FromSQIFlashtoUSB command was received. */
void logMgr_ExportLogFromSQIFlashtoUSB(E_LogType type);

/* Log mangeger initialize*/
void logMgr_Task(void);

/* Init log manager*/
void logMgr_Initialize(void);

/** @brief Read number of lastest logs
 *  @param [in] E_LogType type
 *  @param [in] uint16_t noOfLog
 *  @param [out] : uint8_t *data
 *  @return uint16_t : number of log is read
 */
uint16_t logMgr_ReadLastestLog(E_LogType type, uint16_t noOfLog, uint8_t *data);


/** @brief Get log from log data array
 *  @param [in] type Log Type that want to clear in file
 *  @param [in] int index: index log
 *  @param [in] : uint8_t *data
 *  @param [out] Log_Struct *log: log data
 *  @return None
 */
void logMgr_GetLogFromDataArray(E_LogType type, int index, uint8_t *data, Log_Struct *log);

void logMgr_RestoreFileFromUSB(SYS_FS_HANDLE fileHandle, const char* fileName);
void logMgr_BackupFileToUSB(SYS_FS_HANDLE fileHandle, const char* fileName);

void logMgr_BackupToUSB(void);
void logMgr_RestoreFromUSB(void);

/** @brief logMgr_ReduceLogOver30OlderDay
 *  @param [in] type Log Type that want to clear in file
 *  @return None
 */
void logMgr_ReduceLogOver30OlderDay(E_LogType type);

#endif	/* LOGMGR_H */

/* end of file */