/** @file SQIInterface.c
 *  @brief Contains functions for interface with SQI flash
 *  @author Viet Le
 */
#include "SQIInterface.h"
#include "Gui/GuiInterface.h"
#include "Gui/GuiDefine.h"
#include "crc.h"
#include "mm.h"

#define MAX_FILE_DATA_BUFFER      200*1024//(115*1024) // The biggest file --> Video: 100kb + 2bytes CRC
#define MAX_FILE_NAME            (50)

extern bool g_isMountSQI;

extern bool g_isMountUSB;

SYS_FS_HANDLE graphicBinFile;

SYS_FS_HANDLE s_alarmImageHandle;

__attribute__((section(".ddr_data"), space(prog))) uint8_t clone_buffer[MAX_FILE_DATA_BUFFER];
__attribute__((section(".ddr_data"), space(prog))) uint8_t check_buffer[MAX_FILE_DATA_BUFFER];

//uint8_t* clone_buffer;
//uint8_t* check_buffer;

/** @brief Declare sqi handle */
SYS_FS_HANDLE g_sqiHandle;

/** @brief Function Mount SQI flash
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SQIInterface_Mount(void)
{
    static int counter = 0;

    if (g_isMountSQI == false)
    {
        if (counter < 20) //20*10 = 200ms (10 : delay 10ms system task))
        {
            counter++;
            return;
        }
        //SYS_PRINT("sqi_mount_start\n");
        if (SYS_FS_Mount(SYS_FS_MEDIA_IDX0_DEVICE_NAME_VOLUME_IDX0, SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0, FAT, 0, NULL) == SYS_FS_RES_SUCCESS)
        {
            g_isMountSQI = true;
            //Format
            SQIInterface_Format();
            //SYS_PRINT("sqi_mount_success %d\n");
        }
    }
    else
    {
        if (counter != 0)
        {
            counter = 0;
        }
        //SYS_PRINT("g_isMountSQI %d\n ",g_isMountSQI);
    }
    
    return;
}

/** @brief Read and check CRC file in SQI flash
 *  @param [in] uint8_t *buffer : buffer data
 *  @param [out] None
 *  @return None
 */
static bool SQIInterface_ReadAndCheckCRC(uint8_t *buffer, int32_t fileSize)
{
    SYS_FS_RESULT fsRes;
    bool res = false;

    int32_t fileSize_CRC;
    
    fileSize_CRC = SYS_FS_FileSize(g_sqiHandle);
    if (fileSize != fileSize_CRC - 2)
    {
        SYS_PRINT("\n SQIInterface_ReadAndCheckCRC -  fileSize vs fileSize_CRC = %d %d \n", fileSize, fileSize_CRC);
        return false;
    }
        
        
    if (SYS_FS_FileRead(g_sqiHandle, buffer, fileSize_CRC) == -1)
    {
        /* There was an error while reading the file. Close the file
         * and error out. */
        SYS_PRINT("\n There was an error while reading the file \n");
        SYS_FS_FileClose(g_sqiHandle);

        return false;
    }


    unsigned short temp;

    temp = crc_crc16ccitt(CRC16_START_VAL, fileSize_CRC, buffer);
    if (temp == 0)
    {
//        SYS_PRINT("\n crc image good ");
    }
    else
    {
        SYS_PRINT("\n crc image failure %d \n", temp);
        SYS_FS_FileClose(g_sqiHandle);
        return false;
    }


    fsRes = SYS_FS_FileClose(g_sqiHandle);

    if (fsRes != SYS_FS_RES_SUCCESS)
    {
        SYS_PRINT("\n SYS_FS_FileClose err %d \n", SYS_FS_Error());
        res = false;
    }
    else
    {
//        SYS_PRINT("\n SQIInterface_ReadAndCheckCRC good");
        res = true;
    }

    return res;
}

