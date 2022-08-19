/** @file USBInterface.h
 *  @brief Contains functions for interface with USB
 *  @author Viet Le
 */

#ifndef USBINTERFACE_H
#define	USBINTERFACE_H

#include "stddef.h"
#include "system/fs/sys_fs.h"

//Set file name
void USBInterface_SetFileName(const char* fileName);

//Write to USB
void USBInterface_Write(void* data, size_t size);

//Read to USB
void USBInterface_Read(void* buff, size_t size);

//Move file pointer
void USBInterface_Seek(int32_t offset, SYS_FS_FILE_SEEK_CONTROL whence);

//Flushes the cached information when writing to a file
void USBInterface_FileSync(void);

//Search file in USB
SYS_FS_RESULT USBInterface_Search(const char * path , const char * fileName);

#endif	/* USBINTERFACE_H */

/* end of file */