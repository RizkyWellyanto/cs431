# Chapter 1: Introduction to ARM Cortex-M Processors

The ISA in Cortex-M: **Thumb ISA**
* based on **Thumb-2** technology which supports a mixture of 16-bit and 32-bit instructions.

Harvard bus architecture with unified memory space(***Modified Harward architecture***): instructions and data use the same address space. 
* Harvard architecture: physically *separate* storage and signal pathways for *instructions* and *data*. 
 * In other words, a memory address does not uniquely identify a storage location (as it does in a Von Neumann machine); you also need to know the memory space (instruction or data) to which the address belongs.
* Modified Harvard architecture: allows the contents of the instruction memory to be accessed as if it were data.
 * loosen the strict separation between code and data, while still supporting the higher performance concurrent data and instruction access of the Harvard architecture.
* Von Neumann architecture: same address space - code can also be accessed and treated the same as data, and vice versa. 

## we need different processors for different applications. 
![cortex-m](https://cloud.githubusercontent.com/assets/14265605/10613766/3b478bea-771c-11e5-93f5-a8bb329533b2.png)

## ARM processor v.s. microcontroller
In a typical microcontroller design, the processor (ARM Cortex M) takes only a small part of the silicon area.
![microcontroller](https://cloud.githubusercontent.com/assets/14265605/10613949/3e06ae3c-771d-11e5-9461-61bb785ea3a9.png)

## PI-licencing and Vendors
After a company licenses the Cortex-M processor design, ARM provides the
design source code of the processor in a language called Verilog-HDL (Hardware
Description Language). The design engineers in these companies then add their
own design blocks like peripherals and memories, and use various EDA tools to
convert the whole design from Verilog-HDL and various other forms into a transistor
level chip layout.

## ARM processors
![arm-processors](https://cloud.githubusercontent.com/assets/14265605/10615541/18744108-7726-11e5-82f1-3aeec5eb1bb9.png)

**Cortex-A**
* Application processors that are designed to handle complex
applications such as high-end embedded operating systems (OSs) (e.g., iOS,
Android, Linux, and Windows). These applications require the highest
processing power, virtual memory system support with memory management
units (MMUs), and, optionally, enhanced Java support and a secure program
execution environment. Example products include high-end smartphones,
tablets, televisions, and even computing servers.

**Cortex-R**
* Real-time, high-performance processors targeted primarily at the
higher end of the real-time market e these are applications, such as hard drive
controllers, baseband controllers for mobile communications, and automotive
systems, in which high processing power and high reliability are essential and for
which low latency and determinism are important.

**Cortex-M**
* Processors targeting smaller scale applications such as
microcontrollers and mixed signal design, where criteria like low cost, low
power, energy efficiency, and low interrupt latency are important. At the same
time, the processor design has to be easy to use and able to provide deterministic
behavior as required in many real-time control systems.

## Architecture ISA and Thumb ISA
Architecture version numbers are independent of processor names.

![thumb](https://cloud.githubusercontent.com/assets/14265605/10615649/9b91ca24-7726-11e5-9868-fe62f88d80cb.png)

In order to address different needs of a wide range of application areas, architecture
version 7 is divided into three profiles: 
* Cortex-A Processors: ARMv7-A Architecture
* Cortex-R Processors: ARMv7-R Architecture
* Cortex-M Processors: ARMv7-M & ARMv6-M Architectures

![evolution-of-arm-architecture](https://cloud.githubusercontent.com/assets/14265605/10615814/79606356-7727-11e5-9fb4-cc1a4c509ff8.png)

### About ARM official references:
e.g. *ARMv7-M Architecture Reference Manual (reference 1)* (**must read**) contains: 
* Programmer’s model
* Instruction set
* Memory model
* Debug architecture

e.g. * Technical Reference Manual (TRM)* contains:
* Processor specific information, such as interface details and instruction timing.

e.g. the *Cortex-M3/M4 Devices Generic User Guides* (references 2 and 3) covers:
* the programming model of the processors
* various application notes.

![armv6m](https://cloud.githubusercontent.com/assets/14265605/10616042/b8ff81ee-7728-11e5-8143-4da91312bc27.png)

All the Cortex-M processors support Thumb-2 technology and support
different subsets of the Thumb ISA.
* Thumb-2 has both 16-bit and 32-bit instructions.
 * Thumb-2 Technology is a very important feature of ARMv7. 

## Processor naming

# Chapter 2: Introduction to Embedded Software Development
Typically, only 10% of the silicon die is occupied by the ARM processor.
To use a Cortex-M microcontroller, we only need to understand:
* the processors, e.g. how to use the interrupt features (read this book).
* the programmer's model of the peripherals (read the datasheet of your microcontroller).

Most microcontroller vendors
provide C header files and driver libraries for their microcontrollers, developed with the *Cortex Microcontroller Software Interface Standard (**CMSIS**)*.

In most cases, the processor does all the work of controlling the peripherals and
handles the system management.

The illustrations in this book are mostly based on the **Keil** Microcontroller
Development Kit (MDK-ARM) because of its popularity.

In order to help microcontroller software developers, microcontroller vendors usually
provide header files and C codes that include:
* Definitions of peripheral registers
* Access functions for configuring and accessing the peripherals

The development flow:
![development-flow](https://cloud.githubusercontent.com/assets/14265605/10621414/572a0a8e-7746-11e5-819c-a0168ae34ddc.png)

The compilation process:
![compilatioin](https://cloud.githubusercontent.com/assets/14265605/10621852/f023f6ee-7748-11e5-965e-9f7eec949be2.png)

GCC:
![gcc](https://cloud.githubusercontent.com/assets/14265605/10621980/cb766ef2-7749-11e5-9dec-4ace92c5018b.png)


## Software flow

### Polling
Super-loop polling to service multiple processes:
![polling](https://cloud.githubusercontent.com/assets/14265605/10622430/278e4586-774d-11e5-932f-24565b68396a.png)
Cons:
* can be cubersome when the application gets more complex.
* difficult to define priorities.
* energy is wasted during the polling when service is not required.

### Interrupt driven
Sleep to reduce power, when needs a service, wake up the processor:
![interrupt](https://cloud.githubusercontent.com/assets/14265605/10622521/dae5f700-774d-11e5-9ab2-9d0065485ffa.png)

When parts of the service is not urgent, we can combine polling and interrupt-driven methods:
![polling-interrupt-comb](https://cloud.githubusercontent.com/assets/14265605/10622567/0a554b30-774e-11e5-9cd7-34feed65364e.png)
Using this arrangement, we can reduce the duration of high-priority interrupt handlers
so that lower priority interrupt services can get served quicker. At the same time,
the processor can still enter sleep mode to save power when no servicing is needed.

## Multi-taskiing systems
Some tasks that can take a long time to execute might need to be processed concurrently.
* a Real-Time Operating System(RTOS) can be used to handle the task scheduling.
* Besides task scheduling, RTOSs also have many other features such as semaphores, message passing, etc.

![rtos](https://cloud.githubusercontent.com/assets/14265605/10622747/092081de-774f-11e5-87b9-5c8ac922728b.png)

### Data types in C programming
The integer is always 32-bit in the ARM architecture.

Conventions in ARM programming:

|Terms|Size|
|:---:|:--:|
|Byte | 8-bit|
|Half word|16-bit|
|Word|32-bit|
|Double word|64-bit|