int SQIInterface_CheckFileOnSQIFlashAtIndex(int i)
{
    g_sqiHandle = SYS_FS_FileOpen(g_fileList[i].fileName, SYS_FS_FILE_OPEN_READ);

    if (g_sqiHandle == SYS_FS_HANDLE_INVALID)
    {
        SYS_PRINT("Failed to open %s \n", g_fileList[i].fileName);
        SYS_FS_FileClose(g_sqiHandle);
        LogInterface_WriteDebugLogFile("SQIInterface_CheckFileOnSQIFlash Failed to open %s \n", g_fileList[i].fileName );
        return eGuiUpdateScreenMessageFileNotFound;
    }
    else
    {
        if (SQIInterface_ReadAndCheckCRC(clone_buffer, g_fileList[i].fileSize) == false)
        {
            SYS_PRINT("CRC check failed %s \n", g_fileList[i].fileName);
            LogInterface_WriteDebugLogFile("SQIInterface_CheckFileOnSQIFlash failed CRC at index %d \n", i);
            return eGuiUpdateScreenMessageFileInvalid;
        }
        else
        {
            if (g_fileList[i].id == eIntroVideoAssetId)
            {
                introVideoInputData[(uint32_t)g_fileList[i].data] = (uint8_t*)mm_malloc(g_fileList[i].fileSize);
                memcpy(introVideoInputData[(uint32_t)g_fileList[i].data], clone_buffer, g_fileList[i].fileSize);
            }
            else if (g_fileList[i].id == eAlarmVideoAssetId)
            {
                alarmVideoInputData[(uint32_t)g_fileList[i].data] = (uint8_t*)mm_malloc(g_fileList[i].fileSize);
                memcpy(alarmVideoInputData[(uint32_t)g_fileList[i].data], clone_buffer, g_fileList[i].fileSize);
            }
            else if (g_fileList[i].id == eAudioLow260msAssetId)
            {
                memcpy(audioSquareWave260ms_Low, clone_buffer, g_fileList[i].fileSize);
            }
            else if (g_fileList[i].id == eAudioMedium260msAssetId)
            {
                memcpy(audioSquareWave260ms_Medium, clone_buffer, g_fileList[i].fileSize);
            }
            else if (g_fileList[i].id == eAudioHigh210msAssetId)
            {
                memcpy(audioSquareWave210ms_High, clone_buffer, g_fileList[i].fileSize);
            }
            else
            {
                memcpy(g_fileList[i].data, clone_buffer, g_fileList[i].fileSize);
            }
        }
    }
    return 0;
}

/** @brief Does the sqi flash check have the file yet
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SQIInterface_CheckFileOnSQIFlash(void)
{
    int i;
    int ret = 0;
    if (g_isMountSQI == true)
    {
        if (g_sqiHandle == NULL)
        {            
            for (i = 0; i < NUMBER_FILE_IMAGE_FONT; i++)
            {
                ret = SQIInterface_CheckFileOnSQIFlashAtIndex(i);
                
                //double check in case CRC failed at first
                if (ret == eGuiUpdateScreenMessageFileInvalid )
                {                   
                    ret = SQIInterface_CheckFileOnSQIFlashAtIndex(i);
                    if (ret == eGuiUpdateScreenMessageFileInvalid )
                    {
                        break;
                    }
                    break; //FIXME : this to test if double check CRC work
                }
                else if (ret == eGuiUpdateScreenMessageFileNotFound)
                {
                    break;
                }
                else
                {
                    
                }
            }
            if ( ret == 0 )
            {
                guiInterface_SendEvent(eGuiSQIisNotEmptyId, 1);
            }
            else
            {
                SYS_PRINT("SQIInterface_CheckFileOnSQIFlash failed \n");
                uint32_t totalSector, freeSector;
                if (SYS_FS_DriveSectorGet("mnt/SQIFlash", &totalSector, &freeSector) != SYS_FS_RES_SUCCESS){
                    SYS_PRINT("SYS_FS_DriveSectorGet Failed \n");
                }
                else{
                    SYS_PRINT("totalSector = %u, freeSector = %u \n", totalSector, freeSector);
                }
                guiInterface_SendEvent(eGuiSQIisEmptyId, ret);
            }            
        }
    }
    return;
}

/**
 * @brief Error handler for file copy
 * @param [in] fileHandle Handle of current file, fileHandle if not null
 *             will be closed at the end of this process.
 * @param [in] errType Error type
 * @return None
 */
void SQIInterface_CopyFileErrorHandler(SYS_FS_HANDLE fileHandle, FCPY_ERROR_t errType)
{
    SYS_FS_ERROR err;
    uint8_t fileName[MAX_FILE_NAME];
    uint32_t fileSize;

    switch (errType)
    {
    case FCPY_SYS_FS_ERROR:
        /// Two type of file system error
        if (fileHandle != NULL) err = SYS_FS_FileError(fileHandle);
        else err = SYS_FS_Error();
        /// Print error message to console
        SYS_PRINT("File system error. SYS_FS_FileError Code: %d\n", SYS_FS_FileError(fileHandle));
        SYS_PRINT("File system error. SYS_FS_Error Code: %d\n", SYS_FS_Error());
        break;
    case FCPY_SOURCE_FILE_ERROR:
        // This error occur when source file is empty
        SYS_FS_FileNameGet(fileHandle, fileName, MAX_FILE_NAME);
        SYS_PRINT("Source file: \"%s\" error!\n", fileName);
        break;
    case FCPY_FILE_DATA_MISMATCH:
        // This error occur when validate file after write error
        // Data read and data write mismatch
        SYS_PRINT("Read write data mismatch\n");
        break;
    case FCPY_FILE_SIZE_INTERNAL_ERROR:
        // This error occur when actual size of file and file size read mismatch,
        // It could happen when reading file / writing to file incomplete
        SYS_PRINT("File size error, internal error\n");
        break;
    case FCPY_BUFFER_SIZE_INTERNAL_ERROR:
        // Actual size of file is greater than max buffer size!
        SYS_FS_FileNameGet(fileHandle, fileName, MAX_FILE_NAME);
        fileSize = SYS_FS_FileSize(fileHandle);
        SYS_PRINT("Buffer cache is not enough for \"%s\" \n", fileName);
        SYS_PRINT("Actual file size is: %d", fileSize);
        break;
    default:
        break;
    }

    if (fileHandle != NULL)
    {
        SYS_FS_FileClose(fileHandle);
    }

    return;
}

