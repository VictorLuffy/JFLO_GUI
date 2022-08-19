/*******************************************************************************
 System Tasks File

  File Name:
    system_tasks.c

  Summary:
    This file contains source code necessary to maintain system's polled state
    machines.

  Description:
    This file contains source code necessary to maintain system's polled state
    machines.  It implements the "SYS_Tasks" function that calls the individual
    "Tasks" functions for all the MPLAB Harmony modules in the system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    polled in the system.  These handles are passed into the individual module
    "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "system_definitions.h"
#include "app.h"
#include "HeaterTask.h"
#include "MotorTask.h"
#include "PtDataTask.h"
#include "CommandProcessor.h"
#include "PC_Monitoring.h"
#include "AlarmTask.h"
#include "OperationManager.h"


// *****************************************************************************
// *****************************************************************************
// Section: Local Prototypes
// *****************************************************************************
// *****************************************************************************


 
static void _SYS_Tasks ( void );
void _SYS_RTCC_Tasks(void);
void _SYS_CONSOLE_IDX0_Tasks(void);

 
 

static void _APP_Tasks(void);


// *****************************************************************************
// *****************************************************************************
// Section: System "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/

void SYS_Tasks ( void )
{
//    AUDIO_RESETOn();
//    //POWER_ENABLEOn();
//    if (OperationMgr_CheckStartSignal() == true) {
//        //enable power,
//        POWER_ENABLEOn();
//        //AUDIO_RESETOn();
//    }
    
    /* Create OS Thread for Sys Tasks. */
    xTaskCreate((TaskFunction_t) _SYS_Tasks,
                "Sys Tasks",
                1024, NULL, 1, NULL);
 
    DeviceTask_Create();
 
    //HeaterTask_Create();
    
    //MotorTask_Create();

    PtDataTask_Create();
    
    TaskIdle_Create();
    
    alarmTask_Create();
    
    /**************
     * Start RTOS * 
     **************/
    vTaskStartScheduler(); /* This function never returns. */
}


/*******************************************************************************
  Function:
    void _SYS_Tasks ( void )

  Summary:
    Maintains state machines of system modules.
*/

static void _SYS_Tasks ( void)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    while(1)
    {
      //SYS_PRINT("tickSYS_ %d\n",xTaskGetTickCount());  
      
        /* Maintain system services */
    /* Maintain the file system state machine. */
//    SYS_FS_Tasks();
    /* SYS_COMMAND layer tasks routine */ 
//    SYS_CMD_Tasks();
    /* SYS_TMR Device layer tasks routine */
      
    //run Command processor task
    CmdProcessor_Task();
    SYS_TMR_Tasks(sysObj.sysTmr);               //move to IDLE task
//    SYS_INP_Tasks();

        /* Maintain Device Drivers */
//    DRV_SST26_Tasks(sysObj.drvSst26Obj0);
 
 

        /* Maintain Middleware */
	// update the GFX Abstraction Layer
	/*GFX_Update();
	// update the UI library
	LibAria_Tasks();*/
     
    /* USB HS Driver Task Routine */
//     DRV_USBHS_Tasks(sysObj.drvUSBObject);
     
 
         /* USB Host layer task routine.*/
//    USB_HOST_Tasks(sysObj.usbHostObject0);
    
    
//        FileSystemMgr_Task();               //move to IDLE task

        PC_Monitor_Task();
        
        OperationMgr_Task();
        
        //Watchdog_clearTimer();
        
        /* Task Delay */
        //vTaskDelay(18 / portTICK_PERIOD_MS);
        vTaskDelayUntil(&xLastWakeTime, 18);
    }
}

 void _SYS_CONSOLE_IDX0_Tasks(void)
{
    while(1)
    {
        SYS_CONSOLE_Tasks(sysObj.sysConsole0);
        vTaskDelay(0 / portTICK_PERIOD_MS);
    }
 }

        
      
/*******************************************************************************
  Function:
    void _APP_Tasks ( void )

  Summary:
    Maintains state machine of APP.
*/
/*
static void _APP_Tasks(void)
{
    while(1)
    {
        APP_Tasks();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
*/


/*******************************************************************************
 End of File
 */
