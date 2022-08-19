/** @file {MainScreen.c}
 *  @brief {The screen provides graphic interface to update data. Data include 
 * {Temp, flow, O2 concentration, SpO2, PR, Hearbeat}. Also, Setting Value{Temperature,
 * Flow} will be updated by user in ChangedSettingArea, then this setting will be logged
 * in SettingFile }
 *  @author {Thanh Hiep Nguyen}
 */

/* This section lists the other files that are included in this file.
 */
#include <stddef.h>
#include "math.h"

#include "system_config.h"
#include "system_definitions.h"
#include "system/debug/sys_debug.h"

#include "gfx/libaria/libaria_init.h"

#include "Device/RTC_BQ32002.h"
#include "Alarm/AlarmInterface.h"
#include "MotorTask.h"
#include "HeaterTask.h"


#include "Gui/GuiDefine.h"
#include "Gui/LogInterface.h"
#include "Gui/Setting.h"
#include "Gui/MainScreen.h"
#include "Gui/DisplayControl.h"
#include "Gui/GuiInterface.h"
#include "Gui/AnimationControl.h"
#include "Gui/AlarmExpression.h"
#include "Delay.h"

#define SETTING_AREA_IN_POS_X 42
#define SETTING_AREA_OUT_POS_X 300

#define MAX_DISPLAY_TEMP 99
#define MIN_DISPLAY_TEMP 0

#define MAX_DISPLAY_FLOW 99
#define MIN_DISPLAY_FLOW 0

#define MAX_DISPLAY_O2 100
#define MIN_DISPLAY_O2 0

#define MAX_DISPLAY_SPO2 99
#define MIN_DISPLAY_SPO2 0

#define MAX_DISPLAY_PR 254
#define MIN_DISPLAY_PR 30

#define PRESS_TO_SETTING_POPUP_TIMEOUT 2000
#define NOPRESS_TO_SETTING_POPUP_TIMEOUT 30000

#define TOUCH_TO_TRASITION_TIMEOUT 2000

#define WARMING_UP_CYCLE 1000

/** @brief A varible to store the mainscreen control elements */
MainScreen_Control_Struct gs_mainscreenCtrl;

/** @brief A varible to store the mainscreen monitoring data */
static MainScreen_Data_Struct gs_mainscreenData;

/** @brief Belong to ChangedSetpointArea Animation, need to be create */        
GUI_ANIM_HANDLE g_ChangedSetpointArea_Anim;

/** @brief This function will handle for any touch into LeftPanel --> Switch to SettingScreen, MaintainanceSreen, Alarm test screen */
static void MainScreen_PanelLeftTouchCallback(laWidget* widget, laInput_TouchDownEvent* evt);

/** @brief This function will handle for any touch into RightPanel --> Switch to SettingScreen, MaintainanceSreen, Alarm test screen */
static void MainScreen_PanelRightTouchCallback(laWidget* widget, laInput_TouchDownEvent* evt);

/** @brief Update ChangedSetpointArea if user interface with screen. */
static E_GuiResult MainScreen_ChangedSetpointArea_UpdateSettingWidget(void)
{
    char strbuff[5];
    laString str;    

    if (gs_mainscreenCtrl.currentQualitySetting == eTempSettingId) {
        laWidget_SetY((laWidget*)imgBlueline, btnTemp_SelectToSetting_2->widget.rect.y - 3); 

        laImageWidget_SetImage(imgIcon, &iconTemp);
        
        sprintf(strbuff, "%.2d", (int)gs_mainscreenCtrl.TSettingValue);
        str = laString_CreateFromCharBuffer(strbuff, 
            GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S70_Nor, setting_Get(eLanguageSettingId)));
        laTextFieldWidget_SetText(tfValue, str);
        laString_Destroy(&str);
        
        
        laWidget_SetVisible((laWidget*) lbO2Up_ChangedSetpointArea, LA_FALSE);
        laWidget_SetVisible((laWidget*) lbO2Down_ChangedSetpointArea, LA_FALSE);
        laWidget_SetVisible((laWidget*) iconBidirectionArrow, LA_FALSE);
        laWidget_SetX((laWidget*)btnBack, 79);
        laWidget_SetVisible((laWidget*) btnBack, LA_TRUE);
        laWidget_SetVisible((laWidget*) btnPlus, LA_TRUE);
        laWidget_SetVisible((laWidget*) btnMinus, LA_TRUE);
        laWidget_SetVisible((laWidget*) btnOK, LA_TRUE);
        laWidget_SetVisible((laWidget*) lbO2_ChangedSetpointArea, LA_FALSE);
        
        /* Change the background of color */
        laWidget_SetScheme((laWidget*)btnTemp_SelectToSetting_2, &JFLO_Button_OnChangedSetpointArea_Scheme);      
        laWidget_SetScheme((laWidget*)btnFlow_SelectToSetting_2, &JFLO_Button_Scheme);      
        laWidget_SetScheme((laWidget*)btnO2_SelectToSetting_2, &JFLO_Button_Scheme); 
    } else if (gs_mainscreenCtrl.currentQualitySetting == eFlowSettingId) {
        laWidget_SetY((laWidget*)imgBlueline, btnFlow_SelectToSetting_2->widget.rect.y - 3);
        laImageWidget_SetImage(imgIcon, &iconFlow);

        sprintf(strbuff, "%.2d", (int)gs_mainscreenCtrl.FSettingValue);
        str = laString_CreateFromCharBuffer(strbuff, 
            GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S70_Nor, setting_Get(eLanguageSettingId)));        
        laTextFieldWidget_SetText(tfValue, str);
        laString_Destroy(&str);

        laWidget_SetVisible((laWidget*) lbO2Up_ChangedSetpointArea, LA_FALSE);
        laWidget_SetVisible((laWidget*) lbO2Down_ChangedSetpointArea, LA_FALSE);
        laWidget_SetVisible((laWidget*) iconBidirectionArrow, LA_FALSE);
        laWidget_SetX((laWidget*)btnBack, 79);
        laWidget_SetVisible((laWidget*) btnBack, LA_TRUE);
        laWidget_SetVisible((laWidget*) btnPlus, LA_TRUE);
        laWidget_SetVisible((laWidget*) btnMinus, LA_TRUE);
        laWidget_SetVisible((laWidget*) btnOK, LA_TRUE);
        laWidget_SetVisible((laWidget*) lbO2_ChangedSetpointArea, LA_FALSE);   
        
        /* Change the background of color */
        laWidget_SetScheme((laWidget*)btnTemp_SelectToSetting_2, &JFLO_Button_Scheme);      
        laWidget_SetScheme((laWidget*)btnFlow_SelectToSetting_2, &JFLO_Button_OnChangedSetpointArea_Scheme);      
        laWidget_SetScheme((laWidget*)btnO2_SelectToSetting_2, &JFLO_Button_Scheme);          
    }
    else if (gs_mainscreenCtrl.currentQualitySetting == eFiO2SettingId) {
        laWidget_SetY((laWidget*)imgBlueline, btnO2_SelectToSetting_2->widget.rect.y - 3);
        
        laImageWidget_SetImage(imgIcon, &iconO2);
        
        // 
        sprintf(strbuff, "%.2d", (int)setting_Get(eFiO2SettingId));
        str = laString_CreateFromCharBuffer(strbuff, 
            GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S70_Nor, setting_Get(eLanguageSettingId)));          
        laTextFieldWidget_SetText(tfValue, str);
        laString_Destroy(&str);
        
        laWidget_SetVisible((laWidget*) lbO2Up_ChangedSetpointArea, LA_TRUE);
        laWidget_SetVisible((laWidget*) lbO2Down_ChangedSetpointArea, LA_TRUE);        
        laWidget_SetVisible((laWidget*) iconBidirectionArrow, LA_TRUE);        
        
        laWidget_SetX((laWidget*)btnBack, 116);    
        laWidget_SetVisible((laWidget*) btnBack, LA_TRUE);
        laWidget_SetVisible((laWidget*) btnPlus, LA_FALSE);
        laWidget_SetVisible((laWidget*) btnMinus, LA_FALSE);
        laWidget_SetVisible((laWidget*) btnOK, LA_FALSE);
        laWidget_SetVisible((laWidget*) lbO2_ChangedSetpointArea, LA_TRUE);        
        
        /* Change the background of color */
        laWidget_SetScheme((laWidget*)btnTemp_SelectToSetting_2, &JFLO_Button_Scheme);      
        laWidget_SetScheme((laWidget*)btnFlow_SelectToSetting_2, &JFLO_Button_Scheme);      
        laWidget_SetScheme((laWidget*)btnO2_SelectToSetting_2, &JFLO_Button_OnChangedSetpointArea_Scheme);  
    }

    if (laWidget_GetVisible((laWidget*) imgBlueline ) == LA_FALSE )
        laWidget_SetVisible((laWidget*) imgBlueline, LA_TRUE);    
    
    return eGTrue;
}

