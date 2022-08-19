/** @file AlarmExpression.c
 *  @brief Control the alarm expression display
 *  @author Trac Truong
 */

/* This section lists the other files that are included in this file.
 */
#include "gfx/libaria/libaria_init.h"
#include "Gui/GuiDefine.h"
#include "Gui/DisplayControl.h"
#include "Gui/AlarmExpression.h"
#include "Gui/AnimationControl.h"
#include "Gui/MainScreen.h"
#include "Device/DeviceInterface.h"
#include "Device/GT911.h"

/** @brief flag alarm detail title show done or not*/
static bool isAlarmDetailTitleShow = false;

/** @brief flag alarm detail title show done or not*/
static bool isDetailMessageShow = false;

/** @brief flag alarm detail title show done or not*/
static bool isAdditionalMessageShow = false;

/** @brief alarm detail title tick count*/
static TickType_t xShowAlarmDetailTitleTick = 0;

static bool isVideoJustComplete = true;

/** @brief alarm additional message tick count*/
static TickType_t xAdditionalMessageTick = 0;

/** @brief alarm message tick count*/
static TickType_t xDetailMessageTick = 0;

void AlarmExpression_InitData( void )
{
    alarmExpressionConfigList[eI2C1ErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eI2C1ErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eI2C2ErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eI2C2ErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eI2C3ErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eI2C3ErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eI2C4ErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eI2C4ErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eSPI3ErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eSPI3ErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eUart1ErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eUart1ErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eUart2ErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eUart2ErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eUart4ErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eUart4ErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eUart6ErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eUart6ErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eAirFlowSensorErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eAirFlowSensorErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eO2FlowSensorErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eO2FlowSensorErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eBME280ErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eBME280ErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eADXL345ErrorAlarmID] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eADXL345ErrorAlarmID,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eDRV8308ErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eDRV8308ErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eAudioErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eAudioErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eMotorTaskErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eMotorTaskErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eHeaterTaskErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eHeaterTaskErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eHumidityTaskErrorAlarm] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_eHumidityTaskErrorAlarm,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = false,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eBreathingCircuitNotConnectedAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E001_CheckForTubeConnection,
        .alarmDetailTiltleStrId = string_AlarmDetailTitle_E001_CheckForTubeConnection,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E110_DeviceError,//-1,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E110_DeviceError,//-1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = true,
        .alarmAnimationData = &videoData_E001,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eCheckLeakAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E002_CheckForLeaks,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = true,
        .alarmAnimationData = &videoData_E002,     
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eCheckBlockageAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E003_CheckforBlockage,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = true,
        .alarmAnimationData = &videoData_E003,        
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eNonGenuineCircuitAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E004_TubeCheck,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E004_NonGenuineCircuit,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E004_NonGenuineCircuit,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eBreathingCircuitChangedAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E005_TubeCheck,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E005a_AdultCircuitToPediatricMode,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E005f_DisinfectionCircuitToPediatricMode,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = true,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc_E005,
    };
    alarmExpressionConfigList[eExpiredCircuitAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E006_ExpiredCircuit,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E006_ExpiredCircuit,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E006_ExpiredCircuit,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eCheckConnectionChamberAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E007_CheckConnectionOfChamber,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = true,
        .alarmAnimationData = &videoData_E001,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eNoMoreWaterInChamberAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E008_CheckWater,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = true,
        .alarmAnimationData = &videoData_E001,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eRunOutOfWaterAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E009_CheckWater,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = true,
        .alarmAnimationData = &videoData_E001,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eLowTemperatureAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E010_LowTemperature,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E010a_AmbientTempLowerThan18CelsiusDegree,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E010b_AmbientTempEqualOrOverThan18CelsiusDegree,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eHighTemperatureAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E011_HighTemperature,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E011a_AmbientTempLowerThanSettingTemp,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E011b_AmbientTempEqualOrOverThanSettingTemp,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eHighTemperatureAbnormalityAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E012_HighTemperature,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E012,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E012,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eRoomTemperatureLowToAchieveTargetTemperatureAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E013_CannotReachTargetTemperature,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E013,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E013,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eCheckOperatingConditionsAlarmID] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E014,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E014a_AmbientTempLowerThan42CelsiusDegree,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E014b_AmbientTempEqualOrOverThan42CelsiusDegree,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eDeviceErrorToAchieveTargetTemperatureAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E015_CannotReachTargetTemperature,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E015,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E015,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eOxygenHighAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E016_O2TooHigh,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E016,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E016,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eOxygenLowAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E017_O2TooLow,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E017,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E017,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eDevicePostureAbnormalAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E018_CheckDevicePosition,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,//string_AlarmMessage_Animation_TBD,
        .alarmDetailMsgStrEndId = -1,//string_AlarmMessage_Animation_TBD,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eDevicePostureBadAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E019_CheckDevicePosition,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eSwitchToBatteryModeAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E020_SwitchToBatteryMode,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eBatteryLowAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E021_BatteryLow,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E021,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E021,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eBatteryGetsRunOutAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E022_BatteryGetRunOut,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eStopFunctionAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E023_StopFunction,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E023,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E023,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[ePowerNotEnoughAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E024_PowerIsNotEnough,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E024,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E024,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eSpO2FailedAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E025_SpO2Failed,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E025,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E025,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eSpO2LowAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E026_SpO2Low,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E026,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E026,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eSpO2SensorProbeAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E027_CheckTheSpO2SensorProbe,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E027,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E027,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eSpO2SignalLossAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E028_SpO2SignalLoss,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E028,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E028,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eSpO2SensorUnpluggedAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E029_SpO2SensorUnplugged,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E029,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E029,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eWaterSupplyCheckAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E030_WaterSupplyCheck,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eWaterTankConnectionCheckAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E031_WaterTankConnectionCheck,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eAirFlowAbnormalAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E032_AirFlowAbnormal,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E032,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E032,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eMainUnitBatteryDisconnectedAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E033_MainUnitBatteryDisconnected,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E033,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E033,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eCradleBatteryDisconnectedAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E034_CradleBatteryDisconnected,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E034,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E034,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eFailureOxygenFlowMeasurementFlowSensorAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E100_DeviceError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E100_DeviceError,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E100_DeviceError,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eFailureAirFlowMeasurementFlowSensorAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E101_DeviceError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E101_DeviceError,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E101_DeviceError,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eFailureChamberTemperatureSensorAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E102_DeviceError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E102_DeviceError,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E102_DeviceError,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eFailureCoilTemperatureSendorAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E103_DeviceError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E103_DeviceError,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E103_DeviceError,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eFailureBreathingCircuitOutTemperatureSensorAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E104_DeviceError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E104_DeviceError,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E104_DeviceError,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eFailureEnvironmentSensorAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E105_DeviceError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E105_DeviceError,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E105_DeviceError,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eFailureCurrentSensorAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E106_DeviceError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E106_DeviceError,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E106_DeviceError,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eFailureBlowerAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E107_DeviceError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E107_DeviceError,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E107_DeviceError,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eSpeakerDisconnectedAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E108_DeviceError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E108_DeviceError,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E108_DeviceError,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eFailureWaterLevelSensorAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E109_DeviceError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E109_DeviceError,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E109_DeviceError,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eFailureExternalFlashMemoryAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E110_DeviceError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E110_DeviceError,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E110_DeviceError,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eFailureAccelerationSensorAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E111_DeviceError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E111_DeviceError,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E111_DeviceError,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eFailureLightSensorAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E112_DeviceError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E112_DeviceError,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E112_DeviceError,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eMainUnitBatteryCommunicationErrorAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E113_MainUnitBatteryCommunicationError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eCradleBatteryCommunicationErrorAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E114_CradleBatteryCommunicationError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eCradleCommunicationErrorAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E115_CradleCommunicationError,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eMainMCUFailedOrOutOfControlAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E116_MainMicrocomputerFailed,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = false,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    
    alarmExpressionConfigList[eESP32FailedAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E117_ESP32Failed,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = -1,
        .alarmDetailMsgStrEndId = -1,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    alarmExpressionConfigList[eBreathingCircuitHeaterWireBrokenAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E118_BreathingCircuitHeaterWireBroken,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E118,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E118,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = false,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    
    alarmExpressionConfigList[eFailureSpo2ModuleAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E119,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E119,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E119,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    
    alarmExpressionConfigList[eFailureRTCModuleAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E120,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E120,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E120,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    
    alarmExpressionConfigList[eFailureLCDTouchModuleAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E121,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E121,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E121,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    
    alarmExpressionConfigList[eChamberTemperatureAbnormalAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E124,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E124,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E124,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    
    alarmExpressionConfigList[eTooMuchWaterInTheChamberAlarmId] = (AlarmExpressionConfig) {
        .alarmTitleStrId = string_AlarmTitle_E125,
        .alarmDetailTiltleStrId = -1,
        .alarmDetailMsgStrStartId = string_AlarmMessage_E125,
        .alarmDetailMsgStrEndId = string_AlarmMessage_E125,
        .alarmPopupEnable = true,
        .alarmCloseButtonEnable = true,
        .alarmResetButtonEnable = true,
        .alarmDetailAnimationEnable = false,
        .alarmAnimationData = 0,
        .alarmOKButtonEnable = false,
        .alarmResetButtonFunc = &AlarmExpression_ResetButtonFunc,
    };
    
    AlarmExpression_SetAlarmMuteIconShowFlag(false);
    AlarmExpression_SetReset(false);
    AlarmExpression_InitVideoData();
}

void AlarmExpression_InitVideoData()
{
    //init alarm E001
    videoData_E001.x = ALARM_VIDEO_POS_X;
    videoData_E001.y = ALARM_VIDEO_POS_Y;
    videoData_E001.w = ALARM_VIDEO_WIDTH;
    videoData_E001.h = ALARM_VIDEO_HEIGHT;
    videoData_E001.bytesPerPixel = BYTES_PER_PIXEL;
    videoData_E001.frameRateMS = FRAME_RATE_MS;
    videoData_E001.videoSizeInBytes = ALARM_VIDEO_WIDTH*ALARM_VIDEO_HEIGHT*BYTES_PER_PIXEL*ALARM_E001_TOTAL_FILES;
    videoData_E001.frameData = (FrameData *)mm_malloc(ALARM_E001_TOTAL_FILES*sizeof(FrameData));
    int i = 0;
    // init frame data
    for (i = 0; i < ALARM_E001_TOTAL_FILES ; i++)
    {
        //W/A get address of data ptr, b/c data ptr is init after this called
        videoData_E001.frameData[i].data = (uint8_t*)&ALARM_E001_FRAME_DATA_PTR(i);
        videoData_E001.frameData[i].size = ALARM_E001_FRAME_DATA_SIZE(i);
    }
    
    //init alarm E002
    videoData_E002.x = ALARM_VIDEO_POS_X;
    videoData_E002.y = ALARM_VIDEO_POS_Y;
    videoData_E002.w = ALARM_VIDEO_WIDTH;
    videoData_E002.h = ALARM_VIDEO_HEIGHT;
    videoData_E002.bytesPerPixel = BYTES_PER_PIXEL;
    videoData_E002.frameRateMS = FRAME_RATE_MS;
    videoData_E002.videoSizeInBytes = ALARM_VIDEO_WIDTH*ALARM_VIDEO_HEIGHT*BYTES_PER_PIXEL*ALARM_E002_TOTAL_FILES;
    videoData_E002.frameData = (FrameData *)mm_malloc(ALARM_E002_TOTAL_FILES*sizeof(FrameData));
    
    // init frame data
    for (i = 0; i < ALARM_E002_TOTAL_FILES ; i++)
    {
        videoData_E002.frameData[i].data = (uint8_t*)&ALARM_E002_FRAME_DATA_PTR(i);
        videoData_E002.frameData[i].size = ALARM_E002_FRAME_DATA_SIZE(i);
    }
    
    //init alarm E003
    videoData_E003.x = ALARM_VIDEO_POS_X;
    videoData_E003.y = ALARM_VIDEO_POS_Y;
    videoData_E003.w = ALARM_VIDEO_WIDTH;
    videoData_E003.h = ALARM_VIDEO_HEIGHT;
    videoData_E003.bytesPerPixel = BYTES_PER_PIXEL;
    videoData_E003.frameRateMS = FRAME_RATE_MS;
    videoData_E003.videoSizeInBytes = ALARM_VIDEO_WIDTH*ALARM_VIDEO_HEIGHT*BYTES_PER_PIXEL*ALARM_E003_TOTAL_FILES;
    videoData_E003.frameData = (FrameData *)mm_malloc(ALARM_E003_TOTAL_FILES*sizeof(FrameData));
    
    // init frame data
    for (i = 0; i < ALARM_E003_TOTAL_FILES ; i++)
    {
        videoData_E003.frameData[i].data = (uint8_t*)&ALARM_E003_FRAME_DATA_PTR(i);
        videoData_E003.frameData[i].size = ALARM_E003_FRAME_DATA_SIZE(i);;
    }
    
}

void AlarmExpression_Active(E_AlarmId id, E_AlarmPriority priority)
{
    DisplayControl_SetState(eMainScreenIsShowingDispState);
    
    AlarmExpression_SetAlarmState(eInitAlarmState);
        
    gs_alarmData.id = id;
    gs_alarmData.currentPriority = priority;
    gs_alarmData.status = eActive;
    
    AlarmExpression_SetAlarmPopupAutoShowAtInit(true);
    AlarmExpression_SetAlarmMuteIconShowFlag(false);
    AlarmExpression_SetReset(false);
    AlarmExpression_SetxShowAlarmDetailTitleTick(0);
}

void AlarmExpression_SetAlarmPriority(E_AlarmPriority priority)
{
    gs_alarmData.currentPriority = priority;
}

E_AlarmPriority AlarmExpression_GetAlarmPriority()
{
    return gs_alarmData.currentPriority;
}

void AlarmExpression_Inactive()
{
    gs_alarmData.status = eInactive;
}

/**@brief set tick of alarm detail title showing
 * @param [in]: TickType_t val value of tick that wants to set
 * @param [out]: None
 * @return None
 */
void AlarmExpression_SetxShowAlarmDetailTitleTick(TickType_t val)
{
    xShowAlarmDetailTitleTick = val;
}

/**@brief set tick of alarm detail message showing
 * @param [in]: TickType_t val value of tick that wants to set
 * @param [out]: None
 * @return None
 */
void AlarmExpression_SetShowAlarmDetailMessageTick(TickType_t val)
{
    xDetailMessageTick = val;
}

/**@brief set tick of alarm additional message showing
 * @param [in]: TickType_t val value of tick that wants to set
 * @param [out]: None
 * @return None
 */
void AlarmExpression_SetShowAlarmAdditionalMessageTick(TickType_t val)
{
    xAdditionalMessageTick = val;
}

/**@brief Get new tick count when have action of touch screen
 * @param [in]: None
 * @param [out]: None
 * @return None
 */
void AlarmExpression_ResetTouchScreenNoActionTickCounter(void)
{
    gs_alarmData.xTouchingScreenNoActionTick = xTaskGetTickCount();
}

/**@brief Check no action on touch screen timeout
 * @param [in]: None
 * @param [out]: None
 * @retval true if no action on touch screen timeout
 * @retval false if no action on touch screen not timeout
 */
bool AlarmExpression_CheckTouchScreenNoActionTimeout(void)
{
    TickType_t delta = DisplayControl_CalculateDeltaTick(xTaskGetTickCount(), gs_alarmData.xTouchingScreenNoActionTick);
    
    if (delta >= ALARM_POPUP_TOUCHSCREEN_NO_ACTION_TIMEOUT)
    {
        gs_alarmData.xTouchingScreenNoActionTick = 0;
        return true;
    }
    return false;
}

void AlarmExpression_ShowPopup()
{
    AlarmExpression_SetxShowAlarmDetailTitleTick(0);
    alarmVideoControl.frameIndex = alarmVideoControl.frameTotal;
    
    gs_alarmData.alarmState = eShowAlarmPopupAlarmState;
}

void AlarmExpression_HidePopup()
{
    gs_alarmData.alarmState = eHideAlarmPopupAlarmState;
}

bool AlarmExpression_IsPopupShow()
{
    return isAlarmPopupShow;
}

/**@brief set state of alarm detail title showing
 * @param [in]: bool flag show or not
 * @param [out]: None
 * @return None
 */
void AlarmExpression_SetShowAlarmDetailTitle(bool flag)
{
    isAlarmDetailTitleShow = flag;
}

/**@brief set state of alarm detail title showing
 * @param [in]: bool flag show or not
 * @param [out]: None
 * @retval true if want to show alarm detail title
 * @retval false if don't want to show alarm detail title
 */
bool AlarmExpression_GetShowAlarmDetailTitle()
{
    return isAlarmDetailTitleShow;
}

/**@brief set state of alarm detail title showing
 * @param [in]: None
 * @param [out]: None
 * @retval true show alarm detail title done
 * @retval false show alarm detail title not done
 */
bool AlarmExpression_IsShowAlarmDetailTitleDone()
{
//    SYS_PRINT("\n\nShow ALarm Title popup: %d - [%d] - state [%d]", xShowAlarmDetailTitleTick, AlarmExpression_GetShowAlarmDetailTitle(), gs_alarmData.alarmState);
    if (AlarmExpression_GetShowAlarmDetailTitle())
    {
        SYS_PRINT("\nShow title");
        if (xShowAlarmDetailTitleTick == 0) 
        {
            xShowAlarmDetailTitleTick = xTaskGetTickCount();
            
            laWidget_SetVisible((laWidget*)lbAlarmInfomation, LA_TRUE);
            laLabelWidget_SetHAlignment(lbAlarmInfomation, LA_HALIGN_CENTER);
            laLabelWidget_SetText(lbAlarmInfomation, laString_CreateFromID(AlarmExpression_GetAlarmDetailTitleStringId(gs_alarmData.id)));
            laWidget_Invalidate((laWidget*)rectAlarmArea);
        }

        if (xTaskGetTickCount() - xShowAlarmDetailTitleTick >= ALARM_TITLE_TIMEOUT)
        {
            laWidget_SetVisible((laWidget*)lbAlarmInfomation, LA_FALSE);
//            laLabelWidget_SetText(lbAlarmInfomation, laString_CreateFromID(string_text_Nullstring));
            AlarmExpression_SetShowAlarmDetailTitle(false);
            alarmVideoControl.frameIndex = 0;
            AlarmExpression_SetShowAlarmDetailMessage(true);
            AlarmExpression_SetShowAlarmAdditionalMessage(true);
        }
    }
    return !AlarmExpression_GetShowAlarmDetailTitle();
}

/**@brief Check valid of alarm detail Message
 * @param [in]: None
 * @param [out]: None
 * @retval true if message is valid
 * @retval false if message is invalid
 */
bool AlarmExpression_IsShowAlarmDetailMessage()
{
    if (AlarmExpression_GetAlarmMessageStringId(gs_alarmData.id) != string_text_Nullstring)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**@brief Set showing state of alarm detail message
 * @param [in]: bool is showing or not
 * @param [out]: None
 * @return None
 */
void AlarmExpression_SetShowAlarmDetailMessage(bool flag)
{
    isDetailMessageShow = flag;
}

/**@brief Get state of alarm detail message showing
 * @param [in]: None
 * @param [out]: None
 * @retval true if message is showing
 * @retval false if message is not showing
 */
bool AlarmExpression_GetShowAlarmDetailMessage()
{
    if (!AlarmExpression_IsShowAlarmDetailMessage())
        return false;
    return isDetailMessageShow;
}

/**@brief Set showing state of alarm Additional message
 * @param [in]: bool is showing or not
 * @param [out]: None
 * @return None
 */
void AlarmExpression_SetShowAlarmAdditionalMessage(bool flag)
{
    isAdditionalMessageShow = flag;
}

/**@brief Get state of alarm Additional message showing
 * @param [in]: None
 * @param [out]: None
 * @retval true if message is showing
 * @retval false if message is not showing
 */
bool AlarmExpression_GetShowAlarmAdditionalMessage()
{
    if (!gs_alarmData.alarmAdditionalMessageEnable)
        return false;
    return isAdditionalMessageShow;
}

/**@brief Get message timing
 * @param [in]: None
 * @param [out]: None
 * @return uint16_t time of showing message
 */
uint16_t AlarmExpression_GetMessageTime()
{
    if (AlarmExpression_IsShowAlarmDetailMessage() && gs_alarmData.alarmAdditionalMessageEnable)
    {
        return MESSAGE_TIMEOUT_5S;
    }
    else
    {
        return MESSAGE_TIMEOUT_10S;
    }
}

/**@brief Show alarm message
 * @param [in]: None
 * @param [out]: None
 * @retval true alarm message is show completely
 * @retval false alarm message is still showing
 */
bool AlarmExpression_ShowAlarmMessage()
{
    static uint16_t gs_messageDisplayTime = 0;
    
    gs_messageDisplayTime = AlarmExpression_GetMessageTime();
    
    if (!AlarmExpression_IsShowAlarmDetailMessage() && !gs_alarmData.alarmAdditionalMessageEnable)
    {
        return true;
    }
    if (AlarmExpression_GetShowAlarmDetailMessage())
    {
        if (xDetailMessageTick == 0)
        {
            xDetailMessageTick = xTaskGetTickCount();
            AlarmExpression_SetShowAlarmAdditionalMessage(false);
            
            laWidget_SetVisible((laWidget*)lbAlarmInfomation, LA_TRUE);
            laWidget_SetPosition((laWidget*)lbAlarmInfomation, 24, 1);
            laLabelWidget_SetHAlignment(lbAlarmInfomation, LA_HALIGN_LEFT);
            laLabelWidget_SetText(lbAlarmInfomation, laString_CreateFromID(AlarmExpression_GetAlarmMessageStringId(gs_alarmData.id)));
            laWidget_Invalidate((laWidget*)rectAlarmArea);
        }
        
        if (xTaskGetTickCount() - xDetailMessageTick >= gs_messageDisplayTime)
        {
            laWidget_SetPosition((laWidget*)lbAlarmInfomation, 9, 1);
            laLabelWidget_SetHAlignment(lbAlarmInfomation, LA_HALIGN_CENTER);
//            laLabelWidget_SetText(lbAlarmInfomation, laString_CreateFromID(string_text_Nullstring));
            laWidget_SetVisible((laWidget*)lbAlarmInfomation, LA_FALSE);
            AlarmExpression_SetShowAlarmDetailMessage(false);
            AlarmExpression_SetShowAlarmAdditionalMessage(true);
            
            if (!gs_alarmData.alarmAdditionalMessageEnable)
            {
                return true;
            }
        }
        else
        {
        }
    }
    
    if (AlarmExpression_GetShowAlarmAdditionalMessage())
    {
        if (xAdditionalMessageTick == 0)
        {
            xAdditionalMessageTick = xTaskGetTickCount();
            AlarmExpression_SetShowAlarmDetailMessage(false);
            
            laWidget_SetVisible((laWidget*)lbAlarmInfomation, LA_TRUE);
            laLabelWidget_SetText(lbAlarmInfomation, laString_CreateFromID(string_AlarmMessage_AdditionalMessage_RemoveTheCannula));
            laWidget_Invalidate((laWidget*)rectAlarmArea);
        }
        
        if (xTaskGetTickCount() - xAdditionalMessageTick >= gs_messageDisplayTime)
        {
            laWidget_SetVisible((laWidget*)lbAlarmInfomation, LA_FALSE);
//            laLabelWidget_SetText(lbAlarmInfomation, laString_CreateFromID(string_text_Nullstring));
            AlarmExpression_SetShowAlarmDetailMessage(false);
            AlarmExpression_SetShowAlarmAdditionalMessage(false);
          
            return true;
        }
        else
        {
            
        }
    }
    return false;
}
void AlarmExpression_SetIndicatorVisible(bool flag)
{
    laBool f;
    if (flag) {
        f = LA_TRUE;
    }
    else {
        f = LA_FALSE;
    }
    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
    {
        laWidget_SetVisible((laWidget*)panelLeftAlarm, f);
        laWidget_SetVisible((laWidget*)panelRightAlarm, f);
        laWidget_SetVisible((laWidget*)panelBottomAlarm, f);
//        laWidget_SetVisible((laWidget*)imgAlarmHandIndicator, f);
    }
    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        laWidget_SetVisible((laWidget*)SC_AlarmIndicatorLeftPanel, f);
        laWidget_SetVisible((laWidget*)SC_AlarmIndicatorRightPanel, f);
        laWidget_SetVisible((laWidget*)SC_AlarmIndicatorBottomPanel, f);
//        laWidget_SetVisible((laWidget*)SC_AlarmHandIndicator, f);
    }
    else{
        return;
    }
}

void AlarmExpression_StartIndicatorTimer(uint16_t freq) 
{
    if(!isIndicationTimerStart)
    {
        s_handleAlarmIndicatorBlinking = SYS_TMR_CallbackPeriodic(
                freq, /*(ms) - freq 0.5, dutycyle 50%*/
                1,
                AlarmExpression_FlashingCallback);                  
        isIndicationTimerStart = true;
    }
}
void AlarmExpression_StopIndicatorTimer() 
{
    if (isIndicationTimerStart)
    {
        SYS_TMR_CallbackStop(s_handleAlarmIndicatorBlinking);        
        isIndicationTimerStart = false;
    }
}

void AlarmExpression_SetAlarmIndicatorHighPriority(void) 
{
    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
    {
        laWidget_SetScheme(panelLeftAlarm, &JFLO_AlarmIndicatorHighPriority_Scheme);
        laWidget_SetScheme(panelRightAlarm, &JFLO_AlarmIndicatorHighPriority_Scheme);
        laWidget_SetScheme(panelBottomAlarm, &JFLO_AlarmIndicatorHighPriority_Scheme);    
    }
    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        laWidget_SetScheme(SC_AlarmIndicatorLeftPanel, &JFLO_AlarmIndicatorHighPriority_Scheme);
        laWidget_SetScheme(SC_AlarmIndicatorRightPanel, &JFLO_AlarmIndicatorHighPriority_Scheme);
        laWidget_SetScheme(SC_AlarmIndicatorBottomPanel, &JFLO_AlarmIndicatorHighPriority_Scheme);    
    }
    else{
        return;
    }
    AlarmExpression_StopIndicatorTimer();
    AlarmExpression_StartIndicatorTimer(250);                


}

void AlarmExpression_SetAlarmIndicatorMediumPriority(void) 
{
    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
    {
        laWidget_SetScheme(panelLeftAlarm, &JFLO_AlarmIndicatorMediumPriority_Scheme);
        laWidget_SetScheme(panelRightAlarm, &JFLO_AlarmIndicatorMediumPriority_Scheme);
        laWidget_SetScheme(panelBottomAlarm, &JFLO_AlarmIndicatorMediumPriority_Scheme);
    }
    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        laWidget_SetScheme(SC_AlarmIndicatorLeftPanel, &JFLO_AlarmIndicatorMediumPriority_Scheme);
        laWidget_SetScheme(SC_AlarmIndicatorRightPanel, &JFLO_AlarmIndicatorMediumPriority_Scheme);
        laWidget_SetScheme(SC_AlarmIndicatorBottomPanel, &JFLO_AlarmIndicatorMediumPriority_Scheme);    
    }
    else{
        return;
    }
        
    AlarmExpression_StopIndicatorTimer();
    AlarmExpression_StartIndicatorTimer(1000);    


    
}

void AlarmExpression_SetAlarmIndicatorLow1Priority(void) 
{
    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
    {
        laWidget_SetScheme(panelLeftAlarm, &JFLO_AlarmIndicatorLow1Priority_Scheme);
        laWidget_SetScheme(panelRightAlarm, &JFLO_AlarmIndicatorLow1Priority_Scheme);
        laWidget_SetScheme(panelBottomAlarm, &JFLO_AlarmIndicatorLow1Priority_Scheme);
    }
    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        laWidget_SetScheme(SC_AlarmIndicatorLeftPanel, &JFLO_AlarmIndicatorLow1Priority_Scheme);
        laWidget_SetScheme(SC_AlarmIndicatorRightPanel, &JFLO_AlarmIndicatorLow1Priority_Scheme);
        laWidget_SetScheme(SC_AlarmIndicatorBottomPanel, &JFLO_AlarmIndicatorLow1Priority_Scheme);    
    }
    else{
        return;
    }
        
    AlarmExpression_StopIndicatorTimer();
}

void AlarmExpression_SetAlarmIndicatorLow2Priority(void) 
{
    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
    {
        laWidget_SetScheme(panelLeftAlarm, &JFLO_AlarmIndicatorLow2Priority_Scheme);
        laWidget_SetScheme(panelRightAlarm, &JFLO_AlarmIndicatorLow2Priority_Scheme);
        laWidget_SetScheme(panelBottomAlarm, &JFLO_AlarmIndicatorLow2Priority_Scheme);
    }
    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        laWidget_SetScheme(SC_AlarmIndicatorLeftPanel, &JFLO_AlarmIndicatorLow2Priority_Scheme);
        laWidget_SetScheme(SC_AlarmIndicatorRightPanel, &JFLO_AlarmIndicatorLow2Priority_Scheme);
        laWidget_SetScheme(SC_AlarmIndicatorBottomPanel, &JFLO_AlarmIndicatorLow2Priority_Scheme);    
    }
    else{
        return;
    }
        
    AlarmExpression_StopIndicatorTimer();
}

void AlarmExpression_FlashingCallback ( uintptr_t context, uint32_t currTick)
{
    static bool temp_blinking = false;
    temp_blinking = !temp_blinking;
    AlarmExpression_SetIndicatorVisible(temp_blinking);
    return;
}

void AlarmExpression_SetAlarmPopupShowFlag(bool flag)
{
    isAlarmPopupShow = flag;
}
bool AlarmExpression_GetAlarmPopupShowFlag()
{
    return isAlarmPopupShow;
}

void AlarmExpression_SetAlarmMuteIconShowFlag(bool flag)
{
    isAlarmMuteIconShow = flag;
}
bool AlarmExpression_GetAlarmMuteIconShowFlag()
{
    return isAlarmMuteIconShow;
}

#define FRAME_DECODE_DELAY 2 // delay playing for waiting decoding first frame
void AlarmExpression_FrameUpdateCallback ( uintptr_t context, uint32_t currTick)
{ 
//    SYS_PRINT("VideoScreen_FrameUpdateCallback %d (ms) index %d \n", xTaskGetTickCount() - xCallbackTick, alarmVideoControl.frameIndex);
    xCallbackTick = xTaskGetTickCount();
    if (alarmVideoControl.frameIndex >= alarmVideoControl.frameTotal)
    {
        return;
    }
    if (alarmVideoControl.frameReady <  alarmVideoControl.frameTotal && 
            alarmVideoControl.frameIndex + FRAME_DECODE_DELAY >= alarmVideoControl.frameReady)
    {
        SYS_PRINT("Error : frame invalid %d %d %d \n", alarmVideoControl.frameIndex, alarmVideoControl.frameReady, alarmVideoControl.frameTotal);
        return;
    }
    
    TickType_t xGuiTick = xTaskGetTickCount();
//    SYS_PRINT("VideoControl_FrameUpdate %d/%d  \n", alarmVideoControl.frameIndex ,alarmVideoControl.frameTotal);   
    
    unsigned long line;
    unsigned long frame_add = alarmVideoControl.frameIndex * alarmVideoControl.frameSizeInBytes ;
//    SYS_PRINT("frame_add %d \n" ,frame_add) ;
    unsigned long dest_add = 0;
    unsigned long input_add = 0;
    for (line = 0 ; line < alarmVideoControl.h; line++)
    {
        dest_add = (alarmVideoControl.x + (alarmVideoControl.y + line ) * alarmVideoControl.screenWidth) * alarmVideoControl.bytesPerPixel;
        input_add = frame_add + line * alarmVideoControl.frameLineInBytes;
//        SYS_PRINT("line %d dest_add %d input_add %d \n" ,line, dest_add, input_add );
        // protect overflow
        if ( dest_add + alarmVideoControl.frameLineInBytes > alarmVideoControl.screenSizeInBytes) 
        {
            SYS_PRINT("error line %d dest_add %d input_add %d \n" ,line, dest_add, input_add );
            break;
        }
        if ( input_add + alarmVideoControl.frameLineInBytes > alarmVideoControl.inputSizeInBytes)
        {
            SYS_PRINT("error line %d dest_add %d input_add %d \n" ,line, dest_add, input_add );
            break;
        }
        if ( alarmVideoControl.outputBuffer > 0 && alarmVideoControl.outputBuffer1 > 0)
        {
            VideoControl_MemCopy(alarmVideoControl.outputBuffer + dest_add, 
                    alarmVideoControl.inputBuffer + input_add, 
                    alarmVideoControl.frameLineInBytes);
            VideoControl_MemCopy(alarmVideoControl.outputBuffer1 + dest_add, 
                    alarmVideoControl.inputBuffer + input_add, 
                    alarmVideoControl.frameLineInBytes);    
        }
        else
        {
            SYS_PRINT("error inputBuffer, outputBuffer");
        }
    }
//    SYS_PRINT("dest_add %d \n" ,dest_add) ;
//    SYS_PRINT("input_add %d \n" ,input_add) ;
      
    if (alarmVideoControl.frameIndex < alarmVideoControl.frameTotal )
    {
        alarmVideoControl.frameIndex++;
    }
    xGuiTick = xTaskGetTickCount() - xGuiTick;
    if (xGuiTick > 10)
        SYS_PRINT("AlarmExpression_FrameUpdateCallback %d\n",xGuiTick); 
}

/** @brief Handle the behavior when AlarmReporing Process is working */
void AlarmExpression_Run(void) 
{
//    SYS_PRINT("\nAlarm state: [%d]", gs_alarmData.alarmState);
    switch (gs_alarmData.alarmState)
    {
        case eInactiveAlarmState:
        {
            SYS_PRINT("\n eInactiveAlarmState ");
        }
            break;
        case eIdleAlarmState:     
//            SYS_PRINT("\n eIdleAlarmState ");
        {
            if (gs_alarmData.status == eInactive)
            {
                if (AlarmExpression_IsPopupShow())
                {
                    gs_alarmData.alarmState = eHideAlarmPopupAlarmState;
                }
                else
                {
                    gs_alarmData.alarmState = eCompleteAlarmState;
                }
                break;
            }
            
            if (AlarmExpression_IsReset())
            {
                gs_alarmData.alarmState = eHideAlarmPopupAlarmState;
                break;
            }
            if (AlarmExpression_IsPopupShow() && (GT911_GetTouchScreenState() != eTouchIdleStateID))
            {
                AlarmExpression_ResetTouchScreenNoActionTickCounter();
            }
            
            if(AlarmExpression_IsPopupShow() && AlarmExpression_CheckTouchScreenNoActionTimeout())
            {
                // Hide alarm popup
                gs_alarmData.alarmState = eHideAlarmPopupAlarmState;
            }
            
            if (AlarmExpression_GetAlarmPopupShowFlag())
            {
                if(!AlarmExpression_IsShowAlarmDetailTitleDone())
                    break;
                    
                if (alarmExpressionConfigList[gs_alarmData.id].alarmDetailAnimationEnable)
                {
                    //decode frame
                    if (alarmVideoControl.frameReady < alarmVideoControl.frameTotal)            
                    {
                        uint32_t fileSize = alarmExpressionConfigList[gs_alarmData.id].alarmAnimationData->frameData[alarmVideoControl.frameReady].size;
                        // W/A init function called before file data loaded, so used the pointer
                        uint8_t **compressedBufferAddr = (uint8_t **)alarmExpressionConfigList[gs_alarmData.id].alarmAnimationData->frameData[alarmVideoControl.frameReady].data;
                        TickType_t xGuiTick = xTaskGetTickCount();
                        VideoControl_DecodeFrame(&alarmVideoControl, alarmVideoControl.frameReady, *compressedBufferAddr, fileSize);
                        xGuiTick = xTaskGetTickCount() - xGuiTick;
                        if (xGuiTick > FRAME_RATE_MS )
                        {
                            SYS_PRINT("VideoControl_DecodeLzma (tick) %d alarmVideoControl.frameReady %d \n",xGuiTick, alarmVideoControl.frameReady);
                        }
                        alarmVideoControl.frameReady++;
                    }

                    if (alarmVideoControl.frameIndex >= alarmVideoControl.frameTotal)
                    {
                        // Alarm show message
                        if (AlarmExpression_ShowAlarmMessage())
                        {
                            
                            AlarmExpression_SetShowAlarmAdditionalMessageTick(0);
                            AlarmExpression_SetShowAlarmDetailMessageTick(0);
                            AlarmExpression_SetxShowAlarmDetailTitleTick(0);
                            AlarmExpression_SetShowAlarmDetailTitle(true);
                        }
                    }
                }
                else
                {
                    // Alarm show message
                    if(AlarmExpression_ShowAlarmMessage())
                    {
                        AlarmExpression_SetShowAlarmAdditionalMessageTick(0);
                        AlarmExpression_SetShowAlarmDetailMessageTick(0);
                        AlarmExpression_SetxShowAlarmDetailTitleTick(0);
                        AlarmExpression_SetShowAlarmDetailTitle(true);
                    }
                }
            }
            AlarmExpression_UpdateAlarmMuteIcon();
        }           
            break;
        case eInitAlarmState:
        {
            SYS_PRINT("\nAlarm Init state");
            
            if (gs_alarmData.status == eInactive)
            {
                gs_alarmData.alarmState = eInactiveAlarmState;
                break;
            }
            
            if (AlarmExpression_IsReset())
            {
                AlarmExpression_UpdateAlarmTitle();
                laWidget_SetVisible((laWidget*) panelAlarmTitle, LA_TRUE);
                gs_alarmData.alarmState = eResetAlarmState;
                break;
            }
            
            
//            SYS_PRINT("\n eInitAlarmState ");
            AlarmExpression_SetIndicatorVisible(true);
            laWidget_SetVisible((laWidget*) panelAlarmTitle, LA_TRUE);
            AlarmExpression_UpdateAlarmTitle();
            AlarmExpression_UpdateAlarmIndicator();
            
            AlarmExpression_UpdateAlarmPopup();
            
            
            if (alarmExpressionConfigList[gs_alarmData.id].alarmDetailAnimationEnable)
            {
                AlarmExpression_VideoInit(&alarmVideoControl, alarmExpressionConfigList[gs_alarmData.id].alarmAnimationData);
            }
            // show popup if any
            
            if (AlarmExpression_GetAlarmPopupAutoShowAtInit())
            {
                gs_alarmData.alarmState = eShowAlarmPopupAlarmState; 
                AlarmExpression_SetAlarmPopupAutoShowAtInit(false);// false -> true;
            }
            else
            {
                gs_alarmData.alarmState = eIdleAlarmState; 
            }
        }
            break;
        case eShowAlarmPopupAlarmState:
        {
            if (!alarmExpressionConfigList[gs_alarmData.id].alarmPopupEnable)
            {
                gs_alarmData.alarmState = eIdleAlarmState;
                break;
            }
            
            AlarmExpression_ResetTouchScreenNoActionTickCounter();
            
            MainScreen_SetMonitorPopupVisible(false);
            MainScreen_SetSettingPopupVisible(false);
            MainScreen_SetSettingPopupShow(false);
            MainScreen_SetState(eIdleDispState);
            
            // This cancel the setting popup move in when the alarm popup running
            MainScreen_SetButtonPressedState(false);
            AnimationControl_StopAnim(g_ChangedSetpointArea_Anim);
            
            AnimationControl_StopAnim(g_AlarmBox_Anim_MoveIn);
            AnimationControl_StopAnim(g_AlarmBox_Anim_MoveOut);
            AnimationControl_StartAnim(g_AlarmBox_Anim_MoveIn);
            AlarmExpression_SetxShowAlarmDetailTitleTick(0);
            AlarmExpression_SetShowAlarmDetailTitle(true);
            
            gs_alarmData.alarmState = eShowHideAnimationRunningAlarmState;
        }
            break;
        case eHideAlarmPopupAlarmState:
        {
            if (alarmExpressionConfigList[gs_alarmData.id].alarmDetailAnimationEnable)
            {
                VideoControl_StopPlayVideo(&alarmVideoControl);
            }
            
            laWidget_SetVisible((laWidget*)lbAlarmInfomation, LA_FALSE);
            
            AlarmExpression_SetShowAlarmAdditionalMessageTick(0);
            AlarmExpression_SetShowAlarmDetailMessageTick(0);
            AlarmExpression_SetxShowAlarmDetailTitleTick(0);
            AlarmExpression_SetShowAlarmDetailTitle(true);
            
            
            MainScreen_SetButtonPressedState(false);
            AnimationControl_StopAnim(g_ChangedSetpointArea_Anim);
            AnimationControl_StopAnim(g_AlarmBox_Anim_MoveIn);
            AnimationControl_StopAnim(g_AlarmBox_Anim_MoveOut);
            AnimationControl_StartAnim(g_AlarmBox_Anim_MoveOut);
            gs_alarmData.alarmState = eShowHideAnimationRunningAlarmState;
        }
            break;
        case eShowHideAnimationRunningAlarmState:
        {
            // wait until animation complete
            if (!isAlarmPopupMoving)
            {
                if (isAlarmPopupShow)
                {
                    gs_alarmData.alarmState = eShowAlarmDetailTitle;//ePlayAnimationAlarmState;
                }
                else
                {
                    MainScreen_SetMonitorPopupVisible(true);
                    if (gs_alarmData.status == eInactive)
                    {
                        gs_alarmData.alarmState = eCompleteAlarmState;   
                    }
                    else if (AlarmExpression_IsReset())
                    {
                        AlarmExpression_Deinit();
                        laWidget_SetVisible((laWidget*) panelAlarmTitle, LA_TRUE);
                        gs_alarmData.alarmState = eResetAlarmState;
                    }
                    else
                    {
                        gs_alarmData.alarmState = eIdleAlarmState;   
                    }
                }
            }
        }
            break;
        case eShowAlarmDetailTitle:
        {   
            if (gs_alarmData.status == eInactive)
            {
                if (AlarmExpression_IsPopupShow())
                {
                    gs_alarmData.alarmState = eHideAlarmPopupAlarmState;
                }
                else
                {
                    gs_alarmData.alarmState = eCompleteAlarmState;
                }
                break;
            }
            
            if (AlarmExpression_IsReset())
            {
                gs_alarmData.alarmState = eHideAlarmPopupAlarmState;
                break;
            }
//            
//            laWidget_Invalidate((laWidget*)rectAlarmArea);
//            SYS_PRINT("\n\nShow ALarm Title popup: %d", xShowAlarmDetailTitleTick);
            if(AlarmExpression_IsShowAlarmDetailTitleDone())
            {
                gs_alarmData.alarmState = ePlayAnimationAlarmState;
            }
        }
            break;
        case ePlayAnimationAlarmState:
        {    
            if (alarmExpressionConfigList[gs_alarmData.id].alarmDetailAnimationEnable)
            {
                VideoControl_StartPlayVideo(&alarmVideoControl);
            }
            gs_alarmData.alarmState = eIdleAlarmState;
        }
            break;
        case eStopAnimationAlarmState:
            break;
        case eCompleteAlarmState:
        {
            SYS_PRINT("\n eCompleteAlarmState \n");
            AlarmExpression_Deinit();
        }
            break;
        case eResetAlarmState:
        {
            if (gs_alarmData.status == eInactive)
            {
                gs_alarmData.alarmState = eCompleteAlarmState;
                break;
            }
        }
            break;
        default:
            break;
    }
}

void AlarmExpression_Run_SettingScreen(void) 
{
    switch (gs_alarmData.alarmState)
    {
        case eInactiveAlarmState:
            break;
        case eIdleAlarmState:     
            if (gs_alarmData.status == eInactive)
            {
                gs_alarmData.alarmState = eCompleteAlarmState;
            }
            AlarmExpression_UpdateAlarmMuteIcon();           
            break;
        case eInitAlarmState:
            if (gs_alarmData.status == eInactive)
            {
                gs_alarmData.alarmState = eInactiveAlarmState;
                break;
            }
            AlarmExpression_SetIndicatorVisible(true);
            laWidget_SetVisible((laWidget*)SC_AlarmTitleBarPanel, LA_TRUE);
            AlarmExpression_UpdateAlarmTitle();
            AlarmExpression_UpdateAlarmIndicator();
            
            gs_alarmData.alarmState = eIdleAlarmState; 
            break;
        case eCompleteAlarmState:
            AlarmExpression_Deinit_SettingScreen();
            break;
        default:
            break;
    }
}
void AlarmExpression_Deinit_SettingScreen()
{
    AlarmExpression_StopIndicatorTimer();
    AlarmExpression_SetIndicatorVisible(false);
    laWidget_SetVisible((laWidget*)SC_AlarmTitleBarPanel, LA_FALSE);

    gs_alarmData.alarmState = eInactiveAlarmState;
}
void AlarmExpression_Deinit()
{
//    SYS_PRINT("\n AlarmExpression_Deinit() %d \n", gs_alarmData.id);
    if (alarmExpressionConfigList[gs_alarmData.id].alarmDetailAnimationEnable)
    {
        VideoControl_Deinit(&alarmVideoControl);
        // FIXME: this is repaint after the video stop
        laWidget_Invalidate((laWidget*)rectAlarmArea);
    }

    AlarmExpression_StopIndicatorTimer();
    AlarmExpression_SetIndicatorVisible(false);
    AlarmExpression_SetAlarmPopupShowFlag(false);
    laWidget_SetVisible((laWidget*) rectAlarmArea, LA_FALSE);
    laWidget_SetVisible((laWidget*) panelAlarmTitle, LA_FALSE);
    
//    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
//    {        
//    }
//    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
//    {
//        laWidget_SetVisible((laWidget*)SC_AlarmHandIndicator, LA_TRUE);
//    }
    
    gs_alarmData.alarmState = eInactiveAlarmState;
    if (!MainScreen_IsSettingPopupShow())
    {
        MainScreen_SetMonitorPopupVisible(true);
    }
}

void AlarmExpression_UpdateAlarmIndicator(void)
{
    switch(gs_alarmData.currentPriority)
    {
        case eHighPriority :
        {
            AlarmExpression_SetAlarmIndicatorHighPriority();
            break;
        }
        case eMediumPriority :
        {
            AlarmExpression_SetAlarmIndicatorMediumPriority();
            break;
        }
        case eLow1Priority :
        {
            AlarmExpression_SetAlarmIndicatorLow1Priority();
            break;
        }
        case eLow2Priority:
        {
            AlarmExpression_SetAlarmIndicatorLow2Priority();
            break;
        }
        default:
            break;

    }
}

void AlarmExpression_UpdateAlarmTitle(void)
{
    laLabelWidget* labelWidget;
    laWidget* panelWidget;
    laImageWidget* imgWidget;
    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
    {
        labelWidget = labelAlarmTitle;
        panelWidget = panelAlarmTitle;
    }
    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        labelWidget = SC_AlarmTitleLabel;
        panelWidget = SC_AlarmTitleBarPanel;
    }
    else{
        return;
    }
    laLabelWidget_SetText(labelWidget, laString_CreateFromID(AlarmExpression_GetAlarmTitleStringId(gs_alarmData.id)));
    laWidget_SetVisible((laWidget*)imgWidget, LA_TRUE);
//    SYS_PRINT("AlarmExpression_UpdateAlarmTitle pri %d \n", gs_alarmData.currentPriority);
    switch(gs_alarmData.currentPriority)
    {
        case eHighPriority :
        {
            laWidget_SetScheme((laWidget*)panelWidget, &JFLO_AlarmIndicatorHighPriority_Scheme);
            break;
        }
        case eMediumPriority :
        {
            laWidget_SetScheme((laWidget*)panelWidget, &JFLO_AlarmIndicatorMediumPriority_Scheme);
            break;
        }
        case eLow1Priority :
        {
            laWidget_SetScheme((laWidget*)panelWidget, &JFLO_AlarmIndicatorLow1Priority_Scheme);
            break;
        }
        case eLow2Priority:
        {
            laWidget_SetScheme((laWidget*)panelWidget, &JFLO_AlarmIndicatorLow2Priority_Scheme);
            break;
        }
        default:
            break;
    }
}

