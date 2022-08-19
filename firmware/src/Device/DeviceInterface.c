/** @file GuiInterface.c
 *  @brief Contains functions for interface with Gui
 *  @author Viet Le
 */

#include "system_config.h"
#include "system_definitions.h"
#include "DeviceInterface.h"

/** @brief This operation send event to DevQueue to communicate with DEVICE task
 *  @param [in] E_AudioCommand id: ID of the event such as ePlayHighAlarm, ePlayMediumAlarm,
 * ePlayLow1Alarm, eStopPlay, eMute
 *  @param [in] uint16_t data: content of the event. If event is ePlayHighAlarm/ePlayMediumAlarm/
 * ePlayMediumAlarm, the data the current volume level of setting. If event is eMute, the data is the mute time (seconds).
 * If event is eStopPlay, the data should be zero.
 *  @param [out] None
 *  @return bool : true if event was sent successful, false is event was sent failed
 */
inline bool deviceInterface_SendAudioEvent(E_AudioCommand id, uint16_t data) 
{
    //return value
    bool rtn = true;
    //event to send
    DEVICE_EVENT_t event;
    event.id = eAudioEventId;
    event.eventData.audioData.cmdId = id;
    event.eventData.audioData.data = data;
    //send event
    if (xQueueSendToBack(s_DeviceTaskQueue, &event, 2) != pdPASS) {
        SYS_PRINT("\n send failed: event to Dev task");
        xQueueReset(s_DeviceTaskQueue);
        rtn = false;
    }
    return rtn;
}

/** @brief This operation send alarm event to guiQueue to communicate with GUI task
 *  @param [in] uint8_t id: ID of the event	
 *  @param [in] long data: content of the event 
 *  @param [out] None
 *  @return bool : true if event was sent successful, false is event was sent failed
 */

inline bool deviceInterface_SendEventAlarm(uint8_t id, uint8_t alarmId, uint8_t status) {

}


/* end of file */