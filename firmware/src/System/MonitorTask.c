/* ************************************************************************** */
/** @file [ExternalComTask.c]
 *  @brief {This file contain source code necessary for Patient Data Calculation. 
 * It implements a Patient Data Task, handle event come to Patient Data Task, 
 * initialize dependency components such as spO2 sensors, update flow sensors data, 
 * temperature data and run its algorithm }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "system_config.h"
#include "system_definitions.h"

#include "MonitorTask.h"
#include "Monitor.h"





/** @brief Define Monitor task priority */
#define		MONITOR_TASK_PRIORITY		(tskIDLE_PRIORITY + 0) 

/** @brief Define Monitor task stack size */
#define 	MONITOR_TASK_STACK		(128)

/** @brief Declare Monitor task periodic */
#define         MONITOR_TASK_PERIODIC_MS        (2 / portTICK_PERIOD_MS)        //10 ms





/** @brief local functions  */
static void MonitorTask_Func(void);


/** @brief Initialize PATIENT DATA task and all components that dependency such as
 * UART2 for SPO2 module communication as well as interface for data sharing
 * This function should be called 1 times at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void MonitorTask_Initialize() {
//    //create MUTEX
//    s_PtDataMutex = xSemaphoreCreateMutex();
//    xSemaphoreGive(s_PtDataMutex);
}

/** @brief Function to create Patient Data task run with FreeRTOS
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void MonitorTask_Create(void) {
    xTaskCreate((TaskFunction_t) MonitorTask_Func,
            "Monitor Task",
            MONITOR_TASK_STACK, NULL, MONITOR_TASK_PRIORITY, NULL);
}



/** @brief Function to maintain PATIENT DATA task. This function will be executed
 * automatically by RTOS after PtDataTask_Create() function is called
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 */
void MonitorTask_Func(void) {
  
    //Record execution time
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
      
        monitor_HandleTaskMonitor();
        //wait for next turn
        vTaskDelayUntil(&xLastWakeTime, MONITOR_TASK_PERIODIC_MS);
    }
}



/* *****************************************************************************
 End of File
 */
