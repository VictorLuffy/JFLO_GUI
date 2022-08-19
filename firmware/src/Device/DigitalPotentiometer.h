/* ************************************************************************** */
/** @file [DigitalPotentiometer.h]
 *  @brief {Supply APIs to get pressed state of the 
 *  @author {nguyen truong}
 */
/* ************************************************************************** */




#ifndef _DIGITAL_POTENTIOMETER_H    /* Guard against multiple inclusion */
#define _DIGITAL_POTENTIOMETER_H


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

    /** @brief Potentiometer 7-bit Resistor Network Resolution - 127 Resistors (128 Steps)
    *  @param [in]   uint8_t level ranges from 0 to 127 
    *  @param [out]  None
    *  @return None
    *  @retval None
    */
    void DigitalPotentiometer_SetLevel(uint8_t level);
        
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _DIGITAL_POTENTIOMETER_H */

/* *****************************************************************************
 End of File
 */
