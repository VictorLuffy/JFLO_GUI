/* ************************************************************************** */
/** @file [PtDataTask.h]
 *  @brief {This file contain source code necessary for Patient Data Calculation. 
 * It implements a Patient Data Task, handle event come to Patient Data Task, 
 * initialize dependency components such as spO2 sensors, update flow sensors data, 
 * temperature data and run its algorithm }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */

#ifndef _PT_DATA_TASK_H    /* Guard against multiple inclusion */
#define _PT_DATA_TASK_H



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

/** @brief structure to store all data need to use during Patient data process*/
    typedef struct {
        float o2Concentration; /**< lastest O2 concentration */
        float spO2; /**< lastest spO2 value */
    } PT_PUBLIC_DATA_t;


/** @brief Initialize PATIENT DATA task and all components that dependency such as
 * UART2 for SPO2 module communication as well as interface for data sharing
 * This function should be called 1 times at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PtDataTask_Initialize();

/** @brief Function to create Patient Data task run with FreeRTOS
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void PtDataTask_Create(void);

/** @brief Get sharing data of Patient data 
 * This function can be called from other task to obtain Patient data
 *  @param [in]  None   
 *  @param [out]  PT_PUBLIC_DATA_t* data   external pointer to store data
 *  @return None
 */
bool PtDataTask_GetPublicData(PT_PUBLIC_DATA_t* data);
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _PT_DATA_TASK_H */

/* *****************************************************************************
 End of File
 */