void AlarmExpression_UpdateAlarmMuteIcon(void)
{
    laWidget* widget;
    if (DisplayControl_GetActiveScreenIndex() == MainScreen_ID)
    {
        widget = (laWidget*)imageMuteIcon;
    }
    else if (DisplayControl_GetActiveScreenIndex() == SettingScreen_ID)
    {
        widget = (laWidget*)SC_AlarmMuteIcon;
    }
    else{
        return;
    }
    if (AlarmExpression_GetAlarmMuteIconShowFlag())
    {
        laWidget_SetVisible(widget, LA_TRUE);
    }
    else
    {
        laWidget_SetVisible(widget, LA_FALSE);
    }  
}

void AlarmExpression_UpdateAlarmPopup(void)
{
    if (gs_alarmData.id > eLastAlarmId || gs_alarmData.id < 0)
    return;

    laLabelWidget_SetText(lbAlarmInfomation, laString_CreateFromID(AlarmExpression_GetAlarmDetailTitleStringId(gs_alarmData.id)));
    laWidget_SetVisible((laWidget*)lbAlarmInfomation, LA_TRUE);     
    
    if (alarmExpressionConfigList[gs_alarmData.id].alarmPopupEnable)
    {
        laWidget_SetVisible((laWidget*)rectAlarmArea, LA_TRUE);
    }
    else
    {
        laWidget_SetVisible((laWidget*)rectAlarmArea, LA_FALSE);
        return;
    }
    
    AlarmExpression_UpdateResetButton();
    AlarmExpression_UpdateCloseButton();
}

