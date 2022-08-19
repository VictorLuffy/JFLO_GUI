/** @file SettingScreen_OxygenConcentrationAlarmLimitSetting.c
 *  @brief The interfaces for display oxygen concentration alarm limit setting
 *  @author Trac Truong
 */

/* This section lists the other files that are included in this file.
 */
#include "Gui/SettingScreen_OxygenConcentrationAlarmLimitSetting.h"

#ifdef UNIT_TEST
laLabelWidget* SC_MenuSetting_SettingOxyAlarm_LoLimitLabel = (laLabelWidget*)1;
laLabelWidget* SC_MenuSetting_SettingOxyAlarm_UpLimitLabel = (laLabelWidget*)2;
GFXU_FontAsset BebasNeueBook_S60_Bold_Internal;
#endif

extern void SettingScreen_SetSettingScreenUpdate(bool f);
static int s_oxygenAlarmLimitLower = -1;
static int s_oxygenAlarmLimitLowerInit = -1;
static int s_oxygenAlarmLimitLowerDisplay = -1;

static int s_oxygenAlarmLimitUpper = -1;
static int s_oxygenAlarmLimitUpperInit = -1;
static int s_oxygenAlarmLimitUpperDisplay = -1;


void SettingScreen_OxygenConcentrationAlarmLimitSetting_Init()
{
    s_oxygenAlarmLimitLowerInit = setting_Get(eOxygenAlarmSettingLowerLimitId);
    s_oxygenAlarmLimitLower = s_oxygenAlarmLimitLowerInit;
    s_oxygenAlarmLimitLowerDisplay = -1;
    
    s_oxygenAlarmLimitUpperInit = setting_Get(eOxygenAlarmSettingUpperLimitId);
    s_oxygenAlarmLimitUpper = s_oxygenAlarmLimitUpperInit;
    s_oxygenAlarmLimitUpperDisplay = -1;

}

void SettingScreen_OxygenConcentrationAlarmLimitSetting_Display()
{
    char strbuff[5];
    laString str;

    if (s_oxygenAlarmLimitLowerDisplay != s_oxygenAlarmLimitLower)
    {
        SYS_PRINT("SettingScreen_OxygenConcentrationAlarmLimitSetting_Display lower %d \n", s_oxygenAlarmLimitLower);
        sprintf(strbuff, "%d%%", s_oxygenAlarmLimitLower);
        str = laString_CreateFromCharBuffer(strbuff, &BebasNeueBook_S60_Bold_Internal);      
        laLabelWidget_SetText(SC_MenuSetting_SettingOxyAlarm_LoLimitLabel, str);
        laString_Destroy(&str); 

        s_oxygenAlarmLimitLowerDisplay = s_oxygenAlarmLimitLower;
    }
    if (s_oxygenAlarmLimitUpperDisplay != s_oxygenAlarmLimitUpper)
    {
        SYS_PRINT("SettingScreen_OxygenConcentrationAlarmLimitSetting_Display upper %d \n", s_oxygenAlarmLimitUpper);
        sprintf(strbuff, "%d%%", s_oxygenAlarmLimitUpper);
        str = laString_CreateFromCharBuffer(strbuff, &BebasNeueBook_S60_Bold_Internal);      
        laLabelWidget_SetText(SC_MenuSetting_SettingOxyAlarm_UpLimitLabel, str);
        laString_Destroy(&str); 

        s_oxygenAlarmLimitUpperDisplay = s_oxygenAlarmLimitUpper;
    }
}

void SettingScreen_OxygenConcentrationAlarmLimitSetting_SetCallbackFunction(void)
{
    // Assigning the callback function to widget, these event cannot auto add by generator
}

void SettingScreen_OxygenConcentrationAlarmLimitSetting_LowerInc()
{
    uint8_t max = setting_GetMax(eOxygenAlarmSettingLowerLimitId);
    uint8_t step = setting_GetStep(eOxygenAlarmSettingLowerLimitId);
    s_oxygenAlarmLimitLower = s_oxygenAlarmLimitLower + step;
    s_oxygenAlarmLimitLower = (s_oxygenAlarmLimitLower > max) ? max : s_oxygenAlarmLimitLower;
    SYS_PRINT("SettingScreen_OxygenConcentrationAlarmLimitSetting_Inc %d \n", s_oxygenAlarmLimitLower);
    if (s_oxygenAlarmLimitLowerDisplay != s_oxygenAlarmLimitLower)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}

void SettingScreen_OxygenConcentrationAlarmLimitSetting_LowerDec()
{
    uint8_t min = setting_GetMin(eOxygenAlarmSettingLowerLimitId);
    uint8_t step = setting_GetStep(eOxygenAlarmSettingLowerLimitId);
    s_oxygenAlarmLimitLower = s_oxygenAlarmLimitLower - step;
    s_oxygenAlarmLimitLower = (s_oxygenAlarmLimitLower < min) ? min : s_oxygenAlarmLimitLower;
    SYS_PRINT("SettingScreen_OxygenConcentrationAlarmLimitSetting_Inc %d \n", s_oxygenAlarmLimitLower);
    if (s_oxygenAlarmLimitLowerDisplay != s_oxygenAlarmLimitLower)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }

}

