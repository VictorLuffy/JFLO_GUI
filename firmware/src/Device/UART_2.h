/* ************************************************************************** */
/** @file [UART_2.h]
 *  @brief { control read/write data via UART_2}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */



#ifndef _UART_2_H    /* Guard against multiple inclusion */
#define _UART_2_H



/* This section lists the other files that are included in this file.
 */

#include <stdbool.h>
#include <stdint.h>
#include "system/common/sys_common.h"
#include "system_definitions.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief Initialize UART2, use to communicate with SPO2 sensor. This function 
     * open UART2 as none blocking, read/write enable and attached a buffer to store
     * receive data from UART receiver
     * This function should be called 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void Uart2_Initialize();

    /** @brief Send a packet of data through UART2
     * The data to send will not immediately put on UART2 port, it will store on Uart2TxBuffer
     * queue. Data on that Uart2TxBuffer queue will be put serially first in first out
     *  @param [in]  void *txData: pointer to data packet need to be sent
     *               uint16_t len: size of data packet 
     *  @param [out]  None
     *  @return None
     *  @retval true prepare for sending OK
     *  @retval false some error happen, may be the size of packet data too large compare
     * with Uart2TxBuffer or can not put the data on queue
     */
    bool Uart2_Send(uint8_t* txData, uint16_t len);

    /** @brief Read UART2 receive buffer and store on external buffer
     * After reading out, the UART2 Uart2RxBuffer will empty
     *  @param [in]  uint8_t *rxBuffer: pointer to external buffer to store receive data
     *               uint16_t len: length of external buffer
     *  @param [out]  None
     *  @return int16_t     (>0)number of byte successful read
     *                      (-1) if error occur
     */
    int16_t Uart2_ReadReceiveBuffer(uint8_t* rxBuffer, int16_t len);

    /** @brief Query how may byte available on UART2 Receiver buffer
     *  @param [in]     None
     *  @param [out]    None
     *  @return         size_t      (>0) number of byte available
     *                              (DRV_USART_BUFFER_HANDLE_INVALID) can not get size
     */
    size_t Uart2_GetReceiveBufferSize();

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _UART_2_H */

/* *****************************************************************************
 End of File
 */
