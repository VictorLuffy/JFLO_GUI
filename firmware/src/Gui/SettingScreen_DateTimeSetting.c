/** @file SettingScreen_DateTimeSetting.c
 *  @brief The interfaces for display date time setting
 *  @author Trac Truong
 */

/* This section lists the other files that are included in this file.
 */
#include "Gui/SettingScreen_DateTimeSetting.h"

extern void SettingScreen_SetSettingScreenUpdate(bool f);

static E_DateTimeSetting s_dateTimeSetting = -1;

static E_DateTimeSetting s_dateTimeSettingDisplay = -1;


/**< current value setting */
static DateTimeSetting_Data_Struct s_dateTimeData = {
    .hour = 0,
    .minute = 0,
    .second = 0,
    .year = 0,
    .month = 0,
    .date = 0,
};

/**< init value when open setting */
static DateTimeSetting_Data_Struct s_dateTimeDataInit = {
    .hour = 0,
    .minute = 0,
    .second = 0,
    .year = 0,
    .month = 0,
    .date = 0,
};

/**< value display */
static DateTimeSetting_Data_Struct s_dateTimeDataDisplay = {
    .hour = 0,
    .minute = 0,
    .second = 0,
    .year = 0,
    .month = 0,
    .date = 0,
};

DateTimeSetting_Control_Struct dateTimeSettingControl[eNoOfDateTimeSetting] = {
    [eDefaultDateTimeSetting] = {
        .widget = 0,
        .incFunc = 0,
        .decFunc = 0
    },
    [eHourDateTimeSetting] = {
        .widget = 0,
        .incFunc = &SettingScreen_DateTimeSetting_IncHour,
        .decFunc = &SettingScreen_DateTimeSetting_DecHour
    },
    [eMinuteDateTimeSetting] = {
        .widget = 0,
        .incFunc = &SettingScreen_DateTimeSetting_IncMinute,
        .decFunc = &SettingScreen_DateTimeSetting_DecMinute
    },
    [eSecondDateTimeSetting] = {
        .widget = 0,
        .incFunc = &SettingScreen_DateTimeSetting_IncSecond,
        .decFunc = &SettingScreen_DateTimeSetting_DecSecond
    },
    [eDateDateTimeSetting] = {
        .widget = 0,
        .incFunc = &SettingScreen_DateTimeSetting_IncDate,
        .decFunc = &SettingScreen_DateTimeSetting_DecDate
    },
    [eMonthDateTimeSetting] = {
        .widget = 0,
        .incFunc = &SettingScreen_DateTimeSetting_IncMonth,
        .decFunc = &SettingScreen_DateTimeSetting_DecMonth
    },
    [eYearDateTimeSetting] = {
        .widget = 0,
        .incFunc = &SettingScreen_DateTimeSetting_IncYear,
        .decFunc = &SettingScreen_DateTimeSetting_DecYear
    }
};
void SettingScreen_DateTimeSetting_Init()
{
    dateTimeSettingControl[eYearDateTimeSetting].widget = (laWidget*)SC_MenuSetting_SettingDateTime_YearLabel;
    dateTimeSettingControl[eMonthDateTimeSetting].widget = (laWidget*)SC_MenuSetting_SettingDateTime_MonLabel;
    dateTimeSettingControl[eDateDateTimeSetting].widget = (laWidget*)SC_MenuSetting_SettingDateTime_DateLabel;
    dateTimeSettingControl[eHourDateTimeSetting].widget = (laWidget*)SC_MenuSetting_SettingDateTime_HourLabel;
    dateTimeSettingControl[eMinuteDateTimeSetting].widget = (laWidget*)SC_MenuSetting_SettingDateTime_MinLabel;
    dateTimeSettingControl[eSecondDateTimeSetting].widget = (laWidget*)SC_MenuSetting_SettingDateTime_SecLabel;
       
    RTC_TIME_t currentTime = DisplayControl_GetTime();
   
    s_dateTimeData.second = currentTime.SEC;
    s_dateTimeData.minute = currentTime.MIN;
    s_dateTimeData.hour = currentTime.HOUR;
    s_dateTimeData.date = currentTime.DATE;
    s_dateTimeData.month = currentTime.MONTH;
    s_dateTimeData.year = currentTime.YEAR;
    
    SYS_PRINT("SettingScreen_DateTimeSetting_Init %d/%d/%d %d:%d:%d \n", 
            s_dateTimeData.year,
            s_dateTimeData.month, 
            s_dateTimeData.date,
            s_dateTimeData.hour,
            s_dateTimeData.minute,
            s_dateTimeData.second);
    
    memcpy(&s_dateTimeDataInit, &s_dateTimeData, sizeof(s_dateTimeData));
    
    // init display value with -1 to update with current value
    // if not the data will not be update when back at second time
    memset(&s_dateTimeDataDisplay, -1, sizeof(s_dateTimeData));
    
    // init display value with -1 to update when back at second time
    s_dateTimeSettingDisplay = -1;
    
    SettingScreen_DateTimeSetting_SetSetting(eDefaultDateTimeSetting);
}

