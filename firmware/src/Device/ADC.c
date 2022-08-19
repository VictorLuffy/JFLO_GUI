/* ************************************************************************** */
/** @file [ADC.c]
 *  @brief {set up ADC module 7 to scan multiple channels, register channel AN14,
 * AN16, AN18, AN20, AN21, AN39 to ADC7. 
 * Set up interrupt for ADC conversion complete and automatically post conversion 
 * result to ADC queue. 
 * Support interface to get value of each ADC channel, protect share resource by 
 * MUTEX }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */





/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "portmacro.h"

#include "system_config.h"
#include "system_definitions.h"

#include "ADC.h"


/** @brief size of buffer that store ADC samples data sent from ADC interrupt */
#define     ADC_DATA_QUEUE_SIZE       (200)

/** @brief maximum time in MS a task can wait to get/set ADC data */
#define     ADC_MUTEX_MAX_WAIT_MS       (20)

/** @brief structure of an ADC data send from ADC interrupt, include channel
 * ID and channel data */
typedef struct {
    uint8_t channel; /**< analog channel id */
    uint16_t data; /**< data of corresponding channel */
} ADC_DATA_ITEM_t;

/** @brief a queue to store ADC samples data sent from ADC interrupt */
static QueueHandle_t s_ADCDataQueue = NULL;

/** @brief MUTEX to protect accessing ADC set of values */
static SemaphoreHandle_t s_ADCMutex = NULL;

/** @brief variables to hold latest ADC value of current sensor # 1,  any task 
 can access this value at anytime */
static float s_CurrentSensor1;

/** @brief variables to hold latest ADC value of current sensor # 2,  any task 
 can access this value at anytime */
static float s_CurrentSensor2;

/** @brief variables to hold latest ADC value of light sensor,  any task 
 can access this value at anytime */
static float s_LightSensor;

/** @brief variables to hold latest ADC value of temperature sensor # 1,  any task 
 can access this value at anytime */
static float s_TemperaturSensor1;

/** @brief variables to hold latest ADC value of temperature sensor # 2,  any task 
 can access this value at anytime */
static float s_TemperaturSensor2;

/** @brief variables to hold latest ADC value of temperature sensor # 3,  any task 
 can access this value at anytime */
static float s_TemperaturSensor3;

/** @brief variables to hold latest ADC value of voltage sensor ,  any task 
 can access this value at anytime */
static float s_VoltageSensor;

/** @brief variables to hold latest ADC value of NiMh battery ,  any task 
 can access this value at anytime */
static float s_VoltageNiMh;

/** @brief variables to hold latest ADC value of voltage input ,  any task 
 can access this value at anytime */
static float s_VoltageInput;



/** @brief internal functions declaration */
void ADC_ReportError();

/** @brief Function to initialize ADC7 module (module scan for multiple channels)
 * responsible to scan all ADC channels in the project. The ADC module is setting 
 * up to be triggered by TIMER 5 every 5 ms, automatically interrupt when ADC conversion
 * is finished. Interrupt ISR function will send and event to s_ADCDataQueue to 
 * update value for each channel ADC. A MUTEX is also created to protect multiple
 * task access to ADC channels value.
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void ADC_Initialize ( ) {
    //prepare for ADC7 (ADC number 0)
    DRV_ADC0_Open();

    //Create a queue capable of containing 64 item of ADC_DATA_ITEM_t structures.  
    s_ADCDataQueue = xQueueCreate(ADC_DATA_QUEUE_SIZE, sizeof ( ADC_DATA_ITEM_t));
    if (s_ADCDataQueue == NULL) {
        //Queue was not created and must not be used.
        ADC_ReportError();
    }

    s_ADCMutex = xSemaphoreCreateMutex();

    //reset values
    s_CurrentSensor1 = 0;
    s_CurrentSensor2 = 0;
    s_LightSensor = 0;
    s_VoltageSensor = 0;
    s_VoltageNiMh = 0;
    s_VoltageInput = 0;
}

/** @brief start ADC conversion by starting TIMER 5, the timer trigger for ADC 7
 * module. After each time triggered, all channels registered to ADC 7 will be 
 * scanned
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void ADC_Start ( ) {
    DRV_TMR3_Start();
}

/** @brief stop ADC conversion by starting TIMER 5, the timer trigger for ADC 7
 * module. After each time triggered, all channels registered to ADC 7 will be 
 * scanned
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void ADC_Stop ( ) {
    DRV_TMR3_Stop();
}


/** @brief update ADC value of each channel. Each packet data has channel ID and
 * its data corresponding
 * This function is call from ADC interrupt every time a channel is finished conversion 
 *  @param [in]  uint8_t channelID analog channel ID, can be:
 * - ADC_CURRENT_SENSOR_1
 * - ADC_CURRENT_SENSOR_2
 * - ADC_LIGHT_SENSOR
 * - ADC_TEMPERATURE_SENSOR_1
 * - ADC_TEMPERATURE_SENSOR_2
 * - ADC_TEMPERATURE_SENSOR_3
 * 
 *                 uint16_t channelData raw data (12 bit) after ADC conversion
 *  @param [out]  None
 *  @return None
 *  @retval true ADC data is posted successful to queue
 *  @retval false ADC data is posted to queue failed
 */
