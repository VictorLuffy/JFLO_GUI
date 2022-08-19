/** @file SettingScreen.c
 *  @brief The screen provides graphic interface to update firmware
 *  @author Thanh Hiep Nguyen
 */

/* This section lists the other files that are included in this file.
 */
#include "Gui/SettingScreen.h"
#include "SettingScreen_OxySourceSetting.h"
#include "SettingScreen_LanguageSetting.h"
#include "SettingScreen_OxygenConcentrationAlarmLimitSetting.h"

/** @brief variable to store setting screen status */
static SettingScreen_Data_Struct s_settingScreenData;

const SettingScreen_MenuSetting_Struct MENU_SETTING[eNoOfMenuSetting] = {
    [eDefaultMenuSetting] = {
        .checkDataChangeFunc = 0,
        .saveDataFunc = 0,
        .discardDataFunc = 0,
        .initDataFunc = 0,
        .setCallbackFunc = 0,
        .displayFunc = 0,
    },
    [eLanguageMenuSetting] = {
        .checkDataChangeFunc = &SettingScreen_LanguageSetting_CheckDataChange,
        .saveDataFunc = &SettingScreen_LanguageSetting_SaveSetting,
        .discardDataFunc = &SettingScreen_LanguageSetting_DiscardSetting,
        .initDataFunc = &SettingScreen_LanguageSetting_Init,
        .setCallbackFunc = &SettingScreen_LanguageSetting_SetCallbackFunction,
        .displayFunc = &SettingScreen_LanguageSetting_Display,
    },
    [eDateTimeMenuSetting] = {
        .checkDataChangeFunc = &SettingScreen_DateTimeSetting_CheckDataChange,
        .saveDataFunc = &SettingScreen_DateTimeSetting_SaveSetting,
        .discardDataFunc = &SettingScreen_DateTimeSetting_DiscardSetting,
        .initDataFunc = &SettingScreen_DateTimeSetting_Init,
        .setCallbackFunc = &SettingScreen_DateTimeSetting_SetCallbackFunction,
        .displayFunc = &SettingScreen_DateTimeSetting_Display,
    },
    [eWifiMenuSetting] = {
        .checkDataChangeFunc = &SettingScreen_WifiSetting_CheckDataChange,
        .saveDataFunc = &SettingScreen_WifiSetting_SaveSetting,
        .discardDataFunc = &SettingScreen_WifiSetting_DiscardSetting,
        .initDataFunc = &SettingScreen_WifiSetting_Init,
        .setCallbackFunc = &SettingScreen_WifiSetting_SetCallbackFunction,
        .displayFunc = &SettingScreen_WifiSetting_Display,
    },
    [eBrightnessMenuSetting] = {
        .checkDataChangeFunc = &SettingScreen_BrightnessSetting_CheckDataChange,
        .saveDataFunc = &SettingScreen_BrightnessSetting_SaveSetting,
        .discardDataFunc = &SettingScreen_BrightnessSetting_DiscardSetting,
        .initDataFunc = &SettingScreen_BrightnessSetting_Init,
        .setCallbackFunc = &SettingScreen_BrightnessSetting_SetCallbackFunction,
        .displayFunc = &SettingScreen_BrightnessSetting_Display,
    },
    [eOxygenSourceMenuSetting] = {
        .checkDataChangeFunc = &SettingScreen_OxySourceSetting_CheckDataChange ,
        .saveDataFunc = &SettingScreen_OxySourceSetting_SaveSetting,
        .discardDataFunc = &SettingScreen_OxySourceSetting_DiscardSetting,
        .initDataFunc = &SettingScreen_OxySourceSetting_Init,
        .setCallbackFunc = &SettingScreen_OxySourceSetting_SetCallbackFunction,
        .displayFunc = &SettingScreen_OxySourceSetting_Display,
    },
    [eSpO2AlarmLimitMenuSetting] = {
        .checkDataChangeFunc = &SettingScreen_SpO2AlarmLimitSetting_CheckDataChange,
        .saveDataFunc = &SettingScreen_SpO2AlarmLimitSetting_SaveSetting,
        .discardDataFunc = &SettingScreen_SpO2AlarmLimitSetting_DiscardSetting,
        .initDataFunc = &SettingScreen_SpO2AlarmLimitSetting_Init,
        .setCallbackFunc = &SettingScreen_SpO2AlarmLimitSetting_SetCallbackFunction,
        .displayFunc = &SettingScreen_SpO2AlarmLimitSetting_Display,
    },
    [eOxygenConcentrationAlarmLimitMenuSetting] = {
        .checkDataChangeFunc = &SettingScreen_OxygenConcentrationAlarmLimitSetting_CheckDataChange,
        .saveDataFunc = &SettingScreen_OxygenConcentrationAlarmLimitSetting_SaveSetting,
        .discardDataFunc = &SettingScreen_OxygenConcentrationAlarmLimitSetting_DiscardSetting,
        .initDataFunc = &SettingScreen_OxygenConcentrationAlarmLimitSetting_Init,
        .setCallbackFunc = &SettingScreen_OxygenConcentrationAlarmLimitSetting_SetCallbackFunction,
        .displayFunc = &SettingScreen_OxygenConcentrationAlarmLimitSetting_Display,
    },
    [eAlarmSoundLevelMenuSetting] = {
        .checkDataChangeFunc = &SettingScreen_AlarmSoundLevelSetting_CheckDataChange,
        .saveDataFunc = &SettingScreen_AlarmSoundLevelSetting_SaveSetting,
        .discardDataFunc = &SettingScreen_AlarmSoundLevelSetting_DiscardSetting,
        .initDataFunc = &SettingScreen_AlarmSoundLevelSetting_Init,
        .setCallbackFunc = &SettingScreen_AlarmSoundLevelSetting_SetCallbackFunction,
        .displayFunc = &SettingScreen_AlarmSoundLevelSetting_Display,
    },
};

