/** @file StatusBar.c
 *  @brief The interfaces for display status bar
 *  @author Trac Truong
 */

/* This section lists the other files that are included in this file.
 */
#include <stdio.h>
#include "Gui/StatusBar.h"

#ifdef UNIT_TEST
uint32_t stringTable = 2;
laImageWidget* imgEXTBattery = (laImageWidget*) 1;
laImageWidget* SC_ExternalBatteryIcon = (laImageWidget*) 2;

laImageWidget* imgINTBattery = (laImageWidget*) 3;
laImageWidget* SC_InternalBatteryIcon  = (laImageWidget*) 4;

laImageWidget* imgSocket  = (laImageWidget*) 5;
laWidget* SC_AlarmInfoButton  = (laImageWidget*) 6;
laWidget *imgWifi = (laWidget*) 7;
laWidget *SC_WifiIcon= (laWidget*) 8;
laWidget *SC_PowerACIcon = (laWidget*) 9;
laWidget *btnInfo= (laWidget*) 10;
laWidget *tfCurrentTime= (laWidget*) 11;
laWidget *SC_DateTimeTextField= (laWidget*) 12;


GFXU_ImageAsset iconExtBattery_0;
GFXU_ImageAsset iconExtBattery_1;
GFXU_ImageAsset iconExtBattery_2;
GFXU_ImageAsset iconExtBattery_3;
GFXU_ImageAsset iconExtBattery_4;
GFXU_ImageAsset iconIntBattery_0;
GFXU_ImageAsset iconIntBattery_1;
GFXU_ImageAsset iconIntBattery_2;
GFXU_ImageAsset iconIntBattery_3;
GFXU_ImageAsset iconIntBattery_4;
GFXU_ImageAsset Icon_InternalBatteryNoInserted;
GFXU_ImageAsset iconWifi_0;
GFXU_ImageAsset iconWifi_1;
GFXU_ImageAsset iconWifi_2;
GFXU_ImageAsset iconWifi_3;
GFXU_ImageAsset iconWifi_4;
GFXU_ImageAsset iconSocket;
GFXU_ImageAsset Icon_SocketDisconnected;

uint32_t string_Nums_BebasNeueBook_S20_Bold;
#endif


/** @brief A varible to store the status bar data */
static StatusBar_Data_Struct s_StatusBarData;

void StatusBar_Init()
{  
    s_StatusBarData.externalBatteryPreStatus = eBatteryDisconnect;
    s_StatusBarData.internalBatteryPreStatus = eBatteryDisconnect;
    s_StatusBarData.acPowerPreStatus = eACPowerDisconnect;
    s_StatusBarData.wifiPreStatus = eWifiOff;
    s_StatusBarData.alarmInfoButtonShowPre = false;
    
    if (setting_Get(eWifiSettingId) == eSettingOff)
    {
        StatusBar_SetWifiStatus(eWifiOff);
    }
    else
    {
        StatusBar_SetWifiStatus(eWifiLevel4);
    }
}

void StatusBar_SetExternalBatteryStatus(E_BatteryStatus status)
{
    s_StatusBarData.externalBatteryStatus = status;
}
E_BatteryStatus StatusBar_GetExternalBatteryStatus()
{
    return s_StatusBarData.externalBatteryStatus;
}
void StatusBar_SetInternalBatteryStatus(E_BatteryStatus status)
{
    s_StatusBarData.internalBatteryStatus = status;
}
E_BatteryStatus StatusBar_GetInternalBatteryStatus()
{
    return s_StatusBarData.internalBatteryStatus;
}
void StatusBar_SetACPowerStatus(E_ACPowerStatus status)
{
    s_StatusBarData.acPowerStatus = status;
}
E_ACPowerStatus StatusBar_GetACPowerStatus()
{
    return s_StatusBarData.acPowerStatus;
}
void StatusBar_SetWifiStatus(E_WifiStatus status)
{
    s_StatusBarData.wifiStatus = status;
}
E_WifiStatus StatusBar_GetWifiStatus()
{
    return s_StatusBarData.wifiStatus;
}

