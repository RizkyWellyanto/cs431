#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include <libpic30.h>

#include "lcd.h"
#include "led.h"
#include <time.h>

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
	lcd_printf("Daniel Hsu\nBo Liu\nRon Wright");
        Nop();

        //STEP 0: MAKE SURE THE FRIGGIN' LEDs ARE INITIALIZED TO OUTPUT MODE!!!
        CLEARBIT(LED1_TRIS);
        Nop();
        CLEARBIT(LED2_TRIS);
        Nop();
        CLEARBIT(LED3_TRIS);
        Nop();
        CLEARBIT(LED4_TRIS);
        Nop();

        SETBIT(AD1PCFGHbits.PCFG20);
        Nop();
        SETBIT(TRISEbits.TRISE8);
        Nop();

        //Button #1: PortE Pin08
        //Button #2: PortD Pin10

        int last_value = PORTEbits.RE8;
        int other_last_value = last_value;
        int counter = 0;
	while(1){
            int i, j, done;

            // Toggle LED 4
            TOGGLELED(LED4_PORT);
            Nop();
            LED1_PORT = ~PORTEbits.RE8;
            Nop();
            LED2_PORT = ~PORTDbits.RD10;
            Nop();
            LED3_PORT = PORTDbits.RD10 ^ PORTEbits.RE8;
            Nop();

            done = 0;
            while (!done)
            {
                int have_change = 0;
                for (i = 0; i < 100; ++i)
                {
                    for (j = 0; j < 100; ++j)
                    {
                        if (last_value != PORTEbits.RE8)
                        {
                            last_value = PORTEbits.RE8;
                            have_change = 1;
                            i = j = 100;
                        }
                    }
                }

                if (!have_change)
                {
                    if (last_value != other_last_value)
                    {
                        if (last_value == 0)
                        {
                            ++counter;
                            lcd_clear();
                            lcd_locate(0,0);
                            lcd_printf("Hex: 0x%X\nDec: %d\n", counter, counter);
                            Nop();
                        }
                        other_last_value = last_value;
                    }
                    done = 1;
                }
            }
	}
}
