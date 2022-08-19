/** @file crc.h
 *  @brief Provide interface to init crc, caculator crc and check crc
 *  @author Viet Le
 */

#ifndef CRC_H
#define	CRC_H

#include "stdint.h"

/** @brief Define start value */
#define CRC16_START_VAL        (0x1D0F)

//function to check CRC without initial value
unsigned short crc_CheckNoInit(long nBytes,int8_t *pData);

//function to check CRC with initial value
unsigned short crc_CheckWithInit(unsigned short initValue, long nBytes,int8_t *pData);

//function to check CRC with initial value for image and font
unsigned short crc_crc16ccitt(unsigned short start, int len, const void *buf);

#endif	/* CRC_H */

/* end of file */