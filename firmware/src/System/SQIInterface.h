/** @file SQIInterface.h
 *  @brief Contains functions for interface with SQI flash
 *  @author Viet Le
 */
#ifndef SQIINTERFACE_H
#define	SQIINTERFACE_H

#include "gfx/libaria/libaria_harmony.h"
#include "gfx/libaria/libaria_init.h"

#include "gfx/libaria/libaria.h"

#include "gfx/libaria/inc/libaria_context_rtos.h"
#include "gfx/libaria/inc/libaria_input_rtos.h"
#include "gfx/libaria/libaria_rtos.h"

extern SYS_FS_HANDLE graphicBinFile;

extern SYS_FS_HANDLE s_alarmImageHandle;

/** @brief define file copy status */
typedef enum {
    FILECOPY_ERROR = -1, /**<error */
    FILECOPY_SUCCESS = 0, /**<success */
    FILECOPY_REQUIRE_RECOVERY /**<require recovery */
} FILECOPYSTATUS_t;

/** @brief define file copy error struct */
typedef enum {
    FCPY_SYS_FS_ERROR,              /** File system */
    FCPY_SOURCE_FILE_ERROR,         /** Source file empty */
    FCPY_FILE_DATA_MISMATCH,        /** File read write file size mismatch*/
    FCPY_FILE_SIZE_INTERNAL_ERROR,  /** Internal processing file size mismatch */
    FCPY_BUFFER_SIZE_INTERNAL_ERROR,/** Actual file size was too large */
} FCPY_ERROR_t;


//Mount SQI flash
void SQIInterface_Mount(void);

//Check file on SQI flash at index in list file
int SQIInterface_CheckFileOnSQIFlashAtIndex(int i);

//Check file on SQI flash
void SQIInterface_CheckFileOnSQIFlash(void);

//Single file copy from SD-Card to SQI memory
FILECOPYSTATUS_t SQIInterface_CopyFile(
    const char *readFileName, const char* readFilePath,
    const char *writeFileName, const char* writeFilePath);

//Format SQI flash
void SQIInterface_Format(void);

GFX_Result SQIInterface_externalMediaOpen(GFXU_AssetHeader* ast);

GFX_Result SQIInterface_externalMediaRead(GFXU_ExternalAssetReader* reader,
                                 GFXU_AssetHeader* ast,
                                 void* address,
                                 uint32_t readSize,
                                 uint8_t* destBuffer,
                                 GFXU_MediaReadRequestCallback_FnPtr cb);


void SQIInterface_externalMediaClose(GFXU_AssetHeader* ast);

#endif	/* SQIINTERFACE_H */

/* end of file */