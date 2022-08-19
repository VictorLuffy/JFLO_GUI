/* 
 * File:   PowerOffScreen.h
 * Author: METRAN1
 *
 * Created on July 20, 2022, 4:24 PM
 */

#ifndef POWEROFFSCREEN_H
#define	POWEROFFSCREEN_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdbool.h>
    
void PowerOffScreen_Run(void);
void PowerOffScreen_ReInitialize(void);
void PowerOffScreen_Initialize(void);
void PowerOffScreen_Distructor(void);
void PowerOffScreen_SetIndicatorVisible(bool flag);
void PowerOffScreen_CheckSetSound(void);
void PowerOffScreen_StopSound(void);
#ifdef	__cplusplus
}
#endif

#endif	/* POWEROFFSCREEN_H */

