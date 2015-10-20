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


