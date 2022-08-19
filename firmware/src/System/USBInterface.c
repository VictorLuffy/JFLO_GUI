/** @file USBInterface.c
 *  @brief Contains functions for interface with USB
 *  @author Viet Le
 */

#include "USBInterface.h"
#include "system_config.h"
#include "system_definitions.h"
#include "system/rtcc/sys_rtcc_pic32m.h"
#include "GuiDefine.h"

/** @brief Define max file name */
#define MAX_FILENAME    50

/** @brief Define max data length */
#define MAX_DATA        18

/** @brief Declare file */
static SYS_FS_HANDLE s_fileHandle;

/** @brief Declare file name*/
static char s_fileName[MAX_FILENAME];

//Set file name

/** @brief Function set file name write to USB
 *  @param [in]  const char* fileName: file name
 *  @param [out]  None
 *  @return None
 */
void USBInterface_SetFileName(const char* fileName)
{
    memset(s_fileName, 0, MAX_FILENAME);
    strncpy(s_fileName, fileName, strlen(fileName));

//    SYS_PRINT("\n fileName = %s", fileName);

    //Save log to sdcard
//    if (SYS_FS_CurrentDriveSet(SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0) == SYS_FS_RES_FAILURE)
//    {
//        /* Error while setting current drive */
//        SYS_MESSAGE("\n Error while setting current drive \n");
//        return;
//    }

    char filePath[255];
    strcpy(filePath, SYS_FS_MEDIA_IDX1_MOUNT_NAME_VOLUME_IDX0);
    strcat(filePath, "/");
    strcat(filePath, s_fileName);
    
//    s_fileHandle = SYS_FS_FileOpen((const char*) &s_fileName[0], SYS_FS_FILE_OPEN_APPEND);
    s_fileHandle = SYS_FS_FileOpen(filePath, SYS_FS_FILE_OPEN_WRITE);

    if (s_fileHandle == SYS_FS_HANDLE_INVALID)
    {
        SYS_PRINT("\n open file USB failure %s - %d \n", filePath, SYS_FS_Error());
        return;
    }

    return;
}

/** @brief Function write to USB
 *  @param [in]  void* data: data write
 *  @param [in]  size_t size: data size
 *  @param [out]  None
 *  @return None
 */
void USBInterface_Write(void* data, size_t size)
{
    if (SYS_FS_FileWrite(s_fileHandle, data, size) == -1)
    {
        /* Failed to write to the file. */
        SYS_PRINT("\n Failed to write to the file %d \n", SYS_FS_Error());
        SYS_FS_FileClose(s_fileHandle);

        return;
    }

    return;
}

/** @brief Function write to USB
 *  @param [in]  void* data: data write
 *  @param [in]  size_t size: data size
 *  @param [out]  None
 *  @return None
 */
void USBInterface_Read(void* buff, size_t size)
{
    if (SYS_FS_FileRead(s_fileHandle, buff, size) == -1)
    {
        /* Failed to write to the file. */
        SYS_PRINT("\n Failed to read file \n");
        SYS_FS_FileClose(s_fileHandle);
        return;
    }

    return;
}

/** @brief This function sets the file pointer for a open file associated with the
 *   file handle, as follows:
      whence = SYS_FS_SEEK_SET - File offset is set to offset bytes from the
                                 beginning.
      whence = SYS_FS_SEEK_CUR - File offset is set to its current location
                                 plus offset.
      whence = SYS_FS_SEEK_END - File offset is set to the size of the file
                                 plus offset. The offset specified for this
                                 option should be negative for the file pointer
                                 to be valid.

    Trying to move the file pointer using SYS_FS_FileSeek, beyond the range of
    file will only cause the pointer to be moved to the last location of the
    file
 *  @param [in]  int32_t offset : offset 
 *  @param [in]  SYS_FS_FILE_SEEK_CONTROL whence : whence
 *  @param [out]  None
 *  @return None
 */
void USBInterface_Seek(int32_t offset, SYS_FS_FILE_SEEK_CONTROL whence)
{
    if (SYS_FS_FileSeek(s_fileHandle, offset, whence) == -1)
    {
        /* File seek caused an error */
        SYS_PRINT("\n File seek caused an error \n");
        return;
    }

    return;
}

/** @brief Flushes the cached information when writing to a file
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void USBInterface_FileSync(void)
{
    if (SYS_FS_FileSync(s_fileHandle) != 0)
    {
        /* Could not flush the contents of the file. Error out. */
        SYS_MESSAGE("\n Could not flush the contents of the file. Error out. \n");
        SYS_FS_FileClose(s_fileHandle);

        return;
    }
    else
    {
        SYS_FS_FileClose(s_fileHandle);
    }

    return;
}

/** @brief Search file in USB
 *  @param [in] const char * path : path
 *  @param [in] const char * fileName : file name
 *  @param [out] None
 *  @return None
 */
SYS_FS_RESULT USBInterface_Search(const char * path, const char * fileName)
{
    SYS_FS_HANDLE dirHandle;
    SYS_FS_FSTAT stat;
    char longFileName[MAX_FILENAME];
    SYS_FS_RESULT res;

    // If long file name is used, the following elements of the "stat"
    // structure needs to be initialized with address of proper buffer.
    stat.lfname = longFileName;
    stat.lfsize = MAX_FILENAME;

    dirHandle = SYS_FS_DirOpen(path);

    if (dirHandle != SYS_FS_HANDLE_INVALID)
    {
        // Directory open is successful

        if (SYS_FS_DirSearch(dirHandle, fileName, SYS_FS_ATTR_ARC, &stat) == SYS_FS_RES_SUCCESS)
        {
            // Specified file not found
            res = SYS_FS_RES_SUCCESS;
        }
        else
        {
            // File found. Read the complete file name from "stat.lfname" and
            // other file parameters from the "stat" structure
            res = SYS_FS_RES_FAILURE;
        }

        SYS_FS_DirClose(dirHandle);
    }
    else
    {
        res = SYS_FS_RES_FAILURE;
    }
    
    return res;
}

void USBInterface_CloseFile(void)
{
    SYS_FS_FileClose(s_fileHandle);
}

SYS_FS_RESULT USBInterface_CreateDir(const char* path, const char* name)
{
    char dirPath[255];
    sprintf(dirPath, "%s/%s", path, name);

    SYS_FS_RESULT res = SYS_FS_DirectoryMake(dirPath);
    if(res == SYS_FS_RES_FAILURE)
    {
        // already exist
        if (SYS_FS_Error() == SYS_FS_ERROR_EXIST)
            return SYS_FS_RES_SUCCESS;

        SYS_PRINT("\n SYS_FS_DirectoryMake %d \n", SYS_FS_Error());
        return SYS_FS_RES_FAILURE;
    }
    
    return SYS_FS_RES_SUCCESS;
}

/* end of file */
