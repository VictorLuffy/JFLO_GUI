/** @file SystemTask.h
 *  @brief RTOS task for SYSTEM
 *  @author Viet Le
 */


#ifndef SYSTEM_TASK_H
#define	SYSTEM_TASK_H


/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>





/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
    /** @brief Function to create Charging Task  run with FreeRTOS
     *  @param [in] None
     *  @param [out] None
     *  @return None
     */
    void System_CreateChargingTask(void);

    /** @brief Function to create Charging Task  run with FreeRTOS
     *  @param [in] None
     *  @param [out] None
     *  @return None
     */
    void System_CreateSystemTask(void);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif	/* SYSTEM_TASK_H */

/* end of file */