/** @brief Operating fnc of ChangedSetpointArea's Animation */
static void MainScreen_ChangedSetpointAreaMoveCallback(int state, int pos)
{
    int nextPos = 0;
    int distance = 0;
    
    switch(state){
        case GUI_ANIM_START:
            if (gs_mainscreenCtrl.animCalling[g_ChangedSetpointArea_Anim] == eMoveIn)
            {
                AnimationControl_SetPosFunc(g_ChangedSetpointArea_Anim, GUI_ANIM_DECEL);
                gs_mainscreenCtrl.startPos = SETTING_AREA_OUT_POS_X;
                gs_mainscreenCtrl.destPos = SETTING_AREA_IN_POS_X;
            }
            else if (gs_mainscreenCtrl.animCalling[g_ChangedSetpointArea_Anim] == eMoveOut)
            {
                gs_mainscreenCtrl.startPos = SETTING_AREA_IN_POS_X;
                 gs_mainscreenCtrl.destPos = SETTING_AREA_OUT_POS_X;
                 AnimationControl_SetPosFunc(g_ChangedSetpointArea_Anim, GUI_ANIM_ACCEL);
            }

            if (gs_mainscreenCtrl.animCalling[g_ChangedSetpointArea_Anim] == eMoveIn)
            {
                MainScreen_SetMonitorPopupVisible(false);
            } 
            else if (gs_mainscreenCtrl.animCalling[g_ChangedSetpointArea_Anim] == eMoveOut)  {}
            gs_mainscreenCtrl.prePos = gs_mainscreenCtrl.startPos;
            break;
        case GUI_ANIM_RUNNING:

            if (gs_mainscreenCtrl.animCalling[g_ChangedSetpointArea_Anim] == eMoveIn)
            {
                nextPos = gs_mainscreenCtrl.startPos + (double)(gs_mainscreenCtrl.destPos - gs_mainscreenCtrl.startPos)*( (double) pos / GUI_ANIM_RANGE);
          
                distance = nextPos - gs_mainscreenCtrl.prePos;        
            } else if(gs_mainscreenCtrl.animCalling[g_ChangedSetpointArea_Anim] == eMoveOut)
            {
                nextPos = gs_mainscreenCtrl.startPos + (double)(gs_mainscreenCtrl.destPos - gs_mainscreenCtrl.startPos)*( (double)pos / GUI_ANIM_RANGE);
                           
                distance = nextPos - gs_mainscreenCtrl.prePos;             
            }

            if (nextPos != gs_mainscreenCtrl.prePos)
            {
                /* Move rectangle then minus now - pre to get distance and move other widget */
                laWidget_SetPosition((laWidget*)rectChangedSetpointArea, nextPos, rectChangedSetpointArea->widget.rect.y);  
                laWidget_SetPosition((laWidget*)btnTemp_SelectToSetting_2, nextPos - 30, btnTemp_SelectToSetting_2->widget.rect.y);
                laWidget_SetPosition((laWidget*)btnFlow_SelectToSetting_2, nextPos - 30, btnFlow_SelectToSetting_2->widget.rect.y);
                laWidget_SetPosition((laWidget*)btnO2_SelectToSetting_2, nextPos - 30, btnO2_SelectToSetting_2->widget.rect.y);
            }
            
            /*Save*/
            gs_mainscreenCtrl.prePos = nextPos;
            break;
        case GUI_ANIM_END:
            if (gs_mainscreenCtrl.animCalling[g_ChangedSetpointArea_Anim] == eMoveOut) {
                MainScreen_SetMonitorPopupVisible(true);  
                MainScreen_SetSettingPopupShow(false);
            } 
            else if (gs_mainscreenCtrl.animCalling[g_ChangedSetpointArea_Anim] == eMoveIn)  {
                // FIXME: this w/a is used to fix the issue drawing at temp button error
                laWidget_Invalidate((laWidget*)rectChangedSetpointArea);  
//                MainScreen_SetSettingPopupShow(true);
            }
            
            gs_mainscreenCtrl.animCalling[g_ChangedSetpointArea_Anim] = eNoneAnimCalling;
            
            break;
        default:
            break;
    }
}

/** @brief Set mainscreen's state
 *  @param [in]  state: State in Animation
 *  @param [out]  None
 *  @return None
 */
