#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL
#include <stdio.h>
#include <libpic30.h>
#include "types.h"

#include "lcd.h"
//#include "led.h"
#include "Debouncer.h"
#include "flexmotor.h"

#define TIMER_INTERVEL 0x8fff
#define LOOP_INTERVEL 2000

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT);

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);

void initADCs(){
    // ADC1 for X
    CLEARBIT(AD1CON1bits.ADON);
    //initialize PIN
    SETBIT(TRISEbits.TRISE8); //set TRISE RE8 to input
    CLEARBIT(AD1PCFGHbits.PCFG20); //set AD1 AN20 input pin as analog
    //Configure AD1CON1
    CLEARBIT(AD1CON1bits.AD12B); //set 10b Operation Mode
    AD1CON1bits.FORM = 0; //set integer output
    AD1CON1bits.SSRC = 0x7; //set automatic conversion
    //Configure AD1CON2
    AD1CON2 = 0; //not using scanning sampling
    //Configure AD1CON3
    CLEARBIT(AD1CON3bits.ADRC); //internal clock source
    AD1CON3bits.SAMC = 0x1F; //sample-to-conversion clock = 31Tad
    AD1CON3bits.ADCS = 0x2; //Tad = 3Tcy (Time cycles)
    //Leave AD1CON4 at its default value
    //enable ADC
    SETBIT(AD1CON1bits.ADON);

    // ADC2 for Y
    CLEARBIT(AD2CON1bits.ADON);

    SETBIT(TRISBbits.TRISB4);
    CLEARBIT(AD2PCFGLbits.PCFG4);
    SETBIT(TRISBbits.TRISB5);
    CLEARBIT(AD2PCFGLbits.PCFG5);
    //Configure AD2CON1
    CLEARBIT(AD2CON1bits.AD12B);
    AD2CON1bits.FORM = 0;
    AD2CON1bits.SSRC = 0x7;

    AD2CON2 = 0;
    //Configure AD1CON3
    CLEARBIT(AD2CON3bits.ADRC);
    AD2CON3bits.SAMC = 0x1F;
    AD2CON3bits.ADCS = 0x2;

    SETBIT(AD2CON1bits.ADON);
}


void initCounterTimer3() { // for debouncing
    CLEARBIT(T3CONbits.TON); // Disable Timer
    CLEARBIT(T3CONbits.TCS); // Select internal instruction cycle clock
    CLEARBIT(T1CONbits.TGATE); // Disable gated timer mode

    TMR3 = 0x00; // clear register

    SETBIT(T3CONbits.TON); // turn on Timer3
}

