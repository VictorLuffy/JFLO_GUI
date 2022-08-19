/** @file DisplayControl.c
 *  @brief Main controller for operating UI
 *  @author Thanh Hiep Nguyen
 */

/* This section lists the other files that are included in this file.
 */
#include "DisplayControl.h"
#include "Gui/VideoScreen.h"
#include "Gui/GuiDefine.h"
#include "stdio.h"
#include "stdarg.h"
#include "Delay.h"
#include "Gui/LogInterface.h"
#include "Gui/LogMgr.h"
#include "Gui/File.h"
#include "Gui/UpdateScreen.h"
#include "Gui/VideoScreen.h"
#include "Gui/MainScreen.h"
#include "Gui/SettingScreen.h"
#include "Gui/StatusBar.h"
#include "Gui/PowerOffScreen.h"
#include "System/ApplicationDefinition.h"
#include "Device/RTC_BQ32002.h"
#include "SpO2Data.h"
#include "AlarmExpression.h"
#include "DeviceInterface.h"
#include "AlarmNotificationList.h"


extern bool g_InitializedSQIFiles;
/** @brief A varible to store the displaycontrol monitoring data */
static Display_Data_Struct   gs_dispData;

/** @brief A varible to store current alarm which reporting on UI */
static AlarmNotification s_currentAlarmNotification = {
    .alarmId = eNoneAlarmId,
    .alarmStatus = eInactive,
    .alarmPriority = eLow1Priority,
    .alarmData = {0,0,0,0,0},
};

/** @brief The timer for update time */   
static SYS_TMR_HANDLE s_updateTime;

/** @brief The timer for update time */   
static SYS_TMR_HANDLE s_updateMonitor;

/** @brief Return temperature value from device
 *  @param [in]  None
 *  @param [out]  None
 *  @return Chamber's Temperature
 */
inline float DisplayControl_GetDataTemp(void)
{
    return gs_dispData.dataTemp.chamberOutTemp + 0.5;
}

/** @brief Return temperature value from device
 *  @param [in]  None
 *  @param [out]  None
 *  @return Chamber's Temperature
 */
inline float DisplayControl_GetBreathCircuitOutTemp(void)
{
    return gs_dispData.dataTemp.breathCircuitOutTemp + 0.5;
}

/** @brief Return flow's speed value from device
 *  @param [in]  None
 *  @param [out]  None
 *  @return Air Flow value
 */
inline float DisplayControl_GetDataAirFlow(void)
{
    return gs_dispData.dataFlow.airFlow;
}

/** @brief Return flow's speed value from device
 *  @param [in]  None
 *  @param [out]  None
 *  @retval None
 */
inline float DisplayControl_GetDataO2Flow(void)
{
    return gs_dispData.dataFlow.o2Flow;
}

/** @brief Return flow's speed value from device
 *  @param [in]  None
 *  @param [out]  None
 *  @retval None
 */
float DisplayControl_GetDataFlow(void)
{
    return (gs_dispData.dataFlow.airFlow + gs_dispData.dataFlow.o2Flow + 0.5);
}

/** @brief Return O2Concentration value from device
 *  @param [in]  None
 *  @param [out]  None
 *  @retval float: O2 Concentration
 */
float DisplayControl_GetDataO2Concentration(void)
{
    return gs_dispData.dataO2Concentration;    
}


/** @brief Return flow's speed value from device
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
float DisplayControl_GetDataSpO2(void)
{
    return gs_dispData.spo2Data.aveValue;
}

/** @brief Get the PR infomation.
 *  @param [in]  None
 *  @param [out]  None
 *  @return float
 *  @retval O2 Concentration
 */
float DisplayControl_GetDataPR(void)
{
    return gs_dispData.spo2Data.pulseRate;    
}

/** @brief Get the current time infomation.
 *  @param [in]   None  
 *  @param [out]  None
 *  @return RTC_TIME_t : hour, minute, second, year, month, date of current time.
 */
inline RTC_TIME_t DisplayControl_GetTime(void)
{
    return gs_dispData.currentRTC;
}

/** @brief Set the alarm status in UI.
 *  @param [in]     E_AlarmStatus status : status of alarm.               
 *  @param [out]  None
 *  @return None
 */
