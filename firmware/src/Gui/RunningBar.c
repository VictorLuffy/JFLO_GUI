/** @file RunningBar.c
 *  @brief The interfaces for display running time
 *  @author Trac Truong
 */

/* This section lists the other files that are included in this file.
 */
#include "Gui/RunningBar.h"

#ifdef UNIT_TEST
laWidget* panelNotice = (laWidget*)1;
laLabelWidget* SC_RunningTimeLabel = (laLabelWidget*)2;
laLabelWidget* lbInfo_NoticeArea = (laLabelWidget*)3;
uint32_t string_Nums_S18_Nor =  1;
uint32_t stringTable = 2;
uint32_t JFLO_NoticeRunningArea_Scheme = 3;
#endif

extern TickType_t DisplayControl_CalculateDeltaTick(TickType_t current, TickType_t last);
extern uint32_t DisplayControl_GetRunningTime(void);
extern uint8_t DisplayControl_GetActiveScreenIndex();

void RunningBar_DisplayRunningTime(bool isForceUpdate)
{
    
    static TickType_t lastTick = 0 ; 
    TickType_t currentTick = xTaskGetTickCount() ; 
    TickType_t delta = DisplayControl_CalculateDeltaTick(currentTick, lastTick);
//    SYS_PRINT("RunningBar_DisplayRunningTime %d \n", delta);
    if (isForceUpdate || delta >= RUNNINGBAR_UPDATE_CYCLE_MS)
    {
        lastTick = currentTick;
    }
    else 
        return;

    char strbuff[10];
    laString strTime, strDigitalTime, strRunningText;   

    uint32_t tmp_totalTime = DisplayControl_GetRunningTime();
    uint32_t tmp_runningHour = tmp_totalTime / 3600;
    uint32_t tmp_runningMin = tmp_totalTime / 60 - tmp_runningHour * 60;
    uint32_t tmp_runningSEC = tmp_totalTime % 60;

    // Update UI
    laString_Initialize(&strTime); 
    
    strRunningText = laString_CreateFromID(string_text_Running);

    sprintf(strbuff, "%.2u:%.2u:%.2u", tmp_runningHour, tmp_runningMin, tmp_runningSEC);
    strDigitalTime = laString_CreateFromCharBuffer(strbuff, 
        GFXU_StringFontIndexLookup(&stringTable, string_Nums_S18_Nor, setting_Get(eLanguageSettingId)));  

    laString_Append(&strTime, &strRunningText);
    laString_Append(&strTime, &strDigitalTime);
    
    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
    {
//        SYS_PRINT("MainScreen_ID \n");
        // update background b/c in mainscreen this will support warming up title
        laWidget_SetScheme((laWidget*)panelNotice, &JFLO_NoticeRunningArea_Scheme);
        laWidget_SetScheme((laWidget*)lbInfo_NoticeArea, &JFLO_NoticeRunningArea_Scheme); 
        laLabelWidget_SetText(lbInfo_NoticeArea, strTime );        
    }
    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
//        SYS_PRINT("SettingScreen_ID \n");
        // no need to update background
        laLabelWidget_SetText(SC_RunningTimeLabel, strTime );        
    }
    else {}

    
    laString_Destroy(&strRunningText); 
    laString_Destroy(&strDigitalTime); 
    laString_Destroy(&strTime);      
}

// end of file
