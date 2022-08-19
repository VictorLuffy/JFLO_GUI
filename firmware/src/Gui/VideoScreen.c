/** @file VideoScreen.c
 *  @brief The screen plays the introduction video JFLO.
 *  @author Thanh Hiep Nguyen
 */

/* This section lists the other files that are included in this file.
 */
#include "system_config.h"
#include "system_definitions.h"


#include "gfx/hal/inc/gfx_context.h"
#include "gfx/libaria/inc/libaria_widget.h"
#include "gfx/libaria/inc/libaria_widget_label.h"
#include "gfx/libaria/inc/libaria_widget_button.h"
#include "gfx/libaria/libaria_init.h"

#include "Gui/VideoScreen.h"
#include "Gui/mm.h"
#include "Gui/GuiDefine.h"
#include "Gui/DisplayControl.h"
#include "USBInterface.h"
#include "Audio.h"
#include "PWM_LCDBacklight.h"

/** @brief Bytes in 1 pixel */
#define DEFAULT_BYTES_PER_PIXEL  2

/** @brief SQI Mounting Status */
#define NUM_ROWS_BUFFERED 128

/** @brief Video Layer Number */
#define VIDEO_LAYER_ID      0

/** @brief Startup sound timeout*/
#define STARTUP_SOUND_TIMEOUT   (3000/5)

/** @brief SQI Mounting Status */
static Video_Data_Struct gs_videoscreenData;
///** @brief Playing startup sound counter*/
//static uint16_t gs_startPlayStartupSoundTick = 0;
///** @brief Scheme variable for selfcheck screen*/
//static uint8_t s_currentScheme = eStartupCyanSchemeID;
///** @brief Startup sound playing state */
//static bool gs_videoscreenData.isStartupSoundPlaying = false;

static void VideoScreen_SetIndicatorVisible(bool flag);
static void VideoScreen_SetIndictorScheme(E_SelfCheckIndicatorScheme_t schemeID);
static void VideoScreen_StartupSoundStart(void);
static void VideoScreen_StartupSoundStop(void);
static bool VideoScreen_CheckStartupSoundStop(void);
static void VideoScreen_SetStartupSoundPlayingState(bool flag);
static bool VideoScreen_GetStartupSoundPlayingState(void);
/** @brief Initialize videoscreen's element
 *  @param [in]  None
 *  @param [out]  None
 *  @retval eGNotCompletedYet
 *  @retval eGCompleted
 */
E_GuiResult VideoScreen_Initialize(void)
{
    introVideoControl.state = eVideoControlInitState;
    gs_videoscreenData.loadingValue = 0;
    gs_videoscreenData.startPlayStartupSoundTick = 0;
    gs_videoscreenData.currentScheme = eStartupCyanSchemeID;
    gs_videoscreenData.isStartupSoundPlaying = false;
    VideoScreen_SetIndicatorVisible(false);
    return eGCompleted;
}

/** @brief Initialize videoscreen's element
 *  @param [in]  None
 *  @param [out]  None
 *  @retval eGNotCompletedYet
 *  @retval eGCompleted
 */
E_GuiResult VideoScreen_ReInit(void)
{
    return eGCompleted;
}

