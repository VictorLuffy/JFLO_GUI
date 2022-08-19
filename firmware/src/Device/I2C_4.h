/* ************************************************************************** */
/** @file [I2C_4.h]
 *  @brief {control read/write data via I2C4, manage share resource between multiple 
 * peripherals, and notify when complete a transaction}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */




#ifndef _I2C_4_H    /* Guard against multiple inclusion */
#define _I2C_4_H


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


    /** @brief Function to initialize I2C4, used to read data from Air Flow Sensor 
     * and O2 Flow Sensor, including open I2C port, setting callback function, 
     * initializing memories before operation
     * This function should be called 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void I2C4_Initialize();


    /** @brief write a packet data via I2C4, then wait for it done
     *  @param [in]  uint16_t address: I2C Address need to communicate  
     *              void *writeBuffer: pointer to data packet
     *              size_t size: size of data packet 
     *              uint32_t maxWait: maximum time (in ms) wait for read done. If over
     * time, return error
     *  @param [out]  None
     *  @return None
     *  @retval true write data success
     *  @retval false write data failed
     */
    bool I2C4_Write(uint16_t address,
            void *writeBuffer,
            size_t size,
            uint32_t maxWait);



    /** @brief read data via I2C4 and wait for it done
     *  @param [in]  uint16_t address: I2C Address need to read data  
     *              size_t size: size of data expect to read 
     *              uint32_t maxWait: maximum time (in ms) wait for read done. If over
     * time, return error
     *  @param [out]  void *readBuffer: pointer to store buffer
     *  @return None
     *  @retval true read data success
     *  @retval false read data failed
     */
    bool I2C4_Read(uint16_t address,
            void *readBuffer,
            size_t size,
            uint32_t maxWait);

    void I2C4_ResetComunicate();
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _I2C_4_H */

/* *****************************************************************************
 End of File
 */
