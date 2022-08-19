/** @file SettingScreen_OxySourceSetting.c
 *  @brief The interfaces for display oxy source setting
 *  @author Trac Truong
 */

/* This section lists the other files that are included in this file.
 */
#include "Gui/SettingScreen_OxySourceSetting.h"

#ifdef UNIT_TEST
laWidget* SC_MenuSetting_SettingOxySource_90Label = (laWidget*)1;
laWidget* SC_MenuSetting_SettingOxySource_UnderBarPanel = (laWidget*)2;
laWidget* SC_MenuSetting_SettingOxySource_100Label = (laWidget*)3;

#endif

extern void SettingScreen_SetSettingScreenUpdate(bool f);
extern int32_t DisplayControl_GetCenterPostion(laWidget* widgetA, laWidget* widgetB);
static int s_oxygenSource = -1;
static int s_oxygenSourceInit = -1;
static int s_oxygenSourceDisplay = -1;

void SettingScreen_OxySourceSetting_Init()
{
    s_oxygenSourceInit = setting_Get(eOxygenSourceSettingId);
    s_oxygenSource = s_oxygenSourceInit;
    s_oxygenSourceDisplay = -1;
}

void SettingScreen_OxySourceSetting_Display()
{
    if (s_oxygenSourceDisplay == s_oxygenSource)
    {
        return;
    }
    int32_t x;
    switch (s_oxygenSource)
    {
        case e90OxygenSource:
            x = DisplayControl_GetCenterPostion((laWidget*)SC_MenuSetting_SettingOxySource_90Label, (laWidget*)SC_MenuSetting_SettingOxySource_UnderBarPanel);
            laWidget_SetX((laWidget*)SC_MenuSetting_SettingOxySource_UnderBarPanel, x);
            break;
        case e100OxygenSource:
            x = DisplayControl_GetCenterPostion((laWidget*)SC_MenuSetting_SettingOxySource_100Label, (laWidget*)SC_MenuSetting_SettingOxySource_UnderBarPanel);
            laWidget_SetX((laWidget*)SC_MenuSetting_SettingOxySource_UnderBarPanel, x);
            break;
        default:
            break;            
    }
    s_oxygenSourceDisplay = s_oxygenSource;
}

void SC_MenuSetting_SettingOxySource_90Label_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    if (widget == 0 || evt == 0)
        return;

    SettingScreen_OxySourceSetting_SetSetting(e90OxygenSource);
    
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}

void SC_MenuSetting_SettingOxySource_100Label_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    if (widget == 0 || evt == 0)
        return;

    SettingScreen_OxySourceSetting_SetSetting(e100OxygenSource);
    
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}

void SettingScreen_OxySourceSetting_SetCallbackFunction(void)
{
    // Assigning the callback function to widget, these event cannot auto add by generator
    laWidget_OverrideTouchDownEvent((laWidget*)SC_MenuSetting_SettingOxySource_90Label, &SC_MenuSetting_SettingOxySource_90Label_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_MenuSetting_SettingOxySource_100Label, &SC_MenuSetting_SettingOxySource_100Label_PressedEvent);    
}

void SettingScreen_OxySourceSetting_SetSetting(E_OxygenSource s)
{
    s_oxygenSource = s;
    if (s_oxygenSourceDisplay != s_oxygenSource)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}
E_OxygenSource SettingScreen_OxySourceSetting_GetSetting()
{
    return s_oxygenSource;
}

bool SettingScreen_OxySourceSetting_CheckDataChange()
{
    return ( s_oxygenSourceInit == s_oxygenSource ) ? false : true;
}

void SettingScreen_OxySourceSetting_SaveSetting()
{
    if (!SettingScreen_OxySourceSetting_CheckDataChange())
    {
        SYS_PRINT("SettingScreen_OxySourceSetting_SaveSetting no change \n");
        return;
    }
    SYS_PRINT("SettingScreen_OxySourceSetting_SaveSetting \n");
    
    setting_Set(eOxygenSourceSettingId, s_oxygenSource);
    
    // Write a log
    uint8_t logData[2];
    logData[0] = s_oxygenSourceInit;
    logData[1] = s_oxygenSource;
    logInterface_WriteEventLog(2,(void*)logData , eOxygenSourceSettingChangeUpperLimitEventLogId); 

    
    uint8_t oxyLimitUpper = setting_Get(eOxygenAlarmSettingUpperLimitId);
    
    // limit max upper limit as oxygen source
    if (s_oxygenSource == e90OxygenSource && oxyLimitUpper > 90)
    {
        setting_Set(eOxygenAlarmSettingUpperLimitId, 90);
        
        // Write a log
        uint8_t logData[2];
        logData[0] = oxyLimitUpper;
        logData[1] = 90;
        logInterface_WriteEventLog(2,(void*)logData , eOxygenAlarmSettingChangeUpperLimitEventLogId); 

    }
    setting_Save();
    
    // update init data to not show confirm screen
    s_oxygenSourceInit = s_oxygenSource;
}

void SettingScreen_OxySourceSetting_DiscardSetting()
{
    SYS_PRINT("SettingScreen_OxySourceSetting_DiscardSetting \n");
    s_oxygenSource = s_oxygenSourceInit;
}

// end of file