inline uint32_t DisplayControl_GetTotalOperatingTime(void)
{
    return gs_dispData.totalOperatingTime;
}

/** @brief Reset the total operating time in UI.
 *  @param [in]     E_AlarmStatus status : status of alarm.               
 *  @param [out]  None
 *  @return None
 */
inline void DisplayControl_ResetTotalOperatingTime(void)
{
    gs_dispData.totalOperatingTime = 0;
}

/** @brief Be a Periodical function which working for update monitoring figure
 *  @param [in]  uint32_t currTick: GET_TICK() 
 *               bool isInit: true in displaycontrol initialization, false in period function 
 *  @param [out]  None
 *  @return None
 */
static void DisplayControl_UpdateMonitor( uintptr_t context, uint32_t currTick)
{  
#define JFLO_DEBUG_GUI
#ifndef JFLO_DEBUG_GUI
    if (MotorTask_GetPublicData(&gs_dispData.dataFlow) == false) {
        return;
    }
    
    if (HeaterTask_GetPublicData(&gs_dispData.dataTemp) == false) {
        return;
    }
    if (gs_dispData.dataFlow.airFlow + gs_dispData.dataFlow.o2Flow != 0)
    {        
        gs_dispData.dataO2Concentration = ((0.21 * gs_dispData.dataFlow.airFlow + gs_dispData.dataFlow.o2Flow) / 
                (gs_dispData.dataFlow.airFlow + gs_dispData.dataFlow.o2Flow)) * 100.0;
    }
    
    // Write a spo2 data log
    SpO2Data_GetData(&gs_dispData.spo2Data);
#else
    static int counter = 0;
    if (counter++ < 5)
        return;
    else
    {
        counter = 0;
    }
    static uint8_t test1 = 0;
    if (test1++ > 110)
        test1 = 0;

    static uint8_t test2 = 0;
    if (test2++ > 255)
        test2 = 0;
    
    gs_dispData.dataTemp.chamberOutTemp = test1;
    gs_dispData.dataTemp.breathCircuitOutTemp = test1;
    gs_dispData.dataFlow.airFlow = test1;
    
//    if (gs_dispData.dataFlow.o2Flow < 50)
//        gs_dispData.dataFlow.o2Flow = test1;
//    else
//        gs_dispData.dataFlow.o2Flow = 50;
//  
//        gs_dispData.dataO2Concentration = test1;
  
//    static uint32_t xCurrentTick = 0, xPreTick = 0;
    
//    xCurrentTick = xTaskGetTickCount();
//    if (xPreTick == 0)
//    {
//        xPreTick = xCurrentTick;
//    }
//    
//    if (xCurrentTick - xPreTick < 25000)
//    {
//        gs_dispData.dataFlow.o2Flow
//    }
    
    
//    gs_dispData.dataO2Concentration = test1;
    gs_dispData.spo2Data.aveValue = test1;
    gs_dispData.spo2Data.pulseRate = test2;
#endif    
    static uint8_t spo2_value_pre = 0;
    if (gs_dispData.spo2Data.aveValue != spo2_value_pre)
    {
        logInterface_WriteSpO2Log(gs_dispData.spo2Data.aveValue);
        spo2_value_pre = gs_dispData.spo2Data.aveValue;
    }
    return;
}

/** @brief Be a Periodical function which working for update monitoring figure
 *  @param [in]  uint32_t currTick: GET_TICK() 
 *               bool isInit: true in displaycontrol initialization, false in period function 
 *  @param [out]  None
 *  @return None
 */
