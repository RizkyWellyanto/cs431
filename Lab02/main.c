#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include <libpic30.h>

#include "lcd.h"
#inlcude "led.h"

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);  

// global counters
uint8_t timer2Counter = 0;
uint8_t min = 0;
uint8_t sec = 0;
uint8_t ms = 0;

void initTimer2() {
    CLEARBIT(T2CONbits.TON); // Disable Timer
    CLEARBIT(T2CONbits.TCS); // Select internal instruction cycle clock
    CLEARBIT(T2CONbits.TGATE); // Disable Gated Timer mode
    TMR2 = 0; // clear register
    IPC1bits.T2IP = 0x03; // set Interrupt priority level
    T2CONbits.TCKPS = 0b11; // Select 1:256 Prescaler
    PR2 = 50; // Period = timeInterval (2 ms) * clock frequency / Prescaler
    CLEARBIT(IFS0bits.T2IF); // Clear Timer2 Interrupt Flag
    SETBIT(IEC0bits.T2IE); // Enable Timer2 interrupt
    SETBIT(T2CONbits.TON); // Start Timer
}

void initTimer1() {
    //enable LPOSCEN: use the 32kHz crystal as clock source for >ms intervals
    __builtin_write_OSCCONL(OSCCONL | 2);
    CLEARBIT(T1CONbits.TON); // Disable Timer
    SETBIT(T1CONbits.TCS); // Select external clock
    CLEARBIT(T1CONbits.TSYNC); // Disable Synchronization
    T1CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR1 = 0; // Clear timer register
    
    /* Using external clock:
     * timeInterval = PR1 * Prescaler / clock = 1 sec */
    PR1 = 32767; // Load the period value
    IPC0bits.T1IP = 0x02; // Set Timer1 Interrupt Priority Level
    CLEARBIT(IFS0bits.T1IF); // Clear Timer1 Interrupt Flag
    SETBIT(IEC0bits.T1IE);// Enable Timer1 interrupt
    SETBIT(T1CONbits.TON);// Start Timer
}

void initTimer3() {
    CLEARBIT(T3CONbits.TON); // Disable Timer
    CLEARBIT(T3CONbits.TCS); // Select internal instruction cycle clock
    CLEARBIT(T1CONbits.TGATE); // Disable gated timer mode
    
    TMR3 = 0; // clear register
    
    SETBIT(T3CONbits.TON); // turn on Timer3
}


void initTrigger(){
    SETBIT(TRISEbits.TRISE8); // input
    SETBIT(AD1PCFGHbits.PCFG20);
    SETBIT(IEC1bits.INT1IE); // enable interrupt
    SETBIT(INTCON2bits.INT1EP); // enable FALLING edge-triggered external interrupt 1
    IPC5bits.INT1IP1 = 0x01; // interrupt priority
}

// Obj. 2 toggle LED1 using Timer2 interrupt
void __attribute__ ((__interrupt__)) _T2Interrupt(void){
    if(!(timer2Counter%2)){ // toggle every 2 ms
        TOGGLELED(LED1_PORT);
    }
    ++timer2Counter;
	//Clear the interrupt flag
	CLEARBIT(IFS0bits.T2IF);

    ++ms;
    if(ms >= 1000){
        ms = 0;
    }
}

// Obj. 3 toggle LED2 using Timer1 interrupt
void __attribute__ ((__interrupt__)) _T1Interrupt(void){
	TOGGLELED(LED2_PORT);

	//Clear the interrupt flag
	CLEARBIT(IFS0bits.T1IF);

    ++sec;
    if(sec >= 60){
        sec = 0;
        ++min;
    }
}

// Ob. 5 reset the time to 0 if trigger fired
void __attribute__ ((__interrupt__)) _INT1Interrupt(void){

    min = 0;
    sec = 0;
    ms = 0;

   CLEARBIT(IFS1bits.INT1IF);
}


void main(){
	//Init LCD
	__C30_UART=1;	
	lcd_initialize();
	lcd_clear();
	lcd_locate(0,0);
    
    float mili = 0f;
	
    CLEARLED(LED1_TRIS);
    CLEARLED(LED2_TRIS);
    CLEARLED(LED3_TRIS);
    CLEARLED(LED4_TRIS);
    
    initTimer2();
    initTimer1();
    initTrigger();
    initTimer3();
	
    lcd_printf("Bo Liu, Daniel Hsu, Ron Wright\n");
    
    // counter
    uint16_t cnt = 0;

    while(1){
        CLEARBIT(TMR3);
        ++cnt;

        // Obj. 1 Toggle LED 4  every iteration.
        TOGGLELED(LED4_PORT);
        // Obj. 4 count loop time
        if(cnt == 2500){
            cnt = 0;
            lcd_locate(0,2);
            lcd_printf("time lapse: %d:%.2d:%.3d", min, sec, ms);
            
            lcd_locate(0,4);
            lcd_printf("looptime = %d %.4f ", TMR3, (TMR3/12800.0));
        }
	}
}
