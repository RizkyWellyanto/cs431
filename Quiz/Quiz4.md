2.(20Points) DAC/ADC questions

a.(10 points) Compute the digital value you need to generate 3.1 volts assuming
your Digital to Analog Converter has 12 bit resolution and a range of -2 to +8
volts.

*Answer:*

$V_{Digital} = \frac{V_{Analog} - V_{min}}{V_{max} - V_{min}} \times Range$

$V_{Digital} = \frac{3.1 + 2}{10} \times (2^{12}-1) = 2088.45$

So, the digital value may be 2088. $\Box$


b.(10 points) The Analog to Digital Converter(ADC) reads 956. What is the
corresponding voltage assuming the ADC has 12 bit resolution and a range of -5
to +3 volts?

*Answer:*

$V_{Digital} = \frac{V_{Analog} - V_{min}}{V_{max} - V_{min}} \times Range$

$\rightarrow V_{Analog} = \frac{956 \times 8}{2^{12}-1} - (-5) = -3.1 v$


Bonus Question

3.(10 points) Write a C program to determine the endianness(big or little
endian) on an embedded system ?

```c
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
```

```c
/*
 * File:   main.c
 * Author: boliu1
 *
 * Created on September 18, 2015, 1:49 PM
 */
#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL
#include <stdio.h>
#include <libpic30.h>

#include "lcd.h"
#include "bigEndian.h"

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT);

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);


int main(){
	//Init LCD
	__C30_UART=1;
	lcd_initialize();
	lcd_clear();
	lcd_locate(0,0);
        if(bigEndian()) {
            lcd_printf("Big Endian System!");
        } else {
            lcd_printf("Little Endian System!");
        }

	while(1){

	}

        return 0;
}
```
