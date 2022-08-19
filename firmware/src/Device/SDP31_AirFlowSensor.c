/* ************************************************************************** */
/** @file [SDP31_AirFlowSensor.c]
 *  @brief {support interface to communicate with SDP31 Air Flow Sensor such as: 
 * configure, get flow value, get scale factor, report error if occur
 * during communication}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */




/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#include "FreeRTOS.h"
#include "semphr.h"

#include "system_config.h"
#include "system_definitions.h"
#include "driver/i2c/drv_i2c.h"
#include "I2C_1.h"
#include "SDP31_AirFlowSensor.h"
#include "AlarmInterface.h"
#include "ApplicationDefinition.h"

/** @brief I2C slave address of the Air flow sensor */
#define AIR_FLOW_SENSOR_BASE_ADDR           (0x21)

/** @brief I2C address while writing data to Air flow sensor
 * write address = base address << 1 + write bit (0))
 */
#define AIR_FLOW_SENSOR_WRITE_ADDR          (AIR_FLOW_SENSOR_BASE_ADDR << 1)

/** @brief I2C address while reading data from air flow sensor
 * read address = base address << 1 + read bit(1))
 */
#define AIR_FLOW_SENSOR_READ_ADDR           ((AIR_FLOW_SENSOR_BASE_ADDR << 1) + 1)

/** @brief max time wait for flow sensor communication (in ms) via I2C*/
#define AIR_FLOWSENSOR_COMM_MAX_WAIT_MS     (4 / portTICK_PERIOD_MS)

/** @brief max time wait for MUTEX to access protection resource */
#define AIR_FLOWSENSOR_MUTEX_MAX_WAIT_MS    (2 / portTICK_PERIOD_MS)

#define AIR_FLOWRATE_COEFICIENT             (5.1)//4.44//(4.37*0.6)

#define AIR_FLOWRATE_OFFSET                 (-2.3)//-4

/** @brief scale factor for Air Flow to convert raw data to physical data*/
static uint16_t s_AirFlowSensorScaleFactor = 0;

/** @brief latest Air flow value obtained from Air Flow sensor*/
static float s_AirLastFlow = 0;

/** @brief Flag indicate Air Flow Sensor has error
 * true mean error happened
 * false mean no error */
static E_DeviceErrorState s_AirFlowSensorError = eDeviceNoError;

/** @brief command to configure air flow sensor: configure at continue mode, 
 * average til read*/
const uint8_t airFlowConfigureCmd[] = {0x36, /*0x03*/0x08};


/** @brief internal functions declaration */
static void AirFlowSensor_ReportError();
static bool AirFlowSensor_GetFlowData(int16_t* rawFlow);
static bool AirFlowSensor_GetFullData(int16_t* rawFlow, int16_t* rawTemp, uint16_t* scaleFactor);
static bool AirFlowSensor_Configure();
static bool AirFlowSensor_ReadScaleFactor();

/** @brief Initialize SDP31 sensor measure for Air source by configure its operation 
 * mode and read all scale factor prepare before operation
 * This function should be called after RTOS is started and call 1 time only after
 * start up system
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void AirFlowSensor_Initialize() {
    //configure operation mode
    if (AirFlowSensor_Configure() == false) {
        AirFlowSensor_ReportError();
    }
    //Read sensor scale factor
    if (AirFlowSensor_ReadScaleFactor() == false) {
        AirFlowSensor_ReportError();
    }
    //reset variables
    //s_AirFlowSensorError = eDeviceNoError;
}

/** @brief reset Air flow sensor by putting reset pin to LOW in 2 ms
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void AirFlowSensor_Reset() {
    //reset variables
    s_AirFlowSensorError = eDeviceNoError;
    s_AirLastFlow = 0;
}

/** @brief get current flow from the SDP31 Air Flow Sensor. Since the sensor is
 * configured as "Average til Read",the read value is the average value of all samples
 * from the last read.
 * The raw value, after reading via I2C, is divided by Scale factor. 
 * This function should be called after the sensor is already started operation and
 * the Scale factor is obtained.
 *  @param [in]  None
 *  @param [out]  float* flowVal    pointer to store flow value after divide by scale factor (LPM)
 *  @return None
 *  @retval true    getting flow value OK
 *  @retval false   getting flow value Failed
 */
