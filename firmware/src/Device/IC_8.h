/* ************************************************************************** */
/** @file [IC_8.h]
 *  @brief { This file contains all methods to calculate frequency from PIN M10.
 * It also support 2 measurement unit: Hz and RPM.
 * From Harmony,IC8 is configured as 16 pulse per capture, no interrupt
 * Functions: IC8_GetFrequencyHz() or IC8_GetFrequencyRPM() should called periodically to
 * read value from IC8 and convert it to frequency. Functions IC8_GetLastFrequencyHz()
 * and IC8_GetLastFrequencyRPM() can be used to get latest frequency value without
 * calculation }
 *  @author {nguyen truong, bui phuoc}
 */
/* ************************************************************************** */



#ifndef _IC_8_H    /* Guard against multiple inclusion */
#define _IC_8_H


/* This section lists the other files that are included in this file.
 */
#include <float.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /** @brief Function to initialize IC8, used to calculate frequency from PIN FGOUT 
     * of DRV8308 motor driver. 
     * This function should be called 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void IC8_Initialize();

    /** @brief Function to start IC8 operation
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void IC8_Start();

    /** @brief Function to stop IC8 from operation
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void IC8_Stop();

    /** @brief Calculate frequency from the PIN FGOUT of DRV8308 (in Hz) by reading
     * the IC8 FIFO value and convert to Hz
     *  @param [in]  None   
     *  @param [out]  float* frequency      external memory pointer to store frequency
     *  @return None
     *  @retval true        new frequency data is up to dated
     *  @retval false       no new value is obtained
     */
    bool IC8_GetFrequencyHz(float* frequency);

    /** @brief Calculate frequency from the PIN FGOUT of DRV8308 (in RPM) by reading
     * the IC8 FIFO value and convert to RPM
     *  @param [in]  None   
     *  @param [out]  float* frequency      external memory pointer to store frequency
     *  @return None
     *  @retval true        new frequency data is up to dated
     *  @retval false       no new value is obtained
     */
    bool IC8_GetFrequencyRPM(float* frequency);

    /** @brief Get latest frequency in Hz, not perform any calculation
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None    float       latest frequency value in Hz
     */
    float IC8_GetLastFrequencyHz();

    /** @brief Get latest frequency in RPM, not perform any calculation
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None    float       latest frequency value in Hz
     */
    float IC8_GetLastFrequencyRPM();


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _IC_8_H */

/* *****************************************************************************
 End of File
 */