inline bool ADC_UpdateData(uint8_t channelID, uint16_t channelData) {

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    ADC_DATA_ITEM_t data;
    data.channel = channelID;
    data.data = channelData;

    if (xQueueSendToBackFromISR(s_ADCDataQueue, &data, &xHigherPriorityTaskWoken) != pdPASS) {
        xQueueReset(s_ADCDataQueue);
        return false;
    }

    return true;
}

/** @brief handle ADC data from ADC queue. each ADC channel value is automatically
 * updated to ADC queue after its conversion if done.
 * This operation get average of each ADC channel, then bypass them to a low pass
 * filter to reduce noise. The output of the low pass filter is used to calculate/
 * display by any tasks
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void ADC_HandleData() {
    uint8_t countCurr1 = 0;
    uint8_t countCurr2 = 0;
    uint8_t countLight = 0;
    uint8_t countVolt = 0;
    uint8_t countVoltNiMh = 0;
    uint8_t countVoltInput = 0;

    uint32_t sumCurr1 = 0;
    uint32_t sumCurr2 = 0;
    uint32_t sumLight = 0;
    uint32_t sumVolt = 0;
    uint32_t sumVoltNiMh = 0;
    uint32_t sumVoltInput = 0;

    ADC_DATA_ITEM_t dataEvent;
    while (xQueueReceive(s_ADCDataQueue, &dataEvent, 0) == pdTRUE) {
        uint8_t sensorID = dataEvent.channel;
        uint16_t sensorData = dataEvent.data;

        switch (sensorID) {
            case ADC_CURRENT_SENSOR_1:
                sumCurr1 += sensorData;
                countCurr1++;
                break;
            case ADC_CURRENT_SENSOR_2:
                sumCurr2 += sensorData;
                countCurr2++;
                break;
            case ADC_LIGHT_SENSOR:
                sumLight += sensorData;
                countLight++;
                break;
            case ADC_VOLT_IH_SENSOR:
                sumVolt += sensorData;
                countVolt++;
                break;
            case ADC_VOLT_NIMH_MONITOR:
                sumVoltNiMh += sensorData;
                countVoltNiMh++;
                break;
            case ADC_VOLT_INPUT_MONITOR:
                sumVoltInput += sensorData;
                countVoltInput++;
                break;               
            default:
                break;
        }
    }

    //calculate average output
    if (xSemaphoreTake(s_ADCMutex, ADC_MUTEX_MAX_WAIT_MS) == pdTRUE) {
        if (countCurr1 != 0) {
            s_CurrentSensor1 = (float) sumCurr1 / (float) countCurr1;
        }
        if (countCurr2 != 0) {
            s_CurrentSensor2 = (float) sumCurr2 / (float) countCurr2;
        }
        if (countLight != 0) {
            s_LightSensor = (float) sumLight / (float) countLight;
        }
        if (countVolt != 0) {
            s_VoltageSensor = (float) sumVolt / (float) countVolt;
        }
        if (countVoltNiMh != 0) {
            s_VoltageNiMh = (float) sumVoltNiMh / (float) countVoltNiMh;
        }
        if (countVoltInput != 0) {
            s_VoltageInput= (float) sumVoltInput / (float) countVoltInput;
        }
      
        xSemaphoreGive(s_ADCMutex);
        //SYS_PRINT("%d, %d, %d, %d, %d, %d\n", countCurr1, countCurr2, countVolt, countLight, countVoltNiMh, countVoltInput);
    }

}

/** @brief get ADC value (in count) of indicated channel
 *  @param [in]  uint8_t channelID analog channel ID, can be:
 * - ADC_CURRENT_SENSOR_1
 * - ADC_CURRENT_SENSOR_2
 * - ADC_LIGHT_SENSOR
 * - ADC_TEMPERATURE_SENSOR_1
 * - ADC_TEMPERATURE_SENSOR_2
 * - ADC_TEMPERATURE_SENSOR_3
 * 
 *                 float* channelCount value in count of indicated channel
 *  @param [out]  None
 *  @return None
 *  @retval true getting data successful
 *  @retval false getting data failed
 */
