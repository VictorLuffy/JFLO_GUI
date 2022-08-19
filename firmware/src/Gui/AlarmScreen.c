/** @file AlarmScreen.c
 *  @brief The screen plays the introduction video JFLO.
 *  @author Thanh Hiep Nguyen
 */

/* This section lists the other files that are included in this file.
 */
//#include "system_config.h"
//#include "system_definitions.h"

#include "Gui/VideoScreen.h"
#include "Gui/AlarmScreen.h"
#include "Gui/mm.h"
#include "Gui/GuiDefine.h"
#include "Gui/DisplayControl.h"
#include "USBInterface.h"
#include "VideoControl.h"

#define VIDEO_LAYER_ID      0

static SYS_TMR_HANDLE s_updateAlarmFrame;

AlarmScreen_Control_Struct   gs_alarmscreenCtrl;

uint8_t *rawBuffer;

static bool AlarmScreen_PlayVideoCallback(uint32_t currentTick, bool isInit);


char *ValueToString(int value);


extern bool g_isMountUSB;
extern bool g_isMountSQI;


int AlarmScreen_StartPlay(void)
{
    uint32_t frameperiod = (uint32_t)(1000/gs_alarmscreenCtrl.fps);
    SYS_PRINT("\n fps = %d, ms = %u", gs_alarmscreenCtrl.fps, frameperiod);
    SYS_PRINT("\n frameNum = %d", gs_alarmscreenCtrl.frameNum);
    uint32_t totalSector, freeSector;
    if (SYS_FS_DriveSectorGet("mnt/SQIFlash", &totalSector, &freeSector) != SYS_FS_RES_SUCCESS)
        SYS_PRINT("\nSYS_FS_DriveSectorGet Failed");
    else 
        SYS_PRINT("\n totalSector = %u, freeSector = %u", totalSector, freeSector);
    
    gs_alarmscreenCtrl.frameIndex = 0;   

    AlarmScreen_PlayVideoCallback(GET_TICKS(), true);

    return 0;
}

int AlarmScreen_StopPlay(void)
{


    return 0;
}

/** @brief Call 
 * init control & data variable and change some properties in needed initialization widgets
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
int AlarmScreen_PlayVideo (void ){

    
    return 1;
}


/** @brief Initialize settingscreen's element: Check all settingscreen's widgets have been created yet,
 * init control & data variable and change some properties in needed initialization widgets
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */

E_GuiResult AlarmScreen_Initialize(void)
{
    gs_alarmscreenCtrl.state = eInitDispState;
//    gs_alarmscreenCtrl.fps = 6;
    gs_alarmscreenCtrl.fps = 100;
    gs_alarmscreenCtrl.frameIndex = 0;
    return eGFalse;
}

E_GuiResult AlarmScreen_Reinit(void)
{
    gs_alarmscreenCtrl.state = eInitDispState;
//    gs_alarmscreenCtrl.fps = 6;
    gs_alarmscreenCtrl.fps = 100;
    gs_alarmscreenCtrl.frameIndex = 0;   
    return eGFalse;
}

//void UpdateFrame (uint8_t *src_add, uint32_t x, uint32_t y , uint32_t w, uint32_t h, uint32_t byte_per_pixel,  uint8_t *dest_add, uint32_t dest_w, uint32_t dest_h)
//{    
////    SYS_PRINT(" %d %d %d %d %d %d %d %d %d \n" ,src_add, x, y, w, h, byte_per_pixel, dest_add, dest_w, dest_h );
//	uint32_t line;
//    uint32_t screenSizeInBytes = dest_w * dest_h * byte_per_pixel;
//    uint32_t frameLineInBytes = w * byte_per_pixel;
//    uint32_t frameSizeInBytes = w * h * byte_per_pixel;
//    uint32_t dest_add_line = 0;
//    uint32_t src_add_line = 0;
//
//    for (line = 0 ; line < h; line++)
//    {
//        dest_add_line = x + (y + line ) * dest_w * byte_per_pixel;
//        src_add_line = line * frameLineInBytes;
////        SYS_PRINT("line %d dest_add %d input_add %d \n" ,line, dest_add_line, src_add_line );
//        // protect overflow
//        if ( dest_add_line + frameLineInBytes > screenSizeInBytes) 
//        {
//            SYS_PRINT("error line %d dest_add_line %d frameLineInBytes %d screenSizeInBytes %d \n" ,line, dest_add_line, frameLineInBytes, screenSizeInBytes );
//            break;
//        }
//        if ( src_add_line + frameLineInBytes > frameSizeInBytes)
//        {
//            SYS_PRINT("error line %d src_add_line %d frameLineInBytes %d frameSizeInBytes %d \n" ,line, src_add_line, frameLineInBytes, frameSizeInBytes );
//            break;
//        }
//        if ( dest_add > 0 && src_add > 0)
//        {
//            memcpy(dest_add + dest_add_line, 
//                    src_add + src_add_line, 
//                    frameLineInBytes);
//        }
//        else
//        {
//            SYS_PRINT("error inputBuffer, outputBuffer");
//            return;
//        }
//    }
//}

