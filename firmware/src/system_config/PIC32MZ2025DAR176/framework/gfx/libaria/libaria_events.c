/*******************************************************************************
  MPLAB Harmony Graphics Composer Generated Implementation File

  File Name:
    libaria_events.c

  Summary:
    Build-time generated implementation from the MPLAB Harmony
    Graphics Composer.

  Description:
    Build-time generated implementation from the MPLAB Harmony
    Graphics Composer.

    Created with MPLAB Harmony Version 2.06
*******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END

#include "gfx/libaria/libaria_events.h"

#include "System/SystemInterface.h"

#include "Gui/DisplayControl.h"
#include "Gui/MainScreen.h"
#include "Gui/SettingScreen.h"
#include "Gui/VideoScreen.h"
#include "Gui/UpdateScreen.h"
#include "Gui/GuiDefine.h"
#include "Gui/GuiInterface.h"
#include "AlarmExpression.h"
#include "LogMgr.h"
#include "Audio.h"
// UpdateScreen - ShowEvent
void UpdateScreen_ShowEvent(laScreen* scr)
{
    // Custom Action
    UpdateScreen_Initialize();
}

// UpdateScreen - HideEvent
void UpdateScreen_HideEvent(laScreen* scr)
{
    // Custom Action
    UpdateScreen_Distructor();
}

// btnMainboard_UpdateScreen - PressedEvent
void btnMainboard_UpdateScreen_PressedEvent(laButtonWidget* btn)
{   
//    guiInterface_SendEvent(eGuiUpdateScreenMessageUpdatingAssets, 0);
//    guiInterface_SendEvent(eGuiUpdateScreenMessageCheckingAsset, 0);
    systemInterface_SendEvent(eSystemUpgradeEventId, 1);
    return;
}

// btnCancel_UpdateScreen - PressedEvent
void btnCancel_UpdateScreen_PressedEvent(laButtonWidget* btn)
{
    // Custom Action
    DisplayControl_SetState(eMaintenanceScreenIsShowingDispState);
}

// btnChooseItem_UpdateScreen - PressedEvent
void btnCradle_UpdateScreen_PressedEvent(laButtonWidget* btn)
{
    systemInterface_SendEvent(eCradleUpgradeEventId, 1);
}
    
// btnChamber_UpdateScreen - PressedEvent
void btnChamber_UpdateScreen_PressedEvent(laButtonWidget* btn)
{
    systemInterface_SendEvent(eChamberUpgradeEventId, 1);
}

// btnOK_UpdateScreen - PressedEvent
void btnOK_UpdateScreen_PressedEvent(laButtonWidget* btn)
{
    laWidget_SetVisible((laWidget*)panelMessageBox_UpdateScreen, LA_FALSE);
}

// VideoScreen - ShowEvent
void VideoScreen_ShowEvent(laScreen* scr)
{
    // Custom Action
    VideoScreen_Initialize();
}

// VideoScreen - HideEvent
void VideoScreen_HideEvent(laScreen* scr)
{
    VideoScreen_Distructor();
}

// MainScreen - ShowEvent
void MainScreen_ShowEvent(laScreen* scr)
{
    // Custom Action
    static bool s_isInitialized = false;
    if (s_isInitialized)
        MainScreen_Reinit();
    else {
        MainScreen_Initialize();
        s_isInitialized = true;
    }
}

// MainScreen - HideEvent
void MainScreen_HideEvent(laScreen* scr)
{
    // Custom Action
    MainScreen_Distructor();
}

// btnInfo - PressedEvent
void btnInfo_PressedEvent(laButtonWidget* btn)
{
    // Custom
    DisplayControl_GoToScreen_SettingScreen_AlarmDataLog();
}

// btnAlarmInfo - PressedEvent
void panelAlarmTitle_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
    if (!AlarmExpression_IsReset() && !AlarmExpression_IsPopupShow())
    {
        AlarmExpression_ShowPopup();
    }
}

// btnTemp_SelectToSetting_1 - PressedEvent
void btnTemp_SelectToSetting_1_PressedEvent(jfloButtonWidget* btn)
{
    MainScreen_ResetButtonPressTickCounter();
    MainScreen_SetButtonPressedState(true);
    MainScreen_SetSettingQuatity(eTempSettingId);
}
// btnTemp_SelectToSetting_1 - ReleasedEvent
void btnTemp_SelectToSetting_1_ReleasedEvent(jfloButtonWidget* btn)
{
    MainScreen_SetButtonPressedState(false);
}

// btnFlow_SelectToSetting_1 - PressedEvent
void btnFlow_SelectToSetting_1_PressedEvent(jfloButtonWidget* btn)
{
    MainScreen_ResetButtonPressTickCounter();
    MainScreen_SetButtonPressedState(true);
    MainScreen_SetSettingQuatity(eFlowSettingId);
}

// btnFlow_SelectToSetting_1 - ReleasedEvent
void btnFlow_SelectToSetting_1_ReleasedEvent(jfloButtonWidget* btn)
{
    MainScreen_SetButtonPressedState(false);
}

// btnO2_SelectToSetting_1 - PressedEvent
void btnO2_SelectToSetting_1_PressedEvent(jfloButtonWidget* btn)
{
    MainScreen_ResetButtonPressTickCounter();
    MainScreen_SetButtonPressedState(true);
    MainScreen_SetSettingQuatity(eFiO2SettingId);
}

// btnO2_SelectToSetting_1 - ReleasedEvent
void btnO2_SelectToSetting_1_ReleasedEvent(jfloButtonWidget* btn)
{
    MainScreen_SetButtonPressedState(false);
}

// btnTemp_SelectToSetting_2 - PressedEvent
void btnTemp_SelectToSetting_2_PressedEvent(jfloButtonWidget* btn)
{
    MainScreen_SetSettingQuatity(eTempSettingId);
    MainScreen_SetSubstate(eSettingChangedDispSubstate);
    MainScreen_ResetButtonNoActionTickCounter();
}

// btnFlow_SelectToSetting_2 - PressedEvent
void btnFlow_SelectToSetting_2_PressedEvent(jfloButtonWidget* btn)
{
    MainScreen_SetSettingQuatity(eFlowSettingId);
    MainScreen_SetSubstate(eSettingChangedDispSubstate);
    MainScreen_ResetButtonNoActionTickCounter();
}

// btnO2_SelectToSetting_2 - PressedEvent
void btnO2_SelectToSetting_2_PressedEvent(jfloButtonWidget* btn)
{
    MainScreen_SetSettingQuatity(eFiO2SettingId);
    MainScreen_SetSubstate(eSettingChangedDispSubstate);
    MainScreen_ResetButtonNoActionTickCounter();
    MainScreen_o2FlowChangeTickCounter();
}

// btnMinus - PressedEvent
void btnMinus_PressedEvent(jfloButtonWidget* btn)
{
    MainScreen_HandleBtnMinus();
    MainScreen_ResetButtonNoActionTickCounter();
}

// btnBack - PressedEvent
void btnBack_PressedEvent(jfloButtonWidget* btn)
{
    // cbBtnBack_Pressed
    MainScreen_SetAnimType(eMoveOut,g_ChangedSetpointArea_Anim);

    AnimationControl_StopAnim(g_ChangedSetpointArea_Anim);
    AnimationControl_StartAnim(g_ChangedSetpointArea_Anim);     

    MainScreen_SetSubstate(eQuitSettingScreenDispSubstate);
}

// btnPlus - PressedEvent
void btnPlus_PressedEvent(jfloButtonWidget* btn)
{
    MainScreen_HandleBtnPlus();
    MainScreen_ResetButtonNoActionTickCounter();    
}

// btnOK - PressedEvent
void btnOK_PressedEvent(jfloButtonWidget* btn)
{
    // cbBtnOK_Pressed
    MainScreen_HandleBtnOK();

    MainScreen_SetAnimType(eMoveOut,g_ChangedSetpointArea_Anim);     /// Problem

    AnimationControl_StopAnim(g_ChangedSetpointArea_Anim);
    AnimationControl_StartAnim(g_ChangedSetpointArea_Anim);

    MainScreen_SetSubstate(eQuitSettingScreenDispSubstate);
}

// btnX_AlarmArea - PressedEvent
void btnX_AlarmArea_PressedEvent(laButtonWidget* btn)
{
    AlarmExpression_CloseButtonPressed();
}

// btnAlarmReset_AlarmArea - PressedEvent
void btnAlarmReset_AlarmArea_PressedEvent(laButtonWidget* btn)
{
    AlarmExpression_ResetButtonPressed();
}

// Generated Event Handler - Origin: SettingScreen, Event: ShowEvent
void SettingScreen_ShowEvent(laScreen* scr)
{
    SettingScreen_Initialize();
//    static bool s_isInitalized = false;
//    if (s_isInitalized)
//        SettingScreen_Reinit();
//    else {
//        SettingScreen_Initialize();
//        s_isInitalized = true;
//    }    
}

// Generated Event Handler - Origin: SettingScreen, Event: HideEvent
void SettingScreen_HideEvent(laScreen* scr)
{
    SettingScreen_Distructor();
}

// btnAlarmOpt - PressedEvent
void pnlAlarmOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
//    evt->event.accepted = LA_TRUE;
//    SettingScreen_HandlePanelTouch(eAlarmDataTableSettingMenuId);
}

// btnUpdate_AlarmTable_SettingScreen - PressedEvent
void btnUpdate_AlarmTable_SettingScreen_PressedEvent(laButtonWidget* btn)
{
    // custom
    
}

// btnUpdate_AlarmTable_SettingScreen - ReleasedEvent
void btnUpdate_AlarmTable_SettingScreen_ReleasedEvent(laButtonWidget* btn)
{
    // custom
    
}

// btnPrev_AlarmTable_SettingScreen - PressedEvent
void btnPrev_AlarmTable_SettingScreen_PressedEvent(laButtonWidget* btn)
{
    // custom
    int num = -1;
    SettingScreen_HandleAlarmDataTableSettingId((void*)&num) ;   
    
}

// btnNext_AlarmTable_SettingScreen - PressedEvent
void btnNext_AlarmTable_SettingScreen_PressedEvent(laButtonWidget* btn)
{
    // custom
    int num = 1;
    SettingScreen_HandleAlarmDataTableSettingId((void*)&num);
}

// btnLanguage - PressedEvent
void pnlLanguageOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
//    SettingScreen_HandlePanelTouch(eLanguageSettingMenuId);
//    evt->event.accepted = LA_TRUE;
}

// btnlLanguageSw - PressedEvent
void btnlLanguageSw_PressedEvent(laButtonWidget* btn)
{    
//    if(SettingScreen_GetState() == eShowingComponentDispState)
//    {
//        SettingScreen_HandleLanguageOpt();
//    }   
    
//    laWidget_Invalidate(pnlLangOpt);
}

// btnMode - PressedEvent
void pnlModeOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
//    evt->event.accepted = LA_TRUE;
//    SettingScreen_HandlePanelTouch(eTreatmentModeSettingMenuId);
}

// btnModeSw - PressedEvent
void btnModeSw_PressedEvent(laButtonWidget* btn)
{
//    if(SettingScreen_GetState() == eShowingComponentDispState)
//    {
//        SettingScreen_HandleModeOpt();
//    }      
}

// btnBackgroundOpt - PressedEvent
void pnlBackgroundOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
//    evt->event.accepted = LA_TRUE;
//    SettingScreen_HandlePanelTouch(eBackgroundSettingMenuId);
}

// btnBackgroundSw - PressedEvent
void btnBackgroundSw_PressedEvent(laButtonWidget* btn)
{
//    if(SettingScreen_GetState() == eShowingComponentDispState)
//    {
//        SettingScreen_HandleBackgroundOpt();
//    }              
}

// btnBluetoothOpt - PressedEvent
void pnlBluetoothOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
//    evt->event.accepted = LA_TRUE;
//    SettingScreen_HandlePanelTouch(eBluetoothSettingMenuId);
}

// btnBluettoothSw - PressedEvent
void btnBluettoothSw_PressedEvent(laButtonWidget* btn)
{
//    if(SettingScreen_GetState() == eShowingComponentDispState && SettingScreen_GetOption() == eBluetoothSettingMenuId)
//    {
//        SettingScreen_HandleBluetoothOpt();
//    }    
//    laWidget_Invalidate(pnlBluetoothOpt);    
}

// btnBrightness - PressedEvent
void pnlBrightnessOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
//    evt->event.accepted = LA_TRUE;
//    SettingScreen_HandlePanelTouch(eBrightnessSettingMenuId);
}

// sldBrightnessChg - ValueChangedEvent
void sldBrightnessChg_ValueChangedEvent(laSliderWidget* sld)
{
//    if(SettingScreen_GetState() == eShowingComponentDispState && SettingScreen_GetOption() == eBrightnessSettingMenuId)
//    {
//        SettingScreen_HandleBrightnessOpt((void*)sldBrightnessChg);
//    }  
}

// sldBrightnessChg - ValueChangedEvent
void btnBrightnessModeSw_PressedEvent(laButtonWidget* btn)
{
//    if(SettingScreen_GetState() == eShowingComponentDispState && SettingScreen_GetOption() == eBrightnessSettingMenuId)
//    {
//        SettingScreen_HandleBrightnessOpt((void*) btnBrightnessModeSw);
//    }
}

// btnLogOpt - PressedEvent
void pnlLogOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
//    evt->event.accepted = LA_TRUE;
//    SettingScreen_HandlePanelTouch(eLogDataTableSettingMenuId);
}

// btnUpdate_LogTable_SettingScreen - PressedEvent
void btnUpdate_LogTable_SettingScreen_PressedEvent(laButtonWidget* btn)
{
    // custom
    
}

// btnUpdate_LogTable_SettingScreen - ReleasedEvent
void btnUpdate_LogTable_SettingScreen_ReleasedEvent(laButtonWidget* btn)
{
    // custom
    
}

// SC_AlarmInfoButton - PressedEvent
void SC_AlarmInfoButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_GoToScreen_DataLogSetting();
}

// SC_HomeButton - PressedEvent
void SC_HomeButton_PressedEvent(laButtonWidget* btn)
{
    // Custom
    SettingScreen_GoToScreen_BackToMainScreen();
}

// SC_SettingButton - PressedEvent
void SC_SettingButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_GoToScreen_MenuSetting();
}

// SC_DataLogButton - PressedEvent
void SC_DataLogButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_GoToScreen_DataLogSetting();
}

// SC_MaintenanceButton - PressedEvent
void SC_MaintenanceButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_GoToScreen_Maintenance();
}

// SC_DeviceInformationButton - PressedEvent
void SC_DeviceInformationButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_GoToScreen_DeviceInformation();
}

// SC_MenuSetting_SettingLanguage_NextButton - PressedEvent
void SC_MenuSetting_SettingLanguage_NextButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_LanguageSetting_NextSetting();
}

// SC_MenuSetting_SettingLanguage_PrevButton - PressedEvent
void SC_MenuSetting_SettingLanguage_PrevButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_LanguageSetting_PrevSetting();
}

// SC_MenuSetting_SettingBrightness_NextButton - PressedEvent
void SC_MenuSetting_SettingBrightness_NextButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_BrightnessSetting_Inc();
}

// SC_MenuSetting_SettingBrightness_PrevButton - PressedEvent
void SC_MenuSetting_SettingBrightness_PrevButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_BrightnessSetting_Dec();
}

// SC_MenuSetting_SettingSpo2Alarm_NextButton - PressedEvent
void SC_MenuSetting_SettingSpo2Alarm_NextButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_SpO2AlarmLimitSetting_Inc();
}

// SC_MenuSetting_SettingSpo2Alarm_PrevButton - PressedEvent
void SC_MenuSetting_SettingSpo2Alarm_PrevButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_SpO2AlarmLimitSetting_Dec();
}
// btnPrev_LogTable_SettingScreen - PressedEvent
void btnPrev_LogTable_SettingScreen_PressedEvent(laButtonWidget* btn)
{
//    // custom
//    int num = -1;
//    SettingScreen_HandleLogDataTableSettingId((void*)&num) ;      
}

// btnNext_LogTable_SettingScreen - PressedEvent
void btnNext_LogTable_SettingScreen_PressedEvent(laButtonWidget* btn)
{
//    // custom
//    int num = 1;
//    SettingScreen_HandleLogDataTableSettingId((void*)&num);    
}

// btnDateTimeOpt - PressedEvent
void pnlDateTimeOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
//    evt->event.accepted = LA_TRUE;
//    SettingScreen_HandlePanelTouch(eDateTimeSettingMenuId);
}

// btnDecDateTimeSetting - PressedEvent
void btnDecDateTimeSetting_PressedEvent(laButtonWidget* btn)
{
//    if (SettingScreen_GetState() == eShowingComponentDispState)
//    {
//        interWgt.command = eDecValueCommand;
//        SettingScreen_HandleDateTimeOpt((void*)&interWgt );
//    }
}

// btnIncDateTimeSetting - PressedEvent
void btnIncDateTimeSetting_PressedEvent(laButtonWidget* btn)
{
//    if (SettingScreen_GetState() == eShowingComponentDispState)
//    {
//        interWgt.command = eIncValueCommand;
//        SettingScreen_HandleDateTimeOpt((void*)&interWgt );
//    }    
}

// btnCancelDateTimeSetting - PressedEvent
void btnCancelDateTimeSetting_PressedEvent(laButtonWidget* btn)
{
//    if (SettingScreen_GetState() == eShowingComponentDispState)
//    {
//        interWgt.command = eCancelCommand;
//        SettingScreen_HandleDateTimeOpt((void*)&interWgt );
//    }        
}

// btnOkDateTimeSetting - PressedEvent
void btnOkDateTimeSetting_PressedEvent(laButtonWidget* btn)
{
//    if (SettingScreen_GetState() == eShowingComponentDispState)
//    {
//        interWgt.command = eChangeValueCommand;
//        SettingScreen_HandleDateTimeOpt((void*)&interWgt );
//    }      
}

// btnWifiOpt - PressedEvent
void pnlWifiOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
//    evt->event.accepted = LA_TRUE;
//    SettingScreen_HandlePanelTouch(eWifiSettingMenuId);
}

// btnWifiSw - PressedEvent
void btnWifiSw_PressedEvent(laButtonWidget* btn)
{
//    if(SettingScreen_GetState() == eShowingComponentDispState & SettingScreen_GetOption() == eWifiSettingMenuId)
//    {
//        SettingScreen_HandleWifiOpt();
//    }
//    laWidget_Invalidate(pnlWifiOpt);
}

// Generated Event Handler - Origin: btnSpeaker, Event: PressedEvent
void pnlSpeakerOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt)
{
//    evt->event.accepted = LA_TRUE;
//    SettingScreen_HandlePanelTouch(eSpeakerVolumeSettingMenuId);
}

// Generated Event Handler - Origin: sldSpeakerChg, Event: ValueChangedEvent
void sldSpeakerChg_ValueChangedEvent(laSliderWidget* sld)
{
//    if(SettingScreen_GetState() == eShowingComponentDispState)
//    {
//        SettingScreen_HandleSpeakerOpt();
//    }  
}

// SC_DataLogSettingPrevButton - PressedEvent
void SC_DataLogSettingPrevButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_DataLog_PrevPage();
}

// SC_DataLogSettingNextButton - PressedEvent
void SC_DataLogSettingNextButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_DataLog_NextPage();
}

// SC_MenuSetting_SettingBackButton - PressedEvent
void SC_MenuSetting_SettingBackButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_GoToScreen_MenuSetting();
}

// SC_MenuSetting_SettingSaveButton - PressedEvent
void SC_MenuSetting_SettingSaveButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_SaveMenuSetting();
    SettingScreen_GoToScreen_MenuSetting();
}

// SC_MenuSetting_SettingDateTime_DecButton - PressedEvent
void SC_MenuSetting_SettingDateTime_DecButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_DateTimeSetting_Dec();
}

// SC_MenuSetting_SettingDateTime_IncButton - PressedEvent
void SC_MenuSetting_SettingDateTime_IncButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_DateTimeSetting_Inc();
}

// SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton - PressedEvent
void SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_OxygenConcentrationAlarmLimitSetting_UpperDec();
}

// SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton - PressedEvent
void SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_OxygenConcentrationAlarmLimitSetting_LowerInc();
}

// SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton - PressedEvent
void SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_OxygenConcentrationAlarmLimitSetting_LowerDec();
}

// SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton - PressedEvent
void SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_OxygenConcentrationAlarmLimitSetting_UpperInc();
}

// SC_MenuSetting_SettingAlarmSoundLevel_NextButton - PressedEvent
void SC_MenuSetting_SettingAlarmSoundLevel_NextButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_AlarmSoundLevelSetting_Inc();
}

// SC_MenuSetting_SettingAlarmSoundLevel_PrevButton - PressedEvent
void SC_MenuSetting_SettingAlarmSoundLevel_PrevButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_AlarmSoundLevelSetting_Dec();
}

// SC_SaveConfirmYesButton - PressedEvent
void SC_SaveConfirmYesButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_SaveMenuSetting();
    
    // resume next screen
    SettingScreen_SetNextScreenSetting(SettingScreen_GetNextScreenSetting());
}

// SC_SaveConfirmNoButton - PressedEvent
void SC_SaveConfirmNoButton_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_DiscardMenuSetting();
    
    // resume next screen
    SettingScreen_SetNextScreenSetting(SettingScreen_GetNextScreenSetting());
}

// MaintenanceScreen - ShowEvent
void MaintenanceScreen_ShowEvent(laScreen* scr)
{
    static bool s_isInitialized = false;
    if (s_isInitialized)
        MaintenanceScreen_Reinit();
    else {
        MaintenanceScreen_Initialize();
        s_isInitialized = true;
    }
}

// MaintenanceScreen - HideEvent
void MaintenanceScreen_HideEvent(laScreen* scr)
{
    MaintenanceScreen_Distructor();
}

// btnFormatSQI - PressedEvent
void btnFormatSQI_PressedEvent(laButtonWidget* btn)
{
    // Custom
    systemInterface_SendEvent(eSystemFormatSQIEventId, 0);
}

// btnHome_MaintenanceScreen - PressedEvent
void btnHome_MaintenanceScreen_PressedEvent(laButtonWidget* btn)
{
    // Custom
    DisplayControl_SetState(eMainScreenIsShowingDispState);  
}

// scrollbarMotor - ValueChangedEvent
void scrollbarMotor_ValueChangedEvent(laSliderWidget* sld)
{
    // Custom
    int32_t valueMotor = laSliderWidget_GetSliderValue(scrollbarMotor) + 5;
    
    //send event to Flow Control Task to start motor
    MOTOR_CTRL_EVENT_t event = {.id = eMotorPowerId, .fData = (float) valueMotor};
    MotorTask_SendEvent(event);    
}

// btnOnOffMotor - PressedEvent
void btnOnOffMotor_PressedEvent(laButtonWidget* btn)
{
    // Custom
    static bool s_isOperating = true;
    char strbuff[10];
    laString str;
    
    if (s_isOperating)
    {
        s_isOperating = false;
        
        sprintf(strbuff, "OFF" );
        
        MOTOR_CTRL_EVENT_t event = {.id = eMotorModeId, .iData = eMotorPowerCtrlMode};
        //send event to Flow Control Task to start motor
        MotorTask_SendEvent(event);
        event.id = eMotorStartId;
        event.iData = 0;
        MotorTask_SendEvent(event);           
    } else {
        s_isOperating = true;
        
        sprintf(strbuff, "ON" );
    
        MOTOR_CTRL_EVENT_t event = {.id = eMotorStopId, .iData = 0};
        //send event to Flow Control Task to start motor
        MotorTask_SendEvent(event);     
    }

    str = laString_CreateFromCharBuffer(strbuff, 
        GFXU_StringFontIndexLookup(&stringTable, string_Alphabet_Abel_Regular_S15_Bold, setting_Get(eLanguageSettingId)));
    
    laButtonWidget_SetText(btnOnOffMotor, str);
    
    laString_Destroy(&str);      
}

// scrollbarIH - ValueChangedEvent
void scrollbarIH_ValueChangedEvent(laSliderWidget* sld)
{
    // Custom
    int32_t valueIH = laSliderWidget_GetSliderValue(scrollbarIH) + 30;
    
    //send event to Flow Control Task to start motor
    MOTOR_CTRL_EVENT_t event = {.id = eHeaterTempId, .fData = (float) valueIH};
    MotorTask_SendEvent(event);      
}

// btnOnOffIH - PressedEvent
void btnOnOffIH_PressedEvent(laButtonWidget* btn)
{
    // Custom
    static bool s_isOperating = true;
    char strbuff[10];
    laString str;
    //Action
    
    if (s_isOperating) {
        s_isOperating = false;
        
        sprintf(strbuff, "OFF" );
        
        HEATER_CTRL_EVENT_t event = {.id = eHeaterStopId, .iData = 0};
        HeaterTask_SendEvent(event);         
    } else {
        s_isOperating = true;
        sprintf(strbuff, "ON" );
        
        HEATER_CTRL_EVENT_t event = {.id = eHeaterStartId, .iData = 0};
        HeaterTask_SendEvent(event);          
    }
      
    //Update UI
    str = laString_CreateFromCharBuffer(strbuff, 
        GFXU_StringFontIndexLookup(&stringTable, string_Alphabet_Abel_Regular_S15_Bold, setting_Get(eLanguageSettingId)));
    
    laButtonWidget_SetText(btnOnOffIH, str);
    
    laString_Destroy(&str);  
}

// btnOnOffWaterPump - PressedEvent
void btnOnOffWaterPump_PressedEvent(laButtonWidget* btn)
{
    // Custom
    static bool s_isOperating = true;
    char strbuff[10];
    laString str;
    //Action
    
    if (s_isOperating) {
        s_isOperating = false;
        
        sprintf(strbuff, "OFF" );
        
        HEATER_CTRL_EVENT_t event = {.id = eHeaterStopWaterPumpId, .iData = 0};
        HeaterTask_SendEvent(event);         
    } else {
        s_isOperating = true;
        sprintf(strbuff, "ON" );
        
        HEATER_CTRL_EVENT_t event = {.id = eHeaterStartWaterPumpId, .iData = 0};
        HeaterTask_SendEvent(event);          
    }

    //Update UI
    str = laString_CreateFromCharBuffer(strbuff, 
        GFXU_StringFontIndexLookup(&stringTable, string_Alphabet_Abel_Regular_S15_Bold, setting_Get(eLanguageSettingId)));
    
    laButtonWidget_SetText(btnOnOffWaterPump, str);
    
    laString_Destroy(&str);  
    
}

// btnUpgrade - PressedEvent
void btnUpgrade_PressedEvent(laButtonWidget* btn)
{
    DisplayControl_SetState(eUpdateScreenIsShowingDispState);    
}

// btnLogtoUsb - PressedEvent
void btnLogtoUsb_PressedEvent(laButtonWidget* btn)
{
    if (FileSystemMgr_IsUSBMounted() == true) {
        logInterface_SendLogRequest(eUSBGetEventLogRequestId);
        logInterface_SendLogRequest(eUSBGetAlarmLogRequestId);
        logInterface_SendLogRequest(eUSBGetSpO2LogRequestId);
        
        LogInterface_ExportDebugLogFileToUSB();
        
    }
    else
    {
        SYS_PRINT("USB not found \n");
    }
}

// btnClearLog - PressedEvent

void btnClearLog_PressedEvent(laButtonWidget* btn)
{
    SYS_PRINT("btnClearLog_PressedEvent\n");  
    logInterface_SendLogRequest(eDeleteEventLogRequestId);
    logInterface_SendLogRequest(eDeleteAlarmLogRequestId);
    logInterface_SendLogRequest(eDeleteSpO2LogRequestId);
    logInterface_SendLogRequest(eDeleteDeviceInfoLogRequestId);
//    LogInterface_ClearDebugLogFile();
    
    return;
}
// btnTestHighAlarm_MaintenanceScreen - PressedEvent
static E_AlarmStatus alarmStatus = eInactive;
static E_AlarmId alarmID = eSpeakerDisconnectedAlarmId;
static E_AlarmPriority alarmPriority = eHighPriority;
static uint8_t alarmData[5] = {[ 0 ... 4 ] = 0};
static uint8_t count = 0;
void btnTestHighAlarm_MaintenanceScreen_PressedEvent(laButtonWidget* btn)
{
//     Show Screen (MainScreen) - Show Screen - MainScreen
    if (alarmStatus == eInactive)
    {
        alarmStatus = eActive;
        SYS_PRINT("MaintenanceScreen alarm Active \n");
    }
    else
    {
        alarmStatus = eInactive;
        SYS_PRINT("MaintenanceScreen alarm Inactive \n");
    }
    guiInterface_SendEventAlarm(alarmID, alarmStatus, alarmPriority, alarmData);
}
//#define JFLO_DEBUG_ALARM
// btnTestMediumAlarm_MaintenanceScreen - PressedEvent
void btnTestMediumAlarm_MaintenanceScreen_PressedEvent(laButtonWidget* btn)
{
//    alarmID = alarmID == eBreathingCircuitNotConnectedAlarmId ? eCheckConnectionChamberAlarmId : eBreathingCircuitNotConnectedAlarmId;
    SYS_PRINT("\nIncrease AlarmID %d", count);
//#define JFLO_DEBUG_ALARM
#ifdef JFLO_DEBUG_ALARM
    count++;
    alarmStatus = eInactive;
    guiInterface_SendEventAlarm(alarmID, alarmStatus, alarmPriority, alarmData);
    vTaskDelay(20);
    switch(count)
    {
        case 1: alarmID =   eDeviceErrorToAchieveTargetTemperatureAlarmId;//eBreathingCircuitNotConnectedAlarmId;     break;// E001
        case 2: alarmID =   eCheckLeakAlarmId;     break;// E002
        case 3: alarmID =   eCheckBlockageAlarmId;     break;// E003
        case 4: alarmID =   eCheckConnectionChamberAlarmId;     break;// E007
        case 5: alarmID =   eLowTemperatureAlarmId;     break;// E010
        case 6: alarmID =   eRoomTemperatureLowToAchieveTargetTemperatureAlarmId;     break;// E013
        case 7: alarmID =   eDevicePostureBadAlarmId;     break;// E018
        case 8: alarmID =   eDevicePostureAbnormalAlarmId;     break;// E019
        case 9: alarmID =   eBatteryLowAlarmId;     break;// E021
        case 10: alarmID =  eBatteryGetsRunOutAlarmId;     break;// E022
        case 11: alarmID =  eStopFunctionAlarmId;     break;// E023
        case 12: alarmID =  ePowerNotEnoughAlarmId;     break;// E024
        case 13: alarmID =  eWaterSupplyCheckAlarmId;     break;// E030
        case 14: alarmID =  eWaterTankConnectionCheckAlarmId;     break;// E031
        case 15: alarmID =  eMainUnitBatteryDisconnectedAlarmId;     break;// E033
        case 16: alarmID =  eFailureAccelerationSensorAlarmId;     break;// E111
        case 17: alarmID =  eFailureLightSensorAlarmId;     break;// E112
        case 18: alarmID =  eMainUnitBatteryCommunicationErrorAlarmId;     break;// E113
        case 19: alarmID =  eCradleBatteryCommunicationErrorAlarmId;     break;// E114
        case 20: alarmID =  eCradleCommunicationErrorAlarmId;     break;// E115
        case 21: alarmID =  eMainMCUFailedOrOutOfControlAlarmId;     break;// E116
        case 22: alarmID =  eESP32FailedAlarmId;     break;// E117
        case 23: alarmID = eFailureSpo2ModuleAlarmId; break; //E119
        case 24: alarmID = eFailureRTCModuleAlarmId; break; //E120
        case 25: alarmID = eFailureLCDTouchModuleAlarmId; break;//E121
        case 26: alarmID = eChamberTemperatureAbnormalAlarmId; break; //E124
        case 27: alarmID = eTooMuchWaterInTheChamberAlarmId; break; //E125
    }
#else
    alarmID++; 
#endif
//    alarmID =  eFailureLightSensorAlarmId;
    alarmPriority = eLow1Priority;
    if (alarmPriority  > eHighPriority)
        alarmPriority = eHighPriority;
    if (alarmID  >= eLastAlarmId)
        alarmID = eFirsAlarmId;
    alarmData[0] = 0;
    alarmData[1] = 0;
    alarmData[2] = 0;
    SYS_PRINT("MaintenanceScreen alarm active id %d \n", alarmID);
    alarmStatus = eActive;
    guiInterface_SendEventAlarm(alarmID, alarmStatus, alarmPriority, alarmData);

}
// btnTestLowAlarm_MaintenanceScreen - PressedEvent
void btnTestLowAlarm_MaintenanceScreen_PressedEvent(laButtonWidget* btn)
{
    alarmPriority++;
    if (alarmPriority  > eHighPriority)
        alarmPriority = eLow1Priority;
    
    SYS_PRINT("MaintenanceScreen alarm priority %d \n", alarmPriority);
    alarmStatus = eActive;
    guiInterface_SendEventAlarm(alarmID, alarmStatus, alarmPriority, alarmData);

}
void buttonMessageBoxCancel_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_SetMessageBoxVisible(false);
}

void buttonMessageBoxOK_PressedEvent(laButtonWidget* btn)
{
    SettingScreen_SetMessageBoxVisible(false);
    SettingScreen_MessageBoxOKClicked();
    SettingScreen_HandleModeOpt();
}

// btnDebug4_MaintenanceScreen - PressedEvent
void btnDebug4_MaintenanceScreen_PressedEvent(laButtonWidget* btn)
{
    setting_SetDefault();
    setting_Save();
    
    DeviceInformation_SetDefault();
    DeviceInformation_Save();
}

// btnDebug5_MaintenanceScreen - PressedEvent
bool test_ACPower = false;
void btnDebug5_MaintenanceScreen_PressedEvent(laButtonWidget* btn)
{
//    static bool isMute = false;
//    if (isMute)
//    {
//        guiInterface_SendEvent(eGuiMainScreenAlarmMuteIconShow, 0); 
//        isMute = false;
//    }
//    else
//    {
//        guiInterface_SendEvent(eGuiMainScreenAlarmMuteIconHide, 0); 
//        isMute = true;
//    }
//    test_ACPower = !test_ACPower;
//    DisplayControl_SetState(eMainScreenIsShowingDispState);
    systemInterface_SendEvent(eSystemFormatSQIEventId, 0);
}

// btnDebug6_MaintenanceScreen - PressedEvent
void btnDebug6_MaintenanceScreen_PressedEvent(laButtonWidget* btn)
{
    alarmStatus = eActive;
    alarmID = eBreathingCircuitChangedAlarmId;
    alarmData[0] = 0; //additional mess
    alarmData[1] = 1; // condition mess 
    alarmData[2] = 0; //reset button
    
    SYS_PRINT("alarm_id %d alarm_data_0 %d alarm_data_1 %d alarm_data_2 %d  \n", alarmID, alarmData[0], alarmData[1], alarmData[2]);
    guiInterface_SendEventAlarm(alarmID, alarmStatus, alarmPriority, alarmData);
}

// btnDebug7_MaintenanceScreen - PressedEvent
void btnDebug7_MaintenanceScreen_PressedEvent(laButtonWidget* btn)
{
    if (FileSystemMgr_IsUSBMounted() == true) {
        logMgr_BackupToUSB();
    }
    else
    {
        SYS_PRINT("USB not found \n");
    }

}

// btnDebug8_MaintenanceScreen - PressedEvent
void btnDebug8_MaintenanceScreen_PressedEvent(laButtonWidget* btn)
{
    if (FileSystemMgr_IsUSBMounted() == true) {
        logMgr_RestoreFromUSB();
    }
    else
    {
        SYS_PRINT("USB not found \n");
    }
} 


// PowerOffScreen - ShowEvent
void PowerOffScreen_ShowEvent(laScreen* scr)
{
    static bool s_isInitialized = false;
    if (s_isInitialized)
        PowerOffScreen_Initialize();
    else {
        PowerOffScreen_Initialize();
        s_isInitialized = true;
    }
}

// PowerOffScreen - HideEvent
void PowerOffScreen_HideEvent(laScreen* scr)
{
    PowerOffScreen_Distructor();
}





