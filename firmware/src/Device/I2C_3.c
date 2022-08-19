/* ************************************************************************** */
/** @file [I2C_3.c]
 *  @brief {control read/write data via I2C3, manage share resource between multiple 
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


#include "I2C_3.h"
#include "AlarmInterface.h"
#include "ApplicationDefinition.h"

/** @brief clock cycles for recovery */
#define I2C_RECOVER_NUM_CLOCKS      10U
/** @brief clock frequency for recovery */
#define I2C_RECOVER_CLOCK_FREQ      50000U
/** @brief delay to create clock for recovery */
#define I2C_RECOVER_CLOCK_DELAY_US  (1000000U / (2U * I2C_RECOVER_CLOCK_FREQ))

extern const DRV_I2C_INIT drvI2C3InitData; 

/** @brief I2C index use for Air flow sensor map with Harmony configuration */
#define I2C_3_INDEX                 2          

/** @brief I2C flow sensor handle */
static DRV_HANDLE s_I2C3Handle = DRV_HANDLE_INVALID;

/** @brief I2C flow sensor buffer handle*/
static DRV_I2C_BUFFER_HANDLE s_I2C3BufferHandle;

/** @brief I2C buffer communication status*/
static DRV_I2C_BUFFER_EVENT s_I2C3CommStatus;

/** @brief task notification flag, used for synchronization communication when a 
 * I2C transaction completed*/
static TaskHandle_t s_I2C3NotityFlag = NULL;

/** @brief mutex to protect I2C 4 sharing */
//static SemaphoreHandle_t s_I2C3Mutex = NULL;

/** @brief Flag indicate I2C3 has error
 * true mean error happened
 * false mean no error */
static E_DeviceErrorState s_I2C3Error = eDeviceNoError;

static uint8_t s_I2C3RecoverCounter = 0;

/** @brief internal functions declaration */
static void I2C3_ReportError();


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

bool I2C3_CheckTransferStatus(DRV_I2C_BUFFER_HANDLE drvBufferHandle)
{
    if (DRV_I2C_BUFFER_EVENT_COMPLETE == DRV_I2C_TransferStatusGet(s_I2C3Handle, drvBufferHandle))
    {
        return true; //ok
    }
    else
    {
        return false; //fail
    }
}

/** @brief Function to initialize I2C3, used to communicate with multiple devices\
 * such as: BME280, ADXL345, and Audio Codec. including open I2C port, setting 
 * callback function, initializing memories before operation
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void I2C3_Initialize() {
    if (s_I2C3Handle == DRV_HANDLE_INVALID) {
        s_I2C3Handle = DRV_I2C_Open(I2C_3_INDEX, DRV_IO_INTENT_EXCLUSIVE);
        /* event-handler set up receive callback from DRV_I2C_Tasks */
    }

    //create Mutex
    //s_I2C3Mutex = xSemaphoreCreateMutex();
    //xSemaphoreGive(s_I2C3Mutex);
    
    //reset variables
    s_I2C3Error = eDeviceNoError;
}

/** @brief This function reset I2C communicate by Flushing Queue
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
  */
static void I2C3_ResetComunicate() 
{
    SYS_PRINT("\nI2C3 error >>> Reset I2C3\n");
    I2C3STATbits.IWCOL = 0;
    I2C3STATbits.BCL = 0;
    I2C3STATbits.I2COV = 0;
    
    I2C3CONbits.I2CEN = 0;
    TRISF = SYS_PORT_F_TRIS & 0xFEFF;
    // SCL1 as output
    TRISFbits.TRISF8 = 0;
    
    // SDA1 as input
    TRISFbits.TRISF2 = 1;
    int i;
    do
    {
        for ( i = 0U; i < I2C_RECOVER_NUM_CLOCKS; ++i)
        {
            DelayUs (I2C_RECOVER_CLOCK_DELAY_US);
            LATFbits.LATF8 = 1;
            DelayUs (I2C_RECOVER_CLOCK_DELAY_US);
            LATFbits.LATF8 = 0;
        }
        
        s_I2C3RecoverCounter++;
        if (s_I2C3RecoverCounter > 3)
        {
            break;
        }
        
    }while(!PORTFbits.RF2);
    
    s_I2C3RecoverCounter = 0;
    I2C3CONbits.I2CEN = 1;
    I2C3_Initialize();
    DRV_I2C_QueueFlush(s_I2C3Handle);
}