void SettingScreen_DateTimeSetting_Display()
{
//    SYS_PRINT("SettingScreen_DateTimeSetting_Display %d \n", s_dateTimeSetting);
    SettingScreen_DateTimeSetting_DisplayTime();
    if (s_dateTimeSettingDisplay == s_dateTimeSetting)
    {
        SYS_PRINT("SettingScreen_DateTimeSetting_Display no update \n");
        return;
    }
    switch(s_dateTimeSetting)
    {
        case eDefaultDateTimeSetting:
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingDateTime_UnderBarPanel, LA_FALSE);
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingDateTime_IncButton, LA_FALSE);
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingDateTime_DecButton, LA_FALSE);
            break;
        case eHourDateTimeSetting:
        case eMinuteDateTimeSetting:
        case eSecondDateTimeSetting:
        case eDateDateTimeSetting:
        case eMonthDateTimeSetting:
        case eYearDateTimeSetting:
            // show inc/dec/underbar button
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingDateTime_UnderBarPanel, LA_TRUE);
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingDateTime_IncButton, LA_TRUE);
            laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingDateTime_DecButton, LA_TRUE);

            // calculate inc/dec/underbar pos
            int32_t x = DisplayControl_GetCenterPostion((laWidget*)dateTimeSettingControl[s_dateTimeSetting].widget, (laWidget*)SC_MenuSetting_SettingDateTime_IncButton);
            
            laWidget_SetX((laWidget*)SC_MenuSetting_SettingDateTime_IncButton, x);
            laWidget_SetX((laWidget*)SC_MenuSetting_SettingDateTime_DecButton, x);

            x = DisplayControl_GetCenterPostion((laWidget*)dateTimeSettingControl[s_dateTimeSetting].widget, (laWidget*)SC_MenuSetting_SettingDateTime_UnderBarPanel);
            laWidget_SetX((laWidget*)SC_MenuSetting_SettingDateTime_UnderBarPanel, x);

            break;
        default:
            break;
    }
    s_dateTimeSettingDisplay = s_dateTimeSetting;
}
void SettingScreen_DateTimeSetting_DisplayTime()
{
    char strbuff[10];
    laString strtime;
    
    // check to not re-run this if not change
    if (s_dateTimeData.hour != s_dateTimeDataDisplay.hour)
    {
        sprintf(strbuff, "%.2d", s_dateTimeData.hour);
        strtime = laString_CreateFromCharBuffer(strbuff, 
            GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S20_Bold, setting_Get(eLanguageSettingId)));
        laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_HourLabel, strtime );     
        laString_Destroy(&strtime);     
        
        s_dateTimeDataDisplay.hour = s_dateTimeData.hour;
    }
    else{
        SYS_PRINT("SettingScreen_DateTimeSetting_DisplayTime no update hour %d / %d \n", s_dateTimeData.hour, s_dateTimeDataDisplay.hour);
    }
    if (s_dateTimeData.minute != s_dateTimeDataDisplay.minute)
    {
        sprintf(strbuff, "%.2d", s_dateTimeData.minute);
        strtime = laString_CreateFromCharBuffer(strbuff, 
            GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S20_Bold, setting_Get(eLanguageSettingId)));
        laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_MinLabel, strtime );     
        laString_Destroy(&strtime);    
        
        s_dateTimeDataDisplay.minute = s_dateTimeData.minute;
    }
    else{
        SYS_PRINT("SettingScreen_DateTimeSetting_DisplayTime no update minute %d / %d \n", s_dateTimeData.minute, s_dateTimeDataDisplay.minute);
    }
    if (s_dateTimeData.second != s_dateTimeDataDisplay.second)
    {
        sprintf(strbuff, "%.2d", s_dateTimeData.second);
        strtime = laString_CreateFromCharBuffer(strbuff, 
            GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S20_Bold, setting_Get(eLanguageSettingId)));
        laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_SecLabel, strtime );     
        laString_Destroy(&strtime);         
        
        s_dateTimeDataDisplay.second = s_dateTimeData.second;
    }
    else{
        SYS_PRINT("SettingScreen_DateTimeSetting_DisplayTime no update second %d / %d \n", s_dateTimeData.second, s_dateTimeDataDisplay.second);
    }
    if (s_dateTimeData.date != s_dateTimeDataDisplay.date)
    {
        sprintf(strbuff, "%.2d", s_dateTimeData.date);
        strtime = laString_CreateFromCharBuffer(strbuff, 
            GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S20_Bold, setting_Get(eLanguageSettingId)));
        laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_DateLabel, strtime );     
        laString_Destroy(&strtime);     
        
        s_dateTimeDataDisplay.date = s_dateTimeData.date;
    }
    else{
        SYS_PRINT("SettingScreen_DateTimeSetting_DisplayTime no update date %d / %d \n", s_dateTimeData.date, s_dateTimeDataDisplay.date);
    }
    if (s_dateTimeData.month != s_dateTimeDataDisplay.month)
    {
        sprintf(strbuff, "%.2d", s_dateTimeData.month);
        strtime = laString_CreateFromCharBuffer(strbuff, 
            GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S20_Bold, setting_Get(eLanguageSettingId)));
        laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_MonLabel, strtime );     
        laString_Destroy(&strtime);       
        
        s_dateTimeDataDisplay.month = s_dateTimeData.month;
    }
    else{
        SYS_PRINT("SettingScreen_DateTimeSetting_DisplayTime no update month %d / %d \n", s_dateTimeData.month, s_dateTimeDataDisplay.month);
    }
    if (s_dateTimeData.year != s_dateTimeDataDisplay.year)
    {
        sprintf(strbuff, "%.2d%.2d", 20, s_dateTimeData.year);
        strtime = laString_CreateFromCharBuffer(strbuff, 
            GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S20_Bold, setting_Get(eLanguageSettingId)));
        laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_YearLabel, strtime );     
        laString_Destroy(&strtime);   
        
        s_dateTimeDataDisplay.year = s_dateTimeData.year;
    }
    else{
        SYS_PRINT("SettingScreen_DateTimeSetting_DisplayTime no update year %d / %d \n", s_dateTimeData.year, s_dateTimeDataDisplay.year);
    }
}
void SettingScreen_DateTimeSetting_UpdateDateWhenYearOrMonthChange()
{
    // FIXME: this fix year range
    uint16_t year = 2000 + s_dateTimeData.year;
    uint8_t max_day = SettingScreen_DateTimeSetting_GetMaxDay(s_dateTimeData.month, year);
    if (s_dateTimeData.date > max_day)
    {
        s_dateTimeData.date = max_day;
        SYS_PRINT("SettingScreen_DateTimeSetting_IncMonth new date %d \n", max_day);
    }    
}

