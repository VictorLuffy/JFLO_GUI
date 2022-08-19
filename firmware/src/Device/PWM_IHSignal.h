/* ************************************************************************** */
/** @file [PWM_IHSignal.h]
 *  @brief {Control PWM signal for IH_signal channel of IH circuit, including PWM
 * PWM frequency = 40 Khz fixed, duty cycle = 50% fixed, phase offset range from 0
 * degrees to 180 degrees.
 * All settings and configurations on this file relate tightly with the MPLAB HARMONY configuration.
 * Change the configuration of the HARMONY may lead these function failed}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */

#ifndef _PWMS_IH_SIGNAL_H    /* Guard against multiple inclusion */
#define _PWMS_IH_SIGNAL_H



/* This section lists the other files that are included in this file.
 */

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    //function to initialize PWM IH_Signal channels
    void PWM_IHSignal_init();

    //function to start output signal to IH_signal PIN
    void PWM_IHSignal_start();

    //function to stop output signal to IH_signal PIN
    void PWM_IHSignal_stop();
    
    //function to set phase different 
    void PWM_IHSignal_setPhaseDiff(uint16_t phaseDiffCount);
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _PWMS_IH_SIGNAL_H */

/* *****************************************************************************
 End of File
 */
