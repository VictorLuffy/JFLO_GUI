/** @file SettingScreen_AlarmSoundLevelSetting.c
 *  @brief The interfaces for display alarm sound level setting
 *  @author Trac Truong
 */

/* This section lists the other files that are included in this file.
 */
#include "Gui/SettingScreen_AlarmSoundLevelSetting.h"

#ifdef UNIT_TEST
laLabelWidget* SC_MenuSetting_SettingAlarmSoundLevel_ValueLabel;
GFXU_FontAsset BebasNeueBook_S60_Bold_Internal;
#endif

extern void SettingScreen_SetSettingScreenUpdate(bool f);
static int s_alarmSoundLevel = -1;
static int s_alarmSoundLevelInit = -1;
static int s_alarmSoundLevelDisplay = -1;

void SettingScreen_AlarmSoundLevelSetting_Init()
{
    s_alarmSoundLevelInit = setting_Get(eSpeakerVolumeSettingId);
    s_alarmSoundLevel = s_alarmSoundLevelInit;
    s_alarmSoundLevelDisplay = -1;
}

void SettingScreen_AlarmSoundLevelSetting_Display()
{
    if (s_alarmSoundLevelDisplay == s_alarmSoundLevel)
    {
        return;
    }
    SYS_PRINT("SettingScreen_AlarmSoundLevelSetting_Display %d \n", s_alarmSoundLevel);
    char strbuff[5];
    laString str;
    sprintf(strbuff, "%d", s_alarmSoundLevel);
    str = laString_CreateFromCharBuffer(strbuff, &BebasNeueBook_S60_Bold_Internal);      
    laLabelWidget_SetText(SC_MenuSetting_SettingAlarmSoundLevel_ValueLabel, str);
    laString_Destroy(&str); 

    s_alarmSoundLevelDisplay = s_alarmSoundLevel;
}

void SettingScreen_AlarmSoundLevelSetting_SetCallbackFunction(void)
{
    // Assigning the callback function to widget, these event cannot auto add by generator
}

void SettingScreen_AlarmSoundLevelSetting_Inc()
{
    uint8_t max = setting_GetMax(eSpeakerVolumeSettingId);
    uint8_t step = setting_GetStep(eSpeakerVolumeSettingId);
    s_alarmSoundLevel = s_alarmSoundLevel + step;
    s_alarmSoundLevel = (s_alarmSoundLevel > max) ? max : s_alarmSoundLevel;
    deviceInterface_SendAudioEvent(eTestSound, s_alarmSoundLevel);
    SYS_PRINT("SettingScreen_AlarmSoundLevelSetting_Inc %d \n", s_alarmSoundLevel);
    if (s_alarmSoundLevelDisplay != s_alarmSoundLevel)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}

void SettingScreen_AlarmSoundLevelSetting_Dec()
{
    uint8_t min = setting_GetMin(eSpeakerVolumeSettingId);
    uint8_t step = setting_GetStep(eSpeakerVolumeSettingId);
    s_alarmSoundLevel = s_alarmSoundLevel - step;
    s_alarmSoundLevel = (s_alarmSoundLevel < min) ? min : s_alarmSoundLevel;
    SYS_PRINT("SettingScreen_AlarmSoundLevelSetting_Inc %d \n", s_alarmSoundLevel);
    deviceInterface_SendAudioEvent(eTestSound, s_alarmSoundLevel);
    if (s_alarmSoundLevelDisplay != s_alarmSoundLevel)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}

void SettingScreen_AlarmSoundLevelSetting_SetSetting(int s)
{
    uint8_t min = setting_GetMin(eSpeakerVolumeSettingId);
    uint8_t max = setting_GetMax(eSpeakerVolumeSettingId);
    
    if ( s < min || s > max)
        return;
    
    s_alarmSoundLevel = s;
    if (s_alarmSoundLevelDisplay != s_alarmSoundLevel)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}
int SettingScreen_AlarmSoundLevelSetting_GetSetting()
{
    return s_alarmSoundLevel;
}

bool SettingScreen_AlarmSoundLevelSetting_CheckDataChange()
{
    return ( s_alarmSoundLevelInit == s_alarmSoundLevel ) ? false : true;
}

void SettingScreen_AlarmSoundLevelSetting_SaveSetting()
{
    if (!SettingScreen_AlarmSoundLevelSetting_CheckDataChange())
    {
        SYS_PRINT("SettingScreen_AlarmSoundLevelSetting_SaveSetting no change \n");
        return;
    }
    SYS_PRINT("SettingScreen_AlarmSoundLevelSetting_SaveSetting \n");
    
    setting_SetSpeaker(s_alarmSoundLevel); 
    
    // update init data to not show confirm screen
    s_alarmSoundLevelInit = s_alarmSoundLevel;
}

void SettingScreen_AlarmSoundLevelSetting_DiscardSetting()
{
    SYS_PRINT("SettingScreen_AlarmSoundLevelSetting_DiscardSetting \n");
    s_alarmSoundLevel = s_alarmSoundLevelInit;
}

// end of file