void AlarmExpression_UpdateResetButton(){
    
    if (alarmExpressionConfigList[gs_alarmData.id].alarmResetButtonEnable || alarmExpressionConfigList[gs_alarmData.id].alarmOKButtonEnable)
    {
        laWidget_SetVisible((laWidget*)btnAlarmReset_AlarmArea, LA_TRUE);
        laWidget_SetX((laWidget*)btnX_AlarmArea, 48);//60->49
        if (alarmExpressionConfigList[gs_alarmData.id].alarmOKButtonEnable)
        {
            laButtonWidget_SetPressedImage(btnAlarmReset_AlarmArea, &ButtonRound_OK);
            laButtonWidget_SetReleasedImage(btnAlarmReset_AlarmArea, &ButtonRound_OK);
        }
        else
        {
            laButtonWidget_SetPressedImage(btnAlarmReset_AlarmArea, &ButtonRound_Reset);
            laButtonWidget_SetReleasedImage(btnAlarmReset_AlarmArea, &ButtonRound_Reset);
        }
    }
    else
    {
        laWidget_SetVisible((laWidget*)btnAlarmReset_AlarmArea, LA_FALSE);
        laWidget_SetX((laWidget*)btnX_AlarmArea, 88);//105->89
    }
    
    //this will override default config
    switch(gs_alarmData.alarmResetButtonStatus)
    {
        case eResetButtonHide:
            laWidget_SetVisible((laWidget*)btnAlarmReset_AlarmArea, LA_FALSE);
            laWidget_SetX((laWidget*)btnX_AlarmArea, 88);//105->89
            return;
        case eResetButtonShowReset:
            laWidget_SetVisible((laWidget*)btnAlarmReset_AlarmArea, LA_TRUE);
            laWidget_SetX((laWidget*)btnX_AlarmArea, 48);//60->49
            laButtonWidget_SetPressedImage(btnAlarmReset_AlarmArea, &ButtonRound_Reset);
            laButtonWidget_SetReleasedImage(btnAlarmReset_AlarmArea, &ButtonRound_Reset);
            return;
        case eResetButtonShowOK:        
            laWidget_SetVisible((laWidget*)btnAlarmReset_AlarmArea, LA_TRUE);
            laWidget_SetX((laWidget*)btnX_AlarmArea, 48);//60->49
            laButtonWidget_SetPressedImage(btnAlarmReset_AlarmArea, &ButtonRound_OK);
            laButtonWidget_SetReleasedImage(btnAlarmReset_AlarmArea, &ButtonRound_OK);
            return;
        case eResetButtonDefault:
        default:
            break;
    }
    
}

