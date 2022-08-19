/* ************************************************************************** */
/** @file [PWM_IHCom.h]
 *  @brief {Control PWM signal for IH_com channel of IH circuit, including PWM
 * PWM frequency = 40 Khz fixed, duty cycle = 50% fixed, phase offset 0 degree fixed.
 * All settings and configurations on this file relate tightly with the MPLAB HARMONY configuration.
 * Change the configuration of the HARMONY may lead these function failed}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */

#ifndef _PWM_IH_COMMON_H    /* Guard against multiple inclusion */
#define _PWM_IH_COMMON_H


/* This section lists the other files that are included in this file.
 */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    //function to initialize IH Com PWM channels
    void PWM_IHCom_init();

    //function to start generating output signal to IH_com specified PIN
    void PWM_IHCom_start();
    
    //function to stop generating PWM on specified PIN
    void PWM_IHCom_stop();
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _PWM_IH_COMMON_H */

/* *****************************************************************************
 End of File
 */
