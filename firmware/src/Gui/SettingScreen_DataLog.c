/** @file SettingScreen_DataLog.c
 *  @brief The interfaces for display data log
 *  @author Trac Truong
 */

/* This section lists the other files that are included in this file.
 */
#include "Gui/SettingScreen_DataLog.h"

#ifdef UNIT_TEST
laLabelWidget* SC_DataLogSettingAlarmLabel = (laLabelWidget*)1;
laLabelWidget* SC_DataLogSettingEventLabel = (laLabelWidget*)2;
laLabelWidget* SC_DataLogSettingTimeLabel_1 = (laLabelWidget*)3;
laLabelWidget* SC_DataLogSettingIndicator_1 = (laLabelWidget*)4;
laLabelWidget* SC_DataLogSettingTitleLabel_1 = (laLabelWidget*)5;
laLabelWidget* SC_DataLogSettingStateLabel_1 = (laLabelWidget*)6;
laLabelWidget* SC_DataLogSettingTimeLabel_2 = (laLabelWidget*)7;
laLabelWidget* SC_DataLogSettingIndicator_2 = (laLabelWidget*)8;
laLabelWidget* SC_DataLogSettingTitleLabel_2 = (laLabelWidget*)9;
laLabelWidget* SC_DataLogSettingStateLabel_2 = (laLabelWidget*)10;
laLabelWidget* SC_DataLogSettingTimeLabel_3 = (laLabelWidget*)11;
laLabelWidget* SC_DataLogSettingIndicator_3 = (laLabelWidget*)12;
laLabelWidget* SC_DataLogSettingTitleLabel_3 = (laLabelWidget*)13;
laLabelWidget* SC_DataLogSettingStateLabel_3 = (laLabelWidget*)14;
laLabelWidget* SC_DataLogSettingTimeLabel_4 = (laLabelWidget*)15;
laLabelWidget* SC_DataLogSettingIndicator_4 = (laLabelWidget*)16;
laLabelWidget* SC_DataLogSettingTitleLabel_4 = (laLabelWidget*)17;
laLabelWidget* SC_DataLogSettingStateLabel_4 = (laLabelWidget*)18;
laLabelWidget* SC_DataLogSettingTimeLabel_5 = (laLabelWidget*)19;
laLabelWidget* SC_DataLogSettingIndicator_5 = (laLabelWidget*)20;
laLabelWidget* SC_DataLogSettingTitleLabel_5 = (laLabelWidget*)21;
laLabelWidget* SC_DataLogSettingStateLabel_5 = (laLabelWidget*)22;

laWidget* SC_DataLogSettingUnderbarPanel = (laWidget*)23;
laWidget* SC_DataLogSettingTitleLabel = (laWidget*)24;
laWidget* SC_DataLogSettingStateLabel = (laWidget*)25;
uint32_t string_text_SettingScreen_DataLog_AlarmTitle = 1;
uint32_t string_text_SettingScreen_DataLog_State = 2 ;

GFXU_FontAsset AbelRegular_S20_Bold_Internal;
GFXU_FontAsset AbelRegular_S12_Bold_Internal;
laWidget* SC_DataLogSettingPageNumberLabel = (laWidget*)26;
uint32_t string_text_Nullstring = 3;
laScheme JFLO_AlarmIndicatorLow1Priority_Scheme;
laScheme JFLO_AlarmIndicatorMediumPriority_Scheme;
laScheme JFLO_AlarmIndicatorHighPriority_Scheme;

uint32_t string_text_SettingScreen_DataLog_EventName = 4;
uint32_t string_text_SettingScreen_DataLog_Refer = 5;

#endif

extern void SettingScreen_SetSettingScreenUpdate(bool f);
static int s_dataLogSetting = -1;
static int s_dataLogSettingDisplay = -1;
static bool s_isDataLogSettingDisplayInit = false;

static int16_t s_totalPageNum = -1;
static int16_t s_currentPageNum = -1;
static int16_t s_numLog = -1;

