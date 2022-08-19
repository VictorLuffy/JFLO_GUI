/** @file GuiInterface.c
 *  @brief Contains functions for interface with Gui
 *  @author Viet Le
 */

#include "system_config.h"
#include "system_definitions.h"
#include "AlarmInterface.h"
#include "GuiInterface.h"

/** @brief Declare GUI queue */
extern QueueHandle_t g_guiQueue;

/** @brief This operation send event to guiQueue to communicate with GUI task
 *  @param [in] uint8_t id: ID of the event	
 *  @param [in] long data: content of the event 
 *  @param [out] None
 *  @return bool : true if event was sent successful, false is event was sent failed
 */
inline bool guiInterface_SendEvent(uint8_t id, long data) {
    //return value
    bool rtn = true;
    //event to send
    GUI_EVENT_t event;
    event.id = id;
    event.eventData.data = data;
    //send event
    if (xQueueSendToBack(g_guiQueue, &event, 2) != pdPASS) {
        SYS_PRINT("\n send failed: event to GUI task");
        xQueueReset(g_guiQueue);
        rtn = false;
    }
    return rtn;
}

inline bool guiInterface_SendEventAlarm(uint8_t alarmId, uint8_t status, uint8_t priority, uint8_t *data) {
    //return value
    bool rtn = true;
    //event to send
    GUI_EVENT_t event;
    event.id = eGuiAlarmEventId ;
    memset(event.eventData.alarmData, 0, 8);
    event.eventData.alarmData[ALARM_ID_DATA_INDEX] = alarmId;
    event.eventData.alarmData[ALARM_STATUS_DATA_INDEX] = status;
    event.eventData.alarmData[ALARM_PRIORITY_DATA_INDEX] = priority;
    if (data)
    {
        memcpy(&event.eventData.alarmData[3], data, 5);
////        event.eventData.alarmData[3] = 1;
//        SYS_PRINT("data[0] %d \n ", data[0]);
//        SYS_PRINT("alarmData[3] %d \n ", event.eventData.alarmData[3]);
    }
    //send event
    if (xQueueSendToBack(g_guiQueue, &event, 2) != pdPASS) {
        //        SYS_PRINT("\n send failed: event to GUI task");
        xQueueReset(g_guiQueue);
        rtn = false;
    }

    return rtn;
}


/* end of file */