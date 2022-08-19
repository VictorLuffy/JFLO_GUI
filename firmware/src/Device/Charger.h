/* ************************************************************************** */
/** @file [Charger.h]
 *  @brief {Supply APIs for the control of battery charging, including configure
 * charge option, charge voltage, charge current and input current}
 *  @author {nguyen truong}
 */
/* ************************************************************************** */




#ifndef _CHARGER_H    /* Guard against multiple inclusion */
#define _CHARGER_H


/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>



typedef enum
{
	eInternalCharger = 0,
	eCradleCharger,
} E_ChargeDevice;

typedef enum
{
    
    
    /* ACOK Deglitch Time Adjust || Bit Field [15]
    Adjust ACOK deglitch time.
    After POR, the first time the adapter plug in occurs, deglitch time is
    always 150 ms no matter if this bit is 0 or 1. This bit only sets the
    next ACOK deglitch time after ACFET turns off at least one time. To
    change this option, VCC pin voltage must be above UVLO and
    ACDET pin voltage must be above 0.6 V to enable IC SMBus
    communication. */
    
    // 0: ACOK rising edge deglitch time 150 ms
    ACOK_DEGLITCH_TIME_150MS  =  (0 << 15),
    // 1: ACOK rising edge deglitch time 1.3 s <default at POR (Power On Reset)>
    ACOK_DEGLITCH_TIME_1300MS  =  (1 << 15),
 
    
            
    /**************************************************************************/ 
            
    /*WATCHDOG Timer Adjust || Bit Field [14:13]
    Set maximum delay between consecutive SMBus Write charge
    voltage or charge current command. The charge will be suspended if
    IC does not receive write charge voltage or write charge current
    command within the watchdog time period and watchdog timer is
    enabled.
    The charge will be resumed after receive write charge voltage or
    write charge current command when watchdog timer expires and
    charge suspends. During boost function, the timer is fixed to 175 s if
    it is enabled.*/
    //00: Disable Watchdog Timer
    DISABLE_WATCHDOG_TIMER  = (0 << 13),
            
    //01: Enabled Watchdog, 44 sec
    EN_WATCHDOG_TIMER_44SEC  = (1 << 13),
            
    //10: Enabled Watchdog, 88 sec
    EN_WATCHDOG_TIMER_88SEC  = (2 << 13),
            
    //11: Enable Watchdog Timer (175 s)<default at POR>
    EN_WATCHDOG_TIMER_175SEC  = (3 << 13),
    
    /*=========================================================================*/        
            
            
    /*Battery depletion comparator threshold adjust || Bit Field [12:11]
    This is used for LEARN function and boost mode function battery
    over discharge protection. During LEARN cycle, when the IC detects
    battery voltage is below depletion voltage threshold, the IC turns off
    BATFET and turned on ACFET to power the system from AC
    adapter instead of the battery. During boost mode function, when the
    IC detects battery voltage is below depletion voltage threshold, IC
    stops boost function. The rising edge hysteresis is 340 mV. Set
    ChargeVoltage() register value to 0 V will disable this function.*/
    
            
    /*00: Falling Threshold = 59.19% of voltage regulation limit
    (approximately 2.486 V/cell)*/
    FALLING_THRESHOLD_59_PERCENT  =  (0 << 11),
    
    /*01: Falling Threshold = 62.65% of voltage regulation limit
    (approximately 2.631 V/cell)*/        
    FALLING_THRESHOLD_63_PERCENT  =  (1 << 11),           
                 
    /*10: Falling Threshold = 66.55% of voltage regulation limit
    (approximately 2.795 V/cell)*/
    FALLING_THRESHOLD_67_PERCENT  =  (2 << 11),           
            
    /*11: Falling Threshold = 70.97% of voltage regulation limit
    (approximately 2.981 V/cell) < default at POR>  */
    FALLING_THRESHOLD_71_PERCENT  =  (3 << 11),
            
    /**************************************************************************/
                       
            
    //EMI Switching Frequency Adjust || Bit Field [10]          
    //0: Reduce PWM switching frequency by 18% <default at POR>
    REDUCE_PWM_SW_FRE  = (0 << 10),
            
    //1: Increase PWM switching frequency by 18%
    INCREASE_PWM_SW_FRE  = (1 << 10),
            
    /**************************************************************************/         
            
    //EMI Switching Frequency Enable || Bit Field [9]        
    //0: Disable adjust PWM switching frequency <default at POR>
    DISABLE_PWM_SW_FRE  =  (0 << 9),
            
    //1: Enable adjust PWM switching frequency 
    ENABLE_PWM_SW_FRE  =  (1 << 9),
            
    /**************************************************************************/        
  
    
    /*IFAULT_HI Comparator Threshold Adjust|| Bit Field [8]
    Short circuit protection high-side MOSFET voltage drop comparator threshold*/
    //0: function is disabled
    DISABLE_SHORT_CIRCUIT_PROTECT_HI  =  (0 << 8),
            
    //1: 750 mV <default at POR>
    ENABLE_SHORT_CIRCUIT_PROTECT_HI  =  (1 << 8),
            
    /**************************************************************************/
    
            
    /*IFAULT_LOW Comparator Threshold Adjust|| Bit Field [7]
    Short circuit protection high-side MOSFET voltage drop comparator threshold. 
    This is also used for cycle-by-cycle current limit protection
    threshold during boost function */
    //0: 135 mV <default at POR>
    ENABLE_SHORT_CIRCUIT_PROTECT_LOW_135MV  =  (0 << 7),
            
    //1: 230 mV
    ENABLE_SHORT_CIRCUIT_PROTECT_LOW_230MV  =  (1 << 7),
            
    /**************************************************************************/           
    
    
    /* LEARN enable || Bit Field [6]
    Set this bit 1 start battery learn cycle. IC turns off ACFET and turns
    on BATFET to discharge battery capacity. When battery voltage
    reaches threshold defined in bit [12;11], the BATFET is turned off
    and ACFET is turned on to finish battery learn cycle. After finished
    learn cycle, this bit is automatically reset to 0. Set this bit 0 will stop
    battery learn cycle. IC turns off BATFET and turns on ACFET*/
            
    //0: Disable LEARN Cycle <default at POR>
    DISABLE_LEARN_CYCLE  =  (0 << 6),
            
    //1: Enable LEARN Cycle
    ENABLE_LEARN_CYCLE  =  (1 << 6),
            
    /**************************************************************************/
   
            
    /*IOUT Selection || Bit Field [5]*/
    
    // 0: IOUT is the 20x adapter current amplifier output <default at POR>
    IOUT_ADAPTER_SEL  =  (0 << 5),
            
    // 1: IOUT is the 20x charge current amplifier output       
    IOUT_CHARGE_SEL  =  (1 << 5),        
    
    /**************************************************************************/            
    
    /*BOOST ENABLE || Bit Field [3]*/
    
    // 0: Disable Turbo Boost function <default at POR>
    DISABLE_BOOST  =  (0 << 3),      
    // 1: Enable Turbo Boost function
    ENALBLE_BOOST  =  (1 << 3),      
            
    /**************************************************************************/
                    
    
    /*ACOC Threshold Adjust || Bit Field [1]*/
            
    // 0: function is disabled    
    ACOC_THRESHOLD_DISABLE  =  (0 << 1),
            
    // 1: 3.33x of input current regulation limit <default at POR>
    ACOC_THRESHOLD_333X  =  (1 << 1), 
            
    /**************************************************************************/
            
    /* Charge Inhibit || Bit Field [0] */
            
    // 0: Enable Charge <default at POR>
    ENABLE_CHARGE  =  (0 << 0),
    // 1: Inhibit Charge
    INHIBIT_CHARGE  =  (1 << 0),
            
            
} E_ChargeOption;




