/** @file SettingScreen.h
 *  @brief The screen provides graphic interface to update firmware
 *  @author Thanh Hiep Nguyen
 */

#ifndef _SETTING_SCREEN_H
#define	_SETTING_SCREEN_H

/* This section lists the other files that are included in this file.
 */
#include <string.h>
#include "Gui/AlarmExpression.h"
#include "Gui/SettingScreen_DateTimeSetting.h"
#include "Gui/SettingScreen_WifiSetting.h"
#include "Gui/SettingScreen_LanguageSetting.h"
#include "Gui/SettingScreen_AlarmSoundLevelSetting.h"
#include "Gui/SettingScreen_SpO2AlarmLimitSetting.h"
#include "Gui/SettingScreen_OxySourceSetting.h"
#include "Gui/SettingScreen_OxygenConcentrationAlarmLimitSetting.h"
#include "Gui/SettingScreen_BrightnessSetting.h"
#include "Gui/SettingScreen_DeviceInformation.h"
#include "Gui/SettingScreen_DataLog.h"

typedef enum {
    eMenuScreenSetting, /**< menu screen */  
    eDataLogScreenSetting, /**< data log screen */  
    eMaintenanceScreenSetting, /**< maintenance screen */  
    eDeviceInformationScreenSetting, /**< device information screen */  
    eBackToMainScreenScreenSetting, /**< this notify to back to home screen */  
    eSaveConfimScreenSetting, /**< confirm setting change */            
    eNoOfScreenSetting, /**< no of enum */  
} E_ScreenSetting;

typedef enum {
    eDefaultMenuSetting, /**<default menu setting*/
    eLanguageMenuSetting, /**<Language setting */
    eDateTimeMenuSetting, /**<Wifi setting */
    eWifiMenuSetting, /**<Wifi setting */
    eBrightnessMenuSetting, /**<Brightness setting */
    eOxygenSourceMenuSetting, /**<oxygen source setting */
    eSpO2AlarmLimitMenuSetting, /**<Spo2 Alarm limit  setting */
    eOxygenConcentrationAlarmLimitMenuSetting, /**< Oxygen concentration alarm limit setting */
    eAlarmSoundLevelMenuSetting, /**< speaker volume level setting */
    eNoOfMenuSetting, /**< number of setting menu */
} E_MenuSetting;

// Define setting screen data
typedef struct {
    E_ScreenSetting screenSetting; /**< screen setting */
    E_ScreenSetting screenSettingNext; /**< next screen setting */
    E_MenuSetting menuSetting; /**< menu setting */
    E_MenuSetting menuSettingNext; /**< menu setting */
    E_DataLogSetting dataLogSetting; /**< data log setting*/
    bool isSettingScreenUpdated; /**< if the screen need to redraw */
    bool isShowConfirmScreen; /**< if the setting data changed */
    TickType_t xScreenNoActionTickCounter; /**< tick counter for return home screen */ 
}   SettingScreen_Data_Struct;

// Maximum Log in a page
//#define MAX_LOG_IN_PAGE             5

#define SETTING_SCREEN_NO_ACTION_TIMEOUT_MS 30000

#define UNDERBAR_MENUSETTING_POSITION_X 0
#define UNDERBAR_DATALOG_POSITION_X 60
#define UNDERBAR_MAINTENANCE_POSITION_X 120
#define UNDERBAR_DEVICEINFORMATION_POSITION_X 180

typedef void(*VOID_FUNC)(void);
typedef bool(*BOOL_FUNC)(void);

typedef struct {
    BOOL_FUNC checkDataChangeFunc; /**< checking if menu setting data change function */ 
    VOID_FUNC saveDataFunc; /**< save menu setting function */ 
    VOID_FUNC discardDataFunc;  /**< discard menu setting change function */ 
    VOID_FUNC initDataFunc; /**< init menu setting function */ 
    VOID_FUNC setCallbackFunc; /**< set callback setting function */ 
    VOID_FUNC displayFunc; /**< set display setting function */ 
} SettingScreen_MenuSetting_Struct;

E_GuiResult SettingScreen_Initialize(void);

// Destroy Settingscreen
void SettingScreen_Distructor(void);

// Deinitialize settingscreen's control and data
E_GuiResult SettingScreen_Reinit(void);

/** @brief SettingScreen_DisplayAlarm
 *      This control for alarm title, indicator display
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DisplayAlarm();

/** @brief SettingScreen_DisplayScreenSetting
 *      This control for display screen setting - menu setting, data log, maintenance, device info
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DisplayScreenSetting();
    
/** @brief SettingScreen_CheckScreenNoActionTimeout
 *      This control for checking no action timeout in setting screen - back to home screen when timeout
 *  @param [in] None
 *  @param [out] None
 *  @return bool
 */
bool SettingScreen_CheckScreenNoActionTimeout();

/** @brief SettingScreen_ResetScreenNoActionTimeout
 *      This reset the time no action on screen
 *  @param [in] None
 *  @param [out] None
 *  @return bool
 */
void SettingScreen_ResetScreenNoActionTimeout();

