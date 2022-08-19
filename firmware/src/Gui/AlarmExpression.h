/** @file AlarmScreen.h
 *  @brief 
 *  @author Trac Truong
 */

#ifndef _ALARM_EXPRESION
#define _ALARM_EXPRESION

/* This section lists the other files that are included in this file.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef UNIT_TEST
#include "system_config.h"
#include "system_definitions.h"
#include "gfx/hal/inc/gfx_context.h"
#endif
#include "Gui/DisplayControl.h"
#include "Gui/AnimationControl.h"
#include "VideoControl.h"

#include "mm.h"

#define FRAMES_PER_SEC                      8 //10
#define FRAME_RATE_MS                       1000/FRAMES_PER_SEC
#define FRAMESTEP_INC                       1
#define VIDEO_LAYER_ID                      0
#define BYTES_PER_PIXEL                     2
#define ALARM_VIDEO_WIDTH                   200
#define ALARM_VIDEO_HEIGHT                  200
#define ALARM_VIDEO_POS_X                   36
#define ALARM_VIDEO_POS_Y                   85

#define ALARM_MAX_VIDEO_SIZE 4400000 // this is max data among alarms 
#define ADDITIONAL_MESSAGE_TIMEOUT 5000

#define ALARM_AREA_DISPLAY_IN_POS_X 12
#define ALARM_AREA_DISPLAY_OUT_POS_X 275

//#define JFLO_DEBUG_ALARM

typedef void (*void_func_p)();

typedef struct {    
    int alarmTitleStrId; /**< Alarm title string start id */
    int alarmDetailMsgStrStartId; /**< Alarm detail message string start id */
    int alarmDetailMsgStrEndId; /**< Alarm detail message string start id */
    bool alarmPopupEnable; /**< Alarm popup enable */
    bool alarmCloseButtonEnable; /**< Alarm close button enable */
    bool alarmResetButtonEnable; /**< Alarm reset button enable */
 
    bool alarmDetailAnimationEnable; /**< Alarm detail animation enable */
    VideoData *alarmAnimationData; /**< Alarm animation data */
    bool alarmOKButtonEnable; /**< Alarm ok button enable */
    void_func_p alarmResetButtonFunc; /**< Alarm ok button handler */
} AlarmExpressionConfig;

/** @brief Define the alarm states */
typedef enum
{
	/* Application's state machine's initial state. */
    eIdleAlarmState = 0,        /**< Alarm is active, idle state */
    eInitAlarmState,            /**< Act the initialization of alarm, this call when new alarm id active */
    eShowAlarmPopupAlarmState,       /**< the alarm popup showing */
    eHideAlarmPopupAlarmState,       /**< the alarm popup hiding */
    eShowHideAnimationRunningAlarmState,       /**< the popup animation running */
    ePlayAnimationAlarmState,               /**< Play animation */
    eStopAnimationAlarmState,               /**< Stop animation */
    eInactiveAlarmState,        /**< Alarm is inactive */
    eResetAlarmState,        /**< Alarm only display title */
    eCompleteAlarmState,        /**< Alarm deinitialization */
	/* TODO: Define states used by the application state machine. */
} E_Alarm_States;

/** @brief Define the alarm variable to control the alarm process in mainscreen */ 
typedef struct
{
    E_AlarmId id; /**< Alarm id */
    E_AlarmPriority currentPriority; /**< Alarm priority */
    E_AlarmStatus status; /**< Alarm status */
    E_Alarm_States alarmState; /**< Alarm state */
    bool alarmAdditionalMessageEnable; /**< this store status for additional message */
    int alarmDetailMsgStrOffsetId; /**< Alarm detail message offset id (alarm with multiple conditions) */
    int alarmResetButtonStatus; /**< this store status for overriding the ResetButton display */
    bool isAlarmPopupAutoShowAtInit; /**< alarm popup auto show at init */
} Alarm_Data_Struct;

/** @brief A varible to store the alarm control video */
VideoControl alarmVideoControl;

/** @brief A varible to store the alarm control elements */
Alarm_Data_Struct gs_alarmData;

/** @brief Belong to AlarmBox Animation, need to be create */   
GUI_ANIM_HANDLE g_AlarmBox_Anim_MoveIn;

/** @brief Belong to AlarmBox Animation, need to be create */   
GUI_ANIM_HANDLE g_AlarmBox_Anim_MoveOut;

/** @brief The timer for animation of Alarm Reporting State, need to be create */   
SYS_TMR_HANDLE s_handleAlarmIndicatorBlinking;

/** @brief The timer for animation of Alarm Reporting State, need to be create */   
bool isIndicationTimerStart = false;

/** @brief this resume alarm expression after change screen ( and reinit screen) */   
bool isAlarmExpressionPended = false; 

/** @brief this store status for mute icon */   
bool isAlarmMuteIconShow = false;

/** @brief this store status if alarm reset button pressed */   
bool isAlarmReset = false;