inline void MainScreen_SetState(E_DisplayState state)
{
    gs_mainscreenCtrl.state = state;
    gs_mainscreenCtrl.subState = eIdleDispSubstate;
}

/** @brief Get mainscreen's state
 *  @param [in]  state: State in Animation
 *  @param [out]  None
 *  @return None
 */
inline E_DisplayState MainScreen_GetState(void)
{
    return gs_mainscreenCtrl.state;
}


/** @brief Set mainscreen's state
 *  @param [in]  state: State in Animation
 *  @param [out]  None
 *  @return None
 */
inline void MainScreen_SetSubstate(E_DisplaySubstates substate)
{
    gs_mainscreenCtrl.subState = substate;
}

/** @brief Set mainscreen's state
 *  @param [in]  None
 *  @param [out]  None
 *  @return E_DisplaySubstates: Get SubstateID to update UI
 */
inline E_DisplaySubstates MainScreen_GetSubstate(void)
{
    return gs_mainscreenCtrl.subState;
}

/** @brief Set Setting Quatity
 *  @param [in]  quatity: Save quatity that user want to set new value
 *  @param [out]  None
 *  @return None
 */
inline void MainScreen_SetSettingQuatity(E_SettingId quatity)
{
    gs_mainscreenCtrl.currentQualitySetting = quatity;
}

/** @brief Set mainscreen's state
 *  @param [in]  state: State in Animation
 *  @param [out]  None
 *  @return None
 */
inline E_SettingId MainScreen_GetSettingQuatity(void)
{
    return gs_mainscreenCtrl.currentQualitySetting;
}

/** @brief Set type of animation (MoveIn or MoveOut) in mainscreen
 *  @param [in]  state: State in Animation
 *  @param [out]  None
 *  @return None
 */
void MainScreen_SetAnimType(E_AnimType type, int id)
{
    gs_mainscreenCtrl.animCalling[id] = type;
}

/** @brief Handle the behavior when state of process is ChangedSetpointAreaShowingState*/
static E_GuiResult MainScreen_SettingAreaRun(void)
{
    switch (gs_mainscreenCtrl.subState){
        case eIdleDispSubstate:
            if(!MainScreen_IsSettingPopupShow() && MainScreen_CheckButtonPressTimeout())
            {
//                SYS_PRINT("Show setting popup \n");
                MainScreen_SetSubstate(eInitDispSubstate);
                MainScreen_ResetButtonNoActionTickCounter();
            }
            if(MainScreen_IsSettingPopupShow() && MainScreen_CheckButtonNoActionTimeout())
            {
//                SYS_PRINT("Hide setting popup \n");
                MainScreen_ResetButtonNoActionTickCounter();
                MainScreen_SetAnimType(eMoveOut,g_ChangedSetpointArea_Anim);
                AnimationControl_StopAnim(g_ChangedSetpointArea_Anim);
                AnimationControl_StartAnim(g_ChangedSetpointArea_Anim);     
                MainScreen_SetSubstate(eQuitSettingScreenDispSubstate);
            }
            break;
        case eInitDispSubstate:
            setting_Restore();
            MainScreen_SettingAreaLoadData();
            MainScreen_SetSettingPopupShow(true);
            laWidget_SetX((laWidget*)rectChangedSetpointArea, SETTING_AREA_OUT_POS_X);  
            laWidget_SetX((laWidget*)btnTemp_SelectToSetting_2, SETTING_AREA_OUT_POS_X - 30);
            laWidget_SetX((laWidget*)btnFlow_SelectToSetting_2, SETTING_AREA_OUT_POS_X - 30);
            laWidget_SetX((laWidget*)btnO2_SelectToSetting_2, SETTING_AREA_OUT_POS_X - 30);
            MainScreen_ChangedSetpointArea_UpdateSettingWidget();
            
            MainScreen_SetAnimType(eMoveIn, g_ChangedSetpointArea_Anim);    
            AnimationControl_StopAnim(g_ChangedSetpointArea_Anim);
            AnimationControl_StartAnim(g_ChangedSetpointArea_Anim);
            
            MainScreen_SetSettingPopupVisible(true);
            MainScreen_SetSubstate(eIdleDispSubstate);
            break;
        case eSettingChangedDispSubstate:
            MainScreen_ChangedSetpointArea_UpdateSettingWidget();
            MainScreen_SetSubstate(eIdleDispSubstate);
            break;
           
        case eQuitSettingScreenDispSubstate:
            laWidget_SetVisible((laWidget*)imgBlueline, LA_FALSE);
            
            // Finish state
            if (rectChangedSetpointArea->widget.rect.x == SETTING_AREA_OUT_POS_X)
            {
                MainScreen_SetSubstate(eIdleDispSubstate); 
            }
            break;
        default:
            break;
    }    
    return eGFalse;
}

void MainScreen_DisplayCurrentTemp()
{    
    char strbuff[5] = {0};
    laString str;
    if (gs_mainscreenData.preTemp != gs_mainscreenData.currentTemp)
    {
        if (gs_mainscreenData.currentTemp >= MIN_DISPLAY_TEMP && gs_mainscreenData.currentTemp <= MAX_DISPLAY_TEMP)
        {
            sprintf(strbuff, "%d", gs_mainscreenData.currentTemp);
        }
        else
        {
            sprintf(strbuff, "--");
        }
        str = laString_CreateFromCharBuffer(strbuff, 
            GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S83_Nor, setting_Get(eLanguageSettingId)));

        laTextFieldWidget_SetText(tfCurrentTempValue,  str );
        laString_Destroy(&str);           
        gs_mainscreenData.preTemp = gs_mainscreenData.currentTemp;
    }
}

void MainScreen_DisplayCurrentFlow()
{
    char strbuff[5] = {0};
    laString str;
    if (gs_mainscreenData.preFlow != gs_mainscreenData.currentFlow)
    {
        if (gs_mainscreenData.currentFlow >= MIN_DISPLAY_FLOW && gs_mainscreenData.currentFlow <= MAX_DISPLAY_FLOW)
        {
            sprintf(strbuff, "%d", gs_mainscreenData.currentFlow);
        }
        else
        {
            sprintf(strbuff, "--");
        }
        str = laString_CreateFromCharBuffer(strbuff, 
            GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S83_Nor, setting_Get(eLanguageSettingId)));    

        laTextFieldWidget_SetText(tfCurrentFlowValue, str  );
        laString_Destroy(&str);
        
        gs_mainscreenData.preFlow = gs_mainscreenData.currentFlow;
    }
}