void StatusBar_DisplayExternalBatteryStatus(bool isForceUpdate)
{
    if (!isForceUpdate && s_StatusBarData.externalBatteryPreStatus == s_StatusBarData.externalBatteryStatus)
        return;
    laWidget* widget;
    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
    {
        widget = (laWidget*)imgEXTBattery;
    }
    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        widget = (laWidget*)SC_ExternalBatteryIcon;
    }
    else{
        return;
    }
    switch(s_StatusBarData.externalBatteryStatus)
    {
        case eBatteryDisconnect:
            laWidget_SetVisible(widget, LA_FALSE);
            break;
        case eBatteryLevel0:
            laWidget_SetVisible(widget, LA_TRUE);
            laImageWidget_SetImage((laImageWidget*)widget, &iconExtBattery_0);
            break;
        case eBatteryLevel1:
            laWidget_SetVisible(widget, LA_TRUE);
            laImageWidget_SetImage((laImageWidget*)widget, &iconExtBattery_1);
            break;
        case eBatteryLevel2:
            laWidget_SetVisible(widget, LA_TRUE);
            laImageWidget_SetImage((laImageWidget*)widget, &iconExtBattery_2);
            break;
        case eBatteryLevel3:
            laWidget_SetVisible(widget, LA_TRUE);
            laImageWidget_SetImage((laImageWidget*)widget, &iconExtBattery_3);
            break;
        case eBatteryLevel4:
            laWidget_SetVisible(widget, LA_TRUE);
            laImageWidget_SetImage((laImageWidget*)widget, &iconExtBattery_4);            
            break;
        default:
            break;
    }
    s_StatusBarData.externalBatteryPreStatus = s_StatusBarData.externalBatteryStatus;
}
void StatusBar_DisplayInternalBatteryStatus(bool isForceUpdate)
{
    if (!isForceUpdate && s_StatusBarData.internalBatteryPreStatus == s_StatusBarData.internalBatteryStatus)
        return;
    laImageWidget* widget;
    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
    {
        widget = imgINTBattery;
    }
    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        widget = SC_InternalBatteryIcon;
    }
    else{
        return;
    }
    switch(s_StatusBarData.internalBatteryStatus)
    {
        case eBatteryDisconnect:
            laImageWidget_SetImage(widget, &Icon_InternalBatteryNoInserted);
            break;
        case eBatteryLevel0:
            laImageWidget_SetImage(widget, &iconIntBattery_0);
            break;
        case eBatteryLevel1:
            laImageWidget_SetImage(widget, &iconIntBattery_1);
            break;
        case eBatteryLevel2:
            laImageWidget_SetImage(widget, &iconIntBattery_2);
            break;
        case eBatteryLevel3:
            laImageWidget_SetImage(widget, &iconIntBattery_3);
            break;
        case eBatteryLevel4:
            laImageWidget_SetImage(widget, &iconIntBattery_4);            
            break;
            
        default:
            break;
    }
    s_StatusBarData.internalBatteryPreStatus = s_StatusBarData.internalBatteryStatus;
}
void StatusBar_DisplayACPowerStatus(bool isForceUpdate)
{
    if (!isForceUpdate && s_StatusBarData.acPowerPreStatus == s_StatusBarData.acPowerStatus)
        return;
    laImageWidget* widget;
    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
    {
        widget = imgSocket;
    }
    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        widget = SC_PowerACIcon;
    }
    else{
        return;
    }
    switch(s_StatusBarData.acPowerStatus)
    {
        case eACPowerConnect:
            laImageWidget_SetImage(widget, &iconSocket);
            break;
        case eACPowerDisconnect:
            laImageWidget_SetImage(widget, &Icon_SocketDisconnected);
            break;
        default:
            break;
    }
    s_StatusBarData.acPowerPreStatus = s_StatusBarData.acPowerStatus;
}

