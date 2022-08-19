/** @file VideoScreen.c
 *  @brief The screen provides graphic interface to update firmware
 *  @author Thanh Hiep Nguyen
 */

#ifndef _VIDEO_SCREEN
#define _VIDEO_SCREEN

/* This section lists the other files that are included in this file.
 */
#include "Gui/DisplayControl.h"
#include "GuiDefine.h"
#include "VideoControl.h"

// Section: Global Data Definitions
#define INTRO_VIDEO_SIZE 13056000
#define INTRO_VIDEO_FRAMESIZE 261120
#define FRAME_DECODE_DELAY 2 // delay playing for waiting decoding first frame

/** @brief Define the data variable in VideoScreen */ 
typedef struct
{
    /* The application's current state */
    int32_t loadingValue;
    uint8_t currentScheme;
    uint16_t startPlayStartupSoundTick;
    bool isStartupSoundPlaying;
    
} Video_Data_Struct;

/** @brief Define color ID for self check indicator */
typedef enum
{
    eStartupCyanSchemeID,
    eStartupYellowSchemeID,
    eStartupRedSchemeID
}E_SelfCheckIndicatorScheme_t;

VideoControl introVideoControl;

/** @brief Initialize VideoScreen's element: Check all VideoScreen's widgets have been created yet */
E_GuiResult VideoScreen_Initialize(void);

/** @brief Implement VideoScreen's state, animation function and some action periodlly */
void VideoScreen_Run(void);
void VideoScreen_VideoInit(VideoControl * video_control);
void VideoScreen_DecodeFrame(VideoControl *video_control, uint8_t frameIndex);

#endif	

// end of file