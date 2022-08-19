/* 
 * File:   SpO2Data.h
 * Author: Quoc Viet
 *
 * Created on March 1, 2021, 10:32 AM
 */

#ifndef SPO2DATA_H
#define	SPO2DATA_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct
{
    uint8_t bargraph;
    uint8_t aveValue;
    uint8_t pulseRate; 
    uint8_t signal;
    uint8_t flags; 
    uint8_t insValue; 
    uint8_t plenthWave;
    bool beep;
}SP02_DATA_t;

/** @brief Initialize SpO2 Data
 * This function should be called 1 times at start up
 *  @param [in]  None   
 *  @param [out]  None
 *  @return None
 */
void SpO2Data_Inititalize(void); 

void SpO2Data_Parse(uint8_t receiveByte);
void SpO2Data_Update(void);
SP02_DATA_t SpO2Data_ReadData(void);
uint8_t SPO2Data_GetPlenthValue();
void SpO2Data_GetData(SP02_DATA_t* data);
//
bool SpO2Data_IsMonitoringSpO2Patient(void);
//
bool SpO2Data_IsMonitoringPRPatient(void);

/** @brief Query any error happen with SPO2 module
 *  @param [in]  None   
 *  @param [out] None
 *  @retval true SPO2 module has error
 *  @retval false SPO2 module is OK
 */
bool SpO2Data_IsModuleFailed();

#ifdef	__cplusplus
}
#endif

#endif	/* SPO2DATA_H */

