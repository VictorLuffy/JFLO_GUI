/* ************************************************************************** */
/** @file [PWM_Motor.h]
 *  @brief {Generate PWM with various frequency to control DRV8308 motor driver.
 * the Motor speed will respond follow that input PWM. Motor speed for that application
 * is specified from 400 Hz to 2.0 Khz, respectively with RPM from 6 krpm to 31 krpm.
 * Since the motor has 8 poles (4 pole pairs), 3 phase PMSM, 3 hall sensors mounted on ROTO, the 
 * formula to convert from RPM to Hz will be: f(Hz) = 4 * RPM / 60
 * All settings and configurations on this file relate tightly with the MPLAB HARMONY configuration.
 * Change the configuration of the HARMONY may lead these function failed}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */



/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "system/debug/sys_debug.h"
#include "PWM_Motor.h"
#include "IC_8.h"

/** @brief Number of pole pair of the motor. Follow the data-sheet for that information */
#define MOTOR_NUM_OF_POLE_PAIR          4

/** @brief The time to determine motor stop, when the last IC8 signal come */
#define MOTOR_STOP_DETECT_TIME       100     //100ms 

/** @brief Turn off ENABLE pin, make the motor spinning */
#define MOTOR_ENABLE    MOTOR_ENABLEOff()

/** @brief Turn on ENABLE pin, make no power support for motor */
#define MOTOR_DISABLE   MOTOR_ENABLEOn()

/** @brief Turn on BRAKE pin, make the motor brake from spinning */
#define MOTOR_BRAKE     MOTOR_BRAKEOff()

/** @brief Turn off BRAKE pin, motor will not be braked*/
#define MOTOR_UNBRAKE   MOTOR_BRAKEOn()


#define MOTOR_CW   MOTOR_DIRECTIONOn()
#define MOTOR_CCW   MOTOR_DIRECTIONOff()



/** @brief Clock source for TIMER 2, the timer base of PWM MOTOR 
 Clock source frequency is calculated by input source SYS_CLK_BUS_PERIPHERAL_3 = 100Mhz,
 by pass the PRE-SCALE = TMR_PRESCALE_VALUE_8*/
#define PWM_MOTOR_TIMER_CLK_SOURCE_HZ      ((float)12500000.0)    //12.5 MHz 

/** @brief Initial value of TIMER 2, set to 250, combine with pre-scale value (8) 
 * to get minimum frequency */
#define PWM_MOTOR_TIMER_INIT_VALUE       250

/** @brief OC PWM index assigned for PWM_MOTOR channel, in this case, index 2 */
#define PWM_MOTOR_INDEX                 2 

/** @brief Maximum duty cycle accept for the PWM MOTOR chanel on this application */
#define PWM_MOTOR_MAX_DUTY_CYCLE        ((float)100.0)      //convert to RPM = 31 krpm

/** @brief Minimum duty cycle accept for the PWM MOTOR chanel on this application */
#define PWM_MOTOR_MIN_DUTY_CYCLE          ((float)1.0)       //convert to RPM = 6 krpm




/** @brief Handle PWM for MOTOR control signals */
static DRV_HANDLE s_PWM_MotorHandle = DRV_HANDLE_INVALID;

/** @brief Frequency apply for the PWM MOTOR*/
static uint16_t s_timerInCount = PWM_MOTOR_TIMER_INIT_VALUE; // value to generate 400 Hz PWM 


/** @brief Flag indicate DRV8308 has error */
//static E_DeviceErrorState s_Drv8308Error = eDeviceNoError;

/** @brief Actual speed of the motor, measure by IC8 */
static float s_SpeedInHz = 0;

/** @brief The time tick record the latest IC8 signal. useful to determine the 
 * motor stop */
static TickType_t s_LastSpeedDetectTick = 0;



