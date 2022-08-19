/* ************************************************************************** */
/** @file [PtDataTask.c]
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
#include "PtDataTask.h"
#include "UART_2.h"
#include "ADC.h"




/** @brief Define PATIENT DATA task priority */
#define		PTDATA_TASK_PRIORITY		(tskIDLE_PRIORITY + 1) 

/** @brief Define PATIENT DATA task stack size */
#define 	PTDATA_TASK_STACK			(1*1024)

/** @brief Declare PATIENT DATA task periodic */
#define     PTDATA_TASK_PERIODIC_MS     (50 / portTICK_PERIOD_MS)        //10 ms

/** @brief  Device task communication Queue */
#define 	PTDATA_MAX_WAIT_MUTEX		(5)         //5ms


/** @brief MUTEX to protect Patient Data  sharing */
static SemaphoreHandle_t s_PtDataMutex = NULL;

/** @brief Patient data assets / properties. These data is updated by Motor task, 
 Heater task and Patient Data task; can be get to display on GUI or update system 
 status */
static PT_PUBLIC_DATA_t s_PtData;


/** @brief local functions  */
static void PtDataTask_Func(void);
static void PtDataTask_ResetData(void);
static void PtDataTask_HandleEvent(void);
static void PtDataTask_Maintain(void);



/** @brief Initialize PATIENT DATA task and all components that dependency such as
 * UART2 for SPO2 module communication as well as interface for data sharing
 * This function should be called 1 times at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PtDataTask_Initialize() {
    //Uart2_Initialize();
    //create MUTEX
    s_PtDataMutex = xSemaphoreCreateMutex();
    xSemaphoreGive(s_PtDataMutex);
    //reset variables
    PtDataTask_ResetData();
}

/** @brief Function to create Patient Data task run with FreeRTOS
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void PtDataTask_Create(void) {
    xTaskCreate((TaskFunction_t) PtDataTask_Func,
            "Patient Data Task",
            PTDATA_TASK_STACK, NULL, PTDATA_TASK_PRIORITY, NULL);
}

/** @brief Get all data of Patient data assets
 * This function can be called from other task to obtain Patient data
 *  @param [in]  None   
 *  @param [out]  PT_PUBLIC_DATA_t* data   external pointer to store data
 *  @return None
 */
bool PtDataTask_GetPublicData(PT_PUBLIC_DATA_t* data) {
        //take resource 
    if (xSemaphoreTake(s_PtDataMutex, PTDATA_MAX_WAIT_MUTEX) == pdTRUE) {
        //copy data
        *data = s_PtData;
        //release semaphore
        xSemaphoreGive(s_PtDataMutex);
    }
}

/** @brief Function to maintain PATIENT DATA task. This function will be executed
 * automatically by RTOS after PtDataTask_Create() function is called
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 */
void PtDataTask_Func(void) {
        //Declare time scheduler
    const TickType_t cycleTime = PTDATA_TASK_PERIODIC_MS;
    //Record execution time
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1) {
        //process input event
        PtDataTask_HandleEvent();
        
        //Maintain Patient data task
        PtDataTask_Maintain();
        
        //wait for next turn
        vTaskDelayUntil(&xLastWakeTime, cycleTime);
    }
}

/** @brief Reset Patient Data to default value
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 */
void PtDataTask_ResetData(void) {
    s_PtData.o2Concentration = 0;
    s_PtData.spO2 = 0;
}

/** @brief Handle event sent to Patient Data task via PtData Queue
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 */
void PtDataTask_HandleEvent(void) {

}

/** @brief Maintain Patient Data task, including reading spO2 value, use flows data,
 * temperature data, ... to run its self algorithm
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 */
void PtDataTask_Maintain(void) {
//    Spo2Sensor_GetValue();
}


void Spo2Sensor_GetValue(void) {
    static uint8_t data[100] = {'\0'};
    static uint8_t txData[] = "\n hello UART ";

    Uart2_Send(&txData[0], sizeof (txData));

    memset(&data[0], '\0', 100);
    int16_t receivedByte = Uart2_ReadReceiveBuffer(&data[0], 100);
    if (receivedByte > 0) {
        //            SYS_PRINT("\nreceivedByte: %d; string = %s", receivedByte, &data[0]);
        //            Uart2_Send(&data[0], receivedByte);
    }
}

/* *****************************************************************************
 End of File
 */
