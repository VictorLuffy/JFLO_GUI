/*******************************************************************************
  USART Driver Static implementation

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usart_static.c

  Summary:
    Source code for the USART driver static implementation.

  Description:
    The USART device driver provides a simple interface to manage the USART
    modules on Microchip microcontrollers. This file contains static implementation
    for the USART driver.

  Remarks:
    Static interfaces incorporate the driver instance number within the names
    of the routines, eliminating the need for an object ID or object handle.

    Static single-open interfaces also eliminate the need for the open handle.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "system_definitions.h"


// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* This is the driver static object . */
DRV_USART_OBJ  gDrvUSART0Obj ;

// *****************************************************************************
// *****************************************************************************
// Section: Instance 0 static driver functions
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ DRV_USART0_Initialize(void)
{
    uint32_t clockSource;

    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    dObj = &gDrvUSART0Obj;

    /* Disable the USART module to configure it*/
    PLIB_USART_Disable (USART_ID_1);

    /* Update the USART OBJECT parameters. */
    dObj->interruptNestingCount = 0;
    dObj->queueSizeCurrentRead  = 0;
    dObj->queueSizeCurrentWrite = 0;
    dObj->queueRead             = NULL;
    dObj->queueWrite            = NULL;
    dObj->eventHandler          = NULL;
    dObj->context               = (uintptr_t)NULL;
    dObj->error                 = DRV_USART_ERROR_NONE;

    /* Create the hardware instance mutex. */
    if(OSAL_MUTEX_Create(&(dObj->mutexDriverInstance)) != OSAL_RESULT_TRUE)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Initialize the USART based on configuration settings */
    PLIB_USART_InitializeModeGeneral(USART_ID_1,
            false,  /*Auto baud*/
            false,  /*LoopBack mode*/
            false,  /*Auto wakeup on start*/
            false,  /*IRDA mode*/
            false);  /*Stop In Idle mode*/

    /* Set the line control mode */
    PLIB_USART_LineControlModeSelect(USART_ID_1, DRV_USART_LINE_CONTROL_8NONE1);

    /* We set the receive interrupt mode to receive an interrupt whenever FIFO
       is not empty */
    PLIB_USART_InitializeOperation(USART_ID_1,
            USART_RECEIVE_FIFO_ONE_CHAR,
            USART_TRANSMIT_FIFO_IDLE,
            USART_ENABLE_TX_RX_USED);

    /* Get the USART clock source value*/
    clockSource = SYS_CLK_PeripheralFrequencyGet ( CLK_BUS_PERIPHERAL_2 );

    /* Set the baud rate and enable the USART */
    PLIB_USART_BaudSetAndEnable(USART_ID_1,
            clockSource,
            115200);  /*Desired Baud rate value*/

    /* Clear the interrupts to be on the safer side*/
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_TRANSMIT);
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_RECEIVE);
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_ERROR);

    /* Enable the error interrupt source */
    SYS_INT_SourceEnable(INT_SOURCE_USART_1_ERROR);


    /* Return the driver instance value*/
    return (SYS_MODULE_OBJ)DRV_USART_INDEX_0;
}

void  DRV_USART0_Deinitialize(void)
{
    bool status;
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;

    dObj = &gDrvUSART0Obj;

    /* Deallocate all mutexes */
    if(OSAL_MUTEX_Delete(&(dObj->mutexDriverInstance)) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Mutex Delete Failed");
        return;
    }

    /* Disable the interrupts */
    status = SYS_INT_SourceDisable(INT_SOURCE_USART_1_TRANSMIT) ;
    status = SYS_INT_SourceDisable(INT_SOURCE_USART_1_RECEIVE) ;
    status = SYS_INT_SourceDisable(INT_SOURCE_USART_1_ERROR);
    /* Ignore the warning */
    (void)status;

    /* Disable USART module */
    PLIB_USART_Disable (USART_ID_1);

}


SYS_STATUS DRV_USART0_Status(void)
{
    /* Return the status as ready always */
    return SYS_STATUS_READY;
}


void DRV_USART0_TasksTransmit(void)
{
    /* This is the USART Driver Transmit tasks routine.
       In this function, the driver checks if a transmit
       interrupt is active and performs respective action*/

    /* Reading the transmit interrupt flag */
    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_1_TRANSMIT))
    {
        /* The USART driver is configured to generate an
           interrupt when the FIFO is empty. Additionally
           the queue is not empty. Which means there is
           work to be done in this routine. */
        _DRV_USART0_BufferQueueTxTasks();

        /* Clear up the interrupt flag */
        SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_TRANSMIT);
    }
}

void DRV_USART0_TasksReceive(void)
{
    /* This is the USART Driver Receive tasks routine. If the receive
       interrupt flag is set, the tasks routines are executed.
     */

    /* Reading the receive interrupt flag */
    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_1_RECEIVE))
    {
        _DRV_USART0_BufferQueueRxTasks();

        /* Clear up the interrupt flag */
        SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_RECEIVE);
    }
}


void DRV_USART0_TasksError(void)
{
    /* This is the USART Driver Error tasks routine. In this function, the
     * driver checks if an error interrupt has occurred. If so the error
     * condition is cleared.  */

    /* Reading the error interrupt flag */
    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_1_ERROR))
    {
        /* This means an error has occurred */
        _DRV_USART0_BufferQueueErrorTasks();
        /* Clear up the error interrupt flag */
        SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_ERROR);
    }
}

DRV_HANDLE DRV_USART0_Open(const SYS_MODULE_INDEX index, const DRV_IO_INTENT ioIntent)
{
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;

    dObj = &gDrvUSART0Obj;

    /* Create the semaphores for Read Operation of Read-Write Model*/
    if(OSAL_SEM_Create(&(dObj->semReadDone), OSAL_SEM_TYPE_COUNTING, 1, 0) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Semaphore creation failed");
        return(DRV_HANDLE_INVALID);
    }
    /* Create the semaphores for Write Operation of Read-Write Model*/
    if(OSAL_SEM_Create(&(dObj->semWriteDone), OSAL_SEM_TYPE_COUNTING, 1, 0) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Semaphore creation failed");
        return(DRV_HANDLE_INVALID);
    }


    /* Return the driver instance value*/
    return ((DRV_HANDLE)DRV_USART_INDEX_0 );
}

void DRV_USART0_Close(void)
{
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;

    dObj = &gDrvUSART0Obj;

    /* Deallocate all semaphores */
    if(OSAL_SEM_Delete(&(dObj->semWriteDone)) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Unable to delete client write done semaphore");
        return;
    }
    if(OSAL_SEM_Delete(&(dObj->semReadDone)) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Unable to delete client read done semaphore");
        return;
    }
    return;
}

DRV_USART_CLIENT_STATUS DRV_USART0_ClientStatus(void)
{
    /* Return the status as ready always*/
    return DRV_USART_CLIENT_STATUS_READY;
}

DRV_USART_TRANSFER_STATUS DRV_USART0_TransferStatus( void )
{
    DRV_USART_TRANSFER_STATUS result = 0;

    /* Check if RX data available */
    if(PLIB_USART_ReceiverDataIsAvailable(USART_ID_1))
    {
        result|= DRV_USART_TRANSFER_STATUS_RECEIVER_DATA_PRESENT;
    }
    else
    {
        result|= DRV_USART_TRANSFER_STATUS_RECEIVER_EMPTY;
    }

    /* Check if TX Buffer is empty */
    if(PLIB_USART_TransmitterIsEmpty(USART_ID_1))
    {
        result|= DRV_USART_TRANSFER_STATUS_TRANSMIT_EMPTY;
    }

    /* Check if the TX buffer is full */
    if(PLIB_USART_TransmitterBufferIsFull(USART_ID_1))
    {
        result|= DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL;
    }

    return(result);
}

DRV_USART_ERROR DRV_USART0_ErrorGet(void)
{
    DRV_USART_ERROR error;
    error = gDrvUSART0Obj.error;

    /* Clear the error before returning */
    gDrvUSART0Obj.error = DRV_USART_ERROR_NONE;

    /* Return the error*/
    return(error);
}


// *****************************************************************************
// *****************************************************************************
// Section: Local functions
// *****************************************************************************
// *****************************************************************************

