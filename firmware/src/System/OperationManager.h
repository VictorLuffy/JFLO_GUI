/* 
 * File:   OperationManager.h
 * Author: MetranStaff
 *
 * Created on November 29, 2019, 10:08 AM
 */

#ifndef _OPERATION_MANAGER_H
#define	_OPERATION_MANAGER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    /*Check start signal, just delay 2s to make sure the user want to start
     the machine*/
    bool OperationMgr_CheckStartSignal();
    
    /*check stop condition, detect the ON/OFF button to press 2s while then machine
     is operating */
    bool OperationMgr_CheckStopSignal();
    
    /*monitor power during operation, including check status of AC power connection, internal
    //battery, external battery .... If out of battery or bad power is detected, return false.
     otherwise, return true 
     This function also should call at start up to make sure the power is OK to start*/
    bool OperationMgr_MonitorPower();
    
    /** @brief This operation shutdown the system
    *  @param [in] None
    *  @param [out] None
    *  @return None
    */
    void OperationMgr_PowerOffSystem();

    /*Operation Manager task: check stop condition, check power condition*/
    void OperationMgr_Task();

#ifdef	__cplusplus
}
#endif

#endif	/* _OPERATION_MANAGER_H */

