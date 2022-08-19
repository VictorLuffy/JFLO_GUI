#ifndef _ANIMATION_CONTROL
#define _ANIMATION_CONTROL

#ifndef UNIT_TEST
#include "system_config.h"
#include "system_definitions.h"
#endif
#include "DisplayControl.h"

#define     MAX_GUI_ANIM            6

#define     GUI_TIMER_TICK         int
#define     GUI_TIMER_TIME         int
#define     GUI_ANIM_HANDLE        int
typedef int (* GUI_ANIM_GETPOS_FUNC)(GUI_TIMER_TICK, GUI_TIMER_TICK, GUI_TIMER_TICK*);
typedef void(* GUI_ANIMATION_FUNC)(int, int);

#define GUI_ANIM_ACCEL                  _Calculate_Position_AccelFunction
#define GUI_ANIM_DECEL                  _Calculate_Position_DecelFunction
#define GUI_ANIM_LINEAR                 _Calculate_Position_LinearFunction

#define     GUI_ANIM_HANDLE_t       int

#define GUI_ANIM_START   0
#define GUI_ANIM_RUNNING 1
#define GUI_ANIM_END     2

#define GUI_ANIM_RANGE  255U

//State : 7bit AnimState + 1bit Start
#define START_MASK          0x01
#define ANIM_STATE_MASK     0x0E

typedef enum {
    eIdleAnimExecState,
    eStartAnimExecState,  
    eRunning_NotCbFncAnimExecState,        
    eRunning_CbFncAnimExecState,
    eEndAnimExecState,
} E_AnimExecState;

typedef struct {
  int Pos;
  uint32_t triggerTick;
  uint32_t startTime;
  uint32_t endTime;
  uint32_t startTick, lastTick, Tick;
  uint32_t ts,te,tNow;
  unsigned State;
  uint16_t MinTimePerFrame;
  GUI_ANIM_HANDLE hAnim;
  GUI_TIMER_TIME Period;
  GUI_ANIM_GETPOS_FUNC pfGetPos;
  GUI_ANIMATION_FUNC  pfAnim;
  void * pVoid;
  
  E_AnimExecState execState;
} GUI_ANIM_INFO_t;


//GUI_ANIM_HANDLE GUI_ANIM_Create (GUI_TIMER_TIME Period, unsigned MinTimePerFrame, void * pVoid, void (* pfSliceInfo)(int State, void * _pVoid));
//int             GUI_ANIM_AddItem(GUI_ANIM_HANDLE hAnim, GUI_TIMER_TIME ts, GUI_TIMER_TIME te, GUI_ANIM_GETPOS_FUNC pfGetPos, void * pVoid, GUI_ANIMATION_FUNC * pfAnim);
GUI_ANIM_HANDLE GUI_ANIM_Create (GUI_TIMER_TIME Period,unsigned MinTimePerFrame  , GUI_ANIM_GETPOS_FUNC pfGetPos,  void * pVoid, GUI_ANIMATION_FUNC pfAnim);

void AnimationControl_SetPosFunc(GUI_ANIM_HANDLE_t hAnim, GUI_ANIM_GETPOS_FUNC pfGetPos);

void GUI_ANIM_Stop(GUI_ANIM_HANDLE_t hAnim);
void GUI_ANIM_Start(GUI_ANIM_HANDLE_t hAnim);
E_AnimExecState GUI_ANIM_Exec(GUI_ANIM_HANDLE hAnim);

int32_t _Calculate_Position_AccelFunction(GUI_TIMER_TIME ts, GUI_TIMER_TIME te, GUI_TIMER_TIME* tNow) ;
int32_t _Calculate_Position_DecelFunction(GUI_TIMER_TIME ts, GUI_TIMER_TIME te, GUI_TIMER_TIME* tNow);
int32_t _Calculate_Position_LinearFunction(GUI_TIMER_TIME ts, GUI_TIMER_TIME te, GUI_TIMER_TIME* tNow);


#endif
