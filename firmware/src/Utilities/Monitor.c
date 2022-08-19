/* ************************************************************************** */
/** @file [Monitor.c]
 *  @brief //            Update the WatchDog monitor.
//
//            Monitor tasks within the system keeping statistics of
//            execution time (min, max average), and scheduling interval (min,
//            max, average) and CPU percentage.
//
//            In order to start the timing of a task set the attribute StartNow
//            to 1.  This can be done by examining the address of the variable
//            and using the ICE server to modify the address once the system is
//            past it's start up state.  During startup there is a system back
//            log preventing accurate timing.
//
//            To restart the timing sequence, the ClearData attribute is set to
//            1 and the StartNow attrubute reset to 1.
//
//            Set a break point in the task you are interested.  Step into
//            the monitoring functions and type a *this at the debugger to 
//            examine timing data.
//
//            Note to disable compiling any monitoring code into each task the 
//            "Monitor" flag in *.bld file can be deleted.  
 *  @author {bui phuoc}
 * 
/* ************************************************************************** */


#include <stdlib.h>
#include <stdint.h>


#include "system_config.h"
#include "system_definitions.h"
#include "FreeRTOS.h"
#include "task.h"

#include "Monitor.h"
#include "Watchdog.h"


//#define DEBUG_MONITOR

static uint32_t s_StartedTime[eLastTask];

//use this function when suspend a task 
void monitor_DisableTask(E_TaskID taskID)
{
    tasks[taskID].StatusEnable = false;
}
//use this function when resume a task 
void monitor_EnableTask(E_TaskID taskID)
{
    tasks[taskID].StatusEnable = true;
}



void monitor_UpdateStartedTime(E_TaskID taskID)
{
    s_StartedTime[taskID] = xTaskGetTickCount();
}



//******************************************************************************
//$COMMON.OPERATION$
//    Operation Name: CalcElapsedTime()
//
//    Processing: calculates the elasped time from startTime accounts for clock
//                  roll over
//
//    Input Parameters: startTime 
//    Output Parameters: None
//    Return Values: time in clock ticks
//    Pre-Conditions: None
//    Miscellaneous:
//      This code is mapped to the v851 on-chip ROM.
//      This code cannot be modified and still run on existing CPU boards.
//    Requirements:
//
//******************************************************************************
uint32_t CalcElapsedTime(uint32_t startTime)
{

    const uint32_t MAX_UINT = ~0L;
    uint32_t currentTime;
    uint32_t elapsedTime;

    currentTime = xTaskGetTickCount();
    if (currentTime >= startTime)
    {
        elapsedTime = currentTime - startTime;
    }

    // Deal with rollover
    else
    {
        elapsedTime = (MAX_UINT - startTime + currentTime);
    }

    return(elapsedTime);

}


/******************************************************************************/
//$COMMON.OPERATION$
//    Operation Name: monitor_HandleTaskMonitor
//
//    Processing: Monitors all tasks to check their HeartBeat if a task has
//                not checked in within the WatchDogInterval it is probably not
//                operating properly.
//
//    Input Parameters: None
//
//    Output Parameters: None
//
//    Return Values: None
//
//    Pre-conditions: None
//
//    Miscellaneous: None
//
//    Requirements: None
//
/******************************************************************************/
void monitor_HandleTaskMonitor()
{
    int i;
    static bool  strobe = true;
    
    if (!Watchdog_IsEnable())
    {
        return;
    }
    
    if(strobe)
    {
        static int counter_1s=0;
        static int watchdog_counter = 0;
        counter_1s = counter_1s + 2;
        if(counter_1s > 10000)//10s
        {
            counter_1s = 0;
            watchdog_counter = 0;
        }

        for (i = 0; i < eLastTask; i++)
        {
            if ( (tasks[i].StatusEnable == true)&&(tasks[i].WatchDogInterval != 0) && (s_StartedTime[i] != 0) )
            {
                uint32_t t = CalcElapsedTime(s_StartedTime[i]);
#ifdef DEBUG_MONITOR                
                SYS_PRINT("taskID: %d, Time: %d\n", i, t);
#endif                
                if(t >= tasks[i].WatchDogInterval)
                {
                    watchdog_counter++;
                    if(watchdog_counter>5)
                    {
                        strobe = false;
                        //SafetyValve->SetDesiredState(eOpen);
                        //Timeout(i);
                        //log to file
                        SYS_PRINT("Stuck at Task ID: %d\n", i);
                    }
                    break;
                }
            }
        }
#ifdef DEBUG_MONITOR 
        SYS_PRINT("\n");
#endif
    }


    if (strobe){
        //WDogPtr->StrobeAll();
        Watchdog_clearTimer();
    }
    else
    {
        //reboot
        //SYS_PRINT("Reboot\n");
    }
}
