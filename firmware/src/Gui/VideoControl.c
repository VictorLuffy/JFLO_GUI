#include "VideoControl.h"

static void *SzAlloc(ISzAllocPtr p, size_t size) {return (void*)mm_malloc(size); }
static void SzFree(ISzAllocPtr p, void *address) {mm_free(address); }
const ISzAlloc g_Alloc = { SzAlloc, SzFree };
#define IN_BUF_SIZE (1 << 16)
//#define OUT_BUF_SIZE (1 << 16)
#define OUT_BUF_SIZE (1 << 18)

void VideoControl_DecodeLzma_SingleCall(uint8_t *inputBuffer, uint32_t inputSize, uint8_t *outputBuffer, uint32_t outputSize)
{
//    SYS_PRINT("\nVideoControl_DecodeLzma\n");   
    UInt64 unpackSize;
    int i;
    SRes res = 0;

    /* header: 5 bytes of LZMA properties and 8 bytes of uncompressed size */
    unsigned char header[LZMA_PROPS_SIZE + 8];

    /* Read and parse header */

    memcpy(header, inputBuffer, sizeof(header));

    unpackSize = 0;
    for (i = 0; i < 8; i++)
    {
      unpackSize += (UInt64)header[LZMA_PROPS_SIZE + i] << (i * 8);
//      SYS_PRINT("unpackSize %llu\n", unpackSize);
    }   
    if (unpackSize != outputSize)
    {
        SYS_PRINT("Error : Unmatch unpackSize %llu \n", unpackSize);
        return;
    }
    
    Byte *dest = outputBuffer;
    SizeT destLen = unpackSize;
    const Byte *src = inputBuffer;
    SizeT srcLen = inputSize;

    ELzmaFinishMode finishMode = LZMA_FINISH_END;
    ELzmaStatus status;
//    TickType_t xGuiTick = xTaskGetTickCount();
    res = LzmaDecode(dest,
                     &destLen,
                     src + sizeof(header),
                     &srcLen,
                     header,
                     LZMA_PROPS_SIZE,
                     finishMode,
                     &status,
                     &g_Alloc);

//    xGuiTick = xTaskGetTickCount() - xGuiTick;
//    SYS_PRINT("LzmaDecode %d\n",xGuiTick);     
    if (res != 0)
    {
        SYS_PRINT("[Gui][Debug] LzmaDecode res %d inputSize %d outputSize %d \n",res, inputSize, outputSize);
    }
    
}

