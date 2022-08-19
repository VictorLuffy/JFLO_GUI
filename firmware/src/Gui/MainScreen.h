/** @file MainScreen.h
 *  @brief The screen provides graphic interface to update firmware
 *  @author Thanh Hiep Nguyen
 */

#ifndef MAINSCREEN_H
#define	MAINSCREEN_H

/* This section lists the other files that are included in this file.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "system_config.h"
#include "system_definitions.h"
#include "gfx/hal/inc/gfx_context.h"
#include "Gui/DisplayControl.h"
#include "Gui/AnimationControl.h"
#include "Setting.h"
#include "VideoControl.h"
#include "Gui/StatusBar.h"
#include "Gui/RunningBar.h"
    
/** @brief Define the maximum times which Temp & Flow button will blink in compeleted loading procress */    
#define     BLINK_TIMES                         2
 
/** @brief Define the maximum points on heartbeart graph */       
#define		MAX_POINT_NUM_ON_HEARBEATGRAPH      120

/** @brief Define the type of animation */
typedef enum
{
    eNoneAnimCalling,   /**< None animation is calling*/
    eMoveIn,            /**< Move widgets from virtual screen to actual screen*/
    eMoveOut,           /**< Move widgets from actual screen to virtual screen*/
    eBlink,
} E_AnimType;

/** @brief Define the num of touches */
typedef enum {
    eNoneTouchSingle = 0,   /**< None touch on left panel or right panel*/
    eTouchSingleOne,        /**< The first touch on the 1 panel if you touch left 2 then touch right 1 --> become none touch*/
    eTouchSingleTwo,        /**< The 2nd touch on the 1 panel if you touch left 2 then touch right 1 --> become none touch*/
    eTouchSingleThree,      /**< The 3rd touch on the 1 panel if you touch left 2 then touch right 1 --> become none touch*/
    eTouchSingleFour,       /**< The 4th touch on the 1 panel if you touch left 2 then touch right 1 --> become none touch*/
    eTouchSingleFive,       /**< The 5th touch on the 1 panel if you touch left 2 then touch right 1 --> become none touch*/
    eTouchSingleSix,        /**< The 6th touch on the 1 panel if you touch left 2 then touch right 1 --> become none touch*/
            
    eNoneTouchDouble = 10,        /**< None touch on left panel or right panel*/
    eTouchDoubleOne,    /**< The 1st touch on the left panel if then you touch left pnl continously --> eNoneTouchDouble*/
    eTouchDoubleTwo,     /**<The 1st touch on the right panel if then you touch right pnl continously --> eNoneTouchDouble*/
    eTouchDoubleThree,  /**< The 2nd touch on the left panel if then you touch left pnl continously --> eNoneTouchDouble*/
    eTouchDoubleFour,   /**< The 2nd touch on the right panel if then you touch right pnl continously --> eNoneTouchDouble*/
    eTouchDoubleFive,   /**< The 3rd touch on the left panel if then you touch left pnl  continously --> eNoneTouchDouble*/
    eTouchDoubleSix,   /**< The 3rd touch on the right panel if then you touch right pnl continously --> eNoneTouchDouble*/             
} E_Touch;

typedef enum
{
    eIdleWarmUpState,
    eInitWarmUpState,
    eRunningWarmUpState,
    eFinishedWarmUpState,
    eQuitWarmUpState,
    eNoOfWarmUpState,
} E_WarmUpState;
      
/** @brief Define the control variable in mainscreen */ 
typedef struct
{
    E_DisplayState state;
    E_DisplaySubstates subState;
    E_WarmUpState warmUpState;
    
    E_SettingId currentQualitySetting;
    
    int prePos;
    int destPos;
    int startPos;    
    
    E_AnimType animCalling[4];
    
    int TSettingValue, FSettingValue, OSettingValue, OSettingUpperLimitValue, OSettingLowerLimitValue;
    int TSettingPrevValue, FSettingPrevValue, OSettingPrevValue, OSettingUpperLimitPreValue, OSettingLowerLimitPreValue;
    
    bool isSettingPopupShow;
    
    TickType_t xButtonPressTickCounter; /**< tick counter for button press and hold */ 
    TickType_t xButtonNoActionTickCounter; /**< tick counter for return monitor panel */
    TickType_t xO2FlowNoChangeTickCounter;
    bool isButtonPressed; /**< button press state */ 
    int leftTouchCounter, rightTouchCounter;
}  MainScreen_Control_Struct;

