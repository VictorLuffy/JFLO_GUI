/* ************************************************************************** */
/** @file [PWM_IH.c]
 *  @brief {Control PWM signal for IH circuit, including turn ON, turn OFF, set 
 * Power drop on IH coil (from 0% to 100% Power).
 * All settings and configurations on this file relate tightly with the MPLAB HARMONY configuration.
 * Change the configuration of the HARMONY may lead these function failed}
 *  @author {bui phuoc}
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_definitions.h"
#include "system_config.h"
#include "PWM_IH.h"
#include "PWM_IHCom.h"
#include "PWM_IHSignal.h"


/** @brief Operation status of IH PWM generator */
static bool s_IsOperating = false;

/** @brief Function to initialize PWM support for IH circuit; including initialize
 * OC1 as IH_com and OC2 as IH_signal and Timer 2 as timer base for OCs to work
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_IH_Initialize() {
    //initialize common PWM
    PWM_IHCom_init();

    //initialize Humidity PWM
    PWM_IHSignal_init();
}

/** @brief Function to start output PWM to port pin that specified earlier. starting 
 * process include start OCs module and the timer base also
 * OC1, IH com --> output to PIN RG9
 * OC2, IH signal --> output to PIN RD0
 * Before calling this function, make sure the function PWM_IH_Initialize() had been called
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_IH_Start() {
    //start timer PWM IH
    DRV_TMR2_Start();
    //start generating Com. PWM
    PWM_IHCom_start();
    //start generating Signal PWM
    PWM_IHSignal_start();

    //set operation flag
    s_IsOperating = true;
}

/** @brief Function to stop output PWM to all IH PWM channel PIN.
 * Before calling this function, make sure the function PWM_IH_Initialize() had been called
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_IH_Stop() {
  
    PWM_IHSignal_setPhaseDiff(0);
    //stop timer base
    DRV_TMR2_Stop();
    //stop IH_com channel
    PWM_IHCom_stop();
    //stop IH_signal channel
    PWM_IHSignal_stop();

    //clear operation flag
    s_IsOperating = false;
}

/** @brief Function to get status of IH PWM operation.
 * Before calling this function, make sure the function PWM_IH_Initialize() had been called
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 *  @retval true IH_PWM is running
 *  @retval false IH_PWM is not running, it has stopped
 */
bool PWM_IH_IsOperating() {
    return s_IsOperating;
}

/** @brief Function to set phase difference for IH coil. The Power is  made by the phase 
 * different between Com. channel and Signal channel. 
 * level 0 mean phase difference is 0
 * level 20 mean phase difference is 180 degrees
 * while setting the Power, the Com channel will not change, only Signal channel will 
 * shift its phase to make the phase difference to create Power
 * Before calling this function, make sure the function PWM_IH_Initialize() and PWM_IH_Start()
 * had been called
 *  @param [in]  uint16_t phaseDifferenceLev : level of phase difference, has value from 
 * 0 to 20   
 *  @param [out]  None
 *  @return None
 */
void PWM_IH_SetPhaseDifference(float phaseDifferenceLev) {
    float level = phaseDifferenceLev / 20.0;
    float rawDiffCount = level * (float) PWM_IH_TIMER_PHASE_COUNT;
    uint32_t phaseDiffInCount = (uint32_t) rawDiffCount;
    //adjust power for humidity
    PWM_IHSignal_setPhaseDiff(phaseDiffInCount);
}


/* *****************************************************************************
 End of File
 */
