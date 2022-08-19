/** @file PowerOffScreen.c
 *  @brief The screen provides graphic interface to power off notification
 *  @author Thanh Duong
 */

/* this section lists the other files that are included in this file
 */
#include "PowerOffScreen.h"
#include "SystemInterface.h"
#include "Gui/DisplayControl.h"
#include "Gui/Setting.h"
#include <stdio.h>
#include <stdbool.h>
#include "DeviceInterface.h"
#include "Audio.h"

static uint16_t gs_poweroffTick = 0;

static void PowerOffScreen_CheckSetIndicator(void);

/** @brief
 *  @param [in]
 *  @param [out]
 *  @return None
 */
void PowerOffScreen_Initialize(void)
{
    SYS_PRINT("\nPower off screen init");
   PowerOffScreen_SetIndicatorVisible(false);
}

/** @brief
 *  @param [in]
 *  @param [out]
 *  @return None
 */
void PowerOffScreen_UpdatePoweroffCounter(void)
{
    char strbuff[255] = {0};
    laString strStatus, strNumber, strLabel;
    uint16_t calculatedTick;
    static uint8_t s_preValue = 5, s_currentValue = 5;
    
    if (gs_poweroffTick == 0)
    {
        gs_poweroffTick = (uint16_t)xTaskGetTickCount();
    }
    calculatedTick = (uint16_t)xTaskGetTickCount() - gs_poweroffTick;
    
    if (calculatedTick >= 1000 && calculatedTick < 2000)
    {
        s_currentValue = 4;
    }
    else if (calculatedTick >= 2000 && calculatedTick < 3000)
    {
        s_currentValue = 3;
    }
    else if (calculatedTick >= 3000 && calculatedTick < 4000)
    {
        s_currentValue = 2;
    }
    else if (calculatedTick >= 4000 && calculatedTick < 5000)
    {
        s_currentValue = 1;
    }
    else if (calculatedTick >= 5000)
    {
        s_currentValue = 0;
    }
    
    if (s_preValue == 5 && s_currentValue == 5)
    {
        laString_Initialize(&strStatus); 
        strLabel = laString_CreateFromID(string_text_PowerOffMessage);
        sprintf(strbuff, "\n%d seconds", s_currentValue);
        strNumber = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S20_Bold_Internal);

        laString_Append(&strStatus, &strLabel);
        laString_Append(&strStatus, &strNumber);
        laLabelWidget_SetText(labelPowerOffMessage, strStatus );

        laString_Destroy(&strLabel); 
        laString_Destroy(&strNumber); 
        laString_Destroy(&strStatus);
    }
    
    if (s_preValue != s_currentValue)
    {
        s_preValue = s_currentValue;
        laString_Initialize(&strStatus); 
        strLabel = laString_CreateFromID(string_text_PowerOffMessage);
        sprintf(strbuff, "\n%d seconds", s_currentValue);
        strNumber = laString_CreateFromCharBuffer(strbuff, &AbelRegular_S20_Bold_Internal);

        laString_Append(&strStatus, &strLabel);
        laString_Append(&strStatus, &strNumber);
        laLabelWidget_SetText(labelPowerOffMessage, strStatus );

        laString_Destroy(&strLabel); 
        laString_Destroy(&strNumber); 
        laString_Destroy(&strStatus);
    }
}

/** @brief
 *  @param [in]
 *  @param [out]
 *  @return None
 */
void PowerOffScreen_ReInitialize(void)
{
   PowerOffScreen_Initialize();
}

/** @brief
 *  @param [in]
 *  @param [out]
 *  @return None
 */
void PowerOffScreen_SetIndicatorVisible(bool flag)
{
    laBool f;
    if (flag)
    {
        f = LA_TRUE;
    }
    else
    {
        f = LA_FALSE;
    }
    laWidget_SetVisible((laWidget*)labelPowerOffAdditionalMessage, f);
    laWidget_SetVisible((laWidget*)panelPowerOffAlarmIndicatorTop, f);
    laWidget_SetVisible((laWidget*)panelPowerOffAlarmIndicatorBottom, f);
    laWidget_SetVisible((laWidget*)panelPowerOffAlarmIndicatorLeft, f);
    laWidget_SetVisible((laWidget*)panelPowerOffAlarmIndicatorRight, f);
}

/** @brief
 *  @param [in]
 *  @param [out]
 *  @return None
 */
void PowerOffScreen_Run(void)
{
//    SYS_PRINT("\nRUN POWEROFF SCREEN.");
    PowerOffScreen_UpdatePoweroffCounter();
    PowerOffScreen_CheckSetIndicator();
}

/** @brief
 *  @param [in]
 *  @param [out]
 *  @return None
 */
void PowerOffScreen_Distructor(void)
{
    
}

/** @brief
 *  @param [in]
 *  @param [out]
 *  @return None
 */
static void PowerOffScreen_CheckSetIndicator(void)
{
    float o2Flow;
    
    o2Flow = DisplayControl_GetDataO2Flow();
//    o2Flow = 2.0;
    if (o2Flow > 1.0)
    {
        PowerOffScreen_SetIndicatorVisible(true);
    }
    else
    {
        PowerOffScreen_SetIndicatorVisible(false);
    }
}

/** @brief
 *  @param [in]
 *  @param [out]
 *  @return None
 */
void PowerOffScreen_CheckSetSound(void)
{
    float o2Flow;
    o2Flow = DisplayControl_GetDataO2Flow();
    if (o2Flow > 1.0)
    {
        deviceInterface_SendAudioEvent(ePlayLow1Alarm, setting_Get(eSpeakerVolumeSettingId));
    }
    else
    {
        deviceInterface_SendAudioEvent(eStopPlay, 0);
    }
}

/** @brief
 *  @param [in]
 *  @param [out]
 *  @return None
 */
void PowerOffScreen_StopSound(void)
{
    deviceInterface_SendAudioEvent(eStopPlay, 0);
}