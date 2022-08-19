#include "AnimationControl.h"
#include "stdint.h"
#include "GuiDefine.h"

/** @brief Total of Animations  */
static uint8_t s_animIndex = 0;

/** @brief Animation Objects */
GUI_ANIM_INFO_t animInfo[MAX_GUI_ANIM];


/** @brief Create animation object
 **  @param [in]  GUI_TIMER_TIME period : total animating time 
 **              unsigned minTimePerFrame : ping ms/frame
 **             GUI_ANIM_GETPOS_FUNC pfGetPos : animation function to get the result following to time
 **              void * pVoid : pointer to any parameters 
 **              GUI_ANIMATION_FUNC pfAnim : executing function
 **  @param [out]  None
 **  @return Animation index
 */
GUI_ANIM_HANDLE AnimationControl_CreateAnim (GUI_TIMER_TIME period,unsigned minTimePerFrame, GUI_ANIM_GETPOS_FUNC pfGetPos, void * pVoid, GUI_ANIMATION_FUNC pfAnim)
{
//    SYS_PRINT("\n AnimationControl_CreateAnim");
    animInfo[s_animIndex].tNow = 0;
    animInfo[s_animIndex].te = period;
    animInfo[s_animIndex].Period = period;
    animInfo[s_animIndex].MinTimePerFrame = minTimePerFrame;
    animInfo[s_animIndex].pfGetPos = pfGetPos;
    animInfo[s_animIndex].pVoid = pVoid;
    animInfo[s_animIndex].pfAnim = pfAnim;
    
    return s_animIndex++;
}

/** @brief Change new function to get position
 **  @param [in]    GUI_ANIM_HANDLE_t hAnim : Animation Object
 **                 GUI_ANIM_GETPOS_FUNC pfGetPos : New fucntion to get position
 **  @param [out]   None
 **  @return None
 */
void AnimationControl_SetPosFunc(GUI_ANIM_HANDLE_t hAnim, GUI_ANIM_GETPOS_FUNC pfGetPos)
{
    animInfo[hAnim].pfGetPos = pfGetPos;
}

/** @brief Start animation object
 **  @param [in]    GUI_ANIM_HANDLE_t hAnim : Animation Object
 **  @param [out]   None
 **  @return None
 */
void AnimationControl_StartAnim(GUI_ANIM_HANDLE_t hAnim)
{
//    SYS_PRINT("\n AnimationControl_StartAnim");
    animInfo[hAnim].State = 0x01/*&= (~ANIM_STATE_MASK) | GUI_ANIM_START*/;
}

/** @brief Stop animation object
 **  @param [in]    GUI_ANIM_HANDLE_t hAnim : Animation Object
 **  @param [out]   None
 **  @return None
 */
void AnimationControl_StopAnim(GUI_ANIM_HANDLE_t hAnim)
{
    animInfo[hAnim].State = 0x00;
    animInfo[hAnim].tNow = 0;
    animInfo[hAnim].triggerTick = 0;
    animInfo[hAnim].lastTick = 0;
    animInfo[hAnim].Pos = 0;
}

/** @brief Executing animation object
 **  @param [in]    GUI_ANIM_HANDLE_t hAnim : Animation Object
 **  @param [out]   None
 **  @return None
 */
E_AnimExecState AnimationControl_Exec(GUI_ANIM_HANDLE hAnim)
{
    static bool isOver32bit = false;
    
    uint32_t currentTick;
    animInfo[hAnim].execState = eIdleAnimExecState;

    /*From GUI_ANIM_START to GUI_ANIM_RUNNING*/
    if (animInfo[hAnim].State == (START_MASK | (GUI_ANIM_START << 1)))
    {
//        SYS_PRINT("\n in AnimationControl_Exec - GUI_ANIM_START");
        animInfo[hAnim].execState = eStartAnimExecState;
        animInfo[hAnim].startTick = (uint32_t)GET_TICKS();
        animInfo[hAnim].lastTick = (uint32_t)GET_TICKS();
        animInfo[hAnim].Tick = 0;
        isOver32bit = false;
        
        animInfo[hAnim].startTime = 0;       // ms
        animInfo[hAnim].endTime = animInfo[hAnim].Period;       //ms

        animInfo[hAnim].pfAnim(GUI_ANIM_START, 0);
        animInfo[hAnim].State = START_MASK | (GUI_ANIM_RUNNING << 1);        
    }
    else if( animInfo[hAnim].State == (START_MASK | (GUI_ANIM_RUNNING << 1)))
    {
        animInfo[hAnim].execState = eRunning_NotCbFncAnimExecState;
        
        currentTick = (uint32_t)GET_TICKS(); 
        
        // To avoid overflow in GET_TICKS()
        if (currentTick < animInfo[hAnim].lastTick && !isOver32bit) {
            SYS_PRINT("\n 1");
            uint32_t tmp = currentTick - animInfo[hAnim].lastTick;
            animInfo[hAnim].Tick += tmp;
        } else {
            uint32_t tmp = 0xFFFFFFFF - animInfo[hAnim].lastTick + currentTick;
            animInfo[hAnim].Tick += tmp;
            isOver32bit = true;
        }
        
        // define time to jump in executing function
        if (animInfo[hAnim].Tick > animInfo[hAnim].triggerTick)
        {
            animInfo[hAnim].execState = eRunning_CbFncAnimExecState;
            animInfo[hAnim].triggerTick = animInfo[hAnim].Tick +  animInfo[hAnim].MinTimePerFrame * TICKS_PER_MS /*ms*/;   // Control the callback
            animInfo[hAnim].tNow = (uint32_t)(animInfo[hAnim].Tick / TICKS_PER_MS);          // ms

            if (animInfo[hAnim].tNow < animInfo[hAnim].endTime)
            {         
                animInfo[hAnim].Pos = animInfo[hAnim].pfGetPos( animInfo[hAnim].startTime , animInfo[hAnim].endTime /*Max tick*/
                        , (GUI_TIMER_TIME*)&animInfo[hAnim].tNow);
            }
            else {
                animInfo[hAnim].Pos = GUI_ANIM_RANGE;
                animInfo[hAnim].State = START_MASK | (GUI_ANIM_END << 1);        // Switch to GUI_ANIM_END                  
            }
            
            animInfo[hAnim].pfAnim(GUI_ANIM_RUNNING, animInfo[hAnim].Pos); 
        } else {
            
        }
        
        animInfo[hAnim].lastTick = currentTick;
    }
    else if( animInfo[hAnim].State == (START_MASK | (GUI_ANIM_END  << 1)))
    {
        animInfo[hAnim].execState = eEndAnimExecState;
        animInfo[hAnim].pfAnim(GUI_ANIM_END, 255U);
        animInfo[hAnim].State &= 0x00;
    }
    
    return animInfo[hAnim].execState;
}

