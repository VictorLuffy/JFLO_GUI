/** @file AlarmScreen.c
 *  @brief The screen provides graphic interface to update firmware
 *  @author Thanh Hiep Nguyen
 */

#ifndef _ALARMSCREEN_CONTROL
#define _ALARMSCREEN_CONTROL

/* This section lists the other files that are included in this file.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "system_config.h"
#include "system_definitions.h"
#include "gfx/hal/inc/gfx_context.h"

#include "Gui/DisplayControl.h"

#include "mm.h"

//#ifdef __cplusplus  // Provide C++ Compatibility
//extern "C" {
//#endif
    
// Section: Global Data Definitions
#define FRAMES_PER_SEC                      8 //10
#define FRAME_RATE_MS                       1000/FRAMES_PER_SEC
#define FRAMESTEP_INC                       1
#define VIDEO_LAYER_ID                      0
#define BYTES_PER_PIXEL                     2
typedef struct {
    int state;
    
    //Playback Controls
    int frameIndex;
    int frameNum;
    int fps;
    laLabelWidget* fpsLabel;
    laLabelWidget* bandwidthLabel;
} AlarmScreen_Control_Struct;

/** @brief Initialize AlarmScreen's element: Check all AlarmScreen's widgets have been created yet,
 * init control & data variable and change some properties in needed initialization widgets
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
E_GuiResult AlarmScreen_Initialize(void);

E_GuiResult AlarmScreen_ReInit(void);

/** @brief Implement AlarmScreen's state, animation function and some action periodlly
 *  It was called by DisplayControl.
 *  @param [in]  None
 *  @param [out]  None
 *  @return None
 */
void AlarmScreen_Run(void);


void AlarmScreen_createImagePath(uint8_t alarm);


uint8_t AlarmScreen_GetFrameIndex(void);


uint8_t AlarmScreen_GetFrameNumber(void);

#endif	

// end of file