int main(){
    //Init LCD
    __C30_UART=1;
    lcd_initialize();
    lcd_clear();

    initCounterTimer3(); // for deboucing
    initADCs();

    motor_init(CHANNEL_X); // using Timer2
    motor_init(CHANNEL_Y); // using Timer2

    SETBIT(TRISEbits.TRISE8); // set trigger as task toggle
    SETBIT(TRISDbits.TRISD10); // set thumb as reset

    uint16_t X;
    uint16_t Xmin = 0xffff;
    uint16_t Xmax = 0x0000;

    uint16_t Y;
    uint16_t Ymin = 0xffff;
    uint16_t Ymax = 0x0000;
    uint16_t Xpulse;
    uint16_t Ypulse;

    uint16_t loopCounter = 0;

    Debouncer trigger;
    uint16_t triggerStatus;

    Debouncer thumb; // thumb
    uint16_t thumbStatus;

    enum taskFlags {MEASURE_MAX_X, MEASURE_MIN_X, MEASURE_MAX_Y,
                    MEASURE_MIN_Y, SERVO_X, SERVO_Y}
    taskFlag = MEASURE_MAX_X;

    while(1)
    {
        ++loopCounter;
        if (TMR3 >= TIMER_INTERVEL) // debouncing period
        {
            TMR3 = 0x0000;

            button_read(&trigger, PORTEbits.RE8); // Joy-stick trigger button
            triggerStatus = button_debounced(&trigger);
            if (triggerStatus != UNSTABLE && triggerStatus != UNCHANGED)
            {
                if (triggerStatus == 0 )
                {
                    if (taskFlag++ >= SERVO_Y) // Go to the next task
                    {
                        lcd_locate(0,7);
                        lcd_printf("All Done!");
                    }
                }
            }

            button_read(&thumb, PORTDbits.RD10); // Joy-stick thumb button
            thumbStatus = button_debounced(&thumb);
            if (thumbStatus != UNSTABLE && thumbStatus != UNCHANGED)
            {
                if (thumbStatus == 0 )
                { // reset and start over again
                    lcd_clear();
                    uint16_t Xmin = 0xffff;
                    uint16_t Xmax = 0x0000;
                    uint16_t Ymin = 0xffff;
                    uint16_t Ymax = 0x0000;
                    loopCounter = 0;
                    taskFlag = MEASURE_MAX_X;
                }
            }
        }

        // read X
        if (loopCounter % LOOP_INTERVEL == 0)
        {
            AD1CHS0bits.CH0SA = 0x004; //set ADC to Sample AN4 pin, Joystick X-axis
            SETBIT(AD1CON1bits.SAMP); //start to sample
            while(!AD1CON1bits.DONE); // wait for conversion to finish
            CLEARBIT(AD1CON1bits.DONE); // MUST HAVE! clear conversion done bit
            X = ADC1BUF0;
        }

        // read Y
        if (loopCounter % LOOP_INTERVEL == 0)
        {
            AD2CHS0bits.CH0SA = 0x005; //set ADC to Sample AN5 pin, Joystick Y-axis
            SETBIT(AD2CON1bits.SAMP); //start to sample
            while(!AD2CON1bits.DONE); // wait for conversion to finish
            CLEARBIT(AD2CON1bits.DONE); // MUST HAVE! clear conversion done bit
            Y = ADC2BUF0; 
        }

        switch (taskFlag)
        {
            case MEASURE_MAX_X :

                if (loopCounter >= LOOP_INTERVEL)
                {
                    loopCounter = 0;
                    lcd_locate(0,1);
                    lcd_printf("Xmax:%d   ", X);
                    Xmax = (Xmax > X) ? Xmax : X;
                }
                break;

            case MEASURE_MIN_X :

                if (loopCounter >= LOOP_INTERVEL)
                {
                    loopCounter = 0;
                    lcd_locate(0,2);
                    lcd_printf("Xmin:%d   ", X);
                    Xmin = (Xmin < X) ? Xmin : X;
                }

                break;

            case MEASURE_MAX_Y :      

                if (loopCounter >= LOOP_INTERVEL)
                {
                    loopCounter = 0;
                    lcd_locate(0,3);
                    lcd_printf("Ymax:%d   ", Y);
                    Ymax = (Ymax > Y) ? Ymax : Y;
                }
                break;

            case MEASURE_MIN_Y :

                if (loopCounter >= LOOP_INTERVEL)
                {
                    loopCounter = 0;
                    lcd_locate(0,4);
                    lcd_printf("Ymin:%d   ", Y);
                    Ymin = (Ymin < Y) ? Ymin : Y;
                }
                break;

            case SERVO_X :
                Xpulse = (((X - Xmin) * 140L)/(Xmax - Xmin)) + 220;
                
                if (loopCounter >= LOOP_INTERVEL)
                {
                    loopCounter = 0;
                    motor_set_duty(CHANNEL_X, Xpulse);
                    lcd_locate(0,5);
                    lcd_printf("Xpwm:%d", Xpulse);
                }

            case SERVO_Y :
                Ypulse = (((Y - Ymin) * 140L)/(Ymax - Ymin)) + 220;
              
                if (loopCounter >= LOOP_INTERVEL)
                {
                    loopCounter = 0;
                    motor_set_duty(CHANNEL_Y, Ypulse);
                    lcd_locate(0,6);
                    lcd_printf("Ypwm:%d", Ypulse);
                }
            default :
        	    break;
        }
    }
    
    return 0; 
}
