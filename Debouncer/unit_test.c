#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#define NUM_SAMPLES 10
#include <stdio.h>
#include <libpic30.h>

#include "lcd.h"
#include "Debouncer.h"

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);


void main(){
	//Init LCD
	__C30_UART=1;	
	lcd_initialize();
	lcd_clear();
	lcd_locate(0,0);
	lcd_printf("Hello World!");	
    
    uint16_t loopCounter = 0;
    uint8_t pressCounter = 0;
    Debouncer button1;
	
	while(1){
        ++loopCounter;
        
		if (loopCounter == 5000) // samples the button status every 2k loops, TODO: use real timer
        {
            loopCounter = 0;
            
            button_read(&button1, PORTEbits.RE8);
            
            if(button_debounced(&button1) != UNSTABLE)
            {
                ++pressCounter;
                lcd_printf("button 1 has been pressed:\n");
                lcd_printf("%d times\n", pressCounter);
            }
        }
        
        
	}
}

