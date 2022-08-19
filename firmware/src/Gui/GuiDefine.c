/** @file GuiDefine.c
 *  @brief Contains define of GUI
 *  @author Viet Le
 */

/* This section lists the other files that are included in this file.
 */
#include "stdint.h"
#include "GuiDefine.h"
#include "system/debug/sys_debug.h"

//FIXME: cannot move declare to header ???
/** @brief Storage of image on UI */
uint8_t g_graphicImage[SIZE_GRAPHIC_IMAGE] __attribute__((section(".ddr_data"), space(prog)));

/** @brief Storage of font on UI */
uint8_t g_graphicAbelFont[SIZE_GRAPHIC_ABELFONT] __attribute__((section(".ddr_data"), space(prog)));

/** @brief Storage of font on UI */
uint8_t g_graphicBebasFont[SIZE_GRAPHIC_BEBASFONT] __attribute__((section(".ddr_data"), space(prog)));

/* end of file */