void _DRV_USART0_BufferQueueRxTasks(void)
{
    DRV_USART_BUFFER_OBJ * bufferObj;
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    bool status;

    dObj = &gDrvUSART0Obj;

    /* If this driver is configured for polled mode in an RTOS, the tasks
       routine would be called from another thread. We need to get the driver
       instance mutex before updating the queue. If the driver is configured for
       interrupt mode, then _DRV_USART_TAKE_MUTEX will compile to true */

    if(DRV_USART_INTERRUPT_MODE == false)
    {
        if(OSAL_MUTEX_Lock(&(dObj->mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
        {
            /* We were able to take the mutex */
        }
        else
        {
            /* The mutex acquisition timed out. Return with an
               invalid handle. This code will not execute
               if there is no RTOS. */
            return;
        }
    }

    /* In this function, the driver checks if there are any buffers in queue. If
       so the buffer is serviced. A buffer that is serviced completely is
       removed from the queue. Start by getting the buffer at the head of the
       queue */

    bufferObj = dObj->queueRead;

    if(bufferObj != NULL)
    {
        /* The USART driver is configured to generate an interrupt when the FIFO
           is not empty. Additionally the queue is not empty. Which means there
           is work to done in this routine. Read data from the FIFO until either
           the FIFO is empty or until we have read the requested number of bytes.
           */

        while((PLIB_USART_ReceiverDataIsAvailable(USART_ID_1))
                && (bufferObj->nCurrentBytes < bufferObj->size ))
        {
            bufferObj->buffer[bufferObj->nCurrentBytes] = PLIB_USART_ReceiverByteReceive(USART_ID_1);
            bufferObj->nCurrentBytes ++;
        }

        /* Check if this buffer is done */

        if(bufferObj->nCurrentBytes >= bufferObj->size)
        {
            /* This means the buffer is completed. If there
               is a callback registered with client, then
               call it */

            if((dObj->eventHandler != NULL) && (bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD))
            {
                /* Call the event handler. We additionally increment the
                   interrupt nesting count which lets the driver functions
                   that are called from the event handler know that an
                   interrupt context is active.
                   */

                dObj->interruptNestingCount ++;

                dObj->eventHandler(DRV_USART_BUFFER_EVENT_COMPLETE,
                        bufferObj->bufferHandle,
                        dObj->context);

                dObj->interruptNestingCount --;
            }

            /* Get the next buffer in the queue and deallocate
               this buffer */

            dObj->queueRead = bufferObj->next;
            bufferObj->inUse = false;
            dObj->queueSizeCurrentRead --;

            /* Reset the next and previous pointers */
            bufferObj->next = NULL;
            bufferObj->previous = NULL;

            /* Reset the current head's previous pointer */
            if (dObj->queueRead != NULL)
            {
                dObj->queueRead->previous = NULL;
            }

            /* Check if read write mode and post semaphore */
            if(bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_READ_WRITE)
            {
                /* This means we should post the semaphore */
                _DRV_USART_SEM_POST(&(dObj->semReadDone));
            }
        }
    }

    if(dObj->queueRead == NULL)
    {
        /* The queue is empty. We can disable the interrupt */
        status = SYS_INT_SourceDisable(INT_SOURCE_USART_1_RECEIVE);
        /* Ignore the warning */
        (void)status;
    }

    /* Release the mutex */
   _DRV_USART_RELEASE_MUTEX(&(dObj->mutexDriverInstance));

}

void _DRV_USART0_BufferQueueTxTasks(void)
{
    /* Start by getting the buffer at the head of queue. */

    DRV_USART_BUFFER_OBJ * bufferObj;
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    bool status;

    dObj = &gDrvUSART0Obj;
    bufferObj = dObj->queueWrite;

    /* If this driver is configured for polled mode in an RTOS, the tasks
       routine would be called from another thread. We need to get the driver
       instance mutex before updating the queue. If the driver is configured for
       interrupt mode, then _DRV_USART_TAKE_MUTEX will compile to true */

    if(DRV_USART_INTERRUPT_MODE == false)
    {
        if(OSAL_MUTEX_Lock(&(dObj->mutexDriverInstance), OSAL_WAIT_FOREVER))
        {
            /* We were able to take the mutex */
        }
        else
        {
            /* The mutex acquisition timed out. Return with an
               invalid handle. This code will not execute
               if there is no RTOS. */
            return;
        }
    }

    if(bufferObj != NULL)
    {
        /* This means the queue is not empty. Check if this buffer is done */
        if(bufferObj->nCurrentBytes >= bufferObj->size)
        {
            /* This means the buffer is completed. If there
               is a callback registered with client, then
               call it */

            if((dObj->eventHandler != NULL) && (bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD))
            {
                /* Before calling the event handler, the interrupt nesting
                   counter is incremented. This will allow driver routine that
                   are called from the event handler to know the interrupt
                   nesting level. Events are only generated for buffers that
                   were submitted using the buffer add routine */

                dObj->interruptNestingCount ++;

                dObj->eventHandler(DRV_USART_BUFFER_EVENT_COMPLETE,
                        bufferObj->bufferHandle,
                        dObj->context);

                /* Decrement the nesting count */
                dObj->interruptNestingCount -- ;
            }

            /* Get the next buffer in the queue and deallocate
             * this buffer */

            dObj->queueWrite = bufferObj->next;
            bufferObj->inUse = false;
            dObj->queueSizeCurrentWrite --;

            /* Reset the next and previous pointers */
            bufferObj->next = NULL;
            bufferObj->previous = NULL;

            /* Reset the current head's previous pointer */
            if (dObj->queueWrite != NULL)
            {
                dObj->queueWrite->previous = NULL;
            }

            /* Check if read write mode and post semaphore */
            if(bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_READ_WRITE)
            {
                /* This means we should post the semaphore */
                _DRV_USART_SEM_POST(&(dObj->semWriteDone));

            }
        }
    }

    /* Check if the queue is still not empty and process
       the buffer */

    if(dObj->queueWrite != NULL)
    {
        bufferObj = dObj->queueWrite;

        /* Fill up the FIFO with data until the FIFO is full
           and we have data to send */
        while((!PLIB_USART_TransmitterBufferIsFull(USART_ID_1))
                && (bufferObj->nCurrentBytes < bufferObj->size ))
        {
            PLIB_USART_TransmitterByteSend(USART_ID_1, bufferObj->buffer[bufferObj->nCurrentBytes]);
            bufferObj->nCurrentBytes ++;
        }
    }
    else
    {
        /* If the queue is empty, then disable the TX interrupt */
        status = SYS_INT_SourceDisable(INT_SOURCE_USART_1_TRANSMIT);
        /* Ignore the warning */
        (void)status;
    }

    /* Release the mutex */
    _DRV_USART_RELEASE_MUTEX(&(dObj->mutexDriverInstance));

}

void _DRV_USART0_BufferQueueErrorTasks(void)
{
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    DRV_USART_BUFFER_OBJ * bufferObj;
    bool mutexGrabbed  = true;
    bool status = false;

    dObj = &gDrvUSART0Obj;

    /* USART driver will take care that TX erros( like overflow etc) are not
     * going to occur at any time based on checks before write.
     * So, only RX errors are to be handled/reported */

    /* Get the RX buffer at the head */
    bufferObj = dObj->queueRead;

    /* If this driver is configured for polled mode in an RTOS, the tasks
     * routine would be called from another thread. We need to get the driver
     * instance mutex before updating the queue. If the driver is configured for
     * interrupt mode, then OSAL_MUTEX_Lock will compile to true */

    if(DRV_USART_INTERRUPT_MODE == false)
    {
        if(!OSAL_MUTEX_Lock(&(dObj->mutexDriverInstance), OSAL_WAIT_FOREVER))
        {
            /* The mutex acquisition timed out.
             * This code will not execute if there is no RTOS. */
            mutexGrabbed = false;
        }
    }

    /* mutexGrabbed will always be true for non-RTOS case.
     * Will be false when mutex aquisition timed out in RTOS mode */
    if(true == mutexGrabbed)
    {
        if(bufferObj != NULL)
        {
            /* Get the USART errors */
            dObj->error = PLIB_USART_ErrorsGet(USART_ID_1);

	        /* Clear error condition */
            _DRV_USART0_ErrorConditionClear();

            /* Call event handler in Buffer queue request only.
             * If it's a File i/o request then DRV_USART_Read call itself
             * will return with error result */
            if((dObj->eventHandler != NULL) && (bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD))
            {
                /* Call the event handler with buffer event error state */
                dObj->eventHandler(DRV_USART_BUFFER_EVENT_ERROR,
                        bufferObj->bufferHandle,
                        dObj->context);
            }

            /* Get the next buffer in the queue and deallocate
             * this buffer */

            dObj->queueRead = bufferObj->next;
            bufferObj->inUse = false;
            dObj->queueSizeCurrentRead --;

            /* Reset the next and previous pointers */
            bufferObj->next = NULL;
            bufferObj->previous = NULL;

            /* If queue head is not null, update buffer pointer parameters*/
            if (dObj->queueRead != NULL)
            {
                /* Reset the updated head's previous pointer */
                dObj->queueRead->previous = NULL;
            }
            else
            {
                /* Queue is empty disable the RX interrupt */
                status = SYS_INT_SourceDisable(INT_SOURCE_USART_1_RECEIVE);

                /* Ignore the warning */
                (void)status;
            }
        }
        else
        {
            /* There is no buffer in the queue.
             * Flush the RX to clear the error condition */
            _DRV_USART0_ErrorConditionClear();
        }
    }

    /* Release the mutex in RTOS environment*/
    _DRV_USART_RELEASE_MUTEX(&(dObj->mutexDriverInstance));
}

void _DRV_USART0_ErrorConditionClear()
{
    uint8_t dummyData = 0u;
    /* RX length = (FIFO level + RX register) */
    uint8_t RXlength = _DRV_USART_RX_DEPTH;

    /* If it's a overrun error then clear it to flush FIFO */
    if(USART_ERROR_RECEIVER_OVERRUN & PLIB_USART_ErrorsGet(USART_ID_1))
    {
        PLIB_USART_ReceiverOverrunErrorClear(USART_ID_1);
    }

    /* Read existing error bytes from FIFO to clear parity and framing error flags*/
    while( (USART_ERROR_PARITY | USART_ERROR_FRAMING) & PLIB_USART_ErrorsGet(USART_ID_1) )
    {
        dummyData = PLIB_USART_ReceiverByteReceive(USART_ID_1);
        RXlength--;

        /* Try to flush error bytes for one full FIFO and exit instead of
         * blocking here if more error bytes are received*/
        if(0u == RXlength)
        {
            break;
        }
    }

    /* Ignore the warning */
    (void)dummyData;

    /* Clear error interrupt flag */
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_ERROR);

    /* Clear up the receive interrupt flag so that RX interrupt is not
     * triggered for error bytes*/
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_1_RECEIVE);
}



DRV_USART_BAUD_SET_RESULT DRV_USART0_BaudSet(uint32_t baud)
{
    uint32_t clockSource;
    int32_t brgValueLow=0;
    int32_t brgValueHigh=0;
    DRV_USART_BAUD_SET_RESULT retVal = DRV_USART_BAUD_SET_SUCCESS;
#if defined (PLIB_USART_ExistsModuleBusyStatus)
    bool isEnabled = false;
#endif

    /* Making this function thread safe */
    if(OSAL_MUTEX_Lock(&(gDrvUSART0Obj.mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
    {
        /* We were able to take the mutex */
    }
    else
    {
        /* The mutex timed out */
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Hardware instance mutex time out in DRV_USART_BaudSet() function");
        return(DRV_USART_BAUD_SET_ERROR);
    }

    /* Get the USART clock source value*/
    clockSource = SYS_CLK_PeripheralFrequencyGet ( CLK_BUS_PERIPHERAL_2 );

    /* Calculate low and high baud values */
    brgValueLow  = ( (clockSource/baud) >> 4 ) - 1;
    brgValueHigh = ( (clockSource/baud) >> 2 ) - 1;

#if defined (PLIB_USART_ExistsModuleBusyStatus)
        isEnabled = PLIB_USART_ModuleIsBusy (USART_ID_1);
        if (isEnabled)
        {
            PLIB_USART_Disable (USART_ID_1);
            while (PLIB_USART_ModuleIsBusy (USART_ID_1));
        }
#endif

    /* Check if the baud value can be set with high baud settings */
    if ((brgValueHigh >= 0) && (brgValueHigh <= UINT16_MAX))
    {
        PLIB_USART_BaudRateHighEnable(USART_ID_1);
        PLIB_USART_BaudRateHighSet(USART_ID_1,clockSource,baud);
    }

    /* Check if the baud value can be set with low baud settings */
    else if ((brgValueLow >= 0) && (brgValueLow <= UINT16_MAX))
    {
        PLIB_USART_BaudRateHighDisable(USART_ID_1);
        PLIB_USART_BaudRateSet(USART_ID_1, clockSource, baud);
    }
    else
    {
            retVal = DRV_USART_BAUD_SET_ERROR;
    }

#if defined (PLIB_USART_ExistsModuleBusyStatus)
    if (isEnabled)
    {
        PLIB_USART_Enable (USART_ID_1);
    }
#endif
    /* Unlock Mutex */
    OSAL_MUTEX_Unlock(&(gDrvUSART0Obj.mutexDriverInstance));

    return retVal;
}


DRV_USART_LINE_CONTROL_SET_RESULT DRV_USART0_LineControlSet(DRV_USART_LINE_CONTROL lineControlMode)
{
#if defined (PLIB_USART_ExistsModuleBusyStatus)
    bool isEnabled = false;
#endif
    /* Making this function thread safe */
    if(OSAL_MUTEX_Lock(&(gDrvUSART0Obj.mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
    {
        /* We were able to take the mutex */
    }
    else
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Hardware Instance Mutex time out in DRV_USART_LineControlSet() function");
        return DRV_USART_LINE_CONTROL_SET_ERROR;
    }
#if defined (PLIB_USART_ExistsModuleBusyStatus)
        isEnabled = PLIB_USART_ModuleIsBusy (USART_ID_1);
        if (isEnabled)
        {
            PLIB_USART_Disable (USART_ID_1);
            while (PLIB_USART_ModuleIsBusy (USART_ID_1));
        }
#endif

    /* Set the Line Control Mode */
    PLIB_USART_LineControlModeSelect(USART_ID_1, lineControlMode);

#if defined (PLIB_USART_ExistsModuleBusyStatus)
        if (isEnabled)
        {
            PLIB_USART_Enable (USART_ID_1);
        }
#endif
    OSAL_MUTEX_Unlock(&(gDrvUSART0Obj.mutexDriverInstance));

    /* Return success */
    return(DRV_USART_LINE_CONTROL_SET_SUCCESS);
}


// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* This is the driver static object . */
DRV_USART_OBJ  gDrvUSART1Obj ;

// *****************************************************************************
// *****************************************************************************
// Section: Instance 1 static driver functions
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ DRV_USART1_Initialize(void)
{
    uint32_t clockSource;

    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    dObj = &gDrvUSART1Obj;

    /* Disable the USART module to configure it*/
    PLIB_USART_Disable (USART_ID_2);

    /* Update the USART OBJECT parameters. */
    dObj->interruptNestingCount = 0;
    dObj->queueSizeCurrentRead  = 0;
    dObj->queueSizeCurrentWrite = 0;
    dObj->queueRead             = NULL;
    dObj->queueWrite            = NULL;
    dObj->eventHandler          = NULL;
    dObj->context               = (uintptr_t)NULL;
    dObj->error                 = DRV_USART_ERROR_NONE;

    /* Create the hardware instance mutex. */
    if(OSAL_MUTEX_Create(&(dObj->mutexDriverInstance)) != OSAL_RESULT_TRUE)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Initialize the USART based on configuration settings */
    PLIB_USART_InitializeModeGeneral(USART_ID_2,
            false,  /*Auto baud*/
            false,  /*LoopBack mode*/
            false,  /*Auto wakeup on start*/
            false,  /*IRDA mode*/
            false);  /*Stop In Idle mode*/

    /* Set the line control mode */
    PLIB_USART_LineControlModeSelect(USART_ID_2, DRV_USART_LINE_CONTROL_8NONE1);

    /* We set the receive interrupt mode to receive an interrupt whenever FIFO
       is not empty */
    PLIB_USART_InitializeOperation(USART_ID_2,
            USART_RECEIVE_FIFO_ONE_CHAR,
            USART_TRANSMIT_FIFO_IDLE,
            USART_ENABLE_TX_RX_USED);

    /* Get the USART clock source value*/
    clockSource = SYS_CLK_PeripheralFrequencyGet ( CLK_BUS_PERIPHERAL_2 );

    /* Set the baud rate and enable the USART */
    PLIB_USART_BaudSetAndEnable(USART_ID_2,
            clockSource,
            9600);  /*Desired Baud rate value*/

    /* Clear the interrupts to be on the safer side*/
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_2_TRANSMIT);
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_2_RECEIVE);
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_2_ERROR);

    /* Enable the error interrupt source */
    SYS_INT_SourceEnable(INT_SOURCE_USART_2_ERROR);


    /* Return the driver instance value*/
    return (SYS_MODULE_OBJ)DRV_USART_INDEX_1;
}

void  DRV_USART1_Deinitialize(void)
{
    bool status;
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;

    dObj = &gDrvUSART1Obj;

    /* Deallocate all mutexes */
    if(OSAL_MUTEX_Delete(&(dObj->mutexDriverInstance)) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Mutex Delete Failed");
        return;
    }

    /* Disable the interrupts */
    status = SYS_INT_SourceDisable(INT_SOURCE_USART_2_TRANSMIT) ;
    status = SYS_INT_SourceDisable(INT_SOURCE_USART_2_RECEIVE) ;
    status = SYS_INT_SourceDisable(INT_SOURCE_USART_2_ERROR);
    /* Ignore the warning */
    (void)status;

    /* Disable USART module */
    PLIB_USART_Disable (USART_ID_2);

}


SYS_STATUS DRV_USART1_Status(void)
{
    /* Return the status as ready always */
    return SYS_STATUS_READY;
}


void DRV_USART1_TasksTransmit(void)
{
    /* This is the USART Driver Transmit tasks routine.
       In this function, the driver checks if a transmit
       interrupt is active and performs respective action*/

    /* Reading the transmit interrupt flag */
    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_2_TRANSMIT))
    {
        /* The USART driver is configured to generate an
           interrupt when the FIFO is empty. Additionally
           the queue is not empty. Which means there is
           work to be done in this routine. */
        _DRV_USART1_BufferQueueTxTasks();

        /* Clear up the interrupt flag */
        SYS_INT_SourceStatusClear(INT_SOURCE_USART_2_TRANSMIT);
    }
}

void DRV_USART1_TasksReceive(void)
{
    /* This is the USART Driver Receive tasks routine. If the receive
       interrupt flag is set, the tasks routines are executed.
     */

    /* Reading the receive interrupt flag */
    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_2_RECEIVE))
    {
        _DRV_USART1_BufferQueueRxTasks();

        /* Clear up the interrupt flag */
        SYS_INT_SourceStatusClear(INT_SOURCE_USART_2_RECEIVE);
    }
}


