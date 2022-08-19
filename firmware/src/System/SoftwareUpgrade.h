/** @file SoftwareUpgrade.h
 *  @brief Update software
 *  @author Viet Le
 */

#ifndef _SOFTWARE_UPGRADE_H    /* Guard against multiple inclusion */
#define _SOFTWARE_UPGRADE_H

#include "stdbool.h"

/**@brief Define data type for PIC18 upgrade firmware state machine*/
typedef enum{
    eIdle = 0,
    eJumpToBld,
    eWaitBldResponse,
    eEraseFlash,
    ePrepareFlashData,
    eFlashDataState
} E_PIC18UpgradeFirmwareState;

/**@brief Define enum type for process command state machine*/
typedef enum{
    eSendCommand,
    eHandleCommandResponse
}E_PIC18ProcessCommandState;

/**@brief Define enum type for result of process command*/
typedef enum{
    eProcessFail = 0,
    eProcessDone
} E_processCommandState;
//function to initialize Software upgrade module
void softwareUpgrade_Init();

//function software Upgrade Process
void softwareUpgrade_Process(void);

//function software Upgrade Process
void softwareUpgrade_NewSoftwareForCradle(void);

//function software Upgrade Process
void softwareUpgrade_NewSoftwareForChamber(void);

bool softwareUpgrade_GetFileUpdate(void);

bool softwareUpgrade_IsCopying(void);

bool softwareUpgrade_IsUpgrade(void);

uint32_t sizeOfMainboardFirmware = 0;

#endif /* _SOFTWARE_UPGRADE_H */

/* *****************************************************************************
 End of File
 */