void SC_LanguageMenuPanel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_SetNextMenuSetting(eLanguageMenuSetting);
    SettingScreen_LanguageSetting_Init();
    // this should end of function, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_DateTimeMenuPanel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_SetNextMenuSetting(eDateTimeMenuSetting);
    SettingScreen_DateTimeSetting_Init();
    
    // this should end of function, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_WifiMenuPanel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_SetNextMenuSetting(eWifiMenuSetting);
    SettingScreen_WifiSetting_Init();
    
    // this should end of function, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_BrightnessMenuPanel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_SetNextMenuSetting(eBrightnessMenuSetting);
    SettingScreen_BrightnessSetting_Init();
    
    // this should end of function, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_OxygenSourceMenuPanel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_SetNextMenuSetting(eOxygenSourceMenuSetting);
    SettingScreen_OxySourceSetting_Init();
    // this should end of function, follow sample code
    evt->event.accepted = LA_TRUE;
}

void SC_SpO2AlarmLimitMenuPanel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_SetNextMenuSetting(eSpO2AlarmLimitMenuSetting);
    SettingScreen_SpO2AlarmLimitSetting_Init();
    // this should end of function, follow sample code
    evt->event.accepted = LA_TRUE;
}

void SC_OxygenConcentrationAlarmLimitMenuPanel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_SetNextMenuSetting(eOxygenConcentrationAlarmLimitMenuSetting);
    SettingScreen_OxygenConcentrationAlarmLimitSetting_Init();
    // this should end of function, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_AlarmSoundLevelMenuPanel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_SetNextMenuSetting(eAlarmSoundLevelMenuSetting);
    SettingScreen_AlarmSoundLevelSetting_Init();
    
    // this should end of function, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_AlarmTitleBarPanel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    DisplayControl_SetState(eMainScreenIsShowingDispState);
    AlarmExpression_SetAlarmPopupAutoShowAtInit(true);
    // this should end of function, follow sample code
    evt->event.accepted = LA_TRUE;
}