static void DisplayControl_UpdateDatetime( uintptr_t context, uint32_t currTick)
{    
    RTC_TIME_t newTime;
    uint32_t oldSecond, newSecond;
    int delta;
    
    gs_dispData.isRtcError = rtc_ReportError();
    if (gs_dispData.isRtcError == eDeviceNoError) {
        rtc_GetTime(&newTime);
//        SYS_PRINT("DATE & TIME: %u:%u:%u   %u-%u-20%u\n",newTime.HOUR, newTime.MIN, newTime.SEC, newTime.DAY, newTime.MONTH, newTime.YEAR);
        if (gs_dispData.isCountOpTime)
        {
//            SYS_PRINT("oldSecond %d:%d:%d \n", gs_dispData.currentRTC.HOUR, gs_dispData.currentRTC.MIN, gs_dispData.currentRTC.SEC);
//            SYS_PRINT("newSecond %d:%d:%d \n", newTime.HOUR, newTime.MIN, newTime.SEC);
            oldSecond = gs_dispData.currentRTC.HOUR*3600 + gs_dispData.currentRTC.MIN*60 + gs_dispData.currentRTC.SEC;
            newSecond = newTime.HOUR*3600 + newTime.MIN*60 + newTime.SEC;    
            delta = newSecond - oldSecond;
//            SYS_PRINT("delta %d \n", delta);
            if (delta > 0) 
            {
                gs_dispData.totalOperatingTime += delta;
                gs_dispData.runningTime += delta;
            }
            else if (delta < 0)
            {
                gs_dispData.totalOperatingTime += (86400/*24h*/ + newSecond - oldSecond);
                gs_dispData.runningTime += (86400/*24h*/ + newSecond - oldSecond);
            }
            else
                return;            
        }
    
        gs_dispData.currentRTC = newTime;  
    }
    else {
        SYS_PRINT("\n RTC error");
        gs_dispData.totalOperatingTime++;
        gs_dispData.runningTime++;
    }
    
    return;
}

void DisplayControl_UpdateAlarm(E_AlarmId alarmId, E_AlarmStatus status, E_AlarmPriority priority, uint8_t *data)
{
    if (!data)
        return;

    SYS_PRINT("DisplayControl_UpdateAlarm alarmId %d \n", s_currentAlarmNotification.alarmId);
    SYS_PRINT("DisplayControl_UpdateAlarm pri %d \n", s_currentAlarmNotification.alarmPriority);
    SYS_PRINT("DisplayControl_UpdateAlarm status %d \n", s_currentAlarmNotification.alarmStatus);
    
    if (alarmId != DisplayControl_GetAlarmId())
    {
        if (status == eActive)
        {
            if (DisplayControl_GetAlarmStatus() == eActive)
            {
                SYS_PRINT("AlarmExpression Disable old alarm %d \n", DisplayControl_GetAlarmId());
                if (laContext_GetActiveScreenIndex() == MainScreen_ID)
                {
                    AlarmExpression_Deinit();
                }
            }            
            SYS_PRINT("DisplayControl_UpdateAlarm active alarm %d \n", alarmId);
            AlarmExpression_Active(alarmId, priority);
            AlarmExpression_SetAlarmAdditionalData(data);
            switch(priority)
            {
                case eLow1Priority:
                    deviceInterface_SendAudioEvent(ePlayLow1Alarm, setting_Get(eSpeakerVolumeSettingId));
                    break;
                case eLow2Priority:
                    deviceInterface_SendAudioEvent(ePlayLow1Alarm, setting_Get(eSpeakerVolumeSettingId));
                    break;
                case eMediumPriority:
                    deviceInterface_SendAudioEvent(ePlayMediumAlarm, setting_Get(eSpeakerVolumeSettingId));
                    break;
                case eHighPriority:
                    deviceInterface_SendAudioEvent(ePlayHighAlarm, setting_Get(eSpeakerVolumeSettingId));
                    break;
                default:
                    break;
            }
        }
        else
        {
            SYS_PRINT("DisplayControl_UpdateAlarm do nothing \n");
            // do nothing
        }
    }
    else
    {
        if (status != DisplayControl_GetAlarmStatus())
        {
            if (status == eActive)
            {
                SYS_PRINT("DisplayControl_UpdateAlarm active alarm %d \n", alarmId);
                AlarmExpression_Active(alarmId, priority);
                AlarmExpression_SetAlarmAdditionalData(data);
                switch(priority)
                {
                    case eLow1Priority:
                        deviceInterface_SendAudioEvent(ePlayLow1Alarm, setting_Get(eSpeakerVolumeSettingId));
                        break;
                    case eLow2Priority:
                        deviceInterface_SendAudioEvent(ePlayLow1Alarm, setting_Get(eSpeakerVolumeSettingId));
                        break;
                    case eMediumPriority:
                        deviceInterface_SendAudioEvent(ePlayMediumAlarm, setting_Get(eSpeakerVolumeSettingId));
                        break;
                    case eHighPriority:
                        deviceInterface_SendAudioEvent(ePlayHighAlarm, setting_Get(eSpeakerVolumeSettingId));
                        break;
                    default:
                        break;
                }
                uint8_t logData[2] = {(uint8_t)status, (uint8_t)priority};
                logInterface_WriteAlarmLog(2, logData , alarmId);
            }
            else
            {
                SYS_PRINT("DisplayControl_UpdateAlarm inactive alarm %d \n", alarmId);
                AlarmExpression_Inactive();
                deviceInterface_SendAudioEvent(eStopPlay, 0); 
            }
        }
        else
        {
            if (status == eActive)
            {
                if (priority != DisplayControl_GetAlarmPriority())
                {
                    SYS_PRINT("DisplayControl_UpdateAlarm priority change %d \n", priority);
                    AlarmExpression_SetAlarmPriority(priority);
                    if (laContext_GetActiveScreenIndex() == MainScreen_ID)
                    {
                        AlarmExpression_UpdateAlarmTitle();
                        AlarmExpression_UpdateAlarmIndicator();
                        AlarmExpression_SetIndicatorVisible(true);
                    }
                }
                SYS_PRINT("DisplayControl_UpdateAlarm update data %d \n", data[0]);
                AlarmExpression_SetAlarmAdditionalData(data);
            }
            else
            {
                SYS_PRINT("DisplayControl_UpdateAlarm do nothing \n");
            }
        }
        
    }    
    s_currentAlarmNotification.alarmId = alarmId;
    s_currentAlarmNotification.alarmPriority = priority;
    s_currentAlarmNotification.alarmStatus = status;
    memcpy(s_currentAlarmNotification.alarmData, data, sizeof(s_currentAlarmNotification.alarmData));    
}

