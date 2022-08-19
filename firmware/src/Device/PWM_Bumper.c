///* ************************************************************************** */
///** @file [PWM_Bumper.c]
// *  @brief {Generate PWM to control Water Bumper. 
// * Water Bumper require PWM has frequency 6Hz to 400Hz, duty cycle keep 50%
// * The Bumper PWM channel use OC8 combine with Timer7 to generate PWM
// * All settings and configurations on this file relate tightly with the MPLAB HARMONY configuration.
// * Change the configuration of the HARMONY may lead these function failed}
// *  @author {bui phuoc}
// */
///* ************************************************************************** */
//
//
///* This section lists the other files that are included in this file.
// */
//
//#include <stdint.h>
//#include <stdbool.h>
//#include <stddef.h>
//#include <stdlib.h>
//#include "system_config.h"
//#include "system_definitions.h"
//#include "system/debug/sys_debug.h"
//#include "PWM_Bumper.h"
//
//
//
///** @brief Clock source for TIMER 7, the timer base of PWM BUMPER 
// Clock source frequency is calculated by input source SYS_CLK_BUS_PERIPHERAL_3 = 100Mhz,
// by pass the PRE-SCALE = TMR_PRESCALE_VALUE_256*/
//#define PWM_BUMPER_TIMER_CLK_SOURCE_HZ      ((float)390625.0)    //390.625 KHz 
//
///** @brief Initial value of TIMER 7, set to 65535, combine with pre-scale value (256) 
// * to get minimum frequency */
//#define PWM_BUMPER_TIMER_INIT_VALUE         (65535)
//
///** @brief OC PWM index assigned for PWM BUMPER channel, in this case, index 4 */
//#define PWM_BUMPER_INDEX                    (4) 
//
///** @brief Maximum frequency accept for the PWM BUMPER chanel on this application */
//#define PWM_BUMPER_MAX_FREQ_HZ              ((float)800.0)      
//
///** @brief Minimum frequency accept for the PWM BUMPER chanel on this application */
//#define PWM_BUMPER_MIN_FREQ_HZ              ((float)6.0)       
//
//
///** @brief Handle PWM for BUMPER control signals */
//static DRV_HANDLE s_PWM_BumperHandle = DRV_HANDLE_INVALID;
//
///** @brief Frequency apply for the PWM BUMPER*/
//static uint16_t s_timerInCount = PWM_BUMPER_TIMER_INIT_VALUE; // value to generate 6 Hz PWM 
//
///** @brief Flag indicate PWM bumper is running*/
//static bool s_PWM_Bumper_Running = false;
//
//
//
///** @brief Function to initialize PWM_BUMPER to control Water Bumper power, 
// * includes setting the PWM_BUMPER duty cycle 50%, frequency = 6Hz but not start yet
// *  @param [in]  None   
// *  @param [out]  None
// *  @return None
// */
//void PWM_Bumper_Initialize() {
//    //initialize PWM channels for Humidity signals
//    if (s_PWM_BumperHandle == DRV_HANDLE_INVALID) {
//        s_PWM_BumperHandle = DRV_OC_Open(PWM_BUMPER_INDEX, DRV_IO_INTENT_EXCLUSIVE);
//    }
//
//    //set frequency 6 Hz, but do not start
//    DRV_TMR5_PeriodValueSet(PWM_BUMPER_TIMER_INIT_VALUE);
//
//    //set 50% duty cycle
//    DRV_OC_PulseWidthSet(s_PWM_BumperHandle, PWM_BUMPER_TIMER_INIT_VALUE / 2);
//    
//    //initialize variables
//    s_PWM_Bumper_Running = false;
//}
//
///** @brief Function to start output PWM to PWM_BUMPER PIN.
// * Before calling this function, make sure the function PWM_Bumper_Initialize() had been called
// *  @param [in]  None   
// *  @param [out]  None
// *  @return None
// */
//void PWM_Bumper_Start() {
//    //start timer 0
//    DRV_TMR5_Start();
//    //start OC PWM
//    DRV_OC_Start(s_PWM_BumperHandle, DRV_IO_INTENT_EXCLUSIVE);
//    //set running flag
//    s_PWM_Bumper_Running = true;
//}
//
///** @brief Function to set frequency of the PWM BUMPER while keeping the
// * duty cycle 50%. Frequency can be from 6 Hz to 400 Hz
// * Before calling this function, make sure the function PWM_Bumper_Initialize() had been called
// *  @param [in]  float freInHz: expected frequency in Hz
// *  @param [out]  None
// *  @return None
// */
//void PWM_Bumper_SetFrequency(float freInHz) {
//    //check input value in range
//    float fre = freInHz;
//    if (fre >= PWM_BUMPER_MAX_FREQ_HZ) {
//        fre = PWM_BUMPER_MAX_FREQ_HZ;
//    } else if (fre < PWM_BUMPER_MIN_FREQ_HZ) {
//        //stop PWM,then exit
//        PWM_Bumper_Stop();
//        return;
//    }
//    
//    //convert frequency to timer count
//    float count = PWM_BUMPER_TIMER_CLK_SOURCE_HZ / fre;
//
//    //update timer count
//    s_timerInCount = (uint16_t) count;
//
//    //update timer period
//    DRV_TMR5_PeriodValueSet(s_timerInCount);
//
//    if(fre > 30)
//    //update OC value to keep 50% duty cycle
//        DRV_OC_PulseWidthSet(s_PWM_BumperHandle, s_timerInCount / 2);
//    else
//    //update OC value to keep 50% duty cycle
//        DRV_OC_PulseWidthSet(s_PWM_BumperHandle, s_timerInCount * 95 / 100);
//    
//    //start PWM if already stopped 
//    if (s_PWM_Bumper_Running == false) {
//        PWM_Bumper_Start();
//    }
//}
//
///** @brief Function to stop output PWM to PWM BUMPER PIN.
// * Before calling this function, make sure the function PWM_Bumper_Initialize() had been called
// *  @param [in]  None   
// *  @param [out]  None
// *  @return None
// */
//void PWM_Bumper_Stop() {
//    //stop timer base
//    DRV_TMR5_Stop();
//    //stop OC module
//    DRV_OC_Stop(s_PWM_BumperHandle);
//    //clear running flag
//    s_PWM_Bumper_Running = false;
//}
//
//
///* *****************************************************************************
// End of File
// */