void SettingScreen_SetCallbackFunction(void)
{
    // Assigning the callback function to widget, these event cannot auto add by generator
    laWidget_OverrideTouchDownEvent((laWidget*)SC_LanguageMenuPanel, &SC_LanguageMenuPanel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_DateTimeMenuPanel, &SC_DateTimeMenuPanel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_WifiMenuPanel, &SC_WifiMenuPanel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_BrightnessMenuPanel, &SC_BrightnessMenuPanel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_OxygenSourceMenuPanel, &SC_OxygenSourceMenuPanel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_SpO2AlarmLimitMenuPanel, &SC_SpO2AlarmLimitMenuPanel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_OxygenConcentrationAlarmLimitMenuPanel, &SC_OxygenConcentrationAlarmLimitMenuPanel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_AlarmSoundLevelMenuPanel, &SC_AlarmSoundLevelMenuPanel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_AlarmTitleBarPanel, &SC_AlarmTitleBarPanel_PressedEvent);

    // menu setting callback
    int i;
    for (i = 0; i < eNoOfMenuSetting ; i ++)
    {
        if (MENU_SETTING[i].setCallbackFunc != 0)
        {
            MENU_SETTING[i].setCallbackFunc();
        }
    }
    
    // device information callback
    SettingScreen_DeviceInformation_SetCallbackFunction();
    
    // ddata log callback
    SettingScreen_DataLog_SetCallbackFunction();
}