static bool AlarmScreen_PlayVideoCallback(uint32_t currentTick, bool isInit)
{
    static uint32_t triggerTick;
    static uint32_t lastTick;    
    
    //  Period timer while MainScreen_Run is working
    if (currentTick > triggerTick || lastTick > currentTick || isInit)
    {
        triggerTick = currentTick + (uint32_t)(1000/gs_alarmscreenCtrl.fps) * TICKS_PER_MS;
        lastTick = currentTick;
       
    }
    else 
        return false;    
   
//    SYS_PRINT("\n gs_alarmscreenCtrl.frameIndex = %d", gs_alarmscreenCtrl.frameIndex);
    TickType_t xGuiTick = xTaskGetTickCount();
//    laImageWidget_SetImage(ImageFrame_AlarmScreen, g_alarmImagePointer[gs_alarmscreenCtrl.frameIndex]);
    xGuiTick = xTaskGetTickCount() - xGuiTick;
    if (xGuiTick > 5)
        SYS_PRINT("laImageWidget_SetImage %d\n",xGuiTick); 

//    GFX_Set(GFXF_LAYER_ACTIVE, 0);
//    GFX_Set(GFXF_COLOR_MODE, GFX_COLOR_MODE_RGB_565);
////    GFX_Set(GFXF_COLOR_MODE, GFX_COLOR_MODE_RGB_888);
//
//    
//    uint8_t *compressedBuffer = mm_malloc(5000);
//    char filePath[255];
//    char num[255];
//    itoa(num, gs_alarmscreenCtrl.frameIndex, 10);
////    strcpy(filePath, "IntroVid_");
//    strcpy(filePath, "Plug_AlarmVid_");
//    strcat(filePath, num);
//    strcat(filePath, ".jpeg");
//    VideoControl_ReadBufferFromFile(&compressedBuffer, "/mnt/USB/Upgrade/VideoImages", filePath);
////    VideoControl_ReadBufferFromFile(&compressedBuffer, "/mnt/USB/Upgrade/IntroVideo", filePath);
//    
//    rawBuffer = (uint8_t*)mm_malloc(236*244*2);
//    memset(rawBuffer, 0xFF, 236*244*2);
//
////    rawBuffer = (uint8_t*)mm_malloc(272*480*2);
////    memset(rawBuffer, 0xFF, 272*480*2);
//
//
//    if (g_alarmImagePointer[gs_alarmscreenCtrl.frameIndex]->header.dataAddress == 0)
//    {
//        g_alarmImagePointer[gs_alarmscreenCtrl.frameIndex]->header.dataAddress = compressedBuffer;
//        g_alarmImagePointer[gs_alarmscreenCtrl.frameIndex]->header.dataLocation = 0;        
//        TickType_t xGuiTick = xTaskGetTickCount();
//        GFX_Result ret = GFXU_PreprocessImage(g_alarmImagePointer[gs_alarmscreenCtrl.frameIndex], (uint32_t)rawBuffer, GFX_COLOR_MODE_RGB_565, GFX_FALSE);
//        xGuiTick = xTaskGetTickCount() - xGuiTick;
//        if (xGuiTick > 5)
//            SYS_PRINT("AlarmScreen_PlayVideoCallback %d\n",xGuiTick); 
//
//    }
//
//    UpdateFrame (
//            g_alarmImagePointer[gs_alarmscreenCtrl.frameIndex]->header.dataAddress, 
//            0, 
//            20, 
//            236, 
//            244, 
//            2, 
//            GFX_ActiveContext()->layer.layers[0].buffers[0].pb.pixels , 
//            480, 
//            272);
//
////    UpdateFrame (
////            rawBuffer, 
////            0, 
////            0, 
////            272, 
////            480, 
////            2, 
////            GFX_ActiveContext()->layer.layers[0].buffers[0].pb.pixels , 
////            272, 
////            480);
//    
//    mm_free(rawBuffer);
//    mm_free(compressedBuffer);
    gs_alarmscreenCtrl.frameIndex += FRAMESTEP_INC;
    
    return true;
}