#ifndef UNIT_TEST
LogItem_Struct s_logListItem[MAX_LOG_IN_PAGE] __attribute__((section(".ddr_data"), space(prog)));
uint8_t eventDisplayData[MAX_LOG_DISPLAY*LOG_LEN] __attribute__((section(".ddr_data"), space(prog)));
uint8_t alarmDisplayData[MAX_ALARM_DISPLAY*LOG_LEN] __attribute__((section(".ddr_data"), space(prog)));
#else
LogItem_Struct s_logListItem[MAX_LOG_IN_PAGE];
uint8_t eventDisplayData[MAX_LOG_DISPLAY*LOG_LEN];
uint8_t alarmDisplayData[MAX_ALARM_DISPLAY*LOG_LEN];
#endif


void SettingScreen_DataLog_Init()
{
    s_dataLogSetting = eAlarmDataLogSetting;
    s_dataLogSettingDisplay = -1;
    s_isDataLogSettingDisplayInit = false;
    StatusBar_SetAlarmInfo(false);
}

void SettingScreen_DataLog_Display()
{
    SYS_PRINT("SettingScreen_DataLog_Display \n");
//    if (s_dataLogSettingDisplay == s_dataLogSetting)
//    {
//        return;
//    }
    switch (s_dataLogSetting)
    {
        case eAlarmDataLogSetting:
            SettingScreen_DataLog_DisplayAlarm();
            break;
        case eEventDataLogSetting:
            SettingScreen_DataLog_DisplayEvent();
            break;
        default:
            break;            
    }
//    s_dataLogSettingDisplay = s_dataLogSetting;
}

void SC_DataLogSettingAlarmLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    if (widget == 0 || evt == 0)
        return;

    SettingScreen_DataLog_SetSetting(eAlarmDataLogSetting);
    
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_DataLogSettingEventLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    if (widget == 0 || evt == 0)
        return;

    SettingScreen_DataLog_SetSetting(eEventDataLogSetting);
    
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}

void SettingScreen_DataLog_SetCallbackFunction(void)
{
    // Assigning the callback function to widget, these event cannot auto add by generator
    laWidget_OverrideTouchDownEvent((laWidget*)SC_DataLogSettingAlarmLabel, &SC_DataLogSettingAlarmLabel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_DataLogSettingEventLabel, &SC_DataLogSettingEventLabel_PressedEvent);
    
    // this set log list widget pointer   
    s_logListItem[0].timeWidget = SC_DataLogSettingTimeLabel_1;
    s_logListItem[0].indicatorWidget = SC_DataLogSettingIndicator_1;
    s_logListItem[0].nameWidget = SC_DataLogSettingTitleLabel_1;
    s_logListItem[0].dataWidget = SC_DataLogSettingStateLabel_1;

    s_logListItem[1].timeWidget = SC_DataLogSettingTimeLabel_2;
    s_logListItem[1].indicatorWidget = SC_DataLogSettingIndicator_2;
    s_logListItem[1].nameWidget = SC_DataLogSettingTitleLabel_2;
    s_logListItem[1].dataWidget = SC_DataLogSettingStateLabel_2;

    s_logListItem[2].timeWidget = SC_DataLogSettingTimeLabel_3;
    s_logListItem[2].indicatorWidget = SC_DataLogSettingIndicator_3;
    s_logListItem[2].nameWidget = SC_DataLogSettingTitleLabel_3;
    s_logListItem[2].dataWidget = SC_DataLogSettingStateLabel_3;

    s_logListItem[3].timeWidget = SC_DataLogSettingTimeLabel_4;
    s_logListItem[3].indicatorWidget = SC_DataLogSettingIndicator_4;
    s_logListItem[3].nameWidget = SC_DataLogSettingTitleLabel_4;
    s_logListItem[3].dataWidget = SC_DataLogSettingStateLabel_4;

    s_logListItem[4].timeWidget = SC_DataLogSettingTimeLabel_5;
    s_logListItem[4].indicatorWidget = SC_DataLogSettingIndicator_5;
    s_logListItem[4].nameWidget = SC_DataLogSettingTitleLabel_5;
    s_logListItem[4].dataWidget = SC_DataLogSettingStateLabel_5;    
    
}