void SettingScreen_DateTimeSetting_IncYear()
{
    uint8_t year2Max = setting_GetMax(eYear2SettingId);
    uint8_t year2Min = setting_GetMin(eYear2SettingId);

    uint8_t newSetting = (s_dateTimeData.year >= year2Max) ? year2Min : s_dateTimeData.year + 1; 
    
    s_dateTimeData.year = newSetting;
    SYS_PRINT("SettingScreen_DateTimeSetting_IncYear %d \n", newSetting);
    
    SettingScreen_DateTimeSetting_UpdateDateWhenYearOrMonthChange();
}

void SettingScreen_DateTimeSetting_DecYear()
{
    uint8_t year2Max = setting_GetMax(eYear2SettingId);
    uint8_t year2Min = setting_GetMin(eYear2SettingId);

    uint8_t newSetting = (s_dateTimeData.year <= year2Min) ? year2Max : s_dateTimeData.year - 1; 
    
    s_dateTimeData.year = newSetting;
    SYS_PRINT("SettingScreen_DateTimeSetting_DecYear %d \n", newSetting);
    
    SettingScreen_DateTimeSetting_UpdateDateWhenYearOrMonthChange();
}

void SettingScreen_DateTimeSetting_IncMonth()
{
    uint8_t monthMax = setting_GetMax(eMonthSettingId);
    uint8_t monthMin = setting_GetMin(eMonthSettingId);

    uint8_t newSetting = (s_dateTimeData.month >= monthMax) ? monthMin : s_dateTimeData.month + 1; 
    
    s_dateTimeData.month = newSetting;
    SYS_PRINT("SettingScreen_DateTimeSetting_IncMonth %d \n", newSetting);
    
    SettingScreen_DateTimeSetting_UpdateDateWhenYearOrMonthChange();
    
}
void SettingScreen_DateTimeSetting_DecMonth()
{
    uint8_t monthMax = setting_GetMax(eMonthSettingId);
    uint8_t monthMin = setting_GetMin(eMonthSettingId);

    uint8_t newSetting = (s_dateTimeData.month <= monthMin) ? monthMax : s_dateTimeData.month - 1; 
    
    s_dateTimeData.month = newSetting;
    SYS_PRINT("SettingScreen_DateTimeSetting_DecMonth %d \n", newSetting);
    
    SettingScreen_DateTimeSetting_UpdateDateWhenYearOrMonthChange();
}
void SettingScreen_DateTimeSetting_IncDate()
{
    uint8_t dateMin = setting_GetMin(eDateSettingId);
        // FIXME: this fix year range
    uint16_t year = 2000 + s_dateTimeData.year;
    uint8_t dateMax = SettingScreen_DateTimeSetting_GetMaxDay(s_dateTimeData.month, year);

    uint8_t newSetting = (s_dateTimeData.date >= dateMax) ? dateMin : s_dateTimeData.date + 1; 
    
    s_dateTimeData.date = newSetting;
    SYS_PRINT("SettingScreen_DateTimeSetting_IncDate %d \n", newSetting);
}
void SettingScreen_DateTimeSetting_DecDate()
{
    uint8_t dateMin = setting_GetMin(eDateSettingId);
        // FIXME: this fix year range
    uint16_t year = 2000 + s_dateTimeData.year;
    uint8_t dateMax = SettingScreen_DateTimeSetting_GetMaxDay(s_dateTimeData.month, year);

    uint8_t newSetting = (s_dateTimeData.date <= dateMin) ? dateMax : s_dateTimeData.date - 1; 
    
    s_dateTimeData.date = newSetting;
    SYS_PRINT("SettingScreen_DateTimeSetting_DecDate %d \n", newSetting);
}

