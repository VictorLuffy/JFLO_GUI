/* ************************************************************************** */
/** @file [SPI_3.c]
 *  @brief { This file contains all methods to read/ write via SPI3 serial port
 * compatible with RTOS including open port, detect error, report error, and task
 * synchronization }
 *  @author {nguyen truong, bui phuoc}
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */

#include "FreeRTOS.h"
#include "semphr.h"
#include "system_config.h"
#include "system_definitions.h"

#include "SPI_3.h"
#include "ApplicationDefinition.h"
#include "AlarmInterface.h"


/** @brief SPI3 index on Harmony configuration */
#define SPI3_INDEX  0

/** @brief Turn on CHIP SELECT pin. On the PIC32MZ MCU, the SS pin on SPI module
 should be turn on by software, hardware does not automatically control that pin
 when running on Standard SPI operation mode */
#define SPI3_CS_SELECT()    MOTOR_SPI_SSOn()//DigitalOutput_SetState(eGPIO_OUT_MOTOR_SPI_SS, eGPIO_OUT_HIGH)

/** @brief Turn off CHIP SELECT pin. On the PIC32MZ MCU, the SS pin on SPI module
 should be turn of by software, hardware does not automatically control that pin
 when running on Standard SPI operation mode */
#define SPI3_CS_DESELECT()  MOTOR_SPI_SSOff()//DigitalOutput_SetState(eGPIO_OUT_MOTOR_SPI_SS, eGPIO_OUT_LOW)


/** @brief local functions declaration */
static void SPI3_ReportError();
static bool SPI3_WaitTilDone(uint32_t maxTimeWait);
static bool SPI3_Transmit(uint8_t* txbuffer, size_t length);
static bool SPI3_TransmitReceive(uint8_t* txbuffer, size_t txlen, uint8_t* rxbuffer, size_t rxlen);
static void SPI3_BufferEventHandlerStart(DRV_SPI_BUFFER_EVENT buffEvent,
        DRV_SPI_BUFFER_HANDLE hBufferEvent,
        void* context);
static void SPI3_BufferEventHandlerEnd(DRV_SPI_BUFFER_EVENT buffEvent,
        DRV_SPI_BUFFER_HANDLE hBufferEvent,
        void* context);
static void SPI3_BufferEventHandler(DRV_SPI_BUFFER_EVENT buffEvent,
        DRV_SPI_BUFFER_HANDLE hBufferEvent,
        void* context);


/** @brief Flag indicate SPI3 has error
 * true mean error happened
 * false mean no error */
static E_DeviceErrorState s_SPI3Error = eDeviceNoError;

/** @brief SPI3 handle, use to transmit/ receive data on SPI3 port */
static DRV_HANDLE s_SPI3Handle = DRV_HANDLE_INVALID;

/** @brief  SPI3 write buffer handle, that may useful when register callback 
 function when transmit data via SPI3 port */
static DRV_SPI_BUFFER_HANDLE s_SPI3_BufferHandle;

/** @brief task notification flag, used for synchronization communication when a 
 * SPI3 transaction completed*/
static TaskHandle_t s_SPI3NotityFlag = NULL;


/** @brief SPI3 configuration object, include SPI speed, call back function when 
 start SPI transaction or finish a SPI transaction */
const DRV_SPI_CLIENT_DATA spi3ConfigurationData = {
    .baudRate = 1000000,
    .operationStarting = SPI3_BufferEventHandlerStart,
    .operationEnded = SPI3_BufferEventHandlerEnd,
};

