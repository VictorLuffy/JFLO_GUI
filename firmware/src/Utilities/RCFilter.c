/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */



/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


#include "RCFilter.h"    


void Lpf_Init(RCflt_t *lpf, float initValue)
{
    lpf->clpf = 0.8;
    lpf->fltv[0] = initValue;
    lpf->fltv[1] = initValue;
}


float LPF(RCflt_t *lpf, float md)
{
      lpf->fltv[1] = lpf->clpf * lpf->fltv[0] + (1 - lpf->clpf) * md;
      lpf->fltv[0] = lpf->fltv[1];
      return lpf->fltv[0];
}