/** @brief this store status if alarm popup is show */   
bool isAlarmPopupShow = false ;

/** @brief this store status if alarm popup is moving */   
bool isAlarmPopupMoving = false;

/** @brief E001 alarm animation data */   
VideoData videoData_E001;

/** @brief E001 alarm animation data */   
VideoData videoData_E002;

/** @brief E001 alarm animation data */   
VideoData videoData_E003;

/** @brief E020 alarm animation data */
VideoData videoData_E020;

/** @brief E021 alarm animation data */
VideoData videoData_E021;

/** @brief E022 alarm animation data */
VideoData videoData_E022;

/** @brief E024 alarm animation data */
VideoData videoData_E024;

/** @brief E027 alarm animation data */
VideoData videoData_E027;

/** @brief E028 alarm animation data */
VideoData videoData_E028;

/** @brief E029 alarm animation data */
VideoData videoData_E029;

/** @brief E030 alarm animation data */
VideoData videoData_E030;

/** @brief This called when new alarm is active
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_Active(E_AlarmId id, E_AlarmPriority priority);

/** @brief This called when new alarm is inactive
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_Inactive();

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_ShowPopup();

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_HidePopup();

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetIndicator(int priority, int freq, int duty_cycle);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetTitle(int id);

/** @brief text or animation
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetDetail(int id);

/** @brief This manage the alarm expression display 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_Run();

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetIndicatorVisible(bool flag);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetAlarmSchemeHighPriority(void);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetAlarmSchemeMediumPriority(void);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetAlarmSchemeLow1Priority(void); 

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetAlarmSchemeLow2Priority(void); 


/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_FlashingCallback ( uintptr_t context, uint32_t currTick);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_AlarmAreaMoveInCallback(int state, int pos);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_AlarmAreaMoveOutCallback(int state, int pos);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_UpdateAlarmIndicator(void);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */

void AlarmExpression_StartIndicatorTimer(uint16_t freq) ;

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */

void AlarmExpression_StopIndicatorTimer() ;

/** @brief Init alarm popup video data
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_VideoInit(VideoControl * video_control, VideoData *videoData);

/** @brief init config data
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_InitData( void );

/** @brief init video data
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_InitVideoData( void );

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_UpdateAlarmTitle(void);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_UpdateAlarmPopup(void);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
uint32_t AlarmExpression_GetAlarmTitleStringId(E_AlarmId id);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
uint32_t AlarmExpression_GetAlarmMessageStringId(E_AlarmId id);

/** @brief this function handle disable and cleanup alarm expression
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_Deinit();

/** @brief this function handle disable and cleanup alarm expression
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */

bool AlarmExpression_IsActiveState();

/** @brief this function handle disable and cleanup alarm expression
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
bool AlarmExpression_IsPended();

/** @brief this 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetPended(bool f);

/** @brief this function reactive current alarm id
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetAlarmState(E_Alarm_States state);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
bool AlarmExpression_IsActiveStatus(void);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetAlarmMuteIconShowFlag(bool flag);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
bool AlarmExpression_GetAlarmMuteIconShowFlag();

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_UpdateAlarmMuteIcon(void);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetAlarmPriority(E_AlarmPriority priority);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
E_AlarmPriority AlarmExpression_GetAlarmPriority(void);


/** @brief this function handle addition data for alarm
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetAlarmAdditionalData(uint8_t* data);

/** @brief 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
bool AlarmExpression_IsReset();

/** @brief this 
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetReset(bool f);

/** @brief reset button function for common
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_ResetButtonFunc();

/** @brief reset button function for alarm E005
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_ResetButtonFunc_E005();

/** @brief update reset button display
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_UpdateResetButton();

/** @brief update close button display
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_UpdateCloseButton();


/** @brief close button pressed function, this function is used to link with button event
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_CloseButtonPressed();

/** @brief reset button pressed function, this function is used to link with button event
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_ResetButtonPressed();

/** @brief this function is used to display alarm in setting screen, only support alarm status bar, indicator
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_Run_SettingScreen(void);

/** @brief this function is used to deinit alarm in setting screen
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_Deinit_SettingScreen();

/** @brief AlarmExpression_SetAlarmPopupAutoShowAtInit
 *  @param [in]  None
 *  @param [out]  None
 *  @return bool
 */
bool AlarmExpression_GetAlarmPopupAutoShowAtInit();

/** @brief AlarmExpression_SetAlarmPopupAutoShowAtInit 
 *  @param [in]  bool f
 *  @param [out]  None
 *  @return None
 */
void AlarmExpression_SetAlarmPopupAutoShowAtInit(bool f);


//FIXME: alloc data to ddr section because lack of memory, with this using, variables is alway init with 0 value
__attribute__((section(".ddr_data"), space(prog))) 
AlarmExpressionConfig alarmExpressionConfigList[eNoOfAlarmId];

#endif	

// end of file
