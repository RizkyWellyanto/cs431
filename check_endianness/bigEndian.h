/* 
 * File:   bigEndian.h
 * Author: boliu1
 *
 * Created on September 18, 2015, 1:46 PM
 */

#ifndef ENDIANNESS_CHECK_H
#define	ENDIANNESS_CHECK_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "types.h"

uint16_t bigEndian() {
    uint16_t number = 0x0001;
    uint16_t MSB = number >> 8;
    return MSB;
}

#ifdef	__cplusplus
}
#endif

#endif	/* ENDIANNESS_CHECK_H */