/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /** @brief Function to initialize I2C2, used to config main/cradle charger 
     * as well as read data main/cradle smart battery
     * initializing memories before operation
     * This function should be called 1 time at start up
     *  @param [in]  None   
     *  @param [out]  None
     *  @return None
     */
    void charger_Initialize();  
    
    /** @brief set the option of charger
     *  @param [in]  uint16_t option: charger option 
     *  @param [out]  None
     *  @return None
     *  @retval true if success
     *  @retval false if fail
     */
    bool charger_SetChargeOption(E_ChargeOption option);
    
    /** @brief set the voltage to charge the battery
     *  @param [in]  uint16_t voltage: the voltage (in mV) need to be set 
     * charge voltage range from  1024mV to 19200mV, set charge voltage below 1024mV or 
     * above 19200mV clears the register and terminates charging
     *  @param [out]  void *readBuffer: pointer to store buffer
     *  @return None
     *  @retval true if success
     *  @retval false if failed
     */
    bool charger_SetChargeVoltage(uint16_t voltage);

    
    /** @brief set the current to charge the battery
      *  @param [in]  uint16_t current: the current (in mA) need to be set 
      * Charge current range from  128mA to 8064mA, set charge current below 128mA or 
      * above 8064mA clears the register and terminates charging
      *  @param [out]  None
      *  @return None
      *  @retval true if success
      *  @retval false if failed
      */
    bool charger_SetChargeCurrent(uint16_t current);

    
    /** @brief set the input current
     *  @param [in]  uint16_t current: the current (in mA) need to be set 
     * Input current range from  128mA to 8064mA, set input current below 128mA or 
     * above 8064mA clears the register and terminates charging
     *  @param [out]  None
     *  @return None
     *  @retval true if success
     *  @retval false if failed
     */
    bool charger_SetInputCurrent(uint16_t current);
    
        /** @brief set the input current
     *  @param [in]  uint16_t current: the current (in mA) need to be set 
     * Input current range from  128mA to 8064mA, set input current below 128mA or 
     * above 8064mA clears the register and terminates charging
     *  @param [out]  None
     *  @return None
     *  @retval true if success
     *  @retval false if failed
     */
    
//    bool charger_InitFirmware(void);
//    
//    bool charger_UpgradeFirmware(uint8_t* data);
//    
//    uint8_t charger_SendFirmwareDataPacket(uint8_t* data);
//     
//    void charger_HandleCommand(uint8_t * data, uint8_t command);
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _CHARGER_H */

/* *****************************************************************************
 End of File
 */