/** @brief SettingScreen_SetScreenSetting
 *  @param [in] E_ScreenSetting screenSetting
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SetScreenSetting(E_ScreenSetting screenSetting);

/** @brief SettingScreen_GetScreenSetting
 *  @param [in] None
 *  @param [out] None
 *  @return E_ScreenSetting
 */
E_ScreenSetting SettingScreen_GetScreenSetting();

/** @brief SettingScreen_SetNextScreenSetting
 *  @param [in] E_ScreenSetting screenSettingNext
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SetNextScreenSetting(E_ScreenSetting screenSettingNext);

/** @brief SettingScreen_GetNextScreenSetting
 *  @param [in] None
 *  @param [out] None
 *  @return E_ScreenSetting
 */
E_ScreenSetting SettingScreen_GetNextScreenSetting();


/** @brief SettingScreen_SetMenuSetting
 *  @param [in] E_MenuSetting menuSetting
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SetMenuSetting(E_MenuSetting menuSetting);

/** @brief SettingScreen_GetMenuSetting
 *  @param [in] None
 *  @param [out] None
 *  @return E_MenuSetting
 */
E_MenuSetting SettingScreen_GetMenuSetting();

/** @brief SettingScreen_SetNextMenuSetting
 *  @param [in] E_MenuSetting menuSetting
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SetNextMenuSetting(E_MenuSetting menuSetting);

/** @brief SettingScreen_GetNextMenuSetting
 *  @param [in] None
 *  @param [out] None
 *  @return E_MenuSetting
 */
E_MenuSetting SettingScreen_GetNextMenuSetting();

/** @brief SettingScreen_SetDataLogSetting
 *  @param [in] E_DataLogSetting dataLogSetting
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SetDataLogSetting(E_DataLogSetting dataLogSetting);

/** @brief SettingScreen_GetDataLogSetting
 *  @param [in] None
 *  @param [out] None
 *  @return E_DataLogSetting
 */
E_DataLogSetting SettingScreen_GetDataLogSetting();

/** @brief SettingScreen_SetFlagShowConfirmScreen
 *      Set flag to go confirm screen
 *  @param [in] bool flag
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SetFlagShowConfirmScreen(bool flag);

/** @brief SettingScreen_GetFlagShowConfirmScreen
 *     Get flag to go confirm screen
 *  @param [in] None
 *  @param [out] None
 *  @return bool
 */
bool SettingScreen_GetFlagShowConfirmScreen();

/** @brief SettingScreen_CheckMenuSettingDataChange
 *     Check and set flag if data change and unsaved
 *  @param [in] None
 *  @param [out] None
 *  @return void
 */
void SettingScreen_CheckMenuSettingDataChange();

/** @brief SettingScreen_SaveMenuSetting
 *     Call function pointer to save menu setting
 *  @param [in] None
 *  @param [out] None
 *  @return bool
 */
void SettingScreen_SaveMenuSetting();

/** @brief SettingScreen_DiscardSetting
 *     Call function pointer to discard menu setting
 *  @param [in] None
 *  @param [out] None
 *  @return bool
 */
void SettingScreen_DiscardMenuSetting();

/** @brief SettingScreen_InitMenuSetting
 *     Call function pointer to init menu setting
 *  @param [in] None
 *  @param [out] None
 *  @return bool
 */
void SettingScreen_InitMenuSetting();

/** @brief SettingScreen_DisplayMenuSetting_SubMenu
 *     Call function pointer to display sub menu setting
 *  @param [in] None
 *  @param [out] None
 *  @return bool
 */
void SettingScreen_DisplayMenuSetting_SubMenu();

/** @brief SettingScreen_SetSettingScreenUpdate
 *      Set state to indicate that setting screen is changed
 *  @param [in] bool flag
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SetSettingScreenUpdate(bool flag);

/** @brief SettingScreen_GetSettingScreenUpdate
 *     Get state if setting screen is changed
 *  @param [in] None
 *  @param [out] None
 *  @return bool
 */
bool SettingScreen_GetSettingScreenUpdate();


/** @brief SettingScreen_DisplayScreenSetting
 *      This control for display menu setting - language, datetime, wifi ....
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_DisplayMenuSetting();

/** @brief SettingScreen_SetCallbackFunction
 *      This set callback function for some event that not auto generated by Harmony
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_SetCallbackFunction(void);

/** @brief SettingScreen_GoToScreen_MenuSetting
 *  @param [in] None
 *  @param [out] None
 *  @return E_DateTimeSetting
 */
void SettingScreen_GoToScreen_MenuSetting();

/** @brief SettingScreen_GoToScreen_DataLogSetting
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_GoToScreen_DataLogSetting();

/** @brief SettingScreen_GoToScreen_Maintenance
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_GoToScreen_Maintenance();

/** @brief SettingScreen_GoToScreen_DeviceInformation
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_GoToScreen_DeviceInformation();

/** @brief SettingScreen_GoToScreen_BackToMainScreen
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_GoToScreen_BackToMainScreen();

/** @brief SettingScreen_Run
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SettingScreen_Run(void);

#endif	/* SETTINGSCREEN_H */

/* end of file */
