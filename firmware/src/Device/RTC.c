///** @file RTC.c
// *  @brief This file contains RTC which include RTC initialization,
// *  RTC interrupt handler, and APIs for RTC access.
// *  @author Viet Le
// */
//#include "system_config.h"
//#include "system_definitions.h"
//#include "RTC.h"
//#include "system/rtcc/sys_rtcc_pic32m.h"
//#include "GuiInterface.h"
//
///** @brief Define timestamp number */
//#define TIMESTAMP_NUM 10
//
///** @brief Define Block time mutex */
//#define BLOCKTIME_RTC 10
//
///** @brief Define timestamp struct */
//typedef struct
//{
//    uint32_t stamp[TIMESTAMP_NUM];
//    uint8_t index;
//    uint8_t limit;
//} TIMESTAMP_t;
//
///** @brief Declare local date time */
//static RTC_TIME_t s_localTime = {0};
//
///** @brief Declare rtc alarm handle */
//static SYS_RTCC_ALARM_HANDLE s_rtcAlarm;
//
///** @brief Declare timestamps */
//static TIMESTAMP_t timestamps;
//
///** @brief Semaphore to signal synchronization communication for rtc */
//static SemaphoreHandle_t s_rtcMutex = NULL;
//
///** @brief Declare flag update rtc */
//static bool s_updateRtc = false;
//
///** @brief Callback Functions
// *  @param [in] SYS_RTCC_ALARM_HANDLE handle: rtc alarm handle
// *  @param [in] uintptr_t context: context
// *  @param [out] None
// *  @return None
// */
//void rtc_Callback(SYS_RTCC_ALARM_HANDLE handle, uintptr_t context)
//{
//    /* save typing and help readability */
//    TIMESTAMP_t *times = (TIMESTAMP_t *) context;
//
//    /* the handle will tell us it is our handle */
//    if ((handle == s_rtcAlarm) && times)
//    {
//        s_localTime.YEAR = (uint8_t) PLIB_RTCC_RTCYearGet(RTCC_ID_0);
//        s_localTime.MONTH = (uint8_t) PLIB_RTCC_RTCMonthGet(RTCC_ID_0);
//        s_localTime.DAY = (uint8_t) PLIB_RTCC_RTCDayGet(RTCC_ID_0);
//        s_localTime.HOUR = (uint8_t) PLIB_RTCC_RTCHourGet(RTCC_ID_0);
//        s_localTime.MIN = (uint8_t) PLIB_RTCC_RTCMinuteGet(RTCC_ID_0);
//        s_localTime.SEC = (uint8_t) PLIB_RTCC_RTCSecondGet(RTCC_ID_0);
//
//        //Set flag update to true
//        s_updateRtc = true;
//    }
//
//    return;
//}
//
///** @brief Initialize rtc
// *  @param [in] None
// *  @param [out] None
// *  @return None
// */
//void rtc_Init(void)
//{
//    //Create Semaphore rtc
//    s_rtcMutex = xSemaphoreCreateMutex();
//
//    /* register the callback with system RTCC */
//    timestamps.limit = TIMESTAMP_NUM;
//    timestamps.index = 0;
//    s_rtcAlarm = SYS_RTCC_AlarmRegister(rtc_Callback,
//                                        (uintptr_t) & timestamps);
//
//    /* continue only if the handle is valid */
//    if (s_rtcAlarm != SYS_RTCC_ALARM_HANDLE_INVALID)
//    {
//        SYS_RTCC_AlarmEnable();
//    }
//
//    return;
//}
//
///** @brief Set date time
// *  @param [in] RTC_TIME_t time: date time set
// *  @param [out] None
// *  @return None
// */
//void rtc_Set(RTC_TIME_t time)
//{
//    if (xSemaphoreTake(s_rtcMutex, BLOCKTIME_RTC) == pdTRUE)
//    {
//        //Disable alarm
//        SYS_RTCC_AlarmDisable();       
//        //Set time
//        s_localTime = time;      
//        //Enable alrm
//        SYS_RTCC_AlarmEnable();
//        xSemaphoreGive(s_rtcMutex);
//    }
//    
//    return;
//}
//
///** @brief Get datetime
// *  @param [in] RTC_TIME_t time: data time set
// *  @param [out] None
// *  @return s_localTime: local date time
// */
//RTC_TIME_t rtc_Get(void)
//{
//    RTC_TIME_t time;
//    if (xSemaphoreTake(s_rtcMutex, BLOCKTIME_RTC) == pdTRUE)
//    {
//        time = s_localTime;
//        xSemaphoreGive(s_rtcMutex);
//    }
//    return time;
//}
//
///** @brief Update and send to GUI
// *  @param [in] None
// *  @param [out] None
// *  @return None
// */
//void rtc_CheckAndUpdate(void)
//{
//    //send event to GUI task
//    guiInterface_SendEvent(eGuiUpdateRtcId, 0);
//
//    //Reset flag update
//    s_updateRtc = false;
//
//    return;
//}
//
///* end of file */
