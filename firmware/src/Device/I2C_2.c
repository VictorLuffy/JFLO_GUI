/* ************************************************************************** */
/** @file [I2C_2.c]
 *  @brief {control read/write data via I2C2, manage share resource between multiple 
 * peripherals, and notify when complete a transaction}
 *  @author {bui phuoc, nguyen truong}
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

#include "I2C_2.h"
#include "AlarmInterface.h"
#include "ApplicationDefinition.h"



extern SYSTEM_OBJECTS sysObj;

extern DRV_I2C_INIT drvI2C1InitData;

/** @brief clock cycles for recovery */
#define I2C_RECOVER_NUM_CLOCKS      10U
/** @brief clock frequency for recovery */
#define I2C_RECOVER_CLOCK_FREQ      50000U
/** @brief delay to create clock for recovery */
#define I2C_RECOVER_CLOCK_DELAY_US  (1000000U / (2U * I2C_RECOVER_CLOCK_FREQ))

/** @brief I2C index use for Smart Battery and Charger map with Harmony configuration */
#define I2C_2_INDEX                 1          

/** @brief I2C handle */
static DRV_HANDLE s_I2C2Handle = DRV_HANDLE_INVALID;

/** @brief I2C buffer handle*/
static DRV_I2C_BUFFER_HANDLE s_I2C2BufferHandle;

/** @brief I2C buffer communication status*/
static DRV_I2C_BUFFER_EVENT s_I2C2CommStatus;

/** @brief task notification flag, used for synchronization communication when a 
 * I2C transaction completed*/
static TaskHandle_t s_I2C2NotityFlag = NULL;

/** @brief MUTEX to protect I2C2 sharing */
static SemaphoreHandle_t s_I2C2Mutex = NULL;

/** @brief Flag indicate I2C2 has error
 * true mean error happened
 * false mean no error */
static E_DeviceErrorState s_I2C2Error = eDeviceNoError;

static uint8_t s_I2C2ErrCount = 0;

/** @brief internal functions declaration */
static void I2C2_ReportError();

/** @brief I2C2 recovery counter*/
static uint8_t s_I2C2RecoverCounter = 0;

/** @brief Function to initialize I2C2, used to read data from Air Flow Sensor 
 * and O2 Flow Sensor, including open I2C port, setting callback function, 
 * initializing memories before operation
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void I2C2_Init() {
    if (s_I2C2Handle == DRV_HANDLE_INVALID) {
        s_I2C2Handle = DRV_I2C_Open(I2C_2_INDEX, DRV_IO_INTENT_EXCLUSIVE/*DRV_IO_INTENT_READWRITE*/);
        /* event-handler set up receive callback from DRV_I2C_Tasks */
        //DRV_I2C_BufferEventHandlerSet(s_I2C2Handle, I2C2_StatusCallback, s_I2C2CommStatus);
    }

    //create MUTEX
    if(s_I2C2Mutex == NULL){
      s_I2C2Mutex = xSemaphoreCreateMutex();
      xSemaphoreGive(s_I2C2Mutex);
    }
    
    //reset variables
    s_I2C2Error = eDeviceNoError;
}

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

/** @brief This function reset I2C communicate by Flushing Queue
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
  */
void I2C2_ResetComunicate() 
{
    SYS_PRINT("\nI2C2 error >>> Reset I2C2\n");
    I2C2STATbits.IWCOL = 0;
    I2C2STATbits.BCL = 0;
    I2C2STATbits.I2COV = 0;
    
    I2C2CONbits.I2CEN = 0;
    TRISA = SYS_PORT_G_TRIS & 0xFEFF;
    // SCL1 as output
    TRISAbits.TRISA2 = 0;
    
    // SDA1 as input
    TRISAbits.TRISA3 = 1;
    int i;
    do
    {
        for ( i = 0U; i < I2C_RECOVER_NUM_CLOCKS; ++i)
        {
            DelayUs (I2C_RECOVER_CLOCK_DELAY_US);
            LATAbits.LATA2 = 1;
            DelayUs (I2C_RECOVER_CLOCK_DELAY_US);
            LATAbits.LATA2 = 0;
        }
        
        s_I2C2RecoverCounter++;
        if (s_I2C2RecoverCounter > 3)
        {
            break;
        }
        
    }while(!PORTAbits.RA3);
    
    s_I2C2RecoverCounter = 0;
    I2C2CONbits.I2CEN = 1;
    I2C2_Init();
    DRV_I2C_QueueFlush(s_I2C2Handle);
}


