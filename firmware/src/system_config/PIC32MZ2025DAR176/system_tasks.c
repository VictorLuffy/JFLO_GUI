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
#include "SysTemTask.h"
#include "I2C_2.h"
#include "SmartBattery.h"
#include "UART_1.h"
#include "common.h"
#include "SystemInterface.h"
#include "GuiInterface.h"
// *****************************************************************************
// *****************************************************************************
// Section: Local Prototypes
// *****************************************************************************
// *****************************************************************************


/** @brief Define PATIENT DATA task priority */
#define		CHARGING_TASK_PRIORITY		(tskIDLE_PRIORITY + 1) 

/** @brief Define PATIENT DATA task stack size */
#define 	CHARGING_TASK_STACK			(256)//(1*1024)

/** @brief Declare PATIENT DATA task periodic */
#define     CHARGING_TASK_PERIODIC_MS     (10 / portTICK_PERIOD_MS)        //10 ms


static bool s_systemStarted = false;
 
static void _SYS_Tasks ( void );

extern uint8_t g_UpgradeState;
static uint8_t g_PrevUpgradeState;
extern uint32_t sizeOfMainboardFirmware;


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
    s_systemStarted = true;

    System_CreateSystemTask();
 
    DeviceTask_Create();
 
    //HeaterTask_Create();
    
    //MotorTask_Create();

//    PtDataTask_Create();
    
    GuiTask_Create();
    
    //TODO: gui debug
    alarmTask_Create();
    
    ExtComTask_Create();
    
    MonitorTask_Create();
    
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
        CmdProcessor_Task();
        SYS_TMR_Tasks(sysObj.sysTmr);               //move to IDLE task

        monitor_UpdateStartedTime(eSystem);
        if(s_systemStarted == true)
        {
            //PC_Monitor_Task();
            OperationMgr_Task();
            SystemTask_HandleEvent();
            
            //Bootloader 
            Bootloader_Tasks();

            static int countOfBootloaderReadFile = 0;
            if(g_PrevUpgradeState != g_UpgradeState)
            {
                g_PrevUpgradeState = g_UpgradeState;
                if(g_UpgradeState == BOOTLOADER_ERROR)
                {
                    SYS_PRINT("BOOTLOADER_ERROR\n");
                    guiInterface_SendEvent(eGuiUpdateScreenMessageUpdateFailed, 0);
                }
                if(g_UpgradeState == BOOTLOADER_CLOSE_DATASTREAM)
                {
                    SYS_PRINT("BOOTLOADER_CLOSE_DATASTREAM\n");
                }
                if(g_UpgradeState == BOOTLOADER_READ_FILE)
                {
                    SYS_PRINT("BOOTLOADER_READ_FILE\n");
                    countOfBootloaderReadFile = 0;
                }
                if(g_UpgradeState == BOOTLOADER_UPDATE_COMPLETE)
                {
                    SYS_PRINT("BOOTLOADER_UPDATE_COMPLETE\n");
                }               
            }
            if(g_UpgradeState == BOOTLOADER_READ_FILE )
            {
                countOfBootloaderReadFile++;
                static int pre_percent = 0;
                int percent = 0;
                if (sizeOfMainboardFirmware != 0)
                {
                    percent = countOfBootloaderReadFile*512*100 / sizeOfMainboardFirmware;
                }

                if (percent != pre_percent)
                {
                    pre_percent = percent;
                    SYS_PRINT("BOOTLOADER_READ_FILE status %d / %d \n", countOfBootloaderReadFile*512, sizeOfMainboardFirmware);
                    guiInterface_SendEvent(eGuiUpdateScreenMessageUpdatingMainboardFirmwareStatus, percent);
                    vTaskDelay(50);
                }
            }
            
        }
        else
        {
            static uint32_t cnt = 0;
            Nop();
            if(POWER_SWStateGet() == false)
            {
                cnt++;
            }
            else
            {
                cnt = 0;
            }
            #define START_UP_DELAY  (2000)
            if (cnt >= (START_UP_DELAY/10))
            {
                //reset and start operating system
                vTaskSuspendAll();
                Nop();
                SYS_RESET_SoftwareReset();
            }
        }       
        /* Task Delay */
        vTaskDelayUntil(&xLastWakeTime, 10);
    }
}      

/** @brief Function to maintain Charge control task. This function will be executed
 * automatically by RTOS after System_CreateChargingTask() and vTaskStartSchedulers function is called
 *  @param [in]     None
 *  @param [out]    None
 *  @return None
 */
static void System_ChargeControlTask( void)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    //initialize i2c peripheral to communicate with charger and battery
    I2C2_Init();
    //initialize battery
    smartBattery_Initialize();
    //initialize watchdog
//    Watchdog_Init();
    
    //test uart1
    //Uart1_Initialize();
    
    while(1)
    {   
        powerManagement_UpdateACOKState();
        
        //handle battery control
        smartBattery_Handle();
        //handle battery control
        powerManagement_Handle();
        //Clear watchdog
        Watchdog_clearTimer();
        //Task delay
        vTaskDelayUntil(&xLastWakeTime, CHARGING_TASK_PERIODIC_MS);
    }
}


/** @brief Function to create Charging Task  run with FreeRTOS
 * charging task only run when machine in off state for charge control
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void System_CreateChargingTask(void) {
    xTaskCreate((TaskFunction_t) System_ChargeControlTask,
            "System Charging Task",
            CHARGING_TASK_STACK, NULL, CHARGING_TASK_PRIORITY, NULL);
}

/** @brief Function to create Charging Task  run with FreeRTOS
 *  @param [in] None
 *  @param [out] None
 *  @return None
 */
void System_CreateSystemTask(void) {
    /* Create OS Thread for Sys Tasks. */
    xTaskCreate((TaskFunction_t) _SYS_Tasks,
                "Sys Tasks",
                1024, NULL, 1, NULL);
}




/*******************************************************************************
 End of File
 */