void SettingScreen_DataLog_SetSetting(E_DataLogSetting s)
{
    if (s < 0 || s >= eNoOfDataLogSetting)
        return;
    s_dataLogSetting = s;    
    if (s_dataLogSetting == eAlarmDataLogSetting)
    {
        StatusBar_SetAlarmInfo(false);
    }
    // this always run to update new event/alarm when already display
    if (s_dataLogSettingDisplay != s_dataLogSetting)
    {
        SettingScreen_SetSettingScreenUpdate(true);
        s_isDataLogSettingDisplayInit = false;
    }
}
E_DataLogSetting SettingScreen_DataLog_GetSetting()
{
    return s_dataLogSetting;
}

void SettingScreen_DataLog_DisplayAlarm()
{
    // init data
    if(!s_isDataLogSettingDisplayInit)
    {
        SettingScreen_DataLog_InitAlarmData();
        s_isDataLogSettingDisplayInit = true;
    }
    
    SYS_PRINT("SettingScreen_DataLog_DisplayAlarm \n");
    int32_t x = DisplayControl_GetCenterPostion((laWidget*)SC_DataLogSettingAlarmLabel, (laWidget*)SC_DataLogSettingUnderbarPanel);
    laWidget_SetX((laWidget*)SC_DataLogSettingUnderbarPanel, x);
    
    //title    
    laLabelWidget_SetText(SC_DataLogSettingTitleLabel, laString_CreateFromID(string_text_SettingScreen_DataLog_AlarmTitle));
    laLabelWidget_SetText(SC_DataLogSettingStateLabel, laString_CreateFromID(string_text_SettingScreen_DataLog_State));
    
    char pageStrbuff[10];
    laString pageStr;
    sprintf(pageStrbuff, "%d / %d",s_currentPageNum, s_totalPageNum );     
    pageStr = laString_CreateFromCharBuffer(pageStrbuff, &AbelRegular_S20_Bold_Internal);    
    laLabelWidget_SetText(SC_DataLogSettingPageNumberLabel, pageStr);
    laString_Destroy(&pageStr);    
    
    int indexLogNumInPage;
    int lastIndexLogNumInPage;

    // page 1 : id 0 - 14
    // page 2 : id 15 - 29
    // page 3 : id 30 - 35 -> show this first
    indexLogNumInPage = (s_currentPageNum-1) * MAX_ALARM_IN_PAGE;
    lastIndexLogNumInPage = (s_currentPageNum) * MAX_ALARM_IN_PAGE - 1 ;
    if (lastIndexLogNumInPage >= s_numLog)
        lastIndexLogNumInPage = s_numLog - 1;
   
    uint16_t index = 0;
    for (index = 0; index < MAX_ALARM_IN_PAGE; index++)
    {
        if (indexLogNumInPage <= lastIndexLogNumInPage)
        {
//            SYS_PRINT("\n log index = %d \n", index);
            
            laWidget_SetVisible(s_logListItem[index].indicatorWidget, LA_TRUE);
            laWidget_SetX((laWidget*)s_logListItem[index].nameWidget, ALARM_TITLE_ITEM_POS_X);
            
            Log_Struct logData;
            memset(&logData, 0, sizeof(Log_Struct));
            logData.eCode = 0xff;
            logMgr_GetLogFromDataArray(eAlarmLogTypeID, indexLogNumInPage, alarmDisplayData, &logData);
            /*  Number Column */
            char strbuff[255];
            laString str;

            /* Date Time Column */
            sprintf(strbuff, "%.2d/%.2d/%.2d %.2d:%.2d", logData.time.year_2, logData.time.month, logData.time.date, logData.time.hour, logData.time.minute);
            str = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S12_Bold_Internal);
            laLabelWidget_SetText(s_logListItem[index].timeWidget, str);
            laString_Destroy(&str);
            
            /* Indicator*/
            SettingScreen_DataLog_SetAlarmIndicator(s_logListItem[index].indicatorWidget, logData.data[ALARM_LOG_DATA_PRIORITY]);
            
            /*  Alarm title Column */
            LogInterface_GetAlarmStringFromID(logData.eCode, strbuff);

            str = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S12_Bold_Internal);  
            laLabelWidget_SetText(s_logListItem[index].nameWidget, str);
            laString_Destroy(&str);

            /*  Alarm state Column */
            LogInterface_GetAlarmStatusString(logData.data[ALARM_LOG_DATA_STATUS], strbuff);

            str = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S12_Bold_Internal);  
            laLabelWidget_SetText(s_logListItem[index].dataWidget, str);
            laString_Destroy(&str);
        } 
        else 
        {
//            SYS_PRINT("\n log null index = %d \n", index);
            laWidget_SetVisible(s_logListItem[index].indicatorWidget, LA_FALSE);
            laLabelWidget_SetText(s_logListItem[index].timeWidget, laString_CreateFromID(string_text_Nullstring));
            laLabelWidget_SetText(s_logListItem[index].nameWidget, laString_CreateFromID(string_text_Nullstring));
            laLabelWidget_SetText(s_logListItem[index].dataWidget, laString_CreateFromID(string_text_Nullstring));
        }
        indexLogNumInPage++;
    }    
}

