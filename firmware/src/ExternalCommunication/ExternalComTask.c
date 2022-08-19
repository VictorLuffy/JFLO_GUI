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
#include "ExternalComTask.h"
#include "UART_2.h"
#include "PC_Monitoring.h"
#include "Monitor.h"





/** @brief Define External communication task priority */
#define		EXTCOM_TASK_PRIORITY		(tskIDLE_PRIORITY + 1) 

/** @brief Define External communication task stack size */
#define 	EXTCOM_TASK_STACK		(512)

/** @brief Declare External communication task periodic */
#define         EXTCOM_TASK_PERIODIC_MS         (/*18*/20 / portTICK_PERIOD_MS)        //10 ms

/** @brief  External communication task Queue */
#define 	EXTCOM_MAX_WAIT_MUTEX		(5)         //5ms

//#define CHECK_REMAINING_STACK_SIZE


/** @brief local functions  */
static void ExtComTask_Func(void);
static void ExtComTask_HandleEvent(void);
static void ExtComTask_Maintain(void);



/** @brief Initialize PATIENT DATA task and all components that dependency such as
 * UART2 for SPO2 module communication as well as interface for data sharing
 * This function should be called 1 times at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void ExtComTask_Initialize() {
//    //Uart2_Initialize();
//    //create MUTEX
//    s_PtDataMutex = xSemaphoreCreateMutex();
//    xSemaphoreGive(s_PtDataMutex);
//    //reset variables
//    PtDataTask_ResetData();
    
    //initialize PC monitor module
    PC_Monitor_Initialize();
    
}

/** @brief Function to create Patient Data task run with FreeRTOS
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void ExtComTask_Create(void) {
    xTaskCreate((TaskFunction_t) ExtComTask_Func,
            "External Communication Data Task",
            EXTCOM_TASK_STACK, NULL, EXTCOM_TASK_PRIORITY, NULL);
}



/** @brief Function to maintain PATIENT DATA task. This function will be executed
 * automatically by RTOS after PtDataTask_Create() function is called
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 */
void ExtComTask_Func(void) {
        //Declare time scheduler
    const TickType_t cycleTime = EXTCOM_TASK_PERIODIC_MS;
    //Record execution time
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
#ifdef CHECK_REMAINING_STACK_SIZE
	UBaseType_t uxHighWaterMark;
	uint32_t cnt = 0;
#endif
    
    while (1) {
      
        monitor_UpdateStartedTime(eExtCom);
        //process input event
        ExtComTask_HandleEvent();
        
        //Maintain Patient data task
        ExtComTask_Maintain();
        
#ifdef CHECK_REMAINING_STACK_SIZE
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
        if(cnt>=2000/EXTCOM_TASK_PERIODIC_MS)
        {
                SYS_PRINT("External Com Task remaining stack %d\n",uxHighWaterMark);
                cnt = 0;
        }
        else
        {
                cnt++;
        }
#endif
        
        //wait for next turn
        vTaskDelayUntil(&xLastWakeTime, cycleTime);
    }
}


/** @brief Handle event sent to Patient Data task via PtData Queue
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 */
void ExtComTask_HandleEvent(void) {

}

/** @brief Maintain Patient Data task, including reading spO2 value, use flows data,
 * temperature data, ... to run its self algorithm
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 */
void ExtComTask_Maintain(void) {

    PC_Monitor_Handle();
}


/* *****************************************************************************
 End of File
 */
