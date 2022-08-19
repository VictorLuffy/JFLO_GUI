/** @file DeviceTask.h
 *  @brief RTOS task for control device
 *  @author Viet Le
 */


#ifndef DEVICETASK_H
#define	DEVICETASK_H


/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "AlarmInterface.h"
#include "PC_Monitoring.h"


/** @brief Struct of Device Informations which are received to use for device process*/
typedef struct {
    uint8_t id;
    long data;
} DEVICE_TASK_EVENT_t;

/** @brief Struct of Alarm Informations which are received to use for alarm process*/
typedef struct {
    E_AlarmId id;
    E_AlarmStatus status;
    uint8_t data[5];
} DEVICE_TASK_ALARM_EVENT_t;


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    inline bool DeviceTask_SendEvent(uint8_t id, uint32_t data);

    //Function to initialize Device task component
    void DeviceTask_Initialize(void);

    //Function to create Device task run with FreeRTOS
    void DeviceTask_Create(void);
    
    /** @brief Function to suspend Device task
    *  @param [in] None
    *  @param [out] None
    *  @return None
    */
   void DeviceTask_Suspend(void);

   /** @brief Function to resume Device task
    *  @param [in] None
    *  @param [out] None
    *  @return None
    */
   void DeviceTask_Resume(void);


    /** @brief Function to current Alarm monitor structure
    *  @param [in] ALARM_MONITOR_t storagePlace
    *  @param [out] None
    *  @return None
    */
    void DeviceTask_GetAlarmMonitorStruct(ALARM_MONITOR_t *storagePlace);
    
    /** @brief Function to current PC monitor structure
    *  @param [in] PC_MONITORING_t storagePlace
    *  @param [out] None
    *  @return None
    */
    void DeviceTask_GetPCMonitorStruct(PC_MONITORING_t *storagePlace);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif	/* DEVICETASK_H */

/* end of file */