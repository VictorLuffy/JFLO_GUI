/* ************************************************************************** */
/** @file [UART_6.h]
 *  @brief { control read/write data via UART_6}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */



#ifndef _UART_6_H    /* Guard against multiple inclusion */
#define _UART_6_H



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

    /** @brief Initialize UART6, use to communicate with PC application. This function 
     * open UART6 as none blocking, read/write enable and attached a buffer to store
     * receive data from UART receiver
     * This function should be called 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void Uart6_Initialize();

    /** @brief Send a packet of data through UART6
     * The data to send will not immediately put on UART6 port, it will store on Uart6TxBuffer
     * queue. Data on that Uart6TxBuffer queue will be put serially first in first out
     *  @param [in]  void *txData: pointer to data packet need to be sent
     *               uint16_t len: size of data packet 
     *  @param [out]  None
     *  @return None
     *  @retval true prepare for sending OK
     *  @retval false some error happen, may be the size of packet data too large compare
     * with Uart6TxBuffer or can not put the data on queue
     */
    bool Uart6_Send(uint8_t* txData, uint16_t len);

    /** @brief Read UART6 receive buffer and store on external buffer
     * After reading out, the UART6 Uart6RxBuffer will empty
     *  @param [in]  uint8_t *rxBuffer: pointer to external buffer to store receive data
     *               uint16_t len: length of external buffer
     *  @param [out]  None
     *  @return int16_t     (>0)    number of byte successful read
     *                      (-1)    if error occur
     */
    int16_t Uart6_ReadReceiveBuffer(uint8_t* rxBuffer, int16_t len);

    /** @brief Query how may byte available on UART6 Receiver buffer
     *  @param [in]     None
     *  @param [out]    None
     *  @return         size_t      (>0) number of byte available
     *                              (DRV_USART_BUFFER_HANDLE_INVALID) can not get size
     */
    size_t Uart6_GetReceiveBufferSize();

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _UART_6_H */

/* *****************************************************************************
 End of File
 */
