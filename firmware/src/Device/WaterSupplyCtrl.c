/* ************************************************************************** */
/** @file [WaterSupplyCtrl.h]
 *  @brief Water in tank is supplied by water pump which integrated in cradle. When the main unit
 *  is removed from the cradle for use, the water in the tank should be enough for at least 30 minutes.
 *  This water control function is responsible for keeping the tank full.
 *  @author {truong nguyen}
 */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include "system_config.h"
#include "system_definitions.h"

#include "WaterSupplyCtrl.h"

#include "Cradle.h"
#include "ChamberUnit.h"


/** @brief Declare Device task periodic, millisecond unit */
#define    DEVICE_TASK_PERIODIC_MS                 (200)       
/** @brief Declare max time pump is on, minute unit */
#define    PUMP_ON_MAX_TIME                         (2)         
/** @brief Declare max time count pump is on, minute unit */
#define    PUMP_ON_MAX_TIME_COUNT     (PUMP_ON_MAX_TIME*60*1000/DEVICE_TASK_PERIODIC_MS)

/** @brief status enable of pump */
static bool gs_isEnablePump = true;

/** @brief current pump operation state */
static E_OnOffState gs_PumpCurrentState = eOff;
/** @brief previous pump operation state */
static E_OnOffState gs_PumpPrevState = eOff;


/** @brief Turn off and disable cradle pump
 *  @param [in]     None
 *  @param [out]    None
 *  @retval None    
 */
void waterSupplyCtrl_TurnOffAndDisablePump(void)
{
    gs_isEnablePump = false;
    gs_PumpCurrentState = eOff; 
}

/** @brief 
 *  @param [in]     None
 *  @param [out]    None
 *  @retval true    if calibration success
 *  @retval false   if calibration false
 */
void waterSupplyCtrl_Run(void)
{
    static uint32_t s_CountTimePumpOn = 0;
    
    E_TankWaterLevel currentWaterLevel = Chamber_GetTankWaterLevel();
//    SYS_PRINT("currentWaterLevel : %X\n", currentWaterLevel);
    

    // there is no any alarm requires stop pump
    if (gs_isEnablePump == true)
    {
        //Tank is not full
        if(eTankIsNotFull == currentWaterLevel)
        {
            //set pump on
            gs_PumpCurrentState = eOn;
            //increase time
            s_CountTimePumpOn++;
            //pump run over than 2 minutes
            if(s_CountTimePumpOn >= PUMP_ON_MAX_TIME_COUNT)
            {    
                //clear time count
                s_CountTimePumpOn = 0;
                //set pump off
                gs_PumpCurrentState = eOff;
                //disable pump
                waterSupplyCtrl_TurnOffAndDisablePump();
            }
        }
        E_ChamberConnectState chamberConnectState = chamber_GetChamberConnection();
        //Tank is full or chamber disconnect
        if((eTankIsFull == currentWaterLevel) || (chamberConnectState == eChamberDisconnected))
        {
            //clear time count
            s_CountTimePumpOn = 0;
            //set pump off
            gs_PumpCurrentState = eOff;
        }
    }
    
    //update pump state
    if(gs_PumpPrevState != gs_PumpCurrentState)
    {
        E_ConnectState cradleConnectState = cradle_GetCradleConnection();
        if(cradleConnectState == eConnect)
        {
            cradle_SetWaterSupplyOnOff(gs_PumpCurrentState);
            SYS_PRINT("cradle_SetWaterSupplyOnOff(0: off, 1: On) : %d\n", gs_PumpCurrentState);
            gs_PumpPrevState = gs_PumpCurrentState;
        }
    }
}

/** @brief Handle Water Supply Control
 *  @param [in]     None
 *  @param [out]    None
 *  @retval 
 */
void waterSupplyCtrl_Handle()
{
    E_ConnectState cradleConnectState = cradle_GetCradleConnection();
    if(cradleConnectState == eConnect)
    {
        waterSupplyCtrl_Run();
    }
}
/* *****************************************************************************
 End of File
 */