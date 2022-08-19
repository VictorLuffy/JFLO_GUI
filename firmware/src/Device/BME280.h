/* ************************************************************************** */
/** @file [BME280.h]
 *  @brief {support interface to configure and obtain data from BME280 sensor. 
 * the BME280 integrated pressure sensor, temperature sensor and humidity sensor.
 * Communicate with BME280 via I2C2, which share resource with some other sensors}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */




#ifndef _BME280_H    /* Guard against multiple inclusion */
#define _BME280_H


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

    /** @brief Function to initialize BME280, includes checking product ID, get 
     * Calibration parameters and configure the BME280 to work in expected mode 
     * This function should be called 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void BME280_Initialize();

    /** @brief Read all sensor value, including temperature (c degree), humidity (%) 
     * and pressure (hPa), from BME280 sensor via I2C, then convert them to physical 
     * data and write back to external memory
     *  @param [in]  None   
     *  @param [out]    float *temperature: external pointer to write temperature
     *                  float *pressure: external pointer to write pressure
     *                  float *humidity: external pointer to write humidity
     *  @return None
     *  @retval true reading values OK
     *  @retval false can not read sensor values
     */
    bool BME280_ReadAllValues(float *temperature, float *pressure, float *humidity);

    /** @brief Get last temperature (in C degree) value obtained from BME280. 
     * This function does not generate I2C communication to update data 
     *  @param [in]  None   
     *  @param [out] None
     *  @return float latest temperature value
     */
    float BME280_GetLastsTemperature();

    /** @brief Get last humidity (in %) value obtained from BME280. 
     * This function does not generate I2C communication to update data 
     *  @param [in]  None   
     *  @param [out] None
     *  @return float latest humidity value
     */
    float BME280_GetLastsHumidity();

    /** @brief Get last pressure value (in hPa)obtained from BME280. 
     * This function does not generate I2C communication to update data 
     *  @param [in]  None   
     *  @param [out] None
     *  @return float latest pressure value
     */
    float BME280_GetLastsPressure();

    /** @brief Query any error happen with BME280 
     *  @param [in]  None   
     *  @param [out] None
     *  @return None
     *  @retval true BME280 sensor has error
     *  @retval false BME280 sensor is OK
     */
    bool BME280_IsSensorFailed();

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _BME280_H */

/* *****************************************************************************
 End of File
 */
