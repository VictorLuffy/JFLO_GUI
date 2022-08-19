/* ************************************************************************** */
/** @file [Button.c]
 *  @brief {This file contain source code necessary for the button handle }
 * It is including 
 *  @author {truongnguyen}
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */

//#include <float.h>
//#include <stdint.h>
//#include <stdio.h>

//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"

#include "system_config.h"
#include "Button.h"
#include "DeviceInterface.h"
#include "GuiInterface.h"

#define BUTTON_PANEL_DELAY_TIME 	10

#define BUTTON_CNT_2S               2000/BUTTON_PANEL_DELAY_TIME

#define BUTTON_CNT_WAIT_RELEASE		-1

/** @brief O2 flow (LPM) obtained from O2 Flow Sensor and bypass a LPF */
static E_ButtonStateEvent gs_buttonStateList[eNumOfButton];


/** @brief local functions  */



/** @brief Function to initialize original state of button 
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void button_Init()
{
    int i;
    for (i = eFirstButton; i <= eLastButton; i++)
    {
         gs_buttonStateList[i] = eButtonIdle;
    }
}


/** @brief Function to determine which operation has just acted on Power button 
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
 void button_CheckStateOfPowerButton()
{

    static bool s_prevState = true;
    static bool s_currState = true;
    static int32_t s_buttonCnt = 0;
    s_currState = POWER_SWStateGet();

    if (s_prevState == false)
    {
        if ((s_currState == false)&&(s_buttonCnt != BUTTON_CNT_WAIT_RELEASE))
        {
            s_buttonCnt++;
        }
        if(s_buttonCnt >= BUTTON_CNT_2S)
        {
            s_buttonCnt = BUTTON_CNT_WAIT_RELEASE;
            gs_buttonStateList[ePowerButton] = ePressHold;
        }
        else
        {
            bool state = POWER_SWStateGet();
            if( (state == true)&&(s_buttonCnt != BUTTON_CNT_WAIT_RELEASE) )
            {
                    gs_buttonStateList[ePowerButton] = ePress;
            }
        }
    }
    else
    {
        s_buttonCnt = 0;
    }
    s_prevState = s_currState;
}
 


/** @brief Function to determine which operation has just acted on Mute Button 
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
 void button_CheckStateOfMuteButton()
{

    static bool s_prevState = true;
    static bool s_currState = true;
    static int32_t s_buttonCnt = 0;

    s_currState = MUTE_SWStateGet();

    if (s_prevState == false)
    {
        if ((s_currState == false)&&(s_buttonCnt != BUTTON_CNT_WAIT_RELEASE))
        {
            s_buttonCnt++;
        }
        if(s_buttonCnt >= BUTTON_CNT_2S)
        {
            s_buttonCnt = BUTTON_CNT_WAIT_RELEASE;
            gs_buttonStateList[eMuteButton] = ePressHold;
        }
        else
        {
            bool state = MUTE_SWStateGet();
            if( (state == true)&&(s_buttonCnt != BUTTON_CNT_WAIT_RELEASE) )
            {
                    gs_buttonStateList[eMuteButton] = ePress;
            }
        }
    }
    else
    {
        s_buttonCnt = 0;
    }
    s_prevState = s_currState;
}

/** @brief Function to check state of all buttons
 * This function should be called in a loop task to checking consecutive
 * state of the button
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void button_UpdateStateOfAllButton()
{
    button_CheckStateOfPowerButton();
    button_CheckStateOfMuteButton();
}


/** @brief Function to get state of specific button
 *  @param [in]  E_ButtonID ID: id of button    
 *  @param [out]  None
 *  @return E_ButtonEvent current state of the button
 */
E_ButtonStateEvent button_GetButtonState(E_ButtonID ID)
{
    E_ButtonStateEvent currentState = gs_buttonStateList[ID];
    gs_buttonStateList[ID] = eButtonIdle;//clear current state
    return currentState;
}


void button_Handle()
{
    button_UpdateStateOfAllButton();
    E_ButtonStateEvent muteButton = button_GetButtonState(eMuteButton);
    if(muteButton == ePress)
    {
        //Mute Alarm in 120 seconds
        deviceInterface_SendAudioEvent(eMute, 120);
        guiInterface_SendEvent(eGuiMainScreenAlarmMuteIconShow, 0);
        SYS_PRINT("Button Mute Alarm Pressed\n");
        
    }
    
}

/* *****************************************************************************
 End of File
 */
