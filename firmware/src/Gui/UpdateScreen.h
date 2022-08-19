/** @file UpdateScreen.h
 *  @brief The screen provides graphic interface to update firmware
 *  @author Thanh Hiep Nguyen
 */

#ifndef UpdateScreen_H
#define	UpdateScreen_H

#include "DisplayControl.h"

#define MAINBOARD_FIRMWARE_UPDATE_ID            0x01

#define MAINBOARD_MEDIA_UPDATE_ID               0x02

#define CHAMBER_FIRMWARE_UPDATE_ID              0x04

#define CRADLE_FIRMWARE_UPDATE_ID               0x08

// Init some elements
void UpdateScreen_Initialize(void);

// Reinit some elements
void UpdateScreen_Reinit(void);

// Destroy some elements
void UpdateScreen_Distructor(void);

// Main process of screen
void UpdateScreen_Run(void);

void UpdateScreen_SetItemToUpdate(uint8_t item);

uint8_t UpdateScreen_GetItemToUpdate(void);

#endif	/* MaintenanceScreen_H */

/* end of file */