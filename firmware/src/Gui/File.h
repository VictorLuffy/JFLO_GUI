/** @file File.h
 *  @brief File provides function to set and get setting, log from the file 
 *  @author Viet Le
 */
#ifndef FILE_H
#define	FILE_H

#include "stddef.h"

#ifndef UNIT_TEST
#include "system/fs/sys_fs.h"
#endif

#define         FILE_DEVICE_INFORMATION     "deviceInfo.bin"
#define         FILE_SETTING_NAME           "setting.bin"
#define         FILE_EVENTLOG_NAME          "eventfile.bin"
#define         FILE_ALARMLOG_NAME          "alarmfile.bin"
#define         FILE_SPO2DATA_NAME          "SpO2Data.bin"

//Init and open file
void file_OpenFileOnSQIFlash(void);

//Close file
void file_CloseFileOnSQIFlash(void);

//Write to file
void file_Write(void* data, size_t size, SYS_FS_HANDLE fileHandle);

//Read from file
void file_Read(void* data, size_t size, SYS_FS_HANDLE fileHandle);

//Move file pointer
void file_Seek(SYS_FS_HANDLE fileHandle, int32_t offset, SYS_FS_FILE_SEEK_CONTROL whence);

//Get file size
long file_Size(SYS_FS_HANDLE fileHandle);

//Truncates a file
void file_Truncates(SYS_FS_HANDLE fileHandle);

#endif	/* FILE_H */

/* end of file */