void DRV_USART1_TasksError(void)
{
    /* This is the USART Driver Error tasks routine. In this function, the
     * driver checks if an error interrupt has occurred. If so the error
     * condition is cleared.  */

    /* Reading the error interrupt flag */
    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_2_ERROR))
    {
        /* This means an error has occurred */
        _DRV_USART1_BufferQueueErrorTasks();
        /* Clear up the error interrupt flag */
        SYS_INT_SourceStatusClear(INT_SOURCE_USART_2_ERROR);
    }
}

DRV_HANDLE DRV_USART1_Open(const SYS_MODULE_INDEX index, const DRV_IO_INTENT ioIntent)
{
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;

    dObj = &gDrvUSART1Obj;

    /* Create the semaphores for Read Operation of Read-Write Model*/
    if(OSAL_SEM_Create(&(dObj->semReadDone), OSAL_SEM_TYPE_COUNTING, 1, 0) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Semaphore creation failed");
        return(DRV_HANDLE_INVALID);
    }
    /* Create the semaphores for Write Operation of Read-Write Model*/
    if(OSAL_SEM_Create(&(dObj->semWriteDone), OSAL_SEM_TYPE_COUNTING, 1, 0) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Semaphore creation failed");
        return(DRV_HANDLE_INVALID);
    }


    /* Return the driver instance value*/
    return ((DRV_HANDLE)DRV_USART_INDEX_1 );
}