void MainScreen_DisplayCurrentO2()
{
    char strbuff[5] = {0};
    laString str;
    if (gs_mainscreenData.preO2 != gs_mainscreenData.currentO2)
    {
        if (gs_mainscreenData.currentO2 >= MIN_DISPLAY_O2 && gs_mainscreenData.currentO2 <= MAX_DISPLAY_O2)
        {
            sprintf(strbuff, "%d", gs_mainscreenData.currentO2);
        }
        else
        {
            sprintf(strbuff, "--");
        }

        str = laString_CreateFromCharBuffer(strbuff, 
            GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S83_Nor, setting_Get(eLanguageSettingId)));

        laTextFieldWidget_SetText(tfCurrentO2Value,  str );

        laString_Destroy(&str);
        
        gs_mainscreenData.preO2 = gs_mainscreenData.currentO2;
    }
}

void MainScreen_DisplayCurrentSpO2()
{
    if ( gs_mainscreenData.preSpO2 != gs_mainscreenData.currentSpO2) {
        if (gs_mainscreenData.currentSpO2 >= MIN_DISPLAY_SPO2 && gs_mainscreenData.currentSpO2 <= MAX_DISPLAY_SPO2)
        {
            char strbuff[5] = {0};
            laString str;

            sprintf(strbuff, "%.2d", gs_mainscreenData.currentSpO2);
            str = laString_CreateFromCharBuffer(strbuff, 
            GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S60_Nor, setting_Get(eLanguageSettingId)));
            laTextFieldWidget_SetText(tfSpO2Value,  str );
            laString_Destroy(&str);
            
            laWidget_SetVisible((laWidget*)tfSpO2Value, LA_TRUE);
            laWidget_SetVisible((laWidget*)labelSpO2Dash, LA_FALSE);
        }
        else
        {
            laWidget_SetVisible((laWidget*)tfSpO2Value, LA_FALSE);
            laWidget_SetVisible((laWidget*)labelSpO2Dash, LA_TRUE);
        }
        gs_mainscreenData.preSpO2 = gs_mainscreenData.currentSpO2;
    }
}

void MainScreen_DisplayCurrentPR()
{

    if ( gs_mainscreenData.prePR != gs_mainscreenData.currentPR) {
        if (gs_mainscreenData.currentPR >= MIN_DISPLAY_PR && gs_mainscreenData.currentPR <= MAX_DISPLAY_PR)
        {
            char strbuff[5] = {0};
            laString str;

            sprintf(strbuff, "%.2d", gs_mainscreenData.currentPR);
                    str = laString_CreateFromCharBuffer(strbuff, 
            GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S45_Nor, setting_Get(eLanguageSettingId)));
            laTextFieldWidget_SetText(tfPrValue,  str );
            laString_Destroy(&str);
            
            laWidget_SetVisible((laWidget*)tfPrValue, LA_TRUE);
            laWidget_SetVisible((laWidget*)labelPRDash, LA_FALSE);
        }
        else
        {
            laWidget_SetVisible((laWidget*)tfPrValue, LA_FALSE);
            laWidget_SetVisible((laWidget*)labelPRDash, LA_TRUE);            
        }
        gs_mainscreenData.prePR = gs_mainscreenData.currentPR;
    }
}

/** @brief Be a Periodical function which working for update monitoring figure */
void MainScreen_UpdateMonitor(bool isForceUpdate)
{
    static TickType_t lastTick = 0 ; 
    TickType_t currentTick = xTaskGetTickCount();
    TickType_t delta = DisplayControl_CalculateDeltaTick(currentTick, lastTick);

    if (isForceUpdate || delta >= MONITOR_UPDATE_CYCLE_MS )
    {
//        SYS_PRINT("currentTick %u lastTick %u \n", currentTick, lastTick);
        lastTick = currentTick;
    }
    else 
        return;
    
    // Action
    char strbuff[5] = {0};
    laString str;
    
    gs_mainscreenData.currentTemp = (int)DisplayControl_GetDataTemp();
    gs_mainscreenData.currentFlow = (int)DisplayControl_GetDataFlow();
    gs_mainscreenData.currentO2 = (int)DisplayControl_GetDataO2Concentration();
    gs_mainscreenData.currentSpO2 = (int)DisplayControl_GetDataSpO2();
    gs_mainscreenData.currentPR = (int)DisplayControl_GetDataPR();
    gs_mainscreenData.currentO2Flow = (int)DisplayControl_GetDataO2Flow();

    // Update SpO2
    MainScreen_DisplayCurrentSpO2();
    
    // Update PR
    MainScreen_DisplayCurrentPR();
    
    // FIXME : the text edit will override the video, so skip update these value when alarm play
    if (AlarmExpression_IsPopupShow())
        return;
    // Update Temp
    MainScreen_DisplayCurrentTemp();

    //Update Flow
    MainScreen_DisplayCurrentFlow();

    // Update O2
    MainScreen_DisplayCurrentO2();
    
    // Update O2 flow
    MainScreen_checkO2FlowChange();
    
    return;
}


/** @brief Handle the behavior when state of process is TargetSettingLoadingState */
void MainScreen_SetWarmUpState(E_WarmUpState s)
{
    gs_mainscreenCtrl.warmUpState = s;
}

/** @brief Handle the behavior when state of process is TargetSettingLoadingState */
E_WarmUpState MainScreen_GetWarmUpState()
{
    return gs_mainscreenCtrl.warmUpState;
}

/** @brief Handle the behavior when state of process is TargetSettingLoadingState */
bool MainScreen_IsWarmingUp()
{
    return gs_mainscreenCtrl.warmUpState == eIdleWarmUpState ? false : true;
}

