#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL
#include <stdio.h>
#include <libpic30.h>

#include "lcd.h"
#include "flextouch.h"
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

uint16_t flag = 1;



uint16_t findMedian(uint16_t arrX[])
{
    uint16_t min=10000;
    uint16_t minIndex=0;
    int i =0;
            for(i=0; i<5; i++)
            {
                if(arrX[i]<min)
                {
                    min=arrX[i];
                    minIndex=i;
                }
            }

            uint16_t swap = arrX[0];
            arrX[0] = arrX[minIndex];
            arrX[minIndex] = swap;
            
            min = 10000;

            for(i=1; i<5; i++)
            {
                if(arrX[i]<min)
                {
                    min=arrX[i];
                    minIndex=i;
                }
            }

            swap = arrX[1];
            arrX[1] = arrX[minIndex];
            arrX[minIndex] = swap;

            for(i=2; i<5; i++)
            {
                if(arrX[i]<min)
                {
                    min=arrX[i];
                    minIndex=i;
                }
            }

            return arrX[minIndex];
}



   //
// Initialize timer 2 to 10 ms period with interrupt enabled.
//
void init_timer2()
{
   CLEARBIT(T2CONbits.TON);   // Disable Timer
   CLEARBIT(T2CONbits.TCS);   //Select internal instruction cycle clock (12.8 MHz)
   CLEARBIT(T2CONbits.TGATE); //Disable Gated Timer mode
   TMR2 = 0x00;               //Clear Timer Register
   T2CONbits.TCKPS = 0b10;    //Set Prescaler (1:256)
   PR2 = 2000;                //Set Period
   IPC1bits.T2IP = 0x01;      //Set IPL
   CLEARBIT(IFS0bits.T2IF);   //Clear IF
   //SETBIT(IEC0bits.T2IE);     //Enable INT
   SETBIT(T2CONbits.TON);     //Enable Timer
}

//
// T2 interrupt called each 10 ms.
//
void
__attribute__ (( __interrupt__ )) _T2Interrupt(void)
{
   flag = 1;
   CLEARBIT(IFS0bits.T2IF);
}


void main(){
	//Init LCD
	__C30_UART=1;
	lcd_initialize();
	lcd_clear();
	lcd_locate(0,0);
        init_timer2();

	lcd_printf(" --Team 5 - Lab 6 --");

        uint16_t xCount1 = 0;
        uint16_t xCount2 = 0;
        uint16_t xCount3 = 0;
        uint16_t xCount4 = 0;
        uint16_t xCount5 = 0;
        uint16_t xmedian = 0;

        uint16_t yCount1 = 0;
        uint16_t yCount2 = 0;
        uint16_t yCount3 = 0;
        uint16_t yCount4 = 0;
        uint16_t yCount5 = 0;
        uint16_t ymedian = 0;

        init_adc();
        touch_init();

        
	while(1){

            AD2CHS0bits.CH0SA = 0x009; // y ball values
            touch_select_dim(1);
            //sleep();

           TMR2 = 0x00;
           SETBIT(IEC0bits.T2IE);     //Enable Timer2 INT
           while(!flag);
           flag = 0;

            yCount1 = touch_adc();
            yCount2 = touch_adc();
            yCount3 = touch_adc();
            yCount4 = touch_adc();
            yCount5 = touch_adc();

            uint16_t arrY[5] = {yCount1, yCount2,yCount3,yCount4,yCount5};
            ymedian= findMedian(arrY);

            //x  sampling code here
            AD2CHS0bits.CH0SA = 0x00F; // x ball values
            touch_select_dim(0);
            //sleep();

           TMR2 = 0x00;
           SETBIT(IEC0bits.T2IE);     //Enable Timer2 INT
           while(!flag);
           flag = 0;
   
            xCount1 = touch_adc();
            xCount2 = touch_adc();
            xCount3 = touch_adc();
            xCount4 = touch_adc();
            xCount5 = touch_adc();   

            uint16_t arrX[5] = {xCount1, xCount2,xCount3,xCount4,xCount5};
            xmedian= findMedian(arrX);

            lcd_locate(0, 1);
            lcd_printf("X median is %d  ", xmedian);
            lcd_locate(0, 2);
            lcd_printf("Y median is %d  ", yCount1);
            }
         
	
}
