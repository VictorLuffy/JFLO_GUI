/** @file Watchdog.c
 *  @brief JFlo Mini.
 * 
 *  This file contains the operations required to service the watchdog
 *  timer.
 * 
 *  @author Thach Le
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "Watchdog.h"
#include "system/wdt/sys_wdt.h"

#include "system_config.h"
#include "system_definitions.h"
#include "system/debug/sys_debug.h"

bool isWatchdogEnable = false;

/** @brief Watchdog_Init
 * 
 *  Function to start the Watchdog timer
 * 
 *  @param (I) : None
 *  @param (O) : None
 *  @return None
 */
void Watchdog_Init()
{
    SYS_WDT_Enable(false);
    isWatchdogEnable = true;
}

/** @brief Watchdog_clearTimer
 * 
 *  Function to clear the Watchdog timer
 * 
 *  @param (I) : None
 *  @param (O) : None
 *  @return None
 */
void Watchdog_clearTimer()
{
//    SYS_PRINT("Watchdog_clearTimer \n");
    SYS_WDT_TimerClear();
}

/** @brief Watchdog_Disable
 * 
 *  Function to disable the Watchdog 
 * 
 *  @param (I) : None
 *  @param (O) : None
 *  @return None
 */
void Watchdog_Disable()
{
    isWatchdogEnable = false;
    SYS_WDT_Disable();
}

bool Watchdog_IsEnable()
{
    return isWatchdogEnable;
}