void DRV_USART1_Close(void)
{
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;

    dObj = &gDrvUSART1Obj;

    /* Deallocate all semaphores */
    if(OSAL_SEM_Delete(&(dObj->semWriteDone)) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Unable to delete client write done semaphore");
        return;
    }
    if(OSAL_SEM_Delete(&(dObj->semReadDone)) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Unable to delete client read done semaphore");
        return;
    }
    return;
}

DRV_USART_CLIENT_STATUS DRV_USART1_ClientStatus(void)
{
    /* Return the status as ready always*/
    return DRV_USART_CLIENT_STATUS_READY;
}

DRV_USART_TRANSFER_STATUS DRV_USART1_TransferStatus( void )
{
    DRV_USART_TRANSFER_STATUS result = 0;

    /* Check if RX data available */
    if(PLIB_USART_ReceiverDataIsAvailable(USART_ID_2))
    {
        result|= DRV_USART_TRANSFER_STATUS_RECEIVER_DATA_PRESENT;
    }
    else
    {
        result|= DRV_USART_TRANSFER_STATUS_RECEIVER_EMPTY;
    }

    /* Check if TX Buffer is empty */
    if(PLIB_USART_TransmitterIsEmpty(USART_ID_2))
    {
        result|= DRV_USART_TRANSFER_STATUS_TRANSMIT_EMPTY;
    }

    /* Check if the TX buffer is full */
    if(PLIB_USART_TransmitterBufferIsFull(USART_ID_2))
    {
        result|= DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL;
    }

    return(result);
}

DRV_USART_ERROR DRV_USART1_ErrorGet(void)
{
    DRV_USART_ERROR error;
    error = gDrvUSART1Obj.error;

    /* Clear the error before returning */
    gDrvUSART1Obj.error = DRV_USART_ERROR_NONE;

    /* Return the error*/
    return(error);
}


// *****************************************************************************
// *****************************************************************************
// Section: Local functions
// *****************************************************************************
// *****************************************************************************