/** @brief Handle the behavior when state of process is TargetSettingLoadingState */
static E_GuiResult MainScreen_WarmUpRun(void)
{
    static TickType_t warmingUpTimeMs = 0;
        
    switch (gs_mainscreenCtrl.warmUpState)
    {
        case eIdleWarmUpState:
            break;
        case eInitWarmUpState:
//            SYS_PRINT("MainScreen_WarmUpRun - eInitWarmUpState \n");
            warmingUpTimeMs = xTaskGetTickCount();
            MainScreen_SetWarmUpState(eRunningWarmUpState);
            break;
        case eRunningWarmUpState:
            {
                static uint32_t triggerTick = 0;
                uint32_t currentTick = 0;
                currentTick = (uint32_t)GET_TICKS();

                if (triggerTick < currentTick)
                {
                    triggerTick = currentTick + WARMING_UP_CYCLE*TICKS_PER_MS;

                    if (MainScreen_IsSettingPopupShow() || AlarmExpression_IsPopupShow())
                        break;                   
                    
                    laWidget_SetScheme((laWidget*)lbInfo_NoticeArea, &JFLO_WarmingUp_Scheme);
                    laWidget_SetScheme((laWidget*)panelNotice, &JFLO_WarmingUp_Scheme);
                    laLabelWidget_SetText(lbInfo_NoticeArea, laString_CreateFromID(string_text_WarmingUp_MainScreen));
    
                    // FIXME: debug only
#ifdef JFLO_DEBUG_GUI                    
                    static uint8_t counter = 0;
                    if (counter++ > 4)
                    {
                        MainScreen_SetWarmUpState(eFinishedWarmUpState);
                        counter = 0;
                    }
#endif                    

                    static bool isShow = true;
                    isShow = !isShow;
                    if (isShow)
                    {
                        laWidget_SetVisible((laWidget*)arcTemp_SelectToSetting_1, LA_TRUE);
                        laWidget_SetVisible((laWidget*)arcFlow_SelectToSetting_1, LA_TRUE);
                        laWidget_Invalidate((laWidget*)rectMonitoringArea);
                    }
                    else
                    {
                        laWidget_SetVisible((laWidget*)arcTemp_SelectToSetting_1, LA_FALSE);
                        laWidget_SetVisible((laWidget*)arcFlow_SelectToSetting_1, LA_FALSE);
                        laWidget_Invalidate((laWidget*)rectMonitoringArea);
                    }
                }
            }
            break;           
        case eFinishedWarmUpState:    
        {
            static int tmp_cnt_1;
            laString lastr;      

            tmp_cnt_1++;
            if (tmp_cnt_1 == 1)
            {
                warmingUpTimeMs = xTaskGetTickCount() - warmingUpTimeMs;
//                lastr = laString_CreateFromID(string_text_WarmingUpFinished_MainScreen);  
                char strbuff[255];
                sprintf(strbuff, "Warming Up Finished (%ds)", warmingUpTimeMs/1000);
                lastr = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S20_Bold_SQI);
                laLabelWidget_SetText(lbInfo_NoticeArea, lastr);
                laString_Destroy(&lastr);      
                
                laWidget_SetVisible((laWidget*)arcTemp_SelectToSetting_1, LA_TRUE);
                laWidget_SetVisible((laWidget*)arcFlow_SelectToSetting_1, LA_TRUE);
                laWidget_Invalidate((laWidget*)rectMonitoringArea);
            }

            if (tmp_cnt_1 == 200 ) {
                MainScreen_SetWarmUpState(eQuitWarmUpState);       
                tmp_cnt_1 = 0;
            }     
        }
            break;
        case eQuitWarmUpState:
            MainScreen_SetWarmUpState(eIdleWarmUpState);
            DisplayControl_SetStartTime();
            return eGCompleted;
            break;
        default:
            break;
    }

    
    return eGFalse;
}

/** @brief Be a Periodical function which working for update O2 graph
 *  @param [in]  uint32_t currTick: GET_TICK() 
 *               bool isInit: true in mainscreen initialization, false in period function 
 *  @param [out]  None
 *  @return None
 */
void MainScreen_DrawO2Graph(bool isForceUpdate)
{
    static TickType_t lastTick = 0 ; 
    TickType_t currentTick = xTaskGetTickCount();
    TickType_t delta = DisplayControl_CalculateDeltaTick(currentTick, lastTick);
    
    if (isForceUpdate || delta >= GRAPH_O2_UPDATE_CYCLE_MS)
    {
        lastTick = currentTick;
    }
    else 
        return;
    
    //Action
    static uint16_t t_DataPoint_Nums = 0;
    static int32_t    Value = 0;

    if (isForceUpdate) {
        _laLineGraphWidget_Do(graphHeartBeat, LINE_GRAPH_COMMAND_START_DRAW);
        t_DataPoint_Nums = 0;
        return;
    }

DrawLineGraph_Do:    
#ifdef JFLO_DEBUG_GUI
    if (Value++ > 100 )
        Value = 0;
#else
    Value = SPO2Data_GetPlenthValue();
#endif
    if(t_DataPoint_Nums <= MAX_POINT_NUM_ON_HEARBEATGRAPH /*offset because if offset = 0 -->graph wont be erased clearly*/){
        laLineGraphWidget_SetDataInSeries((laLineGraphWidget*)graphHeartBeat, 0 , t_DataPoint_Nums, Value );
//            laLineGraphWidget_SetSeriesLinesVisible((laLineGraphWidget*)graphHeartBeat, 0, LA_TRUE);

        t_DataPoint_Nums++;
    } 
    else {        
        t_DataPoint_Nums = 0;
    }     

    
    return;
}

/** @brief Handle the behavior when btnPlus was pressed
 *  @param [in]   None
 *  @param [out]  None
 *  @return None
 */
void MainScreen_HandleBtnPlus(void)
{
//    SYS_PRINT("\n Handle Plus");
    uint8_t value;  
    
    if (gs_mainscreenCtrl.currentQualitySetting == eTempSettingId) {
        value = gs_mainscreenCtrl.TSettingValue;
        if (value < setting_GetMax(eTempSettingId)) 
            value = value + setting_GetStep(eTempSettingId);
        else
            return;
        gs_mainscreenCtrl.TSettingValue = value;
    } else if (gs_mainscreenCtrl.currentQualitySetting == eFlowSettingId) {
        value = gs_mainscreenCtrl.FSettingValue;
        if (value < setting_GetMax(eFlowSettingId)) 
            value = value + setting_GetStep(eFlowSettingId);
        else
            return;

        gs_mainscreenCtrl.FSettingValue = value;
    } 
    else {}
    
    MainScreen_SetSubstate(eSettingChangedDispSubstate);
    
    return;
}

/** @brief Handle the behavior when btnMinus was pressed
 *  @param [in]   None
 *  @param [out]  None
 *  @return None
 */
void MainScreen_HandleBtnMinus(void)
{
    SYS_PRINT("\n Handle Minus");
    uint8_t value; 
  
    if (gs_mainscreenCtrl.currentQualitySetting == eTempSettingId) {
        value = gs_mainscreenCtrl.TSettingValue;
        if ( value > setting_GetMin(eTempSettingId))
            value = value - setting_GetStep(eTempSettingId);
        else 
            return;
        gs_mainscreenCtrl.TSettingValue = value;
    } else if (gs_mainscreenCtrl.currentQualitySetting == eFlowSettingId) {
        value = gs_mainscreenCtrl.FSettingValue;
        if ( value > setting_GetMin(eFlowSettingId))
            value = value - setting_GetStep(eFlowSettingId);
        else 
            return;
        gs_mainscreenCtrl.FSettingValue = value;
    } 
    else {}
    
    MainScreen_SetSubstate(eSettingChangedDispSubstate);
}

