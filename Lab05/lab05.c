#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include <libpic30.h>

#include "lcd.h"
#include "led.h"
#include "Debouncer.h"
#include "flexmotor.h"

#define TIMER_INTERVEL 12800

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
    CLEARBIT(AD1CON1bits.AD12B) //set 10b Operation Mode
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
    AD1CHS0bits.CH0SA = 0x004; //set ADC to Sample AN4 pin, Joystick X-axis
    SETBIT(AD1CON1bits.SAMP); //start to sample
    
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
    AD2CHS0bits.CH0SA = 0x005; //set ADC to Sample AN5 pin, Joystick Y-axis
    SETBIT(AD2CON1bits.SAMP); //start to sample
}


void initCounterTimer3() { // for debouncing
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
    
    lcd_printf("Bo Liu, Daniel Hsu, Ron Wright\n");
    
    initCounterTimer3();
    initADCs();
    
    motor_init(CHANNEL_X);
    motor_init(CHANNEL_Y);
    
    uint16_t X;
    uint16_t Xmin;
    uint16_t Xmax;
    
    uint16_t Y;
    uint16_t Ymin;
    uint16_t Ymax;
    
    uint16_t freezeFlag;
    uint16_t loopCounter;
    
    Debouncer button1;
    
    // get initialized values
    while(!AD1CON1bits.DONE); //wait for conversion to finish
    CLEARBIT(AD1CON1bits.DONE); //MUST HAVE! clear conversion done bit
    Xmin = Xmax = X = ADC1BUF0;
    
    while(!AD2CON1bits.DONE); //wait for conversion to finish
    CLEARBIT(AD2CON1bits.DONE); //MUST HAVE! clear conversion done bit
    Ymin = Ymax = Y = ADC2BUF0;

    while(1)
    {
        ++loopCounter;
        if (TMR3 == TIMER_INTERVEL) // samples the button status every 2k loops
        {
            TMR3 = 0;
            
            button_read(&button1, PORTEbits.RE8); // Joy-stick trigger button
            if (button_debounce(&button1) != UNSTABLE)
            {
                if (button_debounce(&button1) == 0 )
                {
                	freezeFlag = 0;
                }
                else
                {
                	freezeFlag = 1;
                }
            }
        }
        
        if(!freezeFlag)
        {
            // read X
            while(!AD1CON1bits.DONE); // wait for conversion to finish
            CLEARBIT(AD1CON1bits.DONE); // MUST HAVE! clear conversion done bit
            X = ADC1BUF0;

            Xmin = (Xmin < X) ? Xmin : X;
            Xmax = (Xmax > X) ? Xmax : X;

            // read Y
            while(!AD2CON1bits.DONE); // wait for conversion to finish
            CLEARBIT(AD2CON1bits.DONE); // MUST HAVE! clear conversion done bit
            Y = ADC2BUF0;

            Ymin = (Ymin < Y) ? Ymin : Y;
            Ymax = (Ymax > Y) ? Ymax : Y;
        }
        
        if(loopCounter == 1000){
            loopCounter = 0;
            lcd_locate(2, 1);
            lcd_printf("Min X = %u, Max X = %u\r Real-time X = %u\n", Xmix, Xmax, X);
        }    
	}
}
