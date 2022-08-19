/* ************************************************************************** */
/** @file [HumidityPower.h]
 *  @brief {This file contain source code necessary for Power calculation support
 * for Humidity controller, including interface to measure the current power apply
 * to IH coil as well as the target power (the power need to maintain both Temperature
 * and Humidity). Some formula in this file relate tightly to information from the
 * Requirement specification document }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */


#ifndef _POWER_CONSUMPTION_H    /* Guard against multiple inclusion */
#define _POWER_CONSUMPTION_H


/* This section lists the other files that are included in this file.
 */
#include <stdbool.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief Calibrate current sensor by calculating the offset voltage
    *  @param [in]     None
    *  @param [out]    None
    *  @retval None
    */
    void HumidityPower_CalibrateCurrentSensorHandle();

    /** @brief Measure current power supply for IH coil. The power is obtained by getting
     * the voltage supply, then multiply with total current pass though the IH. The power
     * is used as input measured of Humidity PID controller
     *  @param [in]     None
     *  @param [out]    float* power        pointer to external memory to store data
     *  @return None
     *  @retval true    if the power is calculated OK
     *  @retval false   if some error happen while calculating the power
     */
    bool HumidityPower_Measure(float* power);


    /** @brief Obtain target power used for Humidity PID controller
     *  @param [in]     float targetAbsHum  target absolute humidity
     *                  float envTemp       environment temperature
     *                  float envRH         environment relative humidity
     *                  float flow          total flow pass through IH
     *                  float outTemp       temperature measure at chamber outlet
     *  @param [out]    None
     *  @return double  target power
     */
    double HumidityPower_Target(float targetAbsHum, float envTemp,
            float envRH, float flow, float outTemp);

    
    float HumidityPower_GetVoltageCurrentSensor1();

    float HumidityPower_GetVoltageCurrentSensor2();
    double HumidityPower_EnvAbsHumidity(float envTemp, float envRH);

    bool HumiditiPower_GetevtR(float *evtR);
    float HumidityPower_GetVoltageSupply();
    float HumidityPower_ConvertVoltageToCurrent_2(float volt, float offset, float sensitive);
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _POWER_CONSUMPTION_H */

/* *****************************************************************************
 End of File
 */