void AlarmExpression_UpdateCloseButton(){
    if (alarmExpressionConfigList[gs_alarmData.id].alarmCloseButtonEnable)
    {
        laWidget_SetVisible((laWidget*)btnX_AlarmArea, LA_TRUE);
    }
    else
    {
        laWidget_SetVisible((laWidget*)btnX_AlarmArea, LA_FALSE);
    }
}

uint32_t AlarmExpression_GetAlarmTitleStringId(E_AlarmId id)
{
    if (id > eLastAlarmId || id < 0)
        return string_text_Nullstring;
    
    if (alarmExpressionConfigList[id].alarmTitleStrId < 0)
        return string_text_Nullstring;
    
    return alarmExpressionConfigList[id].alarmTitleStrId;
}

uint32_t AlarmExpression_GetAlarmMessageStringId(E_AlarmId id)
{
    if (id > eLastAlarmId || id < 0)
        return string_text_Nullstring;

    if (alarmExpressionConfigList[id].alarmDetailMsgStrStartId < 0)
        return string_text_Nullstring;

    if (alarmExpressionConfigList[id].alarmDetailMsgStrEndId < 0)
        return string_text_Nullstring;
    
    if (gs_alarmData.alarmDetailMsgStrOffsetId < 0)
        return string_text_Nullstring;

    if (alarmExpressionConfigList[id].alarmDetailMsgStrStartId + gs_alarmData.alarmDetailMsgStrOffsetId > alarmExpressionConfigList[id].alarmDetailMsgStrEndId)
        return string_text_Nullstring;

    return alarmExpressionConfigList[id].alarmDetailMsgStrStartId + gs_alarmData.alarmDetailMsgStrOffsetId;
}

