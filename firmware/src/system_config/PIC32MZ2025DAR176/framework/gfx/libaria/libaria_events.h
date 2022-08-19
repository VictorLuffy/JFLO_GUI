/*******************************************************************************
  MPLAB Harmony Graphics Composer Generated Definitions Header

  File Name:
    libaria_events.h

  Summary:
    Build-time generated definitions header based on output by the MPLAB Harmony
    Graphics Composer.

  Description:
    Build-time generated definitions header based on output by the MPLAB Harmony
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

#ifndef _LIBARIA_EVENTS_H
#define _LIBARIA_EVENTS_H

#include "gfx/libaria/libaria.h"
#include "gfx/libaria/libaria_init.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// Generated Event Handler - Origin: UpdateScreen, Event: ShowEvent
void UpdateScreen_ShowEvent(laScreen* scr);

// Generated Event Handler - Origin: UpdateScreen, Event: HideEvent
void UpdateScreen_HideEvent(laScreen* scr);

// Generated Event Handler - Origin: btnMainboard_UpdateScreen, Event: PressedEvent
void btnMainboard_UpdateScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnCancel_UpdateScreen, Event: PressedEvent
void btnCancel_UpdateScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnCradle_UpdateScreen, Event: PressedEvent
void btnCradle_UpdateScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnChamber_UpdateScreen, Event: PressedEvent
void btnChamber_UpdateScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnOK_UpdateScreen, Event: PressedEvent
void btnOK_UpdateScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: VideoScreen, Event: ShowEvent
void VideoScreen_ShowEvent(laScreen* scr);

// Generated Event Handler - Origin: VideoScreen, Event: HideEvent
void VideoScreen_HideEvent(laScreen* scr);

// Generated Event Handler - Origin: MainScreen, Event: ShowEvent
void MainScreen_ShowEvent(laScreen* scr);

// Generated Event Handler - Origin: MainScreen, Event: HideEvent
void MainScreen_HideEvent(laScreen* scr);

// Generated Event Handler - Origin: btnInfo, Event: PressedEvent
void btnInfo_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnMinus, Event: PressedEvent
void btnMinus_PressedEvent(jfloButtonWidget* btn);

// Generated Event Handler - Origin: btnBack, Event: PressedEvent
void btnBack_PressedEvent(jfloButtonWidget* btn);

// Generated Event Handler - Origin: btnPlus, Event: PressedEvent
void btnPlus_PressedEvent(jfloButtonWidget* btn);

// Generated Event Handler - Origin: btnOK, Event: PressedEvent
void btnOK_PressedEvent(jfloButtonWidget* btn);

// Generated Event Handler - Origin: btnX_AlarmArea, Event: PressedEvent
void btnX_AlarmArea_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnAlarmReset_AlarmArea, Event: PressedEvent
void btnAlarmReset_AlarmArea_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: panelAlarmTitle, Event: PressedEvent
void panelAlarmTitle_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt);

// Generated Event Handler - Origin: btnTemp_SelectToSetting_1, Event: PressedEvent
void btnTemp_SelectToSetting_1_PressedEvent(jfloButtonWidget* btn);

// Generated Event Handler - Origin: btnTemp_SelectToSetting_1, Event: ReleasedEvent
void btnTemp_SelectToSetting_1_ReleasedEvent(jfloButtonWidget* btn);

// Generated Event Handler - Origin: btnFlow_SelectToSetting_1, Event: PressedEvent
void btnFlow_SelectToSetting_1_PressedEvent(jfloButtonWidget* btn);

// Generated Event Handler - Origin: btnFlow_SelectToSetting_1, Event: ReleasedEvent
void btnFlow_SelectToSetting_1_ReleasedEvent(jfloButtonWidget* btn);

// Generated Event Handler - Origin: btnO2_SelectToSetting_1, Event: PressedEvent
void btnO2_SelectToSetting_1_PressedEvent(jfloButtonWidget* btn);

// Generated Event Handler - Origin: btnO2_SelectToSetting_1, Event: ReleasedEvent
void btnO2_SelectToSetting_1_ReleasedEvent(jfloButtonWidget* btn);


// Generated Event Handler - Origin: btnTemp_SelectToSetting_2, Event: PressedEvent
void btnTemp_SelectToSetting_2_PressedEvent(jfloButtonWidget* btn);

// Generated Event Handler - Origin: btnFlow_SelectToSetting_2, Event: PressedEvent
void btnFlow_SelectToSetting_2_PressedEvent(jfloButtonWidget* btn);

// Generated Event Handler - Origin: btnO2_SelectToSetting_2, Event: PressedEvent
void btnO2_SelectToSetting_2_PressedEvent(jfloButtonWidget* btn);

// Generated Event Handler - Origin: SettingScreen, Event: ShowEvent
void SettingScreen_ShowEvent(laScreen* scr);

// Generated Event Handler - Origin: SettingScreen, Event: HideEvent
void SettingScreen_HideEvent(laScreen* scr);

// Generated Event Handler - Origin: SC_AlarmInfoButton, Event: PressedEvent
void SC_AlarmInfoButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnAlarmOpt, Event: PressedEvent
void pnlAlarmOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt);

// Generated Event Handler - Origin: btnUpdate_AlarmTable_SettingScreen, Event: PressedEvent
void btnUpdate_AlarmTable_SettingScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnUpdate_AlarmTable_SettingScreen, Event: ReleasedEvent
void btnUpdate_AlarmTable_SettingScreen_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnPrev_AlarmTable_SettingScreen, Event: PressedEvent
void btnPrev_AlarmTable_SettingScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnNext_AlarmTable_SettingScreen, Event: PressedEvent
void btnNext_AlarmTable_SettingScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnLanguage, Event: PressedEvent
void pnlLanguageOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt);

// Generated Event Handler - Origin: btnlLanguageSw, Event: PressedEvent
void btnlLanguageSw_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnMode, Event: PressedEvent
void pnlModeOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt);

// Generated Event Handler - Origin: btnModeSw, Event: PressedEvent
void btnModeSw_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnBackgroundOpt, Event: PressedEvent
void pnlBackgroundOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt);

// Generated Event Handler - Origin: btnBackgroundSw, Event: PressedEvent
void btnBackgroundSw_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnBluetoothOpt, Event: PressedEvent
void pnlBluetoothOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt);

// Generated Event Handler - Origin: btnBluettoothSw, Event: PressedEvent
void btnBluettoothSw_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnBrightness, Event: PressedEvent
void pnlBrightnessOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt);

// Generated Event Handler - Origin: sldBrightnessChg, Event: ValueChangedEvent
void sldBrightnessChg_ValueChangedEvent(laSliderWidget* sld);

// Generated Event Handler - Origin: btnBrightnessModeSw, Event: PressedEvent
void btnBrightnessModeSw_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnOkDateTimeSetting, Event: PressedEvent
void btnOkDateTimeSetting_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnLogOpt, Event: PressedEvent
void pnlLogOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt);

// Generated Event Handler - Origin: btnUpdate_LogTable_SettingScreen, Event: PressedEvent
void btnUpdate_LogTable_SettingScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnUpdate_LogTable_SettingScreen, Event: ReleasedEvent
void btnUpdate_LogTable_SettingScreen_ReleasedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnPrev_LogTable_SettingScreen, Event: PressedEvent
void btnPrev_LogTable_SettingScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnNext_LogTable_SettingScreen, Event: PressedEvent
void btnNext_LogTable_SettingScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnDateTimeOpt, Event: PressedEvent
void pnlDateTimeOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt);

// Generated Event Handler - Origin: btnDecDateTimeSetting, Event: PressedEvent
void btnDecDateTimeSetting_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnIncDateTimeSetting, Event: PressedEvent
void btnIncDateTimeSetting_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnCancelDateTimeSetting, Event: PressedEvent
void btnCancelDateTimeSetting_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnOkDateTimeSetting, Event: PressedEvent
void btnOkDateTimeSetting_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnWifiOpt, Event: PressedEvent
void pnlWifiOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt);

// Generated Event Handler - Origin: btnWifiSw, Event: PressedEvent
void btnWifiSw_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnSpeaker, Event: PressedEvent
void pnlSpeakerOpt_PressedEvent(laWidget* widget, laInput_TouchDownEvent* evt);

// Generated Event Handler - Origin: sldSpeakerChg, Event: ValueChangedEvent
void sldSpeakerChg_ValueChangedEvent(laSliderWidget* sld);

// Generated Event Handler - Origin: btnOptStatus, Event: PressedEvent
void btnOptStatus_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_HomeButton, Event: PressedEvent
void SC_HomeButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_SettingButton, Event: PressedEvent
void SC_SettingButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_DataLogButton, Event: PressedEvent
void SC_DataLogButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MaintenanceButton, Event: PressedEvent
void SC_MaintenanceButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_DeviceInformationButton, Event: PressedEvent
void SC_DeviceInformationButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_DataLogSettingPrevButton, Event: PressedEvent
void SC_DataLogSettingPrevButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_DataLogSettingNextButton, Event: PressedEvent
void SC_DataLogSettingNextButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingBackButton, Event: PressedEvent
void SC_MenuSetting_SettingBackButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingSaveButton, Event: PressedEvent
void SC_MenuSetting_SettingSaveButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingLanguage_NextButton, Event: PressedEvent
void SC_MenuSetting_SettingLanguage_NextButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingLanguage_PrevButton, Event: PressedEvent
void SC_MenuSetting_SettingLanguage_PrevButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingDateTime_DecButton, Event: PressedEvent
void SC_MenuSetting_SettingDateTime_DecButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingDateTime_IncButton, Event: PressedEvent
void SC_MenuSetting_SettingDateTime_IncButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingBrightness_NextButton, Event: PressedEvent
void SC_MenuSetting_SettingBrightness_NextButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingBrightness_PrevButton, Event: PressedEvent
void SC_MenuSetting_SettingBrightness_PrevButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingSpo2Alarm_NextButton, Event: PressedEvent
void SC_MenuSetting_SettingSpo2Alarm_NextButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingSpo2Alarm_PrevButton, Event: PressedEvent
void SC_MenuSetting_SettingSpo2Alarm_PrevButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton, Event: PressedEvent
void SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton, Event: PressedEvent
void SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton, Event: PressedEvent
void SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton, Event: PressedEvent
void SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingAlarmSoundLevel_NextButton, Event: PressedEvent
void SC_MenuSetting_SettingAlarmSoundLevel_NextButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_MenuSetting_SettingAlarmSoundLevel_PrevButton, Event: PressedEvent
void SC_MenuSetting_SettingAlarmSoundLevel_PrevButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_SaveConfirmYesButton, Event: PressedEvent
void SC_SaveConfirmYesButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: SC_SaveConfirmNoButton, Event: PressedEvent
void SC_SaveConfirmNoButton_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: MaintenanceScreen, Event: ShowEvent
void MaintenanceScreen_ShowEvent(laScreen* scr);

// Generated Event Handler - Origin: MaintenanceScreen, Event: HideEvent
void MaintenanceScreen_HideEvent(laScreen* scr);

// Generated Event Handler - Origin: btnHome_MaintenanceScreen, Event: PressedEvent
void btnHome_MaintenanceScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: scrollbarMotor, Event: ValueChangedEvent
void scrollbarMotor_ValueChangedEvent(laSliderWidget* sld);

// Generated Event Handler - Origin: btnOnOffMotor, Event: PressedEvent
void btnOnOffMotor_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: scrollbarIH, Event: ValueChangedEvent
void scrollbarIH_ValueChangedEvent(laSliderWidget* sld);

// Generated Event Handler - Origin: btnOnOffIH, Event: PressedEvent
void btnOnOffIH_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnOnOffWaterPump, Event: PressedEvent
void btnOnOffWaterPump_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnUpgrade, Event: PressedEvent
void btnUpgrade_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnLogtoUsb, Event: PressedEvent
void btnLogtoUsb_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnClearLog, Event: PressedEvent
void btnClearLog_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnTestHighAlarm_MaintenanceScreen, Event: PressedEvent
void btnTestHighAlarm_MaintenanceScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnTestMediumAlarm_MaintenanceScreen, Event: PressedEvent
void btnTestMediumAlarm_MaintenanceScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnTestLowAlarm_MaintenanceScreen, Event: PressedEvent
void btnTestLowAlarm_MaintenanceScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnDebug4_MaintenanceScreen, Event: PressedEvent
void btnDebug4_MaintenanceScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnDebug5_MaintenanceScreen, Event: PressedEvent
void btnDebug5_MaintenanceScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnDebug6_MaintenanceScreen, Event: PressedEvent
void btnDebug6_MaintenanceScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnDebug7_MaintenanceScreen, Event: PressedEvent
void btnDebug7_MaintenanceScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: btnDebug8_MaintenanceScreen, Event: PressedEvent
void btnDebug8_MaintenanceScreen_PressedEvent(laButtonWidget* btn);

// Generated Event Handler - Origin: PowerOffScreen, Event: ShowEvent
void PowerOffScreen_ShowEvent(laScreen* scr);

// Generated Event Handler - Origin: PowerOffScreen, Event: HideEvent
void PowerOffScreen_HideEvent(laScreen* scr);



//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // _LIBARIA_EVENTS_H
/*******************************************************************************
 End of File
*/