/**
 * @brief Single file copy from SD-Card to SQI memory
 * @param [in] readFileName Name of file desired to copy
 * @param [in] readFilePath Path of file 
 * @param [in] writeFileName Name of file desired to be placed
 * @param [out] None
 * @retval FILECOPY_SUCCESS File is success copied
 * @retval FILECOPY_ERROR   An error occur while copying file, some message will
 *                          be printed to console in error case.
 */

/* Note: This function assume the desired file was not existed in destination path
 *       Please check file existed before calling this function.
 *       In case of desired file is already existed, it will be overwritten.
 */
FILECOPYSTATUS_t SQIInterface_CopyFile(
                                       const char *readFileName, const char* readFilePath,
                                       const char *writeFileName, const char* writeFilePath)
{
//    clone_buffer = mm_malloc(MAX_FILE_DATA_BUFFER);
//    check_buffer = mm_malloc(MAX_FILE_DATA_BUFFER);
    
    
    int writeFileSize, readFileSize, fileSize;
    SYS_FS_HANDLE readFileHandle, writeFileHandle;

    /// Setting source path
    if (SYS_FS_RES_SUCCESS != SYS_FS_CurrentDriveSet(readFilePath))
    {
        SYS_PRINT("Setting source path \n");
        SQIInterface_CopyFileErrorHandler(NULL, FCPY_SYS_FS_ERROR);
        return FILECOPY_ERROR;
    }

    /// Open source file and clone data to buffer
    readFileHandle = SYS_FS_FileOpen(readFileName, SYS_FS_FILE_OPEN_READ);
    if (SYS_FS_HANDLE_INVALID == readFileHandle)
    {
        SYS_PRINT("Open source file and clone data to buffer \n");
        SQIInterface_CopyFileErrorHandler(readFileHandle, FCPY_SYS_FS_ERROR);
        return FILECOPY_ERROR;
    }

    /// Double check file size for ensuring data transfer size mismatch
    fileSize = SYS_FS_FileSize(readFileHandle);
    if (fileSize > MAX_FILE_DATA_BUFFER)
    {
        /// - If buffer prepare for cache data is not enough throw internal error
        SQIInterface_CopyFileErrorHandler(readFileHandle, FCPY_BUFFER_SIZE_INTERNAL_ERROR);
        return FILECOPY_ERROR;
    }
    else if (fileSize <= 0)
    {
        /// - If source file error, print message to console and return error
        SQIInterface_CopyFileErrorHandler(readFileHandle, FCPY_SOURCE_FILE_ERROR);
        return FILECOPY_ERROR;
    }
    else
    {
        /// - If source file contain data, start cloning to buffer
        readFileSize = SYS_FS_FileRead(readFileHandle,
                                       clone_buffer, sizeof (clone_buffer));
        if (fileSize != readFileSize)
        {
            SQIInterface_CopyFileErrorHandler(readFileHandle, FCPY_FILE_SIZE_INTERNAL_ERROR);
            return FILECOPY_ERROR;
        }
    }
    /// Clone data done, close source file
    SYS_FS_FileClose(readFileHandle);

    /*-----------------------------------------------------------------------*/
    /// Next, open desired file in destination
    if (SYS_FS_RES_SUCCESS != SYS_FS_CurrentDriveSet(writeFilePath))
    {
        SYS_PRINT("Next, open desired file in destination \n");
        SQIInterface_CopyFileErrorHandler(NULL, FCPY_SYS_FS_ERROR);
        return FILECOPY_ERROR;
    }
    writeFileHandle = SYS_FS_FileOpen(writeFileName, SYS_FS_FILE_OPEN_WRITE);
    if (SYS_FS_HANDLE_INVALID == writeFileHandle)
    {
        SYS_PRINT("write desired file in destination \n");
        SQIInterface_CopyFileErrorHandler(writeFileHandle, FCPY_SYS_FS_ERROR);
        return FILECOPY_ERROR;
    }
    /// Write data to file, with number of bytes read
    writeFileSize = SYS_FS_FileWrite(writeFileHandle, clone_buffer, readFileSize);
    if (writeFileSize <= 0)
    {
        SQIInterface_CopyFileErrorHandler(writeFileHandle, FCPY_FILE_SIZE_INTERNAL_ERROR);
        return FILECOPY_ERROR;
    }

    /// Sync file
    if (SYS_FS_RES_SUCCESS != SYS_FS_FileSync(writeFileHandle))
    {
        // Cannot sync file
        SYS_PRINT("Cannot sync file \n");
        SQIInterface_CopyFileErrorHandler(writeFileHandle, FCPY_SYS_FS_ERROR);
        return FILECOPY_ERROR;
    }
    /// Close write file
    SYS_FS_FileClose(writeFileHandle);

    /*-----------------------------------------------------------------------*/
    /// Validate data copied
    /// - Open recent written file
    readFileHandle = SYS_FS_FileOpen(writeFileName, SYS_FS_FILE_OPEN_READ);
    if (SYS_FS_HANDLE_INVALID == readFileHandle)
    {
        SYS_PRINT("Open recent written file \n");
        SQIInterface_CopyFileErrorHandler(readFileHandle, FCPY_SYS_FS_ERROR);
        return FILECOPY_ERROR;
    }
    /// - Clone data to check buffer
    readFileSize = SYS_FS_FileRead(readFileHandle, check_buffer, sizeof (check_buffer));
    if (readFileSize != writeFileSize)
    {
        SQIInterface_CopyFileErrorHandler(readFileHandle, FCPY_FILE_SIZE_INTERNAL_ERROR);
        return FILECOPY_ERROR;
    }
    /// - Compare data read and written
    if (memcmp(clone_buffer, check_buffer, readFileSize) != 0)
    {
        /// - If data read write mismatch, a require recovery signal return
        SQIInterface_CopyFileErrorHandler(readFileHandle, FCPY_FILE_DATA_MISMATCH);
        return FILECOPY_REQUIRE_RECOVERY;
    }
    /// - Close file validated
    SYS_FS_FileClose(readFileHandle);

    
//    mm_free(clone_buffer);
//    mm_free(check_buffer);
    
    return FILECOPY_SUCCESS;
}

