/** @file MaintenanceScreen.c
 *  @brief The screen provides graphic interface to update firmware
 *  @author Viet Le
 */

/* This section lists the other files that are included in this file.
 */
#include "MaintenanceScreen.h"
#include "SystemInterface.h"
#include "DeviceInformation.h"
#include "Gui/DisplayControl.h"
#include "Gui/Setting.h"

#define VERSION_STR "Current Version: "MAIN_FW_VERSION

/** @brief Be a Periodical function which working for update monitoring figure 
 *  @param [in]  currentTick
 *  @param [in]  isInit
 *  @param [out]  None
 *  @return None
 */
static void MaintenanceScreen_UpdateMonitor(uint32_t currentTick, bool isInit) 
{
    static uint32_t lastTick = 0 ; 
    //  Period timer while MainScreen_Run is working
    uint32_t delta = currentTick >= lastTick ? currentTick - lastTick : (uint32_t)(-1) + currentTick - lastTick;
    if (delta >= 1000 * TICKS_PER_MS  || isInit)
    {
//        SYS_PRINT("currentTick %u lastTick %u \n", currentTick, lastTick);
        lastTick = currentTick;
    }
    else 
        return;
    
    // Action
    char strbuff[5] = {0};
    laString str;
    
    float airFlow, o2Flow, O2;
    int breathCircuitOutTemp = 0, chamberOutTemp;
    
    airFlow = DisplayControl_GetDataAirFlow();
    o2Flow = DisplayControl_GetDataO2Flow();
    O2 = DisplayControl_GetDataO2Concentration();
    chamberOutTemp = (int)DisplayControl_GetDataTemp();
    breathCircuitOutTemp = (int)DisplayControl_GetBreathCircuitOutTemp();

    // Update Air Flow
    sprintf(strbuff, "%.2f", airFlow);

    str = laString_CreateFromCharBuffer(strbuff, 
        GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S18_Nor, setting_Get(eLanguageSettingId)));

    laTextFieldWidget_SetText(tfAirFlowMonitor,  str );

    laString_Destroy(&str);    

    // Update O2 Flow
    sprintf(strbuff, "%.2f", o2Flow);

    str = laString_CreateFromCharBuffer(strbuff, 
        GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S18_Nor, setting_Get(eLanguageSettingId)));

    laTextFieldWidget_SetText(tfO2FlowMonitor,  str );

    laString_Destroy(&str);  
    
    // Update O2
    sprintf(strbuff, "%.2f", O2);

    str = laString_CreateFromCharBuffer(strbuff, 
        GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S18_Nor, setting_Get(eLanguageSettingId)));

    laTextFieldWidget_SetText(tfO2Monitor,  str );

    laString_Destroy(&str);  

    // Update Chamber Out Temp
    sprintf(strbuff, "%.2d", breathCircuitOutTemp);

    str = laString_CreateFromCharBuffer(strbuff, 
        GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S18_Nor, setting_Get(eLanguageSettingId)));

    laTextFieldWidget_SetText(tfTempMonitor,  str );

    laString_Destroy(&str); 
    
    return;
}

/** @brief Update version string label
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void MaintenanceScreen_UpdateVersion()
{
    laString str = laString_CreateFromCharBuffer(VERSION_STR, 
                                                GFXU_StringFontIndexLookup(&stringTable, 
                                                                            string_Alphabet_Abel_Regular_S15_Bold, 
                                                                            setting_Get(eLanguageSettingId)));
    laLabelWidget_SetText(lbTitleVersion, str);
    laString_Destroy(&str); 
}

/** @brief Initialize MaintenanceScreen's element: Check all maincreen's widgets have been created yet,
 * init control & data variable and change some properties in needed initialization widgets
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 * @retval E_GuiResult : Result of initialization
 */