/** @brief Function to initialize PWM_MOTOR to control DRV8308 motor driver, 
 * includes setting the PWM MOTOR duty cycle 25%, frequency is 50 kHz but not start yet
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_Motor_init() {
    //initialize PWM channels for Humidity signals
    if (s_PWM_MotorHandle == DRV_HANDLE_INVALID) {
        s_PWM_MotorHandle = DRV_OC_Open(PWM_MOTOR_INDEX, DRV_IO_INTENT_EXCLUSIVE);
    }

    //set frequency 50 kHz, but do not start
    DRV_TMR0_PeriodValueSet(PWM_MOTOR_TIMER_INIT_VALUE);

    //set 50% duty cycle
    DRV_OC_PulseWidthSet(s_PWM_MotorHandle, PWM_MOTOR_TIMER_INIT_VALUE / 2);
    
    IC8_Initialize();
    //s_Drv8308Error = eDeviceNoError;
    s_LastSpeedDetectTick = xTaskGetTickCount();
    s_SpeedInHz = 0;
    //set motor direction
    //MOTOR_DIR_OUTPUTOff();
    //MOTOR_CW;
}

/** @brief Function to start output PWM to PWM_MOTOR PIN.
 * Before calling this function, make sure the function PWM_Motor_init() had been called
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_Motor_start() {
    //start timer 0
    DRV_TMR0_Start();
    //start OC PWM
    DRV_OC_Start(s_PWM_MotorHandle, DRV_IO_INTENT_EXCLUSIVE);
    
    //un-brake motor
    MOTOR_UNBRAKE;
    //vTaskDelay(10);
    //enable motor spinning
    MOTOR_ENABLE;
    //start IC8 for speed measurement
    IC8_Start();
    //reset variables prepare for new operation turn
    s_LastSpeedDetectTick = xTaskGetTickCount();
    s_SpeedInHz = 0;
    
}

///** @brief Function to set frequency of the PWM MOTOR while keeping the
// * duty cycle 50%. Frequency can be from 400 Hz to 2.0 KHz
// * Before calling this function, make sure the function PWM_Motor_init() had been called
// *  @param [in]  float freInHz: expected frequency in Hz
// *  @param [out]  None
// *  @return None
// */
//
//void PWM_Motor_setFrequency(float freInHz) {
//    //check input value in range
//    float fre = freInHz;
//    if (fre > PWM_MOTOR_MAX_FREQ_HZ) {
//        fre = PWM_MOTOR_MAX_FREQ_HZ;
//    } else if (fre < PWM_MOTOR_MIN_FREQ_HZ) {
//        fre = PWM_MOTOR_MIN_FREQ_HZ;
//    }
//
//    //convert frequency to timer count
//    float count = PWM_MOTOR_TIMER_CLK_SOURCE_HZ / fre;
//
//    //update timer count
//    s_timerInCount = (uint16_t) count;
//
//    //update timer period
//    DRV_TMR0_PeriodValueSet(s_timerInCount);
//
//    //update OC value to keep 50% duty cycle
//    DRV_OC_PulseWidthSet(s_PWM_MotorHandle, s_timerInCount / 2);
//}
/** @brief Function to set duty cycle of the PWM MOTOR.
 * Duty cycle can be from 5 to 100% 
 * Before calling this function, make sure the function PWM_Motor_init() had been called
 *  @param [in]  float dutyCycle: expected duty cycle in %
 *  @param [out]  None
 *  @return None
 */

void PWM_Motor_setDutyCycle(float dutyCycle) {
    //check input value in range
    float duty = dutyCycle;
    if (duty > PWM_MOTOR_MAX_DUTY_CYCLE) {
        duty = PWM_MOTOR_MAX_DUTY_CYCLE;
    } else if (duty < PWM_MOTOR_MIN_DUTY_CYCLE) {
        duty = PWM_MOTOR_MIN_DUTY_CYCLE;
    }
    //SYS_PRINT("motor set dutyCycle %.0f \n", dutyCycle);
    //convert duty cycle to timer count
    float count = PWM_MOTOR_TIMER_INIT_VALUE * duty / 100.0;

    //update OC value
    DRV_OC_PulseWidthSet(s_PWM_MotorHandle, count);
}

/** @brief Function to stop output PWM to PWM MOTOR PIN.
 * Before calling this function, make sure the function PWM_Motor_init() had been called
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_Motor_stop() {
    //stop timer base
    DRV_TMR0_Stop();
    //stop OC module
    DRV_OC_Stop(s_PWM_MotorHandle);
    
    MOTOR_BRAKE;
}


/** @brief Calculate motor speed by reading IC8 value
 *  @param [in]  None
 *  @param [out]  None
 *  @return float       speed of motor in Rpm
 */
float PWM_Motor_MonitorSpeed(void) {
    float motorSpeed;
    bool result = IC8_GetFrequencyHz(&motorSpeed);
    if (result == true) {
        s_SpeedInHz = motorSpeed * 120 / (float) MOTOR_NUM_OF_POLE_PAIR / 2;
        s_LastSpeedDetectTick = xTaskGetTickCount();
                //SYS_PRINT("motorSpeed = %.0f\n", s_SpeedInHz);
    } else {
        s_LastSpeedDetectTick++;
        if ((xTaskGetTickCount() - s_LastSpeedDetectTick) >= MOTOR_STOP_DETECT_TIME) {
            //reset speed to 0
            s_SpeedInHz = 0;
        }
    }
    //SYS_PRINT("motorSpeed = %.0f\n", s_DRV8308speedInHz);
    return s_SpeedInHz;
}

/** @brief Calculate motor speed by reading IC8 value
 *  @param [in]  None
 *  @param [out]  None
 *  @return float       speed of motor in Rpm
 */
float PWM_Motor_GetLatestSpeed(void) {
    //SYS_PRINT("motorSpeed = %.0f\n", s_SpeedInHz);
    return s_SpeedInHz;
}


/* *****************************************************************************
 End of File
 */