/** @brief Function to initialize SPI3, used to control motor driver DRV8308 
 * The function first open SPI port, then configure some operation setting like 
 * baud-rate, start call back function, end call back function ...Finally initialize
 * memories before operation
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void SPI3_Initialize() {
    if (s_SPI3Handle == DRV_HANDLE_INVALID) {
        s_SPI3Handle = DRV_SPI_Open(SPI3_INDEX, DRV_IO_INTENT_READWRITE);
        DRV_SPI_ClientConfigure(s_SPI3Handle, &spi3ConfigurationData);
    }

    s_SPI3Error = eDeviceNoError;
}

//bool SPI3_CheckTransferStatus(DRV_SPI_BUFFER_HANDLE drvBufferHandle) 
//{
////    if (DRV_I2C_BUFFER_EVENT_COMPLETE & DRV_I2C_TransferStatusGet(gs_I2C2_Handle, drvBufferHandle))
////    {
////        return true;//ok
////    } 
////    else 
////    {
////        return false;//fail
////    }
//    
//  return PLIB_SPI_IsBusy(SPI_ID_3);
//}


bool SPI3_CheckTransferStatus(DRV_SPI_BUFFER_HANDLE drvBufferHandle)
{
    if (DRV_SPI_BUFFER_EVENT_COMPLETE & DRV_SPI_BufferStatus(drvBufferHandle))
    {
        return true; //ok
    }
    else
    {
        return false; //fail
    }
}




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
bool SPI3_Write(void* writeBuffer, size_t size, uint32_t maxWait) {
//    if (s_SPI3Error != eDeviceNoError) {
//        SPI3_ReportError();
//        return false;
//    }
//
//    TickType_t startTime = xTaskGetTickCount();
//
//    //transmit data and check result
//    if (SPI3_Transmit(writeBuffer, size) == false) {
//        SPI3_ReportError();
//        return false; //error
//    }
//
//    //get remain time
//    TickType_t remainTime = maxWait - (xTaskGetTickCount() - startTime);
//
//    //wait for complete
//    if (SPI3_WaitTilDone(remainTime) == false) {
//        SPI3_ReportError();
//        return false;
//    }
//
//    return true;
  
      DRV_SPI_BUFFER_HANDLE SPI3_BufferHandle = DRV_SPI_BufferAddWrite(s_SPI3Handle, writeBuffer, size, SPI3_BufferEventHandler, NULL);
      uint8_t timeout = 0;
    //check result
    if (SPI3_BufferHandle == DRV_SPI_BUFFER_HANDLE_INVALID) {

        return false; //error
    } 
    else {
      
        vTaskDelay(1);//2
        while (!SPI3_CheckTransferStatus(SPI3_BufferHandle))
        {
            vTaskDelay(1);
            timeout++;
            if(timeout >= 10){
                return false;
            }
        }
        return true;
    }  
}

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
bool SPI3_WriteRead(uint8_t* txbuffer, size_t txlen, uint8_t* rxbuffer, size_t rxlen, uint32_t maxWait) {
//    if (s_SPI3Error != eDeviceNoError) {
//        SPI3_ReportError();
//        return false;
//    }
//
//    TickType_t startTime = xTaskGetTickCount();
//
//    //transmit data and check result
//    if (SPI3_TransmitReceive(txbuffer, txlen, rxbuffer, rxlen) == false) {
//        SPI3_ReportError();
//        return false; //error
//    }
//
//    //get remain time
//    TickType_t remainTime = maxWait - (xTaskGetTickCount() - startTime);
//
//    //wait for complete
//    if (SPI3_WaitTilDone(remainTime) == false) {
//        SPI3_ReportError();
//        return false;
//    }
//
//    return true;
}

/** @brief Transmit a packet data via SPI3, including register a call-back function
 * when the communication is done
 *  @param [in]     uint8_t*txbuffer    pointer to data packet need to transmit
 *                  size_t length       size of data packet 
 *  @param [out]  None
 *  @return None
 *  @retval true transmit data success
 *  @retval false transmit data failed
 */
bool SPI3_Transmit(uint8_t* txbuffer, size_t length) {
    /* Add the buffer to transmit */
    s_SPI3_BufferHandle = DRV_SPI_BufferAddWrite(s_SPI3Handle, txbuffer, length, SPI3_BufferEventHandler, NULL);

    if (s_SPI3_BufferHandle != DRV_SPI_BUFFER_HANDLE_INVALID) {
        /* Store the handle of the calling task. */
        s_SPI3NotityFlag = xTaskGetCurrentTaskHandle();
        //clear error 
        s_SPI3Error = eDeviceNoError;
        return true;
    } else {
        //set error flag
        s_SPI3Error = eDeviceErrorDetected;
        return false;
    }
}

/** @brief Transmit and then receive data via SPI3, including register a call-back 
 * function when the communication is done
 *  @param [in]     uint8_t* txbuffer   pointer to buffer need to transmit  
 *                  size_t txlen        number of byte need to transmit
 *                  size_t rxlen        number of byte need to be read
 *  @param [out]    uint8_t* rxbuffer   pointer to buffer to store rx data
 *  @return None
 *  @retval true receive data success
 *  @retval false receive data failed
 */
bool SPI3_TransmitReceive(uint8_t* txbuffer, size_t txlen, uint8_t* rxbuffer, size_t rxlen) {
    /* Add the buffer pointer to read the data from slave...*/
    s_SPI3_BufferHandle = DRV_SPI_BufferAddWriteRead(s_SPI3Handle,
            txbuffer, txlen,
            rxbuffer, rxlen, SPI3_BufferEventHandler, NULL);

    if (s_SPI3_BufferHandle != DRV_SPI_BUFFER_HANDLE_INVALID) {
        /* Store the handle of the calling task. */
        s_SPI3NotityFlag = xTaskGetCurrentTaskHandle();
        //clear error 
        s_SPI3Error = eDeviceNoError;
        return true;
    } else {
        //set error flag
        s_SPI3Error = eDeviceErrorDetected;
        return false;
    }
}

