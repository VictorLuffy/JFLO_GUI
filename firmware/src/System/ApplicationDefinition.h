/* ************************************************************************** */
/** @file [ApplicationDefinition.h]
 *  @brief {This file contain common definition used in anywhere of project source
 * code }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */

#ifndef _APPLICATION_DEFINITION_H    /* Guard against multiple inclusion */
#define _APPLICATION_DEFINITION_H



/* This section lists the other files that are included in this file.
 */




/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief define error state when a device on system detect an error. each
     * state has corresponding behavior */
    typedef enum {
        /**< state no error: this state indicate that a device has no error. This 
        state allow that device to operate and listen for error detected */
        eDeviceNoError = 0,

        /**< state error detected: this state indicate that a device has detect
         * an error. On this state, device operation will blocked and report error 
        to alarm subsystem */
        eDeviceErrorDetected,

        /**< state error reported: this state indicate that a device has detect
         * an and that error already reported to alarm subsystem. On this state, 
         * device operation will blocked and never comeback until a hardware reset */
        eDeviceErrorReported
    } E_DeviceErrorState;

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _APPLICATION_DEFINITION_H */

/* *****************************************************************************
 End of File
 */
