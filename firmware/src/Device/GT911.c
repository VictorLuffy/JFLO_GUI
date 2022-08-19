/* ************************************************************************** */
/** @file [GT911.c]
 *  @brief {This file contains all methods to communicate with GT911 touch driver,
 * including check data ready, perform I2C read, handle data, update touch event
 * to Graphic library, maintain state machine during run time }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */
#include "system_config.h"
#include "system_definitions.h"
#include "driver/i2c/drv_i2c.h"
#include "FreeRTOS.h"
#include "task.h"
#include "GT911.h"
#include "Delay.h"
#include "ApplicationDefinition.h"


/** @brief I2C index use for GT911 Touch Driver, configure on MPLAB Harmony */
#define GT911_I2C_INDEX   			    3

/** @brief I2C address of GT911 Touch Driver */
#define GT911_BASE_ADDR       (0x5D)

/** @brief I2C address of GT911 Touch Driver for writing
 write address = base address << 1 + write bit (0))*/
#define GT911_WRITE_ADDR      (GT911_BASE_ADDR << 1)

/** @brief I2C address of GT911 Touch Driver for reading
 read address = base address << 1 + read bit(1))*/
#define GT911_READ_ADDR       ((GT911_BASE_ADDR << 1)+1)

/** @brief Size of 1 frame data for each time reading coordinate */
#define GT911_FRAME_SIZE         (10)

/** @brief Pull down reset line of Touch screen controller to make it reset*/
#define GT9111_RESET            TOUCH_RESETOff()

/** @brief Pull up reset line of Touch screen controller to make it un-reset*/
#define GT9111_UN_RESET         TOUCH_RESETOn()


/** @brief status ID for for an touch event */
typedef enum {
    /**< touch pressed: touch pannel is pressed. Detect by previous status event 
     * is relase and current status is pressed */
    eTouchPressed,
    /**< touch hold: touch pannel is pressed and hold. Detect by previous status 
     * event is pressed, and current status event is pressed too, and value of x, y 
     has changed*/
    eTouchMove,
    /**< touch release: touch pannel is release from pressed. Detect by previous 
     * status event is pressed, and current status event is release */
    eTouchReleased
} E_GT911TouchEventID;

/** @brief structure for a GT911 touch event data */
typedef struct {
    E_GT911TouchEventID id; /**< touch panel event id */
    uint16_t xpos; /**< x coordinate */
    uint16_t ypos; /**< x coordinate */
} GT911_TOUCH_DATA_t;

/** @brief Flag indicate Touch has error*/
static E_DeviceErrorState s_GT911Error = eDeviceNoError;


/** @brief I2C driver handle when open I2C comport for GT911 touch controller */
//static DRV_HANDLE s_GT911_I2CDriverHandle = DRV_HANDLE_INVALID;

/** @brief I2C buffer handle for GT911 touch controller */
//static DRV_I2C_BUFFER_HANDLE s_GT911_I2CBufferHandle = DRV_I2C_BUFFER_HANDLE_INVALID;

/** @brief Last touch event data. Initialize as* release at position (0;0) */
static GT911_TOUCH_DATA_t s_TouchLastStatus = {eTouchReleased, 0, 0};

/** @brief Flag indicate a read request. This flag is set when an interrupt from 
 * Touch Controller is occurred */
static bool s_GT911RequestRead = false;

/** @brief State machine perform reading coordinate sequence */
E_GT911StateID s_GT911OperationState = eTouchInitStateID;

/** @brief buffer to store raw data while reading from GT911 touch controller */
static uint8_t s_CoordinateRawData[GT911_FRAME_SIZE];   //__attribute__ ((aligned (16)))

/** @brief command send to GT911 to read coordinate */
const uint8_t GT911ReadCmd[] = {0x81, 0x4e};

/** @brief command send to GT911 to confirm coordinate has been read */
const uint8_t GT911ConfirmCmd[] = {0x81, 0x4e, 0x00};

/** @brief local functions */
static void GT911_UpdateCoordinate(GT911_TOUCH_DATA_t event);
static void GT911_HandleMessage(uint8_t status, uint16_t x, uint16_t y);


