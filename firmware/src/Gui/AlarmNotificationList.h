/** @file DeviceInformation.h
 *  @brief 
 *  @author Trac Truong
 */

#ifndef ALARM_NOTIFICATION_LIST_H
#define ALARM_NOTIFICATION_LIST_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef UNIT_TEST
#include <system/debug/sys_debug.h>
#include "Alarm/AlarmInterface.h"
#endif

#ifndef UNIT_TEST
//#define DEBUG_MSG SYS_PRINT
//#define DEBUG_MSG(...) printf(...)
#define DEBUG_MSG(...)
#else
#define DEBUG_MSG(...)
#endif

#ifdef UNIT_TEST
typedef enum {
    eAlarm01,
    eAlarm02,
    eAlarm03,
    eAlarm04,
    eAlarm05,
    eNoOfAlarmId,
} E_AlarmId;

typedef enum {
    eInactive,
    ePaused,
    eWaitingToActive,
    eActive,
    eNoOfAlarmStatus,
} E_AlarmStatus;

typedef enum {
    eLow,
    eMedium,
    eHigh,
    eNoOfAlarmPriority,
} E_AlarmPriority;



#endif

typedef struct {
    E_AlarmId alarmId; /**< alarm id */
    E_AlarmStatus alarmStatus; /**< alarm status */
    E_AlarmPriority alarmPriority; /**< alarm priority */
    uint8_t alarmData[5]; /**< alarm alarmData */
} AlarmNotification;

typedef enum {
    eNoError, /**< no error */
    eNoActiveAlarm, /**< no active alarm */
    eIndexInvalid, /**< index invalid */
    eSizeInvalid, /**< size invalid */
    eArrayFull, /**< array full */
    eProcessError, /**< process error */
    eNoOfAlarmNotificationError, /**< number of error */
} AlarmNotificationError;

/** Init noitification list */
#define MAX_SIZE_LIST eNoOfAlarmId

/** @brief Init noitification list
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmNotificationList_Init();

/** @brief Remove item with alarm id
 *  @param [in]  alarmId
 *  @param [out]  None
 *  @return AlarmNotificationError
 */
AlarmNotificationError AlarmNotificationList_Remove(E_AlarmId alarmId);

/** @brief Get item with index
 *  @param [in]   index
 *  @param [out]  alarmNotification
 *  @return AlarmNotificationError
 */
AlarmNotificationError AlarmNotificationList_GetItem(unsigned int index, AlarmNotification *alarmNotification);

/** @brief Get current no of items in array
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
int AlarmNotificationList_GetSize();

/** @brief Get index of item match alarmid
 *  @param [in]  alarmId
 *  @param [out]  None
 *  @return index of alarmId ( -1 if not found )
 */
int AlarmNotificationList_GetItemIndex(E_AlarmId alarmId);

/** @brief Get active alarm id in list, there will be only one
 *  @param [in]  None
 *  @param [out]  None
 *  @return index of alarmId ( -1 if not found )
 */
int AlarmNotificationList_GetActiveIndex();

/** @brief Update status for all alarm in lists
 * 
 * There will be only one active alarm selected, others active will be changed to WAITING TO ACTIVE state
 * 
 * Scan over the array and decide which alarm will be active based on priority, order
 * Check first highest priority (active/waiting status)
 *    - if active, other remain active -> waiting (=> list changed )
 *    - if waiting, check if any remain items with equal/higher priority active 
 *        + if yes, no update
 *        + if no, waiting -> active
 * 
 *  @param [in]  None
 *  @param [out]  None
 *  @return AlarmNotificationError
 */
AlarmNotificationError AlarmNotificationList_ProcessAlarmNotificationList();

/** @brief Update alarm
 * 
 * ACTIVE/WAITING
 *     - if no existed
 *         + add new alarm, status will be waiting to active
 *     - if existed
 *         + current paused/waiting -> waiting
 *         + update other alarmData
 * INACTIVE remove if existed
 * PAUSED update if existed
 *      + current active/waiting -> paused
 * 
 * Usage note :
 *    - call AlarmNotificationList_UpdateAllStatus() to update alarm list, decide which alarm is active
 *    - call AlarmNotificationList_GetActiveIndex() to check active alarm and display UI
 * 
 *  @param [in]   alarmNotification
 *  @param [out]  None
 *  @return AlarmNotificationError
 */
AlarmNotificationError AlarmNotificationList_UpdateAlarm(AlarmNotification alarmNotification);

/** @brief Print current alarm list, this function only used for debugging
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmNotificationList_PrintList();

#endif //ALARM_NOTIFICATION_LIST_H

/* end of file */
