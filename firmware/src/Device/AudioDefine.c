/* D:\Portable\NewRequests\20210323_AW-110_8th_Soft Specification\WAVE\20210322_Medium_Synthetic_800Hz_260ms.WAV (21-Mar-21 6:40:08 PM)
   StartOffset(h): 00000000, EndOffset(h): 000059F1, Length(h): 000059F2 */

#include "../Gui/GuiDefine.h"
#include "AudioDefine.h"

uint8_t audioSquareWave260ms_Low[SIZE_SOUND_800HZ_260MS_LOW] __attribute__((section(".ddr_data"), space(prog)));
uint8_t audioSquareWave260ms_Medium[SIZE_SOUND_800HZ_260MS_MEDIUM] __attribute__((section(".ddr_data"), space(prog)));
uint8_t audioSquareWave210ms_High[SIZE_SOUND_800HZ_210MS_HIGH] __attribute__((section(".ddr_data"), space(prog)));

void printValue(void)
{
    SYS_PRINT("\n HIGH: 4 - [%d] | 5 - [%d]", audioSquareWave210ms_High[4], audioSquareWave210ms_High[5]);
    SYS_PRINT("\n LOW: 4 - [%d] | 5 - [%d]", audioSquareWave260ms_Low[4], audioSquareWave260ms_Low[5]);
}