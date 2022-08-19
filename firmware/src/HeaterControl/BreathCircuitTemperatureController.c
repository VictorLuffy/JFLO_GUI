/* ************************************************************************** */
/** @file [BreathCircuitTemperatureController.c]
 *  @brief {A PID controller for Temperature control. This controller take input from 
 * 2 thermal sensor places in the outlet of the machine, and control IH Temperature 
 * to obtain the temperature setting. Since the temperature setting indicate the 
 * temperature at the end of the circuit, a mount of thermal lost between 2 terminals
 * of the circuit should be take into account }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */

#include "system_config.h"
#include "system_definitions.h"

#include "BreathCircuitTemperatureController.h"
#include "PID.h"
#include "DigitalPotentiometer.h"
#include <math.h>

/** @brief The Overall heat transfer coefficient */
#define U_HEAT_TRANSFER_COEFF   (1600)

/** @brief The Len of canula */
#define LEN   (0.03)

/** @brief The Perimeter of the cannula */
#define PHI   (3.14*0.0012)

/** @brief The Specific heat of gas */
#define ACP   (1005)

/** @brief The Gas density */
#define AP   (1.1278)


/** @brief The Proportional gain of Breathing Circuit Temperature's PID controller */
const float BC_TEMPERATURE_CONTROLLER_PID_KP = 15;

/** @brief The Integral gain of Breathing Circuit Temperature's PID controller */
const float BC_TEMPERATURE_CONTROLLER_PID_KI = 4;

/** @brief The Derivative gain of Breathing Circuit Temperature's PID controller */
const float BC_TEMPERATURE_CONTROLLER_PID_KD = 0;

/** @brief Breathing Circuit Temperature's PID controller instance */
static PIDController s_BCTemperaturePidController;

/** @brief current measurement flow, use as input of Breathing Circuit Temperature's PID controller */
static float s_MeasuredTemperature = 0;


/** @brief Function return current measurement to supply input for Breathing Circuit Temperature's PID 
 * controller. This function will automatically called when PID controller run
 *  @param [in]  None   
 *  @param [out]  None
 *  @return float       current temperature measurement
 */
static inline float BreathCircuitTemperatureController_PIDDataIn()
{
    return s_MeasuredTemperature;
}

/** @brief Function perform action after PID controller has been calculated. In
 * this case, it will adjust Power on IH coil. 
 * This function will be called automatically when PID controller has finished 
 * calculation 
 *  @param [in]  float output   the output result after PID controller has calculated   
 *  @param [out]  None
 *  @return  None
 */
static inline void BreathCircuitTemperatureController_PIDDataOut(float output)
{
    float breathCircuitCtlValue = 127 - output;//increase "output" ==> decrease the voltage
//    SYS_PRINT("Br-Circuit %.1f\n", output);
    DigitalPotentiometer_SetLevel((uint8_t)breathCircuitCtlValue);
}

/** @brief Function initialize PID controller for Breathing Circuit Temperature Control and set it up before
 * running
 *  @param [in]  None
 *  @param [out]  None
 *  @return  None
 */
void BreathCircuitTemperatureController_Initialize() {
    PID_CreateController(&s_BCTemperaturePidController, 
            BC_TEMPERATURE_CONTROLLER_PID_KP, 
            BC_TEMPERATURE_CONTROLLER_PID_KI,
            BC_TEMPERATURE_CONTROLLER_PID_KD, 
            BreathCircuitTemperatureController_PIDDataIn, 
            BreathCircuitTemperatureController_PIDDataOut);
    PID_SetOutputBounds(&s_BCTemperaturePidController, 0.0, 60.0);
    PID_SetInputBounds(&s_BCTemperaturePidController, 0.0, 100.0);//100
    PID_SetMaxIntegralCumulation(&s_BCTemperaturePidController, 18.0);//20
    PID_SetEnabled(&s_BCTemperaturePidController, 0);
}

