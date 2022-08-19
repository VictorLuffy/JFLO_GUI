/*******************************************************************************
  MPLAB Harmony Graphics Composer Generated Implementation File

  File Name:
    libaria_init.c

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

#include "gfx/libaria/libaria_init.h"

laScheme JFLO_InativeItem_SettingScreen_Scheme;
laScheme JFLO_Line_Scheme;
laScheme JFLO_ShowingWindow_Scheme;
laScheme ClearScheme;
laScheme JFLO_ReverseItem_SettingScreen_Scheme;
laScheme JFLO_SpO2Graph_Scheme;
laScheme JFLO_SelectedItem_SettingScreen_Scheme;
laScheme JFLO_AlarmIndicatorMediumPriority_Scheme;
laScheme JFLO_MessageBox_Scheme;
laScheme JFLO_AlarmIndicatorLow1Priority_Scheme;
laScheme JFLO_AlarmIndicatorHighPriority_Scheme;
laScheme JFLO_AlarmArea_Scheme;
laScheme JFLO_CyanIndicator_Scheme;
laScheme JFLO_CellTable_Scheme;
laScheme JFLO_Slider_Scheme;
laScheme JFLO_AlarmInfo_Scheme;
laScheme JFLO_NoticeRunningArea_Scheme;
laScheme JFLO_Button_Scheme;
laScheme JFLO_Button_OnChangedSetpointArea_Scheme;
laScheme JFLO_Background_Scheme;
laScheme JFLO_Underbar_Scheme;
laScheme JFLO_RectChangedSetpoint_2_Scheme;
laScheme BlackBackgroundScheme;
laScheme ButtonTextScheme;
laScheme JFLO_SettingForm_Scheme;
laScheme JFLO_Maintainance_Scheme;
laScheme JFLO_MonitoringArea_Scheme;
laScheme JFLO_WarmingUp_Scheme;
laScheme JFLO_RedIndicator_Scheme;
laScheme JFLO_AlarmArea_ButtonScheme;
laScheme JFLO_YellowIndicator_Scheme;
laScheme JFLO_SpO2Data_Scheme;
laScheme JFLO_AlarmIndicatorLow2Priority_Scheme;
laLabelWidget* lbTitle_UpdateScreen;
laButtonWidget* btnMainboard_UpdateScreen;
laButtonWidget* btnCancel_UpdateScreen;
laButtonWidget* btnCradle_UpdateScreen;
laButtonWidget* btnChamber_UpdateScreen;
laWidget* panelMessageBox_UpdateScreen;
laWidget* panelMessageBoxBackground_UpdateScreen;
laLabelWidget* lbMessage_UpdateScreen;
laButtonWidget* btnOK_UpdateScreen;
laProgressBarWidget* prgBarLoading;
laTextFieldWidget* txtInfo_Videoscreen;
laWidget* panelSelfCheckIndicatorTop;
laWidget* panelSelfCheckIndicatorRight;
laWidget* panelSelfCheckIndicatorBottom;
laWidget* panelSelfCheckIndicatorLeft;
laImageWidget* imgWifi;
laTextFieldWidget* tfCurrentTime;
laButtonWidget* btnInfo;
laImageWidget* imgEXTBattery;
laImageWidget* imgINTBattery;
laImageWidget* imgSocket;
laWidget* panelBehindGraph;
laLineGraphWidget* graphHeartBeat;
laLabelWidget* lbSpO2text;
laTextFieldWidget* tfSpO2Value;
laLabelWidget* lbSpO2Unit;
laLabelWidget* lbPRtext;
laTextFieldWidget* tfPrValue;
laLabelWidget* lbPrUnit;
jfloRectangleWidget* rectChangedSetpointArea;
jfloRectangleWidget* rectChangedSetpointArea_2;
laTextFieldWidget* tfCurrentTempSetting;
laTextFieldWidget* tfCurrentFlowSetting;
laTextFieldWidget* tfCurrentO2Setting;
laImageWidget* imgIcon;
laLabelWidget* lbO2Up_ChangedSetpointArea;
laLabelWidget* lbO2Down_ChangedSetpointArea;
laTextFieldWidget* tfValue;
laLabelWidget* lbO2_ChangedSetpointArea;
jfloButtonWidget* btnMinus;
jfloButtonWidget* btnBack;
jfloButtonWidget* btnPlus;
jfloButtonWidget* btnOK;
laImageWidget* iconBidirectionArrow;
laImageWidget* imgBlueline;
jfloRectangleWidget* rectMonitoringArea;
laLabelWidget* lbTempUnit;
laLabelWidget* lbFlowUnit;
laLabelWidget* lbO2Unit;
laTextFieldWidget* tfCurrentTempValue;
laTextFieldWidget* tfCurrentFlowValue;
laTextFieldWidget* tfCurrentO2Value;
jfloButtonWidget* btnTemp_SelectToSetting_1;
laArcWidget* arcTemp_SelectToSetting_1;
jfloButtonWidget* btnFlow_SelectToSetting_1;
laArcWidget* arcFlow_SelectToSetting_1;
jfloButtonWidget* btnO2_SelectToSetting_1;
laArcWidget* arcO2_SelectToSetting_1;
laWidget* panelNotice;
laLabelWidget* lbInfo_NoticeArea;
jfloRectangleWidget* rectAlarmArea;
laLabelWidget* lbAlarmInfomation;
laButtonWidget* btnX_AlarmArea;
laButtonWidget* btnAlarmReset_AlarmArea;
jfloButtonWidget* btnTemp_SelectToSetting_2;
jfloButtonWidget* btnFlow_SelectToSetting_2;
jfloButtonWidget* btnO2_SelectToSetting_2;
laWidget* panelLeftTouch;
laWidget* panelRightTouch;
laWidget* panelLeftAlarm;
laWidget* panelRightAlarm;
laWidget* panelBottomAlarm;
laWidget* panelAlarmTitle;
laLabelWidget* labelAlarmTitle;
laImageWidget* imageMuteIcon;
laImageWidget* iconAlarmActive;
laLabelWidget* labelSpO2Dash;
laLabelWidget* labelPRDash;
laWidget* SC_RunningTimePanel;
laLabelWidget* SC_RunningTimeLabel;
laImageWidget* SC_WifiIcon;
laTextFieldWidget* SC_DateTimeTextField;
laButtonWidget* SC_AlarmInfoButton;
laImageWidget* SC_ExternalBatteryIcon;
laImageWidget* SC_InternalBatteryIcon;
laImageWidget* SC_PowerACIcon;
laButtonWidget* SC_HomeButton;
laLabelWidget* SC_ScreenTitleLabel;
laWidget* SC_MenuSettingPanel;
laWidget* SC_LanguageMenuPanel;
laImageWidget* SC_LanguageMenuIcon;
laLabelWidget* SC_LanguageMenuLabel;
laWidget* SC_DateTimeMenuPanel;
laImageWidget* SC_DateTimeMenuIcon;
laLabelWidget* SC_DateTimeMenuLabel;
laWidget* SC_WifiMenuPanel;
laImageWidget* SC_WifiMenuIcon;
laLabelWidget* SC_WifiMenuLabel;
laWidget* SC_BrightnessMenuPanel;
laImageWidget* SC_BrightnessMenuIcon;
laLabelWidget* SC_BrightnessMenuLabel;
laWidget* SC_OxygenSourceMenuPanel;
laImageWidget* SC_OxygenSourceMenuIcon;
laLabelWidget* SC_OxygenSourceMenuLabel;
laWidget* SC_SpO2AlarmLimitMenuPanel;
laImageWidget* SC_SpO2AlarmLimitMenuIcon;
laLabelWidget* SC_SpO2AlarmLimitMenuLabel;
laWidget* SC_OxygenConcentrationAlarmLimitMenuPanel;
laImageWidget* SC_OxygenConcentrationAlarmLimitMenuIcon;
laLabelWidget* SC_OxygenConcentrationAlarmLimitMenuLabel;
laWidget* SC_AlarmSoundLevelMenuPanel;
laImageWidget* SC_AlarmSoundLevelMenuIcon;
laLabelWidget* SC_AlarmSoundLevelMenuLabel;
laWidget* SC_TabButtonPanel;
laButtonWidget* SC_SettingButton;
laButtonWidget* SC_DataLogButton;
laButtonWidget* SC_MaintenanceButton;
laButtonWidget* SC_DeviceInformationButton;
laWidget* SC_TabButtonUnderBarPanel;
laWidget* SC_AlarmTitleBarPanel;
laLabelWidget* SC_AlarmTitleLabel;
laImageWidget* SC_AlarmMuteIcon;
laImageWidget* SC_AlarmActiveIcon;
laWidget* SC_AlarmIndicatorLeftPanel;
laWidget* SC_AlarmIndicatorRightPanel;
laWidget* SC_AlarmIndicatorBottomPanel;
laWidget* SC_DataLogSettingPanel;
laLabelWidget* SC_DataLogSettingAlarmLabel;
laLabelWidget* SC_DataLogSettingEventLabel;
laWidget* SC_DataLogSettingUnderbarPanel;
laLabelWidget* SC_DataLogSettingTimeLabel;
laLabelWidget* SC_DataLogSettingTitleLabel;
laLabelWidget* SC_DataLogSettingStateLabel;
laLabelWidget* SC_DataLogSettingTimeLabel_1;
laLabelWidget* SC_DataLogSettingTimeLabel_2;
laLabelWidget* SC_DataLogSettingTimeLabel_3;
laLabelWidget* SC_DataLogSettingTimeLabel_4;
laLabelWidget* SC_DataLogSettingTimeLabel_5;
laLabelWidget* SC_DataLogSettingTitleLabel_1;
laLabelWidget* SC_DataLogSettingTitleLabel_2;
laLabelWidget* SC_DataLogSettingTitleLabel_3;
laLabelWidget* SC_DataLogSettingTitleLabel_4;
laLabelWidget* SC_DataLogSettingTitleLabel_5;
laLabelWidget* SC_DataLogSettingStateLabel_1;
laLabelWidget* SC_DataLogSettingStateLabel_2;
laLabelWidget* SC_DataLogSettingStateLabel_3;
laLabelWidget* SC_DataLogSettingStateLabel_4;
laLabelWidget* SC_DataLogSettingStateLabel_5;
laLabelWidget* SC_DataLogSettingPageNumberLabel;
laButtonWidget* SC_DataLogSettingPrevButton;
laButtonWidget* SC_DataLogSettingNextButton;
laWidget* SC_DataLogSettingIndicator_1;
laWidget* SC_DataLogSettingIndicator_2;
laWidget* SC_DataLogSettingIndicator_3;
laWidget* SC_DataLogSettingIndicator_4;
laWidget* SC_DataLogSettingIndicator_5;
laWidget* SC_MaintenancePanel;
laWidget* SC_DeviceInformationPanel;
laLabelWidget* SC_DeviceInformationMainLabel;
laLabelWidget* SC_DeviceInformationChamberLabel;
laLabelWidget* SC_DeviceInformationCradleLabel;
laLabelWidget* SC_DeviceInformationCircuitLabel;
laWidget* SC_DeviceInformationUnderbarPanel;
laLabelWidget* SC_DeviceInformationLabel_1;
laLabelWidget* SC_DeviceInformationLabel_2;
laLabelWidget* SC_DeviceInformationLabel_3;
laLabelWidget* SC_DeviceInformationLabel_4;
laLabelWidget* SC_DeviceInformationLabel_5;
laLabelWidget* SC_DeviceInformationLabel_6;
laWidget* SC_MenuSetting_SettingPanel;
laLabelWidget* SC_MenuSetting_SettingTitleLabel;
laLabelWidget* SC_MenuSetting_SettingMessageLabel;
laButtonWidget* SC_MenuSetting_SettingBackButton;
laButtonWidget* SC_MenuSetting_SettingSaveButton;
laWidget* SC_MenuSetting_SettingLanguagePanel;
laButtonWidget* SC_MenuSetting_SettingLanguage_NextButton;
laButtonWidget* SC_MenuSetting_SettingLanguage_PrevButton;
laLabelWidget* SC_MenuSetting_SettingLanguage_SettingLabel;
laWidget* SC_MenuSetting_SettingDateTimePanel;
laLabelWidget* SC_MenuSetting_SettingDateTime_YearLabel;
laLabelWidget* SC_MenuSetting_SettingDateTime_Seperator_1;
laLabelWidget* SC_MenuSetting_SettingDateTime_MonLabel;
laLabelWidget* SC_MenuSetting_SettingDateTime_Seperator_2;
laLabelWidget* SC_MenuSetting_SettingDateTime_DateLabel;
laLabelWidget* SC_MenuSetting_SettingDateTime_HourLabel;
laLabelWidget* SC_MenuSetting_SettingDateTime_SepLabel_3;
laLabelWidget* SC_MenuSetting_SettingDateTime_MinLabel;
laLabelWidget* SC_MenuSetting_SettingDateTime_Seperator_4;
laLabelWidget* SC_MenuSetting_SettingDateTime_SecLabel;
laButtonWidget* SC_MenuSetting_SettingDateTime_DecButton;
laButtonWidget* SC_MenuSetting_SettingDateTime_IncButton;
laWidget* SC_MenuSetting_SettingDateTime_UnderBarPanel;
laWidget* SC_MenuSetting_SettingWifiPanel;
laLabelWidget* SC_MenuSetting_SettingWifi_OnLabel;
laLabelWidget* SC_MenuSetting_SettingWifi_OffLabel;
laWidget* SC_MenuSetting_SettingWifi_UnderBarPanel;
laWidget* SC_MenuSetting_SettingBrightnessPanel;
laButtonWidget* SC_MenuSetting_SettingBrightness_NextButton;
laButtonWidget* SC_MenuSetting_SettingBrightness_PrevButton;
laLabelWidget* SC_MenuSetting_SettingBrightness_ValueLabel;
laLabelWidget* SC_MenuSetting_SettingBrightness_OffLabel;
laLabelWidget* SC_MenuSetting_SettingBrightness_OnLabel;
laWidget* SC_MenuSetting_SettingBrightness_UnderBar;
laLabelWidget* SC_MenuSetting_SettingBrightness_MessageLabel;
laWidget* SC_MenuSetting_SettingOxySourcePanel;
laLabelWidget* SC_MenuSetting_SettingOxySource_90Label;
laLabelWidget* SC_MenuSetting_SettingOxySource_100Label;
laWidget* SC_MenuSetting_SettingOxySource_UnderBarPanel;
laWidget* SC_MenuSetting_SettingSpo2AlarmPanel;
laButtonWidget* SC_MenuSetting_SettingSpo2Alarm_NextButton;
laButtonWidget* SC_MenuSetting_SettingSpo2Alarm_PrevButton;
laLabelWidget* SC_MenuSetting_SettingSpo2Alarm_ValueLabel;
laWidget* SC_MenuSetting_SettingOxyAlarmPanel;
laButtonWidget* SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton;
laButtonWidget* SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton;
laButtonWidget* SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton;
laButtonWidget* SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton;
laLabelWidget* SC_MenuSetting_SettingOxyAlarm_UpLimitLabel;
laLabelWidget* SC_MenuSetting_SettingOxyAlarm_LoLimitLabel;
laWidget* SC_MenuSetting_SettingAlarmSoundLevelPanel;
laButtonWidget* SC_MenuSetting_SettingAlarmSoundLevel_NextButton;
laButtonWidget* SC_MenuSetting_SettingAlarmSoundLevel_PrevButton;
laLabelWidget* SC_MenuSetting_SettingAlarmSoundLevel_ValueLabel;
laWidget* SC_SaveConfirmPanel;
laImageWidget* SC_SaveConfirmBackgroundImage;
laButtonWidget* SC_SaveConfirmYesButton;
laButtonWidget* SC_SaveConfirmNoButton;
laLabelWidget* SC_SaveConfirmTitleLabel;
laLabelWidget* lbTitleAirflow;
laLabelWidget* lbTitleO2flow;
laLabelWidget* lbTitleTemp;
laLabelWidget* s_titleMotor;
laLabelWidget* lbTitleO2;
laLabelWidget* lbTitleIH;
laButtonWidget* btnHome_MaintenanceScreen;
laTextFieldWidget* tfAirFlowMonitor;
laTextFieldWidget* tfO2FlowMonitor;
laTextFieldWidget* tfTempMonitor;
laTextFieldWidget* tfO2Monitor;
laLabelWidget* lbTitleMotor;
laSliderWidget* scrollbarMotor;
laButtonWidget* btnOnOffMotor;
laSliderWidget* scrollbarIH;
laButtonWidget* btnOnOffIH;
laLabelWidget* lbTitleWaterPump;
laButtonWidget* btnOnOffWaterPump;
laButtonWidget* btnUpgrade;
laLabelWidget* lbTitleVersion;
laButtonWidget* btnLogtoUsb;
laButtonWidget* btnClearLog;
laButtonWidget* btnTestHighAlarm_MaintenanceScreen;
laButtonWidget* btnTestMediumAlarm_MaintenanceScreen;
laButtonWidget* btnTestLowAlarm_MaintenanceScreen;
laButtonWidget* btnDebug4_MaintenanceScreen;
laButtonWidget* btnDebug5_MaintenanceScreen;
laButtonWidget* btnDebug6_MaintenanceScreen;
laButtonWidget* btnDebug7_MaintenanceScreen;
laButtonWidget* btnDebug8_MaintenanceScreen;
laLabelWidget* labelPowerOffMessage;
laLabelWidget* labelPowerOffAdditionalMessage;
laWidget* panelPowerOffAlarmIndicatorTop;
laWidget* panelPowerOffAlarmIndicatorBottom;
laWidget* panelPowerOffAlarmIndicatorLeft;
laWidget* panelPowerOffAlarmIndicatorRight;


static void ScreenCreate_UpdateScreen(laScreen* screen);
static void ScreenCreate_VideoScreen(laScreen* screen);
static void ScreenCreate_MainScreen(laScreen* screen);
static void ScreenCreate_SettingScreen(laScreen* screen);
static void ScreenCreate_MaintenanceScreen(laScreen* screen);
static void ScreenCreate_PowerOffScreen(laScreen* screen);


int32_t libaria_initialize(void)
{
    laScreen* screen;

    laScheme_Initialize(&JFLO_InativeItem_SettingScreen_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_InativeItem_SettingScreen_Scheme.base = 0xC67A;
    JFLO_InativeItem_SettingScreen_Scheme.highlight = 0xC67A;
    JFLO_InativeItem_SettingScreen_Scheme.highlightLight = 0xFFFF;
    JFLO_InativeItem_SettingScreen_Scheme.shadow = 0x8410;
    JFLO_InativeItem_SettingScreen_Scheme.shadowDark = 0x4208;
    JFLO_InativeItem_SettingScreen_Scheme.foreground = 0x0;
    JFLO_InativeItem_SettingScreen_Scheme.foregroundInactive = 0xD71C;
    JFLO_InativeItem_SettingScreen_Scheme.foregroundDisabled = 0x8410;
    JFLO_InativeItem_SettingScreen_Scheme.background = 0xFFFF;
    JFLO_InativeItem_SettingScreen_Scheme.backgroundInactive = 0xD71C;
    JFLO_InativeItem_SettingScreen_Scheme.backgroundDisabled = 0xC67A;
    JFLO_InativeItem_SettingScreen_Scheme.text = 0x0;
    JFLO_InativeItem_SettingScreen_Scheme.textHighlight = 0x1F;
    JFLO_InativeItem_SettingScreen_Scheme.textHighlightText = 0xFFFF;
    JFLO_InativeItem_SettingScreen_Scheme.textInactive = 0xD71C;
    JFLO_InativeItem_SettingScreen_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_Line_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_Line_Scheme.base = 0x7FF;
    JFLO_Line_Scheme.highlight = 0xC67A;
    JFLO_Line_Scheme.highlightLight = 0xFFFF;
    JFLO_Line_Scheme.shadow = 0x8410;
    JFLO_Line_Scheme.shadowDark = 0x4208;
    JFLO_Line_Scheme.foreground = 0x7FF;
    JFLO_Line_Scheme.foregroundInactive = 0xD71C;
    JFLO_Line_Scheme.foregroundDisabled = 0x8410;
    JFLO_Line_Scheme.background = 0xFFFF;
    JFLO_Line_Scheme.backgroundInactive = 0xD71C;
    JFLO_Line_Scheme.backgroundDisabled = 0xC67A;
    JFLO_Line_Scheme.text = 0x0;
    JFLO_Line_Scheme.textHighlight = 0x1F;
    JFLO_Line_Scheme.textHighlightText = 0xFFFF;
    JFLO_Line_Scheme.textInactive = 0xD71C;
    JFLO_Line_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_ShowingWindow_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_ShowingWindow_Scheme.base = 0x8410;
    JFLO_ShowingWindow_Scheme.highlight = 0xC67A;
    JFLO_ShowingWindow_Scheme.highlightLight = 0xFFFF;
    JFLO_ShowingWindow_Scheme.shadow = 0x8410;
    JFLO_ShowingWindow_Scheme.shadowDark = 0xFFFF;
    JFLO_ShowingWindow_Scheme.foreground = 0xFFFF;
    JFLO_ShowingWindow_Scheme.foregroundInactive = 0xD71C;
    JFLO_ShowingWindow_Scheme.foregroundDisabled = 0x8410;
    JFLO_ShowingWindow_Scheme.background = 0x8410;
    JFLO_ShowingWindow_Scheme.backgroundInactive = 0xD71C;
    JFLO_ShowingWindow_Scheme.backgroundDisabled = 0xC67A;
    JFLO_ShowingWindow_Scheme.text = 0xFFFF;
    JFLO_ShowingWindow_Scheme.textHighlight = 0x1F;
    JFLO_ShowingWindow_Scheme.textHighlightText = 0xFFFF;
    JFLO_ShowingWindow_Scheme.textInactive = 0xD71C;
    JFLO_ShowingWindow_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&ClearScheme, GFX_COLOR_MODE_RGB_565);
    ClearScheme.base = 0x0;
    ClearScheme.highlight = 0xC67A;
    ClearScheme.highlightLight = 0xFFFF;
    ClearScheme.shadow = 0x8410;
    ClearScheme.shadowDark = 0x4208;
    ClearScheme.foreground = 0x0;
    ClearScheme.foregroundInactive = 0xD71C;
    ClearScheme.foregroundDisabled = 0x8410;
    ClearScheme.background = 0x0;
    ClearScheme.backgroundInactive = 0xD71C;
    ClearScheme.backgroundDisabled = 0xC67A;
    ClearScheme.text = 0x0;
    ClearScheme.textHighlight = 0x1F;
    ClearScheme.textHighlightText = 0xFFFF;
    ClearScheme.textInactive = 0xD71C;
    ClearScheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_ReverseItem_SettingScreen_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_ReverseItem_SettingScreen_Scheme.base = 0xC67A;
    JFLO_ReverseItem_SettingScreen_Scheme.highlight = 0xFFE0;
    JFLO_ReverseItem_SettingScreen_Scheme.highlightLight = 0xFFFF;
    JFLO_ReverseItem_SettingScreen_Scheme.shadow = 0x8410;
    JFLO_ReverseItem_SettingScreen_Scheme.shadowDark = 0xFFFF;
    JFLO_ReverseItem_SettingScreen_Scheme.foreground = 0xF800;
    JFLO_ReverseItem_SettingScreen_Scheme.foregroundInactive = 0xD71C;
    JFLO_ReverseItem_SettingScreen_Scheme.foregroundDisabled = 0x8410;
    JFLO_ReverseItem_SettingScreen_Scheme.background = 0xF81F;
    JFLO_ReverseItem_SettingScreen_Scheme.backgroundInactive = 0xD71C;
    JFLO_ReverseItem_SettingScreen_Scheme.backgroundDisabled = 0xC67A;
    JFLO_ReverseItem_SettingScreen_Scheme.text = 0x0;
    JFLO_ReverseItem_SettingScreen_Scheme.textHighlight = 0x1F;
    JFLO_ReverseItem_SettingScreen_Scheme.textHighlightText = 0xFFFF;
    JFLO_ReverseItem_SettingScreen_Scheme.textInactive = 0xD71C;
    JFLO_ReverseItem_SettingScreen_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_SpO2Graph_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_SpO2Graph_Scheme.base = 0x4208;
    JFLO_SpO2Graph_Scheme.highlight = 0xC67A;
    JFLO_SpO2Graph_Scheme.highlightLight = 0xFFFF;
    JFLO_SpO2Graph_Scheme.shadow = 0x8410;
    JFLO_SpO2Graph_Scheme.shadowDark = 0x4208;
    JFLO_SpO2Graph_Scheme.foreground = 0x1E3F;
    JFLO_SpO2Graph_Scheme.foregroundInactive = 0x4208;
    JFLO_SpO2Graph_Scheme.foregroundDisabled = 0x4208;
    JFLO_SpO2Graph_Scheme.background = 0xBDF7;
    JFLO_SpO2Graph_Scheme.backgroundInactive = 0xD71C;
    JFLO_SpO2Graph_Scheme.backgroundDisabled = 0xC67A;
    JFLO_SpO2Graph_Scheme.text = 0x4208;
    JFLO_SpO2Graph_Scheme.textHighlight = 0x1F;
    JFLO_SpO2Graph_Scheme.textHighlightText = 0xFFFF;
    JFLO_SpO2Graph_Scheme.textInactive = 0xD71C;
    JFLO_SpO2Graph_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_SelectedItem_SettingScreen_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_SelectedItem_SettingScreen_Scheme.base = 0x8410;
    JFLO_SelectedItem_SettingScreen_Scheme.highlight = 0xC67A;
    JFLO_SelectedItem_SettingScreen_Scheme.highlightLight = 0xFFFF;
    JFLO_SelectedItem_SettingScreen_Scheme.shadow = 0x8410;
    JFLO_SelectedItem_SettingScreen_Scheme.shadowDark = 0x4208;
    JFLO_SelectedItem_SettingScreen_Scheme.foreground = 0xFFFF;
    JFLO_SelectedItem_SettingScreen_Scheme.foregroundInactive = 0xD71C;
    JFLO_SelectedItem_SettingScreen_Scheme.foregroundDisabled = 0x8410;
    JFLO_SelectedItem_SettingScreen_Scheme.background = 0xFDF0;
    JFLO_SelectedItem_SettingScreen_Scheme.backgroundInactive = 0xF800;
    JFLO_SelectedItem_SettingScreen_Scheme.backgroundDisabled = 0xC67A;
    JFLO_SelectedItem_SettingScreen_Scheme.text = 0x0;
    JFLO_SelectedItem_SettingScreen_Scheme.textHighlight = 0x1F;
    JFLO_SelectedItem_SettingScreen_Scheme.textHighlightText = 0xFFFF;
    JFLO_SelectedItem_SettingScreen_Scheme.textInactive = 0xD71C;
    JFLO_SelectedItem_SettingScreen_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_AlarmIndicatorMediumPriority_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_AlarmIndicatorMediumPriority_Scheme.base = 0xFFE0;
    JFLO_AlarmIndicatorMediumPriority_Scheme.highlight = 0xC67A;
    JFLO_AlarmIndicatorMediumPriority_Scheme.highlightLight = 0xFFFF;
    JFLO_AlarmIndicatorMediumPriority_Scheme.shadow = 0x8410;
    JFLO_AlarmIndicatorMediumPriority_Scheme.shadowDark = 0x4208;
    JFLO_AlarmIndicatorMediumPriority_Scheme.foreground = 0x0;
    JFLO_AlarmIndicatorMediumPriority_Scheme.foregroundInactive = 0xD71C;
    JFLO_AlarmIndicatorMediumPriority_Scheme.foregroundDisabled = 0x8410;
    JFLO_AlarmIndicatorMediumPriority_Scheme.background = 0xFFE0;
    JFLO_AlarmIndicatorMediumPriority_Scheme.backgroundInactive = 0xD71C;
    JFLO_AlarmIndicatorMediumPriority_Scheme.backgroundDisabled = 0xC67A;
    JFLO_AlarmIndicatorMediumPriority_Scheme.text = 0x0;
    JFLO_AlarmIndicatorMediumPriority_Scheme.textHighlight = 0x1F;
    JFLO_AlarmIndicatorMediumPriority_Scheme.textHighlightText = 0xFFFF;
    JFLO_AlarmIndicatorMediumPriority_Scheme.textInactive = 0xD71C;
    JFLO_AlarmIndicatorMediumPriority_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_MessageBox_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_MessageBox_Scheme.base = 0x4208;
    JFLO_MessageBox_Scheme.highlight = 0xC67A;
    JFLO_MessageBox_Scheme.highlightLight = 0xFFFF;
    JFLO_MessageBox_Scheme.shadow = 0x8410;
    JFLO_MessageBox_Scheme.shadowDark = 0x7FF;
    JFLO_MessageBox_Scheme.foreground = 0x0;
    JFLO_MessageBox_Scheme.foregroundInactive = 0xD71C;
    JFLO_MessageBox_Scheme.foregroundDisabled = 0x8410;
    JFLO_MessageBox_Scheme.background = 0x4208;
    JFLO_MessageBox_Scheme.backgroundInactive = 0xD71C;
    JFLO_MessageBox_Scheme.backgroundDisabled = 0xC67A;
    JFLO_MessageBox_Scheme.text = 0xFFFF;
    JFLO_MessageBox_Scheme.textHighlight = 0x1F;
    JFLO_MessageBox_Scheme.textHighlightText = 0xFFFF;
    JFLO_MessageBox_Scheme.textInactive = 0xD71C;
    JFLO_MessageBox_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_AlarmIndicatorLow1Priority_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_AlarmIndicatorLow1Priority_Scheme.base = 0xFFE0;
    JFLO_AlarmIndicatorLow1Priority_Scheme.highlight = 0xC67A;
    JFLO_AlarmIndicatorLow1Priority_Scheme.highlightLight = 0xFFFF;
    JFLO_AlarmIndicatorLow1Priority_Scheme.shadow = 0x8410;
    JFLO_AlarmIndicatorLow1Priority_Scheme.shadowDark = 0x4208;
    JFLO_AlarmIndicatorLow1Priority_Scheme.foreground = 0x0;
    JFLO_AlarmIndicatorLow1Priority_Scheme.foregroundInactive = 0xD71C;
    JFLO_AlarmIndicatorLow1Priority_Scheme.foregroundDisabled = 0x8410;
    JFLO_AlarmIndicatorLow1Priority_Scheme.background = 0xFFE0;
    JFLO_AlarmIndicatorLow1Priority_Scheme.backgroundInactive = 0xD71C;
    JFLO_AlarmIndicatorLow1Priority_Scheme.backgroundDisabled = 0xC67A;
    JFLO_AlarmIndicatorLow1Priority_Scheme.text = 0x0;
    JFLO_AlarmIndicatorLow1Priority_Scheme.textHighlight = 0x1F;
    JFLO_AlarmIndicatorLow1Priority_Scheme.textHighlightText = 0xFFFF;
    JFLO_AlarmIndicatorLow1Priority_Scheme.textInactive = 0xD71C;
    JFLO_AlarmIndicatorLow1Priority_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_AlarmIndicatorHighPriority_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_AlarmIndicatorHighPriority_Scheme.base = 0xF800;
    JFLO_AlarmIndicatorHighPriority_Scheme.highlight = 0xC67A;
    JFLO_AlarmIndicatorHighPriority_Scheme.highlightLight = 0xFFFF;
    JFLO_AlarmIndicatorHighPriority_Scheme.shadow = 0x8410;
    JFLO_AlarmIndicatorHighPriority_Scheme.shadowDark = 0x4208;
    JFLO_AlarmIndicatorHighPriority_Scheme.foreground = 0x0;
    JFLO_AlarmIndicatorHighPriority_Scheme.foregroundInactive = 0xD71C;
    JFLO_AlarmIndicatorHighPriority_Scheme.foregroundDisabled = 0x8410;
    JFLO_AlarmIndicatorHighPriority_Scheme.background = 0xF800;
    JFLO_AlarmIndicatorHighPriority_Scheme.backgroundInactive = 0xD71C;
    JFLO_AlarmIndicatorHighPriority_Scheme.backgroundDisabled = 0xC67A;
    JFLO_AlarmIndicatorHighPriority_Scheme.text = 0x0;
    JFLO_AlarmIndicatorHighPriority_Scheme.textHighlight = 0x1F;
    JFLO_AlarmIndicatorHighPriority_Scheme.textHighlightText = 0xFFFF;
    JFLO_AlarmIndicatorHighPriority_Scheme.textInactive = 0xD71C;
    JFLO_AlarmIndicatorHighPriority_Scheme.textDisabled = 0x8C92;
    
    laScheme_Initialize(&JFLO_AlarmArea_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_AlarmArea_Scheme.base = 0xFE04;
    JFLO_AlarmArea_Scheme.highlight = 0xC67A;
    JFLO_AlarmArea_Scheme.highlightLight = 0xFFFF;
    JFLO_AlarmArea_Scheme.shadow = 0x8410;
    JFLO_AlarmArea_Scheme.shadowDark = 0x4208;
    JFLO_AlarmArea_Scheme.foreground = 0x0;
    JFLO_AlarmArea_Scheme.foregroundInactive = 0xD71C;
    JFLO_AlarmArea_Scheme.foregroundDisabled = 0x8410;
    JFLO_AlarmArea_Scheme.background = 0x528A;
    JFLO_AlarmArea_Scheme.backgroundInactive = 0xD71C;
    JFLO_AlarmArea_Scheme.backgroundDisabled = 0xC67A;
    JFLO_AlarmArea_Scheme.text = 0x0;
    JFLO_AlarmArea_Scheme.textHighlight = 0x1F;
    JFLO_AlarmArea_Scheme.textHighlightText = 0xFFFF;
    JFLO_AlarmArea_Scheme.textInactive = 0xD71C;
    JFLO_AlarmArea_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_CyanIndicator_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_CyanIndicator_Scheme.base = 0x7FF;
    JFLO_CyanIndicator_Scheme.highlight = 0xC67A;
    JFLO_CyanIndicator_Scheme.highlightLight = 0xFFFF;
    JFLO_CyanIndicator_Scheme.shadow = 0x8410;
    JFLO_CyanIndicator_Scheme.shadowDark = 0x4208;
    JFLO_CyanIndicator_Scheme.foreground = 0x7FF;
    JFLO_CyanIndicator_Scheme.foregroundInactive = 0xD71C;
    JFLO_CyanIndicator_Scheme.foregroundDisabled = 0x8410;
    JFLO_CyanIndicator_Scheme.background = 0xFFFF;
    JFLO_CyanIndicator_Scheme.backgroundInactive = 0xD71C;
    JFLO_CyanIndicator_Scheme.backgroundDisabled = 0xC67A;
    JFLO_CyanIndicator_Scheme.text = 0x0;
    JFLO_CyanIndicator_Scheme.textHighlight = 0x7FF;
    JFLO_CyanIndicator_Scheme.textHighlightText = 0xFFFF;
    JFLO_CyanIndicator_Scheme.textInactive = 0xD71C;
    JFLO_CyanIndicator_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_CellTable_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_CellTable_Scheme.base = 0xC67A;
    JFLO_CellTable_Scheme.highlight = 0xC67A;
    JFLO_CellTable_Scheme.highlightLight = 0xFFFF;
    JFLO_CellTable_Scheme.shadow = 0x8410;
    JFLO_CellTable_Scheme.shadowDark = 0xFFFF;
    JFLO_CellTable_Scheme.foreground = 0xFFFF;
    JFLO_CellTable_Scheme.foregroundInactive = 0xD71C;
    JFLO_CellTable_Scheme.foregroundDisabled = 0x8410;
    JFLO_CellTable_Scheme.background = 0xFFFF;
    JFLO_CellTable_Scheme.backgroundInactive = 0xD71C;
    JFLO_CellTable_Scheme.backgroundDisabled = 0xC67A;
    JFLO_CellTable_Scheme.text = 0xFFFF;
    JFLO_CellTable_Scheme.textHighlight = 0x1F;
    JFLO_CellTable_Scheme.textHighlightText = 0xFFFF;
    JFLO_CellTable_Scheme.textInactive = 0xD71C;
    JFLO_CellTable_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_Slider_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_Slider_Scheme.base = 0x7FF;
    JFLO_Slider_Scheme.highlight = 0xC67A;
    JFLO_Slider_Scheme.highlightLight = 0xFFFF;
    JFLO_Slider_Scheme.shadow = 0x8410;
    JFLO_Slider_Scheme.shadowDark = 0x4208;
    JFLO_Slider_Scheme.foreground = 0x7FF;
    JFLO_Slider_Scheme.foregroundInactive = 0xD71C;
    JFLO_Slider_Scheme.foregroundDisabled = 0x8410;
    JFLO_Slider_Scheme.background = 0xFFFF;
    JFLO_Slider_Scheme.backgroundInactive = 0xD71C;
    JFLO_Slider_Scheme.backgroundDisabled = 0xC67A;
    JFLO_Slider_Scheme.text = 0x0;
    JFLO_Slider_Scheme.textHighlight = 0x7FF;
    JFLO_Slider_Scheme.textHighlightText = 0xFFFF;
    JFLO_Slider_Scheme.textInactive = 0xD71C;
    JFLO_Slider_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_AlarmInfo_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_AlarmInfo_Scheme.base = 0xC67A;
    JFLO_AlarmInfo_Scheme.highlight = 0xC67A;
    JFLO_AlarmInfo_Scheme.highlightLight = 0xFFFF;
    JFLO_AlarmInfo_Scheme.shadow = 0x8410;
    JFLO_AlarmInfo_Scheme.shadowDark = 0x4208;
    JFLO_AlarmInfo_Scheme.foreground = 0x0;
    JFLO_AlarmInfo_Scheme.foregroundInactive = 0xD71C;
    JFLO_AlarmInfo_Scheme.foregroundDisabled = 0x8410;
    JFLO_AlarmInfo_Scheme.background = 0x528A;
    JFLO_AlarmInfo_Scheme.backgroundInactive = 0xD71C;
    JFLO_AlarmInfo_Scheme.backgroundDisabled = 0xC67A;
    JFLO_AlarmInfo_Scheme.text = 0xFE04;
    JFLO_AlarmInfo_Scheme.textHighlight = 0x1F;
    JFLO_AlarmInfo_Scheme.textHighlightText = 0xFFFF;
    JFLO_AlarmInfo_Scheme.textInactive = 0xD71C;
    JFLO_AlarmInfo_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_NoticeRunningArea_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_NoticeRunningArea_Scheme.base = 0x528A;
    JFLO_NoticeRunningArea_Scheme.highlight = 0xC67A;
    JFLO_NoticeRunningArea_Scheme.highlightLight = 0xFFFF;
    JFLO_NoticeRunningArea_Scheme.shadow = 0x8410;
    JFLO_NoticeRunningArea_Scheme.shadowDark = 0x4208;
    JFLO_NoticeRunningArea_Scheme.foreground = 0x0;
    JFLO_NoticeRunningArea_Scheme.foregroundInactive = 0xD71C;
    JFLO_NoticeRunningArea_Scheme.foregroundDisabled = 0x8410;
    JFLO_NoticeRunningArea_Scheme.background = 0xFFFF;
    JFLO_NoticeRunningArea_Scheme.backgroundInactive = 0xD71C;
    JFLO_NoticeRunningArea_Scheme.backgroundDisabled = 0xC67A;
    JFLO_NoticeRunningArea_Scheme.text = 0x8C71;
    JFLO_NoticeRunningArea_Scheme.textHighlight = 0x1F;
    JFLO_NoticeRunningArea_Scheme.textHighlightText = 0xFFFF;
    JFLO_NoticeRunningArea_Scheme.textInactive = 0xD71C;
    JFLO_NoticeRunningArea_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_Button_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_Button_Scheme.base = 0x4208;
    JFLO_Button_Scheme.highlight = 0x4208;
    JFLO_Button_Scheme.highlightLight = 0xFFFF;
    JFLO_Button_Scheme.shadow = 0x8410;
    JFLO_Button_Scheme.shadowDark = 0x4208;
    JFLO_Button_Scheme.foreground = 0x528A;
    JFLO_Button_Scheme.foregroundInactive = 0xD71C;
    JFLO_Button_Scheme.foregroundDisabled = 0x8410;
    JFLO_Button_Scheme.background = 0x4208;
    JFLO_Button_Scheme.backgroundInactive = 0xD71C;
    JFLO_Button_Scheme.backgroundDisabled = 0xC67A;
    JFLO_Button_Scheme.text = 0x0;
    JFLO_Button_Scheme.textHighlight = 0x1F;
    JFLO_Button_Scheme.textHighlightText = 0xFFFF;
    JFLO_Button_Scheme.textInactive = 0xD71C;
    JFLO_Button_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_Button_OnChangedSetpointArea_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_Button_OnChangedSetpointArea_Scheme.base = 0x528A;
    JFLO_Button_OnChangedSetpointArea_Scheme.highlight = 0x7FF;
    JFLO_Button_OnChangedSetpointArea_Scheme.highlightLight = 0xFFFF;
    JFLO_Button_OnChangedSetpointArea_Scheme.shadow = 0x8410;
    JFLO_Button_OnChangedSetpointArea_Scheme.shadowDark = 0x7FF;
    JFLO_Button_OnChangedSetpointArea_Scheme.foreground = 0x528A;
    JFLO_Button_OnChangedSetpointArea_Scheme.foregroundInactive = 0xD71C;
    JFLO_Button_OnChangedSetpointArea_Scheme.foregroundDisabled = 0x8410;
    JFLO_Button_OnChangedSetpointArea_Scheme.background = 0x528A;
    JFLO_Button_OnChangedSetpointArea_Scheme.backgroundInactive = 0xD71C;
    JFLO_Button_OnChangedSetpointArea_Scheme.backgroundDisabled = 0xC67A;
    JFLO_Button_OnChangedSetpointArea_Scheme.text = 0xFFFF;
    JFLO_Button_OnChangedSetpointArea_Scheme.textHighlight = 0x1F;
    JFLO_Button_OnChangedSetpointArea_Scheme.textHighlightText = 0xFFFF;
    JFLO_Button_OnChangedSetpointArea_Scheme.textInactive = 0xD71C;
    JFLO_Button_OnChangedSetpointArea_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_Background_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_Background_Scheme.base = 0x4208;
    JFLO_Background_Scheme.highlight = 0xC67A;
    JFLO_Background_Scheme.highlightLight = 0xFFFF;
    JFLO_Background_Scheme.shadow = 0x8410;
    JFLO_Background_Scheme.shadowDark = 0x4208;
    JFLO_Background_Scheme.foreground = 0x0;
    JFLO_Background_Scheme.foregroundInactive = 0xD71C;
    JFLO_Background_Scheme.foregroundDisabled = 0x8410;
    JFLO_Background_Scheme.background = 0x4208;
    JFLO_Background_Scheme.backgroundInactive = 0xD71C;
    JFLO_Background_Scheme.backgroundDisabled = 0xC67A;
    JFLO_Background_Scheme.text = 0xFFFF;
    JFLO_Background_Scheme.textHighlight = 0x1F;
    JFLO_Background_Scheme.textHighlightText = 0xFFFF;
    JFLO_Background_Scheme.textInactive = 0xD71C;
    JFLO_Background_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_Underbar_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_Underbar_Scheme.base = 0x7FF;
    JFLO_Underbar_Scheme.highlight = 0xC67A;
    JFLO_Underbar_Scheme.highlightLight = 0xFFFF;
    JFLO_Underbar_Scheme.shadow = 0x8410;
    JFLO_Underbar_Scheme.shadowDark = 0x4208;
    JFLO_Underbar_Scheme.foreground = 0x7FF;
    JFLO_Underbar_Scheme.foregroundInactive = 0xD71C;
    JFLO_Underbar_Scheme.foregroundDisabled = 0x8410;
    JFLO_Underbar_Scheme.background = 0xFFFF;
    JFLO_Underbar_Scheme.backgroundInactive = 0xD71C;
    JFLO_Underbar_Scheme.backgroundDisabled = 0xC67A;
    JFLO_Underbar_Scheme.text = 0x0;
    JFLO_Underbar_Scheme.textHighlight = 0x7FF;
    JFLO_Underbar_Scheme.textHighlightText = 0xFFFF;
    JFLO_Underbar_Scheme.textInactive = 0xD71C;
    JFLO_Underbar_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_RectChangedSetpoint_2_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_RectChangedSetpoint_2_Scheme.base = 0xC67A;
    JFLO_RectChangedSetpoint_2_Scheme.highlight = 0xC67A;
    JFLO_RectChangedSetpoint_2_Scheme.highlightLight = 0xFFFF;
    JFLO_RectChangedSetpoint_2_Scheme.shadow = 0x8410;
    JFLO_RectChangedSetpoint_2_Scheme.shadowDark = 0x4208;
    JFLO_RectChangedSetpoint_2_Scheme.foreground = 0x8410;
    JFLO_RectChangedSetpoint_2_Scheme.foregroundInactive = 0xD71C;
    JFLO_RectChangedSetpoint_2_Scheme.foregroundDisabled = 0x8410;
    JFLO_RectChangedSetpoint_2_Scheme.background = 0xFFFF;
    JFLO_RectChangedSetpoint_2_Scheme.backgroundInactive = 0xD71C;
    JFLO_RectChangedSetpoint_2_Scheme.backgroundDisabled = 0xC67A;
    JFLO_RectChangedSetpoint_2_Scheme.text = 0x0;
    JFLO_RectChangedSetpoint_2_Scheme.textHighlight = 0x1F;
    JFLO_RectChangedSetpoint_2_Scheme.textHighlightText = 0xFFFF;
    JFLO_RectChangedSetpoint_2_Scheme.textInactive = 0xD71C;
    JFLO_RectChangedSetpoint_2_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&BlackBackgroundScheme, GFX_COLOR_MODE_RGB_565);
    BlackBackgroundScheme.base = 0x0;
    BlackBackgroundScheme.highlight = 0x0;
    BlackBackgroundScheme.highlightLight = 0x0;
    BlackBackgroundScheme.shadow = 0x0;
    BlackBackgroundScheme.shadowDark = 0xFFFF;
    BlackBackgroundScheme.foreground = 0x0;
    BlackBackgroundScheme.foregroundInactive = 0x0;
    BlackBackgroundScheme.foregroundDisabled = 0x0;
    BlackBackgroundScheme.background = 0x0;
    BlackBackgroundScheme.backgroundInactive = 0x0;
    BlackBackgroundScheme.backgroundDisabled = 0x0;
    BlackBackgroundScheme.text = 0xFFFF;
    BlackBackgroundScheme.textHighlight = 0x1F;
    BlackBackgroundScheme.textHighlightText = 0x0;
    BlackBackgroundScheme.textInactive = 0x0;
    BlackBackgroundScheme.textDisabled = 0x0;

    laScheme_Initialize(&ButtonTextScheme, GFX_COLOR_MODE_RGB_565);
    ButtonTextScheme.base = 0xC67A;
    ButtonTextScheme.highlight = 0xC67A;
    ButtonTextScheme.highlightLight = 0xFFFF;
    ButtonTextScheme.shadow = 0x8410;
    ButtonTextScheme.shadowDark = 0x4208;
    ButtonTextScheme.foreground = 0x0;
    ButtonTextScheme.foregroundInactive = 0xD71C;
    ButtonTextScheme.foregroundDisabled = 0x8410;
    ButtonTextScheme.background = 0xFFFF;
    ButtonTextScheme.backgroundInactive = 0xD71C;
    ButtonTextScheme.backgroundDisabled = 0xC67A;
    ButtonTextScheme.text = 0xDEFB;
    ButtonTextScheme.textHighlight = 0x1F;
    ButtonTextScheme.textHighlightText = 0xFFFF;
    ButtonTextScheme.textInactive = 0xD71C;
    ButtonTextScheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_SettingForm_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_SettingForm_Scheme.base = 0x7E0;
    JFLO_SettingForm_Scheme.highlight = 0xBDF7;
    JFLO_SettingForm_Scheme.highlightLight = 0xFFFF;
    JFLO_SettingForm_Scheme.shadow = 0x8410;
    JFLO_SettingForm_Scheme.shadowDark = 0x4208;
    JFLO_SettingForm_Scheme.foreground = 0x528A;
    JFLO_SettingForm_Scheme.foregroundInactive = 0xBDF7;
    JFLO_SettingForm_Scheme.foregroundDisabled = 0x8410;
    JFLO_SettingForm_Scheme.background = 0x4208;
    JFLO_SettingForm_Scheme.backgroundInactive = 0x8410;
    JFLO_SettingForm_Scheme.backgroundDisabled = 0xC67A;
    JFLO_SettingForm_Scheme.text = 0xFFFF;
    JFLO_SettingForm_Scheme.textHighlight = 0x1F;
    JFLO_SettingForm_Scheme.textHighlightText = 0x0;
    JFLO_SettingForm_Scheme.textInactive = 0xFC10;
    JFLO_SettingForm_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_Maintainance_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_Maintainance_Scheme.base = 0xC67A;
    JFLO_Maintainance_Scheme.highlight = 0xC67A;
    JFLO_Maintainance_Scheme.highlightLight = 0xFFFF;
    JFLO_Maintainance_Scheme.shadow = 0x8410;
    JFLO_Maintainance_Scheme.shadowDark = 0x4208;
    JFLO_Maintainance_Scheme.foreground = 0x0;
    JFLO_Maintainance_Scheme.foregroundInactive = 0xD71C;
    JFLO_Maintainance_Scheme.foregroundDisabled = 0x8410;
    JFLO_Maintainance_Scheme.background = 0xFFFF;
    JFLO_Maintainance_Scheme.backgroundInactive = 0xD71C;
    JFLO_Maintainance_Scheme.backgroundDisabled = 0xC67A;
    JFLO_Maintainance_Scheme.text = 0xFFFF;
    JFLO_Maintainance_Scheme.textHighlight = 0x1F;
    JFLO_Maintainance_Scheme.textHighlightText = 0xFFFF;
    JFLO_Maintainance_Scheme.textInactive = 0xD71C;
    JFLO_Maintainance_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_MonitoringArea_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_MonitoringArea_Scheme.base = 0x528A;
    JFLO_MonitoringArea_Scheme.highlight = 0x7FF;
    JFLO_MonitoringArea_Scheme.highlightLight = 0xFFFF;
    JFLO_MonitoringArea_Scheme.shadow = 0x8410;
    JFLO_MonitoringArea_Scheme.shadowDark = 0x4208;
    JFLO_MonitoringArea_Scheme.foreground = 0x528A;
    JFLO_MonitoringArea_Scheme.foregroundInactive = 0xD71C;
    JFLO_MonitoringArea_Scheme.foregroundDisabled = 0x8410;
    JFLO_MonitoringArea_Scheme.background = 0x528A;
    JFLO_MonitoringArea_Scheme.backgroundInactive = 0xD71C;
    JFLO_MonitoringArea_Scheme.backgroundDisabled = 0xC67A;
    JFLO_MonitoringArea_Scheme.text = 0xFFFF;
    JFLO_MonitoringArea_Scheme.textHighlight = 0x1F;
    JFLO_MonitoringArea_Scheme.textHighlightText = 0xFFFF;
    JFLO_MonitoringArea_Scheme.textInactive = 0xD71C;
    JFLO_MonitoringArea_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_WarmingUp_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_WarmingUp_Scheme.base = 0x1E3F;
    JFLO_WarmingUp_Scheme.highlight = 0xC67A;
    JFLO_WarmingUp_Scheme.highlightLight = 0xFFFF;
    JFLO_WarmingUp_Scheme.shadow = 0x8410;
    JFLO_WarmingUp_Scheme.shadowDark = 0x4208;
    JFLO_WarmingUp_Scheme.foreground = 0x0;
    JFLO_WarmingUp_Scheme.foregroundInactive = 0xD71C;
    JFLO_WarmingUp_Scheme.foregroundDisabled = 0x8410;
    JFLO_WarmingUp_Scheme.background = 0xFFFF;
    JFLO_WarmingUp_Scheme.backgroundInactive = 0xD71C;
    JFLO_WarmingUp_Scheme.backgroundDisabled = 0xC67A;
    JFLO_WarmingUp_Scheme.text = 0x0;
    JFLO_WarmingUp_Scheme.textHighlight = 0x1F;
    JFLO_WarmingUp_Scheme.textHighlightText = 0xFFFF;
    JFLO_WarmingUp_Scheme.textInactive = 0xD71C;
    JFLO_WarmingUp_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_RedIndicator_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_RedIndicator_Scheme.base = 0xF800;
    JFLO_RedIndicator_Scheme.highlight = 0xC67A;
    JFLO_RedIndicator_Scheme.highlightLight = 0xFFFF;
    JFLO_RedIndicator_Scheme.shadow = 0x8410;
    JFLO_RedIndicator_Scheme.shadowDark = 0x4208;
    JFLO_RedIndicator_Scheme.foreground = 0x0;
    JFLO_RedIndicator_Scheme.foregroundInactive = 0xD71C;
    JFLO_RedIndicator_Scheme.foregroundDisabled = 0x8410;
    JFLO_RedIndicator_Scheme.background = 0xF800;
    JFLO_RedIndicator_Scheme.backgroundInactive = 0xD71C;
    JFLO_RedIndicator_Scheme.backgroundDisabled = 0xC67A;
    JFLO_RedIndicator_Scheme.text = 0x0;
    JFLO_RedIndicator_Scheme.textHighlight = 0x1F;
    JFLO_RedIndicator_Scheme.textHighlightText = 0xFFFF;
    JFLO_RedIndicator_Scheme.textInactive = 0xD71C;
    JFLO_RedIndicator_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_AlarmArea_ButtonScheme, GFX_COLOR_MODE_RGB_565);
    JFLO_AlarmArea_ButtonScheme.base = 0xC67A;
    JFLO_AlarmArea_ButtonScheme.highlight = 0xC67A;
    JFLO_AlarmArea_ButtonScheme.highlightLight = 0xFFFF;
    JFLO_AlarmArea_ButtonScheme.shadow = 0x8410;
    JFLO_AlarmArea_ButtonScheme.shadowDark = 0x4208;
    JFLO_AlarmArea_ButtonScheme.foreground = 0x7FF;
    JFLO_AlarmArea_ButtonScheme.foregroundInactive = 0xD71C;
    JFLO_AlarmArea_ButtonScheme.foregroundDisabled = 0x8410;
    JFLO_AlarmArea_ButtonScheme.background = 0xFFFF;
    JFLO_AlarmArea_ButtonScheme.backgroundInactive = 0xD71C;
    JFLO_AlarmArea_ButtonScheme.backgroundDisabled = 0xC67A;
    JFLO_AlarmArea_ButtonScheme.text = 0x0;
    JFLO_AlarmArea_ButtonScheme.textHighlight = 0x1F;
    JFLO_AlarmArea_ButtonScheme.textHighlightText = 0xFFFF;
    JFLO_AlarmArea_ButtonScheme.textInactive = 0xD71C;
    JFLO_AlarmArea_ButtonScheme.textDisabled = 0x8C92;
    
    laScheme_Initialize(&JFLO_YellowIndicator_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_YellowIndicator_Scheme.base = 0xFFE0;
    JFLO_YellowIndicator_Scheme.highlight = 0xC67A;
    JFLO_YellowIndicator_Scheme.highlightLight = 0xFFFF;
    JFLO_YellowIndicator_Scheme.shadow = 0x8410;
    JFLO_YellowIndicator_Scheme.shadowDark = 0x4208;
    JFLO_YellowIndicator_Scheme.foreground = 0x0;
    JFLO_YellowIndicator_Scheme.foregroundInactive = 0xD71C;
    JFLO_YellowIndicator_Scheme.foregroundDisabled = 0x8410;
    JFLO_YellowIndicator_Scheme.background = 0xFFE0;
    JFLO_YellowIndicator_Scheme.backgroundInactive = 0xD71C;
    JFLO_YellowIndicator_Scheme.backgroundDisabled = 0xC67A;
    JFLO_YellowIndicator_Scheme.text = 0x0;
    JFLO_YellowIndicator_Scheme.textHighlight = 0x1F;
    JFLO_YellowIndicator_Scheme.textHighlightText = 0xFFFF;
    JFLO_YellowIndicator_Scheme.textInactive = 0xD71C;
    JFLO_YellowIndicator_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_SpO2Data_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_SpO2Data_Scheme.base = 0x4208;
    JFLO_SpO2Data_Scheme.highlight = 0xC67A;
    JFLO_SpO2Data_Scheme.highlightLight = 0xFFFF;
    JFLO_SpO2Data_Scheme.shadow = 0x8410;
    JFLO_SpO2Data_Scheme.shadowDark = 0x4208;
    JFLO_SpO2Data_Scheme.foreground = 0x7FF;
    JFLO_SpO2Data_Scheme.foregroundInactive = 0xD71C;
    JFLO_SpO2Data_Scheme.foregroundDisabled = 0x8410;
    JFLO_SpO2Data_Scheme.background = 0x7FF;
    JFLO_SpO2Data_Scheme.backgroundInactive = 0xD71C;
    JFLO_SpO2Data_Scheme.backgroundDisabled = 0xC67A;
    JFLO_SpO2Data_Scheme.text = 0x0;
    JFLO_SpO2Data_Scheme.textHighlight = 0x1F;
    JFLO_SpO2Data_Scheme.textHighlightText = 0xFFFF;
    JFLO_SpO2Data_Scheme.textInactive = 0xD71C;
    JFLO_SpO2Data_Scheme.textDisabled = 0x8C92;

    laScheme_Initialize(&JFLO_AlarmIndicatorLow2Priority_Scheme, GFX_COLOR_MODE_RGB_565);
    JFLO_AlarmIndicatorLow2Priority_Scheme.base = 0x7FF;
    JFLO_AlarmIndicatorLow2Priority_Scheme.highlight = 0xC67A;
    JFLO_AlarmIndicatorLow2Priority_Scheme.highlightLight = 0xFFFF;
    JFLO_AlarmIndicatorLow2Priority_Scheme.shadow = 0x8410;
    JFLO_AlarmIndicatorLow2Priority_Scheme.shadowDark = 0x4208;
    JFLO_AlarmIndicatorLow2Priority_Scheme.foreground = 0x7FF;
    JFLO_AlarmIndicatorLow2Priority_Scheme.foregroundInactive = 0xD71C;
    JFLO_AlarmIndicatorLow2Priority_Scheme.foregroundDisabled = 0x8410;
    JFLO_AlarmIndicatorLow2Priority_Scheme.background = 0xFFFF;
    JFLO_AlarmIndicatorLow2Priority_Scheme.backgroundInactive = 0xD71C;
    JFLO_AlarmIndicatorLow2Priority_Scheme.backgroundDisabled = 0xC67A;
    JFLO_AlarmIndicatorLow2Priority_Scheme.text = 0x0;
    JFLO_AlarmIndicatorLow2Priority_Scheme.textHighlight = 0x7FF;
    JFLO_AlarmIndicatorLow2Priority_Scheme.textHighlightText = 0xFFFF;
    JFLO_AlarmIndicatorLow2Priority_Scheme.textInactive = 0xD71C;
    JFLO_AlarmIndicatorLow2Priority_Scheme.textDisabled = 0x8C92;

    GFX_Set(GFXF_DRAW_PIPELINE_MODE, GFX_PIPELINE_GCUGPU);
    laContext_SetStringTable(&stringTable);

    laContext_SetStringLanguage(language_English);

    screen = laScreen_New(LA_FALSE, LA_FALSE, &ScreenCreate_UpdateScreen);
    laScreen_SetOrientation(screen, LA_SCREEN_ORIENTATION_270);
    laContext_AddScreen(screen);

    screen = laScreen_New(LA_FALSE, LA_FALSE, &ScreenCreate_VideoScreen);
    laScreen_SetOrientation(screen, LA_SCREEN_ORIENTATION_270);
    laContext_AddScreen(screen);

    screen = laScreen_New(LA_FALSE, LA_FALSE, &ScreenCreate_MainScreen);
    laScreen_SetOrientation(screen, LA_SCREEN_ORIENTATION_270);
    laContext_AddScreen(screen);

    screen = laScreen_New(LA_FALSE, LA_FALSE, &ScreenCreate_SettingScreen);
    laScreen_SetOrientation(screen, LA_SCREEN_ORIENTATION_270);
    laContext_AddScreen(screen);

    screen = laScreen_New(LA_FALSE, LA_FALSE, &ScreenCreate_MaintenanceScreen);
    laScreen_SetOrientation(screen, LA_SCREEN_ORIENTATION_270);
    laContext_AddScreen(screen);

    screen = laScreen_New(LA_FALSE, LA_FALSE, &ScreenCreate_PowerOffScreen);
    laScreen_SetOrientation(screen, LA_SCREEN_ORIENTATION_270);
    laContext_AddScreen(screen);

	return 0;
}

static void ScreenCreate_UpdateScreen(laScreen* screen)
{
    laLayer* layer0;

    laScreen_SetShowEventCallback(screen, &UpdateScreen_ShowEvent);
    laScreen_SetHideEventCallback(screen, &UpdateScreen_HideEvent);

    layer0 = laLayer_New();
    laWidget_SetPosition((laWidget*)layer0, 0, 0);
    laWidget_SetSize((laWidget*)layer0, 272, 480);
    laWidget_SetBackgroundType((laWidget*)layer0, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetScheme((laWidget*)layer0, &JFLO_Background_Scheme);
    laLayer_SetBufferCount(layer0, 2);

    laScreen_SetLayer(screen, 0, layer0);

    lbTitle_UpdateScreen = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbTitle_UpdateScreen, 0, 80);
    laWidget_SetSize((laWidget*)lbTitle_UpdateScreen, 270, 40);
    laWidget_SetScheme((laWidget*)lbTitle_UpdateScreen, &JFLO_Maintainance_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbTitle_UpdateScreen, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbTitle_UpdateScreen, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbTitle_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_SelectDeviceToUpdateFirmware));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lbTitle_UpdateScreen);

    btnMainboard_UpdateScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnMainboard_UpdateScreen, 9, 141);
    laWidget_SetSize((laWidget*)btnMainboard_UpdateScreen, 252, 60);
    laWidget_SetScheme((laWidget*)btnMainboard_UpdateScreen, &JFLO_Button_OnChangedSetpointArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnMainboard_UpdateScreen, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)btnMainboard_UpdateScreen, LA_WIDGET_BORDER_LINE);
    laButtonWidget_SetText(btnMainboard_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_Mainboard));
    laButtonWidget_SetPressedEventCallback(btnMainboard_UpdateScreen, &btnMainboard_UpdateScreen_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnMainboard_UpdateScreen);

    btnCancel_UpdateScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnCancel_UpdateScreen, 60, 361);
    laWidget_SetSize((laWidget*)btnCancel_UpdateScreen, 150, 39);
    laWidget_SetScheme((laWidget*)btnCancel_UpdateScreen, &JFLO_Button_OnChangedSetpointArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnCancel_UpdateScreen, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)btnCancel_UpdateScreen, LA_WIDGET_BORDER_LINE);
    laButtonWidget_SetText(btnCancel_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_Cancel));
    laButtonWidget_SetPressedEventCallback(btnCancel_UpdateScreen, &btnCancel_UpdateScreen_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnCancel_UpdateScreen);

    btnCradle_UpdateScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnCradle_UpdateScreen, 9, 281);
    laWidget_SetSize((laWidget*)btnCradle_UpdateScreen, 252, 60);
    laWidget_SetScheme((laWidget*)btnCradle_UpdateScreen, &JFLO_Button_OnChangedSetpointArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnCradle_UpdateScreen, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)btnCradle_UpdateScreen, LA_WIDGET_BORDER_LINE);
    laButtonWidget_SetText(btnCradle_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_Cradle));
    laButtonWidget_SetPressedEventCallback(btnCradle_UpdateScreen, &btnCradle_UpdateScreen_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnCradle_UpdateScreen);

    btnChamber_UpdateScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnChamber_UpdateScreen, 9, 211);
    laWidget_SetSize((laWidget*)btnChamber_UpdateScreen, 252, 60);
    laWidget_SetScheme((laWidget*)btnChamber_UpdateScreen, &JFLO_Button_OnChangedSetpointArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnChamber_UpdateScreen, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)btnChamber_UpdateScreen, LA_WIDGET_BORDER_LINE);
    laButtonWidget_SetText(btnChamber_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_Chamber));
    laButtonWidget_SetPressedEventCallback(btnChamber_UpdateScreen, &btnChamber_UpdateScreen_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnChamber_UpdateScreen);

    panelMessageBox_UpdateScreen = laWidget_New();
    laWidget_SetSize((laWidget*)panelMessageBox_UpdateScreen, 272, 480);
    laWidget_SetVisible((laWidget*)panelMessageBox_UpdateScreen, LA_FALSE);
    laWidget_SetBackgroundType((laWidget*)panelMessageBox_UpdateScreen, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)panelMessageBox_UpdateScreen, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelMessageBox_UpdateScreen);

    panelMessageBoxBackground_UpdateScreen = laWidget_New();
    laWidget_SetPosition((laWidget*)panelMessageBoxBackground_UpdateScreen, 21, 161);
    laWidget_SetSize((laWidget*)panelMessageBoxBackground_UpdateScreen, 230, 150);
    laWidget_SetScheme((laWidget*)panelMessageBoxBackground_UpdateScreen, &JFLO_MessageBox_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelMessageBoxBackground_UpdateScreen, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelMessageBoxBackground_UpdateScreen, LA_WIDGET_BORDER_LINE);
    laWidget_AddChild((laWidget*)panelMessageBox_UpdateScreen, panelMessageBoxBackground_UpdateScreen);

    lbMessage_UpdateScreen = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbMessage_UpdateScreen, 9, 10);
    laWidget_SetSize((laWidget*)lbMessage_UpdateScreen, 210, 71);
    laWidget_SetScheme((laWidget*)lbMessage_UpdateScreen, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbMessage_UpdateScreen, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)lbMessage_UpdateScreen, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbMessage_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_USBNotFound));
    laWidget_AddChild((laWidget*)panelMessageBoxBackground_UpdateScreen, (laWidget*)lbMessage_UpdateScreen);

    btnOK_UpdateScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnOK_UpdateScreen, 30, 90);
    laWidget_SetSize((laWidget*)btnOK_UpdateScreen, 170, 50);
    laWidget_SetScheme((laWidget*)btnOK_UpdateScreen, &JFLO_Button_OnChangedSetpointArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnOK_UpdateScreen, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)btnOK_UpdateScreen, LA_WIDGET_BORDER_LINE);
    laButtonWidget_SetText(btnOK_UpdateScreen, laString_CreateFromID(string_text_UpdateScreen_OK));
    laButtonWidget_SetPressedEventCallback(btnOK_UpdateScreen, &btnOK_UpdateScreen_PressedEvent);

    laWidget_AddChild((laWidget*)panelMessageBoxBackground_UpdateScreen, (laWidget*)btnOK_UpdateScreen);

}

static void ScreenCreate_VideoScreen(laScreen* screen)
{
    laLayer* layer0;

    laScreen_SetShowEventCallback(screen, &VideoScreen_ShowEvent);
    laScreen_SetHideEventCallback(screen, &VideoScreen_HideEvent);

    layer0 = laLayer_New();
    laWidget_SetPosition((laWidget*)layer0, 0, 0);
    laWidget_SetSize((laWidget*)layer0, 272, 480);
    laWidget_SetBackgroundType((laWidget*)layer0, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetScheme((laWidget*)layer0, &ClearScheme);
    laLayer_SetBufferCount(layer0, 2);
    laLayer_SetAlphaEnable(layer0, LA_TRUE);
    laLayer_SetAlphaAmount(layer0, 0xFF);

    laScreen_SetLayer(screen, 0, layer0);

    prgBarLoading = laProgressBarWidget_New();
    laWidget_SetPosition((laWidget*)prgBarLoading, 66, 260);
    laWidget_SetSize((laWidget*)prgBarLoading, 140, 5);
    laWidget_SetScheme((laWidget*)prgBarLoading, &JFLO_Slider_Scheme);
    laWidget_SetBackgroundType((laWidget*)prgBarLoading, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)prgBarLoading, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)prgBarLoading);

    txtInfo_Videoscreen = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)txtInfo_Videoscreen, 67, 205);
    laWidget_SetSize((laWidget*)txtInfo_Videoscreen, 143, 45);
    laWidget_SetVisible((laWidget*)txtInfo_Videoscreen, LA_FALSE);
    laWidget_SetScheme((laWidget*)txtInfo_Videoscreen, &BlackBackgroundScheme);
    laWidget_SetBackgroundType((laWidget*)txtInfo_Videoscreen, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)txtInfo_Videoscreen, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetText(txtInfo_Videoscreen, laString_CreateFromID(string_text_Uploading_VideoScreen));
    laTextFieldWidget_SetCursorEnabled(txtInfo_Videoscreen, LA_TRUE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)txtInfo_Videoscreen);
    
    panelSelfCheckIndicatorTop = laWidget_New();
    laWidget_SetSize((laWidget*)panelSelfCheckIndicatorTop, 272, 5);
    laWidget_SetScheme((laWidget*)panelSelfCheckIndicatorTop, &JFLO_CyanIndicator_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelSelfCheckIndicatorTop, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelSelfCheckIndicatorTop, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelSelfCheckIndicatorTop);

    panelSelfCheckIndicatorRight = laWidget_New();
    laWidget_SetPosition((laWidget*)panelSelfCheckIndicatorRight, 267, 5);
    laWidget_SetSize((laWidget*)panelSelfCheckIndicatorRight, 5, 470);
    laWidget_SetScheme((laWidget*)panelSelfCheckIndicatorRight, &JFLO_CyanIndicator_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelSelfCheckIndicatorRight, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelSelfCheckIndicatorRight, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelSelfCheckIndicatorRight);

    panelSelfCheckIndicatorBottom = laWidget_New();
    laWidget_SetPosition((laWidget*)panelSelfCheckIndicatorBottom, 0, 475);
    laWidget_SetSize((laWidget*)panelSelfCheckIndicatorBottom, 272, 5);
    laWidget_SetScheme((laWidget*)panelSelfCheckIndicatorBottom, &JFLO_CyanIndicator_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelSelfCheckIndicatorBottom, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelSelfCheckIndicatorBottom, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelSelfCheckIndicatorBottom);

    panelSelfCheckIndicatorLeft = laWidget_New();
    laWidget_SetPosition((laWidget*)panelSelfCheckIndicatorLeft, 0, 5);
    laWidget_SetSize((laWidget*)panelSelfCheckIndicatorLeft, 5, 470);
    laWidget_SetScheme((laWidget*)panelSelfCheckIndicatorLeft, &JFLO_CyanIndicator_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelSelfCheckIndicatorLeft, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelSelfCheckIndicatorLeft, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelSelfCheckIndicatorLeft);

}

static void ScreenCreate_MainScreen(laScreen* screen)
{
    laLayer* layer0;

    laScreen_SetShowEventCallback(screen, &MainScreen_ShowEvent);
    laScreen_SetHideEventCallback(screen, &MainScreen_HideEvent);

    layer0 = laLayer_New();
    laWidget_SetPosition((laWidget*)layer0, 0, 0);
    laWidget_SetSize((laWidget*)layer0, 272, 480);
    laWidget_SetBackgroundType((laWidget*)layer0, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetScheme((laWidget*)layer0, &JFLO_Background_Scheme);
    laLayer_SetBufferCount(layer0, 2);
    laLayer_SetAlphaEnable(layer0, LA_TRUE);
    laLayer_SetAlphaAmount(layer0, 0xFF);

    laScreen_SetLayer(screen, 0, layer0);
   
    imgWifi = laImageWidget_New();
    laWidget_SetPosition((laWidget*)imgWifi, 10, 42);
    laWidget_SetSize((laWidget*)imgWifi, 24, 24);
    laWidget_SetScheme((laWidget*)imgWifi, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)imgWifi, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)imgWifi, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(imgWifi, &iconWifi_1);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)imgWifi);

    tfCurrentTime = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)tfCurrentTime, 54, 45);
    laWidget_SetSize((laWidget*)tfCurrentTime, 45, 24);
    laWidget_SetScheme((laWidget*)tfCurrentTime, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)tfCurrentTime, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)tfCurrentTime, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetCursorEnabled(tfCurrentTime, LA_FALSE);
    laTextFieldWidget_SetClearOnFirstEdit(tfCurrentTime, LA_FALSE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)tfCurrentTime);

    btnInfo = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnInfo, 104, 42);
    laWidget_SetSize((laWidget*)btnInfo, 24, 24);
    laWidget_SetBackgroundType((laWidget*)btnInfo, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)btnInfo, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(btnInfo, &iconInfo);
    laButtonWidget_SetReleasedImage(btnInfo, &iconInfo);
    laButtonWidget_SetPressedEventCallback(btnInfo, &btnInfo_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnInfo);

    imgEXTBattery = laImageWidget_New();
    laWidget_SetPosition((laWidget*)imgEXTBattery, 141, 42);
    laWidget_SetSize((laWidget*)imgEXTBattery, 48, 24);
    laWidget_SetBackgroundType((laWidget*)imgEXTBattery, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)imgEXTBattery, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(imgEXTBattery, &iconExtBattery_4);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)imgEXTBattery);

    imgINTBattery = laImageWidget_New();
    laWidget_SetPosition((laWidget*)imgINTBattery, 193, 42);
    laWidget_SetSize((laWidget*)imgINTBattery, 45, 24);
    laWidget_SetBackgroundType((laWidget*)imgINTBattery, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)imgINTBattery, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(imgINTBattery, &Icon_InternalBatteryNoInserted);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)imgINTBattery);

    imgSocket = laImageWidget_New();
    laWidget_SetPosition((laWidget*)imgSocket, 240, 42);
    laWidget_SetSize((laWidget*)imgSocket, 24, 24);
    laWidget_SetScheme((laWidget*)imgSocket, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)imgSocket, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)imgSocket, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(imgSocket, &Icon_SocketDisconnected);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)imgSocket);

    panelBehindGraph = laWidget_New();
    laWidget_SetPosition((laWidget*)panelBehindGraph, 5, 420);
    laWidget_SetSize((laWidget*)panelBehindGraph, 260, 55);
    laWidget_SetOptimizationFlags((laWidget*)panelBehindGraph, LA_WIDGET_OPT_DRAW_ONCE);
    laWidget_SetScheme((laWidget*)panelBehindGraph, &JFLO_SpO2Graph_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelBehindGraph, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelBehindGraph, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelBehindGraph);

    graphHeartBeat = laLineGraphWidget_New();
    laWidget_SetSize((laWidget*)graphHeartBeat, 260, 55);
    laWidget_SetOptimizationFlags((laWidget*)graphHeartBeat, LA_WIDGET_OPT_LOCAL_REDRAW);
    laWidget_SetScheme((laWidget*)graphHeartBeat, &JFLO_SpO2Graph_Scheme);
    laWidget_SetBackgroundType((laWidget*)graphHeartBeat, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)graphHeartBeat, LA_WIDGET_BORDER_NONE);
    laLineGraphWidget_SetFillSeriesArea(graphHeartBeat, LA_FALSE);
    laLineGraphWidget_SetStacked(graphHeartBeat, LA_TRUE);
    laLineGraphWidget_SetValueAxisTickInterval(graphHeartBeat, 0, 50);
    laLineGraphWidget_SetValueAxisLabelsVisible(graphHeartBeat, 0, LA_FALSE);
    laLineGraphWidget_SetValueAxisTicksVisible(graphHeartBeat, 0, LA_FALSE);
    laLineGraphWidget_SetValueAxisSubticksVisible(graphHeartBeat, 0, LA_FALSE);
    laLineGraphWidget_SetGridlinesVisible(graphHeartBeat, 0, LA_FALSE);
    laLineGraphWidget_SetCategoryAxisLabelsVisible(graphHeartBeat, LA_FALSE);
    laLineGraphWidget_SetCategoryAxisTicksVisible(graphHeartBeat, LA_FALSE);
    laLineGraphWidget_AddSeries(graphHeartBeat, NULL);
    laLineGraphWidget_SetSeriesScheme(graphHeartBeat, 0, &JFLO_SpO2Graph_Scheme);
    laLineGraphWidget_SetSeriesPointType(graphHeartBeat, 0, LINE_GRAPH_DATA_POINT_NONE);
    laLineGraphWidget_SetSeriesPointSize(graphHeartBeat, 0, 2);
    laLineGraphWidget_SetSeriesFillPoints(graphHeartBeat, 0, LA_FALSE);
    laWidget_AddChild((laWidget*)panelBehindGraph, (laWidget*)graphHeartBeat);

    lbSpO2text = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbSpO2text, 6, 392);
    laWidget_SetSize((laWidget*)lbSpO2text, 50, 25);
    laWidget_SetScheme((laWidget*)lbSpO2text, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbSpO2text, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)lbSpO2text, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbSpO2text, laString_CreateFromID(string_text_SpO2_MainScreen));
    laLabelWidget_SetHAlignment(lbSpO2text, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lbSpO2text);

    tfSpO2Value = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)tfSpO2Value, 52, 368);
    laWidget_SetSize((laWidget*)tfSpO2Value, 56, 56);
    laWidget_SetVisible((laWidget*)tfSpO2Value, LA_FALSE);
    laWidget_SetOptimizationFlags((laWidget*)tfSpO2Value, LA_WIDGET_OPT_LOCAL_REDRAW);
    laWidget_SetScheme((laWidget*)tfSpO2Value, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)tfSpO2Value, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)tfSpO2Value, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetText(tfSpO2Value, laString_CreateFromID(string_Nums_BebasNeueBook_S60_Nor));
    laTextFieldWidget_SetCursorEnabled(tfSpO2Value, LA_FALSE);
    laTextFieldWidget_SetClearOnFirstEdit(tfSpO2Value, LA_FALSE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)tfSpO2Value);

    lbSpO2Unit = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbSpO2Unit, 101, 390);
    laWidget_SetSize((laWidget*)lbSpO2Unit, 21, 25);
    laWidget_SetScheme((laWidget*)lbSpO2Unit, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbSpO2Unit, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)lbSpO2Unit, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbSpO2Unit, laString_CreateFromID(string_SpO2UnitText_MainScreen));
    laLabelWidget_SetHAlignment(lbSpO2Unit, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lbSpO2Unit);

    lbPRtext = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbPRtext, 148, 390);
    laWidget_SetSize((laWidget*)lbPRtext, 25, 20);
    laWidget_SetScheme((laWidget*)lbPRtext, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbPRtext, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)lbPRtext, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbPRtext, laString_CreateFromID(string_text_PR_MainScreen));
    laLabelWidget_SetHAlignment(lbPRtext, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lbPRtext);

    tfPrValue = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)tfPrValue, 175, 376);
    laWidget_SetSize((laWidget*)tfPrValue, 55, 45);
    laWidget_SetVisible((laWidget*)tfPrValue, LA_FALSE);
    laWidget_SetOptimizationFlags((laWidget*)tfPrValue, LA_WIDGET_OPT_LOCAL_REDRAW);
    laWidget_SetScheme((laWidget*)tfPrValue, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)tfPrValue, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)tfPrValue, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetText(tfPrValue, laString_CreateFromID(string_Nums_BebasNeueBook_S45_Nor));
    laTextFieldWidget_SetAlignment(tfPrValue, LA_HALIGN_CENTER);
    laTextFieldWidget_SetCursorEnabled(tfPrValue, LA_FALSE);
    laTextFieldWidget_SetClearOnFirstEdit(tfPrValue, LA_FALSE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)tfPrValue);

    lbPrUnit = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbPrUnit, 227, 390);
    laWidget_SetSize((laWidget*)lbPrUnit, 30, 25);
    laWidget_SetScheme((laWidget*)lbPrUnit, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbPrUnit, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)lbPrUnit, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbPrUnit, laString_CreateFromID(string_PRUnitText_MainScreen));
    laLabelWidget_SetHAlignment(lbPrUnit, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lbPrUnit);
    
    rectChangedSetpointArea = jfloRectangleWidget_New();
    laWidget_SetPosition((laWidget*)rectChangedSetpointArea, 305, 78);
    laWidget_SetSize((laWidget*)rectChangedSetpointArea, 260, 275);
    laWidget_SetCornerRadius((laWidget*)rectChangedSetpointArea, 30);      
    laWidget_SetScheme((laWidget*)rectChangedSetpointArea, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)rectChangedSetpointArea, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)rectChangedSetpointArea, LA_WIDGET_BORDER_NONE);
    laWidget_SetMargins((laWidget*)rectChangedSetpointArea, 0, 0, 0, 0);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)rectChangedSetpointArea);

    rectChangedSetpointArea_2 = jfloRectangleWidget_New();
    laWidget_SetPosition((laWidget*)rectChangedSetpointArea_2, 70, 8);
    laWidget_SetSize((laWidget*)rectChangedSetpointArea_2, 152, 258);
    laWidget_SetCornerRadius((laWidget*)rectChangedSetpointArea_2, 10);    
    laWidget_SetScheme((laWidget*)rectChangedSetpointArea_2, &JFLO_RectChangedSetpoint_2_Scheme);
    laWidget_SetBackgroundType((laWidget*)rectChangedSetpointArea_2, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)rectChangedSetpointArea_2, LA_WIDGET_BORDER_NONE);
    jfloRectangleWidget_SetThickness(rectChangedSetpointArea_2 , 2);
    laWidget_AddChild((laWidget*)rectChangedSetpointArea, (laWidget*)rectChangedSetpointArea_2);
    
    tfCurrentTempSetting = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)tfCurrentTempSetting, 30, 52);
    laWidget_SetSize((laWidget*)tfCurrentTempSetting, 30, 34);
    laWidget_SetScheme((laWidget*)tfCurrentTempSetting, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)tfCurrentTempSetting, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)tfCurrentTempSetting, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetText(tfCurrentTempSetting, laString_CreateFromID(string_text_Dash_S32));
    laTextFieldWidget_SetCursorEnabled(tfCurrentTempSetting, LA_TRUE);
    laTextFieldWidget_SetClearOnFirstEdit(tfCurrentTempSetting, LA_FALSE);
    laWidget_AddChild((laWidget*)rectChangedSetpointArea, (laWidget*)tfCurrentTempSetting);

    tfCurrentFlowSetting = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)tfCurrentFlowSetting, 30, 142);
    laWidget_SetSize((laWidget*)tfCurrentFlowSetting, 30, 34);
    laWidget_SetScheme((laWidget*)tfCurrentFlowSetting, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)tfCurrentFlowSetting, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)tfCurrentFlowSetting, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetText(tfCurrentFlowSetting, laString_CreateFromID(string_text_Dash_S32));
    laTextFieldWidget_SetCursorEnabled(tfCurrentFlowSetting, LA_TRUE);
    laTextFieldWidget_SetClearOnFirstEdit(tfCurrentFlowSetting, LA_FALSE);
    laWidget_AddChild((laWidget*)rectChangedSetpointArea, (laWidget*)tfCurrentFlowSetting);

    tfCurrentO2Setting = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)tfCurrentO2Setting, 30, 226);
    laWidget_SetSize((laWidget*)tfCurrentO2Setting, 30, 34);
    laWidget_SetScheme((laWidget*)tfCurrentO2Setting, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)tfCurrentO2Setting, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)tfCurrentO2Setting, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetText(tfCurrentO2Setting, laString_CreateFromID(string_text_Dash_S32));
    laTextFieldWidget_SetCursorEnabled(tfCurrentO2Setting, LA_TRUE);
    laTextFieldWidget_SetClearOnFirstEdit(tfCurrentO2Setting, LA_FALSE);
    laWidget_AddChild((laWidget*)rectChangedSetpointArea, (laWidget*)tfCurrentO2Setting);
    
    imgIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)imgIcon, 95, 43);
    laWidget_SetSize((laWidget*)imgIcon, 25, 44);
    laWidget_SetScheme((laWidget*)imgIcon, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)imgIcon, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)imgIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(imgIcon, &iconTemp);
    laWidget_AddChild((laWidget*)rectChangedSetpointArea, (laWidget*)imgIcon);
    
    lbO2Up_ChangedSetpointArea = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbO2Up_ChangedSetpointArea, 180, 32);
    laWidget_SetSize((laWidget*)lbO2Up_ChangedSetpointArea, 25, 20);
    laWidget_SetScheme((laWidget*)lbO2Up_ChangedSetpointArea, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbO2Up_ChangedSetpointArea, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbO2Up_ChangedSetpointArea, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbO2Up_ChangedSetpointArea, laString_CreateFromID(string_text_Dash_S18));
    laLabelWidget_SetHAlignment(lbO2Up_ChangedSetpointArea, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)rectChangedSetpointArea, (laWidget*)lbO2Up_ChangedSetpointArea);

    lbO2Down_ChangedSetpointArea = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbO2Down_ChangedSetpointArea, 180, 75);
    laWidget_SetSize((laWidget*)lbO2Down_ChangedSetpointArea, 25, 20);
    laWidget_SetScheme((laWidget*)lbO2Down_ChangedSetpointArea, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbO2Down_ChangedSetpointArea, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbO2Down_ChangedSetpointArea, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbO2Down_ChangedSetpointArea, laString_CreateFromID(string_text_Dash_S18));
    laLabelWidget_SetHAlignment(lbO2Down_ChangedSetpointArea, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)rectChangedSetpointArea, (laWidget*)lbO2Down_ChangedSetpointArea);

    tfValue = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)tfValue, 120, 32);
    laWidget_SetSize((laWidget*)tfValue, 60, 74);
    laWidget_SetEnabled((laWidget*)tfValue, LA_FALSE);
    laWidget_SetScheme((laWidget*)tfValue, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)tfValue, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)tfValue, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetText(tfValue, laString_CreateFromID(string_text_Dash_S70));
    laTextFieldWidget_SetCursorEnabled(tfValue, LA_TRUE);
    laTextFieldWidget_SetClearOnFirstEdit(tfValue, LA_FALSE);
    laWidget_AddChild((laWidget*)rectChangedSetpointArea, (laWidget*)tfValue);
    
    lbO2_ChangedSetpointArea = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbO2_ChangedSetpointArea, 75, 104);
    laWidget_SetSize((laWidget*)lbO2_ChangedSetpointArea, 130, 80);
    laWidget_SetScheme((laWidget*)lbO2_ChangedSetpointArea, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbO2_ChangedSetpointArea, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)lbO2_ChangedSetpointArea, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbO2_ChangedSetpointArea, laString_CreateFromID(string_text_Nullstring));
    laLabelWidget_SetHAlignment(lbO2_ChangedSetpointArea, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)rectChangedSetpointArea, (laWidget*)lbO2_ChangedSetpointArea);
    
    btnMinus = jfloButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnMinus, 79, 119);
    laWidget_SetSize((laWidget*)btnMinus, 60, 60);
    _jfloButtonWidget_SetCornerRadius(btnMinus, 30);
    laWidget_SetScheme((laWidget*)btnMinus, &JFLO_Button_OnChangedSetpointArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnMinus, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnMinus, LA_WIDGET_BORDER_LINE);
    jfloButtonWidget_SetPressedImage(btnMinus, &iconMinus);
    jfloButtonWidget_SetReleasedImage(btnMinus, &iconMinus);
    jfloButtonWidget_SetPressedEventCallback(btnMinus, &btnMinus_PressedEvent);

    laWidget_AddChild((laWidget*)rectChangedSetpointArea, (laWidget*)btnMinus);

    btnBack = jfloButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnBack, 79, 190);
    laWidget_SetSize((laWidget*)btnBack, 60, 60);
    _jfloButtonWidget_SetCornerRadius(btnBack, 30);
    laWidget_SetScheme((laWidget*)btnBack, &JFLO_Button_OnChangedSetpointArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnBack, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnBack, LA_WIDGET_BORDER_LINE);
    jfloButtonWidget_SetPressedImage(btnBack, &iconBack);
    jfloButtonWidget_SetReleasedImage(btnBack, &iconBack);
    jfloButtonWidget_SetPressedEventCallback(btnBack, &btnBack_PressedEvent);

    laWidget_AddChild((laWidget*)rectChangedSetpointArea, (laWidget*)btnBack);

    btnPlus = jfloButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnPlus, 149, 119);
    laWidget_SetSize((laWidget*)btnPlus, 60, 60);
    _jfloButtonWidget_SetCornerRadius(btnPlus, 30);
    laWidget_SetScheme((laWidget*)btnPlus, &JFLO_Button_OnChangedSetpointArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnPlus, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnPlus, LA_WIDGET_BORDER_LINE);
    jfloButtonWidget_SetPressedImage(btnPlus, &iconPlus);
    jfloButtonWidget_SetReleasedImage(btnPlus, &iconPlus);
    jfloButtonWidget_SetPressedEventCallback(btnPlus, &btnPlus_PressedEvent);

    laWidget_AddChild((laWidget*)rectChangedSetpointArea, (laWidget*)btnPlus);

    btnOK = jfloButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnOK, 149, 190);
    laWidget_SetSize((laWidget*)btnOK, 60, 60);
    _jfloButtonWidget_SetCornerRadius(btnOK, 30);
    laWidget_SetScheme((laWidget*)btnOK, &JFLO_Button_OnChangedSetpointArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnOK, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnOK, LA_WIDGET_BORDER_LINE);
    jfloButtonWidget_SetText(btnOK, laString_CreateFromID(string_text_OK_ChangedSetpointArea));
    jfloButtonWidget_SetPressedEventCallback(btnOK, &btnOK_PressedEvent);

    laWidget_AddChild((laWidget*)rectChangedSetpointArea, (laWidget*)btnOK);

    iconBidirectionArrow = laImageWidget_New();
    laWidget_SetPosition((laWidget*)iconBidirectionArrow, 175, 57);
    laWidget_SetSize((laWidget*)iconBidirectionArrow, 27, 15);
    laWidget_SetBackgroundType((laWidget*)iconBidirectionArrow, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)iconBidirectionArrow, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(iconBidirectionArrow, &Icon_BidirectionArrowVertical);
    laWidget_AddChild((laWidget*)rectChangedSetpointArea, (laWidget*)iconBidirectionArrow);

    imgBlueline = laImageWidget_New();
    laWidget_SetPosition((laWidget*)imgBlueline, 0, 96);
    laWidget_SetSize((laWidget*)imgBlueline, 6, 64);
    laWidget_SetVisible((laWidget*)imgBlueline, LA_FALSE);
    laWidget_SetBackgroundType((laWidget*)imgBlueline, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)imgBlueline, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(imgBlueline, &iconBlueline);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)imgBlueline);

    rectMonitoringArea = jfloRectangleWidget_New();
    laWidget_SetPosition((laWidget*)rectMonitoringArea, 12, 77);
    laWidget_SetSize((laWidget*)rectMonitoringArea, 260, 275);
    laWidget_SetCornerRadius((laWidget*)rectMonitoringArea, 30);
    laWidget_SetScheme((laWidget*)rectMonitoringArea, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)rectMonitoringArea, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)rectMonitoringArea, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)rectMonitoringArea);

    lbTempUnit = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbTempUnit, 188, 31);
    laWidget_SetSize((laWidget*)lbTempUnit, 59, 40);
    laWidget_SetScheme((laWidget*)lbTempUnit, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbTempUnit, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbTempUnit, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbTempUnit, laString_CreateFromID(string_TempUnit_MainScreen));
    laLabelWidget_SetHAlignment(lbTempUnit, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)rectMonitoringArea, (laWidget*)lbTempUnit);

    lbFlowUnit = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbFlowUnit, 188, 123);
    laWidget_SetSize((laWidget*)lbFlowUnit, 58, 37);
    laWidget_SetScheme((laWidget*)lbFlowUnit, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbFlowUnit, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbFlowUnit, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbFlowUnit, laString_CreateFromID(string_FlowUnit_MainScreen));
    laWidget_AddChild((laWidget*)rectMonitoringArea, (laWidget*)lbFlowUnit);

    lbO2Unit = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbO2Unit, 193, 204);
    laWidget_SetSize((laWidget*)lbO2Unit, 34, 36);
    laWidget_SetScheme((laWidget*)lbO2Unit, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbO2Unit, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbO2Unit, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbO2Unit, laString_CreateFromID(string_O2Unit_MainScreen));
    laWidget_AddChild((laWidget*)rectMonitoringArea, (laWidget*)lbO2Unit);

    tfCurrentTempValue = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)tfCurrentTempValue, 94, 17);
    laWidget_SetSize((laWidget*)tfCurrentTempValue, 90, 80);
    laWidget_SetEnabled((laWidget*)tfCurrentTempValue, LA_FALSE);
    laWidget_SetScheme((laWidget*)tfCurrentTempValue, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)tfCurrentTempValue, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)tfCurrentTempValue, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetText(tfCurrentTempValue, laString_CreateFromID(string_text_Dash_S83));
    laTextFieldWidget_SetAlignment(tfCurrentTempValue, LA_HALIGN_CENTER);
    laTextFieldWidget_SetCursorEnabled(tfCurrentTempValue, LA_TRUE);
    laTextFieldWidget_SetClearOnFirstEdit(tfCurrentTempValue, LA_FALSE);
    laWidget_AddChild((laWidget*)rectMonitoringArea, (laWidget*)tfCurrentTempValue);

    tfCurrentFlowValue = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)tfCurrentFlowValue, 94, 103);
    laWidget_SetSize((laWidget*)tfCurrentFlowValue, 90, 80);
    laWidget_SetScheme((laWidget*)tfCurrentFlowValue, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)tfCurrentFlowValue, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)tfCurrentFlowValue, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetText(tfCurrentFlowValue, laString_CreateFromID(string_text_Dash_S83));
    laTextFieldWidget_SetAlignment(tfCurrentFlowValue, LA_HALIGN_CENTER);
    laTextFieldWidget_SetCursorEnabled(tfCurrentFlowValue, LA_TRUE);
    laTextFieldWidget_SetClearOnFirstEdit(tfCurrentFlowValue, LA_FALSE);
    laWidget_AddChild((laWidget*)rectMonitoringArea, (laWidget*)tfCurrentFlowValue);

    tfCurrentO2Value = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)tfCurrentO2Value, 94, 189);
    laWidget_SetSize((laWidget*)tfCurrentO2Value, 90, 80);
    laWidget_SetScheme((laWidget*)tfCurrentO2Value, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)tfCurrentO2Value, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)tfCurrentO2Value, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetText(tfCurrentO2Value, laString_CreateFromID(string_text_Dash_S83));
    laTextFieldWidget_SetAlignment(tfCurrentO2Value, LA_HALIGN_CENTER);
    laTextFieldWidget_SetCursorEnabled(tfCurrentO2Value, LA_TRUE);
    laTextFieldWidget_SetClearOnFirstEdit(tfCurrentO2Value, LA_FALSE);
    laWidget_AddChild((laWidget*)rectMonitoringArea, (laWidget*)tfCurrentO2Value);

    btnTemp_SelectToSetting_1 = jfloButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnTemp_SelectToSetting_1, 24, 21);
    laWidget_SetSize((laWidget*)btnTemp_SelectToSetting_1, 60, 60);
    _jfloButtonWidget_SetCornerRadius(btnTemp_SelectToSetting_1, 26);
    laWidget_SetScheme((laWidget*)btnTemp_SelectToSetting_1, &JFLO_Button_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnTemp_SelectToSetting_1, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnTemp_SelectToSetting_1, LA_WIDGET_BORDER_NONE);
    jfloButtonWidget_SetPressedImage(btnTemp_SelectToSetting_1, &iconTemp);
    jfloButtonWidget_SetReleasedImage(btnTemp_SelectToSetting_1, &iconTemp);
    jfloButtonWidget_SetImagePosition(btnTemp_SelectToSetting_1, LA_RELATIVE_POSITION_BELOW);
    jfloButtonWidget_SetPressedEventCallback(btnTemp_SelectToSetting_1, &btnTemp_SelectToSetting_1_PressedEvent);
    jfloButtonWidget_SetReleasedEventCallback(btnTemp_SelectToSetting_1, &btnTemp_SelectToSetting_1_ReleasedEvent);

    laWidget_AddChild((laWidget*)rectMonitoringArea, (laWidget*)btnTemp_SelectToSetting_1);

    arcTemp_SelectToSetting_1 = laArcWidget_New();
    laWidget_SetSize((laWidget*)arcTemp_SelectToSetting_1, 60, 60);
    laWidget_SetScheme((laWidget*)arcTemp_SelectToSetting_1, &JFLO_Slider_Scheme);
    laWidget_SetBackgroundType((laWidget*)arcTemp_SelectToSetting_1, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)arcTemp_SelectToSetting_1, LA_WIDGET_BORDER_NONE);
    laArcWidget_SetRadius(arcTemp_SelectToSetting_1, 30);
    laArcWidget_SetStartAngle(arcTemp_SelectToSetting_1, 90);
    laArcWidget_SetCenterAngle(arcTemp_SelectToSetting_1, -360);
    laArcWidget_SetThickness(arcTemp_SelectToSetting_1, 2);
    laArcWidget_SetRoundEdge(arcTemp_SelectToSetting_1, LA_FALSE);
    laWidget_AddChild((laWidget*)btnTemp_SelectToSetting_1, (laWidget*)arcTemp_SelectToSetting_1);
    
    btnFlow_SelectToSetting_1 = jfloButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnFlow_SelectToSetting_1, 24, 107);
    laWidget_SetSize((laWidget*)btnFlow_SelectToSetting_1, 60, 60);
    _jfloButtonWidget_SetCornerRadius(btnFlow_SelectToSetting_1, 26);
    laWidget_SetScheme((laWidget*)btnFlow_SelectToSetting_1, &JFLO_Button_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnFlow_SelectToSetting_1, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnFlow_SelectToSetting_1, LA_WIDGET_BORDER_NONE);
    jfloButtonWidget_SetPressedImage(btnFlow_SelectToSetting_1, &iconFlow);
    jfloButtonWidget_SetReleasedImage(btnFlow_SelectToSetting_1, &iconFlow);
    jfloButtonWidget_SetPressedEventCallback(btnFlow_SelectToSetting_1, &btnFlow_SelectToSetting_1_PressedEvent);
    jfloButtonWidget_SetReleasedEventCallback(btnFlow_SelectToSetting_1, &btnFlow_SelectToSetting_1_ReleasedEvent);
    
    laWidget_AddChild((laWidget*)rectMonitoringArea, (laWidget*)btnFlow_SelectToSetting_1);

    arcFlow_SelectToSetting_1 = laArcWidget_New();
    laWidget_SetSize((laWidget*)arcFlow_SelectToSetting_1, 60, 60);
    laWidget_SetScheme((laWidget*)arcFlow_SelectToSetting_1, &JFLO_Slider_Scheme);
    laWidget_SetBackgroundType((laWidget*)arcFlow_SelectToSetting_1, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)arcFlow_SelectToSetting_1, LA_WIDGET_BORDER_NONE);
    laArcWidget_SetRadius(arcFlow_SelectToSetting_1, 30);
    laArcWidget_SetStartAngle(arcFlow_SelectToSetting_1, 90);
    laArcWidget_SetCenterAngle(arcFlow_SelectToSetting_1, -360);
    laArcWidget_SetThickness(arcFlow_SelectToSetting_1, 2);
    laArcWidget_SetRoundEdge(arcFlow_SelectToSetting_1, LA_FALSE);
    laWidget_AddChild((laWidget*)btnFlow_SelectToSetting_1, (laWidget*)arcFlow_SelectToSetting_1);

    btnO2_SelectToSetting_1 = jfloButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnO2_SelectToSetting_1, 24, 193);
    laWidget_SetSize((laWidget*)btnO2_SelectToSetting_1, 60, 60);
    _jfloButtonWidget_SetCornerRadius(btnO2_SelectToSetting_1, 26);
    laWidget_SetScheme((laWidget*)btnO2_SelectToSetting_1, &JFLO_Button_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnO2_SelectToSetting_1, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnO2_SelectToSetting_1, LA_WIDGET_BORDER_NONE);
    jfloButtonWidget_SetPressedImage(btnO2_SelectToSetting_1, &iconO2);
    jfloButtonWidget_SetReleasedImage(btnO2_SelectToSetting_1, &iconO2);
    jfloButtonWidget_SetPressedEventCallback(btnO2_SelectToSetting_1, &btnO2_SelectToSetting_1_PressedEvent);
    jfloButtonWidget_SetReleasedEventCallback(btnO2_SelectToSetting_1, &btnO2_SelectToSetting_1_ReleasedEvent);
    
    laWidget_AddChild((laWidget*)rectMonitoringArea, (laWidget*)btnO2_SelectToSetting_1);

    arcO2_SelectToSetting_1 = laArcWidget_New();
    laWidget_SetSize((laWidget*)arcO2_SelectToSetting_1, 60, 60);
    laWidget_SetScheme((laWidget*)arcO2_SelectToSetting_1, &JFLO_Slider_Scheme);
    laWidget_SetBackgroundType((laWidget*)arcO2_SelectToSetting_1, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)arcO2_SelectToSetting_1, LA_WIDGET_BORDER_NONE);
    laArcWidget_SetRadius(arcO2_SelectToSetting_1, 30);
    laArcWidget_SetStartAngle(arcO2_SelectToSetting_1, 90);
    laArcWidget_SetCenterAngle(arcO2_SelectToSetting_1, 360);
    laArcWidget_SetThickness(arcO2_SelectToSetting_1, 2);
    laArcWidget_SetRoundEdge(arcO2_SelectToSetting_1, LA_TRUE);
    laWidget_AddChild((laWidget*)btnO2_SelectToSetting_1, (laWidget*)arcO2_SelectToSetting_1);
     
    panelNotice = laWidget_New();
    laWidget_SetSize((laWidget*)panelNotice, 272, 33);
    laWidget_SetScheme((laWidget*)panelNotice, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelNotice, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelNotice, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelNotice);

    lbInfo_NoticeArea = laLabelWidget_New();
    laWidget_SetSize((laWidget*)lbInfo_NoticeArea, 272, 33);
    laWidget_SetScheme((laWidget*)lbInfo_NoticeArea, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbInfo_NoticeArea, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbInfo_NoticeArea, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)panelNotice, (laWidget*)lbInfo_NoticeArea);
    
    rectAlarmArea = jfloRectangleWidget_New();
    laWidget_SetPosition((laWidget*)rectAlarmArea, 305, 78);
    laWidget_SetSize((laWidget*)rectAlarmArea, 260, 275);
    laWidget_SetCornerRadius((laWidget*)rectAlarmArea, 30);
    laWidget_SetScheme((laWidget*)rectAlarmArea, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)rectAlarmArea, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)rectAlarmArea, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)rectAlarmArea);

    lbAlarmInfomation = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbAlarmInfomation, 9, 1);//26
    laWidget_SetSize((laWidget*)lbAlarmInfomation, 230, 188);
    laWidget_SetScheme((laWidget*)lbAlarmInfomation, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbAlarmInfomation, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbAlarmInfomation, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetHAlignment(lbAlarmInfomation, LA_HALIGN_CENTER);
    laWidget_AddChild((laWidget*)rectAlarmArea, (laWidget*)lbAlarmInfomation);

    btnX_AlarmArea = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnX_AlarmArea, 48, 220);
    laWidget_SetSize((laWidget*)btnX_AlarmArea, 71, 52);
    laWidget_SetScheme((laWidget*)btnX_AlarmArea, &JFLO_Button_OnChangedSetpointArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnX_AlarmArea, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnX_AlarmArea, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(btnX_AlarmArea, &ButtonRound_Back);
    laButtonWidget_SetReleasedImage(btnX_AlarmArea, &ButtonRound_Back);
    laButtonWidget_SetPressedEventCallback(btnX_AlarmArea, &btnX_AlarmArea_PressedEvent);

    laWidget_AddChild((laWidget*)rectAlarmArea, (laWidget*)btnX_AlarmArea);

    btnAlarmReset_AlarmArea = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnAlarmReset_AlarmArea, 128, 220);
    laWidget_SetSize((laWidget*)btnAlarmReset_AlarmArea, 71, 52);
    laWidget_SetScheme((laWidget*)btnAlarmReset_AlarmArea, &JFLO_Button_OnChangedSetpointArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnAlarmReset_AlarmArea, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnAlarmReset_AlarmArea, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(btnAlarmReset_AlarmArea, &ButtonRound_Reset);
    laButtonWidget_SetReleasedImage(btnAlarmReset_AlarmArea, &ButtonRound_Reset);
    laButtonWidget_SetPressedEventCallback(btnAlarmReset_AlarmArea, &btnAlarmReset_AlarmArea_PressedEvent);

    laWidget_AddChild((laWidget*)rectAlarmArea, (laWidget*)btnAlarmReset_AlarmArea);
    
    btnTemp_SelectToSetting_2 = jfloButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnTemp_SelectToSetting_2, 275, 98);
    laWidget_SetSize((laWidget*)btnTemp_SelectToSetting_2, 66, 60);
    _jfloButtonWidget_SetCornerRadius(btnTemp_SelectToSetting_2, 30);
    laWidget_SetScheme((laWidget*)btnTemp_SelectToSetting_2, &JFLO_Button_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnTemp_SelectToSetting_2, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnTemp_SelectToSetting_2, LA_WIDGET_BORDER_LINE);
    jfloButtonWidget_SetPressedImage(btnTemp_SelectToSetting_2, &iconTemp);
    jfloButtonWidget_SetReleasedImage(btnTemp_SelectToSetting_2, &iconTemp);
    jfloButtonWidget_SetImageMargin(btnTemp_SelectToSetting_2, 0);
    jfloButtonWidget_SetPressedEventCallback(btnTemp_SelectToSetting_2, &btnTemp_SelectToSetting_2_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnTemp_SelectToSetting_2);

    btnFlow_SelectToSetting_2 = jfloButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnFlow_SelectToSetting_2, 275, 184);
    laWidget_SetSize((laWidget*)btnFlow_SelectToSetting_2, 66, 60);
    _jfloButtonWidget_SetCornerRadius(btnFlow_SelectToSetting_2, 30);
    laWidget_SetScheme((laWidget*)btnFlow_SelectToSetting_2, &JFLO_Button_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnFlow_SelectToSetting_2, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnFlow_SelectToSetting_2, LA_WIDGET_BORDER_LINE);
    jfloButtonWidget_SetPressedImage(btnFlow_SelectToSetting_2, &iconFlow);
    jfloButtonWidget_SetReleasedImage(btnFlow_SelectToSetting_2, &iconFlow);
    jfloButtonWidget_SetImageMargin(btnFlow_SelectToSetting_2, 0);
    jfloButtonWidget_SetPressedEventCallback(btnFlow_SelectToSetting_2, &btnFlow_SelectToSetting_2_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnFlow_SelectToSetting_2);

    btnO2_SelectToSetting_2 = jfloButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnO2_SelectToSetting_2, 275, 270);
    laWidget_SetSize((laWidget*)btnO2_SelectToSetting_2, 66, 60);
    _jfloButtonWidget_SetCornerRadius(btnO2_SelectToSetting_2, 30);
    laWidget_SetScheme((laWidget*)btnO2_SelectToSetting_2, &JFLO_Button_Scheme);
    laWidget_SetBackgroundType((laWidget*)btnO2_SelectToSetting_2, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnO2_SelectToSetting_2, LA_WIDGET_BORDER_LINE);
    jfloButtonWidget_SetPressedImage(btnO2_SelectToSetting_2, &iconO2);
    jfloButtonWidget_SetReleasedImage(btnO2_SelectToSetting_2, &iconO2);
    jfloButtonWidget_SetImageMargin(btnO2_SelectToSetting_2, 0);
    jfloButtonWidget_SetPressedEventCallback(btnO2_SelectToSetting_2, &btnO2_SelectToSetting_2_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnO2_SelectToSetting_2);

    panelLeftTouch = laWidget_New();
    laWidget_SetPosition((laWidget*)panelLeftTouch, 0, 381);
    laWidget_SetSize((laWidget*)panelLeftTouch, 100, 100);
    laWidget_SetOptimizationFlags((laWidget*)panelLeftTouch, LA_WIDGET_OPT_DRAW_ONCE);
    laWidget_SetBackgroundType((laWidget*)panelLeftTouch, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)panelLeftTouch, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelLeftTouch);

    panelRightTouch = laWidget_New();
    laWidget_SetPosition((laWidget*)panelRightTouch, 172, 381);
    laWidget_SetSize((laWidget*)panelRightTouch, 100, 100);
    laWidget_SetOptimizationFlags((laWidget*)panelRightTouch, LA_WIDGET_OPT_DRAW_ONCE);
    laWidget_SetBackgroundType((laWidget*)panelRightTouch, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)panelRightTouch, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelRightTouch);
    
    panelLeftAlarm = laWidget_New();
    laWidget_SetPosition((laWidget*)panelLeftAlarm, 0, 33);
    laWidget_SetSize((laWidget*)panelLeftAlarm, 5, 447);
    laWidget_SetOptimizationFlags((laWidget*)panelLeftAlarm, LA_WIDGET_OPT_LOCAL_REDRAW);
    laWidget_SetScheme((laWidget*)panelLeftAlarm, &JFLO_AlarmIndicatorMediumPriority_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelLeftAlarm, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelLeftAlarm, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelLeftAlarm);

    panelRightAlarm = laWidget_New();
    laWidget_SetPosition((laWidget*)panelRightAlarm, 267, 33);
    laWidget_SetSize((laWidget*)panelRightAlarm, 5, 447);
    laWidget_SetOptimizationFlags((laWidget*)panelRightAlarm, LA_WIDGET_OPT_LOCAL_REDRAW);
    laWidget_SetScheme((laWidget*)panelRightAlarm, &JFLO_AlarmIndicatorMediumPriority_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelRightAlarm, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelRightAlarm, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelRightAlarm);

    panelBottomAlarm = laWidget_New();
    laWidget_SetPosition((laWidget*)panelBottomAlarm, 5, 475);
    laWidget_SetSize((laWidget*)panelBottomAlarm, 262, 5);
    laWidget_SetOptimizationFlags((laWidget*)panelBottomAlarm, LA_WIDGET_OPT_LOCAL_REDRAW);
    laWidget_SetScheme((laWidget*)panelBottomAlarm, &JFLO_AlarmIndicatorHighPriority_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelBottomAlarm, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelBottomAlarm, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelBottomAlarm);

    panelAlarmTitle = laWidget_New();
    laWidget_SetSize((laWidget*)panelAlarmTitle, 272, 33);
    laWidget_SetVisible((laWidget*)panelAlarmTitle, LA_FALSE);
    laWidget_SetScheme((laWidget*)panelAlarmTitle, &JFLO_AlarmIndicatorHighPriority_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelAlarmTitle, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelAlarmTitle, LA_WIDGET_BORDER_NONE);
    laWidget_OverrideTouchDownEvent((laWidget*)panelAlarmTitle, &panelAlarmTitle_PressedEvent);
    laWidget_AddChild((laWidget*)layer0, panelAlarmTitle);

    labelAlarmTitle = laLabelWidget_New();
    laWidget_SetSize((laWidget*)labelAlarmTitle, 272, 33);
    laWidget_SetBackgroundType((laWidget*)labelAlarmTitle, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)labelAlarmTitle, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)panelAlarmTitle, (laWidget*)labelAlarmTitle);

    imageMuteIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)imageMuteIcon, 240, 6);
    laWidget_SetSize((laWidget*)imageMuteIcon, 23, 25);
    laWidget_SetVisible((laWidget*)imageMuteIcon, LA_FALSE);
    laWidget_SetScheme((laWidget*)imageMuteIcon, &JFLO_NoticeRunningArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)imageMuteIcon, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)imageMuteIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(imageMuteIcon, &iconMute);
    laWidget_AddChild((laWidget*)panelAlarmTitle, (laWidget*)imageMuteIcon);

    iconAlarmActive = laImageWidget_New();
    laWidget_SetSize((laWidget*)iconAlarmActive, 41, 33);
    laWidget_SetBackgroundType((laWidget*)iconAlarmActive, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)iconAlarmActive, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(iconAlarmActive, &Icon_AlarmActive);
    laWidget_AddChild((laWidget*)panelAlarmTitle, (laWidget*)iconAlarmActive);

    labelSpO2Dash = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)labelSpO2Dash, 62, 400);
    laWidget_SetSize((laWidget*)labelSpO2Dash, 38, 25);
    laWidget_SetScheme((laWidget*)labelSpO2Dash, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)labelSpO2Dash, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)labelSpO2Dash, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(labelSpO2Dash, laString_CreateFromID(string_text_Dash_S60));
    laLabelWidget_SetHAlignment(labelSpO2Dash, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)labelSpO2Dash);

    labelPRDash = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)labelPRDash, 186, 391);
    laWidget_SetSize((laWidget*)labelPRDash, 32, 25);
    laWidget_SetScheme((laWidget*)labelPRDash, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)labelPRDash, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)labelPRDash, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(labelPRDash, laString_CreateFromID(string_text_Dash_S45));
    laLabelWidget_SetHAlignment(labelPRDash, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)labelPRDash);

}

static void ScreenCreate_SettingScreen(laScreen* screen)
{
    laLayer* layer0;

    laScreen_SetShowEventCallback(screen, &SettingScreen_ShowEvent);
    laScreen_SetHideEventCallback(screen, &SettingScreen_HideEvent);

    layer0 = laLayer_New();
    laWidget_SetPosition((laWidget*)layer0, 0, 0);
    laWidget_SetSize((laWidget*)layer0, 272, 480);
    laWidget_SetBackgroundType((laWidget*)layer0, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetScheme((laWidget*)layer0, &JFLO_Background_Scheme);
    laLayer_SetBufferCount(layer0, 2);

    laScreen_SetLayer(screen, 0, layer0);

    SC_RunningTimePanel = laWidget_New();
    laWidget_SetSize((laWidget*)SC_RunningTimePanel, 272, 33);
    laWidget_SetScheme((laWidget*)SC_RunningTimePanel, &JFLO_NoticeRunningArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_RunningTimePanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_RunningTimePanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, SC_RunningTimePanel);

    SC_RunningTimeLabel = laLabelWidget_New();
    laWidget_SetSize((laWidget*)SC_RunningTimeLabel, 272, 33);
    laWidget_SetScheme((laWidget*)SC_RunningTimeLabel, &JFLO_NoticeRunningArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_RunningTimeLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_RunningTimeLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_RunningTimeLabel, laString_CreateFromID(string_text_Running));
    laWidget_AddChild((laWidget*)SC_RunningTimePanel, (laWidget*)SC_RunningTimeLabel);

    SC_WifiIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)SC_WifiIcon, 10, 42);
    laWidget_SetSize((laWidget*)SC_WifiIcon, 24, 24);
    laWidget_SetScheme((laWidget*)SC_WifiIcon, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_WifiIcon, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_WifiIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(SC_WifiIcon, &iconWifi_1);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)SC_WifiIcon);

    SC_DateTimeTextField = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)SC_DateTimeTextField, 54, 45);
    laWidget_SetSize((laWidget*)SC_DateTimeTextField, 45, 24);
    laWidget_SetScheme((laWidget*)SC_DateTimeTextField, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DateTimeTextField, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_DateTimeTextField, LA_WIDGET_BORDER_NONE);
    laTextFieldWidget_SetCursorEnabled(SC_DateTimeTextField, LA_TRUE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)SC_DateTimeTextField);

    SC_AlarmInfoButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_AlarmInfoButton, 104, 42);
    laWidget_SetSize((laWidget*)SC_AlarmInfoButton, 24, 24);
    laWidget_SetBackgroundType((laWidget*)SC_AlarmInfoButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_AlarmInfoButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_AlarmInfoButton, &iconInfo);
    laButtonWidget_SetReleasedImage(SC_AlarmInfoButton, &iconInfo);
    laButtonWidget_SetPressedEventCallback(SC_AlarmInfoButton, &SC_AlarmInfoButton_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)SC_AlarmInfoButton);

    SC_ExternalBatteryIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)SC_ExternalBatteryIcon, 141, 42);
    laWidget_SetSize((laWidget*)SC_ExternalBatteryIcon, 48, 24);
    laWidget_SetBackgroundType((laWidget*)SC_ExternalBatteryIcon, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_ExternalBatteryIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(SC_ExternalBatteryIcon, &iconExtBattery_4);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)SC_ExternalBatteryIcon);

    SC_InternalBatteryIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)SC_InternalBatteryIcon, 193, 42);
    laWidget_SetSize((laWidget*)SC_InternalBatteryIcon, 45, 24);
    laWidget_SetBackgroundType((laWidget*)SC_InternalBatteryIcon, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_InternalBatteryIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(SC_InternalBatteryIcon, &Icon_InternalBatteryNoInserted);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)SC_InternalBatteryIcon);

    SC_PowerACIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)SC_PowerACIcon, 240, 42);
    laWidget_SetSize((laWidget*)SC_PowerACIcon, 24, 24);
    laWidget_SetScheme((laWidget*)SC_PowerACIcon, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_PowerACIcon, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_PowerACIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(SC_PowerACIcon, &Icon_SocketDisconnected);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)SC_PowerACIcon);

    SC_HomeButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_HomeButton, 220, 77);
    laWidget_SetSize((laWidget*)SC_HomeButton, 50, 34);
    laWidget_SetScheme((laWidget*)SC_HomeButton, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_HomeButton, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_HomeButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_HomeButton, &iconRoundedHome);
    laButtonWidget_SetReleasedImage(SC_HomeButton, &iconRoundedHome);
    laButtonWidget_SetPressedEventCallback(SC_HomeButton, &SC_HomeButton_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)SC_HomeButton);

    SC_ScreenTitleLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_ScreenTitleLabel, 50, 80);
    laWidget_SetSize((laWidget*)SC_ScreenTitleLabel, 172, 25);
    laWidget_SetScheme((laWidget*)SC_ScreenTitleLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_ScreenTitleLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_ScreenTitleLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_ScreenTitleLabel, laString_CreateFromID(string_text_SettingScreen_SETTING));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)SC_ScreenTitleLabel);

    SC_MenuSettingPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSettingPanel, 5, 120);
    laWidget_SetSize((laWidget*)SC_MenuSettingPanel, 262, 290);
    laWidget_SetScheme((laWidget*)SC_MenuSettingPanel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSettingPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSettingPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, SC_MenuSettingPanel);

    SC_LanguageMenuPanel = laWidget_New();
    laWidget_SetSize((laWidget*)SC_LanguageMenuPanel, 262, 36);
    laWidget_SetScheme((laWidget*)SC_LanguageMenuPanel, &JFLO_ReverseItem_SettingScreen_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_LanguageMenuPanel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_LanguageMenuPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSettingPanel, SC_LanguageMenuPanel);

    SC_LanguageMenuIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)SC_LanguageMenuIcon, 10, 1);
    laWidget_SetSize((laWidget*)SC_LanguageMenuIcon, 34, 34);
    laWidget_SetScheme((laWidget*)SC_LanguageMenuIcon, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_LanguageMenuIcon, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_LanguageMenuIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(SC_LanguageMenuIcon, &iconRoundedLanguage);
    laWidget_AddChild((laWidget*)SC_LanguageMenuPanel, (laWidget*)SC_LanguageMenuIcon);

    SC_LanguageMenuLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_LanguageMenuLabel, 50, 3);
    laWidget_SetSize((laWidget*)SC_LanguageMenuLabel, 127, 25);
    laWidget_SetScheme((laWidget*)SC_LanguageMenuLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_LanguageMenuLabel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_LanguageMenuLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_LanguageMenuLabel, laString_CreateFromID(string_text_SettingScreen_Menu_LANGUAGE));
    laLabelWidget_SetHAlignment(SC_LanguageMenuLabel, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_LanguageMenuPanel, (laWidget*)SC_LanguageMenuLabel);

    SC_DateTimeMenuPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_DateTimeMenuPanel, 0, 36);
    laWidget_SetSize((laWidget*)SC_DateTimeMenuPanel, 262, 36);
    laWidget_SetScheme((laWidget*)SC_DateTimeMenuPanel, &JFLO_ReverseItem_SettingScreen_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DateTimeMenuPanel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_DateTimeMenuPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSettingPanel, SC_DateTimeMenuPanel);

    SC_DateTimeMenuIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)SC_DateTimeMenuIcon, 10, 1);
    laWidget_SetSize((laWidget*)SC_DateTimeMenuIcon, 34, 34);
    laWidget_SetScheme((laWidget*)SC_DateTimeMenuIcon, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DateTimeMenuIcon, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_DateTimeMenuIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(SC_DateTimeMenuIcon, &Icon_MenuSetting_DateTime);
    laWidget_AddChild((laWidget*)SC_DateTimeMenuPanel, (laWidget*)SC_DateTimeMenuIcon);

    SC_DateTimeMenuLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DateTimeMenuLabel, 50, 3);
    laWidget_SetSize((laWidget*)SC_DateTimeMenuLabel, 157, 25);
    laWidget_SetScheme((laWidget*)SC_DateTimeMenuLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DateTimeMenuLabel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_DateTimeMenuLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DateTimeMenuLabel, laString_CreateFromID(string_text_SettingScreen_Menu_DATETIME));
    laLabelWidget_SetHAlignment(SC_DateTimeMenuLabel, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_DateTimeMenuPanel, (laWidget*)SC_DateTimeMenuLabel);

    SC_WifiMenuPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_WifiMenuPanel, 0, 72);
    laWidget_SetSize((laWidget*)SC_WifiMenuPanel, 262, 36);
    laWidget_SetScheme((laWidget*)SC_WifiMenuPanel, &JFLO_ReverseItem_SettingScreen_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_WifiMenuPanel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_WifiMenuPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSettingPanel, SC_WifiMenuPanel);

    SC_WifiMenuIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)SC_WifiMenuIcon, 10, 1);
    laWidget_SetSize((laWidget*)SC_WifiMenuIcon, 34, 34);
    laWidget_SetScheme((laWidget*)SC_WifiMenuIcon, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_WifiMenuIcon, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_WifiMenuIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(SC_WifiMenuIcon, &iconRoundedWifi);
    laWidget_AddChild((laWidget*)SC_WifiMenuPanel, (laWidget*)SC_WifiMenuIcon);

    SC_WifiMenuLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_WifiMenuLabel, 50, 3);
    laWidget_SetSize((laWidget*)SC_WifiMenuLabel, 157, 25);
    laWidget_SetScheme((laWidget*)SC_WifiMenuLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_WifiMenuLabel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_WifiMenuLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_WifiMenuLabel, laString_CreateFromID(string_text_SettingScreen_Menu_WIFI));
    laLabelWidget_SetHAlignment(SC_WifiMenuLabel, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_WifiMenuPanel, (laWidget*)SC_WifiMenuLabel);

    SC_BrightnessMenuPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_BrightnessMenuPanel, 0, 108);
    laWidget_SetSize((laWidget*)SC_BrightnessMenuPanel, 262, 36);
    laWidget_SetScheme((laWidget*)SC_BrightnessMenuPanel, &JFLO_ReverseItem_SettingScreen_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_BrightnessMenuPanel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_BrightnessMenuPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSettingPanel, SC_BrightnessMenuPanel);

    SC_BrightnessMenuIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)SC_BrightnessMenuIcon, 10, 1);
    laWidget_SetSize((laWidget*)SC_BrightnessMenuIcon, 34, 34);
    laWidget_SetScheme((laWidget*)SC_BrightnessMenuIcon, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_BrightnessMenuIcon, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_BrightnessMenuIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(SC_BrightnessMenuIcon, &iconRoundedBrightness);
    laWidget_AddChild((laWidget*)SC_BrightnessMenuPanel, (laWidget*)SC_BrightnessMenuIcon);
    
    SC_BrightnessMenuLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_BrightnessMenuLabel, 50, 3);
    laWidget_SetSize((laWidget*)SC_BrightnessMenuLabel, 157, 25);
    laWidget_SetScheme((laWidget*)SC_BrightnessMenuLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_BrightnessMenuLabel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_BrightnessMenuLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_BrightnessMenuLabel, laString_CreateFromID(string_text_SettingScreen_Menu_BRIGHTNESS));
    laLabelWidget_SetHAlignment(SC_BrightnessMenuLabel, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_BrightnessMenuPanel, (laWidget*)SC_BrightnessMenuLabel);

    SC_OxygenSourceMenuPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_OxygenSourceMenuPanel, 0, 144);
    laWidget_SetSize((laWidget*)SC_OxygenSourceMenuPanel, 262, 36);
    laWidget_SetScheme((laWidget*)SC_OxygenSourceMenuPanel, &JFLO_ReverseItem_SettingScreen_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_OxygenSourceMenuPanel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_OxygenSourceMenuPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSettingPanel, SC_OxygenSourceMenuPanel);

    SC_OxygenSourceMenuIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)SC_OxygenSourceMenuIcon, 10, 1);
    laWidget_SetSize((laWidget*)SC_OxygenSourceMenuIcon, 34, 34);
    laWidget_SetScheme((laWidget*)SC_OxygenSourceMenuIcon, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_OxygenSourceMenuIcon, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_OxygenSourceMenuIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(SC_OxygenSourceMenuIcon, &Icon_MenuSetting_90_100);
    laWidget_AddChild((laWidget*)SC_OxygenSourceMenuPanel, (laWidget*)SC_OxygenSourceMenuIcon);

    SC_OxygenSourceMenuLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_OxygenSourceMenuLabel, 50, 3);
    laWidget_SetSize((laWidget*)SC_OxygenSourceMenuLabel, 157, 25);
    laWidget_SetScheme((laWidget*)SC_OxygenSourceMenuLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_OxygenSourceMenuLabel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_OxygenSourceMenuLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_OxygenSourceMenuLabel, laString_CreateFromID(string_text_SettingScreen_Menu_OXYGENSOURCE));
    laLabelWidget_SetHAlignment(SC_OxygenSourceMenuLabel, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_OxygenSourceMenuPanel, (laWidget*)SC_OxygenSourceMenuLabel);

    SC_SpO2AlarmLimitMenuPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_SpO2AlarmLimitMenuPanel, 0, 180);
    laWidget_SetSize((laWidget*)SC_SpO2AlarmLimitMenuPanel, 262, 36);
    laWidget_SetScheme((laWidget*)SC_SpO2AlarmLimitMenuPanel, &JFLO_ReverseItem_SettingScreen_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_SpO2AlarmLimitMenuPanel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_SpO2AlarmLimitMenuPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSettingPanel, SC_SpO2AlarmLimitMenuPanel);

    SC_SpO2AlarmLimitMenuIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)SC_SpO2AlarmLimitMenuIcon, 10, 1);
    laWidget_SetSize((laWidget*)SC_SpO2AlarmLimitMenuIcon, 34, 34);
    laWidget_SetScheme((laWidget*)SC_SpO2AlarmLimitMenuIcon, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_SpO2AlarmLimitMenuIcon, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_SpO2AlarmLimitMenuIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(SC_SpO2AlarmLimitMenuIcon, &Icon_MenuSetting_Spo2);
    laWidget_AddChild((laWidget*)SC_SpO2AlarmLimitMenuPanel, (laWidget*)SC_SpO2AlarmLimitMenuIcon);

    SC_SpO2AlarmLimitMenuLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_SpO2AlarmLimitMenuLabel, 50, 3);
    laWidget_SetSize((laWidget*)SC_SpO2AlarmLimitMenuLabel, 157, 25);
    laWidget_SetScheme((laWidget*)SC_SpO2AlarmLimitMenuLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_SpO2AlarmLimitMenuLabel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_SpO2AlarmLimitMenuLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_SpO2AlarmLimitMenuLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SpO2ALARMLIMIT));
    laLabelWidget_SetHAlignment(SC_SpO2AlarmLimitMenuLabel, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_SpO2AlarmLimitMenuPanel, (laWidget*)SC_SpO2AlarmLimitMenuLabel);

    SC_OxygenConcentrationAlarmLimitMenuPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_OxygenConcentrationAlarmLimitMenuPanel, 0, 216);
    laWidget_SetSize((laWidget*)SC_OxygenConcentrationAlarmLimitMenuPanel, 262, 36);
    laWidget_SetScheme((laWidget*)SC_OxygenConcentrationAlarmLimitMenuPanel, &JFLO_ReverseItem_SettingScreen_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_OxygenConcentrationAlarmLimitMenuPanel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_OxygenConcentrationAlarmLimitMenuPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSettingPanel, SC_OxygenConcentrationAlarmLimitMenuPanel);

    SC_OxygenConcentrationAlarmLimitMenuIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)SC_OxygenConcentrationAlarmLimitMenuIcon, 10, 1);
    laWidget_SetSize((laWidget*)SC_OxygenConcentrationAlarmLimitMenuIcon, 34, 34);
    laWidget_SetScheme((laWidget*)SC_OxygenConcentrationAlarmLimitMenuIcon, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_OxygenConcentrationAlarmLimitMenuIcon, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_OxygenConcentrationAlarmLimitMenuIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(SC_OxygenConcentrationAlarmLimitMenuIcon, &Icon_MenuSetting_O2);
    laWidget_AddChild((laWidget*)SC_OxygenConcentrationAlarmLimitMenuPanel, (laWidget*)SC_OxygenConcentrationAlarmLimitMenuIcon);

    SC_OxygenConcentrationAlarmLimitMenuLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_OxygenConcentrationAlarmLimitMenuLabel, 50, 3);
    laWidget_SetSize((laWidget*)SC_OxygenConcentrationAlarmLimitMenuLabel, 212, 25);
    laWidget_SetScheme((laWidget*)SC_OxygenConcentrationAlarmLimitMenuLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_OxygenConcentrationAlarmLimitMenuLabel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_OxygenConcentrationAlarmLimitMenuLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_OxygenConcentrationAlarmLimitMenuLabel, laString_CreateFromID(string_text_SettingScreen_Menu_OXYGENCONCENTRATIONALARMLIMIT));
    laLabelWidget_SetHAlignment(SC_OxygenConcentrationAlarmLimitMenuLabel, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_OxygenConcentrationAlarmLimitMenuPanel, (laWidget*)SC_OxygenConcentrationAlarmLimitMenuLabel);

    SC_AlarmSoundLevelMenuPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_AlarmSoundLevelMenuPanel, 0, 252);
    laWidget_SetSize((laWidget*)SC_AlarmSoundLevelMenuPanel, 262, 36);
    laWidget_SetScheme((laWidget*)SC_AlarmSoundLevelMenuPanel, &JFLO_ReverseItem_SettingScreen_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_AlarmSoundLevelMenuPanel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_AlarmSoundLevelMenuPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSettingPanel, SC_AlarmSoundLevelMenuPanel);

    SC_AlarmSoundLevelMenuIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)SC_AlarmSoundLevelMenuIcon, 10, 1);
    laWidget_SetSize((laWidget*)SC_AlarmSoundLevelMenuIcon, 34, 34);
    laWidget_SetScheme((laWidget*)SC_AlarmSoundLevelMenuIcon, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_AlarmSoundLevelMenuIcon, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_AlarmSoundLevelMenuIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(SC_AlarmSoundLevelMenuIcon, &iconRoundSpeaker);
    laWidget_AddChild((laWidget*)SC_AlarmSoundLevelMenuPanel, (laWidget*)SC_AlarmSoundLevelMenuIcon);

    SC_AlarmSoundLevelMenuLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_AlarmSoundLevelMenuLabel, 50, 3);
    laWidget_SetSize((laWidget*)SC_AlarmSoundLevelMenuLabel, 157, 25);
    laWidget_SetScheme((laWidget*)SC_AlarmSoundLevelMenuLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_AlarmSoundLevelMenuLabel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_AlarmSoundLevelMenuLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_AlarmSoundLevelMenuLabel, laString_CreateFromID(string_text_SettingScreen_Menu_ALARMSOUNDLEVEL));
    laLabelWidget_SetHAlignment(SC_AlarmSoundLevelMenuLabel, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_AlarmSoundLevelMenuPanel, (laWidget*)SC_AlarmSoundLevelMenuLabel);

    SC_TabButtonPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_TabButtonPanel, 21, 425);
    laWidget_SetSize((laWidget*)SC_TabButtonPanel, 224, 50);
    laWidget_SetScheme((laWidget*)SC_TabButtonPanel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_TabButtonPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_TabButtonPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, SC_TabButtonPanel);

    SC_SettingButton = laButtonWidget_New();
    laWidget_SetSize((laWidget*)SC_SettingButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_SettingButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_SettingButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_SettingButton, &Icon_Setting);
    laButtonWidget_SetReleasedImage(SC_SettingButton, &Icon_Setting);
    laButtonWidget_SetPressedEventCallback(SC_SettingButton, &SC_SettingButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_TabButtonPanel, (laWidget*)SC_SettingButton);

    SC_DataLogButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogButton, 61, 0);
    laWidget_SetSize((laWidget*)SC_DataLogButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_DataLogButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_DataLogButton, &Icon_DataLog);
    laButtonWidget_SetReleasedImage(SC_DataLogButton, &Icon_DataLog);
    laButtonWidget_SetPressedEventCallback(SC_DataLogButton, &SC_DataLogButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_TabButtonPanel, (laWidget*)SC_DataLogButton);

    SC_MaintenanceButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MaintenanceButton, 122, 0);
    laWidget_SetSize((laWidget*)SC_MaintenanceButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_MaintenanceButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MaintenanceButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MaintenanceButton, &Icon_Maintenance);
    laButtonWidget_SetReleasedImage(SC_MaintenanceButton, &Icon_Maintenance);
    laButtonWidget_SetPressedEventCallback(SC_MaintenanceButton, &SC_MaintenanceButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_TabButtonPanel, (laWidget*)SC_MaintenanceButton);

    SC_DeviceInformationButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_DeviceInformationButton, 183, 0);
    laWidget_SetSize((laWidget*)SC_DeviceInformationButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_DeviceInformationButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_DeviceInformationButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_DeviceInformationButton, &Icon_DeviceInformation);
    laButtonWidget_SetReleasedImage(SC_DeviceInformationButton, &Icon_DeviceInformation);
    laButtonWidget_SetPressedEventCallback(SC_DeviceInformationButton, &SC_DeviceInformationButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_TabButtonPanel, (laWidget*)SC_DeviceInformationButton);

    SC_TabButtonUnderBarPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_TabButtonUnderBarPanel, 0, 45);
    laWidget_SetSize((laWidget*)SC_TabButtonUnderBarPanel, 40, 3);
    laWidget_SetScheme((laWidget*)SC_TabButtonUnderBarPanel, &JFLO_Slider_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_TabButtonUnderBarPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_TabButtonUnderBarPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_TabButtonPanel, SC_TabButtonUnderBarPanel);

    SC_AlarmTitleBarPanel = laWidget_New();
    laWidget_SetSize((laWidget*)SC_AlarmTitleBarPanel, 272, 33);
    laWidget_SetVisible((laWidget*)SC_AlarmTitleBarPanel, LA_FALSE);
    laWidget_SetScheme((laWidget*)SC_AlarmTitleBarPanel, &JFLO_AlarmIndicatorHighPriority_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_AlarmTitleBarPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_AlarmTitleBarPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, SC_AlarmTitleBarPanel);

    SC_AlarmTitleLabel = laLabelWidget_New();
    laWidget_SetSize((laWidget*)SC_AlarmTitleLabel, 272, 33);
    laWidget_SetBackgroundType((laWidget*)SC_AlarmTitleLabel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_AlarmTitleLabel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_AlarmTitleBarPanel, (laWidget*)SC_AlarmTitleLabel);

    SC_AlarmMuteIcon = laImageWidget_New();
    laWidget_SetPosition((laWidget*)SC_AlarmMuteIcon, 240, 6);
    laWidget_SetSize((laWidget*)SC_AlarmMuteIcon, 23, 25);
    laWidget_SetVisible((laWidget*)SC_AlarmMuteIcon, LA_FALSE);
    laWidget_SetScheme((laWidget*)SC_AlarmMuteIcon, &JFLO_NoticeRunningArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_AlarmMuteIcon, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_AlarmMuteIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(SC_AlarmMuteIcon, &iconMute);
    laWidget_AddChild((laWidget*)SC_AlarmTitleBarPanel, (laWidget*)SC_AlarmMuteIcon);

    SC_AlarmActiveIcon = laImageWidget_New();
    laWidget_SetSize((laWidget*)SC_AlarmActiveIcon, 41, 33);
    laWidget_SetBackgroundType((laWidget*)SC_AlarmActiveIcon, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_AlarmActiveIcon, LA_WIDGET_BORDER_NONE);
    laImageWidget_SetImage(SC_AlarmActiveIcon, &Icon_AlarmActive);
    laWidget_AddChild((laWidget*)SC_AlarmTitleBarPanel, (laWidget*)SC_AlarmActiveIcon);

    SC_AlarmIndicatorLeftPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_AlarmIndicatorLeftPanel, 0, 33);
    laWidget_SetSize((laWidget*)SC_AlarmIndicatorLeftPanel, 5, 447);
    laWidget_SetVisible((laWidget*)SC_AlarmIndicatorLeftPanel, LA_FALSE);
    laWidget_SetOptimizationFlags((laWidget*)SC_AlarmIndicatorLeftPanel, LA_WIDGET_OPT_LOCAL_REDRAW);
    laWidget_SetScheme((laWidget*)SC_AlarmIndicatorLeftPanel, &JFLO_AlarmIndicatorHighPriority_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_AlarmIndicatorLeftPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_AlarmIndicatorLeftPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, SC_AlarmIndicatorLeftPanel);

    SC_AlarmIndicatorRightPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_AlarmIndicatorRightPanel, 267, 33);
    laWidget_SetSize((laWidget*)SC_AlarmIndicatorRightPanel, 5, 447);
    laWidget_SetVisible((laWidget*)SC_AlarmIndicatorRightPanel, LA_FALSE);
    laWidget_SetOptimizationFlags((laWidget*)SC_AlarmIndicatorRightPanel, LA_WIDGET_OPT_LOCAL_REDRAW);
    laWidget_SetScheme((laWidget*)SC_AlarmIndicatorRightPanel, &JFLO_AlarmIndicatorHighPriority_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_AlarmIndicatorRightPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_AlarmIndicatorRightPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, SC_AlarmIndicatorRightPanel);

    SC_AlarmIndicatorBottomPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_AlarmIndicatorBottomPanel, 5, 475);
    laWidget_SetSize((laWidget*)SC_AlarmIndicatorBottomPanel, 262, 5);
    laWidget_SetVisible((laWidget*)SC_AlarmIndicatorBottomPanel, LA_FALSE);
    laWidget_SetOptimizationFlags((laWidget*)SC_AlarmIndicatorBottomPanel, LA_WIDGET_OPT_LOCAL_REDRAW);
    laWidget_SetScheme((laWidget*)SC_AlarmIndicatorBottomPanel, &JFLO_AlarmIndicatorHighPriority_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_AlarmIndicatorBottomPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_AlarmIndicatorBottomPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, SC_AlarmIndicatorBottomPanel);

    SC_DataLogSettingPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingPanel, 5, 120);
    laWidget_SetSize((laWidget*)SC_DataLogSettingPanel, 262, 290);
    laWidget_SetVisible((laWidget*)SC_DataLogSettingPanel, LA_FALSE);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingPanel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, SC_DataLogSettingPanel);

    SC_DataLogSettingAlarmLabel = laLabelWidget_New();
    laWidget_SetSize((laWidget*)SC_DataLogSettingAlarmLabel, 90, 25);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingAlarmLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingAlarmLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingAlarmLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingAlarmLabel, laString_CreateFromID(string_text_SettingScreen_DataLog_ALARM));
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingAlarmLabel);

    SC_DataLogSettingEventLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingEventLabel, 90, 0);
    laWidget_SetSize((laWidget*)SC_DataLogSettingEventLabel, 90, 25);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingEventLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingEventLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingEventLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingEventLabel, laString_CreateFromID(string_text_SettingScreen_DataLog_EVENT));
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingEventLabel);

    SC_DataLogSettingUnderbarPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingUnderbarPanel, 20, 25);
    laWidget_SetSize((laWidget*)SC_DataLogSettingUnderbarPanel, 50, 3);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingUnderbarPanel, &JFLO_Underbar_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingUnderbarPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingUnderbarPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, SC_DataLogSettingUnderbarPanel);

    SC_DataLogSettingTimeLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingTimeLabel, 0, 35);
    laWidget_SetSize((laWidget*)SC_DataLogSettingTimeLabel, 85, 20);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingTimeLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingTimeLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingTimeLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingTimeLabel, laString_CreateFromID(string_text_SettingScreen_DataLog_Time));
    laLabelWidget_SetHAlignment(SC_DataLogSettingTimeLabel, LA_HALIGN_LEFT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingTimeLabel, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingTimeLabel);

    SC_DataLogSettingTitleLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingTitleLabel, 85, 35);
    laWidget_SetSize((laWidget*)SC_DataLogSettingTitleLabel, 100, 20);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingTitleLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingTitleLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingTitleLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingTitleLabel, laString_CreateFromID(string_text_SettingScreen_DataLog_AlarmTitle));
    laLabelWidget_SetHAlignment(SC_DataLogSettingTitleLabel, LA_HALIGN_LEFT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingTitleLabel, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingTitleLabel);

    SC_DataLogSettingStateLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingStateLabel, 195, 35);
    laWidget_SetSize((laWidget*)SC_DataLogSettingStateLabel, 62, 20);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingStateLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingStateLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingStateLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingStateLabel, laString_CreateFromID(string_text_SettingScreen_DataLog_State));
    laLabelWidget_SetHAlignment(SC_DataLogSettingStateLabel, LA_HALIGN_RIGHT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingStateLabel, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingStateLabel);

    SC_DataLogSettingTimeLabel_1 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingTimeLabel_1, 0, 60);
    laWidget_SetSize((laWidget*)SC_DataLogSettingTimeLabel_1, 85, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingTimeLabel_1, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingTimeLabel_1, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingTimeLabel_1, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingTimeLabel_1, laString_CreateFromID(string_text_SettingScreen_DataLog_DateSamle));
    laLabelWidget_SetHAlignment(SC_DataLogSettingTimeLabel_1, LA_HALIGN_LEFT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingTimeLabel_1, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingTimeLabel_1);

    SC_DataLogSettingTimeLabel_2 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingTimeLabel_2, 0, 95);
    laWidget_SetSize((laWidget*)SC_DataLogSettingTimeLabel_2, 85, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingTimeLabel_2, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingTimeLabel_2, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingTimeLabel_2, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingTimeLabel_2, laString_CreateFromID(string_text_SettingScreen_DataLog_DateSamle));
    laLabelWidget_SetHAlignment(SC_DataLogSettingTimeLabel_2, LA_HALIGN_LEFT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingTimeLabel_2, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingTimeLabel_2);

    SC_DataLogSettingTimeLabel_3 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingTimeLabel_3, 0, 130);
    laWidget_SetSize((laWidget*)SC_DataLogSettingTimeLabel_3, 85, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingTimeLabel_3, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingTimeLabel_3, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingTimeLabel_3, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingTimeLabel_3, laString_CreateFromID(string_text_SettingScreen_DataLog_DateSamle));
    laLabelWidget_SetHAlignment(SC_DataLogSettingTimeLabel_3, LA_HALIGN_LEFT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingTimeLabel_3, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingTimeLabel_3);

    SC_DataLogSettingTimeLabel_4 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingTimeLabel_4, 0, 165);
    laWidget_SetSize((laWidget*)SC_DataLogSettingTimeLabel_4, 85, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingTimeLabel_4, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingTimeLabel_4, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingTimeLabel_4, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingTimeLabel_4, laString_CreateFromID(string_text_SettingScreen_DataLog_DateSamle));
    laLabelWidget_SetHAlignment(SC_DataLogSettingTimeLabel_4, LA_HALIGN_LEFT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingTimeLabel_4, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingTimeLabel_4);

    SC_DataLogSettingTimeLabel_5 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingTimeLabel_5, 0, 200);
    laWidget_SetSize((laWidget*)SC_DataLogSettingTimeLabel_5, 85, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingTimeLabel_5, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingTimeLabel_5, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingTimeLabel_5, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingTimeLabel_5, laString_CreateFromID(string_text_SettingScreen_DataLog_DateSamle));
    laLabelWidget_SetHAlignment(SC_DataLogSettingTimeLabel_5, LA_HALIGN_LEFT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingTimeLabel_5, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingTimeLabel_5);

    SC_DataLogSettingTitleLabel_1 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingTitleLabel_1, 88, 60);
    laWidget_SetSize((laWidget*)SC_DataLogSettingTitleLabel_1, 100, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingTitleLabel_1, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingTitleLabel_1, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingTitleLabel_1, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingTitleLabel_1, laString_CreateFromID(string_text_SettingScreen_DataLog_AlarmTitleSample));
    laLabelWidget_SetHAlignment(SC_DataLogSettingTitleLabel_1, LA_HALIGN_LEFT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingTitleLabel_1, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingTitleLabel_1);

    SC_DataLogSettingTitleLabel_2 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingTitleLabel_2, 88, 95);
    laWidget_SetSize((laWidget*)SC_DataLogSettingTitleLabel_2, 100, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingTitleLabel_2, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingTitleLabel_2, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingTitleLabel_2, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingTitleLabel_2, laString_CreateFromID(string_text_SettingScreen_DataLog_AlarmTitleSample));
    laLabelWidget_SetHAlignment(SC_DataLogSettingTitleLabel_2, LA_HALIGN_LEFT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingTitleLabel_2, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingTitleLabel_2);

    SC_DataLogSettingTitleLabel_3 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingTitleLabel_3, 88, 130);
    laWidget_SetSize((laWidget*)SC_DataLogSettingTitleLabel_3, 100, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingTitleLabel_3, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingTitleLabel_3, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingTitleLabel_3, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingTitleLabel_3, laString_CreateFromID(string_text_SettingScreen_DataLog_AlarmTitleSample));
    laLabelWidget_SetHAlignment(SC_DataLogSettingTitleLabel_3, LA_HALIGN_LEFT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingTitleLabel_3, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingTitleLabel_3);

    SC_DataLogSettingTitleLabel_4 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingTitleLabel_4, 88, 165);
    laWidget_SetSize((laWidget*)SC_DataLogSettingTitleLabel_4, 100, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingTitleLabel_4, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingTitleLabel_4, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingTitleLabel_4, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingTitleLabel_4, laString_CreateFromID(string_text_SettingScreen_DataLog_AlarmTitleSample));
    laLabelWidget_SetHAlignment(SC_DataLogSettingTitleLabel_4, LA_HALIGN_LEFT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingTitleLabel_4, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingTitleLabel_4);

    SC_DataLogSettingTitleLabel_5 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingTitleLabel_5, 88, 200);
    laWidget_SetSize((laWidget*)SC_DataLogSettingTitleLabel_5, 100, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingTitleLabel_5, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingTitleLabel_5, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingTitleLabel_5, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingTitleLabel_5, laString_CreateFromID(string_text_SettingScreen_DataLog_AlarmTitleSample));
    laLabelWidget_SetHAlignment(SC_DataLogSettingTitleLabel_5, LA_HALIGN_LEFT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingTitleLabel_5, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingTitleLabel_5);

    SC_DataLogSettingStateLabel_1 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingStateLabel_1, 195, 60);
    laWidget_SetSize((laWidget*)SC_DataLogSettingStateLabel_1, 62, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingStateLabel_1, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingStateLabel_1, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingStateLabel_1, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingStateLabel_1, laString_CreateFromID(string_text_SettingScreen_DataLog_StateSample));
    laLabelWidget_SetHAlignment(SC_DataLogSettingStateLabel_1, LA_HALIGN_RIGHT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingStateLabel_1, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingStateLabel_1);

    SC_DataLogSettingStateLabel_2 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingStateLabel_2, 195, 95);
    laWidget_SetSize((laWidget*)SC_DataLogSettingStateLabel_2, 62, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingStateLabel_2, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingStateLabel_2, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingStateLabel_2, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingStateLabel_2, laString_CreateFromID(string_text_SettingScreen_DataLog_StateSample));
    laLabelWidget_SetHAlignment(SC_DataLogSettingStateLabel_2, LA_HALIGN_RIGHT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingStateLabel_2, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingStateLabel_2);

    SC_DataLogSettingStateLabel_3 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingStateLabel_3, 195, 130);
    laWidget_SetSize((laWidget*)SC_DataLogSettingStateLabel_3, 62, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingStateLabel_3, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingStateLabel_3, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingStateLabel_3, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingStateLabel_3, laString_CreateFromID(string_text_SettingScreen_DataLog_StateSample));
    laLabelWidget_SetHAlignment(SC_DataLogSettingStateLabel_3, LA_HALIGN_RIGHT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingStateLabel_3, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingStateLabel_3);

    SC_DataLogSettingStateLabel_4 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingStateLabel_4, 195, 165);
    laWidget_SetSize((laWidget*)SC_DataLogSettingStateLabel_4, 62, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingStateLabel_4, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingStateLabel_4, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingStateLabel_4, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingStateLabel_4, laString_CreateFromID(string_text_SettingScreen_DataLog_StateSample));
    laLabelWidget_SetHAlignment(SC_DataLogSettingStateLabel_4, LA_HALIGN_RIGHT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingStateLabel_4, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingStateLabel_4);

    SC_DataLogSettingStateLabel_5 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingStateLabel_5, 195, 200);
    laWidget_SetSize((laWidget*)SC_DataLogSettingStateLabel_5, 62, 35);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingStateLabel_5, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingStateLabel_5, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingStateLabel_5, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingStateLabel_5, laString_CreateFromID(string_text_SettingScreen_DataLog_StateSample));
    laLabelWidget_SetHAlignment(SC_DataLogSettingStateLabel_5, LA_HALIGN_RIGHT);
    laLabelWidget_SetVAlignment(SC_DataLogSettingStateLabel_5, LA_VALIGN_TOP);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingStateLabel_5);

    SC_DataLogSettingPageNumberLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingPageNumberLabel, 90, 250);
    laWidget_SetSize((laWidget*)SC_DataLogSettingPageNumberLabel, 82, 30);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingPageNumberLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingPageNumberLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingPageNumberLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DataLogSettingPageNumberLabel, laString_CreateFromID(string_text_SettingScreen_DataLog_PageNumberSample));
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingPageNumberLabel);

    SC_DataLogSettingPrevButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingPrevButton, 45, 248);
    laWidget_SetSize((laWidget*)SC_DataLogSettingPrevButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingPrevButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingPrevButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_DataLogSettingPrevButton, &Icon_SettingScreen_DataLog_Prev);
    laButtonWidget_SetReleasedImage(SC_DataLogSettingPrevButton, &Icon_SettingScreen_DataLog_Prev);
    laButtonWidget_SetPressedEventCallback(SC_DataLogSettingPrevButton, &SC_DataLogSettingPrevButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingPrevButton);

    SC_DataLogSettingNextButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingNextButton, 177, 248);
    laWidget_SetSize((laWidget*)SC_DataLogSettingNextButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingNextButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingNextButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_DataLogSettingNextButton, &Icon_SettingScreen_DataLog_Next);
    laButtonWidget_SetReleasedImage(SC_DataLogSettingNextButton, &Icon_SettingScreen_DataLog_Next);
    laButtonWidget_SetPressedEventCallback(SC_DataLogSettingNextButton, &SC_DataLogSettingNextButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, (laWidget*)SC_DataLogSettingNextButton);

    SC_DataLogSettingIndicator_1 = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingIndicator_1, 85, 65);
    laWidget_SetSize((laWidget*)SC_DataLogSettingIndicator_1, 3, 15);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingIndicator_1, &JFLO_AlarmIndicatorHighPriority_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingIndicator_1, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingIndicator_1, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, SC_DataLogSettingIndicator_1);

    SC_DataLogSettingIndicator_2 = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingIndicator_2, 85, 100);
    laWidget_SetSize((laWidget*)SC_DataLogSettingIndicator_2, 3, 15);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingIndicator_2, &JFLO_AlarmIndicatorHighPriority_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingIndicator_2, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingIndicator_2, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, SC_DataLogSettingIndicator_2);

    SC_DataLogSettingIndicator_3 = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingIndicator_3, 85, 135);
    laWidget_SetSize((laWidget*)SC_DataLogSettingIndicator_3, 3, 15);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingIndicator_3, &JFLO_AlarmIndicatorHighPriority_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingIndicator_3, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingIndicator_3, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, SC_DataLogSettingIndicator_3);

    SC_DataLogSettingIndicator_4 = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingIndicator_4, 85, 170);
    laWidget_SetSize((laWidget*)SC_DataLogSettingIndicator_4, 3, 15);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingIndicator_4, &JFLO_AlarmIndicatorHighPriority_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingIndicator_4, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingIndicator_4, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, SC_DataLogSettingIndicator_4);

    SC_DataLogSettingIndicator_5 = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_DataLogSettingIndicator_5, 85, 205);
    laWidget_SetSize((laWidget*)SC_DataLogSettingIndicator_5, 3, 15);
    laWidget_SetScheme((laWidget*)SC_DataLogSettingIndicator_5, &JFLO_AlarmIndicatorHighPriority_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DataLogSettingIndicator_5, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DataLogSettingIndicator_5, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_DataLogSettingPanel, SC_DataLogSettingIndicator_5);

    SC_MaintenancePanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MaintenancePanel, 5, 120);
    laWidget_SetSize((laWidget*)SC_MaintenancePanel, 262, 290);
    laWidget_SetVisible((laWidget*)SC_MaintenancePanel, LA_FALSE);
    laWidget_SetScheme((laWidget*)SC_MaintenancePanel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MaintenancePanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MaintenancePanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, SC_MaintenancePanel);

    SC_DeviceInformationPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_DeviceInformationPanel, 5, 120);
    laWidget_SetSize((laWidget*)SC_DeviceInformationPanel, 262, 290);
    laWidget_SetVisible((laWidget*)SC_DeviceInformationPanel, LA_FALSE);
    laWidget_SetScheme((laWidget*)SC_DeviceInformationPanel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DeviceInformationPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DeviceInformationPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, SC_DeviceInformationPanel);

    SC_DeviceInformationMainLabel = laLabelWidget_New();
    laWidget_SetSize((laWidget*)SC_DeviceInformationMainLabel, 45, 25);
    laWidget_SetScheme((laWidget*)SC_DeviceInformationMainLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DeviceInformationMainLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DeviceInformationMainLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DeviceInformationMainLabel, laString_CreateFromID(string_text_SettingScreen_DeviceInformation_MAIN));
    laWidget_AddChild((laWidget*)SC_DeviceInformationPanel, (laWidget*)SC_DeviceInformationMainLabel);

    SC_DeviceInformationChamberLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DeviceInformationChamberLabel, 45, 0);
    laWidget_SetSize((laWidget*)SC_DeviceInformationChamberLabel, 85, 25);
    laWidget_SetScheme((laWidget*)SC_DeviceInformationChamberLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DeviceInformationChamberLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DeviceInformationChamberLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DeviceInformationChamberLabel, laString_CreateFromID(string_text_SettingScreen_DeviceInformation_CHAMBER));
    laWidget_AddChild((laWidget*)SC_DeviceInformationPanel, (laWidget*)SC_DeviceInformationChamberLabel);

    SC_DeviceInformationCradleLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DeviceInformationCradleLabel, 130, 0);
    laWidget_SetSize((laWidget*)SC_DeviceInformationCradleLabel, 65, 25);
    laWidget_SetScheme((laWidget*)SC_DeviceInformationCradleLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DeviceInformationCradleLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DeviceInformationCradleLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DeviceInformationCradleLabel, laString_CreateFromID(string_text_SettingScreen_DeviceInformation_CRADLE));
    laWidget_AddChild((laWidget*)SC_DeviceInformationPanel, (laWidget*)SC_DeviceInformationCradleLabel);

    SC_DeviceInformationCircuitLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DeviceInformationCircuitLabel, 195, 0);
    laWidget_SetSize((laWidget*)SC_DeviceInformationCircuitLabel, 65, 25);
    laWidget_SetScheme((laWidget*)SC_DeviceInformationCircuitLabel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DeviceInformationCircuitLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DeviceInformationCircuitLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DeviceInformationCircuitLabel, laString_CreateFromID(string_text_SettingScreen_DeviceInformation_CIRCUIT));
    laWidget_AddChild((laWidget*)SC_DeviceInformationPanel, (laWidget*)SC_DeviceInformationCircuitLabel);

    SC_DeviceInformationUnderbarPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_DeviceInformationUnderbarPanel, 2, 25);
    laWidget_SetSize((laWidget*)SC_DeviceInformationUnderbarPanel, 40, 3);
    laWidget_SetScheme((laWidget*)SC_DeviceInformationUnderbarPanel, &JFLO_Underbar_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DeviceInformationUnderbarPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DeviceInformationUnderbarPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_DeviceInformationPanel, SC_DeviceInformationUnderbarPanel);

    SC_DeviceInformationLabel_1 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DeviceInformationLabel_1, 10, 50);
    laWidget_SetSize((laWidget*)SC_DeviceInformationLabel_1, 240, 25);
    laWidget_SetScheme((laWidget*)SC_DeviceInformationLabel_1, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DeviceInformationLabel_1, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DeviceInformationLabel_1, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DeviceInformationLabel_1, laString_CreateFromID(string_text_SettingScreen_DeviceInformation_Sample1));
    laLabelWidget_SetHAlignment(SC_DeviceInformationLabel_1, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_DeviceInformationPanel, (laWidget*)SC_DeviceInformationLabel_1);

    SC_DeviceInformationLabel_2 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DeviceInformationLabel_2, 10, 80);
    laWidget_SetSize((laWidget*)SC_DeviceInformationLabel_2, 240, 25);
    laWidget_SetScheme((laWidget*)SC_DeviceInformationLabel_2, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DeviceInformationLabel_2, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DeviceInformationLabel_2, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DeviceInformationLabel_2, laString_CreateFromID(string_text_SettingScreen_DeviceInformation_Sample2));
    laLabelWidget_SetHAlignment(SC_DeviceInformationLabel_2, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_DeviceInformationPanel, (laWidget*)SC_DeviceInformationLabel_2);

    SC_DeviceInformationLabel_3 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DeviceInformationLabel_3, 10, 110);
    laWidget_SetSize((laWidget*)SC_DeviceInformationLabel_3, 240, 25);
    laWidget_SetScheme((laWidget*)SC_DeviceInformationLabel_3, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DeviceInformationLabel_3, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DeviceInformationLabel_3, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DeviceInformationLabel_3, laString_CreateFromID(string_text_SettingScreen_DeviceInformation_Sample3));
    laLabelWidget_SetHAlignment(SC_DeviceInformationLabel_3, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_DeviceInformationPanel, (laWidget*)SC_DeviceInformationLabel_3);

    SC_DeviceInformationLabel_4 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DeviceInformationLabel_4, 10, 140);
    laWidget_SetSize((laWidget*)SC_DeviceInformationLabel_4, 240, 25);
    laWidget_SetScheme((laWidget*)SC_DeviceInformationLabel_4, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DeviceInformationLabel_4, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DeviceInformationLabel_4, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DeviceInformationLabel_4, laString_CreateFromID(string_text_SettingScreen_DeviceInformation_Sample4));
    laLabelWidget_SetHAlignment(SC_DeviceInformationLabel_4, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_DeviceInformationPanel, (laWidget*)SC_DeviceInformationLabel_4);

    SC_DeviceInformationLabel_5 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DeviceInformationLabel_5, 10, 170);
    laWidget_SetSize((laWidget*)SC_DeviceInformationLabel_5, 240, 25);
    laWidget_SetScheme((laWidget*)SC_DeviceInformationLabel_5, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DeviceInformationLabel_5, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DeviceInformationLabel_5, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DeviceInformationLabel_5, laString_CreateFromID(string_text_SettingScreen_DeviceInformation_Sample5));
    laLabelWidget_SetHAlignment(SC_DeviceInformationLabel_5, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_DeviceInformationPanel, (laWidget*)SC_DeviceInformationLabel_5);

    SC_DeviceInformationLabel_6 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_DeviceInformationLabel_6, 10, 200);
    laWidget_SetSize((laWidget*)SC_DeviceInformationLabel_6, 240, 25);
    laWidget_SetScheme((laWidget*)SC_DeviceInformationLabel_6, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_DeviceInformationLabel_6, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_DeviceInformationLabel_6, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_DeviceInformationLabel_6, laString_CreateFromID(string_text_SettingScreen_DeviceInformation_Sample6));
    laLabelWidget_SetHAlignment(SC_DeviceInformationLabel_6, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_DeviceInformationPanel, (laWidget*)SC_DeviceInformationLabel_6);

    SC_MenuSetting_SettingPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingPanel, 5, 120);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingPanel, 262, 290);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingPanel, LA_FALSE);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingPanel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, SC_MenuSetting_SettingPanel);

    SC_MenuSetting_SettingTitleLabel = laLabelWidget_New();
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingTitleLabel, 262, 30);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingTitleLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingTitleLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingTitleLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingTitleLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingTitle_OXYGENCONCENTRATIONALARMLIMIT));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingPanel, (laWidget*)SC_MenuSetting_SettingTitleLabel);

    SC_MenuSetting_SettingMessageLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingMessageLabel, 0, 190);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingMessageLabel, 262, 40);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingMessageLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingMessageLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingMessageLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingMessageLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingMessage_OXYGENCONCENTRATIONALARMLIMIT));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingPanel, (laWidget*)SC_MenuSetting_SettingMessageLabel);

    SC_MenuSetting_SettingBackButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingBackButton, 50, 230);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingBackButton, 71, 52);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingBackButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingBackButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingBackButton, &ButtonRound_Back);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingBackButton, &ButtonRound_Back);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingBackButton, &SC_MenuSetting_SettingBackButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingPanel, (laWidget*)SC_MenuSetting_SettingBackButton);

    SC_MenuSetting_SettingSaveButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingSaveButton, 142, 230);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingSaveButton, 71, 51);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingSaveButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingSaveButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingSaveButton, &ButtonRound_Save);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingSaveButton, &ButtonRound_Save);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingSaveButton, &SC_MenuSetting_SettingSaveButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingPanel, (laWidget*)SC_MenuSetting_SettingSaveButton);

    SC_MenuSetting_SettingLanguagePanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingLanguagePanel, 0, 30);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingLanguagePanel, 262, 160);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingLanguagePanel, LA_FALSE);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingLanguagePanel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingLanguagePanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingLanguagePanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingPanel, SC_MenuSetting_SettingLanguagePanel);

    SC_MenuSetting_SettingLanguage_NextButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingLanguage_NextButton, 199, 52);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingLanguage_NextButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingLanguage_NextButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingLanguage_NextButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingLanguage_NextButton, &Icon_SettingScreen_DataLog_Next);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingLanguage_NextButton, &Icon_SettingScreen_DataLog_Next);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingLanguage_NextButton, &SC_MenuSetting_SettingLanguage_NextButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingLanguagePanel, (laWidget*)SC_MenuSetting_SettingLanguage_NextButton);

    SC_MenuSetting_SettingLanguage_PrevButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingLanguage_PrevButton, 23, 53);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingLanguage_PrevButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingLanguage_PrevButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingLanguage_PrevButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingLanguage_PrevButton, &Icon_SettingScreen_DataLog_Prev);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingLanguage_PrevButton, &Icon_SettingScreen_DataLog_Prev);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingLanguage_PrevButton, &SC_MenuSetting_SettingLanguage_PrevButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingLanguagePanel, (laWidget*)SC_MenuSetting_SettingLanguage_PrevButton);

    SC_MenuSetting_SettingLanguage_SettingLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingLanguage_SettingLabel, 68, 44);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingLanguage_SettingLabel, 120, 60);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingLanguage_SettingLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingLanguage_SettingLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingLanguage_SettingLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingLanguage_SettingLabel, laString_CreateFromID(string_text_SettingScreen_Menu_Language_English));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingLanguagePanel, (laWidget*)SC_MenuSetting_SettingLanguage_SettingLabel);

    SC_MenuSetting_SettingDateTimePanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingDateTimePanel, 0, 30);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingDateTimePanel, 262, 160);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingDateTimePanel, LA_FALSE);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingDateTimePanel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingDateTimePanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingDateTimePanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingPanel, SC_MenuSetting_SettingDateTimePanel);

    SC_MenuSetting_SettingDateTime_YearLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingDateTime_YearLabel, 15, 72);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingDateTime_YearLabel, 40, 40);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingDateTime_YearLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingDateTime_YearLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingDateTime_YearLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_YearLabel, laString_CreateFromID(string_text_SettingScreen_Menu_DateTime_Sample1));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingDateTimePanel, (laWidget*)SC_MenuSetting_SettingDateTime_YearLabel);

    SC_MenuSetting_SettingDateTime_Seperator_1 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_1, 55, 72);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_1, 15, 40);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_1, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_1, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_1, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_Seperator_1, laString_CreateFromID(string_text_SettingScreen_Menu_DateTime_Slash));
    laLabelWidget_SetHAlignment(SC_MenuSetting_SettingDateTime_Seperator_1, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingDateTimePanel, (laWidget*)SC_MenuSetting_SettingDateTime_Seperator_1);

    SC_MenuSetting_SettingDateTime_MonLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingDateTime_MonLabel, 70, 72);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingDateTime_MonLabel, 28, 40);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingDateTime_MonLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingDateTime_MonLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingDateTime_MonLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_MonLabel, laString_CreateFromID(string_text_SettingScreen_Menu_DateTime_Sample2));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingDateTimePanel, (laWidget*)SC_MenuSetting_SettingDateTime_MonLabel);

    SC_MenuSetting_SettingDateTime_Seperator_2 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_2, 98, 72);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_2, 15, 40);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_2, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_2, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_2, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_Seperator_2, laString_CreateFromID(string_text_SettingScreen_Menu_DateTime_Slash));
    laLabelWidget_SetHAlignment(SC_MenuSetting_SettingDateTime_Seperator_2, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingDateTimePanel, (laWidget*)SC_MenuSetting_SettingDateTime_Seperator_2);

    SC_MenuSetting_SettingDateTime_DateLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingDateTime_DateLabel, 113, 72);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingDateTime_DateLabel, 28, 40);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingDateTime_DateLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingDateTime_DateLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingDateTime_DateLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_DateLabel, laString_CreateFromID(string_text_SettingScreen_Menu_DateTime_Sample3));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingDateTimePanel, (laWidget*)SC_MenuSetting_SettingDateTime_DateLabel);

    SC_MenuSetting_SettingDateTime_HourLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingDateTime_HourLabel, 150, 72);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingDateTime_HourLabel, 28, 40);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingDateTime_HourLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingDateTime_HourLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingDateTime_HourLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_HourLabel, laString_CreateFromID(string_text_SettingScreen_Menu_DateTime_Sample4));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingDateTimePanel, (laWidget*)SC_MenuSetting_SettingDateTime_HourLabel);

    SC_MenuSetting_SettingDateTime_SepLabel_3 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingDateTime_SepLabel_3, 178, 72);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingDateTime_SepLabel_3, 10, 37);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingDateTime_SepLabel_3, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingDateTime_SepLabel_3, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingDateTime_SepLabel_3, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_SepLabel_3, laString_CreateFromID(string_text_SettingScreen_Menu_DateTime_Colon));
    laLabelWidget_SetHAlignment(SC_MenuSetting_SettingDateTime_SepLabel_3, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingDateTimePanel, (laWidget*)SC_MenuSetting_SettingDateTime_SepLabel_3);

    SC_MenuSetting_SettingDateTime_MinLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingDateTime_MinLabel, 188, 72);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingDateTime_MinLabel, 28, 40);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingDateTime_MinLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingDateTime_MinLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingDateTime_MinLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_MinLabel, laString_CreateFromID(string_text_SettingScreen_Menu_DateTime_Sample5));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingDateTimePanel, (laWidget*)SC_MenuSetting_SettingDateTime_MinLabel);

    SC_MenuSetting_SettingDateTime_Seperator_4 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_4, 216, 72);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_4, 10, 37);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_4, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_4, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingDateTime_Seperator_4, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_Seperator_4, laString_CreateFromID(string_text_SettingScreen_Menu_DateTime_Colon));
    laLabelWidget_SetHAlignment(SC_MenuSetting_SettingDateTime_Seperator_4, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingDateTimePanel, (laWidget*)SC_MenuSetting_SettingDateTime_Seperator_4);

    SC_MenuSetting_SettingDateTime_SecLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingDateTime_SecLabel, 226, 72);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingDateTime_SecLabel, 28, 40);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingDateTime_SecLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingDateTime_SecLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingDateTime_SecLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingDateTime_SecLabel, laString_CreateFromID(string_text_SettingScreen_Menu_DateTime_Sample6));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingDateTimePanel, (laWidget*)SC_MenuSetting_SettingDateTime_SecLabel);

    SC_MenuSetting_SettingDateTime_DecButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingDateTime_DecButton, 7, 112);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingDateTime_DecButton, 50, 40);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingDateTime_DecButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingDateTime_DecButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingDateTime_DecButton, &iconDownSingleArrow);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingDateTime_DecButton, &iconDownSingleArrow);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingDateTime_DecButton, &SC_MenuSetting_SettingDateTime_DecButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingDateTimePanel, (laWidget*)SC_MenuSetting_SettingDateTime_DecButton);

    SC_MenuSetting_SettingDateTime_IncButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingDateTime_IncButton, 7, 32);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingDateTime_IncButton, 50, 40);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingDateTime_IncButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingDateTime_IncButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingDateTime_IncButton, &iconUpSingleArrow);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingDateTime_IncButton, &iconUpSingleArrow);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingDateTime_IncButton, &SC_MenuSetting_SettingDateTime_IncButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingDateTimePanel, (laWidget*)SC_MenuSetting_SettingDateTime_IncButton);

    SC_MenuSetting_SettingDateTime_UnderBarPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingDateTime_UnderBarPanel, 25, 105);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingDateTime_UnderBarPanel, 20, 3);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingDateTime_UnderBarPanel, &JFLO_Underbar_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingDateTime_UnderBarPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingDateTime_UnderBarPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingDateTimePanel, SC_MenuSetting_SettingDateTime_UnderBarPanel);

    SC_MenuSetting_SettingWifiPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingWifiPanel, 0, 30);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingWifiPanel, 262, 160);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingWifiPanel, LA_FALSE);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingWifiPanel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingWifiPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingWifiPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingPanel, SC_MenuSetting_SettingWifiPanel);

    SC_MenuSetting_SettingWifi_OnLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingWifi_OnLabel, 31, 40);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingWifi_OnLabel, 100, 60);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingWifi_OnLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingWifi_OnLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingWifi_OnLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingWifi_OnLabel, laString_CreateFromID(string_text_SettingScreen_Menu_Wifi_ON));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingWifiPanel, (laWidget*)SC_MenuSetting_SettingWifi_OnLabel);

    SC_MenuSetting_SettingWifi_OffLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingWifi_OffLabel, 131, 40);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingWifi_OffLabel, 100, 60);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingWifi_OffLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingWifi_OffLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingWifi_OffLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingWifi_OffLabel, laString_CreateFromID(string_text_SettingScreen_Menu_Wifi_OFF));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingWifiPanel, (laWidget*)SC_MenuSetting_SettingWifi_OffLabel);

    SC_MenuSetting_SettingWifi_UnderBarPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingWifi_UnderBarPanel, 165, 85);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingWifi_UnderBarPanel, 30, 3);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingWifi_UnderBarPanel, &JFLO_Underbar_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingWifi_UnderBarPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingWifi_UnderBarPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingWifiPanel, SC_MenuSetting_SettingWifi_UnderBarPanel);

    SC_MenuSetting_SettingBrightnessPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingBrightnessPanel, 0, 30);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingBrightnessPanel, 262, 160);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingBrightnessPanel, LA_FALSE);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingBrightnessPanel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingBrightnessPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingBrightnessPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingPanel, SC_MenuSetting_SettingBrightnessPanel);

    SC_MenuSetting_SettingBrightness_NextButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingBrightness_NextButton, 199, 32);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingBrightness_NextButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingBrightness_NextButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingBrightness_NextButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingBrightness_NextButton, &Icon_SettingScreen_DataLog_Next);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingBrightness_NextButton, &Icon_SettingScreen_DataLog_Next);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingBrightness_NextButton, &SC_MenuSetting_SettingBrightness_NextButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingBrightnessPanel, (laWidget*)SC_MenuSetting_SettingBrightness_NextButton);

    SC_MenuSetting_SettingBrightness_PrevButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingBrightness_PrevButton, 23, 33);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingBrightness_PrevButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingBrightness_PrevButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingBrightness_PrevButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingBrightness_PrevButton, &Icon_SettingScreen_DataLog_Prev);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingBrightness_PrevButton, &Icon_SettingScreen_DataLog_Prev);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingBrightness_PrevButton, &SC_MenuSetting_SettingBrightness_PrevButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingBrightnessPanel, (laWidget*)SC_MenuSetting_SettingBrightness_PrevButton);

    SC_MenuSetting_SettingBrightness_ValueLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingBrightness_ValueLabel, 68, 24);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingBrightness_ValueLabel, 120, 60);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingBrightness_ValueLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingBrightness_ValueLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingBrightness_ValueLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingBrightness_ValueLabel, laString_CreateFromID(string_text_SettingScreen_Menu_AlarmSound_Sample1));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingBrightnessPanel, (laWidget*)SC_MenuSetting_SettingBrightness_ValueLabel);

    SC_MenuSetting_SettingBrightness_OffLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingBrightness_OffLabel, 212, 125);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingBrightness_OffLabel, 50, 35);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingBrightness_OffLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingBrightness_OffLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingBrightness_OffLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingBrightness_OffLabel, laString_CreateFromID(string_text_SettingScreen_Menu_Brightness_OFF));
    laLabelWidget_SetHAlignment(SC_MenuSetting_SettingBrightness_OffLabel, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingBrightnessPanel, (laWidget*)SC_MenuSetting_SettingBrightness_OffLabel);

    SC_MenuSetting_SettingBrightness_OnLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingBrightness_OnLabel, 212, 90);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingBrightness_OnLabel, 50, 35);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingBrightness_OnLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingBrightness_OnLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingBrightness_OnLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingBrightness_OnLabel, laString_CreateFromID(string_text_SettingScreen_Menu_Brightness_ON));
    laLabelWidget_SetHAlignment(SC_MenuSetting_SettingBrightness_OnLabel, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingBrightnessPanel, (laWidget*)SC_MenuSetting_SettingBrightness_OnLabel);

    SC_MenuSetting_SettingBrightness_UnderBar = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingBrightness_UnderBar, 216, 159);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingBrightness_UnderBar, 30, 3);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingBrightness_UnderBar, &JFLO_Underbar_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingBrightness_UnderBar, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingBrightness_UnderBar, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingBrightnessPanel, SC_MenuSetting_SettingBrightness_UnderBar);

    SC_MenuSetting_SettingBrightness_MessageLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingBrightness_MessageLabel, 23, 108);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingBrightness_MessageLabel, 176, 35);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingBrightness_MessageLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingBrightness_MessageLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingBrightness_MessageLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingBrightness_MessageLabel, laString_CreateFromID(string_text_SettingScreen_Menu_SettingMessage_BRIGHTNESS));
    laLabelWidget_SetHAlignment(SC_MenuSetting_SettingBrightness_MessageLabel, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingBrightnessPanel, (laWidget*)SC_MenuSetting_SettingBrightness_MessageLabel);

    SC_MenuSetting_SettingOxySourcePanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingOxySourcePanel, 0, 30);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingOxySourcePanel, 262, 160);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingOxySourcePanel, LA_FALSE);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingOxySourcePanel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingOxySourcePanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingOxySourcePanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingPanel, SC_MenuSetting_SettingOxySourcePanel);

    SC_MenuSetting_SettingOxySource_90Label = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingOxySource_90Label, 20, 40);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingOxySource_90Label, 100, 60);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingOxySource_90Label, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingOxySource_90Label, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingOxySource_90Label, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingOxySource_90Label, laString_CreateFromID(string_text_SettingScreen_Menu_OxygenSource_90));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingOxySourcePanel, (laWidget*)SC_MenuSetting_SettingOxySource_90Label);

    SC_MenuSetting_SettingOxySource_100Label = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingOxySource_100Label, 136, 40);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingOxySource_100Label, 110, 60);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingOxySource_100Label, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingOxySource_100Label, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingOxySource_100Label, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingOxySource_100Label, laString_CreateFromID(string_text_SettingScreen_Menu_OxygenSource_100));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingOxySourcePanel, (laWidget*)SC_MenuSetting_SettingOxySource_100Label);

    SC_MenuSetting_SettingOxySource_UnderBarPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingOxySource_UnderBarPanel, 155, 100);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingOxySource_UnderBarPanel, 60, 3);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingOxySource_UnderBarPanel, &JFLO_Underbar_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingOxySource_UnderBarPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingOxySource_UnderBarPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingOxySourcePanel, SC_MenuSetting_SettingOxySource_UnderBarPanel);

    SC_MenuSetting_SettingSpo2AlarmPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingSpo2AlarmPanel, 0, 30);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingSpo2AlarmPanel, 262, 160);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingSpo2AlarmPanel, LA_FALSE);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingSpo2AlarmPanel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingSpo2AlarmPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingSpo2AlarmPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingPanel, SC_MenuSetting_SettingSpo2AlarmPanel);

    SC_MenuSetting_SettingSpo2Alarm_NextButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingSpo2Alarm_NextButton, 199, 52);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingSpo2Alarm_NextButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingSpo2Alarm_NextButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingSpo2Alarm_NextButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingSpo2Alarm_NextButton, &Icon_SettingScreen_DataLog_Next);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingSpo2Alarm_NextButton, &Icon_SettingScreen_DataLog_Next);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingSpo2Alarm_NextButton, &SC_MenuSetting_SettingSpo2Alarm_NextButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingSpo2AlarmPanel, (laWidget*)SC_MenuSetting_SettingSpo2Alarm_NextButton);

    SC_MenuSetting_SettingSpo2Alarm_PrevButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingSpo2Alarm_PrevButton, 23, 53);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingSpo2Alarm_PrevButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingSpo2Alarm_PrevButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingSpo2Alarm_PrevButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingSpo2Alarm_PrevButton, &Icon_SettingScreen_DataLog_Prev);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingSpo2Alarm_PrevButton, &Icon_SettingScreen_DataLog_Prev);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingSpo2Alarm_PrevButton, &SC_MenuSetting_SettingSpo2Alarm_PrevButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingSpo2AlarmPanel, (laWidget*)SC_MenuSetting_SettingSpo2Alarm_PrevButton);

    SC_MenuSetting_SettingSpo2Alarm_ValueLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingSpo2Alarm_ValueLabel, 68, 44);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingSpo2Alarm_ValueLabel, 120, 60);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingSpo2Alarm_ValueLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingSpo2Alarm_ValueLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingSpo2Alarm_ValueLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingSpo2Alarm_ValueLabel, laString_CreateFromID(string_text_SettingScreen_Menu_OxyLimit_Sample2));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingSpo2AlarmPanel, (laWidget*)SC_MenuSetting_SettingSpo2Alarm_ValueLabel);

    SC_MenuSetting_SettingOxyAlarmPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingOxyAlarmPanel, 0, 30);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingOxyAlarmPanel, 262, 160);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingOxyAlarmPanel, LA_FALSE);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingOxyAlarmPanel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingOxyAlarmPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingOxyAlarmPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingPanel, SC_MenuSetting_SettingOxyAlarmPanel);

    SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton, 25, 35);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton, &Icon_SettingScreen_DataLog_Prev);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton, &Icon_SettingScreen_DataLog_Prev);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton, &SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingOxyAlarmPanel, (laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimit_DecButton);

    SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton, 201, 99);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton, &Icon_SettingScreen_DataLog_Next);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton, &Icon_SettingScreen_DataLog_Next);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton, &SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingOxyAlarmPanel, (laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimit_IncButton);

    SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton, 25, 100);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton, &Icon_SettingScreen_DataLog_Prev);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton, &Icon_SettingScreen_DataLog_Prev);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton, &SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingOxyAlarmPanel, (laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimit_DecButton);

    SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton, 201, 35);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton, &Icon_SettingScreen_DataLog_Next);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton, &Icon_SettingScreen_DataLog_Next);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton, &SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingOxyAlarmPanel, (laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimit_IncButton);

    SC_MenuSetting_SettingOxyAlarm_UpLimitLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimitLabel, 70, 20);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimitLabel, 120, 60);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimitLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimitLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimitLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingOxyAlarm_UpLimitLabel, laString_CreateFromID(string_text_SettingScreen_Menu_OxyLimit_Sample1));
    laLabelWidget_SetHAlignment(SC_MenuSetting_SettingOxyAlarm_UpLimitLabel, LA_HALIGN_RIGHT);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingOxyAlarmPanel, (laWidget*)SC_MenuSetting_SettingOxyAlarm_UpLimitLabel);

    SC_MenuSetting_SettingOxyAlarm_LoLimitLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimitLabel, 70, 91);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimitLabel, 120, 60);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimitLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimitLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimitLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingOxyAlarm_LoLimitLabel, laString_CreateFromID(string_text_SettingScreen_Menu_OxyLimit_Sample2));
    laLabelWidget_SetHAlignment(SC_MenuSetting_SettingOxyAlarm_LoLimitLabel, LA_HALIGN_RIGHT);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingOxyAlarmPanel, (laWidget*)SC_MenuSetting_SettingOxyAlarm_LoLimitLabel);

    SC_MenuSetting_SettingAlarmSoundLevelPanel = laWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingAlarmSoundLevelPanel, 0, 30);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingAlarmSoundLevelPanel, 262, 160);
    laWidget_SetVisible((laWidget*)SC_MenuSetting_SettingAlarmSoundLevelPanel, LA_FALSE);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingAlarmSoundLevelPanel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingAlarmSoundLevelPanel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingAlarmSoundLevelPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingPanel, SC_MenuSetting_SettingAlarmSoundLevelPanel);

    SC_MenuSetting_SettingAlarmSoundLevel_NextButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_NextButton, 199, 52);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_NextButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_NextButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_NextButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingAlarmSoundLevel_NextButton, &Icon_SettingScreen_DataLog_Next);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingAlarmSoundLevel_NextButton, &Icon_SettingScreen_DataLog_Next);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingAlarmSoundLevel_NextButton, &SC_MenuSetting_SettingAlarmSoundLevel_NextButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingAlarmSoundLevelPanel, (laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_NextButton);

    SC_MenuSetting_SettingAlarmSoundLevel_PrevButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_PrevButton, 23, 53);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_PrevButton, 41, 42);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_PrevButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_PrevButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_MenuSetting_SettingAlarmSoundLevel_PrevButton, &Icon_SettingScreen_DataLog_Prev);
    laButtonWidget_SetReleasedImage(SC_MenuSetting_SettingAlarmSoundLevel_PrevButton, &Icon_SettingScreen_DataLog_Prev);
    laButtonWidget_SetPressedEventCallback(SC_MenuSetting_SettingAlarmSoundLevel_PrevButton, &SC_MenuSetting_SettingAlarmSoundLevel_PrevButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingAlarmSoundLevelPanel, (laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_PrevButton);

    SC_MenuSetting_SettingAlarmSoundLevel_ValueLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_ValueLabel, 68, 44);
    laWidget_SetSize((laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_ValueLabel, 120, 60);
    laWidget_SetScheme((laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_ValueLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_ValueLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_ValueLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_MenuSetting_SettingAlarmSoundLevel_ValueLabel, laString_CreateFromID(string_text_SettingScreen_Menu_AlarmSound_Sample1));
    laWidget_AddChild((laWidget*)SC_MenuSetting_SettingAlarmSoundLevelPanel, (laWidget*)SC_MenuSetting_SettingAlarmSoundLevel_ValueLabel);

    SC_SaveConfirmPanel = laWidget_New();
    laWidget_SetSize((laWidget*)SC_SaveConfirmPanel, 272, 480);
    laWidget_SetX((laWidget*)SC_SaveConfirmPanel, 275);
    laWidget_SetScheme((laWidget*)SC_SaveConfirmPanel, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_SaveConfirmPanel, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_SaveConfirmPanel, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, SC_SaveConfirmPanel);

    SC_SaveConfirmBackgroundImage = laImageWidget_New();
    laWidget_SetPosition((laWidget*)SC_SaveConfirmBackgroundImage, 5, 150);
    laWidget_SetSize((laWidget*)SC_SaveConfirmBackgroundImage, 260, 260);
    laWidget_SetScheme((laWidget*)SC_SaveConfirmBackgroundImage, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_SaveConfirmBackgroundImage, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_SaveConfirmBackgroundImage, LA_WIDGET_BORDER_NONE);
    //FIXME: this cost too much time to render
//    laImageWidget_SetImage(SC_SaveConfirmBackgroundImage, &RectangleRound_252_1);
    laWidget_AddChild((laWidget*)SC_SaveConfirmPanel, (laWidget*)SC_SaveConfirmBackgroundImage);
  
    SC_SaveConfirmYesButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_SaveConfirmYesButton, 58, 269);
    laWidget_SetSize((laWidget*)SC_SaveConfirmYesButton, 71, 52);
    laWidget_SetBackgroundType((laWidget*)SC_SaveConfirmYesButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_SaveConfirmYesButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_SaveConfirmYesButton, &ButtonRound_YES);
    laButtonWidget_SetReleasedImage(SC_SaveConfirmYesButton, &ButtonRound_YES);
    laButtonWidget_SetPressedEventCallback(SC_SaveConfirmYesButton, &SC_SaveConfirmYesButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_SaveConfirmPanel, (laWidget*)SC_SaveConfirmYesButton);

    SC_SaveConfirmNoButton = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)SC_SaveConfirmNoButton, 149, 269);
    laWidget_SetSize((laWidget*)SC_SaveConfirmNoButton, 71, 52);
    laWidget_SetBackgroundType((laWidget*)SC_SaveConfirmNoButton, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)SC_SaveConfirmNoButton, LA_WIDGET_BORDER_NONE);
    laButtonWidget_SetPressedImage(SC_SaveConfirmNoButton, &ButtonRound_NO);
    laButtonWidget_SetReleasedImage(SC_SaveConfirmNoButton, &ButtonRound_NO);
    laButtonWidget_SetPressedEventCallback(SC_SaveConfirmNoButton, &SC_SaveConfirmNoButton_PressedEvent);

    laWidget_AddChild((laWidget*)SC_SaveConfirmPanel, (laWidget*)SC_SaveConfirmNoButton);

    SC_SaveConfirmTitleLabel = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)SC_SaveConfirmTitleLabel, 37, 178);
    laWidget_SetSize((laWidget*)SC_SaveConfirmTitleLabel, 200, 60);
    laWidget_SetScheme((laWidget*)SC_SaveConfirmTitleLabel, &JFLO_MonitoringArea_Scheme);
    laWidget_SetBackgroundType((laWidget*)SC_SaveConfirmTitleLabel, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)SC_SaveConfirmTitleLabel, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(SC_SaveConfirmTitleLabel, laString_CreateFromID(string_text_SettingScreen_SaveConfirmTitle));
    laWidget_AddChild((laWidget*)SC_SaveConfirmPanel, (laWidget*)SC_SaveConfirmTitleLabel);

}

static void ScreenCreate_MaintenanceScreen(laScreen* screen)
{
    laLayer* layer0;

    laScreen_SetShowEventCallback(screen, &MaintenanceScreen_ShowEvent);
    laScreen_SetHideEventCallback(screen, &MaintenanceScreen_HideEvent);

    layer0 = laLayer_New();
    laWidget_SetPosition((laWidget*)layer0, 0, 0);
    laWidget_SetSize((laWidget*)layer0, 272, 480);
    laWidget_SetBackgroundType((laWidget*)layer0, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetScheme((laWidget*)layer0, &JFLO_Background_Scheme);
    laLayer_SetBufferCount(layer0, 2);

    laScreen_SetLayer(screen, 0, layer0);

    lbTitleAirflow = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbTitleAirflow, 10, 2);
    laWidget_SetSize((laWidget*)lbTitleAirflow, 100, 18);
    laWidget_SetScheme((laWidget*)lbTitleAirflow, &JFLO_Maintainance_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbTitleAirflow, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbTitleAirflow, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbTitleAirflow, laString_CreateFromID(string_AirFlow_MaintenanceScreen));
    laLabelWidget_SetHAlignment(lbTitleAirflow, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lbTitleAirflow);

    lbTitleO2flow = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbTitleO2flow, 10, 65);
    laWidget_SetSize((laWidget*)lbTitleO2flow, 100, 18);
    laWidget_SetScheme((laWidget*)lbTitleO2flow, &JFLO_Maintainance_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbTitleO2flow, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbTitleO2flow, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbTitleO2flow, laString_CreateFromID(string_O2Flow_MaintenanceScreen));
    laLabelWidget_SetHAlignment(lbTitleO2flow, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lbTitleO2flow);

    lbTitleTemp = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbTitleTemp, 162, 65);
    laWidget_SetSize((laWidget*)lbTitleTemp, 100, 18);
    laWidget_SetScheme((laWidget*)lbTitleTemp, &JFLO_Maintainance_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbTitleTemp, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbTitleTemp, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbTitleTemp, laString_CreateFromID(string_Temp_MaintenanceScreen));
    laLabelWidget_SetHAlignment(lbTitleTemp, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lbTitleTemp);

    s_titleMotor = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)s_titleMotor, 4, 175);
    laWidget_SetSize((laWidget*)s_titleMotor, 46, 25);
    laWidget_SetBackgroundType((laWidget*)s_titleMotor, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)s_titleMotor, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetHAlignment(s_titleMotor, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)s_titleMotor);

    lbTitleO2 = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbTitleO2, 162, 2);
    laWidget_SetSize((laWidget*)lbTitleO2, 100, 18);
    laWidget_SetScheme((laWidget*)lbTitleO2, &JFLO_Maintainance_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbTitleO2, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbTitleO2, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbTitleO2, laString_CreateFromID(string_O2_MaintenanceScreen));
    laLabelWidget_SetHAlignment(lbTitleO2, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lbTitleO2);

    lbTitleIH = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbTitleIH, 5, 171);
    laWidget_SetSize((laWidget*)lbTitleIH, 20, 20);
    laWidget_SetScheme((laWidget*)lbTitleIH, &JFLO_Maintainance_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbTitleIH, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbTitleIH, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbTitleIH, laString_CreateFromID(string_text_IH_MaintenanceScreen));
    laLabelWidget_SetHAlignment(lbTitleIH, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lbTitleIH);

    btnHome_MaintenanceScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnHome_MaintenanceScreen, 163, 431);
    laWidget_SetSize((laWidget*)btnHome_MaintenanceScreen, 100, 40);
    laWidget_SetBackgroundType((laWidget*)btnHome_MaintenanceScreen, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnHome_MaintenanceScreen, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetText(btnHome_MaintenanceScreen, laString_CreateFromID(string_text_Home_MaintenanceScreen));
    laButtonWidget_SetPressedEventCallback(btnHome_MaintenanceScreen, &btnHome_MaintenanceScreen_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnHome_MaintenanceScreen);

    tfAirFlowMonitor = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)tfAirFlowMonitor, 10, 20);
    laWidget_SetSize((laWidget*)tfAirFlowMonitor, 100, 40);
    laWidget_SetBackgroundType((laWidget*)tfAirFlowMonitor, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)tfAirFlowMonitor, LA_WIDGET_BORDER_BEVEL);
    laTextFieldWidget_SetCursorEnabled(tfAirFlowMonitor, LA_TRUE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)tfAirFlowMonitor);

    tfO2FlowMonitor = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)tfO2FlowMonitor, 10, 85);
    laWidget_SetSize((laWidget*)tfO2FlowMonitor, 100, 40);
    laWidget_SetBackgroundType((laWidget*)tfO2FlowMonitor, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)tfO2FlowMonitor, LA_WIDGET_BORDER_BEVEL);
    laTextFieldWidget_SetCursorEnabled(tfO2FlowMonitor, LA_TRUE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)tfO2FlowMonitor);

    tfTempMonitor = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)tfTempMonitor, 162, 85);
    laWidget_SetSize((laWidget*)tfTempMonitor, 100, 40);
    laWidget_SetBackgroundType((laWidget*)tfTempMonitor, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)tfTempMonitor, LA_WIDGET_BORDER_BEVEL);
    laTextFieldWidget_SetCursorEnabled(tfTempMonitor, LA_TRUE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)tfTempMonitor);

    tfO2Monitor = laTextFieldWidget_New();
    laWidget_SetPosition((laWidget*)tfO2Monitor, 162, 20);
    laWidget_SetSize((laWidget*)tfO2Monitor, 100, 40);
    laWidget_SetBackgroundType((laWidget*)tfO2Monitor, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)tfO2Monitor, LA_WIDGET_BORDER_BEVEL);
    laTextFieldWidget_SetCursorEnabled(tfO2Monitor, LA_TRUE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)tfO2Monitor);

    lbTitleMotor = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbTitleMotor, 5, 125);
    laWidget_SetSize((laWidget*)lbTitleMotor, 150, 20);
    laWidget_SetScheme((laWidget*)lbTitleMotor, &JFLO_Maintainance_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbTitleMotor, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbTitleMotor, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbTitleMotor, laString_CreateFromID(string_text_Motor_MaintenanceScreen));
    laLabelWidget_SetHAlignment(lbTitleMotor, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lbTitleMotor);

    scrollbarMotor = laSliderWidget_New();
    laWidget_SetPosition((laWidget*)scrollbarMotor, 5, 150);
    laWidget_SetSize((laWidget*)scrollbarMotor, 200, 20);
    laWidget_SetScheme((laWidget*)scrollbarMotor, &JFLO_Slider_Scheme);
    laWidget_SetBackgroundType((laWidget*)scrollbarMotor, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)scrollbarMotor, LA_WIDGET_BORDER_BEVEL);
    laSliderWidget_SetOrientation(scrollbarMotor, LA_SLIDER_ORIENT_HORIZONTAL, LA_FALSE);
    laSliderWidget_SetMaximumValue(scrollbarMotor, 105);
    laSliderWidget_SetValueChangedEventCallback(scrollbarMotor, &scrollbarMotor_ValueChangedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)scrollbarMotor);

    btnOnOffMotor = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnOnOffMotor, 210, 146);
    laWidget_SetSize((laWidget*)btnOnOffMotor, 50, 25);
    laWidget_SetBackgroundType((laWidget*)btnOnOffMotor, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnOnOffMotor, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetText(btnOnOffMotor, laString_CreateFromID(string_text_ON_MaintenanceScreen));
    laButtonWidget_SetPressedEventCallback(btnOnOffMotor, &btnOnOffMotor_PressedEvent);
    
    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnOnOffMotor);

    scrollbarIH = laSliderWidget_New();
    laWidget_SetPosition((laWidget*)scrollbarIH, 5, 191);
    laWidget_SetSize((laWidget*)scrollbarIH, 200, 20);
    laWidget_SetScheme((laWidget*)scrollbarIH, &JFLO_Slider_Scheme);
    laWidget_SetBackgroundType((laWidget*)scrollbarIH, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)scrollbarIH, LA_WIDGET_BORDER_BEVEL);
    laSliderWidget_SetOrientation(scrollbarIH, LA_SLIDER_ORIENT_HORIZONTAL, LA_FALSE);
    laSliderWidget_SetMaximumValue(scrollbarIH, 25);
    laSliderWidget_SetValueChangedEventCallback(scrollbarIH, &scrollbarIH_ValueChangedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)scrollbarIH);

    btnOnOffIH = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnOnOffIH, 210, 187);
    laWidget_SetSize((laWidget*)btnOnOffIH, 50, 25);
    laWidget_SetBackgroundType((laWidget*)btnOnOffIH, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnOnOffIH, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetText(btnOnOffIH, laString_CreateFromID(string_text_ON_MaintenanceScreen));
    laButtonWidget_SetPressedEventCallback(btnOnOffIH, &btnOnOffIH_PressedEvent);
    
    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnOnOffIH);

    lbTitleWaterPump = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbTitleWaterPump, 4, 241);
    laWidget_SetSize((laWidget*)lbTitleWaterPump, 85, 20);
    laWidget_SetScheme((laWidget*)lbTitleWaterPump, &JFLO_Maintainance_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbTitleWaterPump, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbTitleWaterPump, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbTitleWaterPump, laString_CreateFromID(string_text_WaterPump_MaintenanceScreen));
    laLabelWidget_SetHAlignment(lbTitleWaterPump, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lbTitleWaterPump);
    
    btnOnOffWaterPump = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnOnOffWaterPump, 100, 240);
    laWidget_SetSize((laWidget*)btnOnOffWaterPump, 51, 29);
    laWidget_SetBackgroundType((laWidget*)btnOnOffWaterPump, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnOnOffWaterPump, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetText(btnOnOffWaterPump, laString_CreateFromID(string_text_ON_MaintenanceScreen));
    laButtonWidget_SetPressedEventCallback(btnOnOffWaterPump, &btnOnOffWaterPump_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnOnOffWaterPump);

    btnUpgrade = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnUpgrade, 170, 392);
    laWidget_SetSize((laWidget*)btnUpgrade, 80, 28);
    laWidget_SetBackgroundType((laWidget*)btnUpgrade, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnUpgrade, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetText(btnUpgrade, laString_CreateFromID(string_text_Upgrade_MaintenanceScreen));
    laButtonWidget_SetPressedEventCallback(btnUpgrade, &btnUpgrade_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnUpgrade);

    lbTitleVersion = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)lbTitleVersion, 2, 452);
    laWidget_SetSize((laWidget*)lbTitleVersion, 159, 20);
    laWidget_SetScheme((laWidget*)lbTitleVersion, &JFLO_Maintainance_Scheme);
    laWidget_SetBackgroundType((laWidget*)lbTitleVersion, LA_WIDGET_BACKGROUND_NONE);
    laWidget_SetBorderType((laWidget*)lbTitleVersion, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(lbTitleVersion, laString_CreateFromID(string_CurrentVersion_MaintenanceScreen));
    laLabelWidget_SetHAlignment(lbTitleVersion, LA_HALIGN_LEFT);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)lbTitleVersion);

    btnLogtoUsb = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnLogtoUsb, 10, 419);
    laWidget_SetSize((laWidget*)btnLogtoUsb, 64, 29);
    laWidget_SetBackgroundType((laWidget*)btnLogtoUsb, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnLogtoUsb, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetText(btnLogtoUsb, laString_CreateFromID(string_text_LogtoUSB_MaintenanceScreen));
    laButtonWidget_SetPressedEventCallback(btnLogtoUsb, &btnLogtoUsb_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnLogtoUsb);

    btnClearLog = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnClearLog, 87, 419);
    laWidget_SetSize((laWidget*)btnClearLog, 64, 29);
    laWidget_SetBackgroundType((laWidget*)btnClearLog, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnClearLog, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetText(btnClearLog, laString_CreateFromID(string_text_ClearLog_MaintenanceScreen));
    laButtonWidget_SetPressedEventCallback(btnClearLog, &btnClearLog_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnClearLog);

    btnTestHighAlarm_MaintenanceScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnTestHighAlarm_MaintenanceScreen, 10, 271);
    laWidget_SetSize((laWidget*)btnTestHighAlarm_MaintenanceScreen, 100, 25);
    laWidget_SetBackgroundType((laWidget*)btnTestHighAlarm_MaintenanceScreen, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnTestHighAlarm_MaintenanceScreen, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetPressedEventCallback(btnTestHighAlarm_MaintenanceScreen, &btnTestHighAlarm_MaintenanceScreen_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnTestHighAlarm_MaintenanceScreen);

    btnTestMediumAlarm_MaintenanceScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnTestMediumAlarm_MaintenanceScreen, 10, 300);
    laWidget_SetSize((laWidget*)btnTestMediumAlarm_MaintenanceScreen, 100, 25);
    laWidget_SetBackgroundType((laWidget*)btnTestMediumAlarm_MaintenanceScreen, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnTestMediumAlarm_MaintenanceScreen, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetPressedEventCallback(btnTestMediumAlarm_MaintenanceScreen, &btnTestMediumAlarm_MaintenanceScreen_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnTestMediumAlarm_MaintenanceScreen);

    btnTestLowAlarm_MaintenanceScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnTestLowAlarm_MaintenanceScreen, 10, 330);
    laWidget_SetSize((laWidget*)btnTestLowAlarm_MaintenanceScreen, 100, 25);
    laWidget_SetBackgroundType((laWidget*)btnTestLowAlarm_MaintenanceScreen, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnTestLowAlarm_MaintenanceScreen, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetPressedEventCallback(btnTestLowAlarm_MaintenanceScreen, &btnTestLowAlarm_MaintenanceScreen_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnTestLowAlarm_MaintenanceScreen);

    btnDebug4_MaintenanceScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnDebug4_MaintenanceScreen, 155, 271);
    laWidget_SetSize((laWidget*)btnDebug4_MaintenanceScreen, 100, 25);
    laWidget_SetBackgroundType((laWidget*)btnDebug4_MaintenanceScreen, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnDebug4_MaintenanceScreen, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetPressedEventCallback(btnDebug4_MaintenanceScreen, &btnDebug4_MaintenanceScreen_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnDebug4_MaintenanceScreen);

    btnDebug5_MaintenanceScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnDebug5_MaintenanceScreen, 155, 300);
    laWidget_SetSize((laWidget*)btnDebug5_MaintenanceScreen, 100, 25);
    laWidget_SetBackgroundType((laWidget*)btnDebug5_MaintenanceScreen, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnDebug5_MaintenanceScreen, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetPressedEventCallback(btnDebug5_MaintenanceScreen, &btnDebug5_MaintenanceScreen_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnDebug5_MaintenanceScreen);

    btnDebug6_MaintenanceScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnDebug6_MaintenanceScreen, 155, 330);
    laWidget_SetSize((laWidget*)btnDebug6_MaintenanceScreen, 100, 25);
    laWidget_SetBackgroundType((laWidget*)btnDebug6_MaintenanceScreen, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnDebug6_MaintenanceScreen, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetPressedEventCallback(btnDebug6_MaintenanceScreen, &btnDebug6_MaintenanceScreen_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnDebug6_MaintenanceScreen);

    btnDebug7_MaintenanceScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnDebug7_MaintenanceScreen, 10, 360);
    laWidget_SetSize((laWidget*)btnDebug7_MaintenanceScreen, 100, 25);
    laWidget_SetBackgroundType((laWidget*)btnDebug7_MaintenanceScreen, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnDebug7_MaintenanceScreen, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetPressedEventCallback(btnDebug7_MaintenanceScreen, &btnDebug7_MaintenanceScreen_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnDebug7_MaintenanceScreen);

    btnDebug8_MaintenanceScreen = laButtonWidget_New();
    laWidget_SetPosition((laWidget*)btnDebug8_MaintenanceScreen, 155, 360);
    laWidget_SetSize((laWidget*)btnDebug8_MaintenanceScreen, 100, 25);
    laWidget_SetBackgroundType((laWidget*)btnDebug8_MaintenanceScreen, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)btnDebug8_MaintenanceScreen, LA_WIDGET_BORDER_BEVEL);
    laButtonWidget_SetPressedEventCallback(btnDebug8_MaintenanceScreen, &btnDebug8_MaintenanceScreen_PressedEvent);

    laWidget_AddChild((laWidget*)layer0, (laWidget*)btnDebug8_MaintenanceScreen);

}

static void ScreenCreate_PowerOffScreen(laScreen* screen)
{
    laLayer* layer0;

    laScreen_SetShowEventCallback(screen, &PowerOffScreen_ShowEvent);
    laScreen_SetHideEventCallback(screen, &PowerOffScreen_HideEvent);

    layer0 = laLayer_New();
    laWidget_SetPosition((laWidget*)layer0, 0, 0);
    laWidget_SetSize((laWidget*)layer0, 272, 480);
    laWidget_SetBackgroundType((laWidget*)layer0, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetScheme((laWidget*)layer0, &JFLO_Background_Scheme);
    laLayer_SetBufferCount(layer0, 2);

    laScreen_SetLayer(screen, 0, layer0);

    labelPowerOffMessage = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)labelPowerOffMessage, 55, 198);
    laWidget_SetSize((laWidget*)labelPowerOffMessage, 162, 64);
    laWidget_SetScheme((laWidget*)labelPowerOffMessage, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)labelPowerOffMessage, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)labelPowerOffMessage, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, (laWidget*)labelPowerOffMessage);

    labelPowerOffAdditionalMessage = laLabelWidget_New();
    laWidget_SetPosition((laWidget*)labelPowerOffAdditionalMessage, 10, 339);
    laWidget_SetSize((laWidget*)labelPowerOffAdditionalMessage, 252, 96);
    laWidget_SetScheme((laWidget*)labelPowerOffAdditionalMessage, &JFLO_Background_Scheme);
    laWidget_SetBackgroundType((laWidget*)labelPowerOffAdditionalMessage, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)labelPowerOffAdditionalMessage, LA_WIDGET_BORDER_NONE);
    laLabelWidget_SetText(labelPowerOffAdditionalMessage, laString_CreateFromID(string_text_PowerOffAdditionalMessage));
    laWidget_AddChild((laWidget*)layer0, (laWidget*)labelPowerOffAdditionalMessage);

    panelPowerOffAlarmIndicatorTop = laWidget_New();
    laWidget_SetSize((laWidget*)panelPowerOffAlarmIndicatorTop, 272, 5);
    laWidget_SetScheme((laWidget*)panelPowerOffAlarmIndicatorTop, &JFLO_AlarmIndicatorLow1Priority_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelPowerOffAlarmIndicatorTop, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelPowerOffAlarmIndicatorTop, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelPowerOffAlarmIndicatorTop);

    panelPowerOffAlarmIndicatorBottom = laWidget_New();
    laWidget_SetPosition((laWidget*)panelPowerOffAlarmIndicatorBottom, 0, 475);
    laWidget_SetSize((laWidget*)panelPowerOffAlarmIndicatorBottom, 272, 5);
    laWidget_SetScheme((laWidget*)panelPowerOffAlarmIndicatorBottom, &JFLO_AlarmIndicatorLow1Priority_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelPowerOffAlarmIndicatorBottom, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelPowerOffAlarmIndicatorBottom, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelPowerOffAlarmIndicatorBottom);

    panelPowerOffAlarmIndicatorLeft = laWidget_New();
    laWidget_SetPosition((laWidget*)panelPowerOffAlarmIndicatorLeft, 0, 5);
    laWidget_SetSize((laWidget*)panelPowerOffAlarmIndicatorLeft, 5, 470);
    laWidget_SetScheme((laWidget*)panelPowerOffAlarmIndicatorLeft, &JFLO_AlarmIndicatorLow1Priority_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelPowerOffAlarmIndicatorLeft, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelPowerOffAlarmIndicatorLeft, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelPowerOffAlarmIndicatorLeft);

    panelPowerOffAlarmIndicatorRight = laWidget_New();
    laWidget_SetPosition((laWidget*)panelPowerOffAlarmIndicatorRight, 267, 5);
    laWidget_SetSize((laWidget*)panelPowerOffAlarmIndicatorRight, 5, 470);
    laWidget_SetScheme((laWidget*)panelPowerOffAlarmIndicatorRight, &JFLO_AlarmIndicatorLow1Priority_Scheme);
    laWidget_SetBackgroundType((laWidget*)panelPowerOffAlarmIndicatorRight, LA_WIDGET_BACKGROUND_FILL);
    laWidget_SetBorderType((laWidget*)panelPowerOffAlarmIndicatorRight, LA_WIDGET_BORDER_NONE);
    laWidget_AddChild((laWidget*)layer0, panelPowerOffAlarmIndicatorRight);

}



