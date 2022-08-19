/* ************************************************************************** */
/** @file [HumidityController.h]
 *  @brief {A PID controller for Humidity control. This controller take input from 
 * power supply for IH Heater, and control the water bumper to obtain the humidity
 * setting. Humidity setting is targeted always 100% (or TBD) }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */



#ifndef _HUMIDITY_CONTROLLER_H    /* Guard against multiple inclusion */
#define _HUMIDITY_CONTROLLER_H


/* This section lists the other files that are included in this file.
 */
#include <stdbool.h>


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief Function initialize PID controller for Humidity Control and set it up before
     * running
     *  @param [in]  None
     *  @param [out]  None
     *  @return  None
     */
    void HumidityController_Initialize();

    /** @brief Function perform PID controller calculation with measurement value is 
     * obtained from IH power, and the target is Humidity setting
     *  @param [in]     float measured     measurement value
     *                  float target    target value
     *  @param [out]  None
     *  @return float       result after PID calculation, in this case, the frequency
     * to set to Water bumper
     */
    float HumidityController_Operate(float measured, float target, float currentTemp, float targetTemp, float envTemp);

    /** @brief Function to anable / disable Humidity's PID controller
     *  @param [in]  bool enable     enable = 1/ disable = 0
     *  @param [out]  None
     *  @return None
     */
    void HumidityController_Enable(bool enable);

    /** @brief Function to set target of Humidity's PID controller. In this case, target 
     * is the Humidity setting value
     *  @param [in]  float target     target to apply to Humidity's PID controller
     *  @param [out]  None
     *  @return None
     */
    void HumidityController_SetTarget(float target);

    float  GetIntegralComponent();
    float  GetDerivativeComponent();
    float GetProportionalComponent();
    //int I_sigma_delta_1(float currentPower, float targetPower );
    void HumidityControler_SetPumpLimit(float lowerLimit, float upperLimit);
    void HumidityControler_Set_Init(float pumpFreq, float currentPower, float targetPower);
    void HumidityController_SetMaxintegral(float maxValue);
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _HUMIDITY_CONTROLLER_H */

/* *****************************************************************************
 End of File
 */
