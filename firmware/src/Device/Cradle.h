/* ************************************************************************** */
/** @file [Cradle.h]
 *  @brief {support interface to communicate with Cradle to get 
 * information, and control, report error if occur during communication}
 *  @author {nguyen truong}
 */
/* ************************************************************************** */




#ifndef _CRADLE_H    /* Guard against multiple inclusion */
#define _CRADLE_H


/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef enum
{  
    eDisconnect,
    eConnect,
}E_ConnectState;


typedef enum
{
    eOff = 0x00,
    eOn = 0x01,
}E_OnOffState;


/** @brief Initialize the cradle unit
 * This function should be called before RTOS is started and call 1 time only after
 * start up system
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void cradle_Initialize();


/** @brief Function to get connection status of cradle
 *  @param [in]  None 
 *  @param [out]  None
 *  @retval E_ConnectState connection status of cradle
 */
E_ConnectState cradle_GetCradleConnection();

/** @brief Function to get battery connection status
 *  @param [in]  None 
 *  @param [out]  None
 *  @retval E_ConnectState connection pointer point to place where stored connection status
 */
E_ConnectState cradle_GetBatteryConnection();

/** @brief Function to get battery remaining percent(%)
 *  @param [out] None
 *  @retval uint16_t remaining percent of battery
 */
uint16_t cradle_GetBatteryRemainingPercent();

/** @brief Function to get battery remaining capacity(mAh)
 *  @param [in]  None 
 *  @param [out] None
 *  @retval uint16_t remaining capacity of battery
 */
uint16_t cradle_GetBatteryRemainingCapacity();

/** @brief Function to get battery status
 *  @param [in]  None
 *  @param [out]  None
 *  @retval uint16_t status of battery
 */
uint16_t cradle_GetBatteryStatus();

/** @brief Function to get battery average current
 *  @param [in]  None
 *  @param [out]  None
 *  @retval uint16_t average current of battery
 */
int16_t cradle_GetBatteryAverageCurrent();

/** @brief Function to get battery voltage
 *  @param [in]  None
 *  @param [out]  None
 *  @retval uint16_t voltage of battery
 */
uint16_t cradle_GetBatteryVoltage();

/** @brief Function to Get Cradle SN
 *  @param [out]  uint8_t *sn : pointer point to storage reading value
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool cradle_GetCradleSN(uint8_t *sn);

/** @brief Function to Get Cradle firmware version
 *  @param [out]  float *fwVer : pointer point to storage reading value
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Cradle_GetCradleFirmwareVersion(uint8_t *fwVer);

/** @brief Function to set Water Supply ON/OFF
 *  @param [in]  E_OnOffState state: state need to set
 *  @retval true if write data success
 *  @retval false if write data failed  
 */
bool cradle_SetWaterSupplyOnOff(E_OnOffState state);

/** @brief Function to set Power ON/OFF
 *  @param [in]  E_OnOffState state: state need to set
 *  @retval true if write data success
 *  @retval false if write data failed  
 */
bool cradle_SetPowerSupplyOnOff(E_OnOffState state);

/** @brief Function to Set Cradle SN
 *  @param [in]  uint8_t sn : pointer to array contains serial data
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Cradle_SetCradleSN(uint8_t *sn);

/** @brief Get current status of cradle communication
*  @param [in]  None 
*  @param [out]  None
*  @return bool
*  @retval true if error, false if not error
*/
bool cradle_IsCommunicationFailure();

void cradle_Run();

bool cradleFirmwareUpdate_InitFirmware(void);

bool Cradle_UpgradeFirmware(uint8_t* data, uint32_t datalen);

bool Hi_TestFunction(void);

/** @brief Function to get status update (success or failed)
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true update data success
 *  @retval false update data failed  
 */
bool Cradle_IsUpdateSuccess();
/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif




    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _CRADLE_H */

/* *****************************************************************************
 End of File
 */
