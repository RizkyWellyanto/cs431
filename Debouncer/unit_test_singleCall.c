#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL
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
    uint8_t pressCounter_timer = 0;
    Debouncer button1;
    uint8_t buttonStatus1;
    button_init(&button1);

    while(1){
        if (loopCounter++ >= 2000)
            loopCounter = 0;
            
        if (loopCounter == 2000) //
        {
            lcd_locate(0,0);
             /* Debugging info starts */
            lcd_printf("LC = %u\n", loopCounter);

            lcd_locate(0,1);
            lcd_printf("%d%d%d%d\n",
                (button1.samples)[0],(button1.samples)[1],
                (button1.samples)[2],(button1.samples)[3]);
        }

        button_sample(&button1, PORTEbits.RE8);
        buttonStatus1 = button_debounced(&button1);
        if (buttonStatus1 != UNSTABLE && buttonStatus1 != UNCHANGED)
        {
            if (buttonStatus1 == 0 )
            {
                ++pressCounter_timer;
                
                if (loopCounter == 2000)
                {
                    lcd_locate(0,5);
                    lcd_printf("Clicked: %u\n", pressCounter_timer);
                }
            }
            else
            {
                // do nothing
            }
        }
    }
    return 0;
}
