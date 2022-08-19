/* ************************************************************************** */
/** @file [WaterSupplyCtrl.h]
 *  @brief Water in tank is supplied by water pump which integrated in cradle. When the main unit
 *  is removed from the cradle for use, the water in the tank should be enough for at least 30 minutes.
 *  This water control function is responsible for keeping the tank full.
 *  @author {truong nguyen}
 */
/* ************************************************************************** */

#ifndef _WATER_SUPPLY_CONTROL_H    /* Guard against multiple inclusion */
#define _WATER_SUPPLY_CONTROL_H


/* This section lists the other files that are included in this file.
 */

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /** @brief Turn off and disable cradle pump
    *  @param [in]     None
    *  @param [out]    None
    *  @retval None    
    */
    void waterSupplyCtrl_TurnOffAndDisablePump(void);
    
    /** @brief Handle Water Supply Control
    *  @param [in]     None
    *  @param [out]    None
    *  @retval 
    */
    void waterSupplyCtrl_Handle();


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _WATER_SUPPLY_CONTROL_H */

/* *****************************************************************************
 End of File
 */
