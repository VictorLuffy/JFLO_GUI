/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _FILE_SYSTEM_MGR_H    /* Guard against multiple inclusion */
#define _FILE_SYSTEM_MGR_H



/* This section lists the other files that are included in this file.
 */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    void FileSystemMgr_Initialize(void);

   void FileSystemMgr_Task();

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _FILE_SYSTEM_MGR_H */

/* *****************************************************************************
 End of File
 */
