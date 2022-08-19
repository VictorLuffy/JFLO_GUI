/* ************************************************************************** */
/** @file [DRV8308.c]
 *  @brief {This file contains method to control DRV8308 motor driver, including
 * configure driver, start driver, stop driver, control speed of driver, detect 
 * FAULT and motor status ...}
 *  @author {truong nguyen + bui phuoc}
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */
#include <float.h>
#include "FreeRTOS.h"
#include "system_config.h"
#include "system_definitions.h"
#include "DRV8308.h"
#include "SPI_3.h"
#include "IC_8.h"
#include "ApplicationDefinition.h"
#include "AlarmInterface.h"


/** @brief Number of pole pair of the motor. Follow the data-sheet for that information */
#define MOTOR_NUM_OF_POLE_PAIR          4

/** @brief The time to determine motor stop, when the last IC8 signal come */
#define DRV8308_STOP_DETECT_TIME       100     //100ms 

/** @brief Turn on ENABLE pin, make the motor spinning */
#define MOTOR_ENABLE    MOTOR_ENABLE_OUTPUTOn()

/** @brief Turn off ENABLE pin, make no power support for motor */
#define MOTOR_DISABLE   MOTOR_ENABLE_OUTPUTOff()

/** @brief Turn on BRAKE pin, make the motor brake from spinning */
#define MOTOR_BRAKE     MOTOR_BRAKE_OUTPUTOn()

/** @brief Turn off BRAKE pin, motor will not be braked*/
#define MOTOR_UNBRAKE   MOTOR_BRAKE_OUTPUTOff()


/** @brief Flag indicate DRV8308 has error */
static E_DeviceErrorState s_Drv8308Error = eDeviceNoError;

/** @brief Actual speed of the motor, measure by IC8 */
static float s_DRV8308speedInHz = 0;

/** @brief The time tick record the latest IC8 signal. useful to determine the 
 * motor stop */
static TickType_t s_DRV8308LastSpeedDetectTick = 0;

static float s_currentPower = 0;

/** @brief local functions  */
static bool DRV8308_WriteSPI(uint8_t addr, uint16_t value);
static bool DRV8308_ReadSPI(uint8_t addr, uint16_t* value);
static void DRV8308_ReportError(void);

/** @brief Initialize DRV8308 motor driver, include initializing SPI3 for driver 
 * communication, initializing IC8 for motor speed measurement
 * This function should be called 1 times at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void DRV8308_Initialize(void) {
    SPI3_Initialize();
    IC8_Initialize();
    s_Drv8308Error = eDeviceNoError;
    s_DRV8308LastSpeedDetectTick = xTaskGetTickCount();
    s_DRV8308speedInHz = 0;
    //set motor direction
    //MOTOR_DIR_OUTPUTOff();
}

/** @brief Configure DRV8308 motor driver before operating by writing to internal
 * configuration register inside DRV8308 via SPI3. For detail information, refer
 * the driver data-sheet
 * This function should be called 1 times at start up, after the funcion DRV8308_Initialize()
 * has been called and the RTOS has started
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
bool DRV8308_Configure(void) {
    if (DRV8308_WriteSPI(0x00, 0x9161) == false)
        DRV8308_ReportError();

    if (DRV8308_WriteSPI(0x01, 0x001E) == false)
        DRV8308_ReportError();

    if (DRV8308_WriteSPI(0x02, 0x04B4) == false)
        DRV8308_ReportError();

    if (DRV8308_WriteSPI(0x03, 0x6F82) == false)
        DRV8308_ReportError();

    if (DRV8308_WriteSPI(0x04, 0x0340) == false)
        DRV8308_ReportError();

    if (DRV8308_WriteSPI(0x05, 0x300A) == false)
        DRV8308_ReportError();

    if (DRV8308_WriteSPI(0x06, 0x84B1) == false)
        DRV8308_ReportError();

    if (DRV8308_WriteSPI(0x07, 0x03BA) == false)
        DRV8308_ReportError();

    if (DRV8308_WriteSPI(0x08, 0x012C) == false)
        DRV8308_ReportError();

    if (DRV8308_WriteSPI(0x09, 0x0250) == false)
        DRV8308_ReportError();

    if (DRV8308_WriteSPI(0x0A, 0xF064) == false)
        DRV8308_ReportError();

    if (DRV8308_WriteSPI(0x0B, 0x0258) == false)//0258
        DRV8308_ReportError();

    return true;
}

/** @brief Start motor spinning by turning on the MOTOR_ENABLE pin on DRV8308 as 
 * well as turn off the BRAKE pin to make the motor spinning. The IC8 for speed 
 * measurement should be started also
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void DRV8308_Start(void) {
    
    //check error
    if (s_Drv8308Error != eDeviceNoError) {
        DRV8308_ReportError();
        return;
    }
    
    //set start speed
    if (DRV8308_WriteSPI(0x0B, 1000) == false) {
        DRV8308_ReportError();
    }

    //un-brake motor
    //MOTOR_UNBRAKE;
    vTaskDelay(10);
    //enable motor spinning
    MOTOR_ENABLE;
    //start IC8 for speed measurement
    IC8_Start();
    //reset variables prepare for new operation turn
    s_DRV8308LastSpeedDetectTick = xTaskGetTickCount();
    s_DRV8308speedInHz = 0;
}

/** @brief Stop motor from spinning by turn on the BRAKE pin and turn off the ENABLE
 * pin. The IC8 for speed measurement also be stopped
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void DRV8308_Stop(void) {
    DRV8308_SetPower(0);
    //MOTOR_DISABLE;
    //MOTOR_BRAKE;
    //IC8_Stop();
}

/** @brief Set motor power by writing to the SPEED register inside the DRV8308. 
 * The input power indicate in percentage. 0% mean stop, 100% mean maximum speed.
 * The input power will be convert to convenient value for SPEED register on DRV8308,
 * which accept value from 0 to 4095
 *  @param [in]  float power        percentage power, 0 to 100%   
 *  @param [out]  None
 *  @return None
 */
