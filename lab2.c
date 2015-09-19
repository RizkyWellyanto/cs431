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


volatile unsigned int counter = 0;

void __attribute__((__interrupt__)) _T1Interrupt(void)
{
    TOGGLELED(LED2_PORT);
    IFS0bits.T1IF = 0;
}

void __attribute__((__interrupt__)) _T2Interrupt(void)
{
    counter += 2;
    TOGGLELED(LED1_PORT);
    IFS0bits.T2IF = 0;
}

void __attribute__((__interrupt__)) _INT1Interrupt(void)
{
    counter = 0;
    IFS1bits.INT1IF = 0;
}

void main(){
	//Init LCD
	__C30_UART=1;	
	lcd_initialize();
	lcd_clear();
	//lcd_locate(0,0);
	//lcd_printf("Daniel Hsu\nBo Liu\nRon Wright");
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

        /*

         PV * Prescaler / 12.8e6 = Period in seconds

         PV = Period in seconds * 12.8e6 / Prescaler
            = 0.002 * 12.8e6 / 256 = 100

         */

        // TIMER 2
        CLEARBIT(T2CONbits.TON);
        CLEARBIT(T2CONbits.TCS);
        CLEARBIT(T2CONbits.TGATE);
        TMR2 = 0x00;
        T2CONbits.TCKPS = 0b11; // 1:256 prescaler
        PR2 = 100;
        IPC1bits.T2IP = 0x02;
        CLEARBIT(IFS0bits.T2IF);
        SETBIT(IEC0bits.T2IE);
        SETBIT(T2CONbits.TON);

        // TIMER 1
        //enable LP0SCEN
        __builtin_write_OSCCONL(OSCCONL | 2);
        T1CONbits.TON = 0;
        T1CONbits.TCS = 1;
        T1CONbits.TSYNC = 0;
        T1CONbits.TCKPS = 0b00; // 1:1 prescaler
        TMR1 = 0x00;
        PR1 = 32767;
        IPC0bits.T1IP = 0x01;
        IFS0bits.T1IF = 0;
        IEC0bits.T1IE = 1;
        T1CONbits.TON = 1;

        // TIMER 3
        CLEARBIT(T3CONbits.TON);
        CLEARBIT(T3CONbits.TCS);
        CLEARBIT(T3CONbits.TGATE);
        TMR3 = 0x00;
        T3CONbits.TCKPS = 0b00; // 1:1 prescaler
        PR3 = 1;
        IPC2bits.T3IP = 0x02;
        CLEARBIT(IFS0bits.T3IF);
        CLEARBIT(IEC0bits.T3IE);
        SETBIT(T3CONbits.TON);


        AD1PCFGHbits.PCFG20 = 1;
        TRISEbits.TRISE8 = 1;
        IEC1bits.INT1IE = 1;
        IPC5bits.INT1IP = 0x03;
        IFS1bits.INT1IF = 0;
        INTCON2bits.INT1EP = 1;

        //Button #1: PortE Pin08
        //Button #2: PortD Pin10

        int iterCount = 0;
        volatile unsigned int last_count = 0;
	while(1){
            // Toggle LED 4
            TOGGLELED(LED4_PORT);
            if (++iterCount == 2000)
            {
                iterCount = 0;

                unsigned int msec = counter % 1000;
                unsigned int sec = (counter / 1000) % 60;
                unsigned int min = (counter / 1000) / 60;

                lcd_locate(0,0);
                lcd_printf("%02u:%02u.%03u\nIteration time: %u (%.4f sec)", min, sec, msec, last_count, last_count * 7.1825e-5f);
                Nop();
            }
            last_count = TMR3;
            TMR3 = 0;
            //Nop();
	}
}
