/* ************************************************************************** */
/** @file [AlarmTask.c]
 *  @brief {AlarmTask module define a RTOS task for check and update alarm status}
 *  @author {Cuong Le}
 */
/* ************************************************************************** */

#ifndef ALARMTASK_H
#define	ALARMTASK_H

#include "FreeRTOS.h"
#include "queue.h"

//Function to initialize Alarm task component
void alarmTask_Initialize(void);

//Function to create Alarm task run with FreeRTOS
void alarmTask_Create(void);

//Function to initialize Alarm task component
void alarmTask_HandleEvent(void);


/** @brief Function to suspend Alarm task
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void alarmTask_Suspend(void);

/** @brief Function to resume Alarm task
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void alarmTask_Resume(void);

//Function Alarm task
#ifndef UNIT_TEST
static void alarmTask_Func();
#else
void alarmTask_Func(void *pvParameters);
#endif

#endif	/* ALARMTASK_H */

/* *****************************************************************************
 End of File
 */