/** @brief Read file into buffer, alloc/free memory handled by caller
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
uint32_t VideoControl_ReadBufferFromFile(uint8_t** buffer, const char* dirPath, const char* fileName)
{
    uint32_t fileSize = 0;
    uint32_t readBytes =0;
    char filePath[255];
    strcpy(filePath, dirPath);
    strcat(filePath, "/");
    strcat(filePath, fileName);

    if (USBInterface_Search(dirPath,fileName) == SYS_FS_RES_SUCCESS)
    {
        SYS_FS_HANDLE file_handle = SYS_FS_FileOpen(filePath, SYS_FS_FILE_OPEN_READ);       
        fileSize = SYS_FS_FileSize(file_handle);
        SYS_FS_FileSeek(file_handle, 0, SYS_FS_SEEK_SET);

        if (fileSize > 0)
        {
            if (*buffer > 0)
            {
                readBytes = SYS_FS_FileRead(file_handle, *buffer, fileSize);
//                SYS_PRINT("\n readBytes = %d ", readBytes);
                if (readBytes != fileSize)
                {
                    SYS_PRINT("\n error readBytes != fileSize %d %d %s ", readBytes, fileSize, filePath );
                    
                }
            }
        }
        // Close file
        SYS_FS_FileClose(file_handle);        
    }    
    else 
    {
        SYS_PRINT("\n File not available in USB %s %s \n", dirPath, fileName);
    }
    return fileSize;
}

/** @brief init video control params, alloc/free memory handled by caller
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void VideoControl_InitControl(VideoControl *video_control)
{
//    SYS_PRINT("VideoControl_InitControl \n");
    if ( xVideoControlSemaphore == NULL )
    {
        xVideoControlSemaphore = xSemaphoreCreateMutex();
    }
    if ( xVideoControlSemaphore != NULL )
    {
    }
    else
    {
        SYS_PRINT("Error: xVideoControlSemaphore is NULL \n");
    }

    // Initialize playback settings
    video_control->gfxContext = GFX_ActiveContext();
    video_control->outputBuffer = video_control->gfxContext->layer.layers[0].buffers[0].pb.pixels;
    video_control->outputBuffer1 = video_control->gfxContext->layer.layers[0].buffers[1].pb.pixels;
    video_control->screenWidth = video_control->gfxContext->display_info->rect.width;
    video_control->screenHeight = video_control->gfxContext->display_info->rect.height;
    video_control->screenSizeInBytes = video_control->screenHeight * video_control->screenWidth * video_control->bytesPerPixel;
    video_control->frameLineInBytes = video_control->w * video_control->bytesPerPixel;
    video_control->frameIndex = 0;    
    video_control->isPlaying = false;
    
    video_control->frameSizeInBytes = video_control->w * video_control->h * video_control->bytesPerPixel;
    video_control->frameTotal = video_control->inputSizeInBytes / video_control->frameSizeInBytes;
    video_control->frameReady = 0;
    video_control->inputBuffer = mm_malloc(video_control->inputSizeInBytes);
    
    //Set video frame (base) to RGB565
    GFX_Set(GFXF_LAYER_ACTIVE, 0); 
    GFX_Set(GFXF_COLOR_MODE, GFX_COLOR_MODE_RGB_565);

    return;
}

void VideoControl_StartPlayVideo(VideoControl * video_control)
{
//    SYS_PRINT("VideoControl_StartPlayVideo \n");            
    if (!video_control->isPlaying)
    {
        video_control->frameIndex = 0;
        video_control->isPlaying = true;
        video_control->updateFrameTimerHandle = 
                SYS_TMR_CallbackPeriodic(
                    video_control->frameRateMS,
                    1,
                    video_control->callback);        
    }
}

void VideoControl_StopPlayVideo(VideoControl * video_control)
{
//    SYS_PRINT("VideoControl_StopPlayVideo \n");            
    if (video_control->isPlaying)
    {
//        SYS_PRINT("VideoControl_StopPlayVideo, stop callback \n"); 
        SYS_TMR_CallbackStop(video_control->updateFrameTimerHandle);
        video_control->isPlaying = false;
    }
}

void VideoControl_Deinit(VideoControl * video_control)
{
//    SYS_PRINT("VideoControl_Deinit \n");
    VideoControl_StopPlayVideo(video_control);
    if (video_control->inputBuffer)
    {
        mm_free(video_control->inputBuffer);
        video_control->inputBuffer = 0;
    }
}

void VideoControl_UpdateFrame (uint8_t *src_add, uint32_t x, uint32_t y , uint32_t w, uint32_t h, uint32_t byte_per_pixel,  uint8_t *dest_add, uint32_t dest_w, uint32_t dest_h)
{    
    SYS_PRINT(" %d %d %d %d %d %d %d %d %d \n" ,src_add, x, y, w, h, byte_per_pixel, dest_add, dest_w, dest_h );
	uint32_t line;
    uint32_t screenSizeInBytes = dest_w * dest_h * byte_per_pixel;
    uint32_t frameLineInBytes = w * byte_per_pixel;
    uint32_t frameSizeInBytes = w * h * byte_per_pixel;
    uint32_t dest_add_line = 0;
    uint32_t src_add_line = 0;

    for (line = 0 ; line < h; line++)
    {
        dest_add_line = x + (y + line ) * dest_w * byte_per_pixel;
        src_add_line = line * frameLineInBytes;
        if ( dest_add_line + frameLineInBytes > screenSizeInBytes) 
        {
            SYS_PRINT("error line %d dest_add_line %d frameLineInBytes %d screenSizeInBytes %d \n" ,line, dest_add_line, frameLineInBytes, screenSizeInBytes );
            break;
        }
        if ( src_add_line + frameLineInBytes > frameSizeInBytes)
        {
            SYS_PRINT("error line %d src_add_line %d frameLineInBytes %d frameSizeInBytes %d \n" ,line, src_add_line, frameLineInBytes, frameSizeInBytes );
            break;
        }
        if ( dest_add > 0 && src_add > 0)
        {
            memcpy(dest_add + dest_add_line, 
                    src_add + src_add_line, 
                    frameLineInBytes);
        }
        else
        {
            SYS_PRINT("error inputBuffer, outputBuffer");
            return;
        }
    }
}

void VideoControl_MemCopy(void* dest, const void *src , size_t s )
{
    if ( xVideoControlSemaphore != NULL )
    {
        if( xSemaphoreTake( xVideoControlSemaphore, ( TickType_t ) 20 ) == pdTRUE )
        {
            memcpy(dest, src, s);
            xSemaphoreGive( xVideoControlSemaphore );
        }
        else
        {
            SYS_PRINT("Error: Failed to take xVideoControlSemaphore \n");
        }
    }
    else
    {
        SYS_PRINT("Error: xVideoControlSemaphore is NULL \n");
    }
}

void VideoControl_DecodeFrame(VideoControl *video_control, uint8_t frameIndex, uint8_t *compressedBuffer, uint16_t compressedSize)
{        
    unsigned long frame_add = frameIndex * video_control->frameSizeInBytes;
    uint8_t *frameBuffer = mm_malloc(video_control->frameSizeInBytes);
//    TickType_t xGuiTick = xTaskGetTickCount();
//    SYS_PRINT("VideoControl_DecodeFrame compressedSize %d outputSize %d \n", compressedSize, video_control->frameSizeInBytes);
    VideoControl_DecodeLzma_SingleCall(compressedBuffer, compressedSize, frameBuffer, video_control->frameSizeInBytes);
    VideoControl_MemCopy(video_control->inputBuffer+frame_add, frameBuffer, video_control->frameSizeInBytes);
//    xGuiTick = xTaskGetTickCount() - xGuiTick;
//    SYS_PRINT("VideoControl_DecodeLzma (tick) %d frameReady %d \n",xGuiTick, frameIndex);    
    mm_free(frameBuffer);
}

/* end of file */