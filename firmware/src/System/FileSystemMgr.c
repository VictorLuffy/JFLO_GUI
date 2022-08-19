/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */

#include "system_config.h"
#include "system_definitions.h"
#include "FileSystemMgr.h"
#include "GuiInterface.h"
#include "SQIInterface.h"
#include "LogInterface.h"

/** @brief Declare sqi handle */
//SYS_FS_HANDLE g_sqiHandle;

/** @brief Declare flag mount SQI */
bool g_isMountSQI = false;

/** @brief Declare flag mount USB */
bool g_isMountUSB = false;


bool FileSystemMgr_IsUSBMounted();
bool FileSystemMgr_IsSQIMounted();

static void systemTask_SysFSEventHandler(SYS_FS_EVENT event, void* eventData, uintptr_t context);
static USB_HOST_EVENT_RESPONSE systemTask_USBHostEventHandler(USB_HOST_EVENT event,
                                                              void* eventData,
                                                              uintptr_t context);
static void systemTask_MountSQIFlash(void);
static void FileSystemMgr_CheckFileOnSQIFlash(void);

bool FileSystemMgr_IsUSBMounted()
{
    return g_isMountUSB;
}

bool FileSystemMgr_IsSQIMounted()
{
    return g_isMountSQI;
}

void FileSystemMgr_Initialize(void)
{
    //Client registers an event handler with file system. This is done once.
    SYS_FS_EventHandlerSet(systemTask_SysFSEventHandler, (uintptr_t) NULL);
    USB_HOST_EventHandlerSet(&systemTask_USBHostEventHandler, 0);
    USB_HOST_BusEnable(0);

    return;
}

void FileSystemMgr_Task()
{

    SYS_FS_Tasks();

    DRV_SST26_Tasks(sysObj.drvSst26Obj0);


    DRV_USBHS_Tasks(sysObj.drvUSBObject);

    USB_HOST_Tasks(sysObj.usbHostObject0);

    //Mount SQI if automount failure
//    TickType_t xFileInitTick = xTaskGetTickCount();

    SQIInterface_Mount();
    
    LogInterface_InitDebugLogFile(); // FIXME
    
    //Check file in SQI flash
    SQIInterface_CheckFileOnSQIFlash();
//    xFileInitTick = xTaskGetTickCount() - xFileInitTick;
//    SYS_PRINT("SQIInterface_CheckFileOnSQIFlash (ms) %d\n",xFileInitTick); 
}

/** @brief Event Processing Technique. Event is received when operation is done
 *  @param [in]  SYS_FS_EVENT event: event handler
 *  @param [in]  void* eventData: event data
 *  @param [in]  uintptr_t context: The value of parameter will be passed back to the client unchanged
 *  @param [out]  None
 *  @return None
 */
void systemTask_SysFSEventHandler(SYS_FS_EVENT event, void* eventData, uintptr_t context)
{
    //SYS_PRINT("\n deviceTask_SysFSEventHandler = %d", event);
    switch (event)
    {
    case SYS_FS_EVENT_MOUNT:
        if (strcmp((const char *) eventData, SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0) == 0)
        {
            SYS_PRINT("\n SYS_FS_EVENT_MOUNT SQI \n");
            g_isMountSQI = true;
        }
        else if (strcmp((const char *) eventData, SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0) == 0)
        {
            SYS_PRINT("\n SYS_FS_EVENT_MOUNT USB \n");
            g_isMountUSB = true;
        }
        break;
    case SYS_FS_EVENT_UNMOUNT:
        if (strcmp((const char *) eventData, SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0) == 0)
        {
            SYS_PRINT("\n SYS_FS_EVENT_UNMOUNT SQI \n");
            g_isMountSQI = false;
        }
        else if (strcmp((const char *) eventData, SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0) == 0)
        {
            SYS_PRINT("\n SYS_FS_EVENT_UNMOUNT USB \n");
            g_isMountUSB = false;
        }
        break;
    case SYS_FS_EVENT_ERROR:
        break;
    default:
        break;
    }
    return;
}

/** @brief Event Processing Technique. Event is received when operation is done
 *  @param [in]  USB_HOST_EVENT event: event handler
 *  @param [in]  void* eventData: event data
 *  @param [in]  uintptr_t context: The value of parameter will be passed back to the client unchanged
 *  @param [out]  None
 *  @return USB_HOST_EVENT_RESPONSE
 */
USB_HOST_EVENT_RESPONSE systemTask_USBHostEventHandler(USB_HOST_EVENT event,
                                                       void* eventData,
                                                       uintptr_t context)
{
    switch (event)
    {
    case USB_HOST_EVENT_DEVICE_UNSUPPORTED:
        break;
    default:
        break;
    }

    return (USB_HOST_EVENT_RESPONSE_NONE);
}



/* *****************************************************************************
 End of File
 */
