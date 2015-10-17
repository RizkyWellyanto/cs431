#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL
#include <stdio.h>
#include <libpic30.h>

#include <time.h>
#include "lcd.h"
#include "flextouch.h"
#include "algorithm.h"

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

	lcd_printf("Lab06");

    const uint16_t NUM_SAMPLES = 5;
    uint16_t samples[NUM_SAMPLES];
    uint16_t i = 0;

    init_adc1();
    touch_init();

    while(1){

        // sample X
        touch_select_dim(DIM_X);

        for (i = 0; i < NUM_SAMPLES; ++i) {
            Delay_ms(2);
            samples[i] = touch_adc();
        }

        
        lcd_locate(0, 1);
        lcd_printf("X = %u        ", find_median(samples, NUM_SAMPLES));
        
        // sample Y
        touch_select_dim(DIM_Y);

        for (i = 0; i < NUM_SAMPLES; ++i) {
            Delay_ms(2);
            samples[i] = touch_adc();
        }

        lcd_locate(0, 2);
        lcd_printf("Y = %u        ", find_median(samples, NUM_SAMPLES));
    }

    return 0;
}