/** @brief wait for current communication on SPI3 done by just waiting a semaphore.
 * When start a communication, the semaphore is taken. when communication is done,
 * the semaphore is given by callback function 
 *  @param [in]  None 
 *  @param [out]  None
 *  @return None
 *  @retval true the communication is done before maximum waiting time --> OK
 *  @retval false over maximum time but the communication is not done yet --> ERROR
 */
bool SPI3_WaitTilDone(uint32_t maxTimeWait) {
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(maxTimeWait);
    /* Wait to be notified that the transmission is complete.  Note the first
    parameter is pdTRUE, which has the effect of clearing the task's notification
    value back to 0, making the notification value act like a binary (rather than
    a counting) semaphore.  */
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE,
            xMaxBlockTime);

    if (ulNotificationValue == 1) {
        s_SPI3Error = eDeviceNoError;
        /* The transmission ended as expected. */
        return true;
    } else {
        /* The call to ulTaskNotifyTake() timed out. */
        s_SPI3Error = eDeviceErrorDetected;
        return false;
    }
}

/** @brief Call back function before SPI start clock data on bus.  This function
 * should register on configuration step, after initializing and just call 1 time
 * In case of SPI3, this function is useful to turn on Slave Select pin
 *  @param [in]  DRV_SPI_BUFFER_EVENT buffEvent     no used on this case 
 *               DRV_SPI_BUFFER_HANDLE hBufferEvent no used on this case
 *               void* context                      no used on this case
 *  @param [out]  None
 *  @return None
 */
void SPI3_BufferEventHandlerStart(DRV_SPI_BUFFER_EVENT buffEvent,
        DRV_SPI_BUFFER_HANDLE hBufferEvent,
        void* context) {
    SPI3_CS_SELECT();
}

/** @brief Call back function after SPI finish transaction on bus.  This function
 * should register on configuration step, after initializing and just call 1 time
 * In case of SPI3, this function is useful to turn off Slave Select pin
 *  @param [in]  DRV_SPI_BUFFER_EVENT buffEvent     no used on this case 
 *               DRV_SPI_BUFFER_HANDLE hBufferEvent no used on this case
 *               void* context                      no used on this case
 *  @param [out]  None
 *  @return None
 */
void SPI3_BufferEventHandlerEnd(DRV_SPI_BUFFER_EVENT buffEvent,
        DRV_SPI_BUFFER_HANDLE hBufferEvent,
        void* context) {
    SPI3_CS_DESELECT();
}

/** @brief Call back function when an action (transmit or receive) on SPI bus is done
 * This function should register on each transaction to make it to be called when
 * that transaction is done
 * In case of SPI3, this function is used to synchronize a transaction with the task
 * running it
 *  @param [in]  DRV_SPI_BUFFER_EVENT buffEvent     no used on this case 
 *               DRV_SPI_BUFFER_HANDLE hBufferEvent no used on this case
 *               void* context                      no used on this case
 *  @param [out]  None
 *  @return None
 */
void SPI3_BufferEventHandler(DRV_SPI_BUFFER_EVENT buffEvent,
        DRV_SPI_BUFFER_HANDLE hBufferEvent,
        void* context) {
//    signed portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    //enter critical code
    UBaseType_t uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();

    switch (buffEvent) {
            /* Buffer event is completed successfully. Data is received */
        case DRV_SPI_BUFFER_EVENT_COMPLETE:
            break;
            /* Buffer event has some error */
        case DRV_SPI_BUFFER_EVENT_ERROR:
        {
            s_SPI3Error = eDeviceErrorDetected;
        }
            break;
        default:
            break;
    }

    /* Notify the task that the transmission is complete. */
//    vTaskNotifyGiveFromISR(s_SPI3NotityFlag, &xHigherPriorityTaskWoken);

    /* There are no transmissions in progress, so no tasks to notify. */
    s_SPI3NotityFlag = NULL;

    //exit critical code
    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}

/** @brief Report error if occur during communication via SPI3, may be send event
 * to Alarm task
 *  @param [in]  None 
 *  @param [out]  None
 *  @return None
 */
void SPI3_ReportError() {
    //check whether an error is detected, then send event to ALarm task
    if (s_SPI3Error == eDeviceErrorDetected) {
        //send event to alarm task
        alarmInterface_SendEvent(eSPI3ErrorAlarm, eActive, eHighPriority, 0);
        //change state
        s_SPI3Error = eDeviceErrorReported;
    }
}


/* *****************************************************************************
 End of File
 */
