/*******************************************************************************
  MPLAB Harmony Graphics Composer Generated Definitions Header

  File Name:
    libaria_macros.h

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

#ifndef _LIBARIA_INIT_H
#define _LIBARIA_INIT_H

#ifndef NATIVE
#include "system_config.h"
#include "system_definitions.h"
#endif

#include "gfx/libaria/libaria.h"
#include "gfx/libaria/libaria_events.h"

#include "gfx/gfx_assets.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

#define LIBARIA_SCREEN_COUNT   6

// reference IDs for generated libaria screens
// screen "MainScreen"
#define MainScreen_ID    2

// screen "MaintenanceScreen"
#define MaintenanceScreen_ID    4

// screen "PowerOffScreen"
#define PowerOffScreen_ID    5

// screen "SettingScreen"
#define SettingScreen_ID    3

// screen "UpdateScreen"
#define UpdateScreen_ID    0

// screen "VideoScreen"
#define VideoScreen_ID    1



extern laScheme JFLO_InativeItem_SettingScreen_Scheme;
extern laScheme JFLO_Line_Scheme;
extern laScheme JFLO_ShowingWindow_Scheme;
extern laScheme ClearScheme;
extern laScheme JFLO_ReverseItem_SettingScreen_Scheme;
extern laScheme JFLO_SpO2Graph_Scheme;
extern laScheme JFLO_SelectedItem_SettingScreen_Scheme;
extern laScheme JFLO_AlarmIndicatorMediumPriority_Scheme;
extern laScheme JFLO_MessageBox_Scheme;
extern laScheme JFLO_AlarmIndicatorLow1Priority_Scheme;
extern laScheme JFLO_AlarmIndicatorHighPriority_Scheme;
extern laScheme JFLO_AlarmArea_Scheme;
extern laScheme JFLO_CyanIndicator_Scheme;
extern laScheme JFLO_CellTable_Scheme;
extern laScheme JFLO_Slider_Scheme;
extern laScheme JFLO_AlarmInfo_Scheme;
extern laScheme JFLO_NoticeRunningArea_Scheme;
extern laScheme JFLO_Button_Scheme;
extern laScheme JFLO_Button_OnChangedSetpointArea_Scheme;
extern laScheme JFLO_Background_Scheme;
extern laScheme JFLO_Underbar_Scheme;
extern laScheme JFLO_RectChangedSetpoint_2_Scheme;
extern laScheme BlackBackgroundScheme;
extern laScheme ButtonTextScheme;
extern laScheme JFLO_SettingForm_Scheme;
extern laScheme JFLO_Maintainance_Scheme;
extern laScheme JFLO_MonitoringArea_Scheme;
extern laScheme JFLO_WarmingUp_Scheme;
extern laScheme JFLO_RedIndicator_Scheme;
extern laScheme JFLO_AlarmArea_ButtonScheme;
extern laScheme JFLO_YellowIndicator_Scheme;
extern laScheme JFLO_SpO2Data_Scheme;
extern laScheme JFLO_AlarmIndicatorLow2Priority_Scheme;
extern laLabelWidget* lbTitle_UpdateScreen;
extern laButtonWidget* btnMainboard_UpdateScreen;
extern laButtonWidget* btnCancel_UpdateScreen;
extern laButtonWidget* btnCradle_UpdateScreen;
extern laButtonWidget* btnChamber_UpdateScreen;
extern laWidget* panelMessageBox_UpdateScreen;
extern laWidget* panelMessageBoxBackground_UpdateScreen;
extern laLabelWidget* lbMessage_UpdateScreen;
extern laButtonWidget* btnOK_UpdateScreen;
extern laProgressBarWidget* prgBarLoading;
extern laTextFieldWidget* txtInfo_Videoscreen;
extern laWidget* panelSelfCheckIndicatorTop;
extern laWidget* panelSelfCheckIndicatorRight;
extern laWidget* panelSelfCheckIndicatorBottom;
extern laWidget* panelSelfCheckIndicatorLeft;
extern laImageWidget* imgWifi;
extern laTextFieldWidget* tfCurrentTime;
extern laButtonWidget* btnInfo;
extern laImageWidget* imgEXTBattery;
extern laImageWidget* imgINTBattery;
extern laImageWidget* imgSocket;
extern laWidget* panelBehindGraph;
extern laLineGraphWidget* graphHeartBeat;
extern laLabelWidget* lbSpO2text;
extern laTextFieldWidget* tfSpO2Value;
extern laLabelWidget* lbSpO2Unit;
extern laLabelWidget* lbPRtext;
extern laTextFieldWidget* tfPrValue;
extern laLabelWidget* lbPrUnit;
extern jfloRectangleWidget* rectChangedSetpointArea;
extern jfloRectangleWidget* rectChangedSetpointArea_2;
extern laTextFieldWidget* tfCurrentTempSetting;
extern laTextFieldWidget* tfCurrentFlowSetting;
extern laTextFieldWidget* tfCurrentO2Setting;
extern laImageWidget* imgIcon;
extern laLabelWidget* lbO2Up_ChangedSetpointArea;
extern laLabelWidget* lbO2Down_ChangedSetpointArea;
extern laTextFieldWidget* tfValue;
extern laLabelWidget* lbO2_ChangedSetpointArea;
extern jfloButtonWidget* btnMinus;
extern jfloButtonWidget* btnBack;
extern jfloButtonWidget* btnPlus;
extern jfloButtonWidget* btnOK;
extern laImageWidget* iconBidirectionArrow;
extern laImageWidget* imgBlueline;
extern jfloRectangleWidget* rectMonitoringArea;
extern laLabelWidget* lbTempUnit;
extern laLabelWidget* lbFlowUnit;
extern laLabelWidget* lbO2Unit;
extern laTextFieldWidget* tfCurrentTempValue;
extern laTextFieldWidget* tfCurrentFlowValue;
extern laTextFieldWidget* tfCurrentO2Value;
extern jfloButtonWidget* btnTemp_SelectToSetting_1;
extern laArcWidget* arcTemp_SelectToSetting_1;
extern jfloButtonWidget* btnFlow_SelectToSetting_1;
extern laArcWidget* arcFlow_SelectToSetting_1;
extern jfloButtonWidget* btnO2_SelectToSetting_1;
extern laArcWidget* arcO2_SelectToSetting_1;
extern laWidget* panelNotice;
extern laLabelWidget* lbInfo_NoticeArea;
extern jfloRectangleWidget* rectAlarmArea;
extern laLabelWidget* lbAlarmInfomation;
extern laButtonWidget* btnX_AlarmArea;
extern laButtonWidget* btnAlarmReset_AlarmArea;
extern jfloButtonWidget* btnTemp_SelectToSetting_2;
extern jfloButtonWidget* btnFlow_SelectToSetting_2;
extern jfloButtonWidget* btnO2_SelectToSetting_2;
extern laWidget* panelLeftTouch;
extern laWidget* panelRightTouch;
extern laWidget* panelLeftAlarm;
extern laWidget* panelRightAlarm;
extern laWidget* panelBottomAlarm;
extern laWidget* panelAlarmTitle;
extern laLabelWidget* labelAlarmTitle;
extern laImageWidget* imageMuteIcon;
extern laImageWidget* iconAlarmActive;
extern laLabelWidget* labelSpO2Dash;
extern laLabelWidget* labelPRDash;
extern laWidget* SC_RunningTimePanel;
extern laLabelWidget* SC_RunningTimeLabel;
extern laImageWidget* SC_WifiIcon;
extern laTextFieldWidget* SC_DateTimeTextField;
extern laButtonWidget* SC_AlarmInfoButton;
extern laImageWidget* SC_ExternalBatteryIcon;
extern laImageWidget* SC_InternalBatteryIcon;
extern laImageWidget* SC_PowerACIcon;
extern laButtonWidget* SC_HomeButton;
extern laLabelWidget* SC_ScreenTitleLabel;
extern laWidget* SC_MenuSettingPanel;
extern laWidget* SC_LanguageMenuPanel;
extern laImageWidget* SC_LanguageMenuIcon;
extern laLabelWidget* SC_LanguageMenuLabel;
extern laWidget* SC_DateTimeMenuPanel;
extern laImageWidget* SC_DateTimeMenuIcon;
extern laLabelWidget* SC_DateTimeMenuLabel;
extern laWidget* SC_WifiMenuPanel;
extern laImageWidget* SC_WifiMenuIcon;
extern laLabelWidget* SC_WifiMenuLabel;
extern laWidget* SC_BrightnessMenuPanel;
extern laImageWidget* SC_BrightnessMenuIcon;
extern laLabelWidget* SC_BrightnessMenuLabel;
extern laWidget* SC_OxygenSourceMenuPanel;
extern laImageWidget* SC_OxygenSourceMenuIcon;
extern laLabelWidget* SC_OxygenSourceMenuLabel;
extern laWidget* SC_SpO2AlarmLimitMenuPanel;
extern laImageWidget* SC_SpO2AlarmLimitMenuIcon;
extern laLabelWidget* SC_SpO2AlarmLimitMenuLabel;
extern laWidget* SC_OxygenConcentrationAlarmLimitMenuPanel;
extern laImageWidget* SC_OxygenConcentrationAlarmLimitMenuIcon;
extern laLabelWidget* SC_OxygenConcentrationAlarmLimitMenuLabel;
extern laWidget* SC_AlarmSoundLevelMenuPanel;
extern laImageWidget* SC_AlarmSoundLevelMenuIcon;
extern laLabelWidget* SC_AlarmSoundLevelMenuLabel;
extern laWidget* SC_TabButtonPanel;
extern laButtonWidget* SC_SettingButton;
extern laButtonWidget* SC_DataLogButton;
extern laButtonWidget* SC_MaintenanceButton;
extern laButtonWidget* SC_DeviceInformationButton;
extern laWidget* SC_TabButtonUnderBarPanel;
extern laWidget* SC_AlarmTitleBarPanel;
extern laLabelWidget* SC_AlarmTitleLabel;
extern laImageWidget* SC_AlarmMuteIcon;
extern laImageWidget* SC_AlarmActiveIcon;
extern laWidget* SC_AlarmIndicatorLeftPanel;
extern laWidget* SC_AlarmIndicatorRightPanel;
extern laWidget* SC_AlarmIndicatorBottomPanel;
extern laWidget* SC_DataLogSettingPanel;
extern laLabelWidget* SC_DataLogSettingAlarmLabel;
extern laLabelWidget* SC_DataLogSettingEventLabel;
extern laWidget* SC_DataLogSettingUnderbarPanel;
extern laLabelWidget* SC_DataLogSettingTimeLabel;
extern laLabelWidget* SC_DataLogSettingTitleLabel;
extern laLabelWidget* SC_DataLogSettingStateLabel;
extern laLabelWidget* SC_DataLogSettingTimeLabel_1;
extern laLabelWidget* SC_DataLogSettingTimeLabel_2;
extern laLabelWidget* SC_DataLogSettingTimeLabel_3;
extern laLabelWidget* SC_DataLogSettingTimeLabel_4;
extern laLabelWidget* SC_DataLogSettingTimeLabel_5;
extern laLabelWidget* SC_DataLogSettingTitleLabel_1;
extern laLabelWidget* SC_DataLogSettingTitleLabel_2;
extern laLabelWidget* SC_DataLogSettingTitleLabel_3;
extern laLabelWidget* SC_DataLogSettingTitleLabel_4;
extern laLabelWidget* SC_DataLogSettingTitleLabel_5;
extern laLabelWidget* SC_DataLogSettingStateLabel_1;
extern laLabelWidget* SC_DataLogSettingStateLabel_2;
extern laLabelWidget* SC_DataLogSettingStateLabel_3;
extern laLabelWidget* SC_DataLogSettingStateLabel_4;
extern laLabelWidget* SC_DataLogSettingStateLabel_5;
extern laLabelWidget* SC_DataLogSettingPageNumberLabel;
extern laButtonWidget* SC_DataLogSettingPrevButton;
extern laButtonWidget* SC_DataLogSettingNextButton;
extern laWidget* SC_DataLogSettingIndicator_1;
extern laWidget* SC_DataLogSettingIndicator_2;
extern laWidget* SC_DataLogSettingIndicator_3;
extern laWidget* SC_DataLogSettingIndicator_4;
extern laWidget* SC_DataLogSettingIndicator_5;
extern laWidget* SC_MaintenancePanel;
extern laWidget* SC_DeviceInformationPanel;
extern laLabelWidget* SC_DeviceInformationMainLabel;
extern laLabelWidget* SC_DeviceInformationChamberLabel;
extern laLabelWidget* SC_DeviceInformationCradleLabel;
extern laLabelWidget* SC_DeviceInformationCircuitLabel;
extern laWidget* SC_DeviceInformationUnderbarPanel;
extern laLabelWidget* SC_DeviceInformationLabel_1;
extern laLabelWidget* SC_DeviceInformationLabel_2;
extern laLabelWidget* SC_DeviceInformationLabel_3;
extern laLabelWidget* SC_DeviceInformationLabel_4;
extern laLabelWidget* SC_DeviceInformationLabel_5;
extern laLabelWidget* SC_DeviceInformationLabel_6;
extern laWidget* SC_MenuSetting_SettingPanel;
extern laLabelWidget* SC_MenuSetting_SettingTitleLabel;
extern laLabelWidget* SC_MenuSetting_SettingMessageLabel;
extern laButtonWidget* SC_MenuSetting_SettingBackButton;
extern laButtonWidget* SC_MenuSetting_SettingSaveButton;
extern laWidget* SC_MenuSetting_SettingLanguagePanel;
extern laButtonWidget* SC_MenuSetting_SettingLanguage_NextButton;
extern laButtonWidget* SC_MenuSetting_SettingLanguage_PrevButton;
extern laLabelWidget* SC_MenuSetting_SettingLanguage_SettingLabel;
extern laWidget* SC_MenuSetting_SettingDateTimePanel;
extern laLabelWidget* SC_MenuSetting_SettingDateTime_YearLabel;
extern laLabelWidget* SC_MenuSetting_SettingDateTime_Seperator_1;
extern laLabelWidget* SC_MenuSetting_SettingDateTime_MonLabel;
extern laLabelWidget* SC_MenuSetting_SettingDateTime_Seperator_2;
extern laLabelWidget* SC_MenuSetting_SettingDateTime_DateLabel;
extern laLabelWidget* SC_MenuSetting_SettingDateTime_HourLabel;
extern laLabelWidget* SC_MenuSetting_SettingDateTime_SepLabel_3;
extern laLabelWidget* SC_MenuSetting_SettingDateTime_MinLabel;
extern laLabelWidget* SC_MenuSetting_SettingDateTime_Seperator_4;
extern laLabelWidget* SC_MenuSetting_SettingDateTime_SecLabel;
extern laButtonWidget* SC_MenuSetting_SettingDateTime_DecButton;
extern laButtonWidget* SC_MenuSetting_SettingDateTime_IncButton;
extern laWidget* SC_MenuSetting_SettingDateTime_UnderBarPanel;
extern laWidget* SC_MenuSetting_SettingWifiPanel;
extern laLabelWidget* SC_MenuSetting_SettingWifi_OnLabel;
extern laLabelWidget* SC_MenuSetting_SettingWifi_OffLabel;
extern laWidget* SC_MenuSetting_SettingWifi_UnderBarPanel;
extern laWidget* SC_MenuSetting_SettingBrightnessPanel;
extern laButtonWidget* SC_MenuSetting_SettingBrightness_NextButton;
extern laButtonWidget* SC_MenuSetting_SettingBrightness_PrevButton;
extern laLabelWidget* SC_MenuSetting_SettingBrightness_ValueLabel;
extern laLabelWidget* SC_MenuSetting_SettingBrightness_OffLabel;
extern laLabelWidget* SC_MenuSetting_SettingBrightness_OnLabel;
extern laWidget* SC_MenuSetting_SettingBrightness_UnderBar;
extern laLabelWidget* SC_MenuSetting_SettingBrightness_MessageLabel;
extern laWidget* SC_MenuSetting_SettingOxySourcePanel;
extern laLabelWidget* SC_MenuSetting_SettingOxySource_90Label;
extern laLabelWidget* SC_MenuSetting_SettingOxySource_100Label;
extern laWidget* SC_MenuSetting_SettingOxySource_UnderBarPanel;
extern laWidget* SC_MenuSetting_SettingSpo2AlarmPanel;
extern laButtonWidget* SC_MenuSetting_SettingSpo2Alarm_NextButton;
extern laButtonWidget* SC_MenuSetting_SettingSpo2Alarm_PrevButton;
extern laLabelWidget* SC_MenuSetting_SettingSpo2Alarm_ValueLabel;
extern laWidget* SC_MenuSetting_SettingOxyAlarmPanel;
extern laButtonWidget* SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton;
extern laButtonWidget* SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton;
extern laButtonWidget* SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton;
extern laButtonWidget* SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton;
extern laLabelWidget* SC_MenuSetting_SettingOxyAlarm_UpLimitLabel;
extern laLabelWidget* SC_MenuSetting_SettingOxyAlarm_LoLimitLabel;
extern laWidget* SC_MenuSetting_SettingAlarmSoundLevelPanel;
extern laButtonWidget* SC_MenuSetting_SettingAlarmSoundLevel_NextButton;
extern laButtonWidget* SC_MenuSetting_SettingAlarmSoundLevel_PrevButton;
extern laLabelWidget* SC_MenuSetting_SettingAlarmSoundLevel_ValueLabel;
extern laWidget* SC_SaveConfirmPanel;
extern laImageWidget* SC_SaveConfirmBackgroundImage;
extern laButtonWidget* SC_SaveConfirmYesButton;
extern laButtonWidget* SC_SaveConfirmNoButton;
extern laLabelWidget* SC_SaveConfirmTitleLabel;
extern laLabelWidget* lbTitleAirflow;
extern laLabelWidget* lbTitleO2flow;
extern laLabelWidget* lbTitleTemp;
extern laLabelWidget* s_titleMotor;
extern laLabelWidget* lbTitleO2;
extern laLabelWidget* lbTitleIH;
extern laButtonWidget* btnHome_MaintenanceScreen;
extern laTextFieldWidget* tfAirFlowMonitor;
extern laTextFieldWidget* tfO2FlowMonitor;
extern laTextFieldWidget* tfTempMonitor;
extern laTextFieldWidget* tfO2Monitor;
extern laLabelWidget* lbTitleMotor;
extern laSliderWidget* scrollbarMotor;
extern laButtonWidget* btnOnOffMotor;
extern laSliderWidget* scrollbarIH;
extern laButtonWidget* btnOnOffIH;
extern laLabelWidget* lbTitleWaterPump;
extern laButtonWidget* btnOnOffWaterPump;
extern laButtonWidget* btnUpgrade;
extern laLabelWidget* lbTitleVersion;
extern laButtonWidget* btnLogtoUsb;
extern laButtonWidget* btnClearLog;
extern laButtonWidget* btnTestHighAlarm_MaintenanceScreen;
extern laButtonWidget* btnTestMediumAlarm_MaintenanceScreen;
extern laButtonWidget* btnTestLowAlarm_MaintenanceScreen;
extern laButtonWidget* btnDebug4_MaintenanceScreen;
extern laButtonWidget* btnDebug5_MaintenanceScreen;
extern laButtonWidget* btnDebug6_MaintenanceScreen;
extern laButtonWidget* btnDebug7_MaintenanceScreen;
extern laButtonWidget* btnDebug8_MaintenanceScreen;
extern laLabelWidget* labelPowerOffMessage;
extern laLabelWidget* labelPowerOffAdditionalMessage;
extern laWidget* panelPowerOffAlarmIndicatorTop;
extern laWidget* panelPowerOffAlarmIndicatorBottom;
extern laWidget* panelPowerOffAlarmIndicatorLeft;
extern laWidget* panelPowerOffAlarmIndicatorRight;


int32_t libaria_initialize(void);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // _LIBARIA_INIT_H
/*******************************************************************************
 End of File
*/
