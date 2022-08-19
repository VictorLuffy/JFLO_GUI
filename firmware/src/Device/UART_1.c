/* ************************************************************************** */
/** @file [UART_1.c]
 *  @brief { control read/write data via UART_2}
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


#include "UART_1.h"
#include "AlarmInterface.h"
#include "ApplicationDefinition.h"


/** @brief UART1 index on Harmony configuration */
#define UART1_DRIVER_INDEX                 0

/** @brief UART1 disable transmitter interrupt */
#define UART1_DISABLE_TX_INT	SYS_INT_SourceDisable(INT_VECTOR_UART1_TX)

/** @brief UART1 enable transmitter interrupt */
#define UART1_ENABLE_TX_INT		SYS_INT_SourceEnable(INT_VECTOR_UART1_TX)

/** @brief UART1 disable receiver interrupt */
#define UART1_DISABLE_RX_INT	SYS_INT_SourceDisable(INT_VECTOR_UART1_RX)

/** @brief UART1 enable receiver interrupt */
#define UART1_ENABLE_RX_INT		SYS_INT_SourceEnable(INT_VECTOR_UART1_RX)

/** @brief UART1 transmitter buffer size */
#define UART1_TX_BUFFER_SIZE    512

/** @brief UART1 receiver buffer size */
#define UART1_RX_BUFFER_SIZE    256



/** @brief UART1 port handle */
static DRV_HANDLE s_Uart1Handle = DRV_HANDLE_INVALID;

/** @brief Flag indicate UART1 has error
 * true mean error happened
 * false mean no error */
static E_DeviceErrorState s_Uart1Error = eDeviceNoError;

/** @brief UART1 RX buffer handle */
static DRV_USART_BUFFER_HANDLE s_Uart1RxBufferHandle;

/** @brief UART1 transmitter buffer pointer, indicate the start address of the 
 * packet locate on Uart1TxBuffer[] to be sent */
static uint16_t Uart1TxBuffPtr = 0;

/** @brief UART1 transmitter buffer, all data need to be transmitted will locate 
 * here and wait for sending. To avoid data over-written while it is not sent yet,
 make sure the buffer is large enough */
static uint8_t __attribute__((coherent)) Uart1TxBuffer[UART1_TX_BUFFER_SIZE] = {'\0'};

/** @brief UART1 receiver buffer, all received data will be located here and wait 
 * for reading out. To avoid data over-written while it is not reading out yet,
 make sure the buffer is large enough. A command to read out data will empty that
 buffer. The buffer full while the UART receiver keep continue to receive data 
 may crash system */
static uint8_t __attribute__((coherent)) Uart1RxBuffer[UART1_RX_BUFFER_SIZE] = {'\0'};

/** @brief internal functions declaration */
static bool Uart1_AttachReceiveBuffer();
static void Uart1_ReportError();

/** @brief Initialize UART1, use to communicate with SPO2 sensor. This function 
 * open UART1 as none blocking, read/write enable and attached a buffer to store
 * receive data from UART receiver
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void Uart1_Initialize() {
    //open UART1 port
    if (s_Uart1Handle == DRV_HANDLE_INVALID) {
        s_Uart1Handle = DRV_USART_Open(UART1_DRIVER_INDEX, DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_NONBLOCKING);
        if (DRV_HANDLE_INVALID == s_Uart1Handle) {
            //set error flag
            s_Uart1Error = eDeviceErrorDetected;
            //report error
            Uart1_ReportError();
            return;
        }
    }

    //attach receive buffer 
    Uart1_AttachReceiveBuffer();
    
    //clear variables
    s_Uart1Error = eDeviceNoError;
}

/** @brief Send a packet of data through UART1
 * The data to send will not immediately put on UART1 port, it will store on Uart1TxBuffer
 * queue. Data on that Uart1TxBuffer queue will be put serially first in first out
 *  @param [in]  void *txData: pointer to data packet need to be sent
 *               uint16_t len: size of data packet 
 *  @param [out]  None
 *  @return None
 *  @retval true prepare for sending OK
 *  @retval false some error happen, may be the size of packet data too large compare
 * with Uart1TxBuffer or can not put the data on queue
 */
