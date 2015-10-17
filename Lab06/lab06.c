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

uint16_t flag = 1;

// Timer 1
void init_timer1()
{
   CLEARBIT(T1CONbits.TON);   // Disable Timer
   CLEARBIT(T1CONbits.TCS);   //Select internal instruction cycle clock (12.8 MHz)
   CLEARBIT(T1CONbits.TGATE); //Disable Gated Timer mode
   TMR1 = 0x00;               //Clear Timer Register
   T1CONbits.TCKPS = 0b10;    //Set Prescaler (1:256)
   PR1 = 2000;                //Set Period
   IPC0bits.T1IP = 0x01;      //Set IPL
   CLEARBIT(IEC0bits.T1IE);     //Enable INT
   CLEARBIT(IFS0bits.T1IF);   //Clear IF
   SETBIT(T1CONbits.TON);     //Enable Timer
}

// T1 interrupt every 10 ms.
void
__attribute__ (( __interrupt__, auto_psv)) _T1Interrupt(void)
{
   flag = 1;
   CLEARBIT(IFS0bits.T1IF);
}

void wait()
{
    flag = 1;
    init_timer1();
    while(!flag); // spins
    flag = 0;
}


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
    uint16_t loopCounter = 0;

    init_adc1();
    touch_init();
    init_timer1();

    while(1){

        if(loopCounter++ >= 2000) {
            loopCounter = 0;
        }

        wait();

        // sample X
        //AD1CHS0bits.CH0SA = 0x00F; // X pin AN15
        //touch_select_dim(DIM_X);

        for (i = 0; i < NUM_SAMPLES; ++i) {
            wait();
            samples[i] = touch_read(DIM_X);
        }

        if(loopCounter == 2000) {
            //lcd_clear_row(1);
            lcd_locate(0, 1);
            lcd_printf("X = %u        ", find_median(samples, 5));
        }

        wait();
        
        // sample Y
        //AD1CHS0bits.CH0SA = 0x009; // Y pin AN9
        //touch_select_dim(DIM_Y);

        for (i = 0; i < NUM_SAMPLES; ++i) {
            wait();
            samples[i] = touch_read(DIM_Y);
        }

        if(loopCounter == 2000) {
            lcd_locate(0, 2);
            lcd_printf("Y = %u        ", find_median(samples, 5));
        }
    }

    return 0;
}
