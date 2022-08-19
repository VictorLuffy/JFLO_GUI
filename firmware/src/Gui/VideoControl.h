#ifndef _VIDEO_CONTROL
#define _VIDEO_CONTROL

#include "../7z/LzmaEnc.h"
#include "../7z/LzmaDec.h"
#include "../7z/7zTypes.h"

#include "Gui/GuiDefine.h"

SemaphoreHandle_t xVideoControlSemaphore = NULL;
TickType_t xCallbackTick;

typedef enum
{
    eVideoControlIdleState,
    eVideoControlInitState,
    eVideoControlStartPlayState,
    eVideoControlPlayingState,
    eVideoControlCompleteState,
    eNoOfVideoControlState,
}VideoControlState;


typedef struct
{
     
    // Video Frame properties
    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
    unsigned int frameSizeInBytes;
    unsigned int frameLineInBytes;
    unsigned int frameIndex;
    unsigned int frameTotal;
    unsigned int frameReady;
    unsigned int frameRateMS;

    
    // Screen properties
    unsigned int screenHeight;
    unsigned int screenWidth;
    unsigned int bytesPerPixel;
    unsigned int screenSizeInBytes;

    
    //Playback Screen settings
    VideoControlState state;
    GFX_Context * gfxContext;
    uint8_t *outputBuffer;
    uint8_t *outputBuffer1;
    uint8_t *inputBuffer;
    unsigned int inputSizeInBytes;
    
    unsigned int playbackScreenID;
    SYS_TMR_HANDLE updateFrameTimerHandle;
    SYS_TMR_CALLBACK callback;
    bool isPlaying;
} VideoControl;

/** @brief Define the alarm animation data */
typedef struct {
    uint8_t *data; /**< data address */
    uint32_t size; /**< size of data */
} FrameData;

/** @brief Define the alarm animation data */
typedef struct {
    unsigned int x;
    unsigned int y;
    unsigned int w;
    unsigned int h;
    unsigned int videoSizeInBytes;
    unsigned int frameRateMS;  
    unsigned int bytesPerPixel;
    FrameData *frameData;
} VideoData;

typedef struct
{
    uint8_t *inputBuffer;
    uint8_t *outputBuffer;
} DecodeLzma ;

void VideoControl_DecodeLzma_SingleCall(uint8_t *inputBuffer, uint32_t inputSize, uint8_t *outputBuffer, uint32_t outputSize);
uint32_t VideoControl_ReadBufferFromFile(uint8_t** buffer, const char* dirPath, const char* fileName);
void VideoControl_InitControl(VideoControl *video_control);
void VideoControl_Deinit(VideoControl * video_control);
void VideoControl_UpdateFrame (uint8_t *src_add, uint32_t x, uint32_t y , uint32_t w, uint32_t h, uint32_t byte_per_pixel,  uint8_t *dest_add, uint32_t dest_w, uint32_t dest_h);
void VideoControl_DecodeFrame(VideoControl *video_control, uint8_t frameIndex, uint8_t *compressedBuffer, uint16_t compressedSize);
void VideoControl_StopPlayVideo(VideoControl * video_control);
void VideoControl_StartPlayVideo(VideoControl * video_control);

#endif
/* end of file */
