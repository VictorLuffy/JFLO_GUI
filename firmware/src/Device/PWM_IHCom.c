/* ************************************************************************** */
/** @file [PWM_IHCom.c]
 *  @brief {Control PWM signal for IH_com channel of IH circuit, including PWM
 * PWM frequency = 40 Khz fixed, duty cycle = 50% fixed, phase offset 0 degree fixed.
 * All settings and configurations on this file relate tightly with the MPLAB HARMONY configuration.
 * Change the configuration of the HARMONY may lead these function failed}
 *  @author {bui phuoc}
 */
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


/** @brief OC PWM index assigned for IH_com channel, in this case, index 0 */
#define PWM_IH_COM_INDEX   0        




/** @brief Handle PWM for IH_com signals */
static DRV_HANDLE   s_PWM_IHComHandle = DRV_HANDLE_INVALID;




/** @brief Function to initialize PWM_com signal support for IH circuit, include
 * setting PWM frequecy = 40 KHz, duty cycle 50%, phase offset 0 degree
 * This function should be called 1 time at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_IHCom_init()
{
    //initialize PWM channels for Common signals
    if (s_PWM_IHComHandle == DRV_HANDLE_INVALID) {
        s_PWM_IHComHandle = DRV_OC_Open(PWM_IH_COM_INDEX, DRV_IO_INTENT_EXCLUSIVE);
    }
    
    //set 50% duty cycle, offset 0
    DRV_OC_CompareValuesDualSet(s_PWM_IHComHandle, 0, PWM_IH_TIMER_PHASE_COUNT);
}




/** @brief Function to start output PWM to IH_com PIN.
 * Before calling this function, make sure the function PWM_IHCom_init() had been called
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_IHCom_start()
{
	DRV_OC_Start(s_PWM_IHComHandle, DRV_IO_INTENT_EXCLUSIVE);
}




/** @brief Function to stop output PWM to IH_com PIN.
 * Before calling this function, make sure the function PWM_IHCom_init() had been called
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void PWM_IHCom_stop() 
{
    DRV_OC_Stop(s_PWM_IHComHandle);
}




/* *****************************************************************************
 End of File
 */
