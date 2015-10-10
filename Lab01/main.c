#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include <libpic30.h>
#include "led.h"
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
    
    // set up I/O
    AD1PCFGHbits.PCFG20 = 1;
    TRISEbits.TRISE8 = 1;
    TRISDbits.TRISD10 = 1;
    
    Debouncer button1; // PORTEbits.RE8
    
    // counters
    uint16_t loopCounter = 0;
    uint8_t pressedCounter = 0;
    
    CLEARLED(LED4_TRIS);
    CLEARLED(LED1_TRIS);
    CLEARLED(LED2_TRIS);
    CLEARLED(LED3_TRIS);
	
    // Obj.1
    lcd_printf("Bo Liu \r Daniel Hsu \r Ron Wright \n");
    
	while(1){
        
        // Obj. 2
        if(!(loopCounter % 50000)){
            TOGGLELED(LED4_PORT);
            loopCounter = 0;
        }
        
        // Obj. 3 trigger button LED, debouncing not needed
        if(!(PORTEbits.RE8)){ // button pressed: pull-up resistor
            SETLED(LED1_PORT);
        } else {
            CLEARLED(LED1_PORT);
        }
        
        // Obj. 4 thumb button LED, deboucing not needed
        if(!(PORTDbits.RD10)){ 
            SETLED(LED2_PORT);
        } else {
            CLEARLED(LED2_PORT);
        }
        
        // Obj. 5
        if(!(PORTEbits.RE8 ^ PORTDbits.RD10)){
            SETLED(LED3_PORT);
        } else {
            CLEARLED(LED3_PORT);
        }
        
        // Obj. 6 Debounced counter
        if(!(loopCounter % 5000)){
            button_read(&button1, PORTEbits.RE8);
            if (button_debounce(&button1) != UNSTABLE)
            {
                if (button_debounce(&button1) == 0 ) 
                {
                	++pressedCounter;
                    lcd_locate(5,10);
                    lcd_printf("Button pressed: %d 0x%x\n", pressedCounter, pressedCounter);
                }
                else
                {
                	// nothing to do
                }
            }
        }   
        loopCounter++;        
	}
}