bool Uart1_Send(uint8_t* txData, uint16_t len) {
    //check for error
    if (s_Uart1Error != eDeviceNoError) {
        Uart1_ReportError();
        return false;
    }
    
    //send data
    DRV_USART_BUFFER_HANDLE bufferHandle;
    if (len > 0 && len < UART1_TX_BUFFER_SIZE) {
        if (len + Uart1TxBuffPtr >= UART1_TX_BUFFER_SIZE) {
            Uart1TxBuffPtr = 0;
        }

        memcpy(&Uart1TxBuffer[Uart1TxBuffPtr], txData, len);

        DRV_USART_BufferAddWrite(s_Uart1Handle, &bufferHandle, (void *) &Uart1TxBuffer[Uart1TxBuffPtr], len);

        if (bufferHandle == DRV_USART_BUFFER_HANDLE_INVALID) {
            //set error flag
            s_Uart1Error = eDeviceErrorDetected;
            //report error
            Uart1_ReportError();
            return false;
        }
        else {
            //adjust pointer
            Uart1TxBuffPtr += len;
            return true;
        }
    } 
    else {
        return false;
    }
}

/** @brief Read UART1 receive buffer and store on external buffer
 * After reading out, the UART1 Uart1RxBuffer will empty
 *  @param [in]  uint8_t *rxBuffer: pointer to external buffer to store receive data
 *               uint16_t len: length of external buffer
 *  @param [out]  None
 *  @return int16_t     (>0)number of byte successful read
 *                      (-1) if error occur
 */
int16_t Uart1_ReadReceiveBuffer(uint8_t* rxBuffer, int16_t len) {
    //check for error
    if (s_Uart1Error != eDeviceNoError) {
        Uart1_ReportError();
        return -1;
    }
    
    //disable interrupt to prevent new data come
    UART1_DISABLE_RX_INT;
    //get number of byte already received
    size_t processedBytes = DRV_USART_BufferCompletedBytesGet(s_Uart1RxBufferHandle);

    if (processedBytes <= 0) {      //empty or invalid returned
        UART1_ENABLE_RX_INT;
        return 0;
    }

    if (len < processedBytes) {
        UART1_ENABLE_RX_INT;
        return -1; //error
    }

    //remove buffer handle
    DRV_USART_BUFFER_RESULT bufferResult;
    bufferResult = DRV_USART_BufferRemove(s_Uart1RxBufferHandle);
    if (DRV_USART_BUFFER_RESULT_REMOVED_SUCCESFULLY == bufferResult) {
        //Buffer removed successful from the queue
        //copy data from temporary buffer to external buffer
        memcpy(rxBuffer, &Uart1RxBuffer[0], processedBytes);
        //register receive buffer queue again
        Uart1_AttachReceiveBuffer();
        //re-enable interrupt
        UART1_ENABLE_RX_INT;

        return processedBytes;
    }
    else {
        //Either buffer is invalid or expired.
        //set error flag
        s_Uart1Error = eDeviceErrorDetected;
        //report error
        Uart1_ReportError();

        UART1_ENABLE_RX_INT;
        return -1; //error
    }
}

/** @brief Attach Uart1RxBuffer to UART1 Receiver. Data from UART1 Receiver will store
 * on attached buffer
 *  @param [in]     None
 *  @param [out]    None
 *  @return         None
 *  @retval true attaching buffer OK
 *  @retval false can not attach buffer to UART1 Receiver
 */
bool Uart1_AttachReceiveBuffer() {
    DRV_USART_BufferAddRead(s_Uart1Handle, &s_Uart1RxBufferHandle, &Uart1RxBuffer[0], UART1_RX_BUFFER_SIZE);
    if (DRV_USART_BUFFER_HANDLE_INVALID == s_Uart1RxBufferHandle) {
        // Error handling here
        //set error flag
        s_Uart1Error = eDeviceErrorDetected;
        //report error
        Uart1_ReportError();
        return false;
    }
    return true;
}

/** @brief Query how may byte available on UART1 Receiver buffer
 *  @param [in]     None
 *  @param [out]    None
 *  @return         size_t      (>0) number of byte available
 *                              (DRV_USART_BUFFER_HANDLE_INVALID) can not get size
 */
size_t Uart1_GetReceiveBufferSize () {
    size_t processedBytes = DRV_USART_BufferCompletedBytesGet(s_Uart1RxBufferHandle);
    return processedBytes;
}
 
/** @brief report error if occur during communication via UART1, may be send event
 * to Alarm task
 *  @param [in]  None 
 *  @param [out]  None
 *  @return None
 */
void Uart1_ReportError() {
        //check whether an error is detected, then send event to ALarm task
    if (s_Uart1Error == eDeviceErrorDetected) {
        //send event to alarm task
        alarmInterface_SendEvent(eUart1ErrorAlarm, eActive, 0);
        SYS_PRINT("\n error at: Uart1_ReportError");
        //change state
        s_Uart1Error = eDeviceErrorReported;
    }
}


/* *****************************************************************************
 End of File
 */