uint32_t AlarmExpression_GetAlarmDetailTitleStringId(E_AlarmId id)
{
    if (id > eLastAlarmId || id < 0)
        return string_text_Nullstring;

    if (alarmExpressionConfigList[id].alarmDetailTiltleStrId < 0)
        return string_text_Nullstring;
    
    return alarmExpressionConfigList[id].alarmDetailTiltleStrId;
}

void AlarmExpression_AlarmAreaMoveInCallback(int state, int pos)
{
    int nextPos = 0;
    static int prePos;
    static int startPos;
    static int destPos;
    int distance = 0;
    
    switch(state){
        case GUI_ANIM_START:
            isAlarmPopupMoving = true;
            startPos = ALARM_AREA_DISPLAY_OUT_POS_X;
            prePos = startPos;
            destPos = ALARM_AREA_DISPLAY_IN_POS_X;

            laWidget_SetVisible((laWidget*)rectMonitoringArea, LA_FALSE);    
            break;
        case GUI_ANIM_RUNNING:
                nextPos = startPos + (double)(destPos - startPos)*( (double)pos / GUI_ANIM_RANGE);

                if (nextPos <= destPos)       
                    nextPos = destPos;           
                distance = nextPos - prePos;        

            if (nextPos != prePos)
            {               
                /* Move rectangle then minus now - pre to get distance and move other widget */
                laWidget_SetX((laWidget*)rectAlarmArea, nextPos);   
            }   
            /*Save*/
            prePos = nextPos;
            break;
        case GUI_ANIM_END:
//            SYS_PRINT("\n MAINSCREEN_STATE_END_ANIM");            
            AlarmExpression_SetAlarmPopupShowFlag(true);
            isAlarmPopupMoving = false;            
            break;
        default:
            break;
    }
}

