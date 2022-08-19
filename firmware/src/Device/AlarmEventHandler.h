/** @file AlarmEventHandler.h
 *  @brief Contains functions for interface with Alarm, handle alarm event in Device
 *  @author Viet Le
 */

#ifndef ALARM_EVENT_HANDLER_H
#define	ALARM_EVENT_HANDLER_H

#include "system_config.h"
#include "system_definitions.h"



/** @brief Declare alarm event queue */
extern QueueHandle_t g_alarmEventQueue;


/** @brief This operation send alarm event to alarmEventQueue to communicate with DEVICE task
 *  @param [in] uint8_t id: ID of the event	
 *  @param [in] long data: content of the event 
 *  @param [out] None
 *  @return bool : true if event was sent successful, false is event was sent failed
 */
inline bool alarmEvenHandlerInterface_SendEventAlarm(uint8_t alarmId, uint8_t status, uint8_t *data);

/** @brief This operation check alarm event from alarm event queue and process them
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void alarmEvenHandler_HandleEvent(void);

/** @brief This operation get status of alarms
 *  @param [in] None
 *  @param [out] None
 *  @return true if there is any a alarm occur, false if there is no alarm
 */
bool alarmEventHandler_GetAlarmsStatus();


#endif	/* ALARM_EVENT_HANDLER_H */

/* end of file */