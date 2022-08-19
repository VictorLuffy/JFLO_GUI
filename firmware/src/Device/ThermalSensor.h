/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _THERMAL_SENSOR_H    /* Guard against multiple inclusion */
#define _THERMAL_SENSOR_H


/* This section lists the other files that are included in this file.
 */



/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

bool ThermalSensor_GetOutletTemp(float* temperature);

bool ThermalSensor_GetCoreTemp(float* temperature);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _THERMAL_SENSOR_H */

/* *****************************************************************************
 End of File
 */