void DisplayControl_InactiveCurrentAlarm()
{
    AlarmExpression_Inactive();
    deviceInterface_SendAudioEvent(eStopPlay, 0); 
}

void DisplayControl_PausedCurrentAlarm()
{
    // set current alarm notification to PAUSE state
    AlarmNotification currentAlarmNotification;
    memcpy(&currentAlarmNotification, &s_currentAlarmNotification, sizeof(AlarmNotification));
    currentAlarmNotification.alarmStatus = ePaused;

    AlarmNotificationList_UpdateAlarm(currentAlarmNotification);
    AlarmNotificationList_ProcessAlarmNotificationList();
    AlarmNotificationList_PrintList();
    int index = AlarmNotificationList_GetActiveIndex();
    if (index >=0)
    {
        AlarmNotification alarmUpdate;
        AlarmNotificationList_GetItem(index, &alarmUpdate);
        DisplayControl_UpdateAlarm(
                alarmUpdate.alarmId,
                alarmUpdate.alarmStatus,
                alarmUpdate.alarmPriority,
                alarmUpdate.alarmData
            );            
    }
    else
    {
        // no active alarm, disable current alarm expression
        DisplayControl_InactiveCurrentAlarm();
    }
}

/** @brief Set Alarm Reporting ID in DisplayControl
 *  @param [in]  None
 *  @param [out]  None
 *  @return uint8_t: ID of alarm is reporting on UI
 */
inline uint8_t DisplayControl_GetAlarmId(void)
{
    return s_currentAlarmNotification.alarmId;
}

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return E_AlarmPriority: 
 */
E_AlarmPriority DisplayControl_GetAlarmPriority(void)
{
    return s_currentAlarmNotification.alarmPriority;
}

E_AlarmStatus DisplayControl_GetAlarmStatus(void)
{
    return s_currentAlarmNotification.alarmStatus;
}

/** @brief Initializing the display controller
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void DisplayControl_Initialize (void)
{
    gs_dispData.state = eInitDispState;
    gs_dispData.isCountOpTime = false;
    gs_dispData.runningTime = 0;
    gs_dispData.totalOperatingTime = 0;
        
    AlarmExpression_InitData();
    AlarmExpression_SetAlarmState(eInactiveAlarmState);
    AlarmExpression_SetAlarmPopupAutoShowAtInit(false);
    MainScreen_SetState(eIdleDispState);
    
    StatusBar_Init();
    
#ifdef JFLO_DEBUG_GUI
    MainScreen_SetWarmUpState(eInitWarmUpState);
    StatusBar_SetInternalBatteryStatus(eBatteryLevel1);
    StatusBar_SetExternalBatteryStatus(eBatteryLevel0);
    StatusBar_SetACPowerStatus(eACPowerConnect);
#endif    
    
    return;
}

/** @brief Set displaycontrol's state
 *  @param [in]  state: State in Animation
 *  @param [out]  None
 *  @return None
 */
