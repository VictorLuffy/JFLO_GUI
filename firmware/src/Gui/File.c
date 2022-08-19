/** @file File.c
 *  @brief File provides function to set and get setting, log from the file 
 *  @author Viet Le
 */

#include "File.h"
#include <stddef.h>
#include "system_config.h"
#include "system/debug/sys_debug.h"
//#include "Setting.h"

/** @brief Declare setting file */
SYS_FS_HANDLE g_settingFile = SYS_FS_HANDLE_INVALID;

/** @brief Declare log file */
SYS_FS_HANDLE g_eventLogFile = SYS_FS_HANDLE_INVALID;

/** @brief Declare alarm file */
SYS_FS_HANDLE g_alarmLogFile = SYS_FS_HANDLE_INVALID;

/** @brief Declare log file */
SYS_FS_HANDLE g_devInfoFile = SYS_FS_HANDLE_INVALID;

/** @brief Declare log file */
SYS_FS_HANDLE g_Spo2DataFile = SYS_FS_HANDLE_INVALID;

bool g_InitializedSQIFiles = false;

/** @brief Function Init and open file
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void file_OpenFileOnSQIFlash(void)
{
    //Setting source path
    if (SYS_FS_RES_SUCCESS != SYS_FS_CurrentDriveSet(SYS_FS_MEDIA_IDX0_MOUNT_NAME_VOLUME_IDX0))
    {
        SYS_PRINT("\n SYS_FS_CurrentDriveSet file_Init");
        return;
    }

    //Open file
    g_settingFile = SYS_FS_FileOpen(FILE_SETTING_NAME, SYS_FS_FILE_OPEN_APPEND_PLUS);
    g_eventLogFile = SYS_FS_FileOpen(FILE_EVENTLOG_NAME, SYS_FS_FILE_OPEN_APPEND_PLUS);
    g_alarmLogFile = SYS_FS_FileOpen(FILE_ALARMLOG_NAME, SYS_FS_FILE_OPEN_APPEND_PLUS);
    g_devInfoFile = SYS_FS_FileOpen(FILE_DEVICE_INFORMATION, SYS_FS_FILE_OPEN_APPEND_PLUS);
    g_Spo2DataFile = SYS_FS_FileOpen(FILE_SPO2DATA_NAME, SYS_FS_FILE_OPEN_APPEND_PLUS);
    
    if ((g_settingFile == SYS_FS_HANDLE_INVALID) 
        || (g_eventLogFile == SYS_FS_HANDLE_INVALID)
        || (g_devInfoFile == SYS_FS_HANDLE_INVALID) 
        || (g_Spo2DataFile == SYS_FS_HANDLE_INVALID) 
        || (g_alarmLogFile == SYS_FS_HANDLE_INVALID))
    {
        SYS_PRINT("\n open file failure \n");
        return;
    }
    else
    {
        g_InitializedSQIFiles = true;
    }
    
    return;
}

/** @brief This function will close all files opening on the SQI
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void file_CloseFileOnSQIFlash(void)
{
    if (!g_InitializedSQIFiles)
        return;
    //Open file
    SYS_FS_FileClose(g_settingFile);
    SYS_FS_FileClose(g_eventLogFile);
    SYS_FS_FileClose(g_alarmLogFile);
    SYS_FS_FileClose(g_devInfoFile);
    SYS_FS_FileClose(g_Spo2DataFile);

    return;
}

/** @brief Function Write to file
 *  @param [in]  void* data : data write
 *  @param [in]  size_t size : size of data
 *  @param [in]  SYS_FS_HANDLE fileHandle : file handle 
 *  @param [out]  None
 *  @return None
 */
void file_Write(void* data, size_t size, SYS_FS_HANDLE fileHandle)
{  
    if (SYS_FS_FileWrite(fileHandle, data, size) == -1)
    {
        /* Failed to write to the file. */
        SYS_PRINT("\n Failed to write to the file \n");
        SYS_FS_FileClose(fileHandle);
        return;
    }

    if (SYS_FS_FileSync(fileHandle) != 0)
    {
        /* Could not flush the contents of the file. Error out. */
        SYS_PRINT("\n Could not flush the contents of the file. Error out. \n");
        SYS_FS_FileClose(fileHandle);

        return;
    }

//    SYS_MESSAGE("\n write success  \n");
    
    return;
}

/** @brief Function Read from file
 *  @param [in]  void* data : data read   
 *  @param [in]  size_t size : size of data  
 *  @param [in]  SYS_FS_HANDLE fileHandle : file handle  
 *  @param [out]  None
 *  @return None
 */
void file_Read(void* data, size_t size, SYS_FS_HANDLE fileHandle)
{
    long fileSize = 0;

    fileSize = SYS_FS_FileSize(fileHandle);

    if (fileSize == -1)
    {
        /* Reading file size was a failure */
        SYS_PRINT("\n Reading file size was a failure *****\n");
        SYS_FS_FileClose(fileHandle);
        return;
    }

    /* Move file pointer to begining of file */
    if (SYS_FS_FileSeek(fileHandle, 0, SYS_FS_SEEK_SET) == -1)
    {
        /* File seek caused an error */
        SYS_PRINT("\n File seek caused an error \n");
        SYS_FS_FileClose(fileHandle);
        return;
    }

    /* Check for original file content */
//    SYS_PRINT("\n Check for original file content \n");

    if (SYS_FS_FileRead(fileHandle, data, size) == -1)
    {
        /* There was an error while reading the file. Close the file
         * and error out. */
        SYS_PRINT("\n There was an error while reading the file \n");
        SYS_FS_FileClose(fileHandle);
        return;
    }

//    SYS_MESSAGE("\n read success \n");

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
 *  @param [in]  SYS_FS_HANDLE fileHandle : file handle   
 *  @param [in]  int32_t offset : offset 
 *  @param [in]  SYS_FS_FILE_SEEK_CONTROL whence : whence
 *  @param [out]  None
 *  @return None
 */
void file_Seek(SYS_FS_HANDLE fileHandle, int32_t offset, SYS_FS_FILE_SEEK_CONTROL whence)
{
    if (SYS_FS_FileSeek(fileHandle, offset, whence) == -1)
    {
        /* File seek caused an error */
        SYS_PRINT("\n File seek caused an error %d\n", SYS_FS_Error());

        return;
    }
}

/** @brief Get file size
 *  @param [in]  SYS_FS_HANDLE fileHandle : file handle       
 *  @param [out]  None
 *  @return long: file size
 */
long file_Size(SYS_FS_HANDLE fileHandle)
{
    long fileSize = 0;

    fileSize = SYS_FS_FileSize(fileHandle);

    if (fileSize == -1)
    {
        /* Reading file size was a failure */
        SYS_PRINT("\n Reading file size was a failure *****\n");

        return fileSize;
    }

    return fileSize;
}

/** @brief Truncates a file
 *  @param [in]  SYS_FS_HANDLE fileHandle : file handle      
 *  @param [out]  None
 *  @return None
 */
void file_Truncates(SYS_FS_HANDLE fileHandle)
{
//    SYS_PRINT("file_Truncates");
    SYS_FS_RESULT res;

    //Move file pointer
    file_Seek(fileHandle, 0, SYS_FS_SEEK_SET);
    //Truncates file
    res = SYS_FS_FileTruncate(fileHandle);

    if (res == SYS_FS_RES_FAILURE)
    {
        SYS_PRINT("\n file_Truncates failure");
    }
    else
    {
//        SYS_PRINT("\n file_Truncates successes");
    }

    return;
}

/* end of file */

