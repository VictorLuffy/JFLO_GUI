/** @file SettingScreen_BrightnessSetting.c
 *  @brief The interfaces for display brightness setting
 *  @author Trac Truong
 */

/* This section lists the other files that are included in this file.
 */
#include "Gui/SettingScreen_BrightnessSetting.h"

#ifdef UNIT_TEST
laWidget* SC_MenuSetting_SettingBrightness_PrevButton = (laWidget*)0;
laWidget* SC_MenuSetting_SettingBrightness_NextButton = (laWidget*)1;
laLabelWidget* SC_MenuSetting_SettingBrightness_ValueLabel = (laLabelWidget*)2;
laWidget* SC_MenuSetting_SettingBrightness_UnderBar = (laWidget*)3;
GFXU_FontAsset BebasNeueBook_S60_Bold_Internal;
laLabelWidget* SC_MenuSetting_SettingBrightness_OffLabel = (laLabelWidget*)4;
laLabelWidget* SC_MenuSetting_SettingBrightness_OnLabel = (laLabelWidget*)5;
#endif

extern void SettingScreen_SetSettingScreenUpdate(bool f);
static int s_brightnessLevel = -1;
static int s_brightnessLevelInit = -1;
static int s_brightnessLevelDisplay = -1;

static int s_brightnessMode = -1;
static int s_brightnessModeInit = -1;
static int s_brightnessModeDisplay = -1;


void SettingScreen_BrightnessSetting_Init()
{
    s_brightnessLevelInit = setting_Get(eBrightnessLevelSettingId);
    s_brightnessLevel = s_brightnessLevelInit;
    s_brightnessLevelDisplay = -1;
    
    s_brightnessModeInit = setting_Get(eBrightnessModeSettingId);
    s_brightnessMode = s_brightnessModeInit;
    s_brightnessModeDisplay = -1;

}

void SettingScreen_BrightnessSetting_Display()
{
    if (s_brightnessModeDisplay !=  s_brightnessMode)
    {
        SYS_PRINT("SettingScreen_BrightnessSetting_Display mode %d \n", s_brightnessMode);
        if (s_brightnessMode == eAutoBrightnessMode)
        {
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingBrightness_PrevButton, LA_FALSE);
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingBrightness_NextButton, LA_FALSE);
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingBrightness_ValueLabel, LA_FALSE);
            laWidget_SetY((laWidget*)SC_MenuSetting_SettingBrightness_UnderBar, UNDERBAR_ON_POSITION_Y);
        }
        else if (s_brightnessMode == eManualBrightnessMode)
        {
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingBrightness_PrevButton, LA_TRUE);
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingBrightness_NextButton, LA_TRUE);
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingBrightness_ValueLabel, LA_TRUE);            
            laWidget_SetY((laWidget*)SC_MenuSetting_SettingBrightness_UnderBar, UNDERBAR_OFF_POSITION_Y);
        }
        else{}
        s_brightnessModeDisplay = s_brightnessMode;
    }
    
    // only update if value change in manual mode
    if (s_brightnessMode == eManualBrightnessMode && s_brightnessLevelDisplay != s_brightnessLevel)
    {
        SYS_PRINT("SettingScreen_BrightnessSetting_Display level %d \n", s_brightnessLevel);
        char strbuff[5];
        laString str;
        sprintf(strbuff, "%d", s_brightnessLevel);
        str = laString_CreateFromCharBuffer(strbuff, &BebasNeueBook_S60_Bold_Internal);      
        laLabelWidget_SetText(SC_MenuSetting_SettingBrightness_ValueLabel, str);
        laString_Destroy(&str); 

        s_brightnessLevelDisplay = s_brightnessLevel;
    }
}
void SC_MenuSetting_SettingBrightness_OffLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    if (widget == 0 || evt == 0)
        return;
    SettingScreen_BrightnessSetting_SetModeSetting(eManualBrightnessMode);
    
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}

void SC_MenuSetting_SettingBrightness_OnLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    if (widget == 0 || evt == 0)
        return;

    SettingScreen_BrightnessSetting_SetModeSetting(eAutoBrightnessMode);
    
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}

void SettingScreen_BrightnessSetting_SetCallbackFunction(void)
{
    // Assigning the callback function to widget, these event cannot auto add by generator
    laWidget_OverrideTouchDownEvent((laWidget*)SC_MenuSetting_SettingBrightness_OffLabel, &SC_MenuSetting_SettingBrightness_OffLabel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_MenuSetting_SettingBrightness_OnLabel, &SC_MenuSetting_SettingBrightness_OnLabel_PressedEvent);   
}

