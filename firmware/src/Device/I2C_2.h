/* ************************************************************************** */
/** @file [I2C_2.h]
 *  @brief {control read/write data via I2C2, manage share resource between multiple 
 * peripherals, and notify when complete a transaction}
 *  @author {bui phuoc, nguyen truong}
 */
/* ************************************************************************** */




#ifndef _I2C_2_H    /* Guard against multiple inclusion */
#define _I2C_2_H


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


    /** @brief Function to initialize I2C2, used to config main/cradle charger as well as 
     * read data main/cradle smart batterry, including open I2C port, setting callback function, 
     * initializing memories before operation
     * This function should be called 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void I2C2_Init();


    /** @brief write a packet data via I2C2, then wait for it done
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
    bool I2C2_Write(uint16_t address,
            void *writeBuffer,
            size_t size,
            uint32_t maxWait);


    bool I2C2_WriteThenRead(uint16_t address,void *writeBuffer, size_t writeSize,
                              void *readBuffer, size_t readSize, uint32_t maxWait);
    
    /** @brief read data via I2C2 and wait for it done
     *  @param [in]  uint16_t address: I2C Address need to read data  
     *              size_t size: size of data expect to read 
     *              uint32_t maxWait: maximum time (in ms) wait for read done. If over
     * time, return error
     *  @param [out]  void *readBuffer: pointer to store buffer
     *  @return None
     *  @retval true read data success
     *  @retval false read data failed
     */
    bool I2C2_Read(uint16_t address,
            void *readBuffer,
            size_t size,
            uint32_t maxWait);



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _I2C_2_H */

/* *****************************************************************************
 End of File
 */
