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

#ifndef _ADXL345_H    /* Guard against multiple inclusion */
#define _ADXL345_H



/* This section lists the other files that are included in this file.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>



/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    bool ADXL345_Initialize();
    
    bool ADXL345_ReadAccelerometer();
    
    
    /** @brief Query any error happen with ADXL345 
    *  @param [in]  None   
    *  @param [out] None
    *  @return None
    *  @retval true ADXL345 sensor has error
    *  @retval false ADXL345 sensor is OK
    */
    bool ADXL345_IsSensorFailed();

    /** @brief Get Accelerometer
    *  @param [in]  None   
    *  @param [out] float* xAngle rotation around X-axis
    *  @param [out] float* yAngle rotation around Y-axis
    *  @return None
    *  @retval true ADXL345 sensor has error
    *  @retval false ADXL345 sensor is OK
    */
    bool ADXL345_GetAccelerometer( float* xAngle, float* yAngle);
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _ADXL345_H */

/* *****************************************************************************
 End of File
 */
