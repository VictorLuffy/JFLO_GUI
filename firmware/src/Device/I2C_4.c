/* ************************************************************************** */
/** @file [I2C_4.c]
 *  @brief {control read/write data via I2C4, manage share resource between multiple 
 * peripherals, and notify when complete a transaction}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */



/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>

#include "FreeRTOS.h"
#include "semphr.h"

#include "system_config.h"
#include "system_definitions.h"
#include "driver/i2c/drv_i2c.h"

#include "I2C_4.h"
#include "AlarmInterface.h"
#include "ApplicationDefinition.h"


/** @brief I2C index use for Air flow sensor map with Harmony configuration */
#define I2C_4_INDEX                 3          

/** @brief I2C flow sensor handle */
DRV_HANDLE s_I2C4Handle = DRV_HANDLE_INVALID;

/** @brief clock cycles for recovery */
#define I2C_RECOVER_NUM_CLOCKS      10U
/** @brief clock frequency for recovery */
#define I2C_RECOVER_CLOCK_FREQ      50000U
/** @brief delay to create clock for recovery */
#define I2C_RECOVER_CLOCK_DELAY_US  (1000000U / (2U * I2C_RECOVER_CLOCK_FREQ))

/** @brief I2C flow sensor buffer handle*/
static DRV_I2C_BUFFER_HANDLE s_I2C4BufferHandle;

/** @brief I2C buffer communication status*/
static DRV_I2C_BUFFER_EVENT s_I2C4CommStatus;

/** @brief task notification flag, used for synchronization communication when a 
 * I2C transaction completed*/
static TaskHandle_t s_I2C4NotityFlag = NULL;

/** @brief mutex to protect I2C 4 sharing */
static SemaphoreHandle_t s_I2C4Mutex = NULL;

/** @brief Flag indicate I2C4 has error
 * true mean error happened
 * false mean no error */
static E_DeviceErrorState s_I2C4Error = eDeviceNoError;

static uint8_t s_I2C4RecoverCounter = 0;


/** @brief internal functions declaration */
static void I2C4_ReportError();

static uint32_t ReadCoreTimer()
{
    volatile uint32_t timer;

    // get the count reg
    asm volatile("mfc0   %0, $9" : "=r"(timer));

    return(timer);
}
#define GetSystemClock() (SYS_CLK_FREQ)
#define us_SCALE   (GetSystemClock()/2000000)
static void DelayUs(unsigned long int usDelay )
{
      register unsigned int startCnt = ReadCoreTimer();
      register unsigned int waitCnt = usDelay * us_SCALE;
 
      while( ReadCoreTimer() - startCnt < waitCnt );
}

bool I2C4_CheckTransferStatus(DRV_I2C_BUFFER_HANDLE drvBufferHandle)
{
    if (DRV_I2C_BUFFER_EVENT_COMPLETE == DRV_I2C_TransferStatusGet(s_I2C4Handle, drvBufferHandle))
    {
        return true; //ok
    }
    else
    {
        return false; //fail
    }
}

/** @brief Function to initialize I2C4, used to communicate with multiple devices\
 * such as: BME280, ADXL345, and Audio Codec. including open I2C port, setting 
 * callback function, initializing memories before operation
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void I2C4_Initialize() {
    
//    DRV_I2C_Bus_Clear(sysObj.drvI2C3);
    if (s_I2C4Handle == DRV_HANDLE_INVALID) {
        s_I2C4Handle = DRV_I2C_Open(I2C_4_INDEX, DRV_IO_INTENT_READWRITE);
        /* event-handler set up receive callback from DRV_I2C_Tasks */
        //DRV_I2C_BufferEventHandlerSet(s_I2C4Handle, I2C4_StatusCallback, s_I2C4CommStatus);
    }

    //create Mutex
    s_I2C4Mutex = xSemaphoreCreateMutex();
    xSemaphoreGive(s_I2C4Mutex);
    
    //reset variables
    s_I2C4Error = eDeviceNoError;
}

/** @brief This function reset I2C communicate by Flushing Queue
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
  */