void SettingScreen_OxygenConcentrationAlarmLimitSetting_UpperInc()
{
    uint8_t source = setting_Get(eOxygenSourceSettingId);
    uint8_t max = setting_GetMax(eOxygenAlarmSettingUpperLimitId);
    
    // limit max upper limit as oxygen source
    if (source == e90OxygenSource)
    {
        max = 90;
    }
    uint8_t step = setting_GetStep(eOxygenAlarmSettingUpperLimitId);
    s_oxygenAlarmLimitUpper = s_oxygenAlarmLimitUpper + step;
    s_oxygenAlarmLimitUpper = (s_oxygenAlarmLimitUpper > max) ? max : s_oxygenAlarmLimitUpper;
    SYS_PRINT("SettingScreen_OxygenConcentrationAlarmLimitSetting_UpperInc %d \n", s_oxygenAlarmLimitUpper);
    if (s_oxygenAlarmLimitUpperDisplay != s_oxygenAlarmLimitUpper)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}

void SettingScreen_OxygenConcentrationAlarmLimitSetting_UpperDec()
{
    uint8_t min = setting_GetMin(eOxygenAlarmSettingUpperLimitId);
    uint8_t step = setting_GetStep(eOxygenAlarmSettingUpperLimitId);
    s_oxygenAlarmLimitUpper = s_oxygenAlarmLimitUpper - step;
    s_oxygenAlarmLimitUpper = (s_oxygenAlarmLimitUpper < min) ? min : s_oxygenAlarmLimitUpper;
    SYS_PRINT("SettingScreen_OxygenConcentrationAlarmLimitSetting_Inc %d \n", s_oxygenAlarmLimitUpper);
    if (s_oxygenAlarmLimitUpperDisplay != s_oxygenAlarmLimitUpper)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }

}

void SettingScreen_OxygenConcentrationAlarmLimitSetting_SetLowerSetting(int s)
{
    uint8_t min = setting_GetMin(eOxygenAlarmSettingLowerLimitId);
    uint8_t max = setting_GetMax(eOxygenAlarmSettingLowerLimitId);
    
    if ( s < min || s > max)
        return;
    
    s_oxygenAlarmLimitLower = s;
    if (s_oxygenAlarmLimitLowerDisplay != s_oxygenAlarmLimitLower)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}
int SettingScreen_OxygenConcentrationAlarmLimitSetting_GetLowerSetting()
{
    return s_oxygenAlarmLimitLower;
}

void SettingScreen_OxygenConcentrationAlarmLimitSetting_SetUpperSetting(int s)
{
    uint8_t min = setting_GetMin(eOxygenAlarmSettingUpperLimitId);
    uint8_t max = setting_GetMax(eOxygenAlarmSettingUpperLimitId);
    
    if ( s < min || s > max)
        return;
    
    s_oxygenAlarmLimitUpper = s;
    if (s_oxygenAlarmLimitUpperDisplay != s_oxygenAlarmLimitUpper)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}
int SettingScreen_OxygenConcentrationAlarmLimitSetting_GetUpperSetting()
{
    return s_oxygenAlarmLimitUpper;
}

bool SettingScreen_OxygenConcentrationAlarmLimitSetting_CheckDataChange()
{
    return ( s_oxygenAlarmLimitLowerInit == s_oxygenAlarmLimitLower && s_oxygenAlarmLimitUpperInit == s_oxygenAlarmLimitUpper ) ? false : true;
}

void SettingScreen_OxygenConcentrationAlarmLimitSetting_SaveSetting()
{
    if (!SettingScreen_OxygenConcentrationAlarmLimitSetting_CheckDataChange())
    {
        SYS_PRINT("SettingScreen_OxygenConcentrationAlarmLimitSetting_SaveSetting no change \n");
        return;
    }
    SYS_PRINT("SettingScreen_OxygenConcentrationAlarmLimitSetting_SaveSetting \n");

    if (s_oxygenAlarmLimitLowerInit != s_oxygenAlarmLimitLower)
    {
        setting_Set(eOxygenAlarmSettingLowerLimitId, s_oxygenAlarmLimitLower);

        // Write a log
        uint8_t logData[2];
        logData[0] =  s_oxygenAlarmLimitLowerInit;
        logData[1] = s_oxygenAlarmLimitLower;
        logInterface_WriteEventLog(2,(void*)logData , eOxygenAlarmSettingChangeLowerLimitEventLogId); 
    }
    if (s_oxygenAlarmLimitUpperInit != s_oxygenAlarmLimitUpper)
    {
         setting_Set(eOxygenAlarmSettingUpperLimitId, s_oxygenAlarmLimitUpper);
         
        // Write a log
        uint8_t logData[2];
        logData[0] =  s_oxygenAlarmLimitUpperInit;
        logData[1] = s_oxygenAlarmLimitUpper;
        logInterface_WriteEventLog(2,(void*)logData , eOxygenAlarmSettingChangeUpperLimitEventLogId); 

    }
    setting_Save();
    
    // update init data to not show confirm screen
    s_oxygenAlarmLimitLowerInit = s_oxygenAlarmLimitLower;
    s_oxygenAlarmLimitUpperInit = s_oxygenAlarmLimitUpper;
}

void SettingScreen_OxygenConcentrationAlarmLimitSetting_DiscardSetting()
{
    SYS_PRINT("SettingScreen_OxygenConcentrationAlarmLimitSetting_DiscardSetting \n");
    s_oxygenAlarmLimitLower = s_oxygenAlarmLimitLowerInit;
    s_oxygenAlarmLimitUpper = s_oxygenAlarmLimitUpperInit;
}

// end of file
