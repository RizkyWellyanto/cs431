#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL
#define NUM_SAMPLES 10
#include <stdio.h>
#include <libpic30.h>

#include "lcd.h"
#include "Debouncer.h"

#define TIMER_INTERVEL 0x8fff

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

int main(){
    //Init LCD
    __C30_UART=1;
    lcd_initialize();
    lcd_clear();
    lcd_locate(0,0);

    initTimer3();

    SETBIT(TRISEbits.TRISE8); // set trigger as input
    SETBIT(AD1PCFGHbits.PCFG20);
    SETBIT(TRISDbits.TRISD10);

    uint16_t loopCounter = 0;
    // uint8_t pressCounter_loop = 0;
    uint8_t pressCounter_timer = 0;
    Debouncer button1;
    uint8_t buttonStatus1;
    button_init(&button1);

    while(1){
        ++loopCounter;
   
        // E.g.: using loopCounter:
        if (loopCounter%2000 == 0) //
        {

            loopCounter = 0;
            lcd_locate(0,0);
             /* Debugging info starts */
            lcd_printf("LC = %u\n", loopCounter);


            lcd_locate(0,1);
            lcd_printf("%d%d%d%d\n",
                (button1.samples)[0],(button1.samples)[1],
                (button1.samples)[2],(button1.samples)[3]);

            lcd_locate(0,2);
            lcd_printf("%x", TMR3);
            /* Debugging info ends */

            button_read(&button1, PORTEbits.RE8);
            buttonStatus1 = button_debounced(&button1);
            if (buttonStatus1 != UNSTABLE && buttonStatus1 != UNCHANGED)
            {
                if (buttonStatus1 == 0 )
                {
                    ++pressCounter_timer;
                    lcd_locate(0,5);
                    lcd_printf("Clicked: %u\n", pressCounter_timer);
                }
                else
                {

                }
            }
        }

        // E.g. using timer:
        if ((uint16_t)TMR3 >= TIMER_INTERVEL) //
        {
            TMR3 = 0x00;

            button_read(&button1, PORTEbits.RE8);
            buttonStatus1 = button_debounced(&button1);
            if (buttonStatus1 != UNSTABLE && buttonStatus1 != UNCHANGED)
            {
                if (buttonStatus1 == 0 )
                {
                    ++pressCounter_timer;
                    lcd_locate(0,6);
                    lcd_printf("Clicked: %u\n", pressCounter_timer);
                }
                else
                {
                	
                }
            }
        }
    }

    return 0;
}
