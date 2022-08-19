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
#include "CrcChamber.h"

#define POLYNOMIAL_CRC8    (0x07)      // 1 0000 0111 x8 + x2 + x1 + x0
#define MSB_CRC8    (0x31)      // 1 0011 0001 x8 + x5 + x4 + 1

bool crcChamber_Check(uint8_t *data, uint8_t len, uint8_t crcExpected)
{
    //calculate CRC for input data
    uint8_t crc = crcChamber_Calculate(data, len);
    if(crc != crcExpected)
        return false;
    else                  
        return true;
}

uint8_t crcChamber_Calculate(uint8_t *buff, uint8_t len)
{
  uint8_t *p = (uint8_t *)buff;
  uint8_t crc8;

  for ( crc8 = 0x00 ; len != 0 ; len-- )
  {
    crc8 ^= *p++;

    uint8_t i;
    for (i = 0 ; i < 8 ; i++ )
    {
      if ( crc8 & 0x80 )
      {
        crc8 <<= 1; crc8 ^= MSB_CRC8;
      }
      else
      {
        crc8 <<= 1;
      }
    }
  }
  return crc8;
}


/* *****************************************************************************
 End of File
 */

