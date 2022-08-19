/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    This file contain all functions to check CRC for Differential Pressure sensor
 * SDP31-500PA. All functions below only compatible with the indicated sensor

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _CRC_FLOW_SENSOR_H    /* Guard against multiple inclusion */
#define _CRC_FLOW_SENSOR_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    //function to check CRC for input data, 
    //return true if CRC good
    //return false if CRC failed
bool crcFlowSensor_Check(uint8_t *data, uint8_t len, uint8_t crcCheck);

    //function to calcuate CRC for input data, 
    //return the CRC result calculated 
uint8_t crcFlowSensor_Calculate(uint8_t *data, uint8_t len);

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _CRC_FLOW_SENSOR_H */

/* *****************************************************************************
 End of File
 */
