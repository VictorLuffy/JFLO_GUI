/* 
 * File:   CrcChamber.h
 * Author: Quoc Viet
 *
 * Created on March 9, 2021, 3:46 PM
 */

#ifndef THERMAL_SENSOR_H
#define	THERMAL_SENSOR_H

#include <stdint.h>
#include <stdbool.h>


#ifdef	__cplusplus
extern "C" {
#endif

#define SENSOR_TYPE_1 0  // Evt temp sensor 
#define SENSOR_TYPE_2 1  // Chamber out temp sensor, 
#define SENSOR_TYPE_3 2  // Breathing circuit out temp sensor


    
    
float convertTemperatureToMillivolt(float temp, uint8_t sensorType);



#ifdef	__cplusplus
}
#endif

#endif	/* THERMAL_SENSOR_H */