void AlarmExpression_AlarmAreaMoveOutCallback(int state, int pos)
{
    int nextPos = 0;
    static int prePos;
    static int startPos;
    static int destPos;
    int distance = 0;
    
    switch(state){
        case GUI_ANIM_START:
//            SYS_PRINT("\n AlarmArea_MOVEIN");
            isAlarmPopupMoving = true;
            startPos = ALARM_AREA_DISPLAY_IN_POS_X;
            prePos = startPos;
            destPos = ALARM_AREA_DISPLAY_OUT_POS_X;
           
        case GUI_ANIM_RUNNING:
            nextPos = startPos + (double)(destPos - startPos)*( (double)pos / GUI_ANIM_RANGE);
            if (nextPos >= ALARM_AREA_DISPLAY_OUT_POS_X)
                nextPos = ALARM_AREA_DISPLAY_OUT_POS_X;                              
            distance = nextPos - prePos;
            if (nextPos != prePos)
            {
                
                /* Move rectangle then minus now - pre to get distance and move other widget */
                laWidget_SetX((laWidget*)rectAlarmArea, nextPos);   
            }   
            /*Save*/
            prePos = nextPos;
            break;
        case GUI_ANIM_END:
//            SYS_PRINT("\n MAINSCREEN_STATE_END_ANIM");
            isAlarmPopupMoving = false;
            AlarmExpression_SetAlarmPopupShowFlag(false);            
            break;
        default:
            break;
    }
}

