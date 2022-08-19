/* ************************************************************************** */
/** @file [CommandProcessor.h]
 *  @brief { This file contains source code necessary to control and maintain 
 * Command Processor such as: adding a command to the Command Processor and handle
 * Callback function from Command Processor }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */



#ifndef _COMMANAD_PROCESSOR_H    /* Guard against multiple inclusion */
#define _COMMANAD_PROCESSOR_H



/* This section lists the other files that are included in this file.
 */



/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief Initialize Command Processor by adding user defined command group to
     * Command Processor system 
     * This function should be called 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void CmdProcessor_Initialize(void);

    /** @brief Maintain Command Processor system
     * This function should be called repeatedly faster than UART console baud rate 
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void CmdProcessor_Task(void);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _COMMANAD_PROCESSOR_H */

/* *****************************************************************************
 End of File
 */