void SettingScreen_DateTimeSetting_IncHour()
{
    uint8_t max = setting_GetMax(eHourSettingId);
    uint8_t min = setting_GetMin(eHourSettingId);
    uint8_t current = s_dateTimeData.hour;
    
    uint8_t newSetting = (current >= max) ? min : current + 1; 
    
    s_dateTimeData.hour = newSetting;
    SYS_PRINT("SettingScreen_DateTimeSetting_IncHour %d \n", newSetting);
}
void SettingScreen_DateTimeSetting_DecHour()
{
    uint8_t max = setting_GetMax(eHourSettingId);
    uint8_t min = setting_GetMin(eHourSettingId);
    uint8_t current = s_dateTimeData.hour;
    
    uint8_t newSetting = (current <= min) ? max : current - 1; 
    
    s_dateTimeData.hour = newSetting;
    SYS_PRINT("SettingScreen_DateTimeSetting_DecHour %d \n", newSetting);
}
void SettingScreen_DateTimeSetting_IncMinute()
{
    uint8_t max = setting_GetMax(eMinuteSettingId);
    uint8_t min = setting_GetMin(eMinuteSettingId);
    uint8_t current = s_dateTimeData.minute;
    
    uint8_t newSetting = (current >= max) ? min : current + 1; 
    
    s_dateTimeData.minute = newSetting;
    SYS_PRINT("SettingScreen_DateTimeSetting_IncMinute %d \n", newSetting);
}
void SettingScreen_DateTimeSetting_DecMinute()
{
    uint8_t max = setting_GetMax(eMinuteSettingId);
    uint8_t min = setting_GetMin(eMinuteSettingId);
    uint8_t current = s_dateTimeData.minute;
    
    uint8_t newSetting = (current <= min) ? max : current - 1; 
    
    s_dateTimeData.minute = newSetting;
    SYS_PRINT("SettingScreen_DateTimeSetting_DecMinute %d \n", newSetting);
}
void SettingScreen_DateTimeSetting_IncSecond()
{
    uint8_t max = setting_GetMax(eSecondSettingId);
    uint8_t min = setting_GetMin(eSecondSettingId);
    uint8_t current = s_dateTimeData.second;
    
    uint8_t newSetting = (current >= max) ? min : current + 1; 
    
    s_dateTimeData.second = newSetting;
    SYS_PRINT("SettingScreen_DateTimeSetting_IncSecond %d \n", newSetting);
}
void SettingScreen_DateTimeSetting_DecSecond()
{
    uint8_t max = setting_GetMax(eSecondSettingId);
    uint8_t min = setting_GetMin(eSecondSettingId);
    uint8_t current = s_dateTimeData.second;
    
    uint8_t newSetting = (current <= min) ? max : current - 1; 
    
    s_dateTimeData.second = newSetting;
    SYS_PRINT("SettingScreen_DateTimeSetting_DecSecond %d \n", newSetting);
}