void AlarmExpression_VideoInit(VideoControl * video_control, VideoData *videoData)
{
    // the screen is oriented to portrait, so the position is need to converted
    video_control->x = videoData->y; //ALARM_VIDEO_POS_Y; // 100;
    video_control->y = videoData->x; //ALARM_VIDEO_POS_X; //36; //(272 - 200) / 2
    video_control->w = videoData->h; //ALARM_VIDEO_HEIGHT; //120; 
    video_control->h = videoData->w; //ALARM_VIDEO_WIDTH; // 200;
    video_control->bytesPerPixel = videoData->bytesPerPixel; //BYTES_PER_PIXEL;
    video_control->frameRateMS = videoData->frameRateMS; //FRAME_RATE_MS;
    video_control->inputSizeInBytes = videoData->videoSizeInBytes; //ALARM_MAX_VIDEO_SIZE;
    video_control->callback = &AlarmExpression_FrameUpdateCallback;
    VideoControl_InitControl(video_control);                                
//    SYS_PRINT("inputSizeInBytes %d frameTotal %d \n" , video_control->inputSizeInBytes , video_control->frameTotal) ;
}


bool AlarmExpression_IsActiveState()
{
    return gs_alarmData.alarmState == eInactiveAlarmState ? false : true ;
}

void AlarmExpression_SetPended(bool f)
{
    isAlarmExpressionPended = f;
}

