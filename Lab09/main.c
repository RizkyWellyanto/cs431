#include "lcd.h"
#include "types.h"
// #include "led.h"
#include "flexmotor.h"
#include "flextouch.h"
#include "pid_controller.h"
#include "algorithm.h"
#include "performance.h"
#include "math.h"

#include <p33Fxxxx.h>

#define FCY 12800000UL

// control task frequency (Hz)
#define RT_FREQ 50

//setpoint parameters
#define SPEED 0.08  // tested up to .12!
#define RADIUS 350
#define CENTER_X 1725
#define CENTER_Y 1650

// Servo defines
#define MAX_DUTY_MICROSEC 2100
#define MIN_DUTY_MICROSEC 900
#define SERVO_PWM_PERIOD_MICROSEC 20000
#define INIT_DUTYX_MICROSEC 1410    // leveled plate on X axis: 1410
#define INIT_DUTYY_MICROSEC 1400     // leveled plate on Y axis: 1400
//#define MOTOR_X_CHAN 1
//#define MOTOR_Y_CHAN 2

// Touch screen definitions
//#define X_DIM 1
//#define Y_DIM 2

#define ADC_SAMPLES (5)

#define KP_X (0.0)
#define KI_X (0.0)
#define KD_X (0.0)

#define KP_Y (0.02)
#define KI_Y (0.0)
#define KD_Y (0.01)

#define BOUNDARY_BUF (50)

uint16_t TOUCH_MIN_X = 340;
uint16_t TOUCH_MAX_X = 3020;
uint16_t TOUCH_MIN_Y = 454;
uint16_t TOUCH_MAX_Y = 2657;

// do not change position of this include
#include <libpic30.h>

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT);

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);

uint16_t samples[ADC_SAMPLES];
uint16_t i = 0; // samples iterator


// control setpoint
double Xpos_set = 1725.0, Ypos_set = 1650.0;

// raw, unfiltered X and Y position of the ball
volatile uint16_t Xpos, Ypos;
volatile uint8_t start = 0;
volatile uint8_t select = DIM_X;
volatile uint8_t deadline_miss = 0;

double pidX, pidY;
uint16_t duty_us_x, duty_us_y;

pid_controller_t controller_x;
pid_controller_t controller_y;

double pidX_controller(double Xp) {
  // double pid;
  // TODO: Implement PID X
  return feed_back(&controller_x, Xp, Xpos_set);
  // return pid;
}


double pidY_controller(double Yp) {
  // double pid;
  // TODO: Implement PID Y
  return feed_back(&controller_y, Yp, Ypos_set);
  // return pid;
}

// Configure the real-time task timer and its interrupt.
void timers_initialize() {

  //Set Timer1 to generate an interrupt every 10ms (100Hz) ==> PR1=500
  CLEARBIT(T1CONbits.TON); //Disable Timer
  CLEARBIT(T1CONbits.TCS); //Select internal instruction cycle clock
  CLEARBIT(T1CONbits.TGATE); //Disable Gated Timer mode
  T1CONbits.TCKPS = 0b11; //Select 1:256 Prescaler
  PR1 = 500; //Load the period value ==> running at 100Hz now!
  TMR1 = 0x00; //Clear timer register
  IPC0bits.T1IP = 0x01; // Set Timer1 Interrupt Priority Level
  CLEARBIT(IFS0bits.T1IF); // Clear Timer1 Interrupt Flag
  SETBIT(IEC0bits.T1IE); // Enable Timer1 interrupt
  SETBIT(T1CONbits.TON); // Start Timer
}