/** @brief Handle the behavior when btnOK was pressed
 *  @param [in]   None
 *  @param [out]  None
 *  @return None
 */
void MainScreen_HandleBtnOK(void)
{
    SYS_PRINT("\n Handle BTN OK");
    E_TreatmentMode mode = setting_Get(eTreatmentModeSettingId);
    if (gs_mainscreenCtrl.currentQualitySetting == eTempSettingId
            && gs_mainscreenCtrl.TSettingValue != gs_mainscreenCtrl.TSettingPrevValue) 
    {
        if (mode == eAdultMode)
        {
            setting_Set(eAdultModeTempSettingId, gs_mainscreenCtrl.TSettingValue);
            setting_Set(eTempSettingId, gs_mainscreenCtrl.TSettingValue);
        }
        else if (mode == eChildMode)
        {
            setting_Set(eChildModeTempSettingId, gs_mainscreenCtrl.TSettingValue);
            setting_Set(eTempSettingId, gs_mainscreenCtrl.TSettingValue);
        }
        else{}
        setting_Save();

        //Send temperature setting
        //send event to Flow Control Task to start motor
        HEATER_CTRL_EVENT_t eventTemp = {.id = eHeaterTempId, .fData = (float) gs_mainscreenCtrl.TSettingValue};
        HeaterTask_SendEvent(eventTemp);

        // Write a log
        uint8_t logData[2];
        logData[0] = gs_mainscreenCtrl.TSettingPrevValue;
        logData[1] = gs_mainscreenCtrl.TSettingValue;
        logInterface_WriteEventLog(2,(void*)logData , eChangeTargetTemperatureSettingEventLogId);   

        gs_mainscreenCtrl.TSettingPrevValue = gs_mainscreenCtrl.TSettingValue;

    } else if (gs_mainscreenCtrl.currentQualitySetting == eFlowSettingId
            && gs_mainscreenCtrl.FSettingValue != gs_mainscreenCtrl.FSettingPrevValue) 
    {  
        if (mode == eAdultMode)
        {
            setting_Set(eAdultModeFlowSettingId, gs_mainscreenCtrl.FSettingValue);
            setting_Set(eFlowSettingId, gs_mainscreenCtrl.FSettingValue);
        }
        else if (mode == eChildMode)
        {
            setting_Set(eChildModeFlowSettingId, gs_mainscreenCtrl.FSettingValue);
            setting_Set(eFlowSettingId, gs_mainscreenCtrl.FSettingValue);
        }
        else{}
        setting_Save();
        //Send flow setting
        MOTOR_CTRL_EVENT_t eventFlow = {.id = eMotorFlowId, .fData = (float) (gs_mainscreenCtrl.FSettingValue)};
        
        //send event to Flow Control Task to start motor
        MotorTask_SendEvent(eventFlow);
        // Write a log
        uint8_t logData[2];
        logData[0] = gs_mainscreenCtrl.FSettingPrevValue;
        logData[1] = gs_mainscreenCtrl.FSettingValue;
        logInterface_WriteEventLog(2,(void*)logData , eChangeTargetFlowSettingEventLogId);     

        gs_mainscreenCtrl.FSettingPrevValue = gs_mainscreenCtrl.FSettingValue;
    }
}

/** @brief Handle the behavior when panelLeftTouch was pressed */
static void MainScreen_PanelLeftTouchCallback(laWidget* widget, laInput_TouchDownEvent* evt)
{
    static TickType_t xPressTickCounter = 0;    
    static bool firstTime = true;

    gs_mainscreenCtrl.rightTouchCounter = 0;
    // This init xPressTickCounter at first time run
    if (firstTime)
    {
        xPressTickCounter = xTaskGetTickCount();
        gs_mainscreenCtrl.leftTouchCounter = 0;
        firstTime = false;
    }
    
    TickType_t delta = DisplayControl_CalculateDeltaTick(xTaskGetTickCount(), xPressTickCounter);
    if (delta <= TOUCH_TO_TRASITION_TIMEOUT)
    {
        gs_mainscreenCtrl.leftTouchCounter++;
    }
    else
    {
        SYS_PRINT("leftTouchCounter overtime %d \n" , delta);   
        gs_mainscreenCtrl.leftTouchCounter = 1;
    }
    
    if (gs_mainscreenCtrl.leftTouchCounter >=3 )
    {
        //Valid case
        DisplayControl_SetState(eMaintenanceScreenIsShowingDispState);
        gs_mainscreenCtrl.leftTouchCounter = 0;
    }
    else if( gs_mainscreenCtrl.leftTouchCounter == 1)
    {
        // Start point
        xPressTickCounter = xTaskGetTickCount();
    }
    else //0, 2 
    {
           
    }
    SYS_PRINT("leftTouchCounter %d \n" , gs_mainscreenCtrl.leftTouchCounter);
    
    // this should end of function, follow sample code
    evt->event.accepted = LA_TRUE;
    return;
}

/** @brief Handle the behavior when panelRightTouch was pressed */
static void MainScreen_PanelRightTouchCallback(laWidget* widget, laInput_TouchDownEvent* evt)
{
    static TickType_t xPressTickCounter = 0;    
    static bool firstTime = true;

    gs_mainscreenCtrl.leftTouchCounter = 0;
    // This init xPressTickCounter at first time run
    if (firstTime)
    {
        xPressTickCounter = xTaskGetTickCount();
        firstTime = false;
        gs_mainscreenCtrl.rightTouchCounter = 0;
    }
    
    TickType_t delta = DisplayControl_CalculateDeltaTick(xTaskGetTickCount(), xPressTickCounter);
    if (delta <= TOUCH_TO_TRASITION_TIMEOUT)
    {
        gs_mainscreenCtrl.rightTouchCounter++;
    }
    else
    {
        SYS_PRINT("rightTouchCounter overtime %d \n" , delta);   
        gs_mainscreenCtrl.rightTouchCounter = 1;
    }
    
    if (gs_mainscreenCtrl.rightTouchCounter >= 3 )
    {
        //Valid case
        DisplayControl_GoToScreen_SettingScreen_DefaultMenu();
        gs_mainscreenCtrl.rightTouchCounter = 0;
    }
    else if( gs_mainscreenCtrl.rightTouchCounter == 1)
    {
        xPressTickCounter = xTaskGetTickCount();
    }
    else //0, 2 
    {
        
    }
    SYS_PRINT("rightTouchCounter %d \n" , gs_mainscreenCtrl.rightTouchCounter);    
    
    // this should end of function, follow sample code
    evt->event.accepted = LA_TRUE;
    return;
}

