/* 
 * File:   Debouncer.h
 * Author: boliu
 *
 * Created on October 1, 2015, 6:56 PM
 */

#ifndef DEBOUNCER_H
#define	DEBOUNCER_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

// Macros
#ifndef NUM_SAMPLES
#define NUM_SAMPLES       10
#endif

#define UNSTABLE          2
    
typedef struct {
    uint8_t samples[NUM_SAMPLES] = {0};
    uint8_t sampleIdx = 0;
    uint8_t debouncedValue = 0;
    uint8_t stable = 0;
} Debouncer;

extern void button_init(Debouncer * button);

extern void button_read(Debouncer * button, uint8_t Port_Reading);

extern uint8_t button_debounce(Debouncer* button);


#ifdef	__cplusplus
}
#endif

#endif	/* DEBOUNCER_H */
