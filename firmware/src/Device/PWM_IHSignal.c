/* ************************************************************************** */
/** @file [PWM_IHSignal.c]
 *  @brief {Control PWM signal for IH_signal channel of IH circuit, including PWM
 * PWM frequency = 40 Khz fixed, duty cycle = 50% fixed, phase offset range from 0
 * degrees to 180 degrees.
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

#include "PWM_IHCom.h"
#include "PWM_IH.h"


/** @brief OC PWM index assigned for IH_signal channel, in this case, index 1 */
#define PWM_IH_SIGNAL_INDEX      1 


/** @brief Handle PWM for IH control signals */
static DRV_HANDLE   s_PWM_IHSignalHandle = DRV_HANDLE_INVALID;


/** @brief Phase different between IH signal and IH com */
static uint32_t s_PhaseDiffInCount = 0;

// ****************** internal functions ********************************

//function to update phase difference for IH_Signal PWM channel
void PWM_IHSignal_update();




/** @brief Function to initialize PWM_signal channel supports for IH circuit, include
 * setting PWM frequecy = 40 KHz, duty cycle 50%, phase offset 0 degree
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_IHSignal_init()
{
    //initialize PWM channels for Humidity signals
    if (s_PWM_IHSignalHandle == DRV_HANDLE_INVALID) {
        s_PWM_IHSignalHandle = DRV_OC_Open(PWM_IH_SIGNAL_INDEX, DRV_IO_INTENT_EXCLUSIVE);
    }
    
    //set 50% duty cycle
    DRV_OC_CompareValuesDualSet(s_PWM_IHSignalHandle, 0, PWM_IH_TIMER_PHASE_COUNT);
}




/** @brief Function to start output PWM to IH_signal PIN.
 * Before calling this function, make sure the function PWM_IHSignal_init() had been called
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_IHSignal_start()
{
    DRV_OC_Start(s_PWM_IHSignalHandle, DRV_IO_INTENT_EXCLUSIVE);
}




/** @brief Function to set phase offset of the IH_signal channel while keeping the
 * duty cycle 50%. Phase offset can be from 0 to 180 degree (half of cycle 360 degree)
 * Before calling this function, make sure the function PWM_IHSignal_init() had been called
 *  @param [in]  uint16_t phaseDiffCount: phase difference degree in count:
 * 0 count -->  0 degrees
 * PWM_IH_TIMER_PHASE_COUNT --> 180 degrees
 *  @param [out]  None
 *  @return None
 */
void PWM_IHSignal_setPhaseDiff(uint32_t phaseDiffCount) 
{
    //get value 
    s_PhaseDiffInCount = phaseDiffCount;
    
    //adjust value in range
    if (s_PhaseDiffInCount > PWM_IH_TIMER_PHASE_COUNT) 
    {
        s_PhaseDiffInCount = PWM_IH_TIMER_PHASE_COUNT;
    }

    //adjust output values
    PWM_IHSignal_update();
}




/** @brief Function to stop output PWM to IH_signal PIN.
 * Before calling this function, make sure the function PWM_IHSignal_init() had been called
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_IHSignal_stop()
{
    DRV_OC_Stop(s_PWM_IHSignalHandle);
}
    



/** @brief Function to apply phase difference to IH_signal channel.
 * Before calling this function, make sure the function PWM_IHSignal_init() had been called
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_IHSignal_update()
{
    uint32_t priCount, secCount;
    //set value for Common High chanel
    priCount = s_PhaseDiffInCount;
    secCount = PWM_IH_TIMER_PHASE_COUNT + s_PhaseDiffInCount;
    //apply new value
    DRV_OC_CompareValuesDualSet(s_PWM_IHSignalHandle, priCount, secCount);
}



/* *****************************************************************************
 End of File
 */
