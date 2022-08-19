/** @file UpdateScreen.c
 *  @brief The screen provides graphic interface to update firmware
 *  @author Thanh Hiep Nguyen
 */

/* This section lists the other files that are included in this file.
 */
#include "MaintenanceScreen.h"
#include "UpdateScreen.h"
#include "SystemInterface.h"
#include "Gui/DisplayControl.h"
#include "Gui/Setting.h"


/** @brief Initialize UpdateScreen's element: Check all UpdateScreen's widgets have been created yet,
 * init control & data variable and change some properties in needed initialization widgets
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void UpdateScreen_Initialize(void)
{
    return;
}

/** @brief Reinitialize UpdateScreen's control and data.
 *  @brief Cause the value of UpdateScreen's control, data is changed and this screen is created by ScreenCreate_UpdateScreen fnc.
 *  This fnc will remake all variable of each widget in UpdateScreen
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void UpdateScreen_Reinit(void)
{
}

/** @brief Distruct some updatescreen backend elements when switched to other one
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void UpdateScreen_Distructor(void)
{
}

/** @brief Main running process
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void UpdateScreen_Run(void)
{

}