void _DRV_USART1_BufferQueueRxTasks(void)
{
    DRV_USART_BUFFER_OBJ * bufferObj;
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    bool status;

    dObj = &gDrvUSART1Obj;

    /* If this driver is configured for polled mode in an RTOS, the tasks
       routine would be called from another thread. We need to get the driver
       instance mutex before updating the queue. If the driver is configured for
       interrupt mode, then _DRV_USART_TAKE_MUTEX will compile to true */

    if(DRV_USART_INTERRUPT_MODE == false)
    {
        if(OSAL_MUTEX_Lock(&(dObj->mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
        {
            /* We were able to take the mutex */
        }
        else
        {
            /* The mutex acquisition timed out. Return with an
               invalid handle. This code will not execute
               if there is no RTOS. */
            return;
        }
    }

    /* In this function, the driver checks if there are any buffers in queue. If
       so the buffer is serviced. A buffer that is serviced completely is
       removed from the queue. Start by getting the buffer at the head of the
       queue */

    bufferObj = dObj->queueRead;

    if(bufferObj != NULL)
    {
        /* The USART driver is configured to generate an interrupt when the FIFO
           is not empty. Additionally the queue is not empty. Which means there
           is work to done in this routine. Read data from the FIFO until either
           the FIFO is empty or until we have read the requested number of bytes.
           */

        while((PLIB_USART_ReceiverDataIsAvailable(USART_ID_2))
                && (bufferObj->nCurrentBytes < bufferObj->size ))
        {
            bufferObj->buffer[bufferObj->nCurrentBytes] = PLIB_USART_ReceiverByteReceive(USART_ID_2);
            bufferObj->nCurrentBytes ++;
        }

        /* Check if this buffer is done */

        if(bufferObj->nCurrentBytes >= bufferObj->size)
        {
            /* This means the buffer is completed. If there
               is a callback registered with client, then
               call it */

            if((dObj->eventHandler != NULL) && (bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD))
            {
                /* Call the event handler. We additionally increment the
                   interrupt nesting count which lets the driver functions
                   that are called from the event handler know that an
                   interrupt context is active.
                   */

                dObj->interruptNestingCount ++;

                dObj->eventHandler(DRV_USART_BUFFER_EVENT_COMPLETE,
                        bufferObj->bufferHandle,
                        dObj->context);

                dObj->interruptNestingCount --;
            }

            /* Get the next buffer in the queue and deallocate
               this buffer */

            dObj->queueRead = bufferObj->next;
            bufferObj->inUse = false;
            dObj->queueSizeCurrentRead --;

            /* Reset the next and previous pointers */
            bufferObj->next = NULL;
            bufferObj->previous = NULL;

            /* Reset the current head's previous pointer */
            if (dObj->queueRead != NULL)
            {
                dObj->queueRead->previous = NULL;
            }

            /* Check if read write mode and post semaphore */
            if(bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_READ_WRITE)
            {
                /* This means we should post the semaphore */
                _DRV_USART_SEM_POST(&(dObj->semReadDone));
            }
        }
    }

    if(dObj->queueRead == NULL)
    {
        /* The queue is empty. We can disable the interrupt */
        status = SYS_INT_SourceDisable(INT_SOURCE_USART_2_RECEIVE);
        /* Ignore the warning */
        (void)status;
    }

    /* Release the mutex */
   _DRV_USART_RELEASE_MUTEX(&(dObj->mutexDriverInstance));

}

void _DRV_USART1_BufferQueueTxTasks(void)
{
    /* Start by getting the buffer at the head of queue. */

    DRV_USART_BUFFER_OBJ * bufferObj;
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    bool status;

    dObj = &gDrvUSART1Obj;
    bufferObj = dObj->queueWrite;

    /* If this driver is configured for polled mode in an RTOS, the tasks
       routine would be called from another thread. We need to get the driver
       instance mutex before updating the queue. If the driver is configured for
       interrupt mode, then _DRV_USART_TAKE_MUTEX will compile to true */

    if(DRV_USART_INTERRUPT_MODE == false)
    {
        if(OSAL_MUTEX_Lock(&(dObj->mutexDriverInstance), OSAL_WAIT_FOREVER))
        {
            /* We were able to take the mutex */
        }
        else
        {
            /* The mutex acquisition timed out. Return with an
               invalid handle. This code will not execute
               if there is no RTOS. */
            return;
        }
    }

    if(bufferObj != NULL)
    {
        /* This means the queue is not empty. Check if this buffer is done */
        if(bufferObj->nCurrentBytes >= bufferObj->size)
        {
            /* This means the buffer is completed. If there
               is a callback registered with client, then
               call it */

            if((dObj->eventHandler != NULL) && (bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD))
            {
                /* Before calling the event handler, the interrupt nesting
                   counter is incremented. This will allow driver routine that
                   are called from the event handler to know the interrupt
                   nesting level. Events are only generated for buffers that
                   were submitted using the buffer add routine */

                dObj->interruptNestingCount ++;

                dObj->eventHandler(DRV_USART_BUFFER_EVENT_COMPLETE,
                        bufferObj->bufferHandle,
                        dObj->context);

                /* Decrement the nesting count */
                dObj->interruptNestingCount -- ;
            }

            /* Get the next buffer in the queue and deallocate
             * this buffer */

            dObj->queueWrite = bufferObj->next;
            bufferObj->inUse = false;
            dObj->queueSizeCurrentWrite --;

            /* Reset the next and previous pointers */
            bufferObj->next = NULL;
            bufferObj->previous = NULL;

            /* Reset the current head's previous pointer */
            if (dObj->queueWrite != NULL)
            {
                dObj->queueWrite->previous = NULL;
            }

            /* Check if read write mode and post semaphore */
            if(bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_READ_WRITE)
            {
                /* This means we should post the semaphore */
                _DRV_USART_SEM_POST(&(dObj->semWriteDone));

            }
        }
    }

    /* Check if the queue is still not empty and process
       the buffer */

    if(dObj->queueWrite != NULL)
    {
        bufferObj = dObj->queueWrite;

        /* Fill up the FIFO with data until the FIFO is full
           and we have data to send */
        while((!PLIB_USART_TransmitterBufferIsFull(USART_ID_2))
                && (bufferObj->nCurrentBytes < bufferObj->size ))
        {
            PLIB_USART_TransmitterByteSend(USART_ID_2, bufferObj->buffer[bufferObj->nCurrentBytes]);
            bufferObj->nCurrentBytes ++;
        }
    }
    else
    {
        /* If the queue is empty, then disable the TX interrupt */
        status = SYS_INT_SourceDisable(INT_SOURCE_USART_2_TRANSMIT);
        /* Ignore the warning */
        (void)status;
    }

    /* Release the mutex */
    _DRV_USART_RELEASE_MUTEX(&(dObj->mutexDriverInstance));

}

void _DRV_USART1_BufferQueueErrorTasks(void)
{
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    DRV_USART_BUFFER_OBJ * bufferObj;
    bool mutexGrabbed  = true;
    bool status = false;

    dObj = &gDrvUSART1Obj;

    /* USART driver will take care that TX erros( like overflow etc) are not
     * going to occur at any time based on checks before write.
     * So, only RX errors are to be handled/reported */

    /* Get the RX buffer at the head */
    bufferObj = dObj->queueRead;

    /* If this driver is configured for polled mode in an RTOS, the tasks
     * routine would be called from another thread. We need to get the driver
     * instance mutex before updating the queue. If the driver is configured for
     * interrupt mode, then OSAL_MUTEX_Lock will compile to true */

    if(DRV_USART_INTERRUPT_MODE == false)
    {
        if(!OSAL_MUTEX_Lock(&(dObj->mutexDriverInstance), OSAL_WAIT_FOREVER))
        {
            /* The mutex acquisition timed out.
             * This code will not execute if there is no RTOS. */
            mutexGrabbed = false;
        }
    }

    /* mutexGrabbed will always be true for non-RTOS case.
     * Will be false when mutex aquisition timed out in RTOS mode */
    if(true == mutexGrabbed)
    {
        if(bufferObj != NULL)
        {
            /* Get the USART errors */
            dObj->error = PLIB_USART_ErrorsGet(USART_ID_2);

	        /* Clear error condition */
            _DRV_USART1_ErrorConditionClear();

            /* Call event handler in Buffer queue request only.
             * If it's a File i/o request then DRV_USART_Read call itself
             * will return with error result */
            if((dObj->eventHandler != NULL) && (bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD))
            {
                /* Call the event handler with buffer event error state */
                dObj->eventHandler(DRV_USART_BUFFER_EVENT_ERROR,
                        bufferObj->bufferHandle,
                        dObj->context);
            }

            /* Get the next buffer in the queue and deallocate
             * this buffer */

            dObj->queueRead = bufferObj->next;
            bufferObj->inUse = false;
            dObj->queueSizeCurrentRead --;

            /* Reset the next and previous pointers */
            bufferObj->next = NULL;
            bufferObj->previous = NULL;

            /* If queue head is not null, update buffer pointer parameters*/
            if (dObj->queueRead != NULL)
            {
                /* Reset the updated head's previous pointer */
                dObj->queueRead->previous = NULL;
            }
            else
            {
                /* Queue is empty disable the RX interrupt */
                status = SYS_INT_SourceDisable(INT_SOURCE_USART_2_RECEIVE);

                /* Ignore the warning */
                (void)status;
            }
        }
        else
        {
            /* There is no buffer in the queue.
             * Flush the RX to clear the error condition */
            _DRV_USART1_ErrorConditionClear();
        }
    }

    /* Release the mutex in RTOS environment*/
    _DRV_USART_RELEASE_MUTEX(&(dObj->mutexDriverInstance));
}

void _DRV_USART1_ErrorConditionClear()
{
    uint8_t dummyData = 0u;
    /* RX length = (FIFO level + RX register) */
    uint8_t RXlength = _DRV_USART_RX_DEPTH;

    /* If it's a overrun error then clear it to flush FIFO */
    if(USART_ERROR_RECEIVER_OVERRUN & PLIB_USART_ErrorsGet(USART_ID_2))
    {
        PLIB_USART_ReceiverOverrunErrorClear(USART_ID_2);
    }

    /* Read existing error bytes from FIFO to clear parity and framing error flags*/
    while( (USART_ERROR_PARITY | USART_ERROR_FRAMING) & PLIB_USART_ErrorsGet(USART_ID_2) )
    {
        dummyData = PLIB_USART_ReceiverByteReceive(USART_ID_2);
        RXlength--;

        /* Try to flush error bytes for one full FIFO and exit instead of
         * blocking here if more error bytes are received*/
        if(0u == RXlength)
        {
            break;
        }
    }

    /* Ignore the warning */
    (void)dummyData;

    /* Clear error interrupt flag */
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_2_ERROR);

    /* Clear up the receive interrupt flag so that RX interrupt is not
     * triggered for error bytes*/
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_2_RECEIVE);
}



DRV_USART_BAUD_SET_RESULT DRV_USART1_BaudSet(uint32_t baud)
{
    uint32_t clockSource;
    int32_t brgValueLow=0;
    int32_t brgValueHigh=0;
    DRV_USART_BAUD_SET_RESULT retVal = DRV_USART_BAUD_SET_SUCCESS;
#if defined (PLIB_USART_ExistsModuleBusyStatus)
    bool isEnabled = false;
#endif

    /* Making this function thread safe */
    if(OSAL_MUTEX_Lock(&(gDrvUSART1Obj.mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
    {
        /* We were able to take the mutex */
    }
    else
    {
        /* The mutex timed out */
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Hardware instance mutex time out in DRV_USART_BaudSet() function");
        return(DRV_USART_BAUD_SET_ERROR);
    }

    /* Get the USART clock source value*/
    clockSource = SYS_CLK_PeripheralFrequencyGet ( CLK_BUS_PERIPHERAL_2 );

    /* Calculate low and high baud values */
    brgValueLow  = ( (clockSource/baud) >> 4 ) - 1;
    brgValueHigh = ( (clockSource/baud) >> 2 ) - 1;

#if defined (PLIB_USART_ExistsModuleBusyStatus)
        isEnabled = PLIB_USART_ModuleIsBusy (USART_ID_2);
        if (isEnabled)
        {
            PLIB_USART_Disable (USART_ID_2);
            while (PLIB_USART_ModuleIsBusy (USART_ID_2));
        }
#endif

    /* Check if the baud value can be set with high baud settings */
    if ((brgValueHigh >= 0) && (brgValueHigh <= UINT16_MAX))
    {
        PLIB_USART_BaudRateHighEnable(USART_ID_2);
        PLIB_USART_BaudRateHighSet(USART_ID_2,clockSource,baud);
    }

    /* Check if the baud value can be set with low baud settings */
    else if ((brgValueLow >= 0) && (brgValueLow <= UINT16_MAX))
    {
        PLIB_USART_BaudRateHighDisable(USART_ID_2);
        PLIB_USART_BaudRateSet(USART_ID_2, clockSource, baud);
    }
    else
    {
            retVal = DRV_USART_BAUD_SET_ERROR;
    }

#if defined (PLIB_USART_ExistsModuleBusyStatus)
    if (isEnabled)
    {
        PLIB_USART_Enable (USART_ID_2);
    }
#endif
    /* Unlock Mutex */
    OSAL_MUTEX_Unlock(&(gDrvUSART1Obj.mutexDriverInstance));

    return retVal;
}


DRV_USART_LINE_CONTROL_SET_RESULT DRV_USART1_LineControlSet(DRV_USART_LINE_CONTROL lineControlMode)
{
#if defined (PLIB_USART_ExistsModuleBusyStatus)
    bool isEnabled = false;
#endif
    /* Making this function thread safe */
    if(OSAL_MUTEX_Lock(&(gDrvUSART1Obj.mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
    {
        /* We were able to take the mutex */
    }
    else
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Hardware Instance Mutex time out in DRV_USART_LineControlSet() function");
        return DRV_USART_LINE_CONTROL_SET_ERROR;
    }
#if defined (PLIB_USART_ExistsModuleBusyStatus)
        isEnabled = PLIB_USART_ModuleIsBusy (USART_ID_2);
        if (isEnabled)
        {
            PLIB_USART_Disable (USART_ID_2);
            while (PLIB_USART_ModuleIsBusy (USART_ID_2));
        }
#endif

    /* Set the Line Control Mode */
    PLIB_USART_LineControlModeSelect(USART_ID_2, lineControlMode);

#if defined (PLIB_USART_ExistsModuleBusyStatus)
        if (isEnabled)
        {
            PLIB_USART_Enable (USART_ID_2);
        }
#endif
    OSAL_MUTEX_Unlock(&(gDrvUSART1Obj.mutexDriverInstance));

    /* Return success */
    return(DRV_USART_LINE_CONTROL_SET_SUCCESS);
}


// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

/* This is the driver static object . */
DRV_USART_OBJ  gDrvUSART2Obj ;

// *****************************************************************************
// *****************************************************************************
// Section: Instance 2 static driver functions
// *****************************************************************************
// *****************************************************************************

SYS_MODULE_OBJ DRV_USART2_Initialize(void)
{
    uint32_t clockSource;

    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    dObj = &gDrvUSART2Obj;

    /* Disable the USART module to configure it*/
    PLIB_USART_Disable (USART_ID_4);

    /* Update the USART OBJECT parameters. */
    dObj->interruptNestingCount = 0;
    dObj->queueSizeCurrentRead  = 0;
    dObj->queueSizeCurrentWrite = 0;
    dObj->queueRead             = NULL;
    dObj->queueWrite            = NULL;
    dObj->eventHandler          = NULL;
    dObj->context               = (uintptr_t)NULL;
    dObj->error                 = DRV_USART_ERROR_NONE;

    /* Create the hardware instance mutex. */
    if(OSAL_MUTEX_Create(&(dObj->mutexDriverInstance)) != OSAL_RESULT_TRUE)
    {
        return SYS_MODULE_OBJ_INVALID;
    }

    /* Initialize the USART based on configuration settings */
    PLIB_USART_InitializeModeGeneral(USART_ID_4,
            false,  /*Auto baud*/
            false,  /*LoopBack mode*/
            false,  /*Auto wakeup on start*/
            false,  /*IRDA mode*/
            false);  /*Stop In Idle mode*/

    /* Set the line control mode */
    PLIB_USART_LineControlModeSelect(USART_ID_4, DRV_USART_LINE_CONTROL_8NONE1);

    /* We set the receive interrupt mode to receive an interrupt whenever FIFO
       is not empty */
    PLIB_USART_InitializeOperation(USART_ID_4,
            USART_RECEIVE_FIFO_ONE_CHAR,
            USART_TRANSMIT_FIFO_IDLE,
            USART_ENABLE_TX_RX_USED);

    /* Get the USART clock source value*/
    clockSource = SYS_CLK_PeripheralFrequencyGet ( CLK_BUS_PERIPHERAL_2 );

    /* Set the baud rate and enable the USART */
    PLIB_USART_BaudSetAndEnable(USART_ID_4,
            clockSource,
            230400);  /*Desired Baud rate value*/

    /* Clear the interrupts to be on the safer side*/
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_4_TRANSMIT);
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_4_RECEIVE);
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_4_ERROR);

    /* Enable the error interrupt source */
    SYS_INT_SourceEnable(INT_SOURCE_USART_4_ERROR);


    /* Return the driver instance value*/
    return (SYS_MODULE_OBJ)DRV_USART_INDEX_2;
}

void  DRV_USART2_Deinitialize(void)
{
    bool status;
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;

    dObj = &gDrvUSART2Obj;

    /* Deallocate all mutexes */
    if(OSAL_MUTEX_Delete(&(dObj->mutexDriverInstance)) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Mutex Delete Failed");
        return;
    }

    /* Disable the interrupts */
    status = SYS_INT_SourceDisable(INT_SOURCE_USART_4_TRANSMIT) ;
    status = SYS_INT_SourceDisable(INT_SOURCE_USART_4_RECEIVE) ;
    status = SYS_INT_SourceDisable(INT_SOURCE_USART_4_ERROR);
    /* Ignore the warning */
    (void)status;

    /* Disable USART module */
    PLIB_USART_Disable (USART_ID_4);

}


SYS_STATUS DRV_USART2_Status(void)
{
    /* Return the status as ready always */
    return SYS_STATUS_READY;
}


void DRV_USART2_TasksTransmit(void)
{
    /* This is the USART Driver Transmit tasks routine.
       In this function, the driver checks if a transmit
       interrupt is active and performs respective action*/

    /* Reading the transmit interrupt flag */
    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_4_TRANSMIT))
    {
        /* The USART driver is configured to generate an
           interrupt when the FIFO is empty. Additionally
           the queue is not empty. Which means there is
           work to be done in this routine. */
        _DRV_USART2_BufferQueueTxTasks();

        /* Clear up the interrupt flag */
        SYS_INT_SourceStatusClear(INT_SOURCE_USART_4_TRANSMIT);
    }
}

void DRV_USART2_TasksReceive(void)
{
    /* This is the USART Driver Receive tasks routine. If the receive
       interrupt flag is set, the tasks routines are executed.
     */

    /* Reading the receive interrupt flag */
    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_4_RECEIVE))
    {
        _DRV_USART2_BufferQueueRxTasks();

        /* Clear up the interrupt flag */
        SYS_INT_SourceStatusClear(INT_SOURCE_USART_4_RECEIVE);
    }
}