void MaintenanceScreen_Initialize(void)
{
    //FIXME:
    //test alarm popup
#ifdef JFLO_DEBUG_GUI
    laString str;
    str = laString_CreateFromCharBuffer("AlarmStatus",
            GFXU_StringFontIndexLookup(&stringTable, string_Alphabet_Abel_Regular_S15_Bold, setting_Get(eLanguageSettingId)));    
    laButtonWidget_SetText(btnTestHighAlarm_MaintenanceScreen, str);
    laString_Destroy(&str);    

    str = laString_CreateFromCharBuffer("Inc AlarmId",
            GFXU_StringFontIndexLookup(&stringTable, string_Alphabet_Abel_Regular_S15_Bold, setting_Get(eLanguageSettingId)));    
    laButtonWidget_SetText(btnTestMediumAlarm_MaintenanceScreen, str);
    laString_Destroy(&str);    
    
    str = laString_CreateFromCharBuffer("Inc AlarmPri",
            GFXU_StringFontIndexLookup(&stringTable, string_Alphabet_Abel_Regular_S15_Bold, setting_Get(eLanguageSettingId)));    
    laButtonWidget_SetText(btnTestLowAlarm_MaintenanceScreen, str);
    laString_Destroy(&str);     
   
    str = laString_CreateFromCharBuffer("Default Setting",
            GFXU_StringFontIndexLookup(&stringTable, string_Alphabet_Abel_Regular_S15_Bold, setting_Get(eLanguageSettingId)));    
    laButtonWidget_SetText(btnDebug4_MaintenanceScreen, str);
    laString_Destroy(&str);     
    
    str = laString_CreateFromCharBuffer("FormatSQI",
            GFXU_StringFontIndexLookup(&stringTable, string_Alphabet_Abel_Regular_S15_Bold, setting_Get(eLanguageSettingId)));    
    laButtonWidget_SetText(btnDebug5_MaintenanceScreen, str);
    laString_Destroy(&str);     
    
    str = laString_CreateFromCharBuffer("AddAlarmMes",
            GFXU_StringFontIndexLookup(&stringTable, string_Alphabet_Abel_Regular_S15_Bold, setting_Get(eLanguageSettingId)));    
    laButtonWidget_SetText(btnDebug6_MaintenanceScreen, str);
    laString_Destroy(&str);     
    
    str = laString_CreateFromCharBuffer("BackupSQI",
            GFXU_StringFontIndexLookup(&stringTable, string_Alphabet_Abel_Regular_S15_Bold, setting_Get(eLanguageSettingId)));    
    laButtonWidget_SetText(btnDebug7_MaintenanceScreen, str);
    laString_Destroy(&str);     

    str = laString_CreateFromCharBuffer("RestoreSQI",
            GFXU_StringFontIndexLookup(&stringTable, string_Alphabet_Abel_Regular_S15_Bold, setting_Get(eLanguageSettingId)));    
    laButtonWidget_SetText(btnDebug8_MaintenanceScreen, str);
    laString_Destroy(&str);     

#else
    laWidget_SetVisible((laWidget*)btnTestHighAlarm_MaintenanceScreen, LA_FALSE);
    laWidget_SetVisible((laWidget*)btnTestMediumAlarm_MaintenanceScreen, LA_FALSE);
    laWidget_SetVisible((laWidget*)btnTestLowAlarm_MaintenanceScreen, LA_FALSE);
    laWidget_SetVisible((laWidget*)btnDebug4_MaintenanceScreen, LA_FALSE);
    laWidget_SetVisible((laWidget*)btnDebug5_MaintenanceScreen, LA_FALSE);
    laWidget_SetVisible((laWidget*)btnDebug6_MaintenanceScreen, LA_FALSE);
    laWidget_SetVisible((laWidget*)btnDebug7_MaintenanceScreen, LA_FALSE);
    laWidget_SetVisible((laWidget*)btnDebug8_MaintenanceScreen, LA_FALSE);
#endif    
    
    MaintenanceScreen_UpdateVersion();
    MaintenanceScreen_UpdateMonitor(0, true);
}

/** @brief Reinitialize MaintenanceScreen's control and data.
 *  @brief Cause the value of MaintenanceScreen's control, data is changed and this screen is created by ScreenCreate_MaintenanceScreen fnc.
 *  This fnc will remake all variable of each widget in MaintenanceScreen
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void MaintenanceScreen_Reinit(void)
{
    MaintenanceScreen_Initialize();
}

/** @brief Distruct some MaintenanceScreen backend elements when switched to other one
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void MaintenanceScreen_Distructor(void)
{

}

/** @brief Implement mainscreen's state, animation function and some action periodlly
 *  It was called by DisplayControl.
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void MaintenanceScreen_Run(void)
{
    uint32_t currentTick = (uint32_t)GET_TICKS();
    
    MaintenanceScreen_UpdateMonitor(currentTick, false);
    
    return;
}

/* end of file */


