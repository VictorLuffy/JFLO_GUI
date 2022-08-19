/* ************************************************************************** */
/** @file [PWM_LCDBacklight.c]
 *  @brief {Generate PWM to adjust the LCD back-light brightness. 
 * LCD back-light require PWM has frequency 1 KHz, duty cycle from 10% (darkest)
 * to 100% (brightest)
 * The LCD Back-light PWM channel use OC6 combine with Timer3 to generate PWM
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
#include "PWM_LCDBacklight.h"
#include "ADC.h"
#include "Setting.h"
#include "ApplicationDefinition.h"
#include "AlarmEventHandler.h"


/** @brief OC PWM index assigned for PWM_LCDBACKLIGHT channel, in this case, index 3 */
#define PWM_LCDBACKLIGHT_INDEX             3

/** @brief Value of TIMER 3, set to 31250, combine with pre-scale value (32) 
 * to generate 1KHz PWM */
#define PWM_LCDBACKLIGHT_TIMER_VALUE       (3125)

/** @brief Maximum duty cycle accept for the PWM LCDBACKLIGHT chanel on this application */
#define PWM_LCDBACKLIGHT_MAX_DUTY          (100)      

/** @brief Minimum duty cycle accept for the PWM LCDBACKLIGHT chanel on this application */
#define PWM_LCDBACKLIGHT_MIN_DUTY          (10)       



/** @brief Handle PWM for LCD Back-light control signals */
static DRV_HANDLE s_PWM_LCDBacklightHandle = DRV_HANDLE_INVALID;

/** @brief Flag indicate illumination sensor has error
 * true mean error happened
 * false mean no error */
static E_DeviceErrorState s_IlluSensorError = eDeviceNoError;

static bool s_IsBackLightEnabled = false;


/** @brief Function to initialize PWM_LCDBACKLIGHT to control LCD back-light 
 * brightness, includes setting the PWM_LCDBACKLIGHT duty cycle 100% but not start yet
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_LCDBacklight_Initialize() {
    //initialize PWM channels for Humidity signals
    if (s_PWM_LCDBacklightHandle == DRV_HANDLE_INVALID) {
        s_PWM_LCDBacklightHandle = DRV_OC_Open(PWM_LCDBACKLIGHT_INDEX, DRV_IO_INTENT_EXCLUSIVE);
    }

    //set frequency 1000 Hz, but do not start
    DRV_TMR1_PeriodValueSet(PWM_LCDBACKLIGHT_TIMER_VALUE);

    //set 100% duty cycle
    DRV_OC_PulseWidthSet(s_PWM_LCDBacklightHandle, 0/*PWM_LCDBACKLIGHT_TIMER_VALUE*/);
}

/** @brief Function to start output PWM to PWM_LCDBACKLIGHT PIN.
 * Before calling this function, make sure the function PWM_LCDBacklight_Initialize() 
 * had been called
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_LCDBacklight_Start() {
    //start timer 0
    DRV_TMR1_Start();
    //start OC PWM
    DRV_OC_Start(s_PWM_LCDBacklightHandle, DRV_IO_INTENT_EXCLUSIVE);
}

/** @brief Function to set duty cycle of the PWM LCDBACKLIGHT while keeping the
 * frequency at 1 KHz. Duty cycle can be from 10% to 100%
 * Before calling this function, make sure the function PWM_LCDBacklight_Initialize() 
 * had been called
 *  @param [in]  uint16_t percentage: expected duty cycle (from 10 to 100)
 *  @param [out]  None
 *  @return None
 */
void PWM_LCDBacklight_SetDutyCycle ( uint16_t percentage ) {
    //check input value in range
    uint16_t duty = percentage;
    if (duty > PWM_LCDBACKLIGHT_MAX_DUTY) {
        duty = PWM_LCDBACKLIGHT_MAX_DUTY;
    } else if (duty < PWM_LCDBACKLIGHT_MIN_DUTY) {
        duty = PWM_LCDBACKLIGHT_MIN_DUTY;
    }

    //convert duty to OC count
    float count = (duty * PWM_LCDBACKLIGHT_TIMER_VALUE) / 100;

    //update OC value to keep 50% duty cycle
    DRV_OC_PulseWidthSet(s_PWM_LCDBacklightHandle, count);
}

/** @brief This function read the voltage of light sensor and control brightness  
 * 
 * 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void PWM_LCDBacklight_HandleBrightness () {
#define NUM_SAMPLE    100
#define HIGH_BRIGHT_VOL    0.05
  
    float lux = 0;
    int i;
    static float s_lux[NUM_SAMPLE] = {};
    static uint32_t s_index = 0;
    static uint32_t s_count = 0;
    

    if ((setting_IsInit() == true) && (s_IsBackLightEnabled))
    {
        //TODO
        //if alarm is active, set brightness to 100%
        if (alarmEventHandler_GetAlarmsStatus() == false)
        {
            if(setting_Get(eBrightnessModeSettingId) == eAutoBrightnessMode) 
            {
                ADC_GetVoltage(ADC_LIGHT_SENSOR, &lux);
                s_lux[s_index] = lux;
                s_index++;
                s_index = s_index % NUM_SAMPLE;
                if(s_count < NUM_SAMPLE)
                {
                    s_count++;
                }
                else
                {
                    float avrLux = 0;
                    for(i = 0; i < NUM_SAMPLE; i++)
                    {
                        avrLux = avrLux + (s_lux[i]/NUM_SAMPLE);
                    }
                    //SYS_PRINT("avrLux %f\n", meanLux);
                    if(avrLux < HIGH_BRIGHT_VOL){
                        PWM_LCDBacklight_SetDutyCycle(10);
                    }
                    else{
                        PWM_LCDBacklight_SetDutyCycle(100);
                    }
                    if(avrLux == 0)
                    {
                      s_IlluSensorError = eDeviceErrorDetected;
                    }
                }
            }
            else
            {
                uint16_t brightness = (uint16_t)setting_Get(eBrightnessLevelSettingId);
                PWM_LCDBacklight_SetDutyCycle(brightness*10);
        //        SYS_PRINT("brightness %d \n", brightness);
        //        SYS_PRINT("speaker %d \n", setting_Get(eSpeakerVolumeSettingId));
            }
        }
        else
        {
            PWM_LCDBacklight_SetDutyCycle(100);
        }
    }
    
}


/** @brief Query any error happen with Illumination sensor 
 *  @param [in]  None   
 *  @param [out] None
 *  @return None
 *  @retval true Illumination sensor has error
 *  @retval false Illumination sensor is OK
 */
bool PWM_LCDBacklight_IsSensorFailed() {
    if (s_IlluSensorError == eDeviceNoError) {
        return false;       //sensor OK
    }
    else {
        return true;        //sensor ERROR
    }
}

/** @brief Enable Back light
 *  @param [in]  None   
 *  @param [out] None
 *  @return None
 */
void PWM_LCDBacklight_Enable() 
{
    s_IsBackLightEnabled = true;
}

/* *****************************************************************************
 End of File
 */
