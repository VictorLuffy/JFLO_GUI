/* ************************************************************************** */
/** @file [TemperatureController.c]
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

#include "TemperatureController.h"
#include "PID.h"
#include "PWM_IH.h"
#include <RCFilter.h>
#include "HeaterTask.h"
#include "math.h"

/** @brief The Proportional gain of Temperature's PID controller */
const float TEMPERATURE_CONTROLLER_PID_KP = 8.0;//6.0;//8;

/** @brief The Integral gain of Temperature's PID controller */
const float TEMPERATURE_CONTROLLER_PID_KI = 0.5;//0.25;//0.25;

/** @brief The Derivative gain of Temperature's PID controller */
const float TEMPERATURE_CONTROLLER_PID_KD = 0;//0.1;//0;

/** @brief Temperature's PID controller instance */
static PIDController s_TemperaturePidController;

/** @brief current measurement flow, use as input of Temperature's PID controller */
static float s_MeasuredTemperature = 0;


/** @brief Function return current measurement to supply input for Temperature's PID 
 * controller. This function will automatically called when PID controller run
 *  @param [in]  None   
 *  @param [out]  None
 *  @return float       current temperature measurement
 */
static inline float TemperatureController_PIDDataIn()
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
static inline void TemperatureController_PIDDataOut(float output)
{
    //SYS_PRINT("IH %.1f\n", output);
   // output = 100.0;
    //SYS_PRINT(",%.2f", output);
    float diffLevel = sqrtf(output);
//    SYS_PRINT("set duty to: %.2f\n", 100*(diffLevel/20));

    PWM_IH_SetPhaseDifference(diffLevel);
}

/** @brief Function initialize PID controller for Temperature Control and set it up before
 * running
 *  @param [in]  None
 *  @param [out]  None
 *  @return  None
 */
void TemperatureController_Initialize() {
    PID_CreateController(&s_TemperaturePidController, 
            TEMPERATURE_CONTROLLER_PID_KP, 
            TEMPERATURE_CONTROLLER_PID_KI,
            TEMPERATURE_CONTROLLER_PID_KD, 
            TemperatureController_PIDDataIn, 
            TemperatureController_PIDDataOut);
    PID_SetOutputBounds(&s_TemperaturePidController, 0.0, 400.0);
    PID_SetInputBounds(&s_TemperaturePidController, 0.0, 100.0);
    PID_SetMaxIntegralCumulation(&s_TemperaturePidController, 800.0);// 800*0.5 = 400;
    PID_SetEnabled(&s_TemperaturePidController, 0);
}

/** @brief Function perform PID controller calculation with measurement value obtained
 * from 2 thermal sensors
 *  @param [in]  float measured     measurement value, in this case: average of 2 thermal sensors
 *  @param [out]  None
 *  @return float       result after PID calculation
 */
float TemperatureController_Operate(float measured, float target) {
    static RCflt_t TempCtrl;
    s_TemperaturePidController.target = target;
    s_MeasuredTemperature = measured;
    PID_Calculate(&s_TemperaturePidController);
    s_TemperaturePidController.output = LPF(&TempCtrl, s_TemperaturePidController.output);
    return s_TemperaturePidController.output;
}

/** @brief Function to anable / disable Temperature's PID controller
 *  @param [in]  bool enable     enable = 1/ disable = 0
 *  @param [out]  None
 *  @return None
 */
void TemperatureController_Enable(bool enable) {
    if (enable) {
        PID_SetEnabled(&s_TemperaturePidController, 1);
    }
    else {
        PID_SetEnabled(&s_TemperaturePidController, 0);
    }
}

/** @brief Function to set target of Temperature's PID controller. In this case, target 
 * is the Temperature setting value
 *  @param [in]  float target     target to apply to Temperature's PID controller
 *  @param [out]  None
 *  @return None
 */
void TemperatureController_SetTarget(float target) {
    s_TemperaturePidController.target = target;
}

void TemperatureController_SetPw(float output)
{
 //   SYS_PRINT("IH %.1f\n", output);
    float diffLevel = sqrtf(output);
    //SYS_PRINT("IH %.4f\r\n", diffLevel);
     
//    SYS_PRINT("set duty to: %.2f\n", 100*(diffLevel/20));

    PWM_IH_SetPhaseDifference(diffLevel);
}

void TemperatureController_SetPwUpperLimit(float upperLimit)
{
    PID_SetOutputBounds(&s_TemperaturePidController, 0.0, upperLimit);
//    SYS_PRINT("set power upper limit to: %.2f\n", 100*(sqrtf(upperLimit)/20));
}

void TemperatureController_SetMaxintegral(float maxValue)
{
  PID_SetMaxIntegralCumulation(&s_TemperaturePidController, (maxValue/s_TemperaturePidController.i));
}
/* *****************************************************************************
 End of File
 */

//int Temp_I_sigma_delta_1(float currentTemp, float targetTemp) {
//    static int sigma_delta_wave_prev = 0;
//    int out = 0;
//    static float integrator_main = 0;
//    static float integrator_sub = 0;    
//    float gain_sub = 0.05;
//    float gain_main = 2.5*targetTemp;
//    
//    float Error = targetTemp - currentTemp;
//    
//    integrator_sub +=gain_sub*Error;
//    /*
//    if(integrator_sub >= 50)
//    {
//        integrator_sub = 50;
//    }
//    if(integrator_sub <= -50)
//    {
//        integrator_sub = -50;
//    }
//    */
//    integrator_main += Error + integrator_sub - (float) sigma_delta_wave_prev * gain_main;
//
//    /*
//    if(integrator_main >= 150)
//    {
//        integrator_main = 150;
//    }
//    if(integrator_main <= -150)
//    {
//        integrator_main = -150;
//    }
//     */
//    
//    if (integrator_main > 0.0) {
//        out = 1;
//    } else {
//        out = -1;
//    }
//
//    sigma_delta_wave_prev = out;
//
//    SYS_PRINT("currentTemp  %.2f integrator_sub  %.2f integrator_main  %.2f output %d Error  %.2f \n",currentTemp, integrator_sub, integrator_main, out, Error);
//    return out;
//}
//
//float TempDeltaSigma2D(float din)
//{
//  static float sg1in = 0;
//  static float sg1out = 0;
//  static float sg2in = 0;
//  static float sg2out = 0;
//  static float dsout = 0;
//  const float k = 2.0;
//  const int qtk = 31; // 8bit?data?5bit?????3bit???
//
//  dsout = floor((floor(sg2out) / qtk)) * qtk; // quantizer
//  sg2in = sg2out + sg1out - dsout * k;
//  sg1in = sg1out - dsout + din;
//  sg1out = sg1in;
//  sg2out = sg2in;
//  // Limit: 127 , -128
//  if (sg2out >= 127)
//  {
//    sg2out = 127;
//  }
//  if (sg2out <= -128)
//  {
//    sg2out = -128;
//  }
//  return dsout;
//}