bool I2C2_CheckTransferStatus(DRV_I2C_BUFFER_HANDLE drvBufferHandle)
{
    if (DRV_I2C_BUFFER_EVENT_COMPLETE == DRV_I2C_TransferStatusGet(s_I2C2Handle, drvBufferHandle))
    {
        return true; //ok
    }
    else
    {
        return false; //fail
    }
}


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
bool I2C2_Write(uint16_t address, void *writeBuffer, size_t size, uint32_t maxWait) {
    
    // DEBUG
//    int i;
//    SYS_PRINT("\nAddress...: %d", address);
//    for ( i = 0; i < size; i++)
//    {
//        SYS_PRINT("   %d   ", (int)writeBuffer[i]);
//    }
//    SYS_PRINT("\n\n");
    //check error
    if (s_I2C2Error != eDeviceNoError) {
        //report error
        //SYS_PRINT("report error\n");
        I2C2_ReportError();
        //return false;
    }
    static uint8_t s_errCnt = 0;
  
    uint32_t maxWaitTime = maxWait;
    DRV_I2C_BUFFER_HANDLE s_I2C2BufferHandle; 
    s_I2C2BufferHandle = DRV_I2C_Transmit(s_I2C2Handle,
                address,
                (void*) writeBuffer,
                size,
                NULL);

    //check result
    if (s_I2C2BufferHandle == DRV_I2C_BUFFER_HANDLE_INVALID) {
        
        s_I2C2ErrCount++;// increase count
        if (s_I2C2ErrCount >= 5)
        {
            //set error flag
            s_I2C2Error = eDeviceErrorDetected;
        }
        SYS_PRINT("I2C2: DRV_I2C_BUFF_Transmit_INVALID\n");
        I2C2_ResetComunicate();
        return false; //error
    } 
    else {
      s_I2C2ErrCount = 0; //clear count
      vTaskDelay(1);
      while (!I2C2_CheckTransferStatus(s_I2C2BufferHandle))
      {
          vTaskDelay(1);
          maxWaitTime--;
          if(maxWaitTime == 0)
          {
              //I2C2_ResetComunicate();
              //s_errCnt++; if(s_errCnt >= 5){I2C2_ResetComunicate(); s_errCnt = 0;} 
              return false;
          }
      }
      //SYS_PRINT("I2C2timeoutW_%d\n", /*maxWait - */maxWaitTime);
      return true;
    }
    
}


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
bool I2C2_Read(uint16_t address, void *readBuffer, size_t size, uint32_t maxWait)
{
    //check error
    if (s_I2C2Error != eDeviceNoError) {
        //report error
        I2C2_ReportError();
        //return false;
    }
  
    uint32_t maxWaitTime = maxWait;
    DRV_I2C_BUFFER_HANDLE s_I2C2BufferHandle; 
    s_I2C2BufferHandle = DRV_I2C_Receive(s_I2C2Handle,
            address,
            (void*) readBuffer,
            size,
            NULL
            );

    //check result
    if (s_I2C2BufferHandle == DRV_I2C_BUFFER_HANDLE_INVALID) {
        s_I2C2ErrCount++;// increase count
        if (s_I2C2ErrCount >= 5)
        {
            //set error flag
            s_I2C2Error = eDeviceErrorDetected;
        }
        SYS_PRINT("I2C2: DRV_I2C_BUFF_Receive_INVALID\n");
        //SYS_PRINT("%X\n", s_I2C2Handle);
        I2C2_ResetComunicate();
        return false;
    } 
    else {
        s_I2C2ErrCount = 0; //clear count
        vTaskDelay(1);
        while (!I2C2_CheckTransferStatus(s_I2C2BufferHandle))
        {
            vTaskDelay(1);
            maxWaitTime--;
            if(maxWaitTime == 0)
            {            
                //Disable I2C and then Enable to reset I2C module
                //I2C2_ResetComunicate();
                DRV_I2C_BUFFER_EVENT i2cstatus;
                i2cstatus = DRV_I2C_TransferStatusGet(s_I2C2Handle, s_I2C2BufferHandle);
                SYS_PRINT("I2C_2 StatusGet %d\n", i2cstatus);
                return false;
            }
        }
        //SYS_PRINT("I2C2timeoutR_%d\n", /*maxWait - */maxWaitTime);
        return true;
    }
    
    
}



/** @brief report error if occur during communication via I2C2, may be send event
 * to Alarm task
 *  @param [in]  None 
 *  @param [out]  None
 *  @return None
 */
void I2C2_ReportError() {
    //check whether an error is detected, then send event to ALarm task
    if (s_I2C2Error == eDeviceErrorDetected) {
        //send event to alarm task
        alarmInterface_SendEvent(eI2C2ErrorAlarm, eActive, eHighPriority, 0);
//        SYS_PRINT("\n error at: I2C2_ReportError\n");
        //change state
        s_I2C2Error = eDeviceErrorReported;
    }
}



/* *****************************************************************************
 End of File
 */
