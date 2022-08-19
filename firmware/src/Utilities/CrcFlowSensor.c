/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


#include "system_definitions.h"
#include "CrcFlowSensor.h"

bool crcFlowSensor_Check(uint8_t *data, uint8_t len, uint8_t crcExpected)
{
    //calculate CRC for input data
    uint8_t crc = crcFlowSensor_Calculate(data, len);
    if(crc != crcExpected)
        return false;
    else                  
        return true;
}

uint8_t crcFlowSensor_Calculate(uint8_t *data, uint8_t len)
{
    uint8_t i = 0;
    uint8_t j = 0;
    const uint8_t POLYNOMIAL = 0x31;
    uint8_t crc = 0xFF;

    for (j=0; j<len; j++)
    {
        crc ^= *data++;
        for (i=0; i<8; i++ )
        {
            crc = ( crc & 0x80 ) ? (crc << 1) ^ POLYNOMIAL : (crc << 1);
        }
    }
    crc &= 0xFF;
    return crc;
}

/* *****************************************************************************
 End of File
 */
