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

int main(){
    
    INIT: 
    
    //Init LCD
    __C30_UART=1;
    lcd_initialize();
    lcd_clear();

    lcd_locate(0,0);
    lcd_printf("*Lab05*");

    init_adc2();

    motor_init(CHANNEL_X); // using Timer2
    motor_init(CHANNEL_Y); // using Timer2

    SETBIT(TRISEbits.TRISE8); // set trigger as task toggle
    SETBIT(AD1PCFGHbits.PCFG20);
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
        if (++loopCounter > LOOP_INTERVEL)
            loopCounter = 0;

        if (loopCounter >= LOOP_INTERVEL) // debouncing period
        {
            button_read(&trigger, PORTEbits.RE8); // Joy-stick trigger button
            triggerStatus = button_debounced(&trigger);
            if (triggerStatus != UNSTABLE && triggerStatus != UNCHANGED)
            {
                if (triggerStatus == 0 )
                {
                    if (++taskFlag > SERVO_Y) // Go to the next task
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
                    goto INIT;
                }
            }
        }

        // read X
        if (loopCounter % LOOP_INTERVEL/5 == 0)
        {
            AD2CHS0bits.CH0SA = 0x004; //set ADC to Sample AN4 pin, Joystick X-axis
            SETBIT(AD2CON1bits.SAMP); //start to sample
            while(!AD2CON1bits.DONE); // wait for conversion to finish
            CLEARBIT(AD2CON1bits.DONE); // MUST HAVE! clear conversion done bit
            X = ADC2BUF0;
        }

        // read Y
        if (loopCounter % LOOP_INTERVEL/5 == 0)
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
                    lcd_locate(0,1);
                    lcd_printf("Xmax:%d   ", X);
                    Xmax = (Xmax > X) ? Xmax : X;
                    //lcd_locate(10,1);
                    //lcd_printf("X_max:%d   ", Xmax);
                }
                break;

            case MEASURE_MIN_X :

                if (loopCounter >= LOOP_INTERVEL)
                {
                    lcd_locate(0,2);
                    lcd_printf("Xmin:%d   ", X);
                    Xmin = (Xmin < X) ? Xmin : X;
                    //lcd_locate(10,2);
                    //lcd_printf("X_min:%d   ", Xmin);
                }

                break;

            case MEASURE_MAX_Y :

                if (loopCounter >= LOOP_INTERVEL)
                {
                    lcd_locate(0,3);
                    lcd_printf("Ymax:%d   ", Y);
                    Ymax = (Ymax > Y) ? Ymax : Y;
                    //lcd_locate(10,3);
                    //lcd_printf("Y_max:%d   ", Ymax);
                }
                break;

            case MEASURE_MIN_Y :

                if (loopCounter >= LOOP_INTERVEL)
                {
                    lcd_locate(0,4);
                    lcd_printf("Ymin:%d   ", Y);
                    Ymin = (Ymin < Y) ? Ymin : Y;
                    //lcd_locate(10,4);
                    //lcd_printf("Y_min:%d   ", Ymin);
                }
                break;

            case SERVO_X :

                Xpulse = 2*240L*(X-Xmin)/(Xmax-Xmin); // HIGH - LOW = 240
                // PERIOD - HIGH = 180
                // the above magic numbers are for servo subtrim and throw adjustment
                // Same for Y

                if (loopCounter >= LOOP_INTERVEL)
                {
                    motor_set_duty(CHANNEL_X, Xpulse);
                    lcd_locate(0,5);
                    lcd_printf("Xpwm:%d", Xpulse);
                }
                break;

            case SERVO_Y :
                Ypulse = 2*240L*(Y-Ymin)/(Ymax-Xmin);

                if (loopCounter >= LOOP_INTERVEL)
                {
                    motor_set_duty(CHANNEL_Y, Ypulse);
                    lcd_locate(0,6);
                    lcd_printf("Ypwm:%d", Ypulse);
                }
                break;
                
            default :
        	    break;
        }
    }

    return 0;
}
