/* ************************************************************************** */
/** @file [TaskIdle.h]
 *  @brief {Task Idle is design to run every task has Idle priority ( priority 0)
 * such as: GUI task, Command Processor Task, ... This design is to optimize 
 * performance of GUI task and system memory }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */


#ifndef _COMMON_H    /* Guard against multiple inclusion */
#define _COMMON_H



/* This section lists the other files that are included in this file.
 */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    
#include "system_config.h"
#include "system_definitions.h"
#include "stdint.h"    
typedef enum 
{
    eDevice,
    eAlarm,
    eGUI,
    eExtCom,
    eSystem,
    eLastTask
}E_TaskID;

/* * * * * * * * * * T Y P E   D E F I N I T I O N S * * * * * * * * */


typedef struct
{

        //$COMMON.ATTRIBUTE$
        //    Name: Name
        //
        //    Description: The task's name (max of 7 char).
        //
        //    Units: None
        //
        //    Range: N/A
        //
        char*   Name;

        //$COMMON.ATTRIBUTE$
        //    Name: StackSize
        //
        //    Description: Number of bytes for the task stack.
        //
        //    Units: bytes
        //
        //    Range: N/A
        //
        uint32_t    StackSize;

        //$COMMON.ATTRIBUTE$
        //    Name: Pri
        //
        //    Description: Priority between 0 and 255 (0 high).
        //
        //    Units: None
        //
        //    Range: N/A
        //
        uint8_t Pri;

        //$COMMON.ATTRIBUTE$
        //    Name: SchedulingInterval
        //
        //    Description:
        //         The number of clock ticks that expire before executing this task again.
        //
        //    Units: Clock Ticks
        //
        //    Range: N/A
        //
        uint32_t    SchedulingInterval;

        //$COMMON.ATTRIBUTE$
        //    Name: WatchDogInterval
        //
        //    Description:
        //           The number of clock ticks that expire before the watch dog is not strobed causing a system reset.
        //
        //    Units: Clock Ticks
        //
        //    Range: N/A
        //
        uint32_t    WatchDogInterval;
        
         //$COMMON.ATTRIBUTE$
        //    Name: Enable status
        //
        //    Description: status enable of Task.
        //
        //    Units: None
        //
        //    Range: true is enable, false is disable
        //
        bool    StatusEnable;
        
} Tasks_t;


extern Tasks_t tasks[eLastTask];


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
