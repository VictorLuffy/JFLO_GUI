/** @file SystemInterface.h
 *  @brief Contains functions for interface with System
 *  @author Viet Le
 */

#ifndef SYSTEMINTERFACE_H
#define	SYSTEMINTERFACE_H

#include "system_config.h"
#include "system_definitions.h"

/** @brief define system event ID */
typedef enum {
    eSystemUpgradeEventId = 0, /**<event to upgrade */
    eSystemLogSettingChangedEventId, /**<setting changed */
    eSystemFormatSQIEventId,/**<Format SQI */
    eCradleUpgradeEventId,            
    eChamberUpgradeEventId,  
    eEsp32UpgradeEventId,
    eSystemPowerOffId,
    eNoOfSystemEventId
} E_SystemEventId;

/** @brief Define union of system event data */
typedef union {
    long type1; //define type 1 as a number of long integer
    short type2[2]; //define type 2 as array of short integer
} System_Event_Data;

/** @brief SYSTEM_EVENT_t define event structure */
typedef struct {
    E_SystemEventId Id;
    System_Event_Data Data;
} SYSTEM_EVENT_t;

//Declare SYSTEM queue
extern QueueHandle_t g_systemQueue;

/** @brief This operation support sending event to system task from other place
 *  @param [in] E_SystemEventId id: id of the event
 *  @param [in] long data : data of the event 
 *  @param [out] None
 *  @return bool: true if event was sent successful, false if event was sent failed	  
 */
inline bool systemInterface_SendEvent(E_SystemEventId id, long data);

/** @brief This operation support sending event to system task from other place 
 *  @param [in] E_SystemEventId id: id of the event to send 
 *  @param [in] short data0: data field number 1 
 *  @param [in] short data1: data field number 2 
 *  @param [out] None
 *  @return bool: true if event was sent successful, false if event was sent failed  
 */
inline bool systemInterface_SendEventType2(E_SystemEventId id, short data0, short data1);

//Init system interface
void systemInterface_Init(void);

#endif	/* SYSTEMINTERFACE_H */

/* end of file */