/** @brief AccelFunction To calculating position following time
 **  @param [in]    GUI_TIMER_TIME ts : The time when animation starts
 * GUI_TIMER_TIME te : The time when animation ends
 * GUI_TIMER_TIME* tNow : Current time
 **  @param [out]   None
 **  @return None
 */
int32_t _Calculate_Position_AccelFunction(GUI_TIMER_TIME ts, GUI_TIMER_TIME te, GUI_TIMER_TIME* tNow) {
    uint32_t Result = 0;
    uint32_t Diff;

    double tmp_1;
    uint32_t tmp_2, tmp_3 = *tNow - ts; 
  
    Diff = te - ts;

    if(*tNow > te) 
        return GUI_ANIM_RANGE;

    if (Diff) {
        tmp_1 = (double)(*tNow - ts) * (*tNow - ts) / (Diff * Diff);
//        Result = (uint32_t)((*tNow - ts) * (*tNow - ts) * GUI_ANIM_RANGE) / (Diff * Diff);  // y = a*x^2
        Result =  tmp_1 * GUI_ANIM_RANGE;
    } else {
      Result = GUI_ANIM_RANGE; // except 0 when te=ts
    }

    if(Result > GUI_ANIM_RANGE)
        Result = GUI_ANIM_RANGE;  


    return Result;
}

/** @brief DecelFunction To calculating position following time
 **  @param [in]    GUI_TIMER_TIME ts : The time when animation starts
 * GUI_TIMER_TIME te : The time when animation ends
 * GUI_TIMER_TIME* tNow : Current time
 **  @param [out]   None
 **  @return None
 */
int32_t _Calculate_Position_DecelFunction(GUI_TIMER_TIME ts, GUI_TIMER_TIME te, GUI_TIMER_TIME* tNow)
{
//    SYS_PRINT("\n Decel Function");
    uint32_t Result = 0;
    uint32_t Diff;

    Diff = te - ts;
    
    if(*tNow > te) 
        return GUI_ANIM_RANGE;
    double tmp_1;
    uint32_t tmp_2, tmp_3 = *tNow - ts;
    
    if (Diff) {
//      Result = (Diff*Diff - (Diff - *tNow) * (Diff - *tNow)) * GUI_ANIM_RANGE / (Diff * Diff);  // y = a*x^2
        tmp_1 = (double)(Diff - tmp_3) * (Diff - tmp_3) / (Diff * Diff);
        tmp_2 =  tmp_1 * GUI_ANIM_RANGE;
        Result = GUI_ANIM_RANGE - (uint32_t)tmp_2;
    } else {
      Result = GUI_ANIM_RANGE; // except 0 when te=ts
    }

    if(Result > GUI_ANIM_RANGE)
        Result = GUI_ANIM_RANGE;
    
    return Result;
}

/** @brief LinearFunction To calculating position following time
 **  @param [in]    GUI_TIMER_TIME ts : The time when animation starts
 * GUI_TIMER_TIME te : The time when animation ends
 * GUI_TIMER_TIME* tNow : Current time
 **  @param [out]   None
 **  @return None
 */
int32_t _Calculate_Position_LinearFunction(GUI_TIMER_TIME ts, GUI_TIMER_TIME te, GUI_TIMER_TIME* tNow)
{
    int32_t Result = 0;
    int Diff;
  
    Diff = te - ts;

    if(*tNow > te) 
        return GUI_ANIM_RANGE;
    double tmp_1;
    uint32_t tmp_2, tmp_3 = *tNow - ts;
    
    if (Diff) {
        tmp_1 = (double)(*tNow - ts)/ Diff;
        Result = tmp_1 * GUI_ANIM_RANGE ;  // y = a*x
    } else {
        Result = GUI_ANIM_RANGE; // except 0 when te=ts
    }
  
    if(Result > GUI_ANIM_RANGE)
        Result = GUI_ANIM_RANGE;  
    
    return Result;
}