void DRV_USART2_TasksError(void)
{
    /* This is the USART Driver Error tasks routine. In this function, the
     * driver checks if an error interrupt has occurred. If so the error
     * condition is cleared.  */

    /* Reading the error interrupt flag */
    if(SYS_INT_SourceStatusGet(INT_SOURCE_USART_4_ERROR))
    {
        /* This means an error has occurred */
        _DRV_USART2_BufferQueueErrorTasks();
        /* Clear up the error interrupt flag */
        SYS_INT_SourceStatusClear(INT_SOURCE_USART_4_ERROR);
    }
}

DRV_HANDLE DRV_USART2_Open(const SYS_MODULE_INDEX index, const DRV_IO_INTENT ioIntent)
{
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;

    dObj = &gDrvUSART2Obj;

    /* Create the semaphores for Read Operation of Read-Write Model*/
    if(OSAL_SEM_Create(&(dObj->semReadDone), OSAL_SEM_TYPE_COUNTING, 1, 0) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Semaphore creation failed");
        return(DRV_HANDLE_INVALID);
    }
    /* Create the semaphores for Write Operation of Read-Write Model*/
    if(OSAL_SEM_Create(&(dObj->semWriteDone), OSAL_SEM_TYPE_COUNTING, 1, 0) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Semaphore creation failed");
        return(DRV_HANDLE_INVALID);
    }


    /* Return the driver instance value*/
    return ((DRV_HANDLE)DRV_USART_INDEX_2 );
}