void SettingScreen_BrightnessSetting_Inc()
{
    uint8_t max = setting_GetMax(eBrightnessLevelSettingId);
    uint8_t step = setting_GetStep(eBrightnessLevelSettingId);
    s_brightnessLevel = s_brightnessLevel + step;
    s_brightnessLevel = (s_brightnessLevel > max) ? max : s_brightnessLevel;
    setting_Set(eBrightnessLevelSettingId, s_brightnessLevel);
    SYS_PRINT("SettingScreen_BrightnessSetting_Inc %d \n", s_brightnessLevel);
    if (s_brightnessLevelDisplay != s_brightnessLevel)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}

void SettingScreen_BrightnessSetting_Dec()
{
    uint8_t min = setting_GetMin(eBrightnessLevelSettingId);
    uint8_t step = setting_GetStep(eBrightnessLevelSettingId);
    s_brightnessLevel = s_brightnessLevel - step;
    s_brightnessLevel = (s_brightnessLevel < min) ? min : s_brightnessLevel;
    setting_Set(eBrightnessLevelSettingId, s_brightnessLevel);
    SYS_PRINT("SettingScreen_BrightnessSetting_Inc %d \n", s_brightnessLevel);
    if (s_brightnessLevelDisplay != s_brightnessLevel)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}

void SettingScreen_BrightnessSetting_SetLevelSetting(int s)
{
    uint8_t min = setting_GetMin(eBrightnessLevelSettingId);
    uint8_t max = setting_GetMax(eBrightnessLevelSettingId);
    
    if ( s < min || s > max)
        return;
    s_brightnessLevel = s;
    setting_Set(eBrightnessLevelSettingId, s_brightnessLevel);
    if (s_brightnessLevelDisplay != s_brightnessLevel)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}
int SettingScreen_BrightnessSetting_GetLevelSetting()
{
    return s_brightnessLevel;
}

void SettingScreen_BrightnessSetting_SetModeSetting(E_BrightnessMode s)
{   
    if ( s < 0 || s >= eNoOfBrightnessMode)
        return;
    s_brightnessMode = s;
    SYS_PRINT("SettingScreen_BrightnessSetting_SetModeSetting %d \n", s_brightnessMode);
    setting_Set(eBrightnessModeSettingId, s_brightnessMode);
    if (s_brightnessModeDisplay != s_brightnessMode)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}
E_BrightnessMode SettingScreen_BrightnessSetting_GetModeSetting()
{
    return s_brightnessMode;
}

bool SettingScreen_BrightnessSetting_CheckDataChange()
{
    return ( s_brightnessLevelInit == s_brightnessLevel && s_brightnessModeInit == s_brightnessMode ) ? false : true;
}

void SettingScreen_BrightnessSetting_SaveSetting()
{
    if (!SettingScreen_BrightnessSetting_CheckDataChange())
    {
        SYS_PRINT("SettingScreen_BrightnessSetting_SaveSetting no change \n");
        return;
    }
    SYS_PRINT("SettingScreen_BrightnessSetting_SaveSetting \n");
    
    setting_Save();

    // Write a log
    uint8_t logData[2];
    if (s_brightnessLevelInit != s_brightnessLevel)
    {
        logData[0] = s_brightnessLevelInit;
        logData[1] = s_brightnessLevel;
        logInterface_WriteEventLog(2,(void*)logData , eChangeIlluminanceLevelSettingEventLogId);          
    }
    if (s_brightnessModeInit != s_brightnessMode)
    {
        if (s_brightnessMode == eManualBrightnessMode)
        {
            logData[0] = ILLUMINANCE_MODE_A_TO_M >> 8;
            logData[1] = (uint8_t)ILLUMINANCE_MODE_A_TO_M;
        }
        else
        {
            logData[0] = ILLUMINANCE_MODE_M_TO_A >> 8;
            logData[1] = (uint8_t)ILLUMINANCE_MODE_M_TO_A;            
        }
        logInterface_WriteEventLog(2,(void*)logData , eChangeIlluminanceSettingEventLogId);   
    }
    // update init data to not show confirm screen
    s_brightnessLevelInit = s_brightnessLevel;
    s_brightnessModeInit = s_brightnessMode;
}

void SettingScreen_BrightnessSetting_DiscardSetting()
{
    SYS_PRINT("SettingScreen_BrightnessSetting_DiscardSetting \n");
    s_brightnessLevel = s_brightnessLevelInit;
    s_brightnessMode = s_brightnessModeInit;
}

// end of file

