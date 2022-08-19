/* ************************************************************************** */
/** @file [Charger.c]
 *  @brief {Supply APIs for the control of battery charging, including configure
 * charge option, charge voltage, charge current and input current}
 *  @author {nguyen truong}
 */
/* ************************************************************************** */


/* This section lists the other files that are included in this file.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>

#include "FreeRTOS.h"
#include "semphr.h"

#include "system_config.h"
#include "system_definitions.h"
//#include "driver/i2c/drv_i2c.h"

#include "Charger.h"
#include "I2C_2.h"
#include "Delay.h"
//#include "DigitalOutput.h"
//#include "AlarmInterface.h"
//#include "ApplicationDefinition.h"



/** @brief I2C slave address of charger, charger is bq24735 from Texas Instrument, that has I2C address is 0x12H*/
#define CHARGER_BASE_ADDR                 (0x12 >> 1)

/** @brief I2C address while writing data to charger
 * write address = base address << 1 + write bit (0))
 */
#define CHARGER_WRITE_ADDR                (CHARGER_BASE_ADDR << 1)

/** @brief I2C address while reading data from charger
 * read address = base address << 1 + read bit(1))
 */
#define CHARGER_READ_ADDR                 ((CHARGER_BASE_ADDR << 1) + 1)

/** @brief max time wait for charger communication (in ms) via I2C*/
#define CHARGER_COMM_MAX_WAIT_MS          (40 / portTICK_PERIOD_MS)

/** @brief max time wait for MUTEX to access protection resource */
#define CHARGER_MUTEX_MAX_WAIT_MS         (2 / portTICK_PERIOD_MS)       




/** @brief address of DeviceID register */
#define DEVICE_ID_REGISTER_ADDR           0xFF

/** @brief address of ManufacturerID register */
#define MANUFACTURE_ID_REGISTER_ADDR      0xFE

/** @brief address of ChargeOption register */
#define CHARGE_OPTION_REGISTER_ADDR       0x12

/** @brief address of Charge current register */
#define CHARGE_CURRENT_REGISTER_ADDR      0x14

/** @brief address of Charge voltage register */
#define CHARGE_VOLTAGE_REGISTER_ADDR      0x15

/** @brief address of input current register */
#define INPUT_CURRENT_REGISTER_ADDR       0x3F



/** @brief the valuable bit of 16 bit charge voltage register is just only from bit4 to bit14 */
#define CHARGE_VOLT_VALUABLE_BIT          0x7FF0
/** @brief the valuable bit of 16 bit charge voltage register is just only from bit6 to bit12 */
#define CHARGE_CURRENT_VALUABLE_BIT       0x1FC0
/** @brief the valuable bit of 16 bit charge voltage register is just only from bit7 to bit12 */
#define INPUT_CURRENT_VALUABLE_BIT        0x1F80



/** @brief Function to read value of register 
 *  @param [in]  uint8_t addrReg :address of register   
 *  @param [out]  uint16_t *readValue : pointer point to storage reading value
 *  @retval true read data success
 *  @retval false read data failed  
 */
static bool charger_ReadRegisterValue(uint8_t addrReg, uint16_t *readValue)
{
    uint8_t buffWrite[1] = {addrReg};
    uint8_t buffRead[2] = {'\0'};


    if (I2C2_Write(CHARGER_WRITE_ADDR, 
              (void*)buffWrite, sizeof(buffWrite), CHARGER_COMM_MAX_WAIT_MS) == false)
    {
        //SYS_PRINT("I2C_2 Write Fail\n");
        return false;
    }
    else
    {         
        if(I2C2_Read(CHARGER_READ_ADDR, 
                (void*)buffRead, sizeof(buffRead), CHARGER_COMM_MAX_WAIT_MS) == false)
        {            
            //SYS_PRINT("I2C_2 Read Fail\n");
            return false;
        }
        else
        {
            *readValue = buffRead[0] + buffRead[1] << 8;
            //SYS_PRINT("I2C_2 Read value: %.4X\n", value);
            return true;
        }
    }
}



/** @brief Function to write value to register 
 *  @param [in]  uint8_t addrReg :address of register   
 *  @param [out]  uint16_t value : the value will be written
 *  @retval true write data success
 *  @retval false write data failed  
 */
static bool charger_WriteRegisterValue(uint8_t addrReg, uint16_t value)
{
    uint8_t buffWrite[3];
    //byte address register
    buffWrite[0] = addrReg;
    //Low Byte
    buffWrite[1] = value & 0xFF;
    //High Byte
    buffWrite[2] = (value >> 8) & 0xFF;
    if (I2C2_Write(CHARGER_WRITE_ADDR, 
          (void*)buffWrite, sizeof(buffWrite), CHARGER_COMM_MAX_WAIT_MS) == false)
    {
        //SYS_PRINT("I2C_2 Write Fail\n");
        return false;
    }
    else
        return true;
    
}







/** @brief set the option of charger
 *  @param [in]  uint16_t option: charger option 
 *  @param [out]  None
 *  @return None
 *  @retval true if success
 *  @retval false if fail
 */
bool charger_SetChargeOption(E_ChargeOption option)
{
    return charger_WriteRegisterValue(CHARGE_OPTION_REGISTER_ADDR, option);
}


/** @brief set the voltage to charge the battery
 *  @param [in]  uint16_t voltage: the voltage (in mV) need to be set 
 * charge voltage range from  1024mV to 19200mV, set charge voltage below 1024mV or 
 * above 19200mV clears the register and terminates charging
 *  @param [out]  void *readBuffer: pointer to store buffer
 *  @return None
 *  @retval true if success
 *  @retval false if failed
 */
bool charger_SetChargeVoltage(uint16_t voltage)
{
    return charger_WriteRegisterValue(CHARGE_VOLTAGE_REGISTER_ADDR, voltage & CHARGE_VOLT_VALUABLE_BIT);
}


/** @brief set the current to charge the battery
  *  @param [in]  uint16_t current: the current (in mA) need to be set 
  * Charge current range from  128mA to 8064mA, set charge current below 128mA or 
  * above 8064mA clears the register and terminates charging
  *  @param [out]  None
  *  @return None
  *  @retval true if success
  *  @retval false if failed
  */
bool charger_SetChargeCurrent(uint16_t current)
{ 
    return charger_WriteRegisterValue(CHARGE_CURRENT_REGISTER_ADDR, current & CHARGE_CURRENT_VALUABLE_BIT);
}


 /** @brief set the input current
  *  @param [in]  uint16_t current: the current (in mA) need to be set 
  * Input current range from  128mA to 8064mA, set input current below 128mA or 
  * above 8064mA clears the register and terminates charging
  *  @param [out]  None
  *  @return None
  *  @retval true if success
  *  @retval false if failed
  */
bool charger_SetInputCurrent(uint16_t current)
{ 
    return charger_WriteRegisterValue(INPUT_CURRENT_REGISTER_ADDR, current & INPUT_CURRENT_VALUABLE_BIT);
}


