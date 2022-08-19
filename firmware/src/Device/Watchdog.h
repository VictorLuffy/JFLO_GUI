/** @file Watchdog.h
 *  @brief JFlo Mini.
 * 
 *  This file contains all the functions prototype for Watchdog module.
 *  Watchdog is used to reset mcu when it malfunctions.
 * 
 *  @author Thach Le
 */

#ifndef WATCHDOG_H
#define	WATCHDOG_H

#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

    //function to initialize Watchdog timer
    void Watchdog_Init();
    //function to clear the Watchdog timer
    void Watchdog_clearTimer();
    //function to disable the Watchdog 
    void Watchdog_Disable();
    
    bool Watchdog_IsEnable();

#ifdef	__cplusplus
}
#endif

#endif	/* WATCHDOG_H */