int main(){
  uint8_t start_r, old_IPL;
  uint8_t hz50_scaler, hz5_scaler, hz1_scaler, sec;
  uint32_t tick = 0;

  hz50_scaler = hz5_scaler = hz1_scaler = sec = 0;

  init_adc1();
  touch_init();
  select = DIM_X;
  touch_select_dim(DIM_X);

  __delay_ms(200);
  lcd_initialize();             // Initialize the LCD

  motor_init(CHANNEL_X);
  motor_set_duty(CHANNEL_X, MID);
  motor_init(CHANNEL_Y);
  motor_set_duty(CHANNEL_Y, MID);

  lcd_clear();
  lcd_locate(0,0);
  lcd_printf("-- Ball position: --");

  timers_initialize();          // Initialize timers


  pid_controller_init(&controller_x, 290, 1000, 0.02, KP_X, KI_X, KD_X);
  pid_controller_init(&controller_y, 285, 1500, 0.02, KP_Y, KI_Y, KD_Y);

  
  while (1) {
    start_r = 0;
    while(!start_r) {
      // disable all maskable interrupts
      SET_AND_SAVE_CPU_IPL(old_IPL, 7);
      start_r = start;

      // enable all maskable interrupts
      RESTORE_CPU_IPL(old_IPL);
    }

    // Periodic real-time task code starts here!!!

    // 50Hz control task
    if(hz50_scaler == 0) {
      calcQEI(Xpos_set, Xpos, Ypos_set, Ypos);

      Xpos_set = CENTER_X + RADIUS * cos(tick * SPEED);
      Ypos_set = CENTER_Y + RADIUS * sin(tick * SPEED);
      tick++;

//      pidX = pidX_controller(Xpos);
//      pidY = pidY_controller(Ypos);
//
      duty_us_x = feed_back(&controller_x, Xpos, Xpos_set);
      duty_us_y = feed_back(&controller_y, Ypos, Ypos_set);

      // TODO: Convert PID to motor duty cycle (900-2100 us)



      // setMotorDuty is a wrapper function that calls your motor_set_duty
      // implementation in flexmotor.c. The 2nd parameter expects a value
      // between 900-2100 us
      motor_set_duty(CHANNEL_X, duty_us_x);
      motor_set_duty(CHANNEL_Y, duty_us_y);
    }

    // 5Hz display task
    if(hz5_scaler == 0) {
      //lcd_locate(0,1);
      //lcd_printf("Xf(t)=%u, (%4.1f)", Xpos, Xpos_set);
      //lcd_locate(0,2);
      //lcd_printf("x%4u (%4.1f)     ", Xpos, Xpos_set);
      lcd_locate(0,3);
      lcd_printf("y%4u (%4.1f)     ", Ypos, Ypos_set);

      //lcd_locate(0,4);
      //lcd_printf("x%u          ", Xpos);
      //lcd_locate(0,5);
      //lcd_printf("y%u          ", Ypos);

      if(deadline_miss >= 1) {
        lcd_locate(0,6);
        lcd_printf("%4d d_misses!!!", deadline_miss);
      }
    }

    // 1Hz seconds display task
    if(hz1_scaler == 0) {
      lcd_locate(0,7);
      lcd_printf("QEI: %5u", getQEI());
      sec++;
    }

    hz50_scaler = (hz50_scaler + 1) % 2;
    hz5_scaler = (hz5_scaler + 1) % 20;
    hz1_scaler = (hz1_scaler + 1) % 100;

    start = 0;
  }

  return 0;
}

// This ISR will execute whenever Timer1 has a compare match.
// it kicks off the periodic execution of user code and performs I/O
// Min period: 10msec due to X,Y switch time for touchscreen
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {

  if(start == 1)
    deadline_miss++;

  if (select == DIM_X) {
    // TODO: read 5 samples from X-dimension and set Xpos as the median

    // touch_select_dim(DIM_X);
    for (i = 0; i < ADC_SAMPLES; ++i) {
        // Delay_ms(1);
        samples[i] = touch_adc();
    }
    Xpos = find_median(samples, ADC_SAMPLES);
    select = DIM_Y;
    touch_select_dim(DIM_Y);
  }
  else {
    // TODO: read 5 samples from Y-dimension and set Ypos as the median

    // touch_select_dim(DIM_Y);
    for (i = 0; i < ADC_SAMPLES; ++i) {
        // Delay_ms(1);
        samples[i] = touch_adc();
    }
    Ypos = find_median(samples, ADC_SAMPLES);
    select = DIM_X;
    touch_select_dim(DIM_X);
  }

  start = 1;

  IFS0bits.T1IF = 0; // clear interrupt flag
}

