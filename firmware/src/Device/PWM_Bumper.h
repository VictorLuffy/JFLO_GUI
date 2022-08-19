///* ************************************************************************** */
///** @file [PWM_Bumper.h]
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
//#ifndef _PWM_BUMPER_H    /* Guard against multiple inclusion */
//#define _PWM_BUMPER_H
//
///* This section lists the other files that are included in this file.
// */
//
//#include <stdint.h>
//#include <stdbool.h>
//
//
///* Provide C++ Compatibility */
//#ifdef __cplusplus
//extern "C" {
//#endif
//
//
//    /** @brief Function to initialize PWM_BUMPER to control Water Bumper power, 
//     * includes setting the PWM_BUMPER duty cycle 50%, frequency = 6Hz but not start yet
//     *  @param [in]  None   
//     *  @param [out]  None
//     *  @return None
//     */
//    void PWM_Bumper_Initialize();
//
//    /** @brief Function to start output PWM to PWM_BUMPER PIN.
//     * Before calling this function, make sure the function PWM_Bumper_Initialize() had been called
//     *  @param [in]  None   
//     *  @param [out]  None
//     *  @return None
//     */
//    void PWM_Bumper_Start();
//
//    /** @brief Function to set frequency of the PWM BUMPER while keeping the
//     * duty cycle 50%. Frequency can be from 6 Hz to 400 Hz
//     * Before calling this function, make sure the function PWM_Bumper_Initialize() had been called
//     *  @param [in]  float freInHz: expected frequency in Hz
//     *  @param [out]  None
//     *  @return None
//     */
//    void PWM_Bumper_SetFrequency(float freInHz);
//
//    /** @brief Function to stop output PWM to PWM BUMPER PIN.
//     * Before calling this function, make sure the function PWM_Bumper_Initialize() had been called
//     *  @param [in]  None   
//     *  @param [out]  None
//     *  @return None
//     */
//    void PWM_Bumper_Stop();
//
//    /* Provide C++ Compatibility */
//#ifdef __cplusplus
//}
//#endif
//
//#endif /* _PWM_BUMPER_H */
//
///* *****************************************************************************
// End of File
// */
