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
#define KP (0.05)
#define KI (0.03)
#define KD (0.01)
#define Set_x (1750)

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
uint16_t x_current = Set_x;
uint16_t duty;

pid_controller_t controller;

void
__attribute__ (( __interrupt__, no_auto_psv )) _T3Interrupt(void)
{
    for (i = 0; i < NUM_SAMPLES; ++i) {
        Delay_ms(1);
        samples[i] = touch_adc();
    }

    x_current = find_median(samples, NUM_SAMPLES);
    duty = feed_back(&controller, x_current);

    lcd_locate(10,7);
    lcd_printf("duty: %u          ", (uint16_t)duty);

    motor_set_duty(CHANNEL_X, duty);

    lcd_locate(10, 0);
    lcd_printf("kp:%.3f", KP);
    lcd_locate(0, 1);
    lcd_printf("ki:%.3f, kd:%.3f", KI, KD);
    lcd_locate(0,2);
    lcd_printf("Set_x:%u", Set_x);
    lcd_locate(0,3);
    lcd_printf("x_current:%u     ", x_current);
    lcd_locate(0,4);
    lcd_printf("P_x: %.2f        ", (double)controller.current_delta*controller.kp);
    lcd_locate(0,5);
    lcd_printf("I_x: %.2f        ", (double)controller.integral*controller.ki);
    lcd_locate(0,6);
    lcd_printf("D_x: %.2f        ", (double)controller.derivative*controller.kd);
    lcd_locate(0,7);
    lcd_printf("F_x: %u          ", duty);

    CLEARBIT(IFS0bits.T3IF);
}

int main(){
	//Init LCD
	__C30_UART=1;
	lcd_initialize();
	lcd_clear();
	lcd_locate(0,0);

	lcd_printf("Lab07");

    init_timer3(); // for timer interrupt

    pid_controller_init(&controller, Set_x, 0.05, KP, KI, KD);

    init_adc1(); // for flextouch

    touch_init();

    motor_init(CHANNEL_Y);
    motor_set_duty(CHANNEL_Y, HIGH); // set Y to a fixed position

    motor_init(CHANNEL_X);

    touch_select_dim(DIM_X);

    while(1);

    return 0;
}
