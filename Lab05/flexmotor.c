/*
 * File:   flexmotor.c
 * Author: Bo Liu
 *
 * Created on October 10, 2015, 11:59 AM
 */

#include "flexmotor.h"

void motor_init(uint8_t chan)
{
    // use Timer 2
    CLEARBIT(T2CONbits.TON); // Disable Timer
    CLEARBIT(T2CONbits.TCS); // Select internal instruction cycle clock
    CLEARBIT(T2CONbits.TGATE); // Disable Gated Timer mode
    TMR2 = 0x00; // Clear timer register
    T2CONbits.TCKPS = 0b10; // Select 1:64 Prescaler
    CLEARBIT(IFS0bits.T2IF); // Clear Timer2 interrupt status flag
    CLEARBIT(IEC0bits.T2IE); // Disable Timer2 interrupt enable control bit
    PR2 = PER; // Set timer period 20ms:

   if(chan == CHANNEL_X )
   {
        CLEARBIT(TRISDbits.TRISD7); // Set OC8 as output
        OC8R = MID; // Set the initial to mid position
        OC8RS = MID; // Load OCRS: next pwm duty cycle
        OC8CON = 0x0006; // Set OC8: PWM, no fault check, Timer2
        SETBIT(T2CONbits.TON); // Turn Timer 2 on
    }
    else if (chan == CHANNEL_Y)
    {
        CLEARBIT(TRISDbits.TRISD6); // Set OC8 as output
        OC7R = MID; // Set the initial throw to mid position
        OC7RS = MID; // Load OCRS: next pwm duty cycle
        OC7CON = 0x0006; //
        SETBIT(T2CONbits.TON); // Turn Timer 2 on
    }
}

void motor_set_duty(uint8_t chan, uint16_t duty_us)
{
    uint16_t duty = HIGH - duty_us * 1e-6 * 128e5 / 64;

    if (chan == CHANNEL_X)
    {
        OC8R = duty;
        OC8RS = duty; /* Load OCRS: next pwm duty cycle */
    }
    else if (chan == CHANNEL_Y)
    {
        OC7R = duty;
        OC7RS = duty; /* Load OCRS: next pwm duty cycle */
    }
}
