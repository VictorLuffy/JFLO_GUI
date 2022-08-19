/* ************************************************************************** */
/** @file [UART_6.c]
 *  @brief { control read/write data via UART_6}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "system/common/sys_common.h"

#include "UART_6.h"
#include "AlarmInterface.h"
#include "ApplicationDefinition.h"



/** @brief UART6 index on Harmony configuration */
#define UART6_DRIVER_INDEX                      3

/** @brief UART6 disable transmitter interrupt */
#define UART6_DISABLE_TX_INT	SYS_INT_SourceDisable(INT_VECTOR_UART6_TX)

/** @brief UART6 enable transmitter interrupt */
#define UART6_ENABLE_TX_INT		SYS_INT_SourceEnable(INT_VECTOR_UART6_TX)

/** @brief UART6 disable receiver interrupt */
#define UART6_DISABLE_RX_INT	SYS_INT_SourceDisable(INT_VECTOR_UART6_RX)

/** @brief UART6 enable receiver interrupt */
#define UART6_ENABLE_RX_INT		SYS_INT_SourceEnable(INT_VECTOR_UART6_RX)

/** @brief UART6 transmitter buffer size */
#define UART6_TX_BUFFER_SIZE    512

/** @brief UART6 receiver buffer size */
#define UART6_RX_BUFFER_SIZE    256



/** @brief UART6 port handle */
static DRV_HANDLE s_Uart6Handle = DRV_HANDLE_INVALID;

/** @brief Flag indicate UART2 has error
 * true mean error happened
 * false mean no error */
static E_DeviceErrorState s_Uart6Error = eDeviceNoError;

/** @brief UART6 RX buffer handle */
static DRV_USART_BUFFER_HANDLE s_Uart6RxBufferHandle;

/** @brief UART6 transmitter buffer pointer, indicate the start address of the 
 * packet locate on Uart6TxBuffer[] to be sent */
static uint16_t Uart6TxBuffPtr = 0;

/** @brief UART6 transmitter buffer, all data need to be transmitted will locate 
 * here and wait for sending. To avoid data over-written while it is not sent yet,
 make sure the buffer is large enough */
static uint8_t __attribute__((coherent)) Uart6TxBuffer[UART6_TX_BUFFER_SIZE] = {'\0'};

/** @brief UART6 receiver buffer, all received data will be located here and wait 
 * for reading out. To avoid data over-written while it is not reading out yet,
 make sure the buffer is large enough. A command to read out data will empty that
 buffer. The buffer full while the UART receiver keep continue to receive data 
 may crash system */
static uint8_t __attribute__((coherent)) Uart6RxBuffer[UART6_RX_BUFFER_SIZE] = {'\0'};

/** @brief internal functions declaration */
static bool Uart6_AttachReceiveBuffer();
static void Uart6_ReportError();

/** @brief Initialize UART6, use to communicate with SPO2 sensor. This function 
 * open UART6 as none blocking, read/write enable and attached a buffer to store
 * receive data from UART receiver
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void Uart6_Initialize() {
    //open UART6 port
    if (s_Uart6Handle == DRV_HANDLE_INVALID) {
        s_Uart6Handle = DRV_USART_Open(UART6_DRIVER_INDEX, DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_NONBLOCKING);
        if (DRV_HANDLE_INVALID == s_Uart6Handle) {
            //set error flag
            s_Uart6Error = eDeviceErrorDetected;
            //report error
            Uart6_ReportError();
            return;
        }
    }

    //attach receive buffer 
    Uart6_AttachReceiveBuffer();
    
    //clear variables
    s_Uart6Error = eDeviceNoError;
}

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
bool Uart6_Send(uint8_t* txData, uint16_t len) {
    //check for error
    if (s_Uart6Error != eDeviceNoError) {
        Uart6_ReportError();
        return false;
    }

    //send data
    DRV_USART_BUFFER_HANDLE bufferHandle;
    if (len > 0 && len < UART6_TX_BUFFER_SIZE) {
        if (len + Uart6TxBuffPtr >= UART6_TX_BUFFER_SIZE) {
            Uart6TxBuffPtr = 0;
        }

        memcpy(&Uart6TxBuffer[Uart6TxBuffPtr], txData, len);

        DRV_USART_BufferAddWrite(s_Uart6Handle, &bufferHandle, (void *) &Uart6TxBuffer[Uart6TxBuffPtr], len);

        if (bufferHandle == DRV_USART_BUFFER_HANDLE_INVALID) {
            //set error flag
            s_Uart6Error = eDeviceErrorDetected;
            //report error
            Uart6_ReportError();
            return false;
        }
        else {
            //adjust pointer
            Uart6TxBuffPtr += len;
            return true;
        }
    } 
    else {
        return false;
    }
}

/** @brief Read UART6 receive buffer and store on external buffer
 * After reading out, the UART6 Uart6RxBuffer will empty
 *  @param [in]  uint8_t *rxBuffer: pointer to external buffer to store receive data
 *               uint16_t len: length of external buffer
 *  @param [out]  None
 *  @return int16_t     (>0)number of byte successful read
 *                      (-1) if error occur
 */
