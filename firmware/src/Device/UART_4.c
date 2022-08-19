/* ************************************************************************** */
/** @file [UART_4.c]
 *  @brief { control read/write data via UART_4}
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


#include "UART_4.h"
#include "AlarmInterface.h"
#include "ApplicationDefinition.h"


/** @brief UART4 index on Harmony configuration */
#define UART4_DRIVER_INDEX                 2

/** @brief UART4 disable transmitter interrupt */
#define UART4_DISABLE_TX_INT	SYS_INT_SourceDisable(INT_VECTOR_UART4_TX)

/** @brief UART4 enable transmitter interrupt */
#define UART4_ENABLE_TX_INT		SYS_INT_SourceEnable(INT_VECTOR_UART4_TX)

/** @brief UART4 disable receiver interrupt */
#define UART4_DISABLE_RX_INT	SYS_INT_SourceDisable(INT_VECTOR_UART4_RX)

/** @brief UART4 enable receiver interrupt */
#define UART4_ENABLE_RX_INT		SYS_INT_SourceEnable(INT_VECTOR_UART4_RX)

/** @brief UART4 transmitter buffer size */
#define UART4_TX_BUFFER_SIZE    512

/** @brief UART4 receiver buffer size */
#define UART4_RX_BUFFER_SIZE    256



/** @brief UART4 port handle */
static DRV_HANDLE s_Uart4Handle = DRV_HANDLE_INVALID;

/** @brief Flag indicate UART4 has error
 * true mean error happened
 * false mean no error */
static E_DeviceErrorState s_Uart4Error = eDeviceNoError;

/** @brief UART4 RX buffer handle */
static DRV_USART_BUFFER_HANDLE s_Uart4RxBufferHandle;

/** @brief UART4 transmitter buffer pointer, indicate the start address of the 
 * packet locate on Uart4TxBuffer[] to be sent */
static uint16_t Uart4TxBuffPtr = 0;

/** @brief UART4 transmitter buffer, all data need to be transmitted will locate 
 * here and wait for sending. To avoid data over-written while it is not sent yet,
 make sure the buffer is large enough */
static uint8_t __attribute__((coherent)) Uart4TxBuffer[UART4_TX_BUFFER_SIZE] = {'\0'};

/** @brief UART4 receiver buffer, all received data will be located here and wait 
 * for reading out. To avoid data over-written while it is not reading out yet,
 make sure the buffer is large enough. A command to read out data will empty that
 buffer. The buffer full while the UART receiver keep continue to receive data 
 may crash system */
static uint8_t __attribute__((coherent)) Uart4RxBuffer[UART4_RX_BUFFER_SIZE] = {'\0'};

/** @brief internal functions declaration */
static bool Uart4_AttachReceiveBuffer();
static void Uart4_ReportError();

/** @brief Initialize UART4, use to communicate with SPO2 sensor. This function 
 * open UART4 as none blocking, read/write enable and attached a buffer to store
 * receive data from UART receiver
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void Uart4_Initialize() {
    //open UART4 port
    if (s_Uart4Handle == DRV_HANDLE_INVALID) {
        s_Uart4Handle = DRV_USART_Open(UART4_DRIVER_INDEX, DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_NONBLOCKING);
        if (DRV_HANDLE_INVALID == s_Uart4Handle) {
            //set error flag
            s_Uart4Error = eDeviceErrorDetected;
            //report error
            Uart4_ReportError();
            return;
        }
    }

    //attach receive buffer 
    Uart4_AttachReceiveBuffer();
    
    //clear variables
    s_Uart4Error = eDeviceNoError;
}

/** @brief Send a packet of data through UART4
 * The data to send will not immediately put on UART4 port, it will store on Uart4TxBuffer
 * queue. Data on that Uart4TxBuffer queue will be put serially first in first out
 *  @param [in]  void *txData: pointer to data packet need to be sent
 *               uint16_t len: size of data packet 
 *  @param [out]  None
 *  @return None
 *  @retval true prepare for sending OK
 *  @retval false some error happen, may be the size of packet data too large compare
 * with Uart4TxBuffer or can not put the data on queue
 */
