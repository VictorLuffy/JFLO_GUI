/** @file SettingScreen_SpO2AlarmLimitSetting.c
 *  @brief The interfaces for display spo2 alarm limit setting
 *  @author Trac Truong
 */

/* This section lists the other files that are included in this file.
 */
#include "Gui/SettingScreen_SpO2AlarmLimitSetting.h"

#ifdef UNIT_TEST
laLabelWidget* SC_MenuSetting_SettingSpo2Alarm_ValueLabel = (laLabelWidget*)1;
GFXU_FontAsset BebasNeueBook_S60_Bold_Internal;
#endif

extern void SettingScreen_SetSettingScreenUpdate(bool f);
static int s_spo2AlarmLimitLower = -1;
static int s_spo2AlarmLimitLowerInit = -1;
static int s_spo2AlarmLimitLowerDisplay = -1;

void SettingScreen_SpO2AlarmLimitSetting_Init()
{
    s_spo2AlarmLimitLowerInit = setting_Get(eSpO2AlarmSettingLowerLimitId);
    s_spo2AlarmLimitLower = s_spo2AlarmLimitLowerInit;
    s_spo2AlarmLimitLowerDisplay = -1;
}

void SettingScreen_SpO2AlarmLimitSetting_Display()
{
    if (s_spo2AlarmLimitLowerDisplay == s_spo2AlarmLimitLower)
    {
        return;
    }
    SYS_PRINT("SettingScreen_SpO2AlarmLimitSetting_Display %d \n", s_spo2AlarmLimitLower);
    char strbuff[5];
    laString str;
    sprintf(strbuff, "%d%%", s_spo2AlarmLimitLower);
    str = laString_CreateFromCharBuffer(strbuff, &BebasNeueBook_S60_Bold_Internal);      
    laLabelWidget_SetText(SC_MenuSetting_SettingSpo2Alarm_ValueLabel, str);
    laString_Destroy(&str); 

    s_spo2AlarmLimitLowerDisplay = s_spo2AlarmLimitLower;
}

void SettingScreen_SpO2AlarmLimitSetting_SetCallbackFunction(void)
{
    // Assigning the callback function to widget, these event cannot auto add by generator
}

void SettingScreen_SpO2AlarmLimitSetting_Inc()
{
    uint8_t max = setting_GetMax(eSpO2AlarmSettingLowerLimitId);
    uint8_t step = setting_GetStep(eSpO2AlarmSettingLowerLimitId);
    s_spo2AlarmLimitLower = s_spo2AlarmLimitLower + step;
    s_spo2AlarmLimitLower = (s_spo2AlarmLimitLower > max) ? max : s_spo2AlarmLimitLower;
    SYS_PRINT("SettingScreen_SpO2AlarmLimitSetting_Inc %d \n", s_spo2AlarmLimitLower);
    if (s_spo2AlarmLimitLowerDisplay != s_spo2AlarmLimitLower)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}

void SettingScreen_SpO2AlarmLimitSetting_Dec()
{
    uint8_t min = setting_GetMin(eSpO2AlarmSettingLowerLimitId);
    uint8_t step = setting_GetStep(eSpO2AlarmSettingLowerLimitId);
    s_spo2AlarmLimitLower = s_spo2AlarmLimitLower - step;
    s_spo2AlarmLimitLower = (s_spo2AlarmLimitLower < min) ? min : s_spo2AlarmLimitLower;
    SYS_PRINT("SettingScreen_SpO2AlarmLimitSetting_Inc %d \n", s_spo2AlarmLimitLower);
    if (s_spo2AlarmLimitLowerDisplay != s_spo2AlarmLimitLower)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }

}

void SettingScreen_SpO2AlarmLimitSetting_SetSetting(int s)
{
    uint8_t min = setting_GetMin(eSpO2AlarmSettingLowerLimitId);
    uint8_t max = setting_GetMax(eSpO2AlarmSettingLowerLimitId);
    
    if ( s < min || s > max)
        return;
    
    s_spo2AlarmLimitLower = s;
    if (s_spo2AlarmLimitLowerDisplay != s_spo2AlarmLimitLower)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}
int SettingScreen_SpO2AlarmLimitSetting_GetSetting()
{
    return s_spo2AlarmLimitLower;
}

bool SettingScreen_SpO2AlarmLimitSetting_CheckDataChange()
{
    return ( s_spo2AlarmLimitLowerInit == s_spo2AlarmLimitLower ) ? false : true;
}

void SettingScreen_SpO2AlarmLimitSetting_SaveSetting()
{
    if (!SettingScreen_SpO2AlarmLimitSetting_CheckDataChange())
    {
        SYS_PRINT("SettingScreen_SpO2AlarmLimitSetting_SaveSetting no change \n");
        return;
    }
    SYS_PRINT("SettingScreen_SpO2AlarmLimitSetting_SaveSetting \n");
    
    setting_Set(eSpO2AlarmSettingLowerLimitId, s_spo2AlarmLimitLower);
    
    // Write a log
    uint8_t logData[2];
    logData[0] = s_spo2AlarmLimitLowerInit;
    logData[1] = s_spo2AlarmLimitLower;
    logInterface_WriteEventLog(2,(void*)logData , eSpO2AlarmSettingChangeLowerLimitEventLogId); 
    
    setting_Save();
    
    // update init data to not show confirm screen
    s_spo2AlarmLimitLowerInit = s_spo2AlarmLimitLower;
}

void SettingScreen_SpO2AlarmLimitSetting_DiscardSetting()
{
    SYS_PRINT("SettingScreen_SpO2AlarmLimitSetting_DiscardSetting \n");
    s_spo2AlarmLimitLower = s_spo2AlarmLimitLowerInit;
}

// end of file
