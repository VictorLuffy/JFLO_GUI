/* ************************************************************************** */
/** @file [TaskIdle.h]
 *  @brief {Task Idle is design to run every task has Idle priority ( priority 0)
 * such as: GUI task, Command Processor Task, ... This design is to optimize 
 * performance of GUI task and system memory }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */


#ifndef _GUI_TASK_H    /* Guard against multiple inclusion */
#define _GUI_TASK_H



/* This section lists the other files that are included in this file.
 */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief Initialize Idle Task, and all tasks run inside Idle Task such as:
     * GUI task, Command Processor task ... 
     * This function should be called 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void GuiTask_Initialize();

    /** @brief Create a task to run in Idle priority
     * This function should be called 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void GuiTask_Create();

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
