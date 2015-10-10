/* 
 * File:   flexmotor.h
 * Author:  Bo Liu
 * Created on: Oct 9, 2015, 11:58 AM
 * Comments:
 * Revision history: 
 */

#ifndef FLEXMOTOR_H
#define	FLEXMOTOR_H
#include "types.h"

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef FCY
#define FCY 12800000UL
#endif
    
#ifndef CYCL
#define CYCL 4000 // 0.2*FCY/64 // 20ms, required by CS431
#endif
    
    
#ifndef CHANNEL_X
#define CHANNEL_X 17
#endif
    
#ifndef CHANNEL_Y
#define CHANNEL_Y 41
#endif
    
#ifndef LOW
#define LOW 0.045*CYCL // 0.9ms -> 0 degree
#endif
    
#ifndef MID
#define MID 0.075*CYCL // 1.5ms -> 90 degree
#endif

#ifndef HIGH
#define HIGH 0.105*CYCL // 2.1ms -> 180 degree
#endif
    
void motor_init(uint8_t chan);
void motor_set_duty(uint8_t chan, uint16_t duty_us);

#ifdef	__cplusplus
}
#endif

#endif	/* FLEXMOTOR_H */
