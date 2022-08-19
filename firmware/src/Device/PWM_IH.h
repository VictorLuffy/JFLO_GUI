/* ************************************************************************** */
/** @file [PWM_IH.h]
 *  @brief {Control PWM signal for IH circuit, including turn ON, turn OFF, set 
 * Power drop on IH coil (from 0% to 100% Power).
 * All settings and configurations on this file relate tightly with the MPLAB HARMONY configuration.
 * Change the configuration of the HARMONY may lead these function failed}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */

#ifndef _PWM_IH_H    /* Guard against multiple inclusion */
#define _PWM_IH_H

/* This section lists the other files that are included in this file.
 */

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    //define timer clock period
#define PWM_IH_TIMER_CLK_PERIOD_NS      (10)//(10)    //for supply clock 100Mhz, period = 1/100Mhz
#define PWM_IH_TIMER_BASE_PERIOD_NS     (50000) //for PWM 20Kz, period = 1/20Khz 
#define PWM_IH_TIMER_CYCLE_COUNT        (PWM_IH_TIMER_BASE_PERIOD_NS/PWM_IH_TIMER_CLK_PERIOD_NS)    //count for 1 cycle
#define PWM_IH_TIMER_PHASE_COUNT        (PWM_IH_TIMER_CYCLE_COUNT/2)                 //count for 1 phase (50% duty cycle always)

    //function to initialize all PWM (2 channels) support for IH control
    void PWM_IH_Initialize();

    //function to start PWM generator
    void PWM_IH_Start();

    //function to stop IH PWM genenrator
    void PWM_IH_Stop();

    //function to get status of IH running
    bool PWM_IH_IsOperating();

    //function to set power for IH (1 --> 20) 
    void PWM_IH_SetPhaseDifference(float phaseDifferenceLev);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _PWM_IH_H */

/* *****************************************************************************
 End of File
 */