uint8_t AlarmScreen_GetFrameIndex(void)
{
    return gs_alarmscreenCtrl.frameIndex;
}

uint8_t AlarmScreen_GetFrameNumber(void)
{
    return gs_alarmscreenCtrl.frameNum;
}

char *ValueToString(int value) {
    static char buffer[12]; // 12 bytes is big enough for an INT32
    int original = value; // save original value

    int c = sizeof (buffer) - 1;

    buffer[c] = 0; // write trailing null in last byte of buffer    

    if (value < 0) // if it's negative, note that and take the absolute value
        value = -value;

    do // write least significant digit of value that's left
    {
        buffer[--c] = (value % 10) + '0';
        value /= 10;
    } while (value);

    if (original < 0)
        buffer[--c] = '-';

    return &buffer[c];
}

void AlarmScreen_createImagePath(uint8_t alarm)
{
    if (alarm == 0)
    {
        gs_alarmscreenCtrl.frameNum = 48;
        uint8_t cnt;
        for (cnt = 0; cnt < gs_alarmscreenCtrl.frameNum ; cnt ++)
        {
//            memset(g_alarmVideoNames[cnt], 0, 80);
//            strcat(g_alarmVideoNames[cnt], "Plug_AlarmVid_");
//            strcat(g_alarmVideoNames[cnt], ValueToString(cnt + 1));
//            strcat(g_alarmVideoNames[cnt], ".jpeg");   
        }    
    }
    
    return;
}

/** @brief Implement AlarmScreen's state, animation function and some action periodically
 *  It was called by DisplayControl.
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmScreen_Run(void) {
//    switch (gs_alarmscreenCtrl.state) {
//        case eIdleDispState:
//            break;
//            
//        case eInitDispState:
//            if (g_isMountSQI)
//            {   
//                SYS_PRINT("\ngs_alarmscreenCtrl.state = eInitDispState;");                
//                gs_alarmscreenCtrl.state = ePlayVideoDispState;
//
//                VideoScreen_VideoInit(&introVideoControl);
//                VideoControl_StartPlayVideo(&introVideoControl);            
//                introVideoControl.frameReady = 0;
//            }
//            break;
//            
//        case ePlayVideoDispState:
////            SYS_PRINT("\ngs_alarmscreenCtrl.state = ePlayVideoDispState;");
////            AlarmScreen_createImagePath(0);
////            AlarmScreen_StartPlay();
//            if (introVideoControl.frameReady < introVideoControl.frameTotal)            
//            {
//                VideoScreen_DecodeFrame(&introVideoControl, introVideoControl.frameReady);
//                introVideoControl.frameReady++;
//            }
//            else
//            {
////                xSemaphoreGive( xVideoControlSemaphore);
////                SYS_PRINT(" Release semaphore \n");
//            }
////            gs_alarmscreenCtrl.state = ePlayingVideoDispState;
//            break;
//            
//        case ePlayingVideoDispState:
////            AlarmScreen_PlayVideoCallback(GET_TICKS(), false);
////            if (gs_alarmscreenCtrl.frameIndex > gs_alarmscreenCtrl.frameNum - 1)
////            {
////                AlarmScreen_StopPlay();
//                DisplayControl_SetState(eMaintenanceScreenIsShowingDispState);
////            }    
//            break;
//            
//        default:
//            break;
//    }
}


/** @brief Distruct some AlarmScreen backend elements when switched to other one
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmScreen_Distructor(void)
{
    
    return;
}

/* end of file */
