/* ************************************************************************** */
/** @file [AlarmInterface.c]
 *  @brief {Contains functions which are ALarm interface}
 *  @author {Cuong Le}
 */
/* ************************************************************************** */

#include "AlarmInterface.h"
#include "GuiInterface.h"

/** @brief Send the alarm message to GUI task  
 *  @param [in] E_AlarmId id : alarm id
 *  @param [in] E_AlarmStatus status : alarm status
 *  @param [in] int32_t data : data which readies to sent
 *  @param [out] None
 *  @return bool : true if event was sent successful, false is event was sent failed
 */
#ifndef UNIT_TEST
inline bool alarmInterface_SendEvent(E_AlarmId id, E_AlarmStatus status, E_AlarmPriority priority, uint8_t *data) {
#else
  bool alarmInterface_SendEvent(E_AlarmId id, E_AlarmStatus status, E_AlarmPriority priority, uint8_t *data) {
#endif
    //Return value
    bool rtn = true;
    ALARM_STAT_t alarmEvent;
    alarmEvent.ID = id;
    alarmEvent.currentStatus = status;
    alarmEvent.currentPriority = priority;
#ifndef UNIT_TEST
    uint8_t alarmData[5] = { [0 ... 4] = 0 };
#else
    uint8_t alarmData[5] = {0,0,0,0,0};
#endif
    memcpy(alarmData, data, 5);
    // TODO_tt: gui debug
    if (guiInterface_SendEventAlarm(alarmEvent.ID, alarmEvent.currentStatus, alarmEvent.currentPriority, alarmData) != pdPASS)
    {
        SYS_PRINT("\n send failed: event to Gui task");
        rtn = false;
    } //tt_debug
    /*
    if (xQueueSendToBack(g_alarmQueueReceive, &alarmEvent, QUEUE_ALARM_WAITING) != pdPASS) {
        SYS_PRINT("\n send failed: event to log task");
        xQueueReset(g_alarmQueueReceive);
        rtn = false;
    }
    */
    
    if (alarmEvenHandlerInterface_SendEventAlarm(id, status, alarmData)  == false)
    {
        rtn = false;
    }

    return rtn;
}

/* *****************************************************************************
 End of File
 */