bool AirFlowSensor_GetFlow(float* flowVal) {
    //check whether sensor error
    if ((s_AirFlowSensorError != eDeviceNoError) || (s_AirFlowSensorScaleFactor == 0))
    {
        AirFlowSensor_ReportError();
        return false;
    }

    //get data from sensor
    int16_t rawFlow = 0;
    if (AirFlowSensor_GetFlowData(&rawFlow) == false) {
        AirFlowSensor_ReportError();
        return false;
    }

    float flow = (float) rawFlow / (float) s_AirFlowSensorScaleFactor;
    flow = fabsf(flow);
    flow = sqrtf(flow) * AIR_FLOWRATE_COEFICIENT + AIR_FLOWRATE_OFFSET;

    //update value here
    s_AirLastFlow = flow;

    //output value
    *flowVal = flow;
    
    return true;
}

/** @brief Get last flow value. This function return previous value obtain from
 * Air flow sensor without I2C communication .
 *  @param [in]  None
 *  @param [out]  None
 *  @return float flow in (LPM)
 */
float AirFlowSensor_GetLastFlow() {
    return s_AirLastFlow;
}

/** @brief send command via I2C1 to configure SDP31 as continuous measurement.
 * Before sending, make sure the I2C1 bus is not busy
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 *  @retval true start SDP31 Air flow sensor OK
 *  @retval false start SDP31 Air flow sensor Failed
 */
bool AirFlowSensor_Configure() {
    int i;
    bool result;
    //transmit data and check result
    for (i = 0; i < 10; i++) {      //max try 10 times
        result = I2C1_Write(AIR_FLOW_SENSOR_WRITE_ADDR, (void*) &airFlowConfigureCmd[0], 
                sizeof (airFlowConfigureCmd), AIR_FLOWSENSOR_COMM_MAX_WAIT_MS);
        if (result == true) {
            break;
        }
        //if sensor is not ready, it may be sensor need time from power supply to start
        //should wait and try again here
        vTaskDelay(5);
    }

    if (result == false) {
        //set error flag
        s_AirFlowSensorError = eDeviceErrorDetected;
        return false;
    }
    else {
        //clear error 
        s_AirFlowSensorError = eDeviceNoError;
        return true;
    }
}

/** @brief Read Scale factor of the SDP31 Air Flow Sensor. The Scale factor is
 * used to convert raw flow data to physical flow in LPM 
 * Scale factor must be get before reading flow. This function should be called
 * 1 time after send command to start operation for the sensor
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 *  @retval true get scale factor OK
 *  @retval false get scale factor Failed
 */
bool AirFlowSensor_ReadScaleFactor() {
    int i;
    bool result;
    //transmit data and check result
    for (i = 0; i < 10; i++) {      //max try 10 times
        //get all data from sensor
        result = AirFlowSensor_GetFullData(NULL, NULL, &s_AirFlowSensorScaleFactor);
        if (result == true) {
            break;
        }
        //if sensor is not ready, it may be sensor need time from power supply to start
        //should wait and try again here
        vTaskDelay(5);
    }
    
    if (result == false) {
        //set error flag
        s_AirFlowSensorError = eDeviceErrorDetected;
        return false;
    }
    else {
        //clear error 
        s_AirFlowSensorError = eDeviceNoError;
        return true;
    }
}

/** @brief Read flow data (3 bytes) from SDP31 Air Flow Sensor via I2C1, 
 * then store it to external buffer. A semaphore is used to synchronize when 
 * I2C transfer has completed. 
 * Make sure the Air Flow Sensor already started before execute this function
 *  @param [in]  None
 *  @param [out]  int16_t* rawFlow: external buffer to store raw flow value
 *  @return None
 *  @retval true getting Flow value OK
 *  @retval false getting Flow value Failed
 */