/** @brief write a packet data via I2C3, then wait for it done
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
bool I2C3_Write(uint16_t address, void *writeBuffer, size_t size, uint32_t maxWait) 
{
    //check error and report
    //    if (s_I2C3Error != eDeviceNoError) {
    //        //report error
    //        I2C3_ReportError();
    //        return false;
    //    }
    
    s_I2C3BufferHandle = DRV_I2C_Transmit(s_I2C3Handle, address,(void*)  writeBuffer, size, NULL);
    uint8_t timeout = 0;
    if (s_I2C3BufferHandle == DRV_I2C_BUFFER_HANDLE_INVALID)
    {
        SYS_PRINT("I2C3: DRV_I2C_BUFF_Transmit_INVALID\n");
        I2C3_ResetComunicate();
        return false; //error
    } 
    else 
    {
        //vTaskDelay(1);
        DelayUs(100);
        while (!I2C3_CheckTransferStatus(s_I2C3BufferHandle))
        {
            vTaskDelay(1);
            timeout++;
            if(timeout >= maxWait){
                //I2C3_ResetComunicate();
//                static uint8_t s_errCnt = 0;
//                s_errCnt++; if(s_errCnt >= 2){I2C3_ResetComunicate(); s_errCnt = 0;} 
                return false;
            }
        }
        //SYS_PRINT("timeout_%d\n", timeout);
        //return true;
    }  
    
    return true;
}

/** @brief read data via I2C3 and wait for it done
 *  @param [in]  uint16_t address: I2C Address need to read data  
 *              size_t size: size of data expect to read 
 *              uint32_t maxWait: maximum time (in ms) wait for read done. If over
 * time, return error
 *  @param [out]  void *readBuffer: pointer to store buffer
 *  @return None
 *  @retval true read data success
 *  @retval false read data failed
 */
bool I2C3_Read(uint16_t address, void *readBuffer, size_t size, uint32_t maxWait) 
{
    //check error
//    if (s_I2C3Error != eDeviceNoError) {
//        //report error
//        I2C3_ReportError();
//        return false;
//    }
    
    
    s_I2C3BufferHandle = DRV_I2C_Receive(s_I2C3Handle, address,(void*)  readBuffer, size, NULL);
    uint8_t timeout = 0;
    if (s_I2C3BufferHandle == DRV_I2C_BUFFER_HANDLE_INVALID) {

        SYS_PRINT("I2C3: DRV_I2C_BUFF_Receive_INVALID\n");
        I2C3_ResetComunicate();
        return false; //error
    } 
    else {
        //vTaskDelay(1);//2
        DelayUs(300);
        while (!I2C3_CheckTransferStatus(s_I2C3BufferHandle))
        {
            vTaskDelay(1);
            timeout++;
            if(timeout >= maxWait){
                //I2C3_ResetComunicate();
//                static uint8_t s_errCnt = 0;
//                s_errCnt++; if(s_errCnt >= 2){I2C3_ResetComunicate(); s_errCnt = 0;} 
                return false;
            }
        }
        //SYS_PRINT("timeoutRd_%d\n", timeout);
        //return true;
    } 
   
    return true;
}



/** @brief report error if occur during communication via I2C3, may be send event
 * to Alarm task
 *  @param [in]  None 
 *  @param [out]  None
 *  @return None
 */
void I2C3_ReportError() {
    //check whether an error is detected, then send event to ALarm task
    if (s_I2C3Error == eDeviceErrorDetected) {
        //send event to alarm task
        alarmInterface_SendEvent(eI2C3ErrorAlarm, eActive, eHighPriority, 0);
        SYS_PRINT("\n error at: I2C3_ReportError");
        //change state
        s_I2C3Error = eDeviceErrorReported;
    }
}





/* *****************************************************************************
 End of File
 */