/** @brief Implement VideoScreen's state
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void VideoScreen_Run(void) {
    switch (introVideoControl.state) {
        case eVideoControlIdleState:
            break;
            
        case eVideoControlInitState:
            if (FileSystemMgr_IsSQIMounted()) //FIXME : is this necessary ?
            {   
//                SYS_PRINT("\n VideoScreen_Run - eInitDispState \n");                
                introVideoControl.state = eVideoControlStartPlayState;
                VideoScreen_VideoInit(&introVideoControl);
                VideoControl_StartPlayVideo(&introVideoControl);            
                introVideoControl.frameReady = 0;
                //enable LCD back light here to fix problem of screen white flash when turn on JFLO
                PWM_LCDBacklight_Enable();
            }
            else
            {
                SYS_PRINT("\n Error: SQI not mount \n");
            }
            break;
            
        case eVideoControlStartPlayState:
            VideoScreen_StartupSoundStart();
            if (introVideoControl.frameReady < introVideoControl.frameTotal)            
            {
                // Playing video
//                SYS_PRINT("VideoScreen_Run - ePlayVideoDispState %d  \n", introVideoControl.frameReady);
                uint8_t *compressedBuffer = introVideoInputData[introVideoControl.frameReady];
                uint16_t size = g_fileList[introVideoControl.frameReady + INTRO_VIDEO_START_INDEX].fileSize ;
                VideoControl_DecodeFrame(&introVideoControl, introVideoControl.frameReady, compressedBuffer, size);
                introVideoControl.frameReady++;
            }
            else
            {
                introVideoControl.state = eVideoControlPlayingState;
            }
            break;
        case eVideoControlPlayingState:
            if (introVideoControl.frameIndex >= introVideoControl.frameTotal)
            {
                introVideoControl.state = eVideoControlCompleteState;
            }

            break;
        case eVideoControlCompleteState:
//            SYS_PRINT("VideoScreen_Run - eFinishedVideoDispState\n");
            VideoControl_StopPlayVideo(&introVideoControl);
            unsigned long frame_add = (introVideoControl.frameTotal - 1) * introVideoControl.frameSizeInBytes ;
            
            memcpy(introVideoControl.gfxContext->layer.layers[0].buffers[1].pb.pixels, 
                introVideoControl.outputBuffer, 
                introVideoControl.frameSizeInBytes);
            memcpy(introVideoControl.gfxContext->layer.layers[0].buffers[1].pb.pixels, 
                introVideoControl.outputBuffer, 
                introVideoControl.frameSizeInBytes);
            
            VideoScreen_SetIndicatorVisible(true);
            
            if (gs_videoscreenData.loadingValue % 20 == 0)
            {
                VideoScreen_SetIndictorScheme(gs_videoscreenData.currentScheme);
                gs_videoscreenData.currentScheme++;
                if (gs_videoscreenData.currentScheme > eStartupRedSchemeID)
                    gs_videoscreenData.currentScheme =  eStartupCyanSchemeID;
            }
            gs_videoscreenData.loadingValue += 2;
            delay_SoftDelay(95);
            laProgressBarWidget_SetValue(prgBarLoading, (uint32_t)gs_videoscreenData.loadingValue);

            JFLO_Slider_Scheme.textHighlight = 0x07FF;
            
            if (gs_videoscreenData.loadingValue == 100)
            { 
//                VideoScreen_SetIndicatorVisible(false);
                VideoControl_Deinit(&introVideoControl);
                DisplayControl_SetState(eMainScreenIsShowingDispState);
                
                //Send start motor event
                MOTOR_CTRL_EVENT_t eventStart = {.id = eMotorStartId, .iData = 0};
                //send event to Flow Control Task to start motor
                MotorTask_SendEvent(eventStart);
//                SYS_PRINT("Video is finished, starting motor\n");
            }
            
            break;   
            
        default:
            break;
    }
    
    if (VideoScreen_GetStartupSoundPlayingState())
    {
        if (VideoScreen_CheckStartupSoundStop())
        {
            VideoScreen_SetStartupSoundPlayingState(false);
            VideoScreen_StartupSoundStop();
        }
    }
    
}

/** @brief Distruct some VideoScreen backend elements when switched to other one
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void VideoScreen_Distructor(void)
{
    
    return;
}

void VideoScreen_FrameUpdateCallback(uintptr_t context, uint32_t currTick)
{   
//    SYS_PRINT("VideoScreen_FrameUpdateCallback %d (ms) index %d \n", xTaskGetTickCount() - xCallbackTick, introVideoControl.frameIndex);
    xCallbackTick = xTaskGetTickCount();
    if (introVideoControl.frameIndex >= introVideoControl.frameTotal ||
            introVideoControl.frameIndex + FRAME_DECODE_DELAY >= introVideoControl.frameReady && 
            introVideoControl.frameReady <  introVideoControl.frameTotal )
    {
//        SYS_PRINT("\n Error : frame invalid %d %d %d \n", introVideoControl.frameIndex, introVideoControl.frameReady, introVideoControl.frameTotal);
        return;
    }
    unsigned long frame_add = introVideoControl.frameIndex * introVideoControl.frameSizeInBytes ;
    VideoControl_MemCopy(GFX_LayerWriteBuffer(GFX_ActiveContext()->layer.active)->pixels,
                        introVideoControl.inputBuffer + frame_add,
                        introVideoControl.frameSizeInBytes);

    GFX_Set(GFXF_LAYER_SWAP, GFX_TRUE);
    if (++introVideoControl.frameIndex >= introVideoControl.frameTotal) 
    {
        introVideoControl.frameIndex = introVideoControl.frameTotal;
    }
}

void VideoScreen_VideoInit(VideoControl * video_control)
{
    // TODO: the screen is oriented to portrait, so the position is need to converted
    video_control->x = 0;
    video_control->y = 0;
    video_control->w = 480; 
    video_control->h = 272;
    video_control->bytesPerPixel = 2;
    video_control->frameRateMS = 1000 / 8 ; // 8fps
    video_control->callback = &VideoScreen_FrameUpdateCallback;
    video_control->inputSizeInBytes = INTRO_VIDEO_SIZE;
    
    VideoControl_InitControl(&introVideoControl);                                
}

/** @brief Set visible for selfcheck screen indicator
 *  @param [in]  bool flag
 *  @param [out]  None
 *  @return None
 */
