/* 
 * File:   AudioDefine.h
 * Author: METRAN1
 *
 * Created on July 28, 2022, 8:52 AM
 */

#ifndef AUDIODEFINE_H
#define	AUDIODEFINE_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#ifndef UNIT_TEST
#include "system_config.h"
#include "system_definitions.h"
#include "gfx/hal/inc/gfx_context.h"
#endif

#include "mm.h"
#include "System/USBInterface.h"
    
/** @brief Size of 20210322_Low_Synthetic_800Hz_260ms.bin */
#define     SIZE_SOUND_800HZ_260MS_LOW                  (23026)

/** @brief Size of 20210322_Medium_Synthetic_800Hz_260ms.bin */
#define     SIZE_SOUND_800HZ_260MS_MEDIUM                  (23026)

/** @brief Size of 20210322_High_Synthetic_800Hz_210ms */
#define     SIZE_SOUND_800HZ_210MS_HIGH                  (18616)

/** @brief Total audio file*/
#define     AUDIO_TOTAL_FILE    (3)

/** @brief file name of sound 260ms low*/
#define     FILE_SOUND_800HZ_260MS_LOW          "Audio_Low_260ms_CRC.bin"
    
/** @brief file name of sound 260ms Medium*/
#define     FILE_SOUND_800HZ_260MS_MEDIUM       "Audio_Medium_260ms_CRC.bin"
    
/** @brief file name of sound 210ms High*/
#define     FILE_SOUND_800HZ_210MS_HIGH         "Audio_High_210ms_CRC.bin"

// Storage of Audio file
extern uint8_t audioSquareWave260ms_Low[];
extern uint8_t audioSquareWave260ms_Medium[];
extern uint8_t audioSquareWave210ms_High[];

void printValue(void);
#ifdef	__cplusplus
}
#endif

#endif	/* AUDIODEFINE_H */

