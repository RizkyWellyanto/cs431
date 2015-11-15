# Chap 1: Introduction

bit (short for "binary digit").

A bit's changing value over time is called a digital *signal*. "Digital" refers to the signal having discrete rather than continuous possible values.

In digital system design, it helps to think of an abstraction of a switch. A switch is a component with a single bit output that is either a 0 or 1 depending on whether the switch is in the off or on position.

A component that can execute some computing functionality is a key part of an embedded system.

We will use an abstraction of a microcontroller, referred to as RIM (Riverside-Irvine Microcontroller), consisting of eight bit-inputs A0, A1, ..., A7 and eight bit-outputs B0, B1, ..., B7. RIM is able to execute C code that can access those inputs and outputs as implicit global variables

The term embedded system, however, commonly refers just to the compute component. The switch and buttons are examples of **sensors**, which convert physical phenomena into digital inputs to the embedded system. The LED is an example of an **actuator**, which converts digital outputs from the embedded system into physical phenomena.

# Chap 3: State Machines
A state machine is a computation model intended for capturing time-ordered behavior.

If none of the current state's transitions has a true condition for a given tick, an implicit transition back to the state itself is taken (thus causing the state's actions to execute each such tick). Good practice, however, is to have an explicit transition point back to the same state with the proper condition, rather than relying on the implicit transition from a state to itself. 

## RIBS generates C code for state machine models
e.g.
```c
/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 9/27/2015 15:22:30 PST
*/

#include "rims.h"

/*Define user variables and functions for this state machine here.*/
enum _States { _pre_red, _red, _pre_yellow, _yellow, _pre_green, _green, _s7 } _State;

TickFct_light() {
   switch(_State) { // Transitions
      case -1:
         _State = _pre_red;
         break;
         case _pre_red: 
         if (A0) {
            _State = _red;
         }
         break;
      case _red: 
         if (!A0) {
            _State = _pre_yellow;
         }
         break;
      case _pre_yellow: 
         if (A0) {
            _State = _yellow;
         }
         break;
      case _yellow: 
         if (!A0) {
            _State = _pre_green;
         }
         break;
      case _pre_green: 
         if (A0) {
            _State = _green;
         }
         break;
      case _green: 
         if (!A0) {
            _State = _pre_red;
         }
         break;
      default:
         _State = _pre_red;
   } // Transitions

   switch(_State) { // State actions
      case _pre_red:
         break;
      case _red:
         B0 = 1;
         B1 = 0;
         B2 = 0;
         break;
      case _pre_yellow:
         break;
      case _yellow:
         B0 = 0;
         B1 = 1;
         B2 = 0;
         break;
      case _pre_green:
         break;
      case _green:
         B0 = 0;
         B1 = 0;
         B2 = 1;
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   
   _State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_light();} // while (1)
} // Main
```

## Chap 3.4 SM to C
The code follows a simple pattern:
* State variable declaration
* Tick function
* main loop that calls tick function

