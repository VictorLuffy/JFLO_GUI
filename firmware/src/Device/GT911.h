/* ************************************************************************** */
/** @file [GT911.h]
 *  @brief {This file contains all methods to communicate with GT911 touch driver,
 * including check data ready, perform I2C read, handle data, update touch event
 * to Graphic library, maintain state machine during run time }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */


#ifndef _GT911_H    /* Guard against multiple inclusion */
#define _GT911_H


/* This section lists the other files that are included in this file.
 */
#include <stdbool.h>
#include <stdint.h>



/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief State machine for reading coordinate from GT911 driver */
    typedef enum {
        /**< initialize state: falling this state at start up or error detected */
        eTouchInitStateID,
        /**< idle state: ready for next reading, this state accept interrupt signal
         * from GT911, then start reading coordinate via I2C */
        eTouchIdleStateID,
        /**< write state: write command to access register internal GT911 to read 
         * coordinate */
        eTouchWriteStateID,
        /**< read state: read data from internal GT911 addrress already selected by
         * write command above */
        eTouchReadStateID,
        /**< parse state: parse raw data has been read, transfer them to coordinate and handle */
        eTouchParseStateID,
        /**< confirm state: send command to GT911 to confirm the coordinate has been 
         * read, then the GT911 will stop generating interrupt and wait for next change
         * on the touch surface */
        eTouchConfirmStateID,
        /**< wait state: wait for confirmation above are done */
        eTouchWaitStateID,
        /**< error state: handle error if occur*/
        eTouchErrorStateID

    } E_GT911StateID;

    /** @brief Initialize GT911 Touch controller, including pull up reset line, open
     * I2C port and ready for communication, reset internal variables
     * This function should call 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void GT911_Initialize();


    /** @brief Request a read. This function is called when GT911 interrupt, indicate 
     * new data are available on GT911 and host should perform a read to update that data
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void GT911_RequestRead();


    /** @brief Maintain state machine to check data ready, read data from GT911
     * This function should call forever inside a task
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void GT911_Task();

    /** @brief Query any error happen with Touch module
     *  @param [in]  None   
     *  @param [out] None
     *  @retval true touch module has error
     *  @retval false touch module is OK
     */
    bool GT911_IsTouchFailure();

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _GT911_H */

/* *****************************************************************************
 End of File
 */
