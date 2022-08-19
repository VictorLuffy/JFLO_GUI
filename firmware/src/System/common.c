/* ************************************************************************** */
/** @file [TaskIdle.c]
 *  @brief {Task Idle is design to run every task has Idle priority ( priority 0)
 * such as: GUI task, Command Processor Task, ... This design is to optimize 
 * performance of GUI task and system memory }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */



/* This section lists the other files that are included in this file.
 */

#include "common.h"


// global tasks array
Tasks_t tasks[eLastTask] = {
    //
    // Name     Stack   Pri             SchedInterval          WatchDogInterval
    // ----     -----   ---             -------------          ---------------

    {"Device",  10,     1,              10,                     500,     true},
    {"Alarm",   50,     2,              50,                     500,     true},
    {"GUI",     5,      5,              5,                      500,     true},
    {"ExtCom",  18,     3,              18,                     1000,     true},
    {"System",  10,     0,              10,                     100,     true},
};

/* *****************************************************************************
 End of File
 */