/** @brief Define the data variable in mainscreen */ 
typedef struct
{
    int preTemp, currentTemp;
    int preFlow, currentFlow;
    int preO2, currentO2;
    int preO2Flow, currentO2Flow;
    int preSpO2, currentSpO2;
    int prePR, currentPR;
} MainScreen_Data_Struct;

/* Belong to ChangedSetpointArea Animation, need to be create */        
extern GUI_ANIM_HANDLE g_ChangedSetpointArea_Anim;

/* Belong to AlarmBox Animation, need to be create */   
extern GUI_ANIM_HANDLE g_AlarmBox_Anim_MoveIn;

/* Belong to AlarmBox Animation, need to be create */   
extern GUI_ANIM_HANDLE g_AlarmBox_Anim_MoveOut;

/* Initialize mainscreen's element: Check all maincreen's widgets have been created yet,
 * init control & data variable and change some properties in needed initialization widgets */
E_GuiResult MainScreen_Initialize(void);

/* Initialize mainscreen's element: Check all maincreen's widgets have been created yet,
 * init control & data variable and change some properties in needed initialization widgets */
E_GuiResult MainScreen_Reinit(void);

/* Implement mainscreen's state, animation function and some action periodlly
 *  It was called by DisplayControl */
void MainScreen_Run(void);

/* To free memory while mainscreen not showing*/
void MainScreen_Distructor(void);

/* Set mainscreen's state */
inline void MainScreen_SetState(E_DisplayState state);

/* Get mainscreen's state */
inline E_DisplayState MainScreen_GetState(void);

/* Set mainscreen's state */
inline void MainScreen_SetSubstate(E_DisplaySubstates substate);

/* Get mainscreen's state */
inline E_DisplaySubstates MainScreen_GetSubstate(void);

/* Get mainscreen's quatity */
inline void MainScreen_SetSettingQuatity(E_SettingId quatity);

/* Get mainscreen's quatity */
inline E_SettingId MainScreen_GetSettingQuatity(void);

/* Set type of animation (MoveIn or MoveOut) in mainscreen */
void MainScreen_SetAnimType(E_AnimType type, int id);

/**/

/* Update ChangedSetpointArea if user interface with screen */
static E_GuiResult MainScreen_ChangedSetpointArea_UpdateSettingWidget(void);

//E_GuiResult MainScreen_Handle_ChangedSetpointArea(void);


/* Handle the behavior when btnPlus was pressed */
void MainScreen_HandleBtnPlus(void);

/* Handle the behavior when btnMinus was pressed */
void MainScreen_HandleBtnMinus(void);

/* Handle the behavior when btnOK was pressed */
void MainScreen_HandleBtnOK(void);

/* Handle the behavior when btnPlus was pressed */
void MainScreen_HandleBtnPlus(void);

/* Handle the behavior when btnPlus was pressed */
void MainScreen_SetMonitorPopupVisible(bool flag);

/* Handle the behavior when btnPlus was pressed */
void MainScreen_SetSettingPopupVisible(bool flag);

/* Load setting from file  */
void MainScreen_SettingAreaLoadData(void);

/* display running time on notification bar  */
E_WarmUpState MainScreen_GetWarmUpState();

/* display running time on notification bar  */
void MainScreen_SetWarmUpState(E_WarmUpState s);

/* display running time on notification bar  */
bool MainScreen_IsWarmingUp(void);

/* display running time on notification bar  */
bool MainScreen_IsSettingPopupShow(void);

void MainScreen_SetSettingPopupShow(bool f);

void MainScreen_ResetButtonPressTickCounter(void);
bool MainScreen_CheckButtonPressTimeout(void);
void MainScreen_SetButtonPressedState(bool state);
void MainScreen_ResetButtonNoActionTickCounter(void);
bool MainScreen_CheckButtonNoActionTimeout(void);
void MainScreen_o2FlowNoChangeTickCounter(void);
void MainScreen_checkO2FlowChange(void);
TickType_t MainScreen_CalculateDeltaTick(TickType_t current, TickType_t last);

#endif

/* end of file */