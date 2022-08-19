/** @file SystemInterface.c
 *  @brief Contains functions for interface with System
 *  @author Viet Le
 */
#include "SystemInterface.h"
#include "Watchdog.h"

/** @brief Declare SYSTEM queue size has 16 items */
#define 	SYSTEM_QUEUE_SIZE			(16)

/** @brief declare SYSTEM queue */
QueueHandle_t g_systemQueue;

/** @brief This operation support sending event to system task from other place
 *  @param [in] E_SystemEventId id: id of the event
 *  @param [in] long data : data of the event 
 *  @param [out] None
 *  @return bool: true if event was sent successful, false if event was sent failed	  
 */
inline bool systemInterface_SendEvent(E_SystemEventId id, long data)
{
    //return value
    bool rtn = true;
    //event to send
    SYSTEM_EVENT_t event;
    event.Id = id;
    event.Data.type1 = data;
    //send event to queue
    if (xQueueSendToBack(g_systemQueue, &event, 2) != pdPASS)
    {
        rtn = false;
    }
    return rtn;
}

/** @brief This operation support sending event to system task from other place 
 *  @param [in] E_SystemEventId id: id of the event to send 
 *  @param [in] short data0: data field number 1 
 *  @param [in] short data1: data field number 2 
 *  @param [out] None
 *  @return bool: true if event was sent successful, false if event was sent failed  
 */
inline bool systemInterface_SendEventType2(E_SystemEventId id, short data0, short data1)
{
    //return value
    bool rtn = true;
    //event to send
    SYSTEM_EVENT_t event;
    event.Id = id;
    event.Data.type2[0] = data0;
    event.Data.type2[1] = data1;
    //send event to queue
    if (xQueueSendToBack(g_systemQueue, &event, 2) != pdPASS)
    {
        //		DebugStr("\n send failed: event to system task");
        rtn = false;
    }
    return rtn;
}

/** @brief Init system interface
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void systemInterface_Init(void)
{
    //create System queue for System task communication
    g_systemQueue = xQueueCreate(SYSTEM_QUEUE_SIZE, sizeof (SYSTEM_EVENT_t));

    return;
}

/** @brief This operation check System event from System queue and process them
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SystemTask_HandleEvent(void)
{
    SYSTEM_EVENT_t systemEvent;

    //wait for queue event
    if (xQueueReceive(g_systemQueue, &systemEvent, 0) == pdTRUE) //wait 0 tick (do not wait)
    {
        SYS_PRINT("g_systemQueue rev\n");
        switch (systemEvent.Id)
        {
        case eSystemUpgradeEventId:
        {         
            //Upgrade
            SYS_PRINT("softwareUpgrade_Process\n");
            Watchdog_Disable();
            softwareUpgrade_Process();
            Watchdog_Init();
            break;
        }
        case eSystemFormatSQIEventId:
            if (SYS_FS_DriveFormat(SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0, SYS_FS_FORMAT_FDISK, 0) != SYS_FS_RES_SUCCESS)
            {
                // Failure, try mounting again
                SYS_PRINT("\n Mount failed \n");
            }
            else
            {
                delay_MS(100);
                SYS_RESET_SoftwareReset();
            }

            break;
        case eCradleUpgradeEventId:
            softwareUpgrade_NewSoftwareForCradle();
            break;
            
        case eChamberUpgradeEventId:
            softwareUpgrade_NewSoftwareForChamber();
            break;
        case eEsp32UpgradeEventId:
            softwareUpgrade_NewSoftwareForEsp32();  
            break;
        case eSystemPowerOffId:
            SYS_RESET_SoftwareReset();
            break;
        default:
            break;
        }
    }
    return;
}

/* end of file */