/** @brief Function Format SQI flash
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void SQIInterface_Format(void)
{
    if (SYS_FS_DriveFormat(SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0, SYS_FS_FORMAT_FDISK, 0) != SYS_FS_RES_SUCCESS)
    {
        // Failure, try mounting again
                SYS_PRINT("\n Failure, try mounting again\n");
    }
    else
    {
        // Mount was successful. Format now.
                SYS_PRINT("\n Format SQI successful\n");
    }

    return;
}

GFX_Result SQIInterface_externalMediaOpen(GFXU_AssetHeader* ast)
{    
    return GFX_SUCCESS;
}

GFX_Result SQIInterface_externalMediaRead(GFXU_ExternalAssetReader* reader,
                                 GFXU_AssetHeader* ast,
                                 void* address,
                                 uint32_t readSize,
                                 uint8_t* destBuffer,
                                 GFXU_MediaReadRequestCallback_FnPtr cb)
{   
    if(ast->dataLocation == GFXU_ASSET_LOCATION_ID_SQIFlash_Images)
    {
//        SYS_PRINT("\n ID_SQIFlash (uint32_t)address = %u, readSize = %u", (uint32_t)address, readSize);
        memcpy(destBuffer, g_graphicImage + (uint32_t)address, readSize);
        if(reader != NULL && cb != NULL)
            cb(reader); // indicate that the data buffer is ready        
        return GFX_SUCCESS;
    }
    
    if(ast->dataLocation == GFXU_ASSET_LOCATION_ID_SQIFlash_AbelFont)
    {
//        SYS_PRINT("\n GFXU_ASSET_LOCATION_ID_SQIFlash_Font (uint32_t)address = %u", (uint32_t)address);
        memcpy(destBuffer, g_graphicAbelFont + (uint32_t)address, readSize);
        if(reader != NULL && cb != NULL)
            cb(reader); // indicate that the data buffer is ready        
        return GFX_SUCCESS;
    }
    
    if(ast->dataLocation == GFXU_ASSET_LOCATION_ID_SQIFlash_BebasFont)
    {
//        SYS_PRINT("\n GFXU_ASSET_LOCATION_ID_SQIFlash_Font (uint32_t)address = %u", (uint32_t)address);
        memcpy(destBuffer, g_graphicBebasFont + (uint32_t)address, readSize);
        
        if(reader != NULL && cb != NULL)
            cb(reader); // indicate that the data buffer is ready        
        
        return GFX_SUCCESS;
    }    
        
    return GFX_FAILURE; // failure tells the decoder to abort and move on
}

void SQIInterface_externalMediaClose(GFXU_AssetHeader* ast)
{
    return;
}
/* end of file */