bool ADC_GetCount(uint8_t channelID, float* channelCount) {
    if (xSemaphoreTake(s_ADCMutex, ADC_MUTEX_MAX_WAIT_MS) == pdFALSE) {
        return false;
    }

    float sensorCount = 0;
    switch (channelID) {
        case ADC_CURRENT_SENSOR_1:
            sensorCount = s_CurrentSensor1;
            break;
        case ADC_CURRENT_SENSOR_2:
            sensorCount = s_CurrentSensor2;
            break;
        case ADC_LIGHT_SENSOR:
            sensorCount = s_LightSensor;
            break;
        case ADC_VOLT_IH_SENSOR:
            sensorCount = s_VoltageSensor;
            break;
        case ADC_VOLT_NIMH_MONITOR:
            sensorCount = s_VoltageNiMh;
            break;
        case ADC_VOLT_INPUT_MONITOR:
            sensorCount = s_VoltageInput;
            break;          
        default:
            break;
    }
    *channelCount = sensorCount;
    xSemaphoreGive(s_ADCMutex);
    return true;
}

/** @brief get ADC Voltage of indicated channel. Return voltage in float, between
 * range 0 to 3.3V
 *  @param [in]  uint8_t channelID analog channel ID, can be:
 * - ADC_CURRENT_SENSOR_1
 * - ADC_CURRENT_SENSOR_2
 * - ADC_LIGHT_SENSOR
 * - ADC_TEMPERATURE_SENSOR_1
 * - ADC_TEMPERATURE_SENSOR_2
 * - ADC_TEMPERATURE_SENSOR_3
 * 
 *                 float* channelVoltage voltage return from channel
 *  @param [out]  None
 *  @return None
 *  @retval true getting voltage successful
 *  @retval false getting voltage failed
 */
bool ADC_GetVoltage(uint8_t channelID, float* channelVoltage) {
    if (xSemaphoreTake(s_ADCMutex, ADC_MUTEX_MAX_WAIT_MS) == pdFALSE) {
        return false;
    }

    float sensorCount = 0;
    switch (channelID) {
        case ADC_CURRENT_SENSOR_1:
            sensorCount = s_CurrentSensor1;
            break;
        case ADC_CURRENT_SENSOR_2:
            sensorCount = s_CurrentSensor2;
            break;
        case ADC_LIGHT_SENSOR:
            sensorCount = s_LightSensor;
            break;
        case ADC_VOLT_IH_SENSOR:
            sensorCount = s_VoltageSensor;
            break; 
        case ADC_VOLT_NIMH_MONITOR:
            sensorCount = s_VoltageNiMh;
            break;
        case ADC_VOLT_INPUT_MONITOR:
            sensorCount = s_VoltageInput;
            break;

        default:
            break;
    }
    
    *channelVoltage = sensorCount / 4096.0 * 3.3; //(12bit ADC, 3.3V voltage supply)
    xSemaphoreGive(s_ADCMutex);
    return true;
}


/** @brief report error if occur in ADC 7 module, may be send event
 * to Alarm task
 *  @param [in]  None 
 *  @param [out]  None
 *  @return None
 */
void ADC_ReportError() {

}

/* *****************************************************************************
 End of File
 */
