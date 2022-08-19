/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


#include "system_definitions.h"
#include "ThermalSensor.h"
#include "math.h"

    
//#define R1_v_1         ((float)41.2)            // R1_v for Chamber out temp sensor, Breathing circuit out temp sensor
//#define R1_v_2         ((float)10.0)            // R1_v for Evt temp sensor    
//
//#define R25_v_1        ((float)49.77)           // R at 25 do C of Chamber out temp sensor, Breathing circuit out temp sensor
//#define R25_v_2        ((float)10.0)            // R at 25 do C of Evt temp sensor     
//
//#define B_v_1          ((float)3944.0)           // B value  of Chamber out temp sensor, Breathing circuit out temp sensor
//#define B_v_2          ((float)3988.0)           //3435.0)          // B value of Evt temp sensor 
//
//#define ABS_ZERO       ((float)273.15)            // Nhi?t ?? K t?i 0 ?
//#define T25C           ((float)298.15)            // Nhi?t ?? K t?i 025 ?
//#define Vdd            ((float)5.0)

#define R1_v_1         ((float)0.51)            //  R1_v for Evt temp sensor  
#define R1_v_2         ((float)41.2)            //  R1_v for Chamber out temp sensor
#define R1_v_3         ((float)41.2)            //  R1_v for Breathing circuit out temp sensor  
    
#define R25_v_1        ((float)10.0)             // R at 25 do C of Evt temp sensor
#define R25_v_2        ((float)47.0)            // R at 25 do C of Chamber temp sensor   
#define R25_v_3        ((float)47.0)            // R at 25 do C of Breathing circuit out temp sensor
    
#define B_v_1          ((float)3492.0)           // B value  of EVT temp sensor
#define B_v_2          ((float)4050.0)           // B value of Chamber temp sensor 
#define B_v_3          ((float)4050.0)           // B value of Breathing circuit out temp sensor
    
#define ABS_ZERO       ((float)273.15)            // Nhi?t ?? K t?i 0 ?
#define T25C           ((float)298.15)            // Nhi?t ?? K t?i 025 ?
#define Vdd            ((float)5.0)


float convertTemperatureToMillivolt(float temperature, uint8_t sensorType) 
{
    float Rt;
    float Tk, Tc;
    float mv;

    Tc = temperature;
    Tk = Tc + ABS_ZERO;

    if(sensorType == SENSOR_TYPE_1)
    {
        Rt = R25_v_1 * exp( B_v_1 * (1.0 / Tk - 1.0 / T25C));
        mv = (Rt * Vdd)/(Rt + R1_v_1);
    }
    else if(sensorType == SENSOR_TYPE_2)
    {
        Rt = R25_v_2 * exp( B_v_2 * (1.0 / Tk - 1.0 / T25C));
        mv = (Rt * Vdd)/(Rt + R1_v_2);
    }
    else if(sensorType == SENSOR_TYPE_3)
    {
        Rt = R25_v_3 * exp( B_v_3 * (1.0 / Tk - 1.0 / T25C));
        mv = (Rt * Vdd)/(Rt + R1_v_3);
    }
    
    return (mv);
}


/* *****************************************************************************
 End of File
 */