E.g. 
![embedded_image61q9a_iivy6y4vu06ma6f3tjpoocjpgz16rjv4wqclevm_10](https://cloud.githubusercontent.com/assets/14265605/10127715/615f74e4-656c-11e5-941a-f6f8ddff9142.png)
```c
#include "RIMS.h"

enum LA_States { LA_SMStart, LA_s0, LA_s1 } LA_State;

void TickFct_Latch()
{
  switch(LA_State) {   // Transitions
     case LA_SMStart:  // Initial transition
        LA_State = LA_s0;
        break;

     case LA_s0:
        if (!A0) {
           LA_State = LA_s0;
        }
        else if (A0) {
           LA_State = LA_s1;
        }
        break;

     case LA_s1:
        if (!A0) {
           LA_State = LA_s0;
        }
        else if (A0) {
           LA_State = LA_s1;
        }
        break;

     default:
        LA_State = LA_SMStart;
        break;
  } // Transitions

  switch(LA_State) {   // State actions
     case LA_s0:
        break;

     case LA_s1:
        B0 = A1;
        break;

     default:
        break;
   } // State actions
}

void main() {
   B = 0x00;              // Initialize outputs
   LA_State = LA_SMStart; // Indicates initial call

   while(1) {
      TickFct_Latch();
   }
}
```

Good practice is to ensure that actions don't wait on an external input value, such as `while (!A0) {};`. Waiting behavior should be captured as states and transitions, e.g. a transition with condition `A0`, so that all time-ordered behavior is visible at the transition level. 
*  a state with actions that wait could cause the SM to violate the basic assumption that SM ticks occur faster than events so that no events are missed.

### Conditions
Condition should follow the *Exactly one* rule.
* violating this rule is not wrong; the SM just becomes non-deterministic: BAD practice.
* cover all the possibilities with an *other* branch.
 * violating this rule is not wrong; the SM just has an implicit transition back to the same state: BAD practice.

When implementing an SM in C, the "other" transition may be implemented as a last `else` branch (with no expression) in the state's transitions if-else code.

## 3.6 Mealy actions
*Moore-type state machine*: state machine models that associate actions with states only.
*Mealy-type state machine*: allows actions on transitions too. 
 * can be used to prevent an transient state

![mealy-action](https://cloud.githubusercontent.com/assets/14265605/10141042/f0a7128e-65cf-11e5-91cd-59b11ab80f55.png)

When translating to C, a transition's actions appear in the transition switch statement, in the appropriate if-else branch.

## 3.7 Capture behavior as an SM

## 3.9 Capture/convert process of embedded system development
A two-step process:
1. *capture* the desired behavior using a computation model appropriate for the desired system behavior, such as SMs.

2. *convert* that captured behavior into an implementation, such as C that will run on a microprocessor.
* can be automatated. Use RIBS.

The concept of thinking of program behavior as state machines, even though the actual code is in C, is the most important concept in this book.

Other state machines:
* UML state machine
* dataflow models
* etc.

# Chap 4: Synchronous SMs

SMs can be extended to support time-interval behavior. For basic SMs, an SM tick was assumed to take a small, unknown, non-zero amount of time. The tick rate can instead be set to a specific rate such as 500 ms, known as the SM's period. An SM with a specific period is a synchronous SM, or synchSM for short.

e.g. Inactivity Alarm:
```c
/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 9/29/2015 9:40:36 PST
*/

#include "rims.h"

/*Define user variables and functions for this state machine here.*/
unsigned char cnt;
unsigned char _Clk;
void TimerISR() {
   _Clk = 1;
}

enum _States { _Init, _Count, _readCnt } _State;

TickFct_BabyMonitor() {
   switch(_State) { // Transitions
      case -1:
         _State = _Init;
         break;
         case _Init: 
         if (1) {
            _State = _Count;
         }
         break;
      case _Count: 
         if (!A0 && !A1) {
            _State = _readCnt;
         }
         else if (A0 || A1) {
            _State = _Init;
         }
         break;
      case _readCnt: 
         if (1) {
            _State = _Count;
         }
         break;
      default:
         _State = _Init;
   } // Transitions

   switch(_State) { // State actions
      case _Init:
         B0 = 0;
         cnt = 0;
         break;
      case _Count:
         ++cnt;
         break;
      case _readCnt:
         if (cnt >= 30)
             B0 = 1;
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodBabyMonitor = 1000; // 1000 ms default
   TimerSet(periodBabyMonitor);
   TimerOn();
   
   _State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_BabyMonitor();
      while(!_Clk); // delay will be interrupted by ISR
      _Clk = 0;
   } // while (1)
} // Main
```

## 4.4 Choosing a period
Choose the period as the *greatest common divisor* or *gcd* of the required time intervals, and then use multiple states (or counting within a state) to obtain the actual desired interval.
* less ticking as much as possible
 * lower power and potential to support more computations on a MC.

### counting the number of times a state ticks may be better than using multiple sates.
![counting-intervals](https://cloud.githubusercontent.com/assets/14265605/10169846/99228c54-6698-11e5-8b8b-4bcc162910ed.png)

## 4.5 Microcontrollers with timers
When the timer ticks, the microcontroller hardware interrupts the main program's execution.
*  Temporarily stop execution of the main program and call a special C function known as an *interrupt service routine* or *ISR*. 
* When that ISR function finishes executing, execution resumes where it previously stopped in the main program.

In RIMS, the ISR is called `TimerISR`.
```c
void TimerISR() {
   // user inserts code here
}
```
The user can insert code into the ISR that should be executed whenever the timer ticks. The user's own main code should ***never*** call `TimerISR()` directly.
* The user should never ***call*** a timer's ISR. But the user ***must*** write code inside the ISR so that the system does something in response to a timer's tick.
* The user's code never calls `TimerISR()` directly. `TimerISR()` is called automatically by the microcontroller hardware.

### Timer-related functions
`TimerISR()` `TimerSet(period)` `TimerOn()` 
* period is an unsigned short indicating the tick period in ms.
* To activate the timer, the user calls `TimerOn()`.

One use of the ISR is to set a global flag to 1.  the following code would toggle B0 every 1 second:
```c
#include "RIMS.h"

volatile unsigned char TimerFlag = 0; // use volatile

void TimerISR() {
   TimerFlag = 1;
}

void main() {

   B = 0;//Initialize output

   TimerSet(1000); // Timer period = 1000 ms (1 sec)
   TimerOn();      // Turn timer on

   while (1) {
      B0 = !B0;            // Toggle B0
      while(!TimerFlag) {} // Wait 1 sec
      TimerFlag = 0;
          // NOTE: better style would use a synchSM
        // This example just illustrates use of an ISR and flag
   }
}
```

## 4.7 State actions should never wait
synchSM behavior becomes indeterminate if state actions include waits.
* A key assumption in a synchSM is that every state's statements always run to completion faster than the synchSM's period. Actions are assumed to run in a small but non-zero time.

# Chap 5: Concurrent synchSMs
RIBS allows capture of multiple tasks using multiple tabs. 
e.g.:
```c
#include "rims.h"

/*This code will be shared between state machines.*/
volatile unsigned char TimerFlag = 0;
void TimerISR() {
   TimerFlag = 1;
}


enum SM1_States { SM1_LedOff, SM1_LedOn } SM1_State;

TickFct_Blink() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_LedOff;
         break;
      case SM1_LedOff:
         if (1) {
            SM1_State = SM1_LedOn;
         }
         break;
      case SM1_LedOn:
         if (1) {
            SM1_State = SM1_LedOff;
         }
         break;
      default:
         SM1_State = SM1_LedOff;
      } // Transitions

   switch(SM1_State) { // State actions
      case SM1_LedOff:
         B0 = 0;
         break;
      case SM1_LedOn:
         B0 = 1;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}

enum SM2_States { SM2_T0, SM2_T1, SM2_T2 } SM2_State;

TickFct_Rotate() {
   /*VARIABLES MUST BE DECLARED STATIC*/
/*e.g., static int x = 0;*/
/*Define user variables for this state machine here. No functions; make them global.*/
   switch(SM2_State) { // Transitions
      case -1:
         SM2_State = SM2_T0;
         break;
      case SM2_T0:
         if (1) {
            SM2_State = SM2_T1;
         }
         break;
      case SM2_T1:
         if (1) {
            SM2_State = SM2_T2;
         }
         break;
      case SM2_T2:
         if (1) {
            SM2_State = SM2_T0;
         }
         break;
      default:
         SM2_State = SM2_T0;
      } // Transitions

   switch(SM2_State) { // State actions
      case SM2_T0:
         B5 = 1;
         B6 = 0;
         B7 = 0;
         break;
      case SM2_T1:
         B5 = 0;
         B6 = 1;
         B7 = 0;
         break;
      case SM2_T2:
         B5 = 0;
         B6 = 0;
         B7 = 1;
         break;
      default: // ADD default behaviour below
         break;
   } // State actions
}
int main() {
   B = 0; //Init outputs
   TimerSet(1000);
   TimerOn();
   SM1_State = -1;
   SM2_State = -1;
   while(1) {
      TickFct_Blink(); // are those real concurrent?
      TickFct_Rotate();
      while (!TimerFlag);
      TimerFlag = 0;
   }
}
```
## 5.4 Converting synchSM local variables to C
When capturing multiple synchSMs using RIBS, the programmer must declare synchSM variables as static, else the generated C may not behave correctly because the variables will not retain their values across calls to the synchSM's tick function. RIBS contains a default comment reminding programmers of this requirement.

## 5.6 Task Communication
Use global variables.
* make sure only one task may write to a global variable, while any number of tasks may read that variable.

A ***handshake*** is a method for task X to cause task Y to carry out some behavior. A handshake involves two single-bit global variables, which we'll call req (for request) and ack (for acknowledge), as follows: Task X raises req, task Y responds by raising ack, task X responds by lowering req, and task Y responds by lowering ack. Task X is known as the master and task Y as the servant. 
* the handshake enables two tasks to correctly transfer data even if those tasks execute at different rates.
 * because the protocol does not require precise synchronization of timing.

## Queues: message passing

# Chap 6: I/O

## 6.1 Sampling Inputs
The synchSM period should be chosen to be as large as possible while still safely satisfying system requirements, in order to reduce microcontroller utilization. *Utilization* is the percentage of time that a microcontroller spends executing tasks, as opposed to waiting idly for the next timer tick. Lower utilization translates to a microcontroller consuming less power, supporting more synchSMs, having less delay from input changes to output changes, and having other desirable features.
* to minize MC utilization: set the period of synchSMs as large as possible.

Real systems do not use SMs; use synchSMs instead.

A system's designer must determine what is the *minimum interevent separation time* that should be detected by the system, meaning the minimum time between any two events on an input signal.
* Setting the period substantially less, as in 10% less or more, is safer.

## 6.2 Latency
The time between the input event and the triggered output event.
* humans can notice 100 ms latency. <50 ms is a good number.
Latency is introduced by:
* the synchSM's period
* having intermediate states that do calculations

## 6.3 Input conditioning

### Debouncing
Modern buttons typically do not bounce for more than 10-20 ms.
* If the sampling of the input signal is fast, the system may incorrectly treat the button as having been pressed multiple times.
* one simple solution: set the synchSM period to longer than a button's bouncing time.

### Filtering
Ignore certain input events, such as noise.
* known as spikes, or *glitches*.
 * While a slower sampling rate reduces the probability of detecting a glitch, a glitch could still be detected as a 1 if the sample is coincidentally taken during the glitch.
 * A more robust solution is to require that a 1 be detected for two or more consecutive samples before being confirmed as a legitimate 1. A larger period also helps.

#### Signal conditioner example:
Suppose we have a period bound of [50, 300]. If we introduce filtering that requires two consecutive 1 samples, the period must be less than 300 / 2 = 150 ms (and still greater than 50 ms). We may choose 100 ms as the period.

![filtering](https://cloud.githubusercontent.com/assets/14265605/10241310/cb11cc00-68ac-11e5-99e5-dc842b542242.png)
* No solution can completely filter out all glitches without also losing real events too.
* Glitches could cause a 1 to temporarily become a 0, and so we might also require that two consecutive 0s be detected before treating the input as having changed from a steady 1 to a 0.

## 6.4 Output glitches
Care should be taken to avoid generating output glitches when output values should instead be steady.
```c
// B could pick up glitch and accidentally increased.
#include "RIMS.h"

unsigned char GetBit(unsigned char x, int k) {
    return ((x & (0x01 << k)) != 0);
}

void main() {
    unsigned char i;
    B = 0; // initialize output
    while (1) {
        B=0;
        for (i=0; i<8; i++) {
            if (GetBit(A, i)) {
                B++;
            }
        }
    }
}
```

```c
// single assignment operation for B
// anti-glitch
#include "RIMS.h"

unsigned char GetBit(unsigned char x, int k) {
    return ((x & (0x01 << k)) != 0);
}

void main() {
    unsigned char cnt; //avoid glitches on B
    unsigned char i;
    B = 0; // initialize output
    while (1) {
        cnt=0;
        for (i=0; i<8; i++) {
            if (GetBit(A, i)) {
            cnt++;
            }
        }
    B = cnt;
    }
}
```

![glitching](https://cloud.githubusercontent.com/assets/14265605/10241493/0b9b1e32-68af-11e5-824b-050e6325e4b2.png)
* potentially glitching.

![glitching_b](https://cloud.githubusercontent.com/assets/14265605/10241558/98c83150-68af-11e5-8266-0abb6ef35b91.png)
* InputCounter_B avoids output glitching.

## 6.5 I/O electrical issues
Output power may not be able to drive devices
* Connecting a *Buffer IC* between the microcontroller and another device helps boost the voltage and/or current of the microcontroller to match the requirements of the other device.

![buffer-ic](https://cloud.githubusercontent.com/assets/14265605/10242122/218515b8-68b4-11e5-8d98-97f91c27e674.png)

Need a lower voltage
* use a *load resistor*.

### Pull-up resistor configuration
Microcontrollers commonly support two configurations for an input pin

1. *normal*
 * In the normal configuration, if a pin is not driven with a 0 or a 1 (e.g., if the pin is unconnected), reading the pin yields an indeterminate value; the pin should thus always be driven.

2. *pull-up*. 
* if a pin is not driven with a 0 or a 1, reading the pin yields a 1. 

![pull-up](https://cloud.githubusercontent.com/assets/14265605/10241910/842d3db4-68b2-11e5-8ca4-7b38123c6e80.png)

A pull-up configuration is useful for a passive button, for example, wherein pressing the button causes a connection between the button's terminals and hence a connection between 0 and a microcontroller input pin (so the pin is read as 0), while releasing the button causes disconnection (so the pin is read as 1).

## 6.6 Too Few Pins
Consider wishing to drive 12 LEDs but with only 8 microcontroller output pins.
Use external registors: *time-multiplexed output*
![time-multiplexed-output](https://cloud.githubusercontent.com/assets/14265605/10242246/170e617e-68b5-11e5-9eaa-8f33ad3168dd.png)

* Time-multiplexed output with rapid refresh: use transistors instead of registers (a transistor passing its input to its output if the control is 1, else disconnecting its output).
 * As long as value 1 comes in frequently enough, the LED will seem to be on (refreshed with value 1).

Similarly for input: *Time-multiplexed input*
* a microcontroller with too few inputs may expand its effective inputs via time-multiplexed reading of input registers (in conjunction with an external multiplexor), or via rapid scanning, which is the input-version of refresh.

Instead of N*M pins, keypads commonly have only N+M pins, or 8 pins for the above 4x4 keypad, arranged as shown in the figure below:
![keypad](https://cloud.githubusercontent.com/assets/14265605/10242429/623cabaa-68b6-11e5-8c31-0b8f07e34322.png)

A microcontroller can poll each button one at a time, a process known as scanning, to detect whether that button is pressed.
* a simple scanning algorithm:

```c
// Returns 1 if the key at row/col is pressed. Returns 0 otherwise.
unsigned char GetSingleButton(unsigned char row, unsigned char col)
{
   // Set B3..B0 outputs to 1 except the bit at position 'col',
   // ensuring B7..B4 are not modified.
   B = (B | 0x0f) & ~(1 << col); // disconnect only B[col]
  
   //  Now read the input pin at position 'row'. If the button at 
   //  row/col is pressed, it will go from pull-up state (1) to 0.
   return ((A & (1 << row)) == 0 ? 1 : 0); // combined with only this row to give a total disconnection.
}


// Returns an index (1 through 16) corresponding to the button pressed, 
// or 0 if no button is pressed. Scanning order is left to right and 
// top to bottom, returning when finding the first pressed button.

unsigned char ScanKeypad()
{
   unsigned char i, j;
   for (i=0; i<4; i++) {
      for (j=0; j<4; j++) {
         if (GetSingleButton(i, j)) {
            return (i * 4) + j + 1;
         }
      }
   }
   return 0;
}
```
 * This scanning algorithm does not detect multiple button presses; instead, the algorithm returns the pressed button that is closest to the top and left, giving such buttons higher priority. If multiple button presses must be detected, the function needs to maintain a list of buttons that are discovered to be pressed, and return that list to the caller at the end of a full scan.

# Chap 7: Peripherals

## Pulse Width Modulation (PWM)

A programmable component that generates pulses to achieve a specified period and duty cycle.

***duty cycle***: the percentage of time the signal is high.
* A signal with a duty cycle of 50% is called a *square wave*.

A ***pulse width modulator*** or ***PWM*** is a programmable component that generates pulses to achieve a specified period and duty cycle. 

### synchSM capture and C implementation
![pwm](https://cloud.githubusercontent.com/assets/14265605/10249016/a8b1348e-68e8-11e5-8530-b033bda801a0.png)

```c
/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 10/2/2015 9:27:18 PST
*/

#include "rims.h"

/*Define user variables and functions for this state machine here.*/
unsigned char H, L;
unsigned char i;
unsigned char _Clk;
void TimerISR() {
   _Clk = 1;
}

enum _States { _Init, _pwmH, _pwmL } _State;

TickFct_PWM() {
   switch(_State) { // Transitions
      case -1:
         _State = _Init;
         break;
         case _Init: 
         if (1) {
            _State = _pwmH;
            i = 0;
         }
         break;
      case _pwmH: 
         if (i < H) {
            _State = _pwmH;
         }
         else if (!(i < H)) {
            _State = _pwmL;
            i = 0;
         }
         break;
      case _pwmL: 
         if (i < L) {
            _State = _pwmL;
         }
         else if (!(i < L)) {
            _State = _pwmH;
            i = 0;
         }
         break;
      default:
         _State = _Init;
   } // Transitions

   switch(_State) { // State actions
      case _Init:
         B0 = 0;
         H = 8;
         L = 12;
         break;
      case _pwmH:
         B0 = 1;
         ++i;
         break;
      case _pwmL:
         B0 = 0;
         ++i;
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodPWM = 100;
   TimerSet(periodPWM);
   TimerOn();
   
   _State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_PWM();
      while(!_Clk);
      _Clk = 0;
   } // while (1)
} // Main
```

## 7.2 UARTs

*universal asynchronous receiver/transmitter* (UART): To conserve limited output pins, microcontrollers typically include a hardware device, called a UART, that automatically transmits eights bits of data serially, meaning one bit at a time, over a single pin.

The UART must first be activated by the RIMS built-in function `UARTOn()`.
* then the system will give you an additional tx pin an additional rx pin.

Using RIMS' UART for transmit:
```c
#include "RIMS.h"

void main() {
   UARTOn(); // activate UART
   while (1) {
      while (!TxReady); // wait for UART transmit ready. TxReady is a global flag variable which is automatically set to 0 by the UART hardware while busy transmitting, and to 1 when done transmitting and thus ready for T to be written again.
      T = 0x61; // transmit 01100001 serially over UART output pin 
   }
}
```

Using RIMS' UART for receive:
```c
#include "RIMS.h"

volatile unsigned char RxFlag = 0;
void RxISR() { // Called automatically when UART receives new data
   RxFlag = 1; 
}

void main()
{
   UARTOn(); 
   while(1){
      while (!RxFlag); // wait until UART receives new data
      RxFlag = 0;
      B = R; // write received data to B output, R is a special global variable. The UART informs the program when new data has been received into R by automatically calling an ISR function "void RxISR()" that the programmer must define.
   }
}
```
* Until R is read by the program, the UART hardware will not overwrite R.

A common error is to forget to turn on the UART. In RIMS, a call to the UartOn() function turns on the UART. Another common error is to fail to read R when data is there, thus preventing the hardware from calling the RxISR function.

TxReady is set to 1 by hardware when there is a transmission in progress. If T is overwritten during a transmission, the transmitted data could be corrupted.

Reading the contents of R allows the UART to receive new data. Reading the contents of R does not call RxISR. 
When 8 bits of data have been successfully received, RxISR is called by hardware. The received data is then available in the RIMS global variable R.

## 7.3 Analog-digital conversion
An ADC is a mixed-signal peripheral device, which is a device that has a mix of both analog and digital I/Os. An ADC, for example, has an analog input and a digital output.

### Three basic parameters
1. The range of analog values to be converted.

2. The range of digital values to correspond with the analog voltages.

 * The number of bits used by an ADC is also referred to as the ADC's *precision*. 

3. The rate at which an ADC is capable of converting analog values to digital.

 * this rate is referred to as the sampling rate.

 * *quantization error*: suppose input range of -2.0 to +2.0 v and 8-bit ADC, 4.0 / 256 = 0.015625.

Most microcontrollers have one or more on-chip ADCs as standard peripherals. Such microcontrollers use special-purpose registers to access the ADC and may offer a number of configuration options. For instance, a software developer may configure a microcontroller's on-chip ADC to operate using a certain analog-input range or with a specific sampling rate or bit precision.

### Implementation
*successive approximation*: A common approach for analog to digital conversion where a controller repeatedly guesses (using binary search) at a digital value using a comparator.
![adc-hardware](https://cloud.githubusercontent.com/assets/14265605/10266537/ad2154aa-6a31-11e5-8f76-ca0765751afe.png)

# Chap 8: Task Scheduler
An earlier section simplified task execution in C by using the same period for every task. On the other hand, different periods are sometimes desired.

## 8.1 Converting different-period tasks to C
Different-period tasks can be converted to C via a method that uses new variables for counting timer ticks.
* the timer period MUST evenly divide all task periods. Choosing the timer period as the greatest common divisor (GCD) ensures this. 
* adding the elapsedTime variables for counting timer periods results in cluttered main code, especially if there are tens of tasks. Furthermore, each task has several items declared in different places (state variable, period, elapsed time counter, etc.) The code is becoming harder to maintain. A more structured approach is needed.

### Task `struct` in C

```c
typedef struct task {
   int state;                  // Task's current state
   unsigned long period;       // Task period
   unsigned long elapsedTime;  // Time elapsed since last task tick
   int (*TickFct)(int);        // Task tick function
} task;

// Defining two new tasks
task BL_task;
task TL_task;

// Setting a task's fields
BL_task.state = BL_SMStart; // Indicates initial state
BL_task.period = 1500;      // Tick function should be called every 1500 ms
BL_task.elapsedTime = 1500; // Time since last tick; initial 1500 ms
                              // causes tick function to be called at program start

// Setting a task's tick function field
BL_task.TickFct = &TickFct_BlinkLed;

// Task tick function with current state parameter
int TickFct_BlinkLed(int state) {
   switch(state) {
   ...
      state = ...
      ...
   return state;
}

// Array holding all tasks
task tasks[2];
const unsigned short tasksNum = 2;

int main() {
  unsigned char i=0;
  tasks[i].state = BL_SMStart;
  tasks[i].period = 500;
  tasks[i].elapsedTime = tasks[i].period; // By setting the elapsed time to the period value, the task will execute immediately at time 0.
  tasks[i].TickFct = &TickFct_BlinkLed;  
  i++;
  tasks[i].state = TL_SMStart;
  tasks[i].period = 500;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_ThreeLeds;

  //...
}
```

## 8.3 We can now use a for loop that processes each task in a round-robin manner

```c
// For each task, call task tick function if task's period is up
for (i=0; i < tasksNum; i++) {
   if (tasks[i].elapsedTime >= tasks[i].period){
      // Task is ready to tick, so call its tick function
      tasks[i].state = tasks[i].TickFct(tasks[i].state); 
      tasks[i].elapsedTime = 0; // Reset the elapsed time
   }
   tasks[i].elapsedTime += tasksPeriodGCD;
}
```

The for loop code carries out what is known as a ***task scheduler***. A task scheduler determines when each task should be executed in a multiple-task system. A task scheduler is commonly found inside the code for an operating system (OS), but can instead be included directly in user code as above, which is especially useful in the absence of an OS.

```c
void TimerISR() {
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) { // Heart of the scheduler code
     if ( tasks[i].elapsedTime >= tasks[i].period ) { // Ready
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = 0;
     }
     tasks[i].elapsedTime += tasksPeriodGCD;
  }
}

int main() {
  // initialize tasks.
  // ...

  TimerSet(tasksPeriodGCD);
  TimerOn();

  while(1) {
     Sleep();
  }
  return 0;
}
```

### A full task scheduler code example
```c
#include "RIMS.h"

typedef struct task {
  int state; // Current state of the task
  unsigned long period; // Rate at which the task should tick
  unsigned long elapsedTime; // Time since task's previous tick
  int (*TickFct)(int); // Function to call for task's tick
} task;

task tasks[2];

const unsigned char tasksNum = 2;
const unsigned long tasksPeriodGCD = 500;
const unsigned long periodBlinkLED = 1500;
const unsigned long periodThreeLEDs = 500;

enum BL_States { BL_SMStart, BL_s1 };
int TickFct_BlinkLED(int state);

int TickFct_ThreeLEDs(int state);
enum TL_States { TL_SMStart, TL_s1, TL_s2, TL_s3 };


void TimerISR() {
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) { // Heart of the scheduler code
     if ( tasks[i].elapsedTime >= tasks[i].period ) { // Ready
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = 0;
     }
     tasks[i].elapsedTime += tasksPeriodGCD;
  }
}

int main() {
  // initialize tasks
  unsigned char i=0;
  tasks[i].state = BL_SMStart;
  tasks[i].period = periodBlinkLED;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_BlinkLED;
  ++i;
  tasks[i].state = TL_SMStart;
  tasks[i].period = periodThreeLEDs;
  tasks[i].elapsedTime = tasks[i].period;
  tasks[i].TickFct = &TickFct_ThreeLEDs;
  // initialize tasks end

  TimerSet(tasksPeriodGCD);
  TimerOn();
  
  while(1) {
     Sleep();
  }
  return 0;
}

int TickFct_BlinkLED(int state) {
  switch(state) { // Transitions
     case BL_SMStart: // Initial transition
        B0 = 0; // Initialization behavior
        state = BL_s1;
        break;
     case BL_s1:
        state = BL_s1;
        break;
     default:
        state = BL_SMStart;
   } // Transitions

  switch(state) { // State actions
     case BL_s1:
        B0 = !B0;
        break;
     default:
        break;
  } // State actions
  return state;
}

int TickFct_ThreeLEDs(int state) {
  switch(state) { // Transitions
     case TL_SMStart: // Initial transition
        state = TL_s1;
        break;
     case TL_s1:
        state = TL_s2;
        break;
     case TL_s2:
        state = TL_s3;
        break;
     case TL_s3:
        state = TL_s1;
        break;
     default:
        state = TL_SMStart;
   } // Transitions

  switch(state) { // State actions
     case TL_s1:
        B2 = 1; B3 = 0; B4 = 0;
        break;
     case TL_s2:
        B2 = 0; B3 = 1; B4 = 0;
        break;
     case TL_s3:
        B2 = 0; B3 = 0; B4 = 1;
        break;
     default:
        break;
  } // State actions
  return state;
}
```
* The above code can serve as the basis for most systems having multiple synchSMs. It can be copied and pasted into RIMS, and then the tick functions replaced by one's own synchSMs (commonly more than just two).

# Chap 9: Programming Issues

## An *lookup table* implementation

```c
// Array for converting Celsius to Fahrenheit.
const unsigned char C2F_uc_array[256] = {32, 34, 36, 37, 39, 41,
  ..., 255, 255, 255};    // middle values are omitted (...)

// Function using array for converting Celsius to Fahrenheit.
inline unsigned char C2F_uc(unsigned char C) {
  return C2F_uc_array[C];
}

```
* The tradeoff present with a lookup table is the larger code size.

## 9.2 Fixed-point programming

*Floating-point* refers to the fact that the same number of bytes can represent real numbers with the decimal point in different places, e.g., 25.61 and 1.428 both have four significant digits but have the decimal point in different places. 

The decimal point can thus move, or *float*. The advantage of floating-point implementation is that a real number can be represented over a wide range of magnitudes while maintaining a constant number of significant digits. 

RIMS does not currently support floating-point.

Rounding problems for integers are eliminated due to the use of real numbers.

Unfortunately, floating-point operations typically run much slower than integer operations.
* briefly, floating-point involves representing the significand and the exponent separately (a real number is composed as: significand x 10^exponent), so arithmetic operations internally may require shifting to obtain the same exponent, performing separate operations on the significand and the exponents, rounding, and normalizing.

Embedded processors typically don't have special floating-point hardware and thus, if they support floating-point at all, then they perform floating-point operations using software functions, resulting in slowdowns of 100x or more compared to integer operations.

## fixed point implementation of real numbers
In a fixed-point representation, we need to decide on the number of bits necessary to represent the whole part of the real number (w) and the number of bits necessary to represent the fractional part of the real number (f).

For example, if using the short type, we may chose w=12 and f=4. Under this representation, written using the notation "(12.4)", we can represent a real number in the range of (-2048.0000 to 2047.9375). We can convert a floating point number (within the proper range) to the fixed-point representation and the other way around as shown in the pair of functions below:

```c
#include <assert.h>

const double MIN_REAL = (short)0x8000 / 16.0;
const double MAX_REAL = (short)0x7fff / 16.0;

short conv_to_fixed_point(double x) {
   if( x < MIN_REAL ) {
     assert( 0 /* Underflow */ );
   }
   else if( x > MAX_REAL ) {
     assert( 0 /* Overflow */ );
   }
   else {
     return (short)(x * 16.0);
   }
}

double conv_to_floating_point(short x) {
   return x / 16.0;
}
```
For example, 2047.9375 would be converted to (12.4) fixed point representation as (short) (2047.9375 * 16.0) = (short)(32767.00000) = 32767. As another example, 3.14 would be converted as (short)(3.14*16.0) = (short)(50.24) = 50. Note that 50 looks nothing like 3.14, but it indeed is the (12.4) fixed point representation of 3.14.

In the above functions, a fixed-point version of a real number is obtained by multiplying it with 2^f. Likewise, a floating-point version of a fixed point number is obtained by dividing by 2^f. As a result, the precision of the real number (i.e., the fractional portion) is determined by f while the range is determined by the choice of w.

Fractions representable using 4 bits:
0 / 16.0 = 0.000000
1 / 16.0 = 0.062500
2 / 16.0 = 0.125000
3 / 16.0 = 0.187500
4 / 16.0 = 0.250000
5 / 16.0 = 0.312500
6 / 16.0 = 0.375000
7 / 16.0 = 0.437500
8 / 16.0 = 0.500000
9 / 16.0 = 0.562500
10 / 16.0 = 0.625000
11 / 16.0 = 0.687500
12 / 16.0 = 0.750000
13 / 16.0 = 0.812500
14 / 16.0 = 0.875000
15 / 16.0 = 0.937500

### Given the number 20 in "5.3" fixed-point format
20: 0001 0100
whole part = 0001 -> whole part = 2.0
fraction part = 100 -> fraction part = 4 * precision
 * precision = 1/(2^3) = 1/8
 fraction part = 4 * 1/8 = 0.5

20 in "5.3" is 2.5

### Convert the number 4.75 to "5.3" fixed-point format
4.75 * 2^f = 4.75 * 8 = 38
38: 00100110

#### Addition and Substraction
Once a real number is converted to its fixed-point representation, we can perform the basic add/subtract operations as shown below. Note that all fixed-point numbers must use the same representation:
```c
#include <assert.h>

const short MIN_FP = 0x8000;
const short MAX_FP = 0x7fff;

inline short fixed_point_check(long x) {
   if( x < MIN_FP ) {
     assert( 0 /* underflow */ );
   }
   else if( x > MAX_FP ) {
     assert( 0 /* overflow */ );
   }
   else {
     return (short)x;
   }
}

inline short fixed_point_add(short x, short y) {
   return fixed_point_check( (long)x + (long)y );
}

inline short fixed_point_sub(short x, short y) {
   return fixed_point_check( (long)x - (long)y );
}
```

#### Multiplication and Division
```c
inline short fixed_point_mul(short x, short y) {
   long r = (long)x * (long)y;
   return fixed_point_check( r >> f ); // with "12.4", f = 4
}

inline short fixed_point_div(short x, short y) {
   long r = ((long)x << f) / (long)y; // with "12.4", f = 4
   return fixed_point_check( r );
}
```
When multiplying two fixed-point integers, the decimal point shifts left by some number of bits (f in this example). To illustrate, consider multiplication of the decimal numbers 3.1 and 2.7. In the first step, we multiply the two integers 31 and 27 to obtain the integer 837. Then, we place the decimal point two places from the right to obtain 8.37. To maintain our fixed-point format, we need to re-adjust the decimal point by shifting right exactly f bits. Likewise, when dividing, we need to shift the result left exactly f bits, which we do prior to the integer division so as to not lose any precision.

#### Comparison
The integer comparison operations work on fixed-point number without modifications.

## Using lookup tables to implement fixed-point arithmetics

E.g. Implementation of `compute_distance` with "12.4" format:

First try:
```c
#include <math.h>
typedef struct {
   short x, y;  // fixed point format (12.4)
} point_t;

short compute_distance(point_t p1, point_t p2) {
   short t1 = fixed_point_sub(p2.x, p1.x);
   short t2 = fixed_point_sub(p2.y, p1.y);
   short t3;
   double t4;
   t1 = fixed_point_mul(t1, t1);
   t2 = fixed_point_mul(t2, t2);
   t3 = fixed_point_add(t1, t2);
   t4 = conv_to_floating_point(t3); // to use sqrt()
   t4 = sqrt(t4);
   return conv_to_fixed_point(t4); // has to convert back
}
```

We can implement a fixed-point version of `sqrt`, or better-off we can use Lookup tables:

```c
// pre-computing square roots
#include <stdio.h>
#include <math.h>
#include <assert.h>

const double MIN_REAL = (short)0x8000 / 16.0;
const double MAX_REAL = (short)0x7fff / 16.0;

short conv_to_fixed_point(double x) {
   if( x < MIN_REAL ) {
     assert( 0 /* underflow */ );
   }
   else if( x > MAX_REAL ) {
     assert( 0 /* overflow */ );
   }
   else {
     return (short)(x * 16.0);
   }
}

double conv_to_floating_point(short x) {
   return x / 16.0;
}

void main() {
   short i;
   printf("const short sqrt_loopkup_table[] = {\r\n");
   for(i=0; i<32678; i++) {
     printf("   %i,\n",conv_to_fixed_point(sqrt(conv_to_floating_point(i))));
   }
   printf("}\r\n");
   return 0;
}
```
results in:
```c
const short sqrt_loopkup_table[] = {
   0,
   4,
   5,
   6,
   8,
   8,
   9,
   10,
   11,
   12,
   12,
   13,
   13,
   14,
   14,
   15,
   16,
   16,
   16,
   17,
   17,
   18,
   18,
   19,
   19,
   ...
};
```
Now the efficient implementation:
```c
short fixed_point_sqrt(short x) {
   if( x < 0 ) {
     assert( 0 /* Can't take sqrt of a negative number */ );
   }
   else {
     return sqrt_loopkup_table[x];
   }
}

typedef struct {
   short x, y;  // fixed point format (12.4)
} point_t;

short compute_distance(point_t p1, point_t p2) {
   short t1 = fixed_point_sub(p2.x, p1.x);
   short t2 = fixed_point_sub(p2.y, p1.y);
   short t3;
   t1 = fixed_point_mul(t1, t1);
   t2 = fixed_point_mul(t2, t2);
   t3 = fixed_point_add(t1, t2);
   return fixed_point_sqrt(t3);
}
```
The above example used 64K bytes of memory to store the square root results. This is probably far more than the amount of code necessary to implement the square root function using an algorithm. However, the lookup approach will be much faster than the algorithmic version.

# Chap 10: Utilization and scheduling

## Timer-overrun exception
Detecting timer overrun within scheduler code:
```c
unsigned char processingRdyTasks = 0;
void TimerISR() {
  unsigned char i;
  if (processingRdyTasks) { // 1.1 detect if overrun exception
     printf("Period too short to complete tasks.\r\n"); // 1.2 handle the exception
     return;
  }
  processingRdyTasks = 1; // 2. raise flag, ready to process tasks

  for (i = 0; i < tasksNum; ++i) { // schedular code chooses the task 
     if ( tasks[i].elapsedTime >= tasks[i].period ) { // Ready
        tasks[i].state = tasks[i].TickFct(tasks[i].state); // 3. process tasks
        tasks[i].elapsedTime = 0;
     }
     tasks[i].elapsedTime += tasksPeriodGCD;
  }
  processingRdyTasks = 0; // 4. lower flag
}
```

## Utilization
***Utilization*** is a value that a programmer can compute to predict whether timer overrun will occur. For a microcontroller executing a single task, utilization can be defined as:
* utilization = time-per-task-tick / task-period
 * A task requiring 450 ms per tick and with a 1000 ms period has utilization of 450 ms / 1000 ms or 0.45. Utilization is normally presented as a percentage, such as 45%. If that same task had a period of 300 ms, utilization would be 450 ms / 300 ms, or 150%. Utilization greater than 100% indicates timer overrun will occur.

Solutions:
1. Increase the task period.

2. Reduce the time for actions per tick.

 * more efficient code
 * splitting a synchSM state into two states, each doing half the work load and *transitions* to the other.


## Utilization analysis: Worst-case senario

Note that utilization analysis usually ignores the additional C instructions required to implement a task in C, such as the switch statement instructions in a tick function, or the scheduler code. For typical-sized tasks and typical-speed microcontrollers, the number of such "overhead" instructions is negligibly small.

Microcontroller datasheets may provide details. 

It also depends on the C compiler.

Experiments and analysis tools will also help.

*Tips*: 1 GHz yields 1 nanosec. 1 MHz yields 1 microsec

## Utilization for multiple tasks

### microcontroller usage diagram
![microcontroller-usage-diagram](https://cloud.githubusercontent.com/assets/14265605/10281857/f71de2a6-6b3c-11e5-9908-b25ae6c8488a.png)

* a dotted block represents a ready task that isn't executing, and a solid block represents a task that is executing. 

When two or more tasks have different periods: use the *hyperperiod* method:
![microcontroller-usage-diagram-hyperperiod](https://cloud.githubusercontent.com/assets/14265605/10282043/24ec3ea2-6b3e-11e5-81ab-f08956f18eb4.png)

* every 600 ms, the pattern of execution should repeat; the *hyperperiod* is 600 ms.

![microcontroller-usage-diagram-hyperperiod-timeoverrun](https://cloud.githubusercontent.com/assets/14265605/10282234/41474d2a-6b3f-11e5-997e-cc0e9ca1b252.png)

* even utilization < 100% for multiple tasks senario, time overrun could still occur.
 * A fast-ticking task may yield timer overrun if the other task has a long WCET(worst case execution time), causing the scheduler to be busy executing that task when a timer tick occurs.

* The key here is that utilization is a computed value that does not take into account the details of the scheduler. While utilization is useful initially, a more thorough analysis with scheduler awareness reveals a more accurate picture.

## Jitter
A task's jitter is the delay between the time that a task was *ready* to start executing and the time that it *actually starts* executing on the microcontroller.
* Jitter is caused by two or more tasks being ready; the scheduler executes one of those tasks, causing jitter in the other tasks. 

An example where jitter matters is a system where Task1 blinks an LED repeatedly, and Task2 computes something. Jitter for Task1 might cause the LED to blink in an inconsistent manner, whereas jitter for Task2 might be more acceptable. Another example is a system where Task1 monitors an input for a change, and Task2 computes something. Jitter for Task1 might cause an input event be missed, or introduce extra latency.

Good scheduling approaches:
* avoid time overruns
* reducing jitter

## Scheduling
By convention, all tasks are initially considered ready when the system is started (namely, at time 0). Thus, each task should execute once at startup, and then wait until its period elapses to execute again.
![task-status-sm](https://cloud.githubusercontent.com/assets/14265605/10282427/7d66d090-6b40-11e5-8dc1-b3adc85b50d1.png)

Scheduling is necessary because a microcontroller can only execute one task at a time. A scheduler is the code responsible for scheduling tasks. Scheduling does ***not*** impact utilization analysis, but does impact features of task execution, such as:
* timer overrun (causing missed tasks), 
* jitter, 
* meeting deadlines.

***deadline***: the time by which a task must complete after becoming ready. 
* Else, the system has "missed a deadline" and is considered to have not executed correctly.

If a programmer does not specify a deadline for a task, then by default the deadline is equal to the period.
![missed-deadline](https://cloud.githubusercontent.com/assets/14265605/10282581/31ac8568-6b41-11e5-8e08-a904694b0c60.png)

* The diagram shows that task TL misses a deadline during the task's first execution of each hyperperiod, due to jitter caused by the scheduler executing BL before TL.

A *static-priority* scheduler assigns a priority to each task before the tasks begin executing, and those priorities don't change during runtime.
* common: highest priority to tasks with shortest deadlines.

###### ideas to implement priority
A more general approach may introduce new fields to the task structure to store a deadline value, a WCET value, or a programmer-assigned priority number, and then a function can be called that sorts the tasks array by a particular field before the main code's while (1) loop.

![priority-queue-based-scheduler](https://cloud.githubusercontent.com/assets/14265605/10283126/d586739a-6b43-11e5-8da5-9dae4796a440.png)

* B experiences average jitter of 25 ms and C of (50+25)/2 = 37.5 ms, but no deadlines are missed.

A *dynamic-priority* scheduler determines task priorities as the program runs, meaning those priorities may change.
* A common dynamic approach assigns ready tasks with the *earliest deadlines* the highest priority.
 * known as *earliest deadline first (EDF)*
* Dynamic-priority schedulers may reduce jitter and missed deadlines, at the expense of more complex scheduler code. EDF scheduling is commonly considered when some tasks are triggered by events rather than all tasks being periodic.

## Preemptive and non-preemptive scheduler
A *preemptive* scheduler may temporarily stop (preempt) an executing task if a higher-priority task becomes ready. A *non-preemptive* scheduler does not stop an executing task; the task must run to completion before the scheduler can execute another task.

### Preemptive simple task scheduler
```c
typedef struct task {
  unsigned char running; // 1 indicates task is running
  int state;             // Current state of state machine.
  unsigned long period; // Rate at which the task should tick
  unsigned long elapsedTime; // Time since task's previous tick
  int (*TickFct)(int); // Function to call for task's tick
} task;
// ...
unsigned char runningTasks[3] = {255}; // Track running tasks, [0] always idleTask
const unsigned long idleTask = 255; // 0 highest priority, 255 lowest
unsigned char currentTask = 0; // Index of highest priority task in runningTasks

void TimerISR() {
  unsigned char i;
  for (i=0; i < tasksNum; ++i) { // Heart of scheduler code
     if (  (tasks[i].elapsedTime >= tasks[i].period) // Task ready
         && (runningTasks[currentTask] > i) // Task priority > current task priority
         && (!tasks[i].running) // Task not already running (no self-preemption)
        ) {
        
        tasks[i].elapsedTime = 0; // Reset time since last tick
        tasks[i].running = 1; // Mark as running
        
        currentTask += 1;
        runningTasks[currentTask] = i; // Add to runningTasks
        
        tasks[i].state = tasks[i].TickFct(tasks[i].state); // Execute tick
      
        tasks[i].running = 0; // Mark as not running
        runningTasks[currentTask] = idleTask; // Remove from runningTasks
        currentTask -= 1;
     }
     tasks[i].elapsedTime += tasksPeriodGCD;
  }
}
```

* First, note that the elapsedTime data member is reset to 0 before executing the task, whereas the non-preemptive version resets elapsedTime after task execution. 
 * This modification is to ensure that a preempted task does not attempt to execute again immediately during the next scheduling. 
* Second, the task is marked as running and added to the currentTasks array before being executed. Once the task completes, the task is marked as not running and removed from the array.

Using a preemptive scheduler introduces *nested interrupts*, which occur when an interrupt handler interrupts itself.

A *critical section* indicates a section of code that must not be interrupted (or accessed by multiple tasks at once, in the context of multi-threaded systems).

```c
// TimerISR with critical section protection
void TimerISR() {
  unsigned char i;
  for (i=0; i < tasksNum; ++i) { // Heart of scheduler code
     if (  (tasks[i].elapsedTime >= tasks[i].period) // Task ready
         && (runningTasks[currentTask] > i) // Task priority > current task priority
         && (!tasks[i].running) // Task not already running (no self-preemption)
        ) {
        
        DisableInterrupts(); // Enter critical section. Prevent timer interrupts.
        tasks[i].elapsedTime = 0; // Reset time since last tick
        tasks[i].running = 1; // Mark as running
        
        currentTask += 1;
        runningTasks[currentTask] = i; // Add to runningTasks
        TimerOn(); //Exit critical section
        
        tasks[i].state = tasks[i].TickFct(tasks[i].state); // Execute tick
      
        DisableInterrupts(); // Enter critical section. Prevent timer interrupts.
        tasks[i].running = 0; // Mark as not running
        runningTasks[currentTask] = idleTask; // Remove from runningTasks
        currentTask -= 1;
        TimerOn(); //Exit critical section
     }
     tasks[i].elapsedTime += tasksPeriodGCD;
  }
}
```
* *context switching* is handled automatically via nested ISR function calls and returns.

### RTOS

Embedded systems sometimes use a real-time operating system (RTOS) 

A key part of an RTOS is a mechanism to allow users to define tasks (sometimes called threads or processes), including their periods and priorities. Another key part is a task scheduler, such as the scheduler we created above.

## Triggered synchSMs

Sometimes a synchSM tick just samples an input (such as input A0) to detect a change, a process known as *polling*. To reduce the microcontroller being used for polling, some microcontrollers come with ***special hardware*** that detects a change on an input and calls a special ISR in response such as `inputChangeISR()`. 

***Benefits***:
* Reducing polling reduces microcontroller utilization, decreases jitter, reduces missed deadlines, and/or enables more tasks to be implemented on the microcontroller.
* Using existing special hardware that detects events on pins can yield the additional benefit of sampling the pin faster than sampling achieved by polling done by a task on a microprocessor.

![triggered-synchsm](https://cloud.githubusercontent.com/assets/14265605/10286670/7aaa22e4-6b54-11e5-8c56-0acecfd9220d.png)
* The trigger detection occurs by some other means *outside* the synchSM.

In scheduling terminology, a task triggered by an event is known as an *aperiodic* task, in contrast to a *periodic* task that ticks at a known rate.

RIMS does not currently have built-in support for detecting events on pins, but RIMS does have built-in support for detecting a UART character receive, and thus a UART receive can be used to trigger a synchSM. 

### Example synchSM triggered by a UART receive
```c
#include "RIMS.h"

typedef struct task {
   signed   char state;
   unsigned long period;
   unsigned long elapsedTime;
   unsigned char active; // 1: active, 0: inactive
   int (*TickFct)(int);
} task;

task tasks[2]; // Global so visible to tick fct and ISR
const unsigned short tasksNum = 2;

const unsigned long BL_period = 1500;
const unsigned long TLT_period = 500;
const unsigned long tasksPeriodGCD = 500;

enum BL_States { BL_SMStart, BL_LEDOff, BL_LEDOn };
int TickFct_BlinkLed(int state);

enum TLT_States { TLT_SMStart, TLT_T0, TLT_T1, TLT_T2, TLT_Init, TLT_Inactive };
int TickFct_ThreeLedsTriggered(int state);

unsigned char processingRdyTasks = 0;
void TimerISR() {
   unsigned char i;
   if (processingRdyTasks) {
      printf("Timer ticked before task processing done.\r\n");
   }
   else { // Heart of the scheduler code
      processingRdyTasks = 1;
      for (i=0; i < tasksNum; ++i) {
         if (tasks[i].elapsedTime >= tasks[i].period && tasks[i].active) {
         // Ready
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].elapsedTime = 0;
         }
         if (tasks[i].active) { tasks[i].elapsedTime += tasksPeriodGCD; }
      }
      processingRdyTasks = 0;
   }
}


volatile unsigned char RxFlag=0;
volatile unsigned char RxData=0;

const unsigned int tasksTLTIndex = 1; // Reference the TLT task in RxISR and TickFct_ThreeLedsTriggered

void RxISR(void) {
   tasks[tasksTLTIndex].active = 1; // TLT triggered by UART rx; make active
   tasks[tasksTLTIndex].elapsedTime = tasks[tasksTLTIndex].period; // Make ready to tick
   RxData = R; // Read to global var to avoid problems if chars come too fast
   RxFlag = 1;
}

// Task tick functions 

int TickFct_BlinkLed(int state) {
   puts("BL Tick\r\n"); 
   switch(state) { // Transitions
      case BL_SMStart:
         state = BL_LEDOff;
         break;
      case BL_LEDOff:
         state = BL_LEDOn;
         break;
      case BL_LEDOn:
         state = BL_LEDOff;
         break;
      default:
         state = BL_SMStart;
         break;
   }

   switch(state) { // State actions
      case BL_LEDOff:
         B0 = 0;
         break;
      case BL_LEDOn:
         B0 = 1;
         break;
      default:
         break;
   }
   return state;
}

int TickFct_ThreeLedsTriggered(int state) {
  puts("TLT Tick\r\n"); 
   switch(state) { // Transitions
      case TLT_SMStart:
         state = TLT_Init;
         break;
         case TLT_T0:
         if (1) {
            state = TLT_T1;
         }
         break;
      case TLT_T1:
         if (1) {
            state = TLT_T2;
         }
         break;
      case TLT_T2:
         if (1) {
            state = TLT_Inactive;
         }
         break;
      case TLT_Init:
         if (1) {
            state = TLT_Inactive;
         }
         break;
      case TLT_Inactive: // Note: special "Inactive" state's
                         // transitions are same as any other state
         if (RxData == 'g') {
            state = TLT_T0;
         }
         else {
            state = TLT_Inactive;
         }
         break;
      default:
         state = TLT_SMStart;
        break;
   } // Transitions

   switch(state) { // State actions
      case TLT_T0:
         B5 = 1; B6 = 0; B7 = 0;
         break;
      case TLT_T1:
         B5 = 0; B6 = 1; B7 = 0;
         break;
      case TLT_T2:
         B5 = 0; B6 = 0; B7 = 1;
         break;
      case TLT_Init:
         B5 = 0; B6 = 0; B7 = 0; 
         break;
      case TLT_Inactive:
         B5 = 0; B6 = 0; B7 = 0;
         tasks[tasksTLTIndex].active = 0; // Special "Inactive" state 
                                          // makes task inactive
         break;
      default: 
         break;
   } // State actions
   return state; 
}

void main()
{
   unsigned char i=0;
   tasks[i].state       = BL_SMStart;
   tasks[i].period      = BL_period;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].active      = 1; // Task starts as active
   tasks[i].TickFct     = &TickFct_BlinkLed;
   i++;   
   tasks[i].state       = TLT_SMStart;
   tasks[i].period      = TLT_period;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].active      = 1; // Task starts as active
   tasks[i].TickFct     = &TickFct_ThreeLedsTriggered;

   TimerSet(tasksPeriodGCD);
   TimerOn();
   UARTOn(); 
   
   while(1) { Sleep(); }
}
```

## `Sleep()` in RIM and power saving
To save energy consumption, a microcontroller may be asleep 99% of the time or more, waking up for brief periods, performing a burst of processing, and going back to sleep again.

[**Supercapacitors**](https://en.wikipedia.org/wiki/Supercapacitor): charge more quickly, can be recharged many more times than batteries but presently can't store as much energy (perhaps 1/10th). 
* However, modest-sized supercaps may store several thousand Joules, and thus could power a low-power microcontroller for days or years (depending on sleep time).

## [RIOS](http://riosscheduler.org/)
```c
// Non-preemptive, simple tasks
#include "RIMS.h"

/*
   Copyright (c) 2013 Frank Vahid, Tony Givargis, and
   Bailey Miller. Univ. of California, Riverside and Irvine.
   RIOS version 1.2
*/
typedef struct task {
   unsigned long period;      // Rate at which the task should tick
   unsigned long elapsedTime; // Time since task's last tick
   void (*TickFct)(void);     // Function to call for task's tick
} task;

task tasks[2];
const unsigned char tasksNum = 2;
const unsigned long tasksPeriodGCD = 200; // Timer tick rate
const unsigned long periodToggle   = 1000;
const unsigned long periodSequence = 200;

void TickFct_Toggle(void);
void TickFct_Sequence(void);

unsigned char TimerFlag = 0;
void TimerISR(void) {
   if (TimerFlag) {
      printf("Timer ticked before task processing done.\n");
   }
   else {
      TimerFlag = 1;
   }
   return;
}

int main(void) {
   // Priority assigned to lower position tasks in array
   unsigned char i = 0;
   tasks[i].period      = periodSequence;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].TickFct     = &TickFct_Sequence;
   ++i;
   tasks[i].period      = periodToggle;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].TickFct     = &TickFct_Toggle;

   TimerSet(tasksPeriodGCD);
   TimerOn();

   while(1) {
      // Heart of the scheduler code
      for (i=0; i < tasksNum; ++i) {
         if (tasks[i].elapsedTime >= tasks[i].period) { // Ready
            tasks[i].TickFct(); //execute task tick
            tasks[i].elapsedTime = 0;
         tasks[i].elapsedTime += tasksPeriodGCD;
      }
      TimerFlag = 0;
      while (!TimerFlag) {
         Sleep();
      }
   }
}

// Task: Toggle an output
void TickFct_Toggle(void)   {
   static unsigned char init = 1;
   if (init) { // Initialization behavior
      B0 = 0;
      init = 0;
   }
   else { // Normal behavior
      B0 = !B0;
   }
}

 // Task: Sequence a 1 across 3 outputs
void TickFct_Sequence(void) {
   static unsigned char init = 1;
   unsigned char tmp = 0;
   if (init) { // Initialization behavior
      init = 0;
      B2   = 1;
      B3   = 0;
      B4   = 0;
   }
   else { // Normal behavior
      tmp = B4;
      B4  = B3;
      B3  = B2;
      B2  = tmp;
   }
}
```

```c
// Non-preemptive, state machine tasks
#include "RIMS.h"
/*
   Copyright (c) 2013 Frank Vahid, Tony Givargis, and
   Bailey Miller. Univ. of California, Riverside and Irvine.
   RIOS version 1.2
*/
typedef struct task {
   unsigned long period;      // Rate at which the task should tick
   unsigned long elapsedTime; // Time since task's last tick
   int state;                 //Current state
   int (*TickFct)(int);       // Function to call for task's tick
} task;

enum TG_States { TG_None, TG_s1 };
int TickFct_Toggle(int);

enum SQ_States { SQ_None, SQ_s1, SQ_s2, SQ_s3 };
int TickFct_Sequence(int);

unsigned char TimerFlag = 0;
void TimerISR(void) {
   if (TimerFlag) {
      printf("Timer ticked before task processing done.\n");
}
   else {
      TimerFlag = 1;
   }
   return;
}

int main(void) {
   task tasks[2];
   const unsigned char tasksNum = 2;
   const unsigned long tasksPeriodGCD = 200; // Timer tick rate
   const unsigned long periodToggle   = 1000;
   const unsigned long periodSequence = 200;

   unsigned char i = 0;
   tasks[i].period      = periodToggle;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].TickFct     = &TickFct_Toggle;
   tasks[i].state       = TG_None;
   i++ ;
   tasks[i].period      = periodSequence;
   tasks[i].elapsedTime = tasks[i].period;
   tasks[i].TickFct     = &TickFct_Sequence;
   tasks[i].state       = SQ_None;

   TimerSet(tasksPeriodGCD);
   TimerOn();

   while(1) {
      // Heart of the scheduler code
      for (i = 0; i < tasksNum; ++i) {
         if (tasks[i].elapsedTime >= tasks[i].period) { // Ready
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].elapsedTime = 0;
         }
         tasks[i].elapsedTime += tasksPeriodGCD;
      }
      TimerFlag = 0;
      while (!TimerFlag) {
         Sleep();
      }
   }
   return 0;
}

int TickFct_Toggle(int state) {
   switch(state) { // Transitions
      case TG_None: // Initial transition
         B0 = 0; // Initialization behavior
         state = TG_s1;
         break;
      case TG_s1:
         state = TG_s1;
         break;
      default:
         state = TG_None;
    }
    switch(state) { // State actions
      case TG_s1:
         B0 = !B0;
         break;
      default:
         break;
   }
   return state;
}
int TickFct_Sequence(int state) {
   switch(state) { // Transitions
      case SQ_None: // Initial transition
         state = SQ_s1;
         break;
      case SQ_s1:
         state = SQ_s2;
         break;
      case SQ_s2:
         state = SQ_s3;
         break;
      case SQ_s3:
         state = SQ_s1;
         break;
      default:
         state = SQ_None;
    }
    switch(state) { // State actions
      case SQ_s1:
         B2 = 1;
         B3 = 0;
         B4 = 0;
         break;
      case SQ_s2:
         B2 = 0;
         B3 = 1;
         B4 = 0;
         break;
      case SQ_s3:
         B2 = 0;
         B3 = 0;
         B4 = 1;
         break;
      default:
         break;
   }
   return state;
}
```

# Chap 11: Control Systems
##11.1 *on-off control*: 
* overshoot
 * or slower *rise time*
* oscillation
 * considered *out of control*

## 11.2 *Proportional control*
`Actuator = Kp * Error`

## 11.3 *Proportional-derivative (PD) control*
`Actuator = Kp*Error - Kd*Deriv`
* `Kd` is typically larger than `Kp`. The derivative term is *subtracted* from the proportional term rather than added, because if the error is positive and the slope is very large (as in situation B above), the actuator value should be reduced.

## 11.4 *Proportional-integral-derivative (PID) control*

An undesirable feature in a control system is steady-state error, wherein the actual output value never actually reaches the desired output value:
![steady-state-error](https://cloud.githubusercontent.com/assets/14265605/10196728/ef8a91a6-6759-11e5-8f46-8db17d529926.png)

In a water heater, steady-state error could occur if the proportional constant Kp was determined with the surrounding air at a normal room temperature, but then the water heater is installed into a very cold garage, such that the constant Kp does not create a flame strong enough to overcome the loss of heat from the water tank to the surrounding air of the garage. In a cruise-controller, Kp may have been determined with no wind, but then the car is driving into a strong head wind. Steady state error can also occur with the actual value being greater than the desired value, which could occur on a hot day for the water heater or driving with a tail wind for the cruise controller.

The sum of the points of a plot can be computed as the area under the curve, which is the plot's integral, so this term is called an integral term.

To prevent this term from growing too much, the term is typically constrained to be within a max and min value, as follows:

```c
Integ = Integ + Error;
if (Integ > IntegMax) { 
   Integ = IntegMax; 
}
else if (Integ < IntegMin) { 
   Integ = IntegMin; 
}
```

`Actuator = Kp*Error + Ki*Integ - Kd*Deriv`
* `Ki` is typically much smaller than `Kp`

First, a controller's Actuator calculation could result in a value outside the allowed Actuator value range. For the earlier water heater example, only values between 0 and 200 are allowed. A controller may therefore introduce a temporary variable, compute the actuator value as above, and then set that value to 200 if the computed value is greater than 200, or set that value to 0 if the computed value is less than 0, before finally setting Actuator to that temporary value.

Second, a sampling rate impacts the calculation of the derivative. If a very fast sampling rate is being used, the derivative might be computed as the average of the past several computed slopes, to avoid very rapid changes.

Third, PID control is commonly done using floating point numbers rather than integers. However, on a microcontroller lacking floating-point hardware, a designer sometimes can achieve acceptable control using integers.

## 11.5 PID tuning

The Ziegler-Nichols method (crude, not for safety-critical systems):
1. Set `Ki = 0` and `Kd = 0` initially, and set `Kp` to some small value initially (e.g., 1).

2. Change the desired output value and observe the actual output value. Increase `Kp` until the actual output value oscillates with a constant amplitude (the oscillation amplitude is not increasing over time, nor is it decreasing over time -- it's a steady oscillation).

3. Record the value of `Kp`, calling it `Ku`. Also record the oscillation period (in seconds), calling it `Pu`.

4. Set `Kp = Ku / 1.7`, `Ki = (Kp * 2) / Pu`, `Kd = (Kp * Pu) / 8`.


# Chap 12: Digital Signal Processing

## Sensors

## Actuator
converts its analog or digital input signal to some form of energy.

## Clipping

## Bias correction
Many signals (including audio, video, and electromagnetic waves) are centered around 0. 
* a bias will lead to a waste in the ADC conversion precision.

## Sample rate and quantization
![quantization](https://cloud.githubusercontent.com/assets/14265605/10294451/f4b8d8ca-6b7f-11e5-87e0-2bda3c391ab0.png)
Loss of value along the time axis: sampling rate.
Loss of value along the space axis: quantization/bit-width of the ADC, called resolution.

### *dynamic range*
Combine *range* and *gap* in a single formula, such as:
```
range = V_max - V_min
gap = range / 2^N
dynamic range = range / gap 
              = (V_max - V_min) / ((V_max - V_min) / 2^N) 
              = (V_max - V_min) / (V_max - V_min) * 2^N 
              = 2^N
```
Interestingly, all N-bit ADCs have the same dynamic range regardless of their input voltage specification. Engineers often express dynamic range using the decibel logarithmic scale as shown below:
```
dynamic range (dB) = 10 * log( range^2 / gap^2 ) 
                   = 10 * log( range/gap )^2 
                   = 20 * log( range/gap ) 
                   = 20 * log( 2 )^N = N * 20 * log( 2 ) 
                   = 6.02 * N
```
* note that range and gap are squared.

Dynamic range sums up, as a single number, the range and precision of a system. 

E.g.  the human ear has a dynamic range of 130 dB. If we were to design an audio system that fully engage our human subject, we would consider using an ADC that could deliver about 130 dB. 

### sampling rate
The ***Sampling Theorem***(*Nyquist* rate): an analog signal can be reconstructed from its samples if the sampling rate is at least ***twice*** the ***highest*** frequency present in the signal. 

## Aliasing
*aliasing*: is the presence of a false or unexpected signal in our signal path.
* think about the spinning wheel example. When it is faster enough, you will perceive that the wheel is spinning in the opposite direction.

Suppose we have noise in the Ghz range. Due to *aliasing*, it can still inject into the interpretation of Khz range thus we need to eliminate the noise even if they are out of our perception range.
* Our recording, for instance, would produce an ugly sound that is directly from the microwave own, even though the microwave own is not generating any sounds.

One costy solution is to over-sample but there is a better solution: *anti-aliasing filter*, a.k.a. *low pass filter*.

## Low pass filter

## Playback path
*receiving path*: the signal traveling from the source (sensor) to the point where it is digitized:
![complete dsp](https://cloud.githubusercontent.com/assets/14265605/10295198/9893526e-6b84-11e5-8a66-1c9f8570c851.png)

*playback path*: the path from the digitized stream to an actuator:
![playback-path](https://cloud.githubusercontent.com/assets/14265605/10295217/be8f31f4-6b84-11e5-961a-83b1642d74ad.png)
* The speaker is the actuator. The amplifier must have a gain that is sufficiently high to drive, or operate, the speaker. To determine the gain, one must study the electrical requirements of the speaker. The bias correction is placed to eliminate any unwanted bias that has been introduced along the return path. The low pass filter is placed ahead of the amplifier so that unecessary noise, that has entered the system, is eliminated prior to amplification.

## Digital to analog converter
Key characteristics:
* Input range, or quantization, in bits
* Output range
* Sampling Rate

a DAC is simply an ADC in reverse.  the DAC parameters must be matched to those of the ADC parameters.

E.g.:
![completedsp-with-mc](https://cloud.githubusercontent.com/assets/14265605/10295339/b60d0d2a-6b85-11e5-9c79-824ff440d68c.png)

Using a microprocessor allows us to implement concepts using software.
* e.g. Guitar effects paddles.
* e.g. Echo transform:
![echo-transform](https://cloud.githubusercontent.com/assets/14265605/10295448/4c0f2f24-6b86-11e5-9e90-70fbed141485.png)

* e.g. Digital low pass filter:
![digital-low-pass-filter](https://cloud.githubusercontent.com/assets/14265605/10295527/d25371a8-6b86-11e5-973c-154d9c993f7e.png)


## Digital processing
FFT

# Chap 13: FPGAs
an FPGA is programmed with circuits that execute concurrently.
* FPGAs contain an array of programmable logic blocks, and a hierarchy of reconfigurable interconnects that allow the blocks to be "wired together".

The VHDL description can be provided to a synthesis tool, which will convert the BlinkLed process into a custom processor circuit. 

The state variable becomes a register. etc.

# Chap 14: Pattern Recognition
An ***embedded system pattern recognition framework*** is a structured approach to monitoring sensors to detect particular patterns and for controlling actuators in response.
![pattern](https://cloud.githubusercontent.com/assets/14265605/10300005/ca518744-6bb6-11e5-961f-d1e4fdcf2f30.png)

Upon noticing that a particular system represents a pattern recognition problem, an embedded system designer may create a system with the three distinct parts of the embedded system pattern recognition framework, to create a well-organized maintainable system.

The period of each part can be different.

e.g. 
![room-occupancy-pattern-recognition](https://cloud.githubusercontent.com/assets/14265605/10300068/9caf879a-6bb7-11e5-9ab7-4f2633b0a487.png)

## Feature extraction

## Pattern recognition
A *decision tree* is the logical combination of multiple thresholds. 
