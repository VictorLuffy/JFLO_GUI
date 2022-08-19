/* ************************************************************************** */
/** @file [SPI_3.h]
 *  @brief { This file contains all methods to read/ write via SPI3 serial port
 * compatible with RTOS including open port, detect error, report error, and task
 * synchronization }
 *  @author {nguyen truong, bui phuoc}
 */
/* ************************************************************************** */



#ifndef _SPI3_H    /* Guard against multiple inclusion */
#define _SPI3_H



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



    /** @brief Function to initialize SPI3, used to control motor driver DRV8308 
     * The function first open SPI port, then configure some operation setting like 
     * baud-rate, start call back function, end call back function ...Finally initialize
     * memories before operation
     * This function should be called 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void SPI3_Initialize();


    /** @brief Write a packet data via SPI3, then wait for it done
     *  @param [in]  void *writeBuffer: pointer to data packet
     *              size_t size: size of data packet 
     *              uint32_t maxWait: maximum time (in ms) wait for read done. If over
     * time, return error
     *  @param [out]  None
     *  @return None
     *  @retval true write data success
     *  @retval false write data failed
     */
    bool SPI3_Write(void* writeBuffer, size_t size, uint32_t maxWait);


    /** @brief write to SPI3 and then read data back SPI3 and wait for both done
     *  @param [in]  uint8_t* txbuffer  Tx buffer data need to be write 
     *               size_t txlen       size of Tx buffer 
     *               size_t rxlen       Number of byte need to be read
     *               uint32_t maxWait: maximum time (in ms) wait for read done. If over
     * time, return error
     *  @param [out]  uint8_t* rxbuffer  Rx buffer to store data read back
     *  @return None
     *  @retval true read data success
     *  @retval false read data failed
     */
    bool SPI3_WriteRead(uint8_t* txbuffer, size_t txlen, uint8_t* rxbuffer, size_t rxlen, uint32_t maxWait);



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _SPI3_H */

/* *****************************************************************************
 End of File
 */