void MainScreen_InitPreData()
{
    gs_mainscreenData.preTemp = -1;
    gs_mainscreenData.preFlow = -1;
    gs_mainscreenData.preO2 = -1;  
    gs_mainscreenData.preSpO2 = -1;
    gs_mainscreenData.prePR = -1;
    gs_mainscreenData.preO2Flow = -1;
    
    gs_mainscreenCtrl.TSettingPrevValue = -1;
    gs_mainscreenCtrl.FSettingPrevValue = -1;
    gs_mainscreenCtrl.OSettingPrevValue = -1;
    gs_mainscreenCtrl.OSettingUpperLimitPreValue = -1;
    gs_mainscreenCtrl.OSettingLowerLimitPreValue = -1;
    
    MainScreen_SetSettingPopupShow(false);
    gs_mainscreenCtrl.isButtonPressed = false;
   
    RunningBar_DisplayRunningTime(true);
    StatusBar_DisplayAll(true);
    MainScreen_UpdateMonitor(true);
    MainScreen_DrawO2Graph(true);

    int count = MAX_POINT_NUM_ON_HEARBEATGRAPH;
    while (count-- > 0)
    {
        laLineGraphWidget_AddCategory(graphHeartBeat, NULL);
        laLineGraphWidget_AddDataToSeries(graphHeartBeat, 0, 0, NULL);
    }
    
    laWidget_OverrideTouchDownEvent((laWidget*)panelLeftTouch, &MainScreen_PanelLeftTouchCallback);
    laWidget_OverrideTouchDownEvent((laWidget*)panelRightTouch, &MainScreen_PanelRightTouchCallback);    
            
    AlarmExpression_SetIndicatorVisible(false);
    
}

/** @brief Initialize mainscreen's element: Check all maincreen's widgets have been created yet,
 * init control & data variable and change some properties in needed initialization widgets
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 * @retval E_GuiResult : Result of initialization
 */
E_GuiResult MainScreen_Initialize(void)
{
    // this use to correct the active screen id, must be placed at start of this function
    DisplayControl_SetActiveScreenIndex(MainScreen_ID);
    MainScreen_InitPreData();  
        
    g_ChangedSetpointArea_Anim = AnimationControl_CreateAnim(300, 5, GUI_ANIM_ACCEL, NULL, MainScreen_ChangedSetpointAreaMoveCallback);
    g_AlarmBox_Anim_MoveIn = AnimationControl_CreateAnim(300, 5, GUI_ANIM_DECEL, NULL, AlarmExpression_AlarmAreaMoveInCallback);
    g_AlarmBox_Anim_MoveOut = AnimationControl_CreateAnim(300, 5, GUI_ANIM_ACCEL, NULL, AlarmExpression_AlarmAreaMoveOutCallback);

    return eGTrue;
}



