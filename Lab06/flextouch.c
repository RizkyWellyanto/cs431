/*
 * File:   flextouch.c
 * Author: Bo Liu
 *
 * Created on October 14, 2015, 2:31 PM
 */

#include "flextouch.h"

//Initialize the ADC
void init_adc1(){
    
    // 0. Disable ADC1
    CLEARBIT(AD1CON1bits.ADON);

    // 1. set input pins <- delegate to reading code
    // SETBIT(TRISEbits.TRISE1); // set input. ??? which port
    // SETBIT(TRISEbits.TRISE2); // set input. ??? which port
    // SETBIT(TRISEbits.TRISE3); // set input. ??? which port
    
    // 2. set analog pins
    CLEARBIT(AD1PCFGLbits.PCFG15); // set analog. X is hardwired to AN15
    CLEARBIT(AD1PCFGLbits.PCFG9); // set analog. Y is hardwired to AN9
    
    // 3. Configure AD1CON1
    SETBIT(AD1CON1bits.AD12B); // set 12-bit mode
    AD1CON1bits.FORM = 0; // set integer output
    AD1CON1bits.SSRC = 0x7; // set automatic conversion

    // 4. Configure AD1CON2
    AD1CON2 = 0; // not using scanning sampling
    
    // 5. Configure AD1CON3
    CLEARBIT(AD1CON3bits.ADRC); // internal clock source
    AD1CON3bits.SAMC = 0x1F; // sample-to-conversion clock = 31Tad
    AD1CON3bits.ADCS = 0x2; // Tad = 3Tcy (Time cycles)

    // 6. Enable ADC1
    SETBIT(AD1CON1bits.ADON);
}



//Initialize the touch screen
void touch_init(){
    //set up the I/O pins E1, E2, E3 to be output pins
    CLEARBIT(TRISEbits.TRISE1); //I/O pin set to output
    CLEARBIT(TRISEbits.TRISE2); //I/O pin set to output
    CLEARBIT(TRISEbits.TRISE3); //I/O pin set to output
}

// select the dimension for the touch Screen
void touch_select_dim(uint8_t dim) {

    if(dim == DIM_X) { 
        // set up the I/O pins E1, E2, E3 for X
        CLEARBIT(PORTEbits.RE1);
        SETBIT(PORTEbits.RE2);
        SETBIT(PORTEbits.RE3);
    } else if (dim == DIM_Y) { 
        // set up the I/O pins E1, E2, E3 for Y
        SETBIT(PORTEbits.RE1);
        CLEARBIT(PORTEbits.RE2);
        CLEARBIT(PORTEbits.RE3);
    }
}

uint16_t touch_adc(){

    SETBIT(AD1CON1bits.SAMP); // start to sample
    while(!AD1CON1bits.DONE); // wait for conversion to finish
    CLEARBIT(AD1CON1bits.DONE); // MUST HAVE! clear conversion done bit

    return ADC1BUF0;
}

uint16_t touch_read(uint8_t dim) {
    
    if(dim == DIM_X) { 
        // set up the I/O pins E1, E2, E3 for X
        CLEARBIT(PORTEbits.RE1);
        SETBIT(PORTEbits.RE2);
        SETBIT(PORTEbits.RE3);
        
        AD1CHS0bits.CH0SA = 0x000E; // AN15: X dimension
        SETBIT(AD1CON1bits.SAMP); // start to sample
        while(!AD1CON1bits.DONE); // wait for conversion to finish
        CLEARBIT(AD1CON1bits.DONE); // MUST HAVE! clear conversion done bit

        return ADC1BUF0;
        
    } else if (dim == DIM_Y) { 
        // set up the I/O pins E1, E2, E3 for Y
        SETBIT(PORTEbits.RE1);
        CLEARBIT(PORTEbits.RE2);
        CLEARBIT(PORTEbits.RE3);
                
        AD1CHS0bits.CH0SA = 0x0009; // AN9: Y dimension
        SETBIT(AD1CON1bits.SAMP); // start to sample
        while(!AD1CON1bits.DONE); // wait for conversion to finish
        CLEARBIT(AD1CON1bits.DONE); // MUST HAVE! clear conversion done bit

        return ADC1BUF0;        
    }
}