bool AlarmExpression_IsPended()
{
    return isAlarmExpressionPended;
}

void AlarmExpression_SetAlarmState(E_Alarm_States state)
{
    gs_alarmData.alarmState = state;
}

bool AlarmExpression_IsActiveStatus()
{
    return gs_alarmData.status == eInactive ? false : true ;
}

void AlarmExpression_SetAlarmAdditionalData(uint8_t* data)
{
    if (!data)
        return;
    
    if (data[0] == 1)
    {
        SYS_PRINT("alarmAdditionalMessageEnable true \n");
        gs_alarmData.alarmAdditionalMessageEnable = true;
    }
    else
    {
        gs_alarmData.alarmAdditionalMessageEnable = false;
    }
    gs_alarmData.alarmDetailMsgStrOffsetId = data[1];
    SYS_PRINT("alarmDetailMsgStrOffsetId %d\n", data[1]);
    gs_alarmData.alarmResetButtonStatus = data[2];
    SYS_PRINT("alarmResetButtonStatus %d\n", data[2]);
}

bool AlarmExpression_IsReset()
{
    return isAlarmReset;
}

void AlarmExpression_SetReset(bool f)
{
    isAlarmReset = f;
}

void AlarmExpression_ResetButtonFunc()
{
//    AlarmExpression_SetReset(true);
//    deviceInterface_SendAudioEvent(eStopPlay, 0); 
    DisplayControl_PausedCurrentAlarm();
}

void AlarmExpression_ResetButtonFunc_E005()
{
    switch(gs_alarmData.alarmDetailMsgStrOffsetId)
    {
        case eE005AdultCircuitToPediatricMode:
            setting_SetMode(eChildMode);
            AlarmExpression_Inactive();
            break;
        case eE005PediatricCircuitToAdultMode:
            setting_SetMode(eAdultMode);
            AlarmExpression_Inactive();
            break;
        case eE005AdultCircuitToDisinfectionMode:
        case eE005PediatricCircuitToDisinfectionMode:
        case eE005DisinfectionCircuitToAdultMode:
        case eE005DisinfectionCircuitToPediatricMode:
            break;
        default:
            SYS_PRINT("AlarmExpression_OKButtonHandler_E005 error %d\n", gs_alarmData.alarmDetailMsgStrOffsetId);
            break;
    }
}

void AlarmExpression_CloseButtonPressed()
{
    SYS_PRINT("AlarmExpression_CloseButtonPressed \n");
    AlarmExpression_HidePopup();
}

void AlarmExpression_ResetButtonPressed()
{
    SYS_PRINT("AlarmExpression_ResetButtonPressed \n");
    if (gs_alarmData.id > eLastAlarmId || gs_alarmData.id < 0)
    {
        SYS_PRINT("AlarmExpression_ResetButtonPressed error %d \n", gs_alarmData.id);
        return;
    }
    if (alarmExpressionConfigList[gs_alarmData.id].alarmResetButtonFunc)
    {
        alarmExpressionConfigList[gs_alarmData.id].alarmResetButtonFunc();
    }
    else
    {
        SYS_PRINT("AlarmExpression_ResetButtonPressed null function pointer \n");
    }
}

bool AlarmExpression_GetAlarmPopupAutoShowAtInit()
{
    return gs_alarmData.isAlarmPopupAutoShowAtInit;
}

void AlarmExpression_SetAlarmPopupAutoShowAtInit(bool f)
{
    gs_alarmData.isAlarmPopupAutoShowAtInit = f;
}

/* end of file */