/** @brief Reinitialize mainscreen's control and data.
 *  @brief Cause the value of mainscreen's control, data is changed and this screen is created by ScreenCreate_MainScreen fnc.
 *  This fnc will remake all variable of each widget in mainscreen
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
E_GuiResult MainScreen_Reinit(void)
{
    // this use to correct the active screen id, must be placed at start of this function
    DisplayControl_SetActiveScreenIndex(MainScreen_ID);

    MainScreen_InitPreData();  
      
//    if (AlarmExpression_IsPended())
//    {
//        AlarmExpression_SetPended(false);
        if (AlarmExpression_IsActiveStatus())
        {
            SYS_PRINT("\n AlarmExpression resumed %d %d \n", gs_alarmData.id, gs_alarmData.currentPriority);
            AlarmExpression_SetAlarmState(eInitAlarmState);   
        }
//    }

    return eGCompleted;
}

/** @brief Distruct some mainscreen backend elements when switched to other one
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void MainScreen_Distructor(void)
{
    if (AlarmExpression_IsActiveState())
    {
        SYS_PRINT("\n AlarmExpression suspended %d %d \n", gs_alarmData.id, gs_alarmData.currentPriority);
//        AlarmExpression_SetPended(true);
        AlarmExpression_Deinit();
    }
    laLineGraphWidget_DestroyAll(graphHeartBeat);
    return;
}

/** @brief Implement mainscreen's state, animation function and some action periodlly
 *  It was called by DisplayControl.
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void MainScreen_Run(void)
{
    /// Always run
    AnimationControl_Exec(g_ChangedSetpointArea_Anim);
    AnimationControl_Exec(g_AlarmBox_Anim_MoveIn);
    AnimationControl_Exec(g_AlarmBox_Anim_MoveOut);
    
    /// Update periodical action on UI
    MainScreen_UpdateMonitor(false);
    MainScreen_DrawO2Graph(false);   
    
    if (!AlarmExpression_IsActiveState() && !MainScreen_IsWarmingUp())
    {
        RunningBar_DisplayRunningTime(false);
    }

    // Change when any update status bar
    StatusBar_DisplayAll(false);
    
    /// Update Alarm State on UI
    AlarmExpression_Run();
    
    MainScreen_WarmUpRun();
    
    MainScreen_SettingAreaRun();
}

void MainScreen_SetMonitorPopupVisible(bool flag)
{
    if (flag)
    {
        laWidget_SetVisible((laWidget*)rectMonitoringArea, LA_TRUE);
    }
    else
    {
        laWidget_SetVisible((laWidget*)rectMonitoringArea, LA_FALSE);    
    }
}

void MainScreen_SetSettingPopupVisible(bool flag)
{
    if (flag)
    {
        laWidget_SetVisible((laWidget*)rectChangedSetpointArea, LA_TRUE);  
        laWidget_SetVisible((laWidget*)btnTemp_SelectToSetting_2, LA_TRUE);
        laWidget_SetVisible((laWidget*)btnFlow_SelectToSetting_2, LA_TRUE);
        laWidget_SetVisible((laWidget*)btnO2_SelectToSetting_2, LA_TRUE);
        laWidget_SetVisible((laWidget*)imgBlueline, LA_TRUE);
    }
    else
    {
        laWidget_SetVisible((laWidget*)rectChangedSetpointArea, LA_FALSE);  
        laWidget_SetVisible((laWidget*)btnTemp_SelectToSetting_2, LA_FALSE);
        laWidget_SetVisible((laWidget*)btnFlow_SelectToSetting_2, LA_FALSE);
        laWidget_SetVisible((laWidget*)btnO2_SelectToSetting_2, LA_FALSE);
        laWidget_SetVisible((laWidget*)imgBlueline, LA_FALSE);
    }
}
void MainScreen_SettingAreaLoadData(void)
{
    char strbuff[15];
    laString str;    

    //
    gs_mainscreenCtrl.TSettingValue = setting_Get(eTempSettingId);
    gs_mainscreenCtrl.FSettingValue = setting_Get(eFlowSettingId);
    gs_mainscreenCtrl.OSettingValue = setting_Get(eFiO2SettingId);
    gs_mainscreenCtrl.OSettingUpperLimitValue = setting_Get(eOxygenAlarmSettingUpperLimitId);
    gs_mainscreenCtrl.OSettingLowerLimitValue = setting_Get(eOxygenAlarmSettingLowerLimitId); 
    gs_mainscreenCtrl.TSettingPrevValue = gs_mainscreenCtrl.TSettingValue;
    gs_mainscreenCtrl.FSettingPrevValue = gs_mainscreenCtrl.FSettingValue;
    gs_mainscreenCtrl.OSettingPrevValue = gs_mainscreenCtrl.OSettingValue;
    gs_mainscreenCtrl.OSettingUpperLimitPreValue = gs_mainscreenCtrl.OSettingUpperLimitValue;
    gs_mainscreenCtrl.OSettingLowerLimitPreValue = gs_mainscreenCtrl.OSettingLowerLimitValue;
    //

    sprintf(strbuff, "%.2d", gs_mainscreenCtrl.TSettingValue);
    str = laString_CreateFromCharBuffer(strbuff, 
        GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S32_Nor, setting_Get(eLanguageSettingId)));            
    laTextFieldWidget_SetText(tfCurrentTempSetting , str);
    laString_Destroy(&str);
    
    sprintf(strbuff, "%.2d", gs_mainscreenCtrl.FSettingValue);
    str = laString_CreateFromCharBuffer(strbuff, 
        GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S32_Nor, setting_Get(eLanguageSettingId)));              
    laTextFieldWidget_SetText(tfCurrentFlowSetting , str);
    laString_Destroy(&str);
    
    sprintf(strbuff, "%.2d", gs_mainscreenCtrl.OSettingValue);
    str = laString_CreateFromCharBuffer(strbuff, 
        GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S32_Nor, setting_Get(eLanguageSettingId)));   
    laTextFieldWidget_SetText(tfCurrentO2Setting , str);    
    laString_Destroy(&str);
        
    sprintf(strbuff, "%.2d", gs_mainscreenCtrl.OSettingUpperLimitValue);
    str = laString_CreateFromCharBuffer(strbuff, 
        GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S18_Nor, setting_Get(eLanguageSettingId)));    
    laLabelWidget_SetText(lbO2Up_ChangedSetpointArea, str);    
    laString_Destroy(&str);
    
    sprintf(strbuff, "%.2d", gs_mainscreenCtrl.OSettingLowerLimitValue);
    str = laString_CreateFromCharBuffer(strbuff, 
        GFXU_StringFontIndexLookup(&stringTable, string_Nums_BebasNeueBook_S18_Nor, setting_Get(eLanguageSettingId)));    
    laLabelWidget_SetText(lbO2Down_ChangedSetpointArea, str);    
    laString_Destroy(&str);       
}

bool MainScreen_IsSettingPopupShow(void)
{
    return gs_mainscreenCtrl.isSettingPopupShow;
}

void MainScreen_SetSettingPopupShow(bool f)
{
    gs_mainscreenCtrl.isSettingPopupShow = f;
}

void MainScreen_ResetButtonPressTickCounter(void)
{
    gs_mainscreenCtrl.xButtonPressTickCounter = xTaskGetTickCount();
//    SYS_PRINT("DisplayControl_ResetButtonPressTickCounter %d \n", gs_dispData.xButtonPressTickCounter);
}

bool MainScreen_CheckButtonPressTimeout(void)
{
    if (!gs_mainscreenCtrl.isButtonPressed) 
        return false;  

    TickType_t delta = DisplayControl_CalculateDeltaTick(xTaskGetTickCount(), gs_mainscreenCtrl.xButtonPressTickCounter );
    if ( delta >= PRESS_TO_SETTING_POPUP_TIMEOUT)
    {
//        SYS_PRINT("DisplayControl_CheckButtonPressTickCounter timeout %d \n", delta);        
        gs_mainscreenCtrl.xButtonPressTickCounter = 0;
        gs_mainscreenCtrl.isButtonPressed = false;
        return true;
    }   
    return false;
}

void MainScreen_ResetButtonNoActionTickCounter(void)
{
    gs_mainscreenCtrl.xButtonNoActionTickCounter = xTaskGetTickCount();
//    SYS_PRINT("DisplayControl_ResetButtonNoActionTickCounter %d \n", gs_dispData.xButtonNoActionTickCounter);
}

bool MainScreen_CheckButtonNoActionTimeout(void)
{
    TickType_t delta = DisplayControl_CalculateDeltaTick(xTaskGetTickCount(), gs_mainscreenCtrl.xButtonNoActionTickCounter );
    TickType_t delta_1 = DisplayControl_CalculateDeltaTick(xTaskGetTickCount(), gs_mainscreenCtrl.xO2FlowNoChangeTickCounter);
    if (gs_mainscreenCtrl.currentQualitySetting == eFiO2SettingId)
    {   
        if ( delta >= NOPRESS_TO_SETTING_POPUP_TIMEOUT && delta_1 >= NOPRESS_TO_SETTING_POPUP_TIMEOUT)
        {
    //        SYS_PRINT("DisplayControl_CheckButtonNoActionTimeout timeout %d \n", delta);        
            gs_mainscreenCtrl.xButtonNoActionTickCounter = 0;
            return true;
        }
    }
    else
    {
        if ( delta >= NOPRESS_TO_SETTING_POPUP_TIMEOUT)
        {
    //        SYS_PRINT("DisplayControl_CheckButtonNoActionTimeout timeout %d \n", delta);        
            gs_mainscreenCtrl.xButtonNoActionTickCounter = 0;
            return true;
        }
    }
    return false;
}

void MainScreen_SetButtonPressedState(bool state)
{
    gs_mainscreenCtrl.isButtonPressed = state;
}

void MainScreen_o2FlowNoChangeTickCounter(void)
{
    gs_mainscreenCtrl.xO2FlowNoChangeTickCounter = xTaskGetTickCount();
}

void MainScreen_checkO2FlowChange(void)
{
    char strbuff[5] = {0};
    laString str;
    if (gs_mainscreenData.preO2Flow != gs_mainscreenData.currentO2Flow)
    {
        MainScreen_o2FlowNoChangeTickCounter();
        gs_mainscreenData.preO2Flow = gs_mainscreenData.currentO2Flow;
    }
    
}
// end of file