void SettingScreen_DataLog_SetAlarmIndicator(laWidget* w, uint8_t data)
{
    if (w == 0)
        return;
    
    switch(data)
    {
        case eLow1Priority:
            laWidget_SetScheme(w, &JFLO_AlarmIndicatorLow1Priority_Scheme);
            break;
        case eLow2Priority:
            laWidget_SetScheme(w, &JFLO_AlarmIndicatorLow2Priority_Scheme);
            break;
        case eMediumPriority:
            laWidget_SetScheme(w, &JFLO_AlarmIndicatorMediumPriority_Scheme);
            break;
        case eHighPriority:
            laWidget_SetScheme(w, &JFLO_AlarmIndicatorHighPriority_Scheme);
            break;
        default:
            break;
    }
}

void SettingScreen_DataLog_DisplayEvent()
{
    // init data
    if(!s_isDataLogSettingDisplayInit)
    {
        SettingScreen_DataLog_InitEventData();
        s_isDataLogSettingDisplayInit = true;
    }
    SYS_PRINT("SettingScreen_DataLog_DisplayEvent \n");
    int32_t x = DisplayControl_GetCenterPostion((laWidget*)SC_DataLogSettingEventLabel, (laWidget*)SC_DataLogSettingUnderbarPanel);
    laWidget_SetX((laWidget*)SC_DataLogSettingUnderbarPanel, x);
    
    // init title    
    laLabelWidget_SetText(SC_DataLogSettingTitleLabel, laString_CreateFromID(string_text_SettingScreen_DataLog_EventName));
    laLabelWidget_SetText(SC_DataLogSettingStateLabel, laString_CreateFromID(string_text_SettingScreen_DataLog_Refer));

    char pageStrbuff[10];
    laString pageStr;
    sprintf(pageStrbuff, "%d / %d",s_currentPageNum, s_totalPageNum );     
    pageStr = laString_CreateFromCharBuffer(pageStrbuff, &AbelRegular_S20_Bold_Internal);    
    laLabelWidget_SetText(SC_DataLogSettingPageNumberLabel, pageStr);
    laString_Destroy(&pageStr);    
    
    int indexLogNumInPage;
    int lastIndexLogNumInPage;

    // page 1 : id 0 - 14
    // page 2 : id 15 - 29
    // page 3 : id 30 - 35 -> show this first
    indexLogNumInPage = (s_currentPageNum-1) * MAX_LOG_IN_PAGE;
    lastIndexLogNumInPage = (s_currentPageNum) * MAX_LOG_IN_PAGE - 1 ;
    if (lastIndexLogNumInPage >= s_numLog)
        lastIndexLogNumInPage = s_numLog - 1;
   
    uint16_t index = 0;
    for (index = 0; index < MAX_LOG_IN_PAGE; index++)
    {
        if (indexLogNumInPage <= lastIndexLogNumInPage)
        {
//            SYS_PRINT("\n log index = %d \n", index);
            
            laWidget_SetVisible(s_logListItem[index].indicatorWidget, LA_FALSE);
            laWidget_SetX((laWidget*)s_logListItem[index].nameWidget, EVENT_NAME_ITEM_POS_X);
            
            Log_Struct logData;
            memset(&logData, 0, sizeof(Log_Struct));
            logData.eCode = 0xff;
            logMgr_GetLogFromDataArray(eAlarmLogTypeID, indexLogNumInPage, eventDisplayData, &logData);
            /*  Number Column */
            char strbuff[255];
            laString str;

            /* Date Time Column */
            sprintf(strbuff, "%.2d/%.2d/%.2d %.2d:%.2d", logData.time.year_2, logData.time.month, logData.time.date, logData.time.hour, logData.time.minute);
            str = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S12_Bold_Internal);
            laLabelWidget_SetText(s_logListItem[index].timeWidget, str);
            laString_Destroy(&str);
            
            /*  Event name column */
            LogInterface_GetEventStringFromID(logData.eCode, strbuff);

            str = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S12_Bold_Internal);  
            laLabelWidget_SetText(s_logListItem[index].nameWidget, str);
            laString_Destroy(&str);

            /*  Event refer column */
            LogInterface_GetEventDataString(logData.eCode, logData.data, strbuff);

            str = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S12_Bold_Internal);  
            laLabelWidget_SetText(s_logListItem[index].dataWidget, str);
            laString_Destroy(&str);
            
        } 
        else 
        {
//            SYS_PRINT("\n log null index = %d \n", index);
            laWidget_SetVisible(s_logListItem[index].indicatorWidget, LA_FALSE);
            laLabelWidget_SetText(s_logListItem[index].timeWidget, laString_CreateFromID(string_text_Nullstring));
            laLabelWidget_SetText(s_logListItem[index].nameWidget, laString_CreateFromID(string_text_Nullstring));
            laLabelWidget_SetText(s_logListItem[index].dataWidget, laString_CreateFromID(string_text_Nullstring));
        }
        indexLogNumInPage++;
    }    
}

