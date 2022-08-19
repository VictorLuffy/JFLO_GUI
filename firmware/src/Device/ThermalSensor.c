///* ************************************************************************** */
///** Descriptive File Name
//
//  @Company
//    Company Name
//
//  @File Name
//    filename.c
//
//  @Summary
//    Brief description of the file.
//
//  @Description
//    Describe the purpose of this file.
// */
///* ************************************************************************** */
//
//
///* This section lists the other files that are included in this file.
// */
//#include <math.h>
//#include "ADC.h"
//#include "ThermalSensor.h"
//
//#define THERMAL_SENSOR_OUTLET_TEMPERATURE         ADC_TEMPERATURE_SENSOR_1 // Chamber out
//#define THERMAL_SENSOR_OUTLET_TEMPERATURE_2       ADC_TEMPERATURE_SENSOR_2 // Breath Ciruit out 
//#define THERMAL_SENSOR_IH_PROTECT                 ADC_TEMPERATURE_SENSOR_3 // core 
//
////local functions
//static float calcTemp(float mv);
//
//
//bool ThermalSensor_GetOutletTemp(float* temperature) {
//    float tempSensor;
//    if (ADC_GetVoltage(THERMAL_SENSOR_OUTLET_TEMPERATURE, &tempSensor) == false) 
//    {
//        //error
//        return false;
//    }
//    
//    //convert from voltage to temperature
//    *temperature = calcTemp(tempSensor);
//    
//    return true;
//}
//
//bool ThermalSensor_GetBreathCiruitOutletTemp(float* temperature) {
//    float tempSensor;
//    if (ADC_GetVoltage(THERMAL_SENSOR_OUTLET_TEMPERATURE_2, &tempSensor) == false) 
//    {
//        //error
//        return false;
//    }
//    
//    //convert from voltage to temperature
//    *temperature = calcTemp(tempSensor);
//    
//    return true;
//}
//
//bool ThermalSensor_GetCoreTemp(float* temperature)
//{
//    float tempSensor;
//    if (ADC_GetVoltage(THERMAL_SENSOR_IH_PROTECT, &tempSensor) == false) 
//    {
//        //error
//        return false;
//    }
//    
//    //convert from voltage to temperature
//    *temperature = calcTemp(tempSensor);
//    
//    return true;
//}
//
//#define R1_v          41.2            // Gi?tr? ?i?n tr? R1_v
//#define R25_v        49.77          //  ?i?n tr? t?i 25 ?
//#define B_v          3944              // B value
//#define ABS_ZERO    273.15  // Nhi?t ?? K t?i 0 ?
//#define T25C        298.15          // Nhi?t ?? K t?i 025 ?
//#define Vdd         ((float)3.3)
//
//float calcTemp(float mv) {
//   float Rt;
//   float Tk, Tc;
//   Rt = R1_v * mv / (Vdd - mv); // ADC 0 - 3.3 V normalize
//   Tk = 1.0 / (log(Rt / R25_v) / B_v + 1.0 / T25C);
//   Tc = Tk - ABS_ZERO;
//   return (Tc);
//}
//
//
///* *****************************************************************************
// End of File
// */
