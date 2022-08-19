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

#ifndef _RCFILTER_H    /* Guard against multiple inclusion */
#define _RCFILTER_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
typedef struct {
    float clpf;
    float fltv[2];
}RCflt_t;

void Lpf_Init(RCflt_t *lpf, float initValue);
float LPF(RCflt_t *lpf, float md);
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _RCFILTER_H */

/* *****************************************************************************
 End of File
 */