/** @brief Initialize settingscreen's element: Check all settingscreen's widgets have been created yet,
 * init control & data variable and change some properties in needed initialization widgets
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
E_GuiResult SettingScreen_Initialize(void)
{
    // this use to correct the active screen id, must be placed at start of this function
    DisplayControl_SetActiveScreenIndex(SettingScreen_ID);
    SettingScreen_SetCallbackFunction();   
    SettingScreen_SetFlagShowConfirmScreen(false);
    if (AlarmExpression_IsActiveStatus())
    {
        SYS_PRINT("\n AlarmExpression resumed %d %d \n", gs_alarmData.id, gs_alarmData.currentPriority);
        AlarmExpression_SetAlarmState(eInitAlarmState);
    }
    else
    {
        RunningBar_DisplayRunningTime(true);
    }
    StatusBar_DisplayAll(true);
    SettingScreen_ResetScreenNoActionTimeout();
    
    // this reduce cycle to drawing target screen
    SettingScreen_Run(); 
    return eGTrue;
}

/** @brief Deinitialize settingscreen's control and data.
 *  @brief Cause the value of settingscreen's control, data is changed and this screen is created by ScreenCreate_MainScreen fnc.
 *  This fnc will remake all variable of each widget in settingscreen
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
E_GuiResult SettingScreen_Reinit(void)
{
    return eGCompleted;
}



/** @brief Implement SettingScreen's state, animation function and some action periodically
 *  It was called by DisplayControl.
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void SettingScreen_Run(void)
{   
    // Update alarm
    AlarmExpression_Run_SettingScreen();
    
    // Update running time
    RunningBar_DisplayRunningTime(false);
    
    // Update status icon
    StatusBar_DisplayAll(false);
    
    // Update screen
    SettingScreen_DisplayScreenSetting();
    
    // Check setting screen timeout
    if (SettingScreen_CheckScreenNoActionTimeout())
    {
        SettingScreen_SetFlagShowConfirmScreen(false); // discard confirm screen
        SettingScreen_DiscardMenuSetting();
        SettingScreen_SetNextScreenSetting(eBackToMainScreenScreenSetting);
    }
#ifdef JFLO_DEBUG_GUI
//    DisplayControl_TestAlarm();
#endif
    return;
}

/** @brief Distruct some SettingScreen backend elements when switched to other one
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void SettingScreen_Distructor(void)
{
    if (AlarmExpression_IsActiveState())
    {
        SYS_PRINT("\n AlarmExpression suspended %d %d \n", gs_alarmData.id, gs_alarmData.currentPriority);
        AlarmExpression_Deinit_SettingScreen();
    }
    setting_Restore();
 
}
void SettingScreen_DisplayScreenSetting_Common()
{
    // [W/A][tt] : move the panel out of the screen, laWidget_SetVisible not redraw correctly
    laWidget_SetX((laWidget*)SC_SaveConfirmPanel, 275);
    laWidget_SetVisible((laWidget*)SC_MenuSettingPanel, LA_FALSE);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingPanel, LA_FALSE);
    laWidget_SetVisible((laWidget*)SC_DataLogSettingPanel, LA_FALSE);
    laWidget_SetVisible((laWidget*)SC_MaintenancePanel, LA_FALSE);
    laWidget_SetVisible((laWidget*)SC_DeviceInformationPanel, LA_FALSE);
}
void SettingScreen_DisplayScreenSetting()
{
    if (!SettingScreen_GetSettingScreenUpdate())
        return;

    // check if screen switch with setting changed
    // this should be only occur with current display is eMenuScreenSetting, and switch to other screen
    if (SettingScreen_GetFlagShowConfirmScreen())
    {
        // when setting unsaved, display confirm screen
        SettingScreen_SetScreenSetting(eSaveConfimScreenSetting);
        SettingScreen_SetFlagShowConfirmScreen(false);
    }
    else
    {
        SettingScreen_SetScreenSetting(SettingScreen_GetNextScreenSetting());
        SettingScreen_SetMenuSetting(SettingScreen_GetNextMenuSetting());
    }
    
    SettingScreen_SetSettingScreenUpdate(false);
    
    switch(SettingScreen_GetScreenSetting())
    {
        case eMenuScreenSetting:
            SettingScreen_DisplayScreenSetting_Common();
            laWidget_SetVisible((laWidget*)SC_MenuSettingPanel, LA_TRUE);
            laWidget_SetX((laWidget*)SC_TabButtonUnderBarPanel, UNDERBAR_MENUSETTING_POSITION_X);
            laLabelWidget_SetText(SC_ScreenTitleLabel, laString_CreateFromID(string_text_SettingScreen_SETTING));
            SettingScreen_DisplayMenuSetting();
            break;
        case eDataLogScreenSetting:
            SettingScreen_DisplayScreenSetting_Common();
            laWidget_SetVisible((laWidget*)SC_DataLogSettingPanel, LA_TRUE);
            laWidget_SetX((laWidget*)SC_TabButtonUnderBarPanel, UNDERBAR_DATALOG_POSITION_X);
            laLabelWidget_SetText(SC_ScreenTitleLabel, laString_CreateFromID(string_text_SettingScreen_DATALOG));
            SettingScreen_DataLog_Display();
            break;
        case eMaintenanceScreenSetting:
            SettingScreen_DisplayScreenSetting_Common();
            laWidget_SetVisible((laWidget*)SC_MaintenancePanel, LA_TRUE);
            laWidget_SetX((laWidget*)SC_TabButtonUnderBarPanel, UNDERBAR_MAINTENANCE_POSITION_X);
            laLabelWidget_SetText(SC_ScreenTitleLabel, laString_CreateFromID(string_text_SettingScreen_MAINTENANCE));
            break;
        case eDeviceInformationScreenSetting:
            SettingScreen_DisplayScreenSetting_Common();
            laWidget_SetVisible((laWidget*)SC_DeviceInformationPanel, LA_TRUE);
            laWidget_SetX((laWidget*)SC_TabButtonUnderBarPanel, UNDERBAR_DEVICEINFORMATION_POSITION_X);
            laLabelWidget_SetText(SC_ScreenTitleLabel, laString_CreateFromID(string_text_SettingScreen_DEVICEINFORMATION));
            SettingScreen_DeviceInformation_Display();
            break;        
        case eSaveConfimScreenSetting:
            // [W/A][tt] : move the panel out of the screen, laWidget_SetVisible not redraw correctly
            laWidget_SetX((laWidget*)SC_SaveConfirmPanel, 0);
            break;
        case eBackToMainScreenScreenSetting:
            DisplayControl_SetState(eMainScreenIsShowingDispState);
        default:
            break;
    }
}
void SettingScreen_DisplayMenuSetting_CommonSet()
{
    laWidget_SetVisible((laWidget*)SC_MenuSettingPanel, LA_FALSE);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingPanel, LA_TRUE);

    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingLanguagePanel, LA_FALSE);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingDateTimePanel, LA_FALSE);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingWifiPanel, LA_FALSE);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingBrightnessPanel, LA_FALSE);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingOxySourcePanel, LA_FALSE);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingSpo2AlarmPanel, LA_FALSE);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingOxyAlarmPanel, LA_FALSE);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingAlarmSoundLevelPanel, LA_FALSE);
}
void SettingScreen_DisplayMenuSetting()
{
    switch(s_settingScreenData.menuSetting)
    {
        case eDefaultMenuSetting:
            //default screen in menu setting when init, so no need to update here
            break;
        case eLanguageMenuSetting:
            SettingScreen_DisplayMenuSetting_CommonSet();
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingLanguagePanel, LA_TRUE);            
            laLabelWidget_SetText(SC_MenuSetting_SettingTitleLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingTitle_LANGUAGE));
            laLabelWidget_SetText(SC_MenuSetting_SettingMessageLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingMessage_LANGUAGE)); 
            break;
        case eDateTimeMenuSetting:
            SettingScreen_DisplayMenuSetting_CommonSet();
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingDateTimePanel, LA_TRUE);           
            laLabelWidget_SetText(SC_MenuSetting_SettingTitleLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingTitle_DATETIME));
            laLabelWidget_SetText(SC_MenuSetting_SettingMessageLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingMessage_DATETIME));            
            break;
        case eWifiMenuSetting:
            SettingScreen_DisplayMenuSetting_CommonSet();
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingWifiPanel, LA_TRUE);            
            laLabelWidget_SetText(SC_MenuSetting_SettingTitleLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingTitle_WIFI));
            laLabelWidget_SetText(SC_MenuSetting_SettingMessageLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingMessage_WIFI));            
            break;
        case eBrightnessMenuSetting:
            SettingScreen_DisplayMenuSetting_CommonSet();
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingBrightnessPanel, LA_TRUE);            
            laLabelWidget_SetText(SC_MenuSetting_SettingTitleLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingTitle_BRIGHTNESS));
            laLabelWidget_SetText(SC_MenuSetting_SettingMessageLabel, laString_CreateFromID(string_text_Nullstring));
            break;
        case eOxygenSourceMenuSetting:
            SettingScreen_DisplayMenuSetting_CommonSet();
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingOxySourcePanel, LA_TRUE);            
            laLabelWidget_SetText(SC_MenuSetting_SettingTitleLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingTitle_OXYGENSOURCECONCENTRATION));
            laLabelWidget_SetText(SC_MenuSetting_SettingMessageLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingMessage_OXYGENSOURCECONCENTRATION));            
            break;
        case eSpO2AlarmLimitMenuSetting:
            SettingScreen_DisplayMenuSetting_CommonSet();
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingSpo2AlarmPanel, LA_TRUE);            
            laLabelWidget_SetText(SC_MenuSetting_SettingTitleLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingTitle_SPO2ALARMLIMIT));
            laLabelWidget_SetText(SC_MenuSetting_SettingMessageLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingMessage_SPO2ALARMLIMIT));            
            break;
        case eOxygenConcentrationAlarmLimitMenuSetting:
            SettingScreen_DisplayMenuSetting_CommonSet();
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingOxyAlarmPanel, LA_TRUE);
            laLabelWidget_SetText(SC_MenuSetting_SettingTitleLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingTitle_OXYGENCONCENTRATIONALARMLIMIT));
            laLabelWidget_SetText(SC_MenuSetting_SettingMessageLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingMessage_OXYGENCONCENTRATIONALARMLIMIT));            
            break;
        case eAlarmSoundLevelMenuSetting:
            SettingScreen_DisplayMenuSetting_CommonSet();
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingAlarmSoundLevelPanel, LA_TRUE);
            laLabelWidget_SetText(SC_MenuSetting_SettingTitleLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingTitle_ALARMSOUNDLEVEL));
            laLabelWidget_SetText(SC_MenuSetting_SettingMessageLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingMessage_ALARMSOUNDLEVEL));            
            break;
        default:
            break;
    }
    SettingScreen_DisplayMenuSetting_SubMenu();
    
}
 
void SettingScreen_SetScreenSetting(E_ScreenSetting screenSetting)
{
    if (screenSetting < 0 || screenSetting >= eNoOfScreenSetting)
        return;
    s_settingScreenData.screenSetting = screenSetting;
    s_settingScreenData.isSettingScreenUpdated = true;
}
E_ScreenSetting SettingScreen_GetScreenSetting()
{
    return s_settingScreenData.screenSetting;
}

void SettingScreen_SetNextScreenSetting(E_ScreenSetting screenSettingNext)
{
    if (screenSettingNext < 0 || screenSettingNext >= eNoOfScreenSetting)
        return;
    s_settingScreenData.screenSettingNext = screenSettingNext;
    s_settingScreenData.isSettingScreenUpdated = true;
}
E_ScreenSetting SettingScreen_GetNextScreenSetting()
{
    return s_settingScreenData.screenSettingNext;
}


void SettingScreen_SetMenuSetting(E_MenuSetting menuSetting)
{
    if (menuSetting < 0 || menuSetting >= eNoOfMenuSetting)
        return;
    s_settingScreenData.menuSetting = menuSetting;
    s_settingScreenData.isSettingScreenUpdated = true;
}
E_MenuSetting SettingScreen_GetMenuSetting()
{
    return s_settingScreenData.menuSetting;
}

void SettingScreen_SetNextMenuSetting(E_MenuSetting menuSetting)
{
    if (menuSetting < 0 || menuSetting >= eNoOfMenuSetting)
        return;
    s_settingScreenData.menuSettingNext = menuSetting;
    s_settingScreenData.isSettingScreenUpdated = true;
}
E_MenuSetting SettingScreen_GetNextMenuSetting()
{
    return s_settingScreenData.menuSettingNext;
}

void SettingScreen_SetDataLogSetting(E_DataLogSetting dataLogSetting)
{
    if (dataLogSetting < 0 || dataLogSetting >= eNoOfDataLogSetting)
        return;
    s_settingScreenData.dataLogSetting = dataLogSetting;    
    s_settingScreenData.isSettingScreenUpdated = true;
}
E_DataLogSetting SettingScreen_GetDataLogSetting()
{
    return s_settingScreenData.dataLogSetting;
}

void SettingScreen_SetFlagShowConfirmScreen(bool flag)
{
    s_settingScreenData.isShowConfirmScreen = flag;
}

bool SettingScreen_GetFlagShowConfirmScreen()
{
    return s_settingScreenData.isShowConfirmScreen;
}

void SettingScreen_CheckSettingDataChange()
{   
    // check if in menu screen setting
    if (s_settingScreenData.screenSetting != eMenuScreenSetting)
        return;       
    
    // check range enum, not check data in default menu screen
    if (s_settingScreenData.menuSetting < eDefaultMenuSetting || s_settingScreenData.menuSetting >= eNoOfMenuSetting )
        return;
        
    if (MENU_SETTING[s_settingScreenData.menuSetting].checkDataChangeFunc != 0)
    {
        SettingScreen_SetFlagShowConfirmScreen(MENU_SETTING[s_settingScreenData.menuSetting].checkDataChangeFunc());
    }
}

void SettingScreen_DisplayMenuSetting_SubMenu()
{
    SYS_PRINT(" SettingScreen_DisplaySetting %d \n",s_settingScreenData.menuSetting);
    if (s_settingScreenData.menuSetting < eDefaultMenuSetting || s_settingScreenData.menuSetting >= eNoOfMenuSetting )
        return;
        
    if (MENU_SETTING[s_settingScreenData.menuSetting].displayFunc != 0)
    {
        MENU_SETTING[s_settingScreenData.menuSetting].displayFunc();
    }
}

void SettingScreen_SaveMenuSetting()
{
    SYS_PRINT(" SettingScreen_SaveSetting %d \n",s_settingScreenData.menuSetting);
    if (s_settingScreenData.menuSetting < eDefaultMenuSetting || s_settingScreenData.menuSetting >= eNoOfMenuSetting )
        return;
        
    if (MENU_SETTING[s_settingScreenData.menuSetting].saveDataFunc != 0)
    {
        MENU_SETTING[s_settingScreenData.menuSetting].saveDataFunc();
    }
}

void SettingScreen_DiscardMenuSetting()
{
    if (s_settingScreenData.menuSetting < eDefaultMenuSetting || s_settingScreenData.menuSetting >= eNoOfMenuSetting )
        return;
        
    if (MENU_SETTING[s_settingScreenData.menuSetting].discardDataFunc != 0)
    {
        MENU_SETTING[s_settingScreenData.menuSetting].discardDataFunc();
    }    
}

void SettingScreen_SetSettingScreenUpdate(bool flag)
{
    if (flag)
    {
        SettingScreen_ResetScreenNoActionTimeout();
    }
    s_settingScreenData.isSettingScreenUpdated = flag;
}

bool SettingScreen_GetSettingScreenUpdate()
{
    return s_settingScreenData.isSettingScreenUpdated;
}

bool SettingScreen_CheckScreenNoActionTimeout()
{
    TickType_t delta = DisplayControl_CalculateDeltaTick(xTaskGetTickCount(), s_settingScreenData.xScreenNoActionTickCounter );
    if ( delta >= SETTING_SCREEN_NO_ACTION_TIMEOUT_MS)
    {
        SYS_PRINT("SettingScreen_CheckScreenNoActionTimeout timeout %d \n", delta);        
        SettingScreen_ResetScreenNoActionTimeout();
        return true;
    }   
    return false;    
}

void SettingScreen_ResetScreenNoActionTimeout()
{
    SYS_PRINT("SettingScreen_ResetScreenNoActionTimeout\n");        
    s_settingScreenData.xScreenNoActionTickCounter = xTaskGetTickCount();
}

void SettingScreen_GoToScreen_MenuSetting()
{
    // check if current screen is setting screen
    if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        SettingScreen_CheckSettingDataChange();
    }
    SettingScreen_SetNextScreenSetting(eMenuScreenSetting);    
    SettingScreen_SetNextMenuSetting(eDefaultMenuSetting);
}
void SettingScreen_GoToScreen_DataLogSetting()
{
    // check if current screen is setting screen
    if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        SettingScreen_CheckSettingDataChange();
    }
    SettingScreen_SetNextScreenSetting(eDataLogScreenSetting);
    SettingScreen_DataLog_Init();
}
void SettingScreen_GoToScreen_Maintenance()
{
    // check if current screen is setting screen
    if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        SettingScreen_CheckSettingDataChange();
    }
    SettingScreen_SetNextScreenSetting(eMaintenanceScreenSetting);
}
void SettingScreen_GoToScreen_DeviceInformation()
{
    // check if current screen is setting screen
    if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        SettingScreen_CheckSettingDataChange();
    }
    SettingScreen_SetNextScreenSetting(eDeviceInformationScreenSetting);
    SettingScreen_DeviceInformation_Init();
}

void SettingScreen_GoToScreen_BackToMainScreen()
{
    // check if current screen is setting screen
    if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        SettingScreen_CheckSettingDataChange();
    }
    SettingScreen_SetNextScreenSetting(eBackToMainScreenScreenSetting);
}



// end of file