void SettingScreen_DataLog_InitAlarmData()
{
    SYS_PRINT("SettingScreen_DataLog_InitAlarmData \n");
    s_numLog = logMgr_ReadLastestLog(eAlarmLogTypeID, MAX_ALARM_DISPLAY, alarmDisplayData);

    // Calculate first page
    s_totalPageNum = s_numLog / MAX_ALARM_IN_PAGE;
    s_totalPageNum = s_numLog % MAX_ALARM_IN_PAGE > 0 ? s_totalPageNum + 1 : s_totalPageNum ;
    if (s_totalPageNum <= 0)
    {
        // limit at 1->+
        s_totalPageNum = 1;
    }
    s_currentPageNum = 1;
    SYS_PRINT("s_totalPageNum: %d \n ", s_totalPageNum);           
    SYS_PRINT("s_currentPageNum: %d \n", s_currentPageNum);        
    SYS_PRINT("s_numLog: %d \n", s_numLog);
}

void SettingScreen_DataLog_NextPage()
{
    if (s_currentPageNum < s_totalPageNum)
    {
        s_currentPageNum++;     
        SettingScreen_SetSettingScreenUpdate(true);
        s_dataLogSettingDisplay = -1;
    }
    SYS_PRINT("SettingScreen_DataLog_NextPage %d \n", s_currentPageNum);
}

void SettingScreen_DataLog_PrevPage()
{
    if (s_currentPageNum > 1 )
    {
        s_currentPageNum--;
        SettingScreen_SetSettingScreenUpdate(true);
        s_dataLogSettingDisplay = -1;
    }
    SYS_PRINT("SettingScreen_DataLog_PrevPage %d \n", s_currentPageNum);
}

void SettingScreen_DataLog_InitEventData()
{
    SYS_PRINT("SettingScreen_DataLog_InitEventData \n");
    s_numLog = logMgr_ReadLastestLog(eEventLogTypeID, MAX_LOG_DISPLAY, eventDisplayData);

    // Calculate first page
    s_totalPageNum = s_numLog / MAX_LOG_IN_PAGE;
    s_totalPageNum = s_numLog % MAX_LOG_IN_PAGE > 0 ? s_totalPageNum + 1 : s_totalPageNum ;
    if (s_totalPageNum <= 0)
    {
        // limit at 1->+
        s_totalPageNum = 1;
    }
    s_currentPageNum = 1;
    SYS_PRINT("s_totalPageNum: %d \n ", s_totalPageNum);           
    SYS_PRINT("s_currentPageNum: %d \n", s_currentPageNum);        
    SYS_PRINT("s_numLog: %d \n", s_numLog);
}

void SettingScreen_DataLog_SetInitDisplayData(bool f)
{
    s_isDataLogSettingDisplayInit = f;
}

bool SettingScreen_DataLog_GetInitDisplayData()
{
    return s_isDataLogSettingDisplayInit;
}

// end of file