bool AirFlowSensor_GetFlowData(int16_t* rawFlow) {
#define AIR_FLOW_BYTE_NUM   (3)
    static uint16_t s_errorCount = 0;
    uint8_t tempBuffer[AIR_FLOW_BYTE_NUM] = {0};

    bool result = I2C1_Read(AIR_FLOW_SENSOR_READ_ADDR, (void*) &tempBuffer[0], 
                            AIR_FLOW_BYTE_NUM, AIR_FLOWSENSOR_COMM_MAX_WAIT_MS);
    if (result == true) 
    {
        //check CRC
        if (crcFlowSensor_Check(tempBuffer, 2, tempBuffer[2])) 
        {
            //calculate flow and output it
            *rawFlow = (int16_t) ((tempBuffer[0] << 8) + tempBuffer[1]);
            //clear error 
            s_AirFlowSensorError = eDeviceNoError;
            //clear err counter
            s_errorCount = 0;
        }
        else 
        {
           // result = false;
            s_errorCount++; SYS_PRINT ("Air sensor CRC Fail\n");
        }
    }
    else{
        s_errorCount++;
        result = true;
        SYS_PRINT ("Air sensor err\n");
    }
    // error in 3 seconds continuously
    if(s_errorCount >= 300){
        result = false;
    }
    
    
    //check result
    if (result == false) {
        //set error flag
        s_AirFlowSensorError = eDeviceErrorDetected;
        return false;
    }
    else {
        //clear error 
        s_AirFlowSensorError = eDeviceNoError;
        return true;
    }
}

/** @brief Read full data (9 bytes) from SDP31 Air Flow Sensor via I2C1, includes 
 * 3 bytes Flow, 3 bytes Temperature and 3 bytes Scale factor, then store it to 
 * external buffer. A semaphore is used to synchronize when I2C transfer has completed. 
 * Make sure the Air Flow Sensor already started before execute this function
 *  @param [in]  None
 *  @param [out]    int16_t* rawFlow: external buffer pointer to store raw flow value
 *                  int16_t* rawTemp: external buffer pointer to store raw temperature value
 *                  uint16_t* scaleFactor: external buffer pointer to store raw Scale Factor value
 *  @return None
 *  @retval true getting these data OK
 *  @retval false getting these data Failed
 */
bool AirFlowSensor_GetFullData(int16_t* rawFlow, int16_t* rawTemp, uint16_t* scaleFactor) {
#define AIR_FULL_DATA_BYTE_NUM                   (9)
    uint8_t tempBuffer[AIR_FULL_DATA_BYTE_NUM] = {0};

    //execute I2C transfer, then check result
    if (I2C1_Read(AIR_FLOW_SENSOR_READ_ADDR, (void*) &tempBuffer[0], AIR_FULL_DATA_BYTE_NUM, AIR_FLOWSENSOR_COMM_MAX_WAIT_MS) == false) {
        //set error flag
        s_AirFlowSensorError = eDeviceErrorDetected;
        return false;
    }

    //check CRC for raw flow
    if (crcFlowSensor_Check(&tempBuffer[0], 2, tempBuffer[2])
            && crcFlowSensor_Check(&tempBuffer[3], 2, tempBuffer[5])
            && crcFlowSensor_Check(&tempBuffer[6], 2, tempBuffer[8])) {
        //output value for flow
        if (rawFlow != NULL)
            *rawFlow = (tempBuffer[0] << 8) + tempBuffer[1];
        //output value for temperature
        if (rawTemp != NULL)
            *rawTemp = (tempBuffer[3] << 8) + tempBuffer[4];
        //output value for scale factor
        if (scaleFactor != NULL)
            *scaleFactor = (tempBuffer[6] << 8) + tempBuffer[7];

        //clear error 
        s_AirFlowSensorError = eDeviceNoError;
        //return as good
        return true;
    } 
    else {
        //set error flag
        s_AirFlowSensorError = eDeviceErrorDetected;
        return false;
    }
}

/** @brief report error if occur during communication with SDP31 Air Flow Sensor. 
 * An error detected may be sent via an event to Alarm task
 *  @param [in]  None 
 *  @param [out]  None
 *  @return None
 */
void AirFlowSensor_ReportError() {
    //check whether an error is detected, then send event to ALarm task
    if (s_AirFlowSensorError == eDeviceErrorDetected)
    {
        SYS_PRINT("\n AirFlowSensor_ReportError\n");
        //send event to alarm task
        //alarmInterface_SendEvent(eAirFlowSensorErrorAlarm, eActive, 0);
        //change state
        s_AirFlowSensorError = eDeviceErrorReported;
    }
}
/** @brief Query any error happen with Air Flow Sensor
 *  @param [in]  None   
 *  @param [out] None
 *  @retval true Air Flow sensor has error
 *  @retval false Air Flow sensor is OK
 */
bool AirFlowSensor_IsSensorFailed() {
    if (s_AirFlowSensorError == eDeviceNoError) {
        return false;       //sensor OK
    }
    else {
        return true;        //sensor ERROR
    }
}


/* *****************************************************************************
 End of File
 */
