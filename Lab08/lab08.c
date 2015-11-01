#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL
#include <stdio.h>
#include <libpic30.h>

#include <time.h>
#include "lcd.h"
#include "flextouch.h"
#include "flexmotor.h"
#include "algorithm.h"
#include "pid_controller.h"

#define NUM_SAMPLES (5)
#define KP (0.1)
#define KI (0.1)
#define KD (0.02)
#define Set_x (1500)
#define Set_y (1500)

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT);

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);

void init_timer3()
{
   CLEARBIT(T3CONbits.TON);   // Disable Timer
   CLEARBIT(T3CONbits.TCS);   //Select internal instruction cycle clock (12.8 MHz)
   CLEARBIT(T3CONbits.TGATE); //Disable Gated Timer mode
   TMR3 = 0x00;               //Clear Timer Register
   T3CONbits.TCKPS = 0b10;    //Set Prescaler (1:64)
   PR3 = 10000;                //Set Period
   IPC2bits.T3IP = 0x01;      //Set IPL
   CLEARBIT(IFS0bits.T3IF);   //Clear IF
   SETBIT(IEC0bits.T3IE);     //Enable INT
   SETBIT(T3CONbits.TON);     //Enable Timer
}

uint16_t samples[NUM_SAMPLES];
uint16_t i = 0;
uint16_t x_set, x_current = Set_x;
uint16_t y_set, y_current = Set_y;
uint16_t js_x;
uint16_t js_y;

pid_controller_t controller_x;
pid_controller_t controller_y;

void
__attribute__ (( __interrupt__, no_auto_psv )) _T3Interrupt(void)
{
    // pid control on x
    touch_select_dim(DIM_X);
    for (i = 0; i < NUM_SAMPLES; ++i) {
        Delay_ms(1);
        samples[i] = touch_adc();
    }

    x_current = find_median(samples, NUM_SAMPLES);

    motor_set_duty(CHANNEL_X, feed_back(&controller_x, x_current, x_set));
    
    // pid control on y
    touch_select_dim(DIM_Y);
    for (i = 0; i < NUM_SAMPLES; ++i) {
        Delay_ms(1);
        samples[i] = touch_adc();
    }

    y_current = find_median(samples, NUM_SAMPLES);

    motor_set_duty(CHANNEL_Y, feed_back(&controller_y, y_current, y_set));

    CLEARBIT(IFS0bits.T3IF);
}

void init_trigger(){
    SETBIT(TRISEbits.TRISE8); // input
    SETBIT(AD1PCFGHbits.PCFG20);
    SETBIT(IEC1bits.INT1IE); // enable interrupt
    SETBIT(INTCON2bits.INT1EP); // enable FALLING edge-triggered external interrupt 1
    IPC5bits.INT1IP1 = 0x01; // interrupt priority
}

void __attribute__ ((__interrupt__)) _INT1Interrupt(void){

    x_set = js_x;
    y_set = js_y;

   CLEARBIT(IFS1bits.INT1IF);
}

int main(){
	//Init LCD
	__C30_UART=1;
	lcd_initialize();
	lcd_clear();
	lcd_locate(0,0);

	lcd_printf("Lab07");

    init_timer3(); // for timer interrupt

    pid_controller_init(&controller_x, 290, 0.05, KP, KI, KD);
    pid_controller_init(&controller_y, 290, 0.05, KP, KI, KD);

    init_adc1(); // for flextouch
    touch_init();
    
    init_adc2(); // for joystick
    init_trigger();

    motor_init(CHANNEL_Y);
    motor_init(CHANNEL_X);
    
    uint16_t loopCounter = 0;
    // display_arena();

    while(1) {
        // sample Joystick
        AD2CHS0bits.CH0SA = 0x004; //set ADC to Sample AN4 pin, Joystick X-axis
        SETBIT(AD2CON1bits.SAMP); //start to sample
        while(!AD2CON1bits.DONE); // wait for conversion to finish
        CLEARBIT(AD2CON1bits.DONE); // MUST HAVE! clear conversion done bit
        // TODO: need to do a mapping from ADC2BUF0 to js_x
        js_x = ADC2BUF0;
        
        AD2CHS0bits.CH0SA = 0x005; //set ADC to Sample AN5 pin, Joystick Y-axis
        SETBIT(AD2CON1bits.SAMP); //start to sample
        while(!AD2CON1bits.DONE); // wait for conversion to finish
        CLEARBIT(AD2CON1bits.DONE); // MUST HAVE! clear conversion done bit
        // TODO: need to do a mapping from ADC2BUF0 to js_y
        js_y = ADC2BUF0;

        if (loopCounter++ >= 2000) {
            loopCounter = 0;
        // display(x_set, y_set, js_x, js_y);
        }
        
    }

    return 0;
}