void DRV8308_SetPower(float power) {
    if (s_Drv8308Error != eDeviceNoError) {
        DRV8308_ReportError();
        return;
    }

    if (power > 100)
        power = 100;
    else if (power < 0)
        power = 0;
    s_currentPower = power;
    float fSpeed = ((float) 4095 * power) / (float) 100.0;
    uint16_t speed = (uint16_t) fSpeed;
    //set value to speed register
    DRV8308_WriteSPI(0x0B, speed);
}

/** @brief Query whether DRV8308 has error or not
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 *  @retval true        if driver error
 *  @retval false       if driver has been operating normally
 */
bool DRV8308_IsError(void) {
    if (s_Drv8308Error != eDeviceNoError) {
        return true;
    } else {
        return false;
    }
}

/** @brief Calculate motor speed by reading IC8 value
 *  @param [in]  None
 *  @param [out]  None
 *  @return float       speed of motor in Rpm
 */
float DRV8308_MonitorSpeed(void) {
    float motorSpeed;
    bool result = IC8_GetFrequencyHz(&motorSpeed);
    if (result == true) {
        s_DRV8308speedInHz = motorSpeed * 120 / (float) MOTOR_NUM_OF_POLE_PAIR / 2;
        s_DRV8308LastSpeedDetectTick = xTaskGetTickCount();
                //SYS_PRINT("\n motorSpeed = %f", s_DRV8308speedInHz);
    } else {
        s_DRV8308LastSpeedDetectTick++;
        if ((xTaskGetTickCount() - s_DRV8308LastSpeedDetectTick) >= DRV8308_STOP_DETECT_TIME) {
            //reset speed to 0
            s_DRV8308speedInHz = 0;
        }
    }
    //SYS_PRINT("motorSpeed = %.0f\n", s_DRV8308speedInHz);
    return s_DRV8308speedInHz;
}

/** @brief Calculate motor speed by reading IC8 value
 *  @param [in]  None
 *  @param [out]  None
 *  @return float       speed of motor in Rpm
 */
float DRV8308_GetLatestSpeed(void) 
{
    if(s_currentPower <= 0)
    {
        DRV8308_MonitorSpeed();
    }
    //SYS_PRINT("motorSpeed = %.0f\n", s_DRV8308speedInHz);
    return s_DRV8308speedInHz;
}


/** @brief Write a value to indicated register on DRV8308, via SPI3
 *  @param [in]     uint8_t addr        address of the register
 *                  uint16_t value      new value need to be updated
 *  @param [out]  None
 *  @return None
 *  @retval true        if writing successful
 *  @retval false       if writing failed
 */
bool DRV8308_WriteSPI(uint8_t addr, uint16_t value) {
#define DRV8308_REGISTER_WRITE_SIZE     3
    uint8_t bufferW[DRV8308_REGISTER_WRITE_SIZE];
    bufferW[0] = addr;
    bufferW[1] = (value >> 8) & 0xFF;
    bufferW[2] = value & 0xFF;
    /* Add to the write buffer and transmit*/
    if (SPI3_Write(&bufferW[0], DRV8308_REGISTER_WRITE_SIZE, 10) == true) {
        //clear error 
        s_Drv8308Error = eDeviceNoError;
        return true;
    } else {
        //set error flag
        s_Drv8308Error = eDeviceErrorDetected;
        return false;
    }
}

/** @brief Read a value from indicated register on DRV8308, via SPI3
 *  @param [in]     uint8_t addr        address of the register
 *                  uint16_t *value      external pointer buffer store value of register
 *  @param [out]  None
 *  @return None
 *  @retval true        if reading successful
 *  @retval false       if reading failed
 */
bool DRV8308_ReadSPI(uint8_t addr, uint16_t* value) {
#define DRV8308_REGISTER_READ_SIZE     3
    uint8_t address = addr | (1 << 7);
    uint8_t bufferR[DRV8308_REGISTER_READ_SIZE];
    uint16_t readValue = 0;
    /* Add to the write buffer and transmit*/
    if (SPI3_WriteRead(&address, sizeof (address), &bufferR[0], sizeof (bufferR), 10) == true) {
        readValue = (bufferR[1] << 8) + bufferR[2];
        //clear error 
        s_Drv8308Error = eDeviceNoError;
        *value = readValue;
        return true;
    } else {
        //set error flag
        s_Drv8308Error = eDeviceErrorDetected;
        return false;
    }
}

/** @brief Report error if occur on DRV8308, may be send event to Alarm task
 *  @param [in]  None 
 *  @param [out]  None
 *  @return None
 */
void DRV8308_ReportError(void) {
    //check whether an error is detected, then send event to ALarm task
    if (s_Drv8308Error == eDeviceErrorDetected) {
        //send event to alarm task
        alarmInterface_SendEvent(eDRV8308ErrorAlarm, eActive, eHighPriority, 0);
        //change state
        s_Drv8308Error = eDeviceErrorReported;
    }
}

bool DRV8308_CheckMotorError(void)
{
    if(s_currentPower >= 0)
    {
       if(s_DRV8308speedInHz <= 0)
          return false;
    }
    else
    {
        return true;
    }
}

float DRV8308_GetCurrentPower(void)
{
    //SYS_PRINT("DRV8308_CurrentPower %.1f\n", s_currentPower);
    return s_currentPower;
}
/* *****************************************************************************
 End of File
 */
