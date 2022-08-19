/* 
 * File:   ChamberUnit.h
 * Author: Quoc Viet
 *
 * Created on March 9, 2021, 2:11 PM
 */

#ifndef CHAMBERUNIT_H
#define	CHAMBERUNIT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef	__cplusplus
extern "C" {
#endif

//typedef union{
//    uint64_t chamberSerial;
//    float pumpFreq;
//    uint64_t breathCircuitMafCode;
//    uint64_t breathCircuitSerial;
//    uint64_t breathCircuitFacCode;
//    uint64_t breathCircuitStartUseDay;
//
//}DATA_WRITE_TO_CHAMBER;
    
typedef enum{
    eTankIsDisconnected = 0x55,
    eTankIsNotFull      = 0x01,
    eTankIsFull         = 0x10,
    eTankIsUnknow       = 0xFF,
}E_TankWaterLevel;

/** @brief List of the type of Breathing Circuit */
typedef enum
{
    eTypePediatric,
    eTypeAdult,
    eTypeDisinfection,
    eTypeOther,        
} E_BreathingCircuitType;

typedef enum
{  
    eChamberDisconnected,
    eChamberConnected,
}E_ChamberConnectState;


void Chamber_Initialize();

/** @brief Function to Get Chamber SN
 *  @param [out]  uint8_t *sn : pointer point to storage reading value
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_GetChamberSN(uint8_t *sn);

/** @brief Function to Get Chamber Out temp
 *  @param [out]  None
 *  @retval float chamber outlet temperature
 */
    float Chamber_GetChamberOutTemp(void);

/** @brief Function to Get EVT temp
 *  @param [out]  None
 *  @retval float EVT temperature
 */
    float Chamber_GetEVTTemp(void);

/** @brief Function to Get usage time of chamber
 *  @param [out] None
 *  @retval uint32_t chamber usage time
 */
uint32_t Chamber_GetChamberUsedTime(void);

/** @brief Function to Get Chamber FW version
 *  @param [out]  float *rawFW : pointer point to storage reading value
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_GetChamberFWVer(uint8_t *fwVer);


/** @brief Function to Get Tank water level
 *  @param [out] None
 *  @retval E_TankWaterLevel
 */
E_TankWaterLevel Chamber_GetTankWaterLevel(void);

/** @brief Function to Get Breathing circuit manufacture code
 *  @param [out] None
 *  @retval uint8_t manufacture code
 */
uint8_t Chamber_GetBreathingCircuitManufactureCode();

/** @brief Function to Get Breathing circuit serial number
 *  @param [out]  float *sn : pointer point to storage reading value
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_GetBreathingCircuitSerialNumber(uint8_t* sn);

/** @brief Function to Get Breathing Circuit manufacture date
 *  @param [out]  None
 *  @retval uint32_t manufacture date(format:YYYYMMDD,Ex: Date(Hex) : 0x143A52D-> Date(Dec) : 21210413-> Date(Time): 2021/4/13)
 */
uint32_t Chamber_GetBreathingCircuitManufactureDate();

/** @brief Function to Get Breathing Circuit factory code
 *  @param [out] None
 *  @retval uint8_t Factory code : 01 (Metran JP),  02 (Vitec VN)
 */
uint8_t Chamber_GetBreathingCircuitFactoryCode(void);

/** @brief Function to Get Breathing Circuit start used date
 *  @param [out] None
 *  @retval uint32_t start use date(format:YYYYMMDD,Ex: Date(Hex) : 0x143A52D-> Date(Dec) : 21210413-> Date(Time): 2021/4/13)
 */
uint32_t Chamber_GetBreathingCircuitStartUsedDay(void);

/** @brief Function to Get Breathing Circuit cycle count
 *  @param [out] None
 *  @retval uint16_t cycle count
 */

uint16_t Chamber_GetBreathingCircuitCycleCount(void);

/** @brief Function to Get Breathing Circuit used time (in hour)
 *  @param [out] None
 *  @retval uint16_t usage time 
 */
uint16_t Chamber_GetBreathingCircuitUsedTime(void);

/** @brief Function to Get Breathing Circuit outlet temperature
 *  @param [out] None
 *  @retval float breathing circuit outlet temperature
 */
float Chamber_GetBreathingCircuitTemperature(void);


/** @brief Function to Get breathing circuit type
 *  @param [out] None
 *  @retval E_BreathingCircuitType: Type = 'P' :For Kid breathing circuit, A' :For Adult breathing circuit, 'S' :For Sterilization circuit
 */
E_BreathingCircuitType Chamber_GetBreathingCircuitType();


/** @brief Function to get connection status of breathing circuit
 *  @param [in]  None 
 *  @param [out]  None
 *  @retval true if connect, false if disconnect
 */
bool chamber_GetBreathingCircuitConnection();


/** @brief Function to get connection status of chamber
 *  @param [in]  None 
 *  @param [out]  None
 *  @retval E_ConnectState connection status of chamber
 */
E_ChamberConnectState chamber_GetChamberConnection();


/** @brief Function to Set Chamber SN
 *  @param [in]  uint8_t sn : pointer to array contains serial data
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetChamberSN(uint8_t *sn);

/** @brief Function to Set Water pump control Frequency
 *  @param [in]  float value : the value will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetPumpFreq(float setFreq);

/** @brief Function to Reset used time of chamber
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true if success
 *  @retval false if failed  
 */
bool Chamber_ResetChamberUsedTime(void);

/** @brief Function to Reset all chamber data
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true if success
 *  @retval false if failed  
 */
bool Chamber_ResetChamberAll(void);

/** @brief Function to Set Breath circuit manufacture code
 *  @param [in]  uint32_t code : the value will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitManufactureCode(uint32_t code);

/** @brief Function to Set Breath circuit serial number
 *  @param [in]  uint8_t sn : pointer to array contains serial data
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitSerialNumber(uint8_t* sn);

/** @brief Function to Set Breath circuit factory code
 *  @param [in]  uint8_t code : the value will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitFactoryCode(uint8_t code);

/** @brief Function to Set Breath circuit start used day
 *  @param [in]  uint32_t date : the value will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitStartUsedDay(uint32_t date);

/** @brief Function to Reset breathing circuit cycle count
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_ResetBreathingCircuitCycleCount(void);

/** @brief Function to Reset breathing circuit used time
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_ResetBreathingCircuitUsedTime(void);

/** @brief Function to Reset all breathing circuit data
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_ResetBreathingCircuitAll(void);

/** @brief Function to set Breath circuit manufacture date
 *  @param [in]  uint32_t date : the value will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitManufactureDate(uint32_t date);
/** @brief Function to set Breath circuit use time
 *  @param [in]  uint16_t time : the time will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitUseTime(uint16_t time);

/** @brief Function to set Breath circuit type
 *  @param [in]  E_BreathingCircuitType type : type will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitType(E_BreathingCircuitType type);

/** @brief Function to set Breath circuit cycle count
 *  @param [in]  uint16_t count : count will be set
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_SetBreathCircuitCycleCount(uint16_t count);

/** @brief Function to Reset all breathing circuit data
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_InitFirmware(void);


/** @brief Function to Reset all breathing circuit data
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true write data success
 *  @retval false write data failed  
 */
bool Chamber_UpgradeFirmware(uint8_t* data, uint32_t datalen);

void Chamber_Run();

/** @brief Function to get status update (success or failed)
 *  @param [in]  None
 *  @param [out]  None
 *  @retval true update data success
 *  @retval false update data failed  
 */
bool Chamber_IsUpdateSuccess();

#ifdef	__cplusplus
}
#endif

#endif	/* CHAMBERUNIT_H */