void DRV_USART2_Close(void)
{
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;

    dObj = &gDrvUSART2Obj;

    /* Deallocate all semaphores */
    if(OSAL_SEM_Delete(&(dObj->semWriteDone)) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Unable to delete client write done semaphore");
        return;
    }
    if(OSAL_SEM_Delete(&(dObj->semReadDone)) != OSAL_RESULT_TRUE)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Unable to delete client read done semaphore");
        return;
    }
    return;
}

DRV_USART_CLIENT_STATUS DRV_USART2_ClientStatus(void)
{
    /* Return the status as ready always*/
    return DRV_USART_CLIENT_STATUS_READY;
}

DRV_USART_TRANSFER_STATUS DRV_USART2_TransferStatus( void )
{
    DRV_USART_TRANSFER_STATUS result = 0;

    /* Check if RX data available */
    if(PLIB_USART_ReceiverDataIsAvailable(USART_ID_4))
    {
        result|= DRV_USART_TRANSFER_STATUS_RECEIVER_DATA_PRESENT;
    }
    else
    {
        result|= DRV_USART_TRANSFER_STATUS_RECEIVER_EMPTY;
    }

    /* Check if TX Buffer is empty */
    if(PLIB_USART_TransmitterIsEmpty(USART_ID_4))
    {
        result|= DRV_USART_TRANSFER_STATUS_TRANSMIT_EMPTY;
    }

    /* Check if the TX buffer is full */
    if(PLIB_USART_TransmitterBufferIsFull(USART_ID_4))
    {
        result|= DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL;
    }

    return(result);
}

DRV_USART_ERROR DRV_USART2_ErrorGet(void)
{
    DRV_USART_ERROR error;
    error = gDrvUSART2Obj.error;

    /* Clear the error before returning */
    gDrvUSART2Obj.error = DRV_USART_ERROR_NONE;

    /* Return the error*/
    return(error);
}


// *****************************************************************************
// *****************************************************************************
// Section: Local functions
// *****************************************************************************
// *****************************************************************************