/** @brief Initialize GT911 Touch controller, including pull up reset line, open
 * I2C port and ready for communication, reset internal variables
 * This function should call 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void GT911_Initialize() {
    //if (s_GT911_I2CDriverHandle == DRV_HANDLE_INVALID) {
    //    s_GT911_I2CDriverHandle = DRV_I2C_Open(GT911_I2C_INDEX, DRV_IO_INTENT_EXCLUSIVE);
    //}
            
    GT9111_RESET;
    delay_HardDelay(10);
    //pull up reset line
    GT9111_UN_RESET;

    //initialize value for last state of positions
    s_TouchLastStatus.id = eTouchReleased;
    s_TouchLastStatus.xpos = 0;
    s_TouchLastStatus.ypos = 0;
}

/** @brief Reset GT911 Touch controller, by pulling the reset line down to 0V and
 * wait for 10 ms
 * This function may call when some error happen on  GT911 and can not be solved
 * by software
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void GT911_Reset() {
    //pull down reset line
    GT9111_RESET;
    vTaskDelay(20);
    //pull up reset line
    GT9111_UN_RESET;
}

/** @brief Request a read. This function is called when GT911 interrupt, indicate 
 * new data are available on GT911 and host should perform a read to update that data
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void GT911_RequestRead() {
    s_GT911RequestRead = true;
}


/** @brief Maintain state machine to check data ready, read data from GT911
 * This function should call forever inside a task
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void GT911_Task() {

    static uint16_t s_counterErr = 0;
    uint16_t x = 0;
    uint16_t y = 0;
    uint8_t status = 0;

    switch (s_GT911OperationState) {
        case eTouchInitStateID:
            GT911_Initialize();
            s_GT911OperationState = eTouchIdleStateID;
            break;
        case eTouchIdleStateID:
            if (s_GT911RequestRead == true) {
                s_GT911OperationState = eTouchWriteStateID;
                s_GT911RequestRead = false;
                //SYS_PRINT("INT3interupt\n");
            }
            break;
        case eTouchWriteStateID:
            
            if( I2C4_Write(GT911_WRITE_ADDR, (void*) &GT911ReadCmd[0], sizeof (GT911ReadCmd), 10) == false)
            {
                s_GT911OperationState = eTouchErrorStateID;
            }
            else
            {
                s_GT911OperationState = eTouchReadStateID;
                s_counterErr = 0;
            }
            break;
            
        case eTouchReadStateID:
            
            if(I2C4_Read(GT911_READ_ADDR, (void*) &s_CoordinateRawData[0], 7, 10) == false)
            {
                s_GT911OperationState = eTouchErrorStateID;
            }
            else{
                s_GT911OperationState = eTouchParseStateID;
                s_counterErr = 0;
            }
            break;
            
        case eTouchParseStateID:
            
            x = 256 * s_CoordinateRawData[3] + s_CoordinateRawData[2];
            y = 256 * s_CoordinateRawData[5] + s_CoordinateRawData[4];
            status = s_CoordinateRawData[0];
            GT911_HandleMessage(status, x, y);

            //next do confirm read
            s_GT911OperationState = eTouchConfirmStateID;
            break;
        case eTouchConfirmStateID:

            if(I2C4_Write(GT911_WRITE_ADDR, (void*) &GT911ConfirmCmd[0], sizeof (GT911ConfirmCmd), 10) == false)
            {
                s_GT911OperationState = eTouchErrorStateID;
            }
            else
            {
                s_GT911OperationState = eTouchIdleStateID;
                s_counterErr = 0;
            }
            break;
            
        default:
            //handle error: try again if 
            s_counterErr++;
            if(s_GT911Error >= 3){
                s_GT911Error = eDeviceErrorDetected;
            }
            s_GT911OperationState = eTouchInitStateID;
            break;
    }
}


/** @brief Update new touch data to Graphic library. In this case, Aria graphic 
 * library is used
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void GT911_UpdateCoordinate(GT911_TOUCH_DATA_t event) {
    uint8_t deviceId = 0; //default device ID
    uint16_t xpos = event.xpos;
    uint16_t ypos = event.ypos;
    E_GT911TouchEventID id = event.id;

    switch (id) {
        case eTouchPressed:
            laInput_InjectTouchDown(deviceId, xpos, ypos);
            break;
        case eTouchMove:
            laInput_InjectTouchMoved(deviceId, xpos, ypos);
            break;
        case eTouchReleased:
            laInput_InjectTouchUp(deviceId, xpos, ypos);
            break;
        default:
            break;
    }

    //update for last event data
    s_TouchLastStatus = event;
}

/** @brief Handle message data has just read from GT911
 *  @param [in]  uint8_t status     status of GT911 device
 *                              status[7] = 0 -> data not ready
 *                              status[7] = 1 -> data ready
 *                              status[3:0] = 0 -> release event
 *                              status[3:0] = 1 -> press event
 *              uint16_t x      x position
 *              uint16_t y      y position
 *  @param [out]  None
 *  @return None
 */
void GT911_HandleMessage(uint8_t status, uint16_t x, uint16_t y) {
    //check MSB bit of status indicate touch driver is ready or not
    if ((status & 0x80) == 0) {
        return; //return if driver is not ready
    }

    //SYS_PRINT("\n GT911_HandleMessage (%d,%d)", x, y);
    
    uint8_t event = status & 0x0F; //get signal pressed/released by 4 LSB bit
    GT911_TOUCH_DATA_t currentData;
    currentData.xpos = x;
    currentData.ypos = y;
    switch (event) {
        case 0x01: // touch down / pressed
            if (s_TouchLastStatus.id != eTouchReleased) { //touch hold
                if ((s_TouchLastStatus.xpos != currentData.xpos) ||
                        (s_TouchLastStatus.ypos != currentData.ypos)) {
                    //set event type
                    currentData.id = eTouchMove;
                    //update coordinate
                    GT911_UpdateCoordinate(currentData);
                }
            } else {
                //touch pressed
                currentData.id = eTouchPressed;
                //update coordinate
                GT911_UpdateCoordinate(currentData);
            }
            break;
        case 0x00: // touch up / released
            if (s_TouchLastStatus.id != eTouchReleased) {
                //set event type
                currentData.id = eTouchReleased;
                //update coordinate
                GT911_UpdateCoordinate(currentData);
            }
            break;

        default:
            break;
    }
}

/** @brief Query any error happen with Touch module
 *  @param [in]  None   
 *  @param [out] None
 *  @retval true touch module has error
 *  @retval false touch module is OK
 */
bool GT911_IsTouchFailure() {
    if (s_GT911Error == eDeviceNoError) 
    {
        return false;
    }
    else 
    {
        return true;
    }
}

/* *****************************************************************************
 End of File
 */