void StatusBar_DisplayWifiStatus(bool isForceUpdate)
{
    if (!isForceUpdate && s_StatusBarData.wifiPreStatus == s_StatusBarData.wifiStatus)
        return;
    laWidget* widget;
    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
    {
        widget = (laWidget*)imgWifi;
    }
    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        widget = (laWidget*)SC_WifiIcon;
    }
    else{
        return;
    }
    
    switch(s_StatusBarData.wifiStatus)
    {
        case eWifiOff:
            laWidget_SetVisible(widget, LA_FALSE);
            break;
        case eWifiLevel1:
            laWidget_SetVisible(widget, LA_TRUE);
            laImageWidget_SetImage((laImageWidget*)widget, &iconWifi_1);
            break;
        case eWifiLevel2:
            laWidget_SetVisible(widget, LA_TRUE);
            laImageWidget_SetImage((laImageWidget*)widget, &iconWifi_2);
            break;
        case eWifiLevel3:
            laWidget_SetVisible(widget, LA_TRUE);
            laImageWidget_SetImage((laImageWidget*)widget, &iconWifi_3);
            break;
        case eWifiLevel4:
            laWidget_SetVisible(widget, LA_TRUE);
            laImageWidget_SetImage((laImageWidget*)widget, &iconWifi_4);
            break;            
        default:
            break;
    }
    s_StatusBarData.wifiPreStatus = s_StatusBarData.wifiStatus;        
}

void StatusBar_DisplayAll(bool isForceUpdate)
{
    StatusBar_DisplayWifiStatus(isForceUpdate);
    StatusBar_DisplayTime(isForceUpdate);
    StatusBar_DisplayExternalBatteryStatus(isForceUpdate);
    StatusBar_DisplayInternalBatteryStatus(isForceUpdate);
    StatusBar_DisplayACPowerStatus(isForceUpdate);
    StatusBar_DisplayAlarmInfo(isForceUpdate);
}
void StatusBar_DisplayTime(bool isForceUpdate)
{
    static TickType_t lastTick = 0 ; 
    TickType_t currentTick = xTaskGetTickCount() ; 
    TickType_t delta = DisplayControl_CalculateDeltaTick(currentTick, lastTick);
    
    if (isForceUpdate || delta >= DATETIME_UPDATE_CYCLE_MS)
    {
        lastTick = currentTick;
    }
    else 
        return;
    
    char strbuff[10];
    laString strTime;

    RTC_TIME_t time = DisplayControl_GetTime();
    
    sprintf(strbuff, "%.2d:%.2d", time.HOUR, time.MIN);
    strTime = laString_CreateFromCharBuffer(strbuff, 
        GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S20_Bold, setting_Get(eLanguageSettingId)));

    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
    {
        laTextFieldWidget_SetText(tfCurrentTime, strTime );
    }
    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        laTextFieldWidget_SetText(SC_DateTimeTextField, strTime );       
    }
    else {}
    laString_Destroy(&strTime); 
}

void StatusBar_SetAlarmInfo(bool f)
{
    s_StatusBarData.alarmInfoButtonShow = f;
}

bool StatusBar_GetAlarmInfo()
{
    return s_StatusBarData.alarmInfoButtonShow;
}

void StatusBar_DisplayAlarmInfo(bool isForceUpdate)
{
    if (!isForceUpdate && s_StatusBarData.alarmInfoButtonShowPre == s_StatusBarData.alarmInfoButtonShow)
        return;
    laWidget* widget;
    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
    {
        widget = (laWidget*)btnInfo;
    }
    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        widget = (laWidget*)SC_AlarmInfoButton;
    }
    else{
        return;
    }
    laWidget_SetVisible(widget, s_StatusBarData.alarmInfoButtonShow ? LA_TRUE : LA_FALSE);
    s_StatusBarData.alarmInfoButtonShowPre = s_StatusBarData.alarmInfoButtonShow;
}

// end of file

