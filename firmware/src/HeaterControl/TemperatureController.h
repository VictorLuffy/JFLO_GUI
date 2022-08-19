/* ************************************************************************** */
/** @file [TemperatureController.h]
 *  @brief {A PID controller for Temperature control. This controller take input from 
 * 2 thermal sensor places in the outlet of the machine, and control IH Temperature 
 * to obtain the temperature setting. Since the temperature setting indicate the 
 * temperature at the end of the circuit, a mount of thermal lost between 2 terminals
 * of the circuit should be take into account }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */

#ifndef _TEMPERATURE_CONTROLLER_H    /* Guard against multiple inclusion */
#define _TEMPERATURE_CONTROLLER_H


/* This section lists the other files that are included in this file.
 */
#include <stdbool.h>


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief Function initialize PID controller for Temperature Control and set it up before
     * running
     *  @param [in]  None
     *  @param [out]  None
     *  @return  None
     */
    void TemperatureController_Initialize();

    /** @brief Function perform PID controller calculation with measurement value obtained
     * from 2 thermal sensors
     *  @param [in]  float measured     measurement value, in this case: average of 2 thermal sensors
     *  @param [out]  None
     *  @return float       result after PID calculation
     */
    float TemperatureController_Operate(float measured, float target);

    /** @brief Function to anable / disable Temperature's PID controller
     *  @param [in]  bool enable     enable = 1/ disable = 0
     *  @param [out]  None
     *  @return None
     */
    void TemperatureController_Enable(bool enable);

    /** @brief Function to set target of Temperature's PID controller. In this case, target 
     * is the Temperature setting value
     *  @param [in]  float target     target to apply to Temperature's PID controller
     *  @param [out]  None
     *  @return None
     */
    void TemperatureController_SetTarget(float target);
    
    void TemperatureController_SetPw(float output);

    void TemperatureController_SetPwUpperLimit(float upperLimit);
    
    void TemperatureController_SetMaxintegral(float maxValue);
    
//    int Temp_I_sigma_delta_1(float currentTemp, float targetTemp);
//    float TempDeltaSigma2D(float din);
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _TEMPERATURE_CONTROLLER_H */

/* *****************************************************************************
 End of File
 */
