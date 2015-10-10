#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#define NUM_SAMPLES 10
#include <stdio.h>
#include <libpic30.h>

#include "lcd.h"
#include "Debouncer.h"

#define CLK 12800000
#define TIMER_INTERVEL 0.001 * CLK 

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);

void initTimer3() {
    CLEARBIT(T3CONbits.TON); // Disable Timer
    CLEARBIT(T3CONbits.TCS); // Select internal instruction cycle clock
    CLEARBIT(T1CONbits.TGATE); // Disable gated timer mode
    
    TMR3 = 0; // clear register
    
    SETBIT(T3CONbits.TON); // turn on Timer3
}

void main(){
    //Init LCD
    __C30_UART=1;	
    lcd_initialize();
    lcd_clear();
    lcd_locate(0,0);
    lcd_printf("Hello World!");	
    
    initTimer3();
    
    uint16_t loopCounter = 0;
    uint8_t pressCounter_loop = 0;
    uint8_t pressCounter_timer = 0;
    Debouncer button1;
	
    while(1){
        ++loopCounter;
        
        if (loopCounter == 5000) // samples the button status every 2k loops
        {
            loopCounter = 0;
            
            button_read(&button1, PORTEbits.RE8);
            if (button_debounce(&button1) != UNSTABLE)
            {
                if (button_debounce(&button1) == 0 )
                {
                	++pressCounter_loop;
                	lcd_printf("Using loop count:\rbutton 1 has been pressed: %d times\n", pressCounter_loop);
                }
                else
                {
                	// nothing to do
                }
            }
        }
        
        if (TMR3 == TIMER_INTERVEL) // samples the button status every 2k loops
        {
            TMR3 = 0;
            
            button_read(&button1, PORTEbits.RE8);
            if (button_debounce(&button1) != UNSTABLE)
            {
                if (button_debounce(&button1) == 0 )
                {
                	++pressCounter_timer;
                	lcd_locate(3,0);
                	lcd_printf("Using timer:\r button 1 has been pressed: %d times\n", pressCounter_timer);
                }
                else
                {
                	// nothing to do
                }
            }
        }
    }
}

