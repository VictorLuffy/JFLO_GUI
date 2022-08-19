
#include <stdbool.h>
#include "system_config.h"
#include "system_definitions.h"

#include "OperationManager.h"
#include "GuiInterface.h"
#include "MotorTask.h"
#include "HeaterTask.h"
#include "Watchdog.h"
#include "Button.h"
#include "DRV8308.h"
#include "SysTemTask.h"
#include "File.h"
#include "SoftwareUpgrade.h"
#include "Cradle.h"
//#include "Audio.h"
#include "../Gui/PowerOffScreen.h"

static void _SYS_DelayUs(uint16_t microseconds);

void _SYS_DelayMs(uint16_t ms);

static TickType_t lastBtnPressedTick = 0;
    
/*Check start signal, just delay 2s to make sure the user want to start
 the machine*/
bool OperationMgr_CheckStartSignal() {
#define START_UP_DELAY  (2000)

  uint32_t cnt = 0;
    
    int i = 0;
    for (i = 0; i < 55; i ++)
    {
        powerManagement_UpdateACOKState();
        _SYS_DelayMs(10);
    }
  
    if(powerManagement_ACOKStateGet() == false)//AC disconnect
    {
        while(true)
        {
            if(POWER_SWStateGet() == false){
                cnt++;
            }
            else{
                cnt = 0;
            }
            if (cnt >= START_UP_DELAY/40){
                return true;
            }
            _SYS_DelayMs(40);
        }
    }
    else//AC connect
    {
        if(POWER_SWStateGet() == true)//no power button push
        {
            //create charge task and system task to handle charge
            System_CreateChargingTask();
            System_CreateSystemTask();
            /**************
            * Start RTOS * 
            **************/
            vTaskStartScheduler(); /* This function never returns. */
        }
        else{
            return true;
        }
    }
}

/*check stop condition, detect the ON/OFF button to press 2s while then machine
 is operating */
bool OperationMgr_CheckStopSignal() {
    
#define TIME_PRESS_MS (2000)
#define SYS_TASK_PERIOD (10)
    static bool s_prevPowerSWstate = false;
    static int32_t s_cntPressTime = -1;

    if (SoftwareUpgrade_IsCopying())
    {
        return false;
    }
    
    bool powerSw = POWER_SWStateGet();
    if(powerSw == false)
    {
        if (s_prevPowerSWstate == false)
        {
            if(s_cntPressTime != (-1)){
                s_cntPressTime++;
            }
            if(s_cntPressTime >= TIME_PRESS_MS/SYS_TASK_PERIOD){
                return true;
            }
        }
    }
    else
    {
        s_cntPressTime = 0;
    }
    s_prevPowerSWstate = powerSw;
    return false;
}

/*monitor power during operation, including check status of AC power connection, internal
//battery, external battery .... If out of battery or bad power is detected, return false.
 otherwise, return true 
 This function also should call at start up to make sure the power is OK to start*/
bool OperationMgr_MonitorPower() {

}


void OperationMgr_PowerOffSystem()
{
    SYS_PRINT("\nPower down\n");
    //display pop up on GUI
    if (!SoftwareUpgrade_IsUpgrade())
    {
        guiInterface_SendEvent(eGuiChangeToPowerOffScreenId,0);
        guiInterface_SendEvent(eGuiTurnOffMachineId, 0);
        PowerOffScreen_StopSound();
        vTaskDelay(200);
        PowerOffScreen_CheckSetSound();
        
         //stop heater
        HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId};
        HeaterTask_SendEvent(hEvent);
        vTaskDelay(4800);
        //stop motor
        MOTOR_CTRL_EVENT_t mEvent = {.id = eMotorStopId};
        MotorTask_SendEvent(mEvent); 
    }
       
    //turn off cradle pump
    cradle_SetWaterSupplyOnOff(eOff);
    //Watchdog_Disable();
    // Deinit file system
    file_CloseFileOnSQIFlash();
    LogInterface_DeInitDebugLogFile();

    vTaskDelay(500);
    //turn off LCD back light     
    PWM_LCDBacklight_SetDutyCycle(0);
    
    vTaskSuspendAll();
       
    //vTaskEnterCritical();
    SYS_RESET_SoftwareReset();
    POWER_ENABLEOff();
}


/*Operation Manager task: check stop condition, check power condition*/
void OperationMgr_Task() {
    //check stop condition
    //    bool powerSw = POWER_SWStateGet();
    //    if (powerSw == false)       //pressed 
    //    {
    //        if((xTaskGetTickCount() - lastBtnPressedTick) >= 2000) {
    //            POWER_ENABLEOff();
    //            //display pop up on GUI
    //            guiInterface_SendEvent(eGuiTurnOffMachineId, 0);
    //            //stop motor
    //            MOTOR_CTRL_EVENT_t mEvent = {.id = eMotorStopId};
    //            MotorTask_SendEvent(mEvent);
    //            //stop heater
    //            HEATER_CTRL_EVENT_t hEvent = {.id = eHeaterStopId};
    //            HeaterTask_SendEvent(hEvent);
    //        }
    //    }
    //    else                    //released
    //    {
    //        lastBtnPressedTick = xTaskGetTickCount();
    //    }
    
//    if(button_GetButtonState(ePowerButton) == ePressHold )
    if(OperationMgr_CheckStopSignal() == true )
    {   
        
        OperationMgr_PowerOffSystem();
    }  
}


#define READ_CORE_TIMER()                 _CP0_GET_COUNT()          // Read the MIPS Core Timer
 
void _SYS_DelayUs(uint16_t microseconds)
{
    uint32_t time;
    
    time = READ_CORE_TIMER(); // Read Core Timer    
    time += (SYS_CLK_FREQ / 2 / 1000000) * microseconds; // calc the Stop Time    
    while ((int32_t)(time - READ_CORE_TIMER()) > 0){};    
}

void _SYS_DelayMs(uint16_t ms) {
    uint32_t i = 0;
    for (i = 0; i < ms; i++) {
        _SYS_DelayUs(1000);
    }
}
  