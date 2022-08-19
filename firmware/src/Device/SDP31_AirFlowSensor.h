/* ************************************************************************** */
/** @file [SDP31_AirFlowSensor.c]
 *  @brief {support interface to communicate with SDP31 Air Flow Sensor such as: 
 * configure, get flow value, get scale factor, report error if occur
 * during communication}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */




#ifndef _DSP31_AIR_FLOW_SENSOR_H    /* Guard against multiple inclusion */
#define _DSP31_AIR_FLOW_SENSOR_H


/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /** @brief Initialize SDP31 sensor measure for Air source by configure its operation 
     * mode and read all scale factor prepare before operation
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void AirFlowSensor_Initialize();


    /** @brief reset Air flow sensor by putting reset pin to LOW in 2 ms
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void AirFlowSensor_Reset();


    /** @brief get current flow from the SDP31 Air Flow Sensor. Since the sensor is
     * configured as "Average til Read",the read value is the average value of all samples
     * from the last read.
     * The raw value, after reading via I2C, is divided by Scale factor. 
     * This function should be called after the sensor is already started operation and
     * the Scale factor is obtained.
     *  @param [in]  None
     *  @param [out]  float* flowVal    pointer to store flow value after divide by scale factor (LPM)
     *  @return None
     *  @retval true    getting flow value OK
     *  @retval false   getting flow value Failed
     */
    bool AirFlowSensor_GetFlow(float* flowVal);


    /** @brief Get last flow value. This function return previous value obtain from
     * Air flow sensor without I2C communication .
     *  @param [in]  None
     *  @param [out]  None
     *  @return float flow in (LPM)
     */
    float AirFlowSensor_GetLastFlow();

    /** @brief Query any error happen with Air Flow Sensor
     *  @param [in]  None   
     *  @param [out] None
     *  @retval true Air Flow sensor has error
     *  @retval false Air Flow sensor is OK
     */
    bool AirFlowSensor_IsSensorFailed();

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _DSP31_AIR_FLOW_SENSOR_H */

/* *****************************************************************************
 End of File
 */
