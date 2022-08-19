/* ************************************************************************** */
/** @file [Monitor.h]
 *  @brief {A PID controller, modified from a PID controller of author Nick Mosher
 * Support all method to implement an turning a PID controller}
 *  @author {bui phuoc}
 * 
/* ************************************************************************** */



#ifndef _MONITOR_H
#define _MONITOR_H



/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#include"common.h" 
    
void monitor_DisableTask(E_TaskID taskID);

void monitor_EnableTask(E_TaskID taskID);

void monitor_UpdateStartedTime(E_TaskID taskID);

void monitor_HandleTaskMonitor();

#ifdef __cplusplus
}
#endif

#endif // _MONITOR_H
