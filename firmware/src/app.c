/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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

#include "app.h"
#include "DeviceTask.h"
#include "HeaterTask.h"
#include "MotorTask.h"
#include "PtDataTask.h"
#include "GuiTask.h"
#include "FileSystemMgr.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;
/* Encryption related variables */
static uint32_t __attribute__ ((coherent, aligned (16))) app_icBuffer[APP_IC_NUM_EDGES];

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/******************************************************************************
  Function:
    static void APP_IC_Task (void)

   Remarks:
    Captures timer value on programmed edge of ICx pin.

*/
static void APP_IC_Task (void)
{		
	switch(appData.icStates)
	{
		default:
		
		case APP_IC_START:
			/* Initialize number of IC Edges variable */
			appData.numICEdges = 0;
			
			/* Start the input capture operation */
			DRV_IC_Start(appData.handleIC0, DRV_IO_INTENT_READWRITE);
			
			/* Start Timer Driver */
			DRV_TMR_Start (appData.handleTMR0);		
			
			appData.icStates = APP_IC_READ_DATA;
			
			break;
			
		case APP_IC_READ_DATA:
			
			/* Check for buffer empty and read the buffer value */
			if (!DRV_IC_BufferIsEmpty(appData.handleIC0))
			{
				app_icBuffer[appData.numICEdges] = DRV_IC_Capture32BitDataRead(appData.handleIC0);
				appData.numICEdges++;
			}
				
			if (appData.numICEdges == APP_IC_NUM_EDGES)
			{
				/* Stop input capture and timer drivers */
				DRV_IC_Stop (appData.handleIC0);
				DRV_TMR_Stop (appData.handleTMR0);	
	
				appData.icStates = APP_IC_DONE;
			}
			
			break;
			
		case APP_IC_DONE:
			
			break;
	}
}

/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    appData.handleIC0 = DRV_HANDLE_INVALID;
	appData.handleTMR0 = DRV_HANDLE_INVALID;
    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    
    //AUDIO_RESETOn();
//    POWER_ENABLEOn();
    if (OperationMgr_CheckStartSignal() == true) 
    {
        //enable power,
        POWER_ENABLEOn();
        AUDIO_RESETOn();
    }
    
    HeaterTask_Initialize();
    
    MotorTask_Inititalize();
    
    DeviceTask_Initialize();
    
//    HumidityTask_Initialize();
    
//    PtDataTask_Initialize();
    
    GuiTask_Initialize();
    
    FileSystemMgr_Initialize();
    
    alarmTask_Initialize();
    
    ExtComTask_Initialize();
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
       
            /* Open the IC driver if it has not been previously opened */
            if (appData.handleIC0 == DRV_HANDLE_INVALID)
            {
                appData.handleIC0 = DRV_IC_Open(APP_IC_INDEX, DRV_IO_INTENT_READWRITE);
                appInitialized &= ( DRV_HANDLE_INVALID != appData.handleIC0 );
            }
        
			if (appData.handleTMR0 == DRV_HANDLE_INVALID)
            {
                appData.handleTMR0 = DRV_TMR_Open(APP_TMR_INDEX, DRV_IO_INTENT_READWRITE);
                appInitialized &= ( DRV_HANDLE_INVALID != appData.handleTMR0 );
            }
        
            if (appInitialized)
            {
            
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
		    /* Run the state machine for servicing Input Capture */
            APP_IC_Task();
			
        
            break;
        }

        /* TODO: implement your application state machine.*/
        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 

/*******************************************************************************
 End of File
 */
