/* ************************************************************************** */
/** @file [USB_Power.h]
 *  @brief {This file support methods to control power supply for USB port, as 
 * well as detect USB over current and query for Over current status}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */




#ifndef _USB_H    /* Guard against multiple inclusion */
#define _USB_H


/* This section lists the other files that are included in this file.
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "peripheral/ports/plib_ports.h"



/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief Function to initialize USB power by just enable power supply for USB port
     * This function should be called 1 times when start the application
     *  @param [in] None
     *  @param [out] None
     *  @return None
     */
    void USB_Power_Initialize();

    /** @brief Function to enable or disable power supply to USB port.
     * normally, power are supply to USB port, except in case of over current
     *  @param [in]     uint8_t port    no used
     *                  bool enable     - true = enable / - false = disable
     *  @param [out] None
     *  @return None
     */
    void USB_Power_Enable(uint8_t port, bool enable);

    /** @brief This function return status of USB power, whether it is Over current
     * or not
     *  @param [in]  None
     *  @param [out] None
     *  @return     None
     *  @retval true    over current is detected
     *  @retval false   over current did not happen
     */
    bool USB_Power_IsOverCurrent(uint8_t port);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _USB_H */

/* *****************************************************************************
 End of File
 */