void SettingScreen_DateTimeSetting_Dec()
{
    if (s_dateTimeSetting < 0 || s_dateTimeSetting >= eNoOfDateTimeSetting)
        return;
    if (dateTimeSettingControl[s_dateTimeSetting].decFunc != 0)
    {
        dateTimeSettingControl[s_dateTimeSetting].decFunc();
        SettingScreen_SetSettingScreenUpdate(true);
    }

}

void SettingScreen_DateTimeSetting_Inc()
{
    if (s_dateTimeSetting < 0 || s_dateTimeSetting >= eNoOfDateTimeSetting)
        return;
    if (dateTimeSettingControl[s_dateTimeSetting].incFunc != 0)
    {
        dateTimeSettingControl[s_dateTimeSetting].incFunc();
        SettingScreen_SetSettingScreenUpdate(true);
    }
}

uint8_t SettingScreen_DateTimeSetting_GetMaxDay(uint8_t month, uint16_t year)
{
    // check boundary
    if (month > 12)
        return 0;

    uint8_t max = 31;

    if (month == 2)
    {
        if (SettingScreen_DateTimeSetting_CheckLeapYear(year))
            max = 29;
        else
            max = 28;
    }
    else
    {
        if (SettingScreen_DateTimeSetting_CheckMonth31(month))
            max = 31;
        else
            max = 30;
    }
    return max;
}

bool SettingScreen_DateTimeSetting_CheckLeapYear(uint16_t year)
{
    // leap year if perfectly divisible by 400
    if (year % 400 == 0) {
        return true;
    }
    // not a leap year if divisible by 100
    // but not divisible by 400
    else if (year % 100 == 0) {
        return false;
    }
    // leap year if not divisible by 100
    // but divisible by 4
    else if (year % 4 == 0) {
        return true;
    }
    // all other years are not leap years
    else {
        return false;
    }
}

bool SettingScreen_DateTimeSetting_CheckMonth31(uint8_t month)
{
    const uint8_t months[7] = {1,3,5,7,8,10,12};
    int i;
    for (i = 0 ; i < 7 ; i ++)
    {
        if (month == months[i])
            return true;
    }
    return false;
}
void SC_MenuSetting_SettingDateTime_YearLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_DateTimeSetting_SetSetting(eYearDateTimeSetting);

    
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_MenuSetting_SettingDateTime_MonLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_DateTimeSetting_SetSetting(eMonthDateTimeSetting);

    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_MenuSetting_SettingDateTime_DateLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_DateTimeSetting_SetSetting(eDateDateTimeSetting);

    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_MenuSetting_SettingDateTime_HourLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_DateTimeSetting_SetSetting(eHourDateTimeSetting);

    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_MenuSetting_SettingDateTime_MinLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_DateTimeSetting_SetSetting(eMinuteDateTimeSetting);

    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}
void SC_MenuSetting_SettingDateTime_SecLabel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_DateTimeSetting_SetSetting(eSecondDateTimeSetting);
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}

void SC_MenuSetting_SettingDateTimePanel_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    SettingScreen_DateTimeSetting_SetSetting(eDefaultDateTimeSetting);
    //this should end of funtion, follow sample code
    evt->event.accepted = LA_TRUE;
}

void SettingScreen_DateTimeSetting_SetCallbackFunction(void)
{
    // Assigning the callback function to widget, these event cannot auto add by generator
    laWidget_OverrideTouchDownEvent((laWidget*)SC_MenuSetting_SettingDateTime_YearLabel, &SC_MenuSetting_SettingDateTime_YearLabel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_MenuSetting_SettingDateTime_MonLabel, &SC_MenuSetting_SettingDateTime_MonLabel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_MenuSetting_SettingDateTime_DateLabel, &SC_MenuSetting_SettingDateTime_DateLabel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_MenuSetting_SettingDateTime_HourLabel, &SC_MenuSetting_SettingDateTime_HourLabel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_MenuSetting_SettingDateTime_MinLabel, &SC_MenuSetting_SettingDateTime_MinLabel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_MenuSetting_SettingDateTime_SecLabel, &SC_MenuSetting_SettingDateTime_SecLabel_PressedEvent);
    laWidget_OverrideTouchDownEvent((laWidget*)SC_MenuSetting_SettingDateTimePanel, &SC_MenuSetting_SettingDateTimePanel_PressedEvent);
    
}