/** @brief Function perform PID controller calculation with measurement value obtained
 * from 2 thermal sensors
 *  @param [in]  float measured     measurement value, in this case: average of 2 thermal sensors
 *  @param [out]  None
 *  @return float       result after PID calculation
 */
float BreathCircuitTemperatureController_Operate(float measured, float target) {
    s_BCTemperaturePidController.target = target;
    s_MeasuredTemperature = measured;
    PID_Calculate(&s_BCTemperaturePidController);
    return s_BCTemperaturePidController.output;
}

/** @brief Function to anable / disable Breathing Circuit Temperature's PID controller
 *  @param [in]  bool enable     enable = 1/ disable = 0
 *  @param [out]  None
 *  @return None
 */
void BreathCircuitTemperatureController_Enable(bool enable) {
    if (enable) {
        PID_SetEnabled(&s_BCTemperaturePidController, 1);
    }
    else {
        PID_SetEnabled(&s_BCTemperaturePidController, 0);
    }
}

/** @brief Function to set target of Breathing Circuit Temperature's PID controller. In this case, target 
 * is the Temperature setting value
 *  @param [in]  float target     target to apply to Temperature's PID controller
 *  @param [out]  None
 *  @return None
 */
void BreathCircuitTemperatureController_SetTarget(float target) {
    s_BCTemperaturePidController.target = target;
}


/** @brief Calculate breathing circuit outlet target temperature 
 *  @param [in]     float envTemp   environment temperature
 *                  float setFlow   setting flow rate
 *                  float setTemp   setting temperature
 *  @param [out]    None
 *  @return float  breathing circuit outlet temperature 
 */
//float BreathCircuitTemperatureController_CalculateOutletTartgetTemperature(float setFlow, float setTemp, float envTemp)
//{
//#define Acp     (1006.0)
//#define ARho    (1.16)    
//#define Alpha   (0.01*314)
//#define Len     (0.4)
//    
//    float Kf = 1 / (setFlow * ARho * Acp);
//    float U = 470.0 + 12.3 * setFlow;
//    
//    float outletTargetTemp = (setFlow - envTemp) / (pow(M_E, (-1)* Kf * U * Alpha * Len));
//    return outletTargetTemp;
//}
/* *****************************************************************************
 End of File
 */

float BcGetIntegralComponent()
{
   return PID_GetIntegralComponent(&s_BCTemperaturePidController);
    
}

float BcGetDerivativeComponent()
{
   return PID_GetDerivativeComponent(&s_BCTemperaturePidController);
    
}

float BcGetProportionalComponent()
{
   return PID_GetProportionalComponent(&s_BCTemperaturePidController);
    
}

void BcTemperatureController_SetPwUpperLimit(float upperLimit)
{
    PID_SetOutputBounds(&s_BCTemperaturePidController, 0.0, upperLimit);
}

void BcTemperatureController_SetMaxintegral(float maxValue)
{
  PID_SetMaxIntegralCumulation(&s_BCTemperaturePidController, (maxValue/s_BCTemperaturePidController.i));
}


/** @brief Obtain Breathing Circuit outlet target temperature from setting temperature, ambient temperature
 * and setting flow rate
 *  @param [in]     float ambientTemp   environment temperature
 *  @param [in]     float setTemp     setting temperature
 *  @param [in]     float setFlow     setting flow (Lpm)
 *  @param [out]    None
 *  @return float  Breathing Circuit outlet target temperature
 */
float BreathCircuitTemperatureController_CalculateBcOutletTargetTemperature(float ambientTemp, float setTemp, float setFlow)
{
    float targetTemp = 0;
    float expPara = 0;
    //convert flow rate from Lpm to m3ps
    float setFlowM3ps = setFlow / 60.0 / 1000.0;
    if(setFlowM3ps != 0)
    {
        expPara = exp( (U_HEAT_TRANSFER_COEFF * LEN * PHI)/(setFlowM3ps * ACP * AP) );
    }
    targetTemp = ambientTemp + (setTemp - ambientTemp)*expPara;
    return targetTemp;
}