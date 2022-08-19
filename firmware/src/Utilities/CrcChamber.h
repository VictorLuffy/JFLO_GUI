/* 
 * File:   CrcChamber.h
 * Author: Quoc Viet
 *
 * Created on March 9, 2021, 3:46 PM
 */

#ifndef CRCCHAMBER_H
#define	CRCCHAMBER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#ifdef	__cplusplus
extern "C" {
#endif

    //function to check CRC for input data, 
    //return true if CRC good
    //return false if CRC failed
bool crcChamber_Check(uint8_t *data, uint8_t len, uint8_t crcCheck);

    //function to calcuate CRC for input data, 
    //return the CRC result calculated 
uint8_t crcChamber_Calculate( uint8_t *buff, uint8_t len );


#ifdef	__cplusplus
}
#endif

#endif	/* CRCCHAMBER_H */