void SettingScreen_DateTimeSetting_SetSetting(E_DateTimeSetting dataTimeSetting)
{
    if (dataTimeSetting < 0 || dataTimeSetting >= eNoOfDateTimeSetting)
        return;
    s_dateTimeSetting = dataTimeSetting;    
    if (s_dateTimeSettingDisplay != s_dateTimeSetting)
    {
        SettingScreen_SetSettingScreenUpdate(true);
    }
}
E_DateTimeSetting SettingScreen_DateTimeSetting_GetSetting()
{
    return s_dateTimeSetting;
}

bool SettingScreen_DateTimeSetting_CheckDataChange()
{
    int ret = memcmp(&s_dateTimeData, &s_dateTimeDataInit, sizeof(s_dateTimeDataInit));
    return ret == 0 ? false : true;
}

void SettingScreen_DateTimeSetting_SaveSetting()
{
    if (!SettingScreen_DateTimeSetting_CheckDataChange())
    {
        SYS_PRINT("SettingScreen_DateTimeSetting_SaveSetting no change \n");
        return;
    }
    SYS_PRINT("SettingScreen_DateTimeSetting_SaveSetting \n");
    
    // Write a log
    Timestamp old = logMgr_getRtcTime();
    Timestamp new = {   .year_1 = 20,
                        .year_2 = s_dateTimeData.year,
                        .month = s_dateTimeData.month,
                        .date = s_dateTimeData.date,
                        .hour = s_dateTimeData.hour,
                        .minute = s_dateTimeData.minute,
                        .second = s_dateTimeData.second,
                    };
    uint8_t logData[16];
    memcpy((void*)logData, (void*)&old, sizeof(Timestamp));
    memcpy((void*)(logData + sizeof(Timestamp)), (void*)&new, sizeof(Timestamp));        
    logInterface_WriteEventLog(sizeof(Timestamp) * 2, (void*)logData, eDateChangeEventLogId);        

    char strbuff[255];
    sprintf(strbuff, "%.2d%.2d-%.2d-%.2d %.2d:%.2d:%.2d -> %.2d%.2d-%.2d-%.2d %.2d:%.2d:%.2d", 
                    logData[0],
                    logData[1],
                    logData[2],
                    logData[3],
                    logData[4],
                    logData[5],
                    logData[6],
                    logData[7],
                    logData[8],
                    logData[9],
                    logData[10],
                    logData[11],
                    logData[12],
                    logData[13]
                    );
    SYS_PRINT("\n %s \n", strbuff);

    // update total operating time
    uint32_t op_time = DisplayControl_GetTotalOperatingTime();
    if (setting_Get(eTreatmentModeSettingId) == eAdultMode)
    {
        DeviceInformation_AddOperationTimeAdult(op_time);
        DeviceInformation_Save();
    }
    else if (setting_Get(eTreatmentModeSettingId) == eChildMode)
    {
        DeviceInformation_AddOperationTimeChild(op_time);
        DeviceInformation_Save();
    }
    
    
    RTC_TIME_t newTime = {.HOUR = s_dateTimeData.hour,
                        .MIN = s_dateTimeData.minute,
                    .SEC = s_dateTimeData.second,
                    .DATE = s_dateTimeData.date,
                    .MONTH = s_dateTimeData.month,
                    .YEAR = s_dateTimeData.year
    };
    rtc_SetTime(newTime);
    rtc_ReadTime(); // update time value in buffer
    DisplayControl_SetStartTime();
    DisplayControl_ResetTotalOperatingTime();
    
    // update init data to not show confirm screen
    SettingScreen_DateTimeSetting_DiscardSetting();
}

void SettingScreen_DateTimeSetting_DiscardSetting()
{
    SYS_PRINT("SettingScreen_DateTimeSetting_DiscardSetting \n");
    memcpy(&s_dateTimeDataInit, &s_dateTimeData, sizeof(s_dateTimeData));
}

// end of file
