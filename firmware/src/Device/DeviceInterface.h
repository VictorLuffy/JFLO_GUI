

/** @file DeviceInterface.h
 *  @brief Contains functions for interface with Device
 *  @author Viet Le
 */

#ifndef DEVICE_INTERFACE_H
#define	DEVICE_INTERFACE_H

#include "system_config.h"
#include "system_definitions.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "stdint.h"
#include "stdbool.h"

#include "Audio.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    

/** @brief Define Device event data */
typedef struct {
    uint8_t cmdId;
    uint16_t data;
} Audio_Data_t;


/** @brief Define event send from other task to Device */
typedef enum {
    eAudioEventId,
    eAlarmEventId,
} E_DeviceEventId;

/** @brief Define union of device event data */
typedef union {
    Audio_Data_t audioData;
    uint16_t alarmData[2];
} Dev_Event_Data;

/** @brief Define Device event data */
typedef struct {
    E_DeviceEventId id;
    Dev_Event_Data eventData;
} DEVICE_EVENT_t;

/** @brief Declare Device queue */
extern QueueHandle_t s_DeviceTaskQueue;

/** @brief This operation send event to DevQueue to communicate with DEVICE task
 *  @param [in] uint8_t id: ID of the event such as	ePlayHighAlarm, ePlayMediumAlarm,
 * ePlayLow1Alarm, eStopPlay,eMute
 *  @param [in] uint16_t data: content of the event. If event is ePlayHighAlarm/ePlayMediumAlarm/
 * ePlayMediumAlarm, the data the current volume level of setting. If event is eMute, the data is the mute time (seconds).
 * If event is eStopPlay, the data should be zero.
 *  @param [out] None
 *  @return bool : true if event was sent successful, false is event was sent failed
 */
inline bool deviceInterface_SendAudioEvent(E_AudioCommand id, uint16_t data);

/** @brief This operation send alarm event to deviceQueue to communicate with Device task
 *  @param [in] uint8_t id: ID of the event	
 *  @param [in] long data: content of the event 
 *  @param [out] None
 *  @return bool : true if event was sent successful, false is event was sent failed
 */
inline bool deviceInterface_SendEventAlarm(uint8_t id, uint8_t alarmId, uint8_t status);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif


#endif	/* DEVICE_INTERFACE_H */

/* end of file */