void _DRV_USART2_BufferQueueRxTasks(void)
{
    DRV_USART_BUFFER_OBJ * bufferObj;
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    bool status;

    dObj = &gDrvUSART2Obj;

    /* If this driver is configured for polled mode in an RTOS, the tasks
       routine would be called from another thread. We need to get the driver
       instance mutex before updating the queue. If the driver is configured for
       interrupt mode, then _DRV_USART_TAKE_MUTEX will compile to true */

    if(DRV_USART_INTERRUPT_MODE == false)
    {
        if(OSAL_MUTEX_Lock(&(dObj->mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
        {
            /* We were able to take the mutex */
        }
        else
        {
            /* The mutex acquisition timed out. Return with an
               invalid handle. This code will not execute
               if there is no RTOS. */
            return;
        }
    }

    /* In this function, the driver checks if there are any buffers in queue. If
       so the buffer is serviced. A buffer that is serviced completely is
       removed from the queue. Start by getting the buffer at the head of the
       queue */

    bufferObj = dObj->queueRead;

    if(bufferObj != NULL)
    {
        /* The USART driver is configured to generate an interrupt when the FIFO
           is not empty. Additionally the queue is not empty. Which means there
           is work to done in this routine. Read data from the FIFO until either
           the FIFO is empty or until we have read the requested number of bytes.
           */

        while((PLIB_USART_ReceiverDataIsAvailable(USART_ID_4))
                && (bufferObj->nCurrentBytes < bufferObj->size ))
        {
            bufferObj->buffer[bufferObj->nCurrentBytes] = PLIB_USART_ReceiverByteReceive(USART_ID_4);
            bufferObj->nCurrentBytes ++;
        }

        /* Check if this buffer is done */

        if(bufferObj->nCurrentBytes >= bufferObj->size)
        {
            /* This means the buffer is completed. If there
               is a callback registered with client, then
               call it */

            if((dObj->eventHandler != NULL) && (bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD))
            {
                /* Call the event handler. We additionally increment the
                   interrupt nesting count which lets the driver functions
                   that are called from the event handler know that an
                   interrupt context is active.
                   */

                dObj->interruptNestingCount ++;

                dObj->eventHandler(DRV_USART_BUFFER_EVENT_COMPLETE,
                        bufferObj->bufferHandle,
                        dObj->context);

                dObj->interruptNestingCount --;
            }

            /* Get the next buffer in the queue and deallocate
               this buffer */

            dObj->queueRead = bufferObj->next;
            bufferObj->inUse = false;
            dObj->queueSizeCurrentRead --;

            /* Reset the next and previous pointers */
            bufferObj->next = NULL;
            bufferObj->previous = NULL;

            /* Reset the current head's previous pointer */
            if (dObj->queueRead != NULL)
            {
                dObj->queueRead->previous = NULL;
            }

            /* Check if read write mode and post semaphore */
            if(bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_READ_WRITE)
            {
                /* This means we should post the semaphore */
                _DRV_USART_SEM_POST(&(dObj->semReadDone));
            }
        }
    }

    if(dObj->queueRead == NULL)
    {
        /* The queue is empty. We can disable the interrupt */
        status = SYS_INT_SourceDisable(INT_SOURCE_USART_4_RECEIVE);
        /* Ignore the warning */
        (void)status;
    }

    /* Release the mutex */
   _DRV_USART_RELEASE_MUTEX(&(dObj->mutexDriverInstance));

}

void _DRV_USART2_BufferQueueTxTasks(void)
{
    /* Start by getting the buffer at the head of queue. */

    DRV_USART_BUFFER_OBJ * bufferObj;
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    bool status;

    dObj = &gDrvUSART2Obj;
    bufferObj = dObj->queueWrite;

    /* If this driver is configured for polled mode in an RTOS, the tasks
       routine would be called from another thread. We need to get the driver
       instance mutex before updating the queue. If the driver is configured for
       interrupt mode, then _DRV_USART_TAKE_MUTEX will compile to true */

    if(DRV_USART_INTERRUPT_MODE == false)
    {
        if(OSAL_MUTEX_Lock(&(dObj->mutexDriverInstance), OSAL_WAIT_FOREVER))
        {
            /* We were able to take the mutex */
        }
        else
        {
            /* The mutex acquisition timed out. Return with an
               invalid handle. This code will not execute
               if there is no RTOS. */
            return;
        }
    }

    if(bufferObj != NULL)
    {
        /* This means the queue is not empty. Check if this buffer is done */
        if(bufferObj->nCurrentBytes >= bufferObj->size)
        {
            /* This means the buffer is completed. If there
               is a callback registered with client, then
               call it */

            if((dObj->eventHandler != NULL) && (bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD))
            {
                /* Before calling the event handler, the interrupt nesting
                   counter is incremented. This will allow driver routine that
                   are called from the event handler to know the interrupt
                   nesting level. Events are only generated for buffers that
                   were submitted using the buffer add routine */

                dObj->interruptNestingCount ++;

                dObj->eventHandler(DRV_USART_BUFFER_EVENT_COMPLETE,
                        bufferObj->bufferHandle,
                        dObj->context);

                /* Decrement the nesting count */
                dObj->interruptNestingCount -- ;
            }

            /* Get the next buffer in the queue and deallocate
             * this buffer */

            dObj->queueWrite = bufferObj->next;
            bufferObj->inUse = false;
            dObj->queueSizeCurrentWrite --;

            /* Reset the next and previous pointers */
            bufferObj->next = NULL;
            bufferObj->previous = NULL;

            /* Reset the current head's previous pointer */
            if (dObj->queueWrite != NULL)
            {
                dObj->queueWrite->previous = NULL;
            }

            /* Check if read write mode and post semaphore */
            if(bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_READ_WRITE)
            {
                /* This means we should post the semaphore */
                _DRV_USART_SEM_POST(&(dObj->semWriteDone));

            }
        }
    }

    /* Check if the queue is still not empty and process
       the buffer */

    if(dObj->queueWrite != NULL)
    {
        bufferObj = dObj->queueWrite;

        /* Fill up the FIFO with data until the FIFO is full
           and we have data to send */
        while((!PLIB_USART_TransmitterBufferIsFull(USART_ID_4))
                && (bufferObj->nCurrentBytes < bufferObj->size ))
        {
            PLIB_USART_TransmitterByteSend(USART_ID_4, bufferObj->buffer[bufferObj->nCurrentBytes]);
            bufferObj->nCurrentBytes ++;
        }
    }
    else
    {
        /* If the queue is empty, then disable the TX interrupt */
        status = SYS_INT_SourceDisable(INT_SOURCE_USART_4_TRANSMIT);
        /* Ignore the warning */
        (void)status;
    }

    /* Release the mutex */
    _DRV_USART_RELEASE_MUTEX(&(dObj->mutexDriverInstance));

}

void _DRV_USART2_BufferQueueErrorTasks(void)
{
    DRV_USART_OBJ *dObj = (DRV_USART_OBJ*)NULL;
    DRV_USART_BUFFER_OBJ * bufferObj;
    bool mutexGrabbed  = true;
    bool status = false;

    dObj = &gDrvUSART2Obj;

    /* USART driver will take care that TX erros( like overflow etc) are not
     * going to occur at any time based on checks before write.
     * So, only RX errors are to be handled/reported */

    /* Get the RX buffer at the head */
    bufferObj = dObj->queueRead;

    /* If this driver is configured for polled mode in an RTOS, the tasks
     * routine would be called from another thread. We need to get the driver
     * instance mutex before updating the queue. If the driver is configured for
     * interrupt mode, then OSAL_MUTEX_Lock will compile to true */

    if(DRV_USART_INTERRUPT_MODE == false)
    {
        if(!OSAL_MUTEX_Lock(&(dObj->mutexDriverInstance), OSAL_WAIT_FOREVER))
        {
            /* The mutex acquisition timed out.
             * This code will not execute if there is no RTOS. */
            mutexGrabbed = false;
        }
    }

    /* mutexGrabbed will always be true for non-RTOS case.
     * Will be false when mutex aquisition timed out in RTOS mode */
    if(true == mutexGrabbed)
    {
        if(bufferObj != NULL)
        {
            /* Get the USART errors */
            dObj->error = PLIB_USART_ErrorsGet(USART_ID_4);

	        /* Clear error condition */
            _DRV_USART2_ErrorConditionClear();

            /* Call event handler in Buffer queue request only.
             * If it's a File i/o request then DRV_USART_Read call itself
             * will return with error result */
            if((dObj->eventHandler != NULL) && (bufferObj->flags & DRV_USART_BUFFER_OBJ_FLAG_BUFFER_ADD))
            {
                /* Call the event handler with buffer event error state */
                dObj->eventHandler(DRV_USART_BUFFER_EVENT_ERROR,
                        bufferObj->bufferHandle,
                        dObj->context);
            }

            /* Get the next buffer in the queue and deallocate
             * this buffer */

            dObj->queueRead = bufferObj->next;
            bufferObj->inUse = false;
            dObj->queueSizeCurrentRead --;

            /* Reset the next and previous pointers */
            bufferObj->next = NULL;
            bufferObj->previous = NULL;

            /* If queue head is not null, update buffer pointer parameters*/
            if (dObj->queueRead != NULL)
            {
                /* Reset the updated head's previous pointer */
                dObj->queueRead->previous = NULL;
            }
            else
            {
                /* Queue is empty disable the RX interrupt */
                status = SYS_INT_SourceDisable(INT_SOURCE_USART_4_RECEIVE);

                /* Ignore the warning */
                (void)status;
            }
        }
        else
        {
            /* There is no buffer in the queue.
             * Flush the RX to clear the error condition */
            _DRV_USART2_ErrorConditionClear();
        }
    }

    /* Release the mutex in RTOS environment*/
    _DRV_USART_RELEASE_MUTEX(&(dObj->mutexDriverInstance));
}

void _DRV_USART2_ErrorConditionClear()
{
    uint8_t dummyData = 0u;
    /* RX length = (FIFO level + RX register) */
    uint8_t RXlength = _DRV_USART_RX_DEPTH;

    /* If it's a overrun error then clear it to flush FIFO */
    if(USART_ERROR_RECEIVER_OVERRUN & PLIB_USART_ErrorsGet(USART_ID_4))
    {
        PLIB_USART_ReceiverOverrunErrorClear(USART_ID_4);
    }

    /* Read existing error bytes from FIFO to clear parity and framing error flags*/
    while( (USART_ERROR_PARITY | USART_ERROR_FRAMING) & PLIB_USART_ErrorsGet(USART_ID_4) )
    {
        dummyData = PLIB_USART_ReceiverByteReceive(USART_ID_4);
        RXlength--;

        /* Try to flush error bytes for one full FIFO and exit instead of
         * blocking here if more error bytes are received*/
        if(0u == RXlength)
        {
            break;
        }
    }

    /* Ignore the warning */
    (void)dummyData;

    /* Clear error interrupt flag */
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_4_ERROR);

    /* Clear up the receive interrupt flag so that RX interrupt is not
     * triggered for error bytes*/
    SYS_INT_SourceStatusClear(INT_SOURCE_USART_4_RECEIVE);
}



DRV_USART_BAUD_SET_RESULT DRV_USART2_BaudSet(uint32_t baud)
{
    uint32_t clockSource;
    int32_t brgValueLow=0;
    int32_t brgValueHigh=0;
    DRV_USART_BAUD_SET_RESULT retVal = DRV_USART_BAUD_SET_SUCCESS;
#if defined (PLIB_USART_ExistsModuleBusyStatus)
    bool isEnabled = false;
#endif

    /* Making this function thread safe */
    if(OSAL_MUTEX_Lock(&(gDrvUSART2Obj.mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
    {
        /* We were able to take the mutex */
    }
    else
    {
        /* The mutex timed out */
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Hardware instance mutex time out in DRV_USART_BaudSet() function");
        return(DRV_USART_BAUD_SET_ERROR);
    }

    /* Get the USART clock source value*/
    clockSource = SYS_CLK_PeripheralFrequencyGet ( CLK_BUS_PERIPHERAL_2 );

    /* Calculate low and high baud values */
    brgValueLow  = ( (clockSource/baud) >> 4 ) - 1;
    brgValueHigh = ( (clockSource/baud) >> 2 ) - 1;

#if defined (PLIB_USART_ExistsModuleBusyStatus)
        isEnabled = PLIB_USART_ModuleIsBusy (USART_ID_4);
        if (isEnabled)
        {
            PLIB_USART_Disable (USART_ID_4);
            while (PLIB_USART_ModuleIsBusy (USART_ID_4));
        }
#endif

    /* Check if the baud value can be set with high baud settings */
    if ((brgValueHigh >= 0) && (brgValueHigh <= UINT16_MAX))
    {
        PLIB_USART_BaudRateHighEnable(USART_ID_4);
        PLIB_USART_BaudRateHighSet(USART_ID_4,clockSource,baud);
    }

    /* Check if the baud value can be set with low baud settings */
    else if ((brgValueLow >= 0) && (brgValueLow <= UINT16_MAX))
    {
        PLIB_USART_BaudRateHighDisable(USART_ID_4);
        PLIB_USART_BaudRateSet(USART_ID_4, clockSource, baud);
    }
    else
    {
            retVal = DRV_USART_BAUD_SET_ERROR;
    }

#if defined (PLIB_USART_ExistsModuleBusyStatus)
    if (isEnabled)
    {
        PLIB_USART_Enable (USART_ID_4);
    }
#endif
    /* Unlock Mutex */
    OSAL_MUTEX_Unlock(&(gDrvUSART2Obj.mutexDriverInstance));

    return retVal;
}


DRV_USART_LINE_CONTROL_SET_RESULT DRV_USART2_LineControlSet(DRV_USART_LINE_CONTROL lineControlMode)
{
#if defined (PLIB_USART_ExistsModuleBusyStatus)
    bool isEnabled = false;
#endif
    /* Making this function thread safe */
    if(OSAL_MUTEX_Lock(&(gDrvUSART2Obj.mutexDriverInstance), OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
    {
        /* We were able to take the mutex */
    }
    else
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_DEBUG, "\r\nUSART Driver: Hardware Instance Mutex time out in DRV_USART_LineControlSet() function");
        return DRV_USART_LINE_CONTROL_SET_ERROR;
    }
#if defined (PLIB_USART_ExistsModuleBusyStatus)
        isEnabled = PLIB_USART_ModuleIsBusy (USART_ID_4);
        if (isEnabled)
        {
            PLIB_USART_Disable (USART_ID_4);
            while (PLIB_USART_ModuleIsBusy (USART_ID_4));
        }
#endif

    /* Set the Line Control Mode */
    PLIB_USART_LineControlModeSelect(USART_ID_4, lineControlMode);

#if defined (PLIB_USART_ExistsModuleBusyStatus)
        if (isEnabled)
        {
            PLIB_USART_Enable (USART_ID_4);
        }
#endif
    OSAL_MUTEX_Unlock(&(gDrvUSART2Obj.mutexDriverInstance));

    /* Return success */
    return(DRV_USART_LINE_CONTROL_SET_SUCCESS);
}

/*******************************************************************************
 End of File
*/
