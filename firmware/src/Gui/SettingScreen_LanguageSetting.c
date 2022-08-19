/** @file SettingScreen_LanguageSetting.c
 *  @brief The interfaces for display language setting
 *  @author Trac Truong
 */

/* This section lists the other files that are included in this file.
 */
#include "Gui/SettingScreen_LanguageSetting.h"

#ifdef UNIT_TEST
laLabelWidget* SC_MenuSetting_SettingLanguage_SettingLabel = (laLabelWidget*)1;

#endif

extern void SettingScreen_SetSettingScreenUpdate(bool f);
static E_Language s_language = -1;
static E_Language s_languageInit = -1;
static E_Language s_languageDisplay = -1;

void SettingScreen_LanguageSetting_Init()
{
    s_languageInit = setting_Get(eLanguageSettingId);
    s_language = s_languageInit;
    s_languageDisplay = -1;
}

void SettingScreen_LanguageSetting_Display()
{
    if (s_languageDisplay == s_language)
    {
        return;
    }
    SYS_PRINT("SettingScreen_LanguageSetting_Display %d \n", s_language);
    switch (s_language)
    {
        case eJapanese:
            laLabelWidget_SetText(SC_MenuSetting_SettingLanguage_SettingLabel, laString_CreateFromID(string_text_SettingScreen_Menu_Language_Japanese));
            break;
        case eEnglish:
            laLabelWidget_SetText(SC_MenuSetting_SettingLanguage_SettingLabel, laString_CreateFromID(string_text_SettingScreen_Menu_Language_English));
            break;
        case eVietnamese:
            laLabelWidget_SetText(SC_MenuSetting_SettingLanguage_SettingLabel, laString_CreateFromID(string_text_SettingScreen_Menu_Language_Vietnamese));
            break;
        default:
            break;
    }
    s_languageDisplay = s_language;
}

void SettingScreen_LanguageSetting_SetCallbackFunction(void)
{
    // Assigning the callback function to widget, these event cannot auto add by generator
}

void SettingScreen_LanguageSetting_NextSetting()
{
    if (s_language + 1 >= eNoOfLanguage)
    {
        s_language = 0; // first language enum
    }
    else
    {
        s_language++;
    }
    SYS_PRINT("SettingScreen_LanguageSetting_NextSetting %d \n", s_language);
    SettingScreen_SetSettingScreenUpdate(true);
}

void SettingScreen_LanguageSetting_PrevSetting()
{
    if ((int)s_language - 1 < 0)
    {
        s_language = eNoOfLanguage-1; // last language enum
    }
    else
    {
        s_language--;
    }
    SYS_PRINT("SettingScreen_LanguageSetting_PrevSetting %d \n", s_language);
    SettingScreen_SetSettingScreenUpdate(true);
}

void SettingScreen_LanguageSetting_SetSetting(E_Language s)
{
    if ( s < 0 || s >= eNoOfLanguage)
        return;

    s_language = s;
    if (s_languageDisplay != s_language)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }

}
E_Language SettingScreen_LanguageSetting_GetSetting()
{
    return s_language;
}

bool SettingScreen_LanguageSetting_CheckDataChange()
{
    return ( s_languageInit == s_language ) ? false : true;
}

void SettingScreen_LanguageSetting_SaveSetting()
{
    if (!SettingScreen_LanguageSetting_CheckDataChange())
    {
        SYS_PRINT("SettingScreen_LanguageSetting_SaveSetting no change \n");
        return;
    }
    SYS_PRINT("SettingScreen_LanguageSetting_SaveSetting \n");
    
    // Write a log
    //FIXME: wait to have other language string
//    if (s_language > 0 && s_language < eNoOfLanguage)
//    {
    //    setting_Set(eLanguageSettingId, s_language);
    //    setting_Save();
//    }
        
    // Write a log
    uint8_t logData[2];
    logData[0] = s_languageInit ;
    logData[1] = s_language;
    logInterface_WriteEventLog(2,(void*)logData , eChangeLanguageSettingEventLogId); 
    
    // update init data to not show confirm screen
    s_languageInit = s_language;
}

void SettingScreen_LanguageSetting_DiscardSetting()
{
    SYS_PRINT("SettingScreen_LanguageSetting_DiscardSetting \n");
     s_language = s_languageInit;
}

// end of file
