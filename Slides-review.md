# Lec 1 Introduction
The next frontier is the research for the convergence between the cyber world and the physical world: ***Cyber Physical Systems***
* Mainframes => Desktops and Internet => ubiquitous/physical computing => networked systems of embedded computing systems

> [computing and communications technologies will be embedded into everyday objects of all kinds to allow objects to sense and react to their changing environments.](http://www.nap.edu/read/10193/chapter/1#xviii)

# Lec 2 Lab board UI

## Using the LCD

LCD is connected to UART1
```c
#include "lcd.h"
lcd_printf() // macro: print to buffer, flush to LCD
void lcd_locate(uint8_t row, uint8_t col) // move cursor to (row[0-8], col[0-20])
void lcd_clear()
void lcd_clear_row(uint8_t row)
fflush()
```

helloworld.c
```c
void main()
{
    lcd_initialize();
    lcd_clear();
    lcd_locate(0,0);
    
    lcd_printf("Hello World!\r");

    while(1); // idle task: once initialization is done, there is nothing to do for the main() program. while(1); prevents this thread from exiting.
}
```

Be careful when choosing the data type: 
* performing shifts on signed integer is not advised because (according to ANSI C) the result is machine dependent (arithmetic shift or logic shift).

> write down a conversion table from binary to hex.

## bit manipulation
```c
x |=0x01; // set LSB 1
x ^=0x01; // toggle LSB
```

The `BV(i)` Macro: returns a mask consisting of all 0s, except for bit i which is 1.

```c
x |= BV(4); // set bit 4 in x to 1
x &= ~BV(4); // set bit 4 in x to 0
x ^= BV(4); // Toggle bit 4 in x

// can be extend to:
x |= BV(2)|BV(4); // set bits 2,4 in x to 1
x &= ~(BV(2)|BV(4)|BV(5)); // set bits 2,4,5 in x to 0
x ^= BV(2)|BV(4)|BV(5); // toggle bits 2,4,5 in x

// and more powerful:
x |= (y&BV(3))>>3<<6; // replace bit 6 of x with bit 3 of y 
```
## Registers
the dsPIC33f has 16-bit registers.
* they are memory-mapped into the data address space.

`p33Fxxxx` library allows registers be read and written by name as if they are variables.
e.g. `T1CON = 0x0001; // sets the timer 1 configuration register /// datasheet says bit 0 is not implemented??`

## Digital I/O

85 general purpose digital I/O pins divided into seven 16-bit ports (A - G).
* `PORTAbits.x` `PORTCbits.x` `PORTGbits.x`

### Input or output direction of each pin may be selected individually using:
`TRISx` registers: Data Direction register
* output(`0`)
* input(`1`)

### Output pin
Bit in `PORTx` sets the output voltage low (`0`) or high (`1`).
`PORTAbits.RA10 = 1; // sets pin RA10 high`

### Input pin
Bit in `PORTx` tells if the current input voltage is low (`0`) or high (`1`).
`uint8_t test = PORTAbits.RA10; // sets test to input value of RA10`

Bit in `OCDx` disables (`0`) or enables (`1`) and external pull-up resistor.
`OCDAbits.RA10 = 1; // enables external pull-up resistor on RA10`

## LEDs

5 LEDs are connected to digital I/O pins in PORTA bits 0, 4, 5, 9, and 10.
e.g. `PORTAbits.RA0`: LED 0; `PORTAbits.RA10`: LED 4.


**Note**: One instruction cycle is required between a port direction change/port write and a read of the same I/O port.
* insert a `nop()` between consecutive manipulations of port:
```c
CLEARBIT(TRISABITS.TRISA10); // sets LED4 to output
Nop(); 
```

Macros already have `Nop()`, use Macros:
```c
SETLED(PORTAbits.RA10); // turn on LED4
CLEARLED(PORTAbits.RA10); // turn off LED4
```


## Joystick Buttons
Two buttons:
* B1 is trigger
* B2 is thumb

Readings:
* `1`: not pressed
* `0`: pressed
 * because pull-up resistors are used. Pressing buttons shorts the pin to GND.

![pull-up](https://cloud.githubusercontent.com/assets/14265605/10241910/842d3db4-68b2-11e5-8ca4-7b38123c6e80.png)

B1 is connected to `PORTE`. B2 to `PORTD`
* `if (PORTEbits.RE8 == 0) // if B1 is pressed?`
* `uint8_t x = (~PORTDbits.RD10); // set x to 1 if B2 is pressed, 0 otherwise`

## [debouncing](http://www.ganssle.com/debouncing.pdf)
The bouncing only lasts for several milliseconds, but this is long enough to be noticeable on the microcontroller.

There is no hardware low pass filter on the Flexui board.
* a software debouncer is needed.
* an [implementation](https://github.com/bo-rc/cs431/tree/master/Debouncer)

# Lec3 Interrupts

Reading: lab manual pp. 9-10. Additional material is on the dsPIC datasheet (section 6) and the dsPIC C compiler manual (pp. 105-123).

Interrupts are handled by hardware directly first.
* below OS level

## How can we correctly resume the process after an interrupt
**Hardware**: Save `PC` and status flags
**User Application**: Save the registers and pop them back at the end
*separation of responsibility*: Flexibility
* e.g. during debugging, users want to know the values in registers

When the interrupt handler starts, further interrupt is *Disabled* to allow for executing instructions atomically.

## When some interrupt is more urgent, what would you do?
Step 1: If you need to execute some instructions atomically, keep the interrupt disabled until it is done.
Step 2: Next enable interrupts with higher priority.

## When interrupt is caused by external device readings, how can the ISR share the data with applications who needs the data?
1. the application should block on a `mutex`.

2. the ISR should:
 * move the data to the buffer
 * unlock the `mutex` to wake up the application waiting on the data

## ISR and `mutex` and application and locked forever: i do not understand this slide.

## When ISR passes data to application by the sharing of global variables,
Interrupt enable/disable must be used to avoid *race conditions*: An error condition peculiar to parallel programs in which the outcome of a program changes as the relative scheduling of different control flows varies.
* In engineering, we would like the outcome be predictable and repeatable.

## Using `mutex`
```c
// process A
while(1) {
    pthread_mutex_lock(&mutex);
    list->insert(12);
    pthread_mutex_unlock(&mutex);
    sleep(1);
}

// process B
while(1) {
    pthread_mutex_lock(&mutex);
    list->print();
    pthread_mutex_unlock(&mutex);
    sleep(1);
}
```

## Race between ISR and User Process

Don't lock in user process with ISR: deadlock
* hardware is designed to finish ISR before any application task can start.

All processors enforce the following design rule:
Neither application tasks nor the OS can run until all the ISRs are done.
* rationale: atomic execution of multiple instructions in ISR

**Solutions**:
Race conditions between User processes: 
* disable preemption (only the process itself can voluntarily relinquish the CPU)
* use semaphores
Race conditions between a process and an ISR, or between different ISRs
* disable interrupts

## Interrupts
***Synchronous Interrupt*** (***exception*** in 80x86 manuals): produced by the CPU control unit while executing instructions.
* synchronous because the control unit issues them only after terminating the execution of an instruction.
* caused by programming errors or anomalous conditions that must be handled by the kernel.
 * divide by zero, invalid op code, stack segment overflow, protection and page faults
* can be generated by calling the `int` instruction

***Asynchronous Interrupt*** (***interrupt*** in 80x86 manuals): generated by other hardware devices at arbitrary times w.r.t. the CPU clock signals.
* issued by interval timers and I/O devices.
 * keystroke etc.

## Interrupt Handling Sequence

Interrupt service routine is responsible to handle the event that caused the interrupt.

Content of registers need to be saved before it can be altered. When writing an ISR in C, the pushes and pops are generated as needed by the compiler.

![interrupt-model-x86](https://cloud.githubusercontent.com/assets/14265605/10384950/5a75657c-6e08-11e5-9a9d-2d671a8e0592.png)
* IF: Interrupt-enable flag
* IRET: Interrupt return

![interrupt-model-dspic33f](https://cloud.githubusercontent.com/assets/14265605/10384999/ccc21396-6e08-11e5-9a94-b30b9c872caf.png)
* [RETFIE](http://tutor.al-williams.com/) is exactly like a return, but it also sets the global interrupt enable (GIE). When a hardware interrupt occurs, it clears GIE and executes what amounts to a CALL instruction. Using RETFIE allows you to enable interrupts and return to the main program all in one step. If you don't want interrupts enabled again, just execute a RETURN instead.

### **Interrupt Vector Table**: the addresses of ISRs

Each ISR is identified by the starting address of the vector:
![interrupt-vector-table-dspic33f](https://cloud.githubusercontent.com/assets/14265605/10385121/5b041e78-6e0a-11e5-80cc-ab8e6cf36de5.png)
* *Natural order priority*: used only to resolve conflicts between simultaneous pending interrupts with same priority level.
 * once the priority conflict is resolved and the exception process begins, the CPU can be interrupted only by a source with higher priority.

## Interrupt handling and Interrupt flag in dsPIC33F
An interrupt is triggered by an event that sets the Interrupt Flag (IFSx: Interrupt Flag Status Register).

The corresponding interrupt flag must be cleared by software (inside the ISR).

If an interrupt condition occurs while the corresponding interrupt enable bit (IECx: Interrupt Enable Control Registers) is cleared, the Interrupt Flag will be set and remembered until the interrupt is re-enabled, or the flag is cleared by software.

Each user interrupt can have a priority from 0 to 7 (with 1 being the lowest priority and 0 having the effect of disabling the interrupt source). The interrupt priority is set in the IPCx registers (Interrupt Priority Control Registers).

### Install and ISR
use macro:
```c
void __attribute__ ((__interrupt__)) PRIMARY_NAME(void);
```
* PRIMARY_NAMEs:
![dspic33f-interrupts](https://cloud.githubusercontent.com/assets/14265605/10385336/eacd25b6-6e0c-11e5-8497-145ebee9ad06.png)

e.g.
```c
void __attribute__((__interrupt__)) _T1Interrupt(void)
{
    global_counter++; // increment a global counter, use volatile
    IFS0bits.T1IF = 0; // clear the interrupt flag
}
```

## Nested Interrupts
Interrupts are nestable by default.
* can be interrupted by a higher-level Interrupt.

Disable nested interrupts:
set `INTCON1<15>` (Interrupt Nesting Disable control bit).
* when set, all interrupts in progress raise priority to level 7 by setting `IPL<2:0> = 111`.

When interrupt nesting is disabled, the user-assigned interrupt priority levels have no effect except to resolve conflicts between simultaneous pending interrupts.

The `IPL<2:0>` bits become read-only when interrupt nesting is disabled.

## Disable Interrupt Instruction (DISI)
The DISI instruction can disable interrupts with priorities from 1 to 6 for up to 16384 instruction cycles. This instruction is useful for executing time-critical code segments. The DISI instruction only disables interrupts with priority levels 1-6. Priority level 7 interrupts and all trap events can still interrupt the CPU when the DISI instruction is active.

## TODO: look at lab, understand each flag.

# Lec4 Timer, A/D

Read: lab manual (pp. 14-16, 29-32, and pp38-40). Additional material is on dsPic data sheet (section 16 (ADC)), MCP4822 data sheet, and manual of DAS1602 PCI I/O board

## dsPIC Timer/Counters
The dsPic microcontroller has nine 16-bits (Timer1-Timer9) Timer/Counters.

**Timer1** is Type A
* has two clocks:
 * system clock: 12.8MHz
 * an *asynchronous* 32.767 kHz watch crystal clock.

**Timer2**, **4**, **6**, **8** are Type B
* system clock: 12.8MHz
* can be coupled with **Timer3**, **5**, **7**, **9** (type C timers) to make 32 bit timers.

A *prescaler* may be used to slow down the timer frequency.

### Timer registers
Each timer has the following r/w registers:
* `TMRx`: 16-bit Timer Count register
 * e.g. `TMR2 = 0x00; // clear timer2 register`
* `PRx`: 16-bit Timer Period register
 * e.g. `PR2 = 50; // set timer2 period to 50`
* TxCON: 16-bit Timer Control register
 * e.g. `SETBIT(T2CONbits.TON); // Start Timer2`
 * e.g. `T2CONbits.TCKPS = 0b11; // Select 1:256 Prescaler for timer2`
 * e.g. `T1CONbits.TCS = 1; //Select external clock`
 * e.g. `T1CONbits.TSYNC = 0; //Disable Synchronization`

Interrupt control for each timer module:
* `TxIE`: Interrupt Enable Control bit (register `IEC(x-1)`)
 * e.g. `IEC0bits.T1IE = 1; // Enable Timer1 interrupt`
* `TxIF`: Interrupt Flag Status bit (register `IFS(x-1)`)
 * e.g. `IFS0bits.T2IF = 0; // clear timer2 interrupt flag`
* `TxIP`: Interrupt Priority Control bits (register `IPC(x-1)`)
 * e.g. `IPC0bits.T2IP = 0x03; // Set Timer2 Interrupt Priority Level`

When the timer is enabled, `TMRx` increments by one on every rising edge of the input clock and the timer can generate an interrupt when `TMRx` matches the period set on `PRx`. 
* called "timerx expires".
* After a period match, `TMRx` is automatically reset to `0`.

### How to operate a timer
* disable and initialize Timer using `TxCON` register
* clear the Timer register `TMRx`
* set the period value in `PRx`
* use `IPC(x-1)`, `IFS(x-1)`, and `IEC(x-1)` registers to configure interrupt
* enable Timer using `TxCON` register

e.g. 
```c
void initialize_Timer1(){

    //enable LPOSCEN
    __builtin_write_OSCCONL(OSCCONL | 2);
    T1CONbits.TON = 0; //Disable Timer
    T1CONbits.TCS = 1; //Select external clock
    T1CONbits.TSYNC = 0; //Disable Synchronization
    T1CONbits.TCKPS = 0b00; //Select 1:1 Prescaler
    TMR1 = 0x00; //Clear timer register
    PR1 = 32767; //Load the period value
    IPC0bits.T1IP = 0x02; // Set Timer1 Interrupt Priority Level
    IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1;// Enable Timer1 interrupt
    T1CONbits.TON = 1;// Start Timer
}
```
* Timer expires after `Period * prescaler / clock frequency`.

*What happens if you forget to reset the interrupt flag to `0`*
* The first time the interrupt will be triggered at the expected time.
* after first time, every `Max_Period * prescaler / clock frequency`.
 * `TCNT0` continues to increase past `OCR0` until it overflows.

## A/D dsPIC board
dsPIC board:
* 2 Analog to Digital Converter (ADC)
 * 10-bit or 12-bit resolution
 * 0 to +3.3 volt range is the current configuration
 * `ADC1` has access to analog input pins `AN0-31`
 * `ADC2` has access to analog input pins `0-15`

4-ch are connected as:

| PIN  | PORT |ADC channel|Connection|
|:----:|:----:|:---------:|:--------:|
|`RB4` | B    | ch4       | Joystick x-axis|
|`RB5` | B    | ch5       | Joystick y-axis|
|`RB15`| B    | ch15      | Balance board x-axis|
|`RB9` | B    | ch9       | Balance board y-axis|

### Operations overview
* Initialization
* Select analog input pin (`AN0-31`)
* Start conversion
* Wait for conversion to finish
* Collect conversion result

To sample an ADC channel:
```c
AD2CHS0bits.CH0SA = 0x004; //set ADC to Sample AN4 pin
SETBIT(AD2CON1bits.SAMP); //start to sample
while(!AD2CON1bits.DONE); //wait for conversion to finish
CLEARBIT(AD2CON1bits.DONE); //MUST HAVE! clear conversion done bit

maxX = ADC2BUF0; // retrieve value
```

# The general conversion rule
![addc](https://cloud.githubusercontent.com/assets/14265605/10387207/18b48f9c-6e25-11e5-881c-145f293e389a.png)

## Quantization

*quantization error*: suppose input range of -2.0 to +2.0 v and 8-bit ADC, 4.0 / 256 = 0.015625.

*Quiz*: What are these ranges for? What should be the rule in picking a range?
*Answer*: These ranges represent the max and min permitted voltages for the ADC. Pick the smallest range that still includes the max and min voltage values that can be read from the source.

## D/A 

* Digital to Analog Converter (DAC)
 * 12-bit resolution
 * 0 to +4.096 volt range
 * 16 bits
  * Bit 15 is the Channel select (A=0/B=1)
  * Bit 14 is not used (set to 0 or 1)
  * Bit 13 is gain (set to 0 for max Vout=4.096)
  * Bit 12 is Output Shutdown Control (set to 1 for “active mode operation”; Vout is available)
  * Bits 11-0 is the 12 bit data

# Lec5 Periodic Tasks

Read: Book: Programming for The Real World, Bill O. Gallmeister, O’Reilly&Associates, Inc.
See pp.55-69. User guide of PCI-DAS1602-12

***Jitter***: the deviation of specified input/output time.
* Jitters introduce measurement errors into sampled signals.
* *occurs* when the I/O actions is preempted
 * The lack of atomicity in running reads (inputs) and writes (outputs) can lead jitters in I/O.

***drift***: the starting time of a period task is delayed.
* *occurs* when the action to set next period’s starting time is preempted.
 * e.g. you want to `sleep(10)`, but right before this call, you are preempted by a higher-priority task for 50 sec, then equivalently you freeze for 50 sec then sleep for 10 sec. Next period starts after 50 + 10 = 60 sec.

![jitter-drift](https://cloud.githubusercontent.com/assets/14265605/10387447/0b5a0ba4-6e27-11e5-819c-0d450767c8a3.png)

Any task executes in the user space can be preempted by higher priority tasks, OS, signal handler and interrupt service routines.
* The only place where we can execute a sequence of instructions without being interrupted or preempted is inside an interrupt (or signal) handler

### the root cause and fix of drift and jitter?
The key to create jitters:
* preempt the I/O operation

The key to create drift:
* preempt the start of next period.

#### Knowing the cause, what is the key idea that can fix it? 
* Not allow preemption or interrupts to I/O or the start times of periods

What is the mechanism to implement the key idea? 
* use Interrupt-based timers
* To solve the drift problem
 * use a periodic hardware based timer to kick off each instance of a periodic task, it will be ready at the correct time instants.
* To minimize the jitter problem when I/O is short
 * do the I/O in the timer interrupt handler. As long as each task finishes before its end of period, I/O can be done at nearly the regular instants of timer interrupts: it gives the highest regularity for software based solution.

Interrupt does give atomicity in the execution of instructions. However, the fact that the ISR executes below the level of OS is also a liability.
* lack of protection
* difficult to extend

The solution: ***virtualization***
* *signal* is virtualized interrupt service.
* Design:
 * Protection: the scope of the failure should be limited to one’s own application
 * Atomicity: without being interrupted by applications and by other signal handlers (unless you enables nested interrupts). But signal handlers can be interrupted by OS after the expiration of the timer set by the OS (staying too long in handler).
 * Extension: unlimited software timers

## POSIX.1 Signals

Signals are similar to hardware interrupts. However, they are managed and delivered by the OS.
* used for 
 * Exception handling (bad pointer accesses, divide by zero, etc.)
 * Process notification of asynchronous event occurrence (I/O completion, timer expiration, etc.)
 * Process termination in abnormal circumstances
 * Interprocess communication

*signal mask*: The collection of signals that are currently blocked.
* Each process has its own signal mask.
* child process inherits parent's mask.

### setting the process mask
```c
#include <signal.h>

       int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
```
`how`: `SIG_BLOCK`, `SIG_UNBLOCK`, `SIG_SETMASK`
`oldset`: If `oldset` is non-`NULL`, the previous value of the signal mask is
       stored in `oldset`.

There are two spare signals available to user applications: `SIGUSR1` and `SIGUSR2`. Any application can use them as it wants.

### Using signals
![signal-handlers](https://cloud.githubusercontent.com/assets/14265605/10388517/653c7eaa-6e30-11e5-9a66-a45e707b9f94.png)
* Signal handlers execute in user space
 * A signal handler can interact with the regular execution flow of the corresponding process by simply sharing global variables

Event of *sending a signal*:

1. The OS updates the descriptor of the destination process to represent that a new signal has been sent.

2. Signals that have been sent but not yet received are called pending signals. At any time, only one pending signal of a given type may exist for a process; additional pending signals of the same type to the same process are not queued but simply discarded.

Event of *receiving a signal*:

1.If the sent signal is blocked by the process mask, the process will not receive the signal until it removes the block: the signal remains pending.

2.When switching from Kernel Mode to User Mode, check whether a signal has been sent to the current executing process (this happens at every timer interrupt). Unblocked signals are received.

3.Determine whether ignore the signal, execute a default action or execute user’s signal handler.

use encpasulation:
```c
struct sigaction {
    void(*sa_handler)();
    sigset_t sa_mask;
    int sa_flags;
    // ...
};
```

An e.g:
```c
#define SIG_QUERY_COMPLETE SIGUSR2
void query_has_completed (int signo)
{
   // ...
}
main()
{
    struct sigaction sa;
    sigset_t mask, wait_mask; // these are process masks
    sa.sa_handler = query_has_completed;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIG_QUERY_COMPLETE);
    sa.sa_flags = 0;

    sigefillset(&mask); // block ALL signals
    sigaddset(&mask, SIG_QUERY_COMPLETE); // allow only SIG_QUERY_COMPLETE signal to be caught

    sigprocmask(SIG_SETMASK, &mask, NULL); // installs the process mask

    sigemptyset(&wait_mask);
    sigaction(SIG_QUERY_COMPLETE, &sa, NULL); // installs the handler
    // ...
    while (1) {
        /* deal with the screen & issue a query to the server */
        /* Await server response */
        sigsuspend(&wait_mask); // wait for ANY SIGNAL because it uses an empty mask
}
//...
```
* No other task can kill your task by sending an unexpected signal.
* This style is good for applications such as the controller of a drone. 
* The commonly used signal is `SIGALARM` to wake your task up periodically. You can also use it to handle a unexpected device failure.

To prevent race condition:
```c
// user process
// ...
while (1){
    // ...
    mask SIGUSR1; // disable signal
    print list;
    unmask SIGUSR1; // enable signal
    // ...
    sleep(1);
}

// handler
Analog_Digital_Conv_ISR()
{
    insert an elem in list;
    return;
}
```

Signal handlers do not run in multitasking: there is just one single process going off and handling various signals. Any time the signal handler blocks, the entire process blocks.

# Lec6 periodic tasks 2

Read: Book: Programming for The Real World, Bill O. Gallmeister, O’Reilly&Associates, Inc.
See pp.69-82 and 173-193. Lab manual pp.33-35.

How to synchronize with a particular signal:
```c
while(1) {
    /* synchronize with a signal */
    sigsuspend(&wait_signals_other_than_this_to_fire);
};
```

Unless `sig_atomic` is used, OS can still interrupt signal handlers after a timeout generated by a hardware timer.

*Quiz Questions*:
* What happens if you forget to block an unwanted signal?
 * Ans: a default handler, TERMINATES your applications!

* There are MANY signals that you do not want for your application. What is the best practice to set up a signal mask so that you will not accidentally miss blocking some unwanted signals?
 * Ans: Step 1: block ALL signals. Step 2: unblock only those you want to handle.

* In real time applications, we want to ensure that a low priority task can be preempted by higher priority tasks that are more urgent, but signal handlers cannot be interrupted. What should we do?
 * Ans: Get out of the way ASAP. That is, do the job that is necessary in the handler only. Typically, it is I/O. And pass the data to applications via shared variables.

## [signals](http://perugini.cps.udayton.edu/teaching/courses/cps445/lecture_notes/signals.html)
blocking != ignoring
* a blocked signal is not thrown away
 * it is delivered when unblocked
* blocked signals do not affect the behavior of the process until they are delivered

use `write` in signal handlers rather than `fprintf` or `strlen` as `write` is *async-signal-safe*
* means that it can be called safely from inside a handler
 * no such guarantees for `fprintf` or `strlen`

Signal handlers do not run in multitasking: there is just one single process going off and handling various signals. Any time the signal handler blocks, the entire process blocks.

# Lec16: RMA Intro

## real-time system requirements

1. Schedulable utilization

2. Worst-case response time for each of tasks

3. Stability (getting critical tasks done)

Quiz: Which task should have higher priority?
* Answer: the task with higher rate (according to RM) unless the system is
overloaded!

Priorities should be assigned according to ***Rate*** not to *importance (criticality)*
* otherwise there is no lower bound of processor utilization, below which tasks deadlines can be guaranteed.

## criticality vs. priority
Criticality: importance of the task

Priority: proportional to job period.
* determines which job should be scheduled first

An important find in real-time computing theory is that importance may or
may not correspond to scheduling priority.
* Importance matters only when tasks cannot be scheduled (overload condition) without missing deadlines. 

*Rate Monotonic* Scheduling [**RM**]: Tasks with smaller periods are assigned
higher priorities (static priority).

*Earliest Deadline First* [**EDF**]: Jobs are prioritized based on absolute
deadlines (dynamic priority).

For periodic tasks with relative deadlines equal to their periods:
* RM priority assignment is the optimal static priority assignment
 * No other static priority assignment can do better
 * Yet, it cannot achieve 100% CPU utilization
* Earliest deadline first scheduling is the optimal dynamic priority policy
 * EDF can achieve 100% CPU utilization

FAA forbids EDF scheduling because it aces at 1. but fails at 2. and 3.

# Lec17: Exact Schedulability Analysis

GRMS (***Generalized Rate-Monotonic Scheduling Theory***) is a practical theory that will enable you to design and
analyze multi-threaded real time applications.
* GRMS is supported by almost all the commercially available RTOS

To achieve 100% utilization when using fixed priorities, assign periods so that all
tasks are harmonic. This means that for each task, its period is an exact multiple of
every other task that has a shorter period. 

A set of n periodic tasks is schedulable if: 

![least-upper-bound-utilization](https://cloud.githubusercontent.com/assets/14265605/11313731/c4b8374e-8fa4-11e5-89a4-5784dad3cf6a.png)
* For harmonic task sets, the utilization bound is U(n)=1.00 for all n.
Otherwise, for large n, the bound converges to ln2 ~ 0.69.
* It is a sufficient condition
 * hence, it is inconclusive if it fails!
* most significant results in real-time scheduling theory

## POSIX.4 scheduling interface

`SCHED_FIFO`:
`SCHED_RR`:
`SCHED_OTHER`:

`sched_get_priority_min()`
`sched_get_priority_max()`

`shield_yield()`

`sched_rr_get_interval()`

## POSIX.4 memory locking interface
`mlock()` and `mlockall`

## The Exact Schedulability Test
The exact test for a task checks if this task can meet its first deadline.
* If a task meets its first deadline when all higher priority tasks are
started at the same time, then this task’s future deadlines will always be met. 
 * It holds only for fixed priority scheduling!

![exact-analysis](https://cloud.githubusercontent.com/assets/14265605/11314153/904c9302-8fa8-11e5-844a-96f252675320.png)

*r_i*: the response time of job i.

Both the Utilization Bound and the Exact schedulability test make the following
assumptions:
* All the tasks are periodic
* Tasks are scheduled according to RMS
* All tasks are independent and do not share resources (data)
* Tasks do not self-suspend during their execution
* Scheduler overhead (context-switch) is negligible

The algorithm:
* Suppose we have n tasks, and we pick a task, say i, to see if it is schedulable.
* We initialize the testing by assuming all the higher priority tasks from 1 to i-1 will
only preempt task i once.
 * Hence, the initially presumed finishing time for task i is just the sum of C_1 to C_i, which we call r_0.
* We now check the actual arrival of higher priority tasks within the duration r_0 and
then presume that it will be all the preemption task i will experience. So we
compute r_1 under this assumption.
* We will repeat this process until one of the two conditions occur:
 * 1. The rn eventually exceeds the deadline of task i. In this case we terminate
the iteration process and conclude that task i is not schedulable.
 * 2. The series rn converges to a fixed point (i.e., it stops increasing). If this fixed
point is less than or equal to the deadline, then the task is schedulable and we
terminate the schedulability test.

# Lec18
**Priority Inversion**: When a high priority task is delayed/blocked by lower priority tasks.
* during synchronization of using `mutex`

**Unbounded Priority Inversion**: When the duration of *priority inversion* is not bounded by a function of the duration of critical sections.

**Critical instant theorem**: If a task meets its first deadline when *all higher priority tasks* are started *at the same time*, then this task’s future deadlines will always be met. The exact test for a task checks if this task can meet its first deadline.
* holds only for fixed priority scheduling

**It is a common mistake** to assume that if a higher priority task is not
schedulable so are the lower priority tasks.

## Modeling Context Switch Time in RM
S: amount of time to deschedule the current executing task and schedule a new one.
* higher-priority task can cause at-most (worst case senario) 2 context switches in its period to a lower-priority task.
    * this happens when the higher-priority finishes before the deadline of the lower-priority task.
        * Switch out of CPU due to preemption: S<sub>1</sub> + Switch back to CPU due to finished preemption: S<sub>2</sub>
            * The process is symmetrical so the cost of context switch at worst case is: **2S**

* Just replace `C<sub>i</sub> = (C<sub>i</sub> + 2S)` in both the UB test and exact test.

## Modeling Pre-period Deadlines in RM
The pre-period deadline of a task does not affect the schedulability of other tasks!

When the task has a deadline: `D < P`
* ***UB test***: it is *as if* the task has a longer execution time:
    * but the period is NOT changed:
        * just replace `C<sub>i</sub> = (C<sub>i</sub> + (P-D))` for this task
    * the `(P-D)` time does not occupy CPU, so this effect is a *what-if* analysis only for this task: Other tasks won't be affected.
    * now you need to check all N equations of adding each task to check the schedulability, since the pre-period deadline is *local* to the specific task.
        * w/o the *local*-only effect, just 1 equation including all tasks will be enough to check the schedulability of the set.
        * you have to do this:

![ub-test-pre-period-deadline](https://cloud.githubusercontent.com/assets/14265605/11698149/edefbf5a-9e82-11e5-88d3-4ff2be330d22.png)

* ***Exact test***: 
    * just move the **deadline** from `P` to `D`
    * **period** not changed.
    * still local to this task: analysis of other tasks not changed.

# Lec 19

## Modeling Blocking time in RM
*Preemption*: short-period tasks delays long-period tasks.
*Blocking*: long-period tasks delays short-period tasks.
* this could happen, e.g.
    * the data I/O of a long-period tasks could run at the top priority, to reduce jitter, and could block.

In both ***UB test*** and ***Exact test***, just add blocking time `B` to `C`.
* this effect is also ***local***.
e.g.:

![ub-test-blocking](https://cloud.githubusercontent.com/assets/14265605/11699051/d10a7376-9e87-11e5-85d0-99c25207021b.png)

## Aperiodic tasks
**Aperiodic task**: runs at irregular intervals

*Rate monotonic* scheduling is a *periodic* framework. To handle aperiodics, we
must ***convert*** the aperiodic event service into a periodic framework.
* Except in the case of using interrupt handler to serve aperiodics

The basic idea is to *periodically allocate CPU cycles* to each stream of aperiodic requests. 
* This CPU allocation is called ***aperiodic server**, we will cover:
    * ***Polling server***
    * ***Sporadic server***

### Polling server
behaves just like a periodic task with `C = budget`, `P = period`.
The **UB test** and **Exact test** can be applied as if the server is another periodic task.

Service delay of a polling server is, on average, roughly half of the server period.

# Lec 20