static void VideoScreen_SetIndicatorVisible(bool flag)
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
    
    laWidget_SetVisible((laWidget *)panelSelfCheckIndicatorLeft, f);
    laWidget_SetVisible((laWidget *)panelSelfCheckIndicatorRight, f);
    laWidget_SetVisible((laWidget *)panelSelfCheckIndicatorTop, f);
    laWidget_SetVisible((laWidget *)panelSelfCheckIndicatorBottom, f);
}

/** @brief Set scheme for selfcheck screen indicator
 *  @param [in]  bool flag
 *  @param [out]  None
 *  @return None
 */
static void VideoScreen_SetIndictorScheme(E_SelfCheckIndicatorScheme_t schemeID)
{
//    if (schemeID < eStartupCyanSchemeID || schemeID > eStartupRedSchemeID)
//        schemeID = eStartupCyanSchemeID;
    
    switch(schemeID)
    {
    case eStartupCyanSchemeID:
    {
        laWidget_SetScheme(panelSelfCheckIndicatorLeft, &JFLO_CyanIndicator_Scheme);
        laWidget_SetScheme(panelSelfCheckIndicatorRight, &JFLO_CyanIndicator_Scheme);
        laWidget_SetScheme(panelSelfCheckIndicatorTop, &JFLO_CyanIndicator_Scheme);
        laWidget_SetScheme(panelSelfCheckIndicatorBottom, &JFLO_CyanIndicator_Scheme);
    }
        break;
    case eStartupYellowSchemeID:
    {
        laWidget_SetScheme(panelSelfCheckIndicatorLeft, &JFLO_YellowIndicator_Scheme);
        laWidget_SetScheme(panelSelfCheckIndicatorRight, &JFLO_YellowIndicator_Scheme);
        laWidget_SetScheme(panelSelfCheckIndicatorTop, &JFLO_YellowIndicator_Scheme);
        laWidget_SetScheme(panelSelfCheckIndicatorBottom, &JFLO_YellowIndicator_Scheme);
    }
        break;
    case eStartupRedSchemeID:
    {
        laWidget_SetScheme(panelSelfCheckIndicatorLeft, &JFLO_RedIndicator_Scheme);
        laWidget_SetScheme(panelSelfCheckIndicatorRight, &JFLO_RedIndicator_Scheme);
        laWidget_SetScheme(panelSelfCheckIndicatorTop, &JFLO_RedIndicator_Scheme);
        laWidget_SetScheme(panelSelfCheckIndicatorBottom, &JFLO_RedIndicator_Scheme);
    }
        break;
    }
}

/** @brief Set startup sound playing state
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
static void VideoScreen_SetStartupSoundPlayingState(bool flag)
{
    gs_videoscreenData.isStartupSoundPlaying = flag;
}

/** @brief Get startup sound playing state
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
static bool VideoScreen_GetStartupSoundPlayingState(void)
{
    return gs_videoscreenData.isStartupSoundPlaying;
}

/** @brief Send Audio event to play startup sound
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
static void VideoScreen_StartupSoundStart(void)
{
    if (gs_videoscreenData.startPlayStartupSoundTick == 0)
    {
        gs_videoscreenData.startPlayStartupSoundTick = xTaskGetTickCount();
        deviceInterface_SendAudioEvent(ePlayStartupSound ,setting_Get(eSpeakerVolumeSettingId));
        VideoScreen_SetStartupSoundPlayingState(true);
        
    }
}

/** @brief Send Audio event to play stop sound
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
static void VideoScreen_StartupSoundStop(void)
{
    deviceInterface_SendAudioEvent(eStopPlay , 0);
}

/** @brief Check stop play startup sound sound
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
static bool VideoScreen_CheckStartupSoundStop(void)
{
    if ((xTaskGetTickCount() - gs_videoscreenData.startPlayStartupSoundTick) >= 3000)
    {
        return true;
    }
    else
    {
        return false;
    }
}


/* end of file */
