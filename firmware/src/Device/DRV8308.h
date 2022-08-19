/* ************************************************************************** */
/** @file [DRV8308.h]
 *  @brief {This file contains method to control DRV8308 motor driver, including
 * configure driver, start driver, stop driver, control speed of driver, detect 
 * FAULT and motor status ...}
 *  @author {truong nguyen + bui phuoc}
 */
/* ************************************************************************** */


#ifndef _DRV8308_H    /* Guard against multiple inclusion */
#define _DRV8308_H


/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>
#include "system_config.h"
#include "system_definitions.h"



/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief Initialize DRV8308 motor driver, include initializing SPI3 for driver 
     * communication, initializing IC8 for motor speed measurement
     * This function should be called 1 times at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void DRV8308_Initialize(void);

    /** @brief Configure DRV8308 motor driver before operating by writing to internal
     * configuration register inside DRV8308 via SPI3. For detail information, refer
     * the driver data-sheet
     * This function should be called 1 times at start up, after the funcion DRV8308_Initialize()
     * has been called and the RTOS has started
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    bool DRV8308_Configure(void);

    /** @brief Start motor spinning by turning on the MOTOR_ENABLE pin on DRV8308 as 
     * well as turn off the BRAKE pin to make the motor spinning. The IC8 for speed 
     * measurement should be started also
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void DRV8308_Start(void);

    /** @brief Stop motor from spinning by turn on the BRAKE pin and turn off the ENABLE
     * pin. The IC8 for speed measurement also be stopped
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void DRV8308_Stop(void);

    /** @brief Set motor power by writing to the SPEED register inside the DRV8308. 
     * The input power indicate in percentage. 0% mean stop, 100% mean maximum speed.
     * The input power will be convert to convenient value for SPEED register on DRV8308,
     * which accept value from 0 to 4095
     *  @param [in]  float power        percentage power, 0 to 100%   
     *  @param [out]  None
     *  @return None
     */
    void DRV8308_SetPower(float power);

    /** @brief Query whether DRV8308 has error or not
     *  @param [in]  None
     *  @param [out]  None
     *  @return None
     *  @retval true        if driver error
     *  @retval false       if driver has been operating normally
     */
    bool DRV8308_IsError(void);

    /** @brief Calculate motor speed by reading IC8 value
     *  @param [in]  None
     *  @param [out]  None
     *  @return float       speed of motor in Rpm
     */
    float DRV8308_MonitorSpeed(void);
    
    float DRV8308_GetLatestSpeed(void);
    
    bool DRV8308_CheckMotorError(void);
    
    float DRV8308_GetCurrentPower(void);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _DRV8308_H */

/* *****************************************************************************
 End of File
 */
