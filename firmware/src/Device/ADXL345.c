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

#include "ApplicationDefinition.h"
#include "I2C_3.h"
#include "ADXL345.h"



/** @brief I2C slave address of the ADXL345 */
#define ADXL345_BASE_ADDR       (0x53)

/** @brief I2C address while writing data to ADXL345
 * write address = base address << 1 + write bit (0))
 */
#define ADXL345_WRITE_ADDR      (ADXL345_BASE_ADDR << 1)

/** @brief I2C address while reading data from ADXL345
 * read address = base address << 1 + read bit(1))
 */
#define ADXL345_READ_ADDR       ((ADXL345_BASE_ADDR << 1) + 1)

/** @brief max time wait for ADXL345 communication (in ms) via I2C*/
#define ADXL345_MAX_WAIT_MS          (10)


static E_DeviceErrorState s_ADXL345Error = eDeviceNoError;

/** @brief rotation around X-axis */
static float s_roll;

/** @brief rotation around Y-axis */
static float s_pitch;


static void ADXL345_ReportError();

/** @brief Function to initialize ADXL345, includes configuring the ADXL345 to work in expected mode 
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
bool ADXL345_Initialize() {
    uint8_t configureData[2] = {
        0x2D, //POWER_CTL Register - 0x2D
        0x08 // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable
    };

    int i;
    bool result;
    for (i = 0; i < 10; i++) {      //max try 10 times
        result = I2C3_Write(ADXL345_WRITE_ADDR, &configureData[0], sizeof(configureData), ADXL345_MAX_WAIT_MS);
        if (result == true) {
            break;
        }
        //if sensor is not ready, it may be sensor need time from power supply to start
        //should wait and try again here
        vTaskDelay(10);
    }
    
    if (result == false) {
        SYS_PRINT("ERROR while trying to configure ADXL345 sensor\n");
        return false;
    }
    else {
    /*
        uint8_t status;
        if ( I2C3_Read(ADXL345_READ_ADDR, &status, 1, ADXL345_MAX_WAIT_MS) == false ) {
            SYS_PRINT("2. ERROR while trying to configure ADXL345 sensor");
            return false;
        }*/
        //SYS_PRINT("configure ADXL345 sensor SUCCESS \n");
        return true;
    }
}


bool ADXL345_ReadAccelerometer() {
    uint8_t address = 0x32;
    uint8_t buffer[6];
    static uint16_t s_errorCount = 0;
            
    //if (s_ADXL345Error != eDeviceNoError) {
    //    //report error
    //    ADXL345_ReportError();
    //    return false;
    //}
    bool result = I2C3_Write(ADXL345_WRITE_ADDR, &address, sizeof(address), ADXL345_MAX_WAIT_MS);
    
    if ( result == false)
    {
        //SYS_PRINT("1. ERROR while trying writing coordinate from  ADXL345 sensor");
        s_errorCount++;
    }
    else
    {   
        result = I2C3_Read(ADXL345_READ_ADDR, buffer, sizeof(buffer), ADXL345_MAX_WAIT_MS);
        
        if (result == false ) 
        {
            //SYS_PRINT("2. ERROR while trying reading coordinate from  ADXL345 sensor");
            s_errorCount++;
        }
        else{
            s_errorCount = 0;
            s_ADXL345Error = eDeviceNoError;
        }
    }
  
    // error in 3 seconds continuously
    if(s_errorCount >= (3000/200))
    {
        s_ADXL345Error = eDeviceErrorDetected;
    }
    
    if(result == false)
    {
        return false;
    }
    
    float rawX = buffer[0] + (((int8_t)buffer[1])<< 8);
    float rawY = buffer[2] + (((int8_t)buffer[3]) << 8);
    float rawZ = buffer[4] + (((int8_t)buffer[5]) << 8);
   
    

    float x = rawX / 256.0;
    float y = rawY / 256.0;
    float z = rawZ / 256.0;
  
    //SYS_PRINT("x = %.2f; y = %.2f; z = %.2f\n", x, y, z);
  
    // Calculate Roll and Pitch (rotation around X-axis, rotation around Y-axis)
    float PI = 3.14159265359;
    s_roll = atan(y / sqrt(pow(x, 2) + pow(z, 2))) * 180 / PI;
    s_pitch = atan(-1 * x / sqrt(pow(y, 2) + pow(z, 2))) * 180 / PI;

    // Low-pass filter
    //float rollF = 0.94 * rollF + 0.06 * roll;
    //float pitchF = 0.94 * pitchF + 0.06 * pitch;
  
    //SYS_PRINT("roll = %.2f; pitch = %.2f\n", s_roll, s_pitch);
    //SYS_PRINT("rollF = %.2f; pitchF = %.2f\n", rollF, pitchF);
    return true;
}

/** @brief Report error if occur during communicate with ADXL345
 *  @param [in]   None
 *  @param [out]  None
 *  @return None
 */
void ADXL345_ReportError() {
    //check whether an error is detected, then send event to ALarm task
    if (s_ADXL345Error == eDeviceErrorDetected) {
        //send event to alarm task
        //alarmInterface_SendEvent(eBME280ErrorAlarm, eActive, 0);
        SYS_PRINT("\n error at: ADXL345_ReportError");
        //change state
        s_ADXL345Error = eDeviceErrorReported;
        
        //HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId};
        //HeaterTask_SendEvent(hEvent);
    }
}

/** @brief Query any error happen with ADXL345 
 *  @param [in]  None   
 *  @param [out] None
 *  @return None
 *  @retval true ADXL345 sensor has error
 *  @retval false ADXL345 sensor is OK
 */
bool ADXL345_IsSensorFailed() {
    if (s_ADXL345Error == eDeviceNoError) {
        return false;       //sensor OK
    }
    else {
        return true;        //sensor ERROR
    }
}    

/** @brief Get Accelerometer
 *  @param [in]  None   
 *  @param [out] float* xAngle rotation around X-axis
 *  @param [out] float* yAngle rotation around Y-axis
 *  @return None
 *  @retval true ADXL345 sensor has error
 *  @retval false ADXL345 sensor is OK
 */
bool ADXL345_GetAccelerometer( float* xAngle, float* yAngle) {
    if (s_ADXL345Error == eDeviceNoError)
    {
        *xAngle = s_pitch;
        *yAngle = s_roll;
        return true;       //sensor OK
    }
    else 
    {      
        return false;        //sensor ERROR
    }
}   

/* *****************************************************************************
 End of File
 */
