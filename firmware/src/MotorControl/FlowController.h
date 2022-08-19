/* ************************************************************************** */
/** @file [FlowController.h]
 *  @brief {A PID controller for Flow control. This controller take input from 
 * Air Flow Sensor & O2 Flow Sensor and control Blower follow the Flow setting }
 *  @author {bui phuoc}
 */
/* ************************************************************************** */



#ifndef _FLOW_CONTROLLER_H    /* Guard against multiple inclusion */
#define _FLOW_CONTROLLER_H


/* This section lists the other files that are included in this file.
 */
#include <stdbool.h>


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief Function initialize PID controller for Flow Control and set it up before
     * running
     *  @param [in]  None
     *  @param [out]  None
     *  @return  None
     */
    void FlowController_Initialize();

    /** @brief Function perform PID controller calculation with measurement value obtained
     * from Air + O2 flow sensor
     *  @param [in]  float measured     measurement value, in this case: Air + O2 Flow
     *  @param [out]  None
     *  @return float       result after PID calculation
     */
    float FlowController_Operate(float measured, float target);

    /** @brief Function to anable / disable Flow's PID controller
     *  @param [in]  bool enable     enable = 1/ disable = 0
     *  @param [out]  None
     *  @return None
     */
    void FlowController_Enable(bool enable);

    /** @brief Function to set target of Flow's PID controller. In this case, target 
     * is the Flow setting value
     *  @param [in]  float target     target to apply to Flow's PID controller
     *  @param [out]  None
     *  @return None
     */
    void FlowController_SetTarget(float target);
    
    float FlowController_GetControlValue(void);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _FLOW_CONTROLLER_H */

/* *****************************************************************************
 End of File
 */