inline void DisplayControl_SetState(E_DisplayState state) {
    gs_dispData.state = state;
}

/** @brief Get displaycontrol's state
 *  @param [in]  state: State in Animation
 *  @param [out]  None
 *  @return None
 */
E_DisplayState DisplayControl_GetState(void) {
    return gs_dispData.state;
}

bool DisplayControl_SetStartTime(void)
{
    SYS_PRINT("\n DisplayControl_SetStartTime \n");
    rtc_GetTime(&gs_dispData.currentRTC);
    SYS_PRINT("currentRTC %d:%d:%d \n", gs_dispData.currentRTC.HOUR, gs_dispData.currentRTC.MIN, gs_dispData.currentRTC.SEC);
    gs_dispData.isCountOpTime = true;
    
    return true;
}


/** @brief Running display controller
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void DisplayControl_Tasks(void)
{
//    SYS_PRINT("\n\nDisplay State: [%d]", gs_dispData.state);
    /// Update periodical action on UI
//    uint32_t currentTick = (uint32_t)xTaskGetTickCount();
//    SYS_PRINT("\n\nDisplay State: [%d]", currentTick);
//    DisplayControl_SetState(eIdleDispState);
    /// Handle mainscreen operation in normal state
    switch (gs_dispData.state) 
    {
        case eIdleDispState:
            break;
        case eInitDispState:
            if (g_InitializedSQIFiles == false)
                return;
            setting_Restore();
            //TODO: debug_gui
            s_updateTime = SYS_TMR_CallbackPeriodic(
                    1000/*ms*/,
                    1,
                    DisplayControl_UpdateDatetime);
            
            s_updateMonitor = SYS_TMR_CallbackPeriodic(
                    200/*ms*/,
                    1,
                    DisplayControl_UpdateMonitor);                  
            DisplayControl_SetState(eVideoScreenIsShowingDispState); 
            break;
        case eUpdateScreenIsShowingDispState:
            if (laContext_GetActiveScreenIndex() == UpdateScreen_ID)
                UpdateScreen_Run();
            else {
                laContext_SetActiveScreen(UpdateScreen_ID);
//                laContext_SetStringLanguage(setting_Get(eLanguageSettingId));
                laContext_SetStringLanguage(language_English);
            }
            break;
            
        case eVideoScreenIsShowingDispState:
            if (laContext_GetActiveScreenIndex() == VideoScreen_ID)
                VideoScreen_Run();
            else {   
                laContext_SetActiveScreen(VideoScreen_ID);
//                laContext_SetStringLanguage(setting_Get(eLanguageSettingId));
                laContext_SetStringLanguage(language_English);
            }
            break;
            
        case eMainScreenIsShowingDispState:
            if (laContext_GetActiveScreenIndex() == MainScreen_ID)
                MainScreen_Run();              
            else {
                laContext_SetActiveScreen(MainScreen_ID);   
                laContext_SetStringLanguage(language_English);
//                laContext_SetStringLanguage(setting_Get(eLanguageSettingId));
            }
            break;

            
        case eSettingScreenIsShowingDispState:
            if (laContext_GetActiveScreenIndex() == SettingScreen_ID)
                SettingScreen_Run();       
            else {
                laContext_SetActiveScreen(SettingScreen_ID);     
                laContext_SetStringLanguage(setting_Get(eLanguageSettingId));
            }
            break;
        
        case eMaintenanceScreenIsShowingDispState:
            if (laContext_GetActiveScreenIndex() == MaintenanceScreen_ID)
            {
//                SYS_PRINT("\nRun Maintenance.");
                MaintenanceScreen_Run();
            }
            else {
//                SYS_PRINT("\nMaintenance continue.");
                laContext_SetActiveScreen(MaintenanceScreen_ID);       
//                laContext_SetStringLanguage(setting_Get(eLanguageSettingId));
                laContext_SetStringLanguage(language_English);
            }
            break;
        case ePowerOffScreenIsShowingDispState:
        {
            if (laContext_GetActiveScreenIndex() == PowerOffScreen_ID)
            {
//                SYS_PRINT("\nRun PowerOffScreen.");
                PowerOffScreen_Run();
            }
            else
            {
//                SYS_PRINT("\nSet PowerOffScreen.");
                laContext_SetActiveScreen(PowerOffScreen_ID);
                laContext_SetStringLanguage(language_English);
            }
        }
        break;
        default:
            break;
    }
    
    return;
}

