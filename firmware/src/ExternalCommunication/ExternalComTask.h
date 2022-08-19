/* ************************************************************************** */
/** @file [PtDataTask.h]
 *  @brief {This file contain source code necessary for Patient Data Calculation. 
 * It implements a Patient Data Task, handle event come to Patient Data Task, 
 * initialize dependency components such as spO2 sensors, update flow sensors data, 
 * temperature data and run its algorithm }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */

#ifndef _EXT_COMMUNICATION_TASK_H    /* Guard against multiple inclusion */
#define _EXT_COMMUNICATION_TASK_H



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



/** @brief Initialize PATIENT DATA task and all components that dependency such as
 * UART2 for SPO2 module communication as well as interface for data sharing
 * This function should be called 1 times at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
//void PtDataTask_Initialize();

/** @brief Function to create Patient Data task run with FreeRTOS
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
//void PtDataTask_Create(void);

    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _PT_DATA_TASK_H */

/* *****************************************************************************
 End of File
 */