int16_t Uart6_ReadReceiveBuffer(uint8_t* rxBuffer, int16_t len) {
    //check for error
    if (s_Uart6Error != eDeviceNoError) {
        Uart6_ReportError();
        return -1;
    }
    
    //disable interrupt to prevent new data come
    UART6_DISABLE_RX_INT;
    //get number of byte already received
    size_t processedBytes = DRV_USART_BufferCompletedBytesGet(s_Uart6RxBufferHandle);

    if (processedBytes <= 0) {      //empty or invalid returned
        UART6_ENABLE_RX_INT;
        return 0;
    }

    if (len < processedBytes) {
        UART6_ENABLE_RX_INT;
        return -1; //error
    }

    //remove buffer handle
    DRV_USART_BUFFER_RESULT bufferResult;
    bufferResult = DRV_USART_BufferRemove(s_Uart6RxBufferHandle);
    if (DRV_USART_BUFFER_RESULT_REMOVED_SUCCESFULLY == bufferResult) {
        //Buffer removed successful from the queue
        //copy data from temporary buffer to external buffer
        memcpy(rxBuffer, &Uart6RxBuffer[0], processedBytes);

        //register receive buffer queue again
        Uart6_AttachReceiveBuffer();

        UART6_ENABLE_RX_INT;

        return processedBytes;
    }
    else {
        //Either buffer is invalid or expired.
        //set error flag
        s_Uart6Error = eDeviceErrorDetected;
        //report error
        Uart6_ReportError();

        UART6_ENABLE_RX_INT;
        return -1; //error
    }
}

/** @brief Attach Uart6RxBuffer to UART6 Receiver. Data from UART6 Receiver will store
 * on attached buffer
 *  @param [in]     None
 *  @param [out]    None
 *  @return         None
 *  @retval true attaching buffer OK
 *  @retval false can not attach buffer to UART6 Receiver
 */
bool Uart6_AttachReceiveBuffer() {
    DRV_USART_BufferAddRead(s_Uart6Handle, &s_Uart6RxBufferHandle, &Uart6RxBuffer[0], UART6_RX_BUFFER_SIZE);
    if (DRV_USART_BUFFER_HANDLE_INVALID == s_Uart6RxBufferHandle) {
        // Error handling here
        SYS_PRINT("\n UART 6 Attached buffer error ********");
        Uart6_ReportError();
        return false;
    }
    return true;
}

/** @brief Query how may byte available on UART6 Receiver buffer
 *  @param [in]     None
 *  @param [out]    None
 *  @return         size_t      (>0) number of byte available
 *                              (DRV_USART_BUFFER_HANDLE_INVALID) can not get size
 */
size_t Uart6_GetReceiveBufferSize () {
    size_t processedBytes = DRV_USART_BufferCompletedBytesGet(s_Uart6RxBufferHandle);
    return processedBytes;
}
 
/** @brief report error if occur during communication via UART6, may be send event
 * to Alarm task
 *  @param [in]  None 
 *  @param [out]  None
 *  @return None
 */
void Uart6_ReportError() {
    //check whether an error is detected, then send event to ALarm task
    if (s_Uart6Error == eDeviceErrorDetected) {
        //send event to alarm task
        alarmInterface_SendEvent(eUart6ErrorAlarm, eActive, eHighPriority, 0);
        SYS_PRINT("\n error at: Uart6_ReportError");
        //change state
        s_Uart6Error = eDeviceErrorReported;
    }
}


/* *****************************************************************************
 End of File
 */