uint32_t DisplayControl_GetRunningTime(void)
{
    return gs_dispData.runningTime;
}


void DisplayControl_PrintNum(char* str, int num)
{
    SYS_PRINT(str);
    SYS_PRINT("%d", num);
}

void DisplayControl_PrintHex(char* str, int num)
{
    SYS_PRINT(str);
    SYS_PRINT("%x", num);
}

void DisplayControl_PrintString(const char* str)
{
    SYS_PRINT(str);
}

TickType_t DisplayControl_CalculateDeltaTick(TickType_t current, TickType_t last)
{
    TickType_t delta = current >= last 
            ? current - last 
            : (TickType_t)(-1) - last + current  ;    
    return delta;
}

uint8_t DisplayControl_GetActiveScreenIndex()
{
    return gs_dispData.screenShowId;
}
void DisplayControl_SetActiveScreenIndex(uint8_t id)
{
    gs_dispData.screenShowId = id;
}

void DisplayControl_GoToScreen_SettingScreen_DefaultMenu()
{
    DisplayControl_SetState(eSettingScreenIsShowingDispState);
    SettingScreen_GoToScreen_MenuSetting();    
}

void DisplayControl_GoToScreen_SettingScreen_AlarmDataLog()
{
    DisplayControl_SetState(eSettingScreenIsShowingDispState);
    SettingScreen_GoToScreen_DataLogSetting();
}

void DisplayControl_UpdateAlarmInfoButton(E_AlarmStatus alarmStatus)
{
    if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID 
            && SettingScreen_GetScreenSetting() == eDataLogScreenSetting
            && SettingScreen_DataLog_GetSetting() == eAlarmDataLogSetting)
    {
        SYS_PRINT("DisplayControl_UpdateAlarmInfoButton \n");
        SettingScreen_DataLog_SetInitDisplayData(false); // force to reload data
        SettingScreen_SetSettingScreenUpdate(true);
        return;
    }
    if (alarmStatus == eActive || alarmStatus == eWaitingToActive)
    {
        StatusBar_SetAlarmInfo(true);
    }
}

int32_t DisplayControl_GetCenterPostion(laWidget* widgetA, laWidget* widgetB)
{
    int32_t x_wgt = laWidget_GetX(widgetA);
    int32_t w_wgt = laWidget_GetWidth(widgetA);
    int32_t w_inc = laWidget_GetWidth(widgetB);
    return x_wgt + (w_wgt/2 - w_inc/2);
}

#ifdef JFLO_DEBUG_GUI
static E_AlarmStatus alarmStatus = eInactive;
static E_AlarmId alarmID = eBreathingCircuitNotConnectedAlarmId;
static E_AlarmPriority alarmPriority = eHighPriority;
static uint8_t alarmData[5] = {[ 0 ... 4 ] = 0};

void DisplayControl_TestAlarm()
{
    static TickType_t xPressTickCounter = 0;
    TickType_t delta = DisplayControl_CalculateDeltaTick(xTaskGetTickCount(), xPressTickCounter);
    if (delta < 5000){
        return;
    }
    xPressTickCounter = xTaskGetTickCount();
    alarmID++;
    if (alarmID  >= eLastAlarmId)
        alarmID = eFirsAlarmId;
    alarmData[0] = 0;
    alarmData[1] = 0;
    alarmData[2] = 0;
    SYS_PRINT("DisplayControl_TestAlarm alarm active id %d \n", alarmID);
    alarmStatus = eActive;
    guiInterface_SendEventAlarm(alarmID, alarmStatus, alarmPriority, alarmData);
}
#endif



// end of file
