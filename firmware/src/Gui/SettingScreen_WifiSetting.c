/** @file SettingScreen_WifiSetting.c
 *  @brief The interfaces for display wifi setting
 *  @author Trac Truong
 */

/* This section lists the other files that are included in this file.
 */
#include "Gui/SettingScreen_WifiSetting.h"

extern void SettingScreen_SetSettingScreenUpdate(bool f);

static E_OnOff s_wifiStatus = -1;
static E_OnOff s_wifiStatusInit = -1;
static E_OnOff s_wifiStatusDisplay = -1;

void SettingScreen_WifiSetting_Init()
{
    s_wifiStatusInit = setting_Get(eWifiSettingId);
    s_wifiStatus = s_wifiStatusInit;
    s_wifiStatusDisplay = -1;
}

void SettingScreen_WifiSetting_Display()
{
    if (s_wifiStatusDisplay == s_wifiStatus)
    {
        return;
    }
    int32_t x;
    switch (s_wifiStatus)
    {
        case eSettingOff:
            x = DisplayControl_GetCenterPostion((laWidget*)SC_MenuSetting_SettingWifi_OffLabel, (laWidget*)SC_MenuSetting_SettingWifi_UnderBarPanel);
            laWidget_SetX((laWidget*)SC_MenuSetting_SettingWifi_UnderBarPanel, x);
            break;
        case eSettingOn:
            x = DisplayControl_GetCenterPostion((laWidget*)SC_MenuSetting_SettingWifi_OnLabel, (laWidget*)SC_MenuSetting_SettingWifi_UnderBarPanel);
            laWidget_SetX((laWidget*)SC_MenuSetting_SettingWifi_UnderBarPanel, x);
            break;
        default:
            break;            
    }
    s_wifiStatusDisplay = s_wifiStatus;
}

void SC_MenuSetting_SettingWifi_OffLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    if (widget == 0 || evt == 0)
        return;
    SettingScreen_WifiSetting_SetStatus(eSettingOff);
    
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}

void SC_MenuSetting_SettingWifi_OnLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    if (widget == 0 || evt == 0)
        return;

    SettingScreen_WifiSetting_SetStatus(eSettingOn);
    
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}

void SettingScreen_WifiSetting_SetCallbackFunction(void)
{
    // Assigning the callback function to widget, these event cannot auto add by generator
    laWidget_OverrideTouchDownEvent((laWidget*)SC_MenuSetting_SettingWifi_OffLabel, &SC_MenuSetting_SettingWifi_OffLabel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_MenuSetting_SettingWifi_OnLabel, &SC_MenuSetting_SettingWifi_OnLabel_PressedEvent);    
}

void SettingScreen_WifiSetting_SetStatus(E_OnOff s)
{
    s_wifiStatus = s;
    if (s_wifiStatusDisplay != s_wifiStatus)
    {
        if (s_wifiStatus == eSettingOff)
        {
            StatusBar_SetWifiStatus(eWifiOff);
        }
        else
        {
            StatusBar_SetWifiStatus(eWifiLevel4);
        }
        SettingScreen_SetSettingScreenUpdate(true);
    }
}
E_OnOff SettingScreen_WifiSetting_GetStatus()
{
    return s_wifiStatus;
}

bool SettingScreen_WifiSetting_CheckDataChange()
{
    return ( s_wifiStatusInit == s_wifiStatus ) ? false : true;
}

void SettingScreen_WifiSetting_SaveSetting()
{
    if (!SettingScreen_WifiSetting_CheckDataChange())
    {
        SYS_PRINT("SettingScreen_WifiSetting_SaveSetting no change \n");
        return;
    }
    SYS_PRINT("SettingScreen_WifiSetting_SaveSetting \n");
    
    setting_SetWifi(s_wifiStatus);
    
    // update init data to not show confirm screen
    s_wifiStatusInit = s_wifiStatus;
}

void SettingScreen_WifiSetting_DiscardSetting()
{
    SYS_PRINT("SettingScreen_WifiSetting_DiscardSetting \n");
    if (s_wifiStatusInit == eSettingOff)
    {
        StatusBar_SetWifiStatus(eWifiOff);
    }
    else
    {
        StatusBar_SetWifiStatus(eWifiLevel4);
    }
    s_wifiStatus = s_wifiStatusInit;
}

// end of file