bool Uart4_Send(uint8_t* txData, uint16_t len) {
    //check for error
    if (s_Uart4Error != eDeviceNoError) {
        Uart4_ReportError();
        return false;
    }
    
    //send data
    DRV_USART_BUFFER_HANDLE bufferHandle;
    if (len > 0 && len < UART4_TX_BUFFER_SIZE) {
        if (len + Uart4TxBuffPtr >= UART4_TX_BUFFER_SIZE) {
            Uart4TxBuffPtr = 0;
        }

        memcpy(&Uart4TxBuffer[Uart4TxBuffPtr], txData, len);

        DRV_USART_BufferAddWrite(s_Uart4Handle, &bufferHandle, (void *) &Uart4TxBuffer[Uart4TxBuffPtr], len);

        if (bufferHandle == DRV_USART_BUFFER_HANDLE_INVALID) {
            //set error flag
            s_Uart4Error = eDeviceErrorDetected;
            //report error
            Uart4_ReportError();
            return false;
        }
        else {
            //adjust pointer
            Uart4TxBuffPtr += len;
            return true;
        }
    } 
    else {
        return false;
    }
}

/** @brief Read UART4 receive buffer and store on external buffer
 * After reading out, the UART4 Uart4RxBuffer will empty
 *  @param [in]  uint8_t *rxBuffer: pointer to external buffer to store receive data
 *               uint16_t len: length of external buffer
 *  @param [out]  None
 *  @return int16_t     (>0)number of byte successful read
 *                      (-1) if error occur
 */
int16_t Uart4_ReadReceiveBuffer(uint8_t* rxBuffer, int16_t len) {
    //check for error
    if (s_Uart4Error != eDeviceNoError) {
        Uart4_ReportError();
        return -1;
    }
    
    //disable interrupt to prevent new data come
    UART4_DISABLE_RX_INT;
    //get number of byte already received
    size_t processedBytes = DRV_USART_BufferCompletedBytesGet(s_Uart4RxBufferHandle);

    if (processedBytes <= 0) {      //empty or invalid returned
        UART4_ENABLE_RX_INT;
        return 0;
    }

    if (len < processedBytes) {
        UART4_ENABLE_RX_INT;
        return -1; //error
    }

    //remove buffer handle
    DRV_USART_BUFFER_RESULT bufferResult;
    bufferResult = DRV_USART_BufferRemove(s_Uart4RxBufferHandle);
    if (DRV_USART_BUFFER_RESULT_REMOVED_SUCCESFULLY == bufferResult) {
        //Buffer removed successful from the queue
        //copy data from temporary buffer to external buffer
        memcpy(rxBuffer, &Uart4RxBuffer[0], processedBytes);
        //register receive buffer queue again
        Uart4_AttachReceiveBuffer();
        //re-enable interrupt
        UART4_ENABLE_RX_INT;

        return processedBytes;
    }
    else {
        //Either buffer is invalid or expired.
        //set error flag
        s_Uart4Error = eDeviceErrorDetected;
        //report error
        Uart4_ReportError();

        UART4_ENABLE_RX_INT;
        return -1; //error
    }
}

/** @brief Attach Uart4RxBuffer to UART4 Receiver. Data from UART4 Receiver will store
 * on attached buffer
 *  @param [in]     None
 *  @param [out]    None
 *  @return         None
 *  @retval true attaching buffer OK
 *  @retval false can not attach buffer to UART4 Receiver
 */
bool Uart4_AttachReceiveBuffer() {
    DRV_USART_BufferAddRead(s_Uart4Handle, &s_Uart4RxBufferHandle, &Uart4RxBuffer[0], UART4_RX_BUFFER_SIZE);
    if (DRV_USART_BUFFER_HANDLE_INVALID == s_Uart4RxBufferHandle) {
        // Error handling here
        //set error flag
        s_Uart4Error = eDeviceErrorDetected;
        //report error
        Uart4_ReportError();
        return false;
    }
    return true;
 
}

/** @brief Query how may byte available on UART4 Receiver buffer
 *  @param [in]     None
 *  @param [out]    None
 *  @return         size_t      (>0) number of byte available
 *                              (DRV_USART_BUFFER_HANDLE_INVALID) can not get size
 */
size_t Uart4_GetReceiveBufferSize () {
    size_t processedBytes = DRV_USART_BufferCompletedBytesGet(s_Uart4RxBufferHandle);
    return processedBytes;
}
 
/** @brief report error if occur during communication via UART4, may be send event
 * to Alarm task
 *  @param [in]  None 
 *  @param [out]  None
 *  @return None
 */
void Uart4_ReportError() {
        //check whether an error is detected, then send event to ALarm task
    if (s_Uart4Error == eDeviceErrorDetected) {
        //send event to alarm task
        alarmInterface_SendEvent(eUart4ErrorAlarm, eActive, eHighPriority, 0);
        SYS_PRINT("\n error at: Uart4_ReportError");
        //change state
        s_Uart4Error = eDeviceErrorReported;
    }
}


/* *****************************************************************************
 End of File
 */
