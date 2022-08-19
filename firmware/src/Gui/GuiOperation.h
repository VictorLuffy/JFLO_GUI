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

#ifndef _GUI_OPERATION_H    /* Guard against multiple inclusion */
#define _GUI_OPERATION_H



/* This section lists the other files that are included in this file.
 */



/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

void GUI_Initialize(void) ;

void GUI_Task(void) ;

void GUI_Prepare();

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _GUI_OPERATION_H */

/* *****************************************************************************
 End of File
 */
