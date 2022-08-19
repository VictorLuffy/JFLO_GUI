/* ************************************************************************** */
/** @file [SmartBattery.h]
 *  @brief {support interface to communicate with Smart Battery to get 
 * information, report error if occur during communication}
 *  @author {nguyen truong}
 */
/* ************************************************************************** */




#ifndef _SMART_BATTERY_H    /* Guard against multiple inclusion */
#define _SMART_BATTERY_H


/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>




typedef enum
{
	eInternalBattery = 0,
	eCradleBattery,
} E_BatteryDevice;

typedef enum
{
	eNotCharging = 0,
	eCharging,
} E_BatteryChargeState;

typedef enum
{     
    eBATTNotConnected = 0,
    eBATTConnected = 1,        
}E_BATTERY_CONNECT_STAT;


typedef enum
{     
    eDCNotConnected = 0,
    eDCConnected = 1,        
}E_DC_CONNECT_STAT;

/** @brief Status bit flags of Smart battery*/
#define OVER_CHARGED_ALARM          (1 << 15)
#define TERMINATE_CHARGE_ALARM      (1 << 14)
#define OVER_TEMP_ALARM             (1 << 12)
#define TERMINATE_DISCHARGE_ALARM   (1 << 11)
#define REMAINING_CAPACITY_ALARM    (1 << 9)
#define REMAINING_TIME_ALARM        (1 << 8)
#define BATTERY_INITIALIZED         (1 << 7)
#define BATTERY_DISCHARGING         (1 << 6)
#define BATTERY_FULLY_CHARGED       (1 << 5)
#define BATTERY_FULLY_DISCHARGED    (1 << 4)


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    
//    bool smartBattery_ReadData(uint8_t command, uint16_t *readValue);
//    bool smartBattery_WriteData(uint8_t command, uint16_t value);
    
    
    /** @brief Get connection state of battery 
    *  @param [in]  None
    *  @param [out]  None
    *  @return E_BATTERY_CONNECT_STAT
    *  @retval the value indicated the connection state 
    */
    E_BATTERY_CONNECT_STAT smartBattery_GetBatteryConnectionState();
    
    
    /** @brief Get connection state of DC power 
    *  @param [in]  None  
    *  @param [out]  None
    *  @return E_DC_CONNECT_STAT
    *  @retval the value indicated the connection state 
    */
    E_DC_CONNECT_STAT smartBattery_GetDCConnectionState();
    
    
    
    /** @brief Get remaining  capacity (percentage) of battery 
    *  @param [in]  E_BatteryDevice device: the battery need to get (internal or cradle battery)  
    *  @param [out]  None
    *  @return uint16_t
    *  @retval the value indicated the remaining percentage of battery
    */
    uint16_t smartBattery_GetRemainingPercentage();
    
    /** @brief Get remaining capacity (mAh) of battery 
     *  @param [in]  None 
     *  @param [out]  None
     *  @return uint16_t
     *  @retval remainCapacity: remaining capacity of battery (mAh)
     */
    uint16_t smartBattery_GetRemainingCapacity();
    
    
    /** @brief Get present voltage of battery 
    *  @param [in]  None 
    *  @param [out]  None
    *  @return uint16_t
    *  @retval voltage: present voltage of battery (mV)
    */
    uint16_t smartBattery_GetVoltage();
    
    /** @brief Get current of battery 
    *  @param [in]  None 
    *  @param [out]  None
    *  @return uint16_t
    *  @retval Returns the current being supplied (or accepted) through the battery terminals - mA
    */
    int16_t smartBattery_GetCurrent();
    
    /** @brief Get present status of battery 
     *  @param [in]  None 
     *  @param [out]  None
     *  @return uint16_t
     *  @retval status: present status of battery
     */
    uint16_t smartBattery_GetStatus();

    /** @brief Get predicted remaining battery life of battery 
    *  @param [in]  None 
    *  @param [out]  None
    *  @return uint16_t
    *  @retval Returns the predicted remaining battery life at the present rate of discharge (minutes)
    */
    uint16_t smartBattery_GetRunTimeToEmpty();

    
    /** @brief Get present temperature of battery 
    *  @param [in]  None  
    *  @param [out]  None
    *  @return float
    *  @retval temperature: present temperature of battery (°C)
    */    
    float smartBattery_GetTemperature();
    
    /** @brief Get state of health (SOH) battery, SOH is the ratio of FullChargeCapacity() and DesignCapacity()
    *  @param [in]  None  
    *  @param [out]  None
    *  @return float
    *  @retval SOH: ratio of FullChargeCapacity() and DesignCapacity()
    */  
    float smartBattery_GetStateOfHealth();

    /** @brief Get current charging state of battery (charging or not)
     *  @param [in]  None  
     *  @param [out]  None
     *  @return E_BatteryChargeState
     *  @retval chargeState: current charging state of battery
     */
    E_BatteryChargeState smartBattery_GetChargeState();
    
    
    
    /** @brief Set charge option when initialize,
    * this function should be called after initializing the I2C2  
    *  @param [in]  None  
    *  @param [out]  None
    *  @return None
    *  @retval None
    */
   void smartBattery_Initialize();
   
   
    /** @brief run task for handling battery (handle charing and get information from battery),
    * this function should be called in Device Task loop
    * read data from battery and control charging
    *  @param [in]  None
    *  @param [out]  None
    *  @return None
    */
     void smartBattery_Handle();
     
     /** @brief Get current status of battery communication
    *  @param [in]  None 
    *  @param [out]  None
    *  @return bool
    *  @retval true if error, false if not error
    */
    bool smartBattery_IsBatteryCommunicationFailure();


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _SMART_BATTERY_H */

/* *****************************************************************************
 End of File
 */