void I2C4_ResetComunicate() 
{
    SYS_PRINT("\nI2C4 error >>> Reset I2C4\n");

    I2C4STATbits.IWCOL = 0;
    I2C4STATbits.BCL = 0;
    I2C4STATbits.I2COV = 0;
    
    I2C4CONbits.I2CEN = 0;
    TRISG = SYS_PORT_G_TRIS & 0xFEFF;
    TRISGbits.TRISG8 = 0;
    TRISGbits.TRISG7 = 1;
    int i;
    do
    {
        for ( i = 0U; i < I2C_RECOVER_NUM_CLOCKS; ++i)
        {
            DelayUs (I2C_RECOVER_CLOCK_DELAY_US);
            LATGbits.LATG8 = 1;
            DelayUs (I2C_RECOVER_CLOCK_DELAY_US);
            LATGbits.LATG8 = 0;
        }
        
        s_I2C4RecoverCounter++;
        if (s_I2C4RecoverCounter > 3)
        {
            break;
        }
        
    }while(!PORTGbits.RG7);
    
    s_I2C4RecoverCounter = 0;
    I2C4CONbits.I2CEN = 1;
    I2C4_Initialize();
    DRV_I2C_QueueFlush(s_I2C4Handle); 
}


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
bool I2C4_Write(uint16_t address, void *writeBuffer, size_t size, uint32_t maxWait) 
{
    //check error
    //    if (s_I2C4Error != eDeviceNoError) {
    //        //report error
    //        I2C4_ReportError();
    //        return false;
    //    }
    
    //take resource 
    if (xSemaphoreTake(s_I2C4Mutex, maxWait) == pdFALSE) {
        //report error
        //I2C4_ReportError();
        return false;
    }

    s_I2C4BufferHandle = DRV_I2C_Transmit(s_I2C4Handle, address,(void*)  writeBuffer, size, NULL);
    uint8_t timeout = 0;
    if (s_I2C4BufferHandle == DRV_I2C_BUFFER_HANDLE_INVALID) 
    {
        xSemaphoreGive(s_I2C4Mutex);
        I2C4_ResetComunicate();
        return false; //error
    } 
    else 
    {
        //vTaskDelay(1);
        DelayUs(100);
        while (!I2C4_CheckTransferStatus(s_I2C4BufferHandle))
        {
            vTaskDelay(1);
            timeout++;
            if(timeout >= maxWait){
                xSemaphoreGive(s_I2C4Mutex);
                //I2C4_ResetComunicate();
                return false;
            }
        }
    }  
    xSemaphoreGive(s_I2C4Mutex);
    return true;
}

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
bool I2C4_Read(uint16_t address, void *readBuffer, size_t size, uint32_t maxWait) 
{
    //check error
//    if (s_I2C4Error != eDeviceNoError) {
//        //report error
//        I2C4_ReportError();
//        return false;
//    }
    
    //take resource 
    if (xSemaphoreTake(s_I2C4Mutex, maxWait) == pdFALSE) {
        //report error
        //I2C4_ReportError();
        return false;
    }
    
    s_I2C4BufferHandle = DRV_I2C_Receive(s_I2C4Handle, address,(void*)  readBuffer, size, NULL);
    uint8_t timeout = 0;
    if (s_I2C4BufferHandle == DRV_I2C_BUFFER_HANDLE_INVALID)
    {
        xSemaphoreGive(s_I2C4Mutex);
        I2C4_ResetComunicate();
        return false; //error
    } 
    else 
    {
        //vTaskDelay(1);
        DelayUs(300);
        while (!I2C4_CheckTransferStatus(s_I2C4BufferHandle))
        {
            vTaskDelay(1);
            timeout++;
            if(timeout >= maxWait)
            {
                xSemaphoreGive(s_I2C4Mutex);
                //I2C4_ResetComunicate();
                return false;
            }
        }
    } 
     
    xSemaphoreGive(s_I2C4Mutex);
    return true;
}


/** @brief report error if occur during communication via I2C4, may be send event
 * to Alarm task
 *  @param [in]  None 
 *  @param [out]  None
 *  @return None
 */
void I2C4_ReportError() {
    //check whether an error is detected, then send event to ALarm task
    if (s_I2C4Error == eDeviceErrorDetected) {
        //send event to alarm task
        alarmInterface_SendEvent(eI2C4ErrorAlarm, eActive, eHighPriority, 0);
        SYS_PRINT("\n error at: I2C4_ReportError");
        //change state
        s_I2C4Error = eDeviceErrorReported;
    }
}





/* *****************************************************************************
 End of File
 */
