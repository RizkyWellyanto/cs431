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

## Inputs, outputs, and peripherals accesses
On most microcontrollers, the peripherals are memory-mapped. 

E.g. *General Purpose Input Output (**GPIO**)* register set as a number of pointers as:
```c
/* STM32F 100RBT6B e GPIO A Port Configuration Register Low */
#define GPIOA_CRL (*((volatile unsigned long *) (0x40010800)))
/* STM32F 100RBT6B e GPIO A Port Configuration Register High */
#define GPIOA_CRH (*((volatile unsigned long *) (0x40010804)))
/* STM32F 100RBT6B e GPIO A Port Input Data Register */
#define GPIOA_IDR (*((volatile unsigned long *) (0x40010808)))
/* STM32F 100RBT6B e GPIO A Port Output Data Register */
#define GPIOA_ODR (*((volatile unsigned long *) (0x4001080C)))
/* STM32F 100RBT6B e GPIO A Port Bit Set/Reset Register */
#define GPIOA_BSRR(*((volatile unsigned long *) (0x40010810)))
/* STM32F 100RBT6B e GPIO A Port Bit Reset Register */
#define GPIOA_BRR (*((volatile unsigned long *) (0x40010814)))
/* STM32F 100RBT6B e GPIO A Port Configuration Lock Register */
#define GPIOA_LCKR (*((volatile unsigned long *) (0x40010818)))
```
Then we can use the definitions directly:
```c
void GPIOA_reset(void) /* Reset GPIO A */
{
  // Set all pins as analog input mode
  GPIOA_CRL = 0; // Bit 0 to 7, all set as analog input
  GPIOA_CRH = 0; // Bit 8 to 15, all set as analog input
  GPIOA_ODR = 0; // Default output value is 0
  return;
}
```
But this coding style is not scalable for larger projects:
* each register address definition increases code size
* multiple instantiations of the same peripheral: definition are repeated => increases code size
* the reset function is not generalized enough

Solution: [define the peripheral registers as data structures](https://github.com/bo-rc/cs431/blob/master/ARM-Accredited-Engineer.md#common-practice-for-delcaring-peripheral-registers)

Typically, a peripheral requires an initialization process before it can be used. This might include some of the following steps (*All these initialization steps are carried out by programming peripheral registers in various peripheral blocks.*):
* Programming the clock control circuitry to enable the clock signal connection to
the peripheral, and clock signal connection to corresponding I/O pins if needed.
* Configure the operation mode of the I/Opins.
 * Most microcontrollers have multiplexed I/O pins that can be used for multiple purposes.
* you might also
need to program additional configuration registers to define the expected electrical
characteristics such as output type (voltage, pull up/down, open drain, etc.).
* Peripheral configuration.
 * use driver functions provided by the vendor to reduce the programming work.
* Interrupt configuration.

## Common practice for delcaring peripheral registers
*used by almost all Cortex-M microcontroller device-driver packages*

```c
typedef struct
{
  __IO uint32_t CRL; // The “__IO” is defined in a standardized header file in CMSIS. It implies a volatile data item that can be read or written to by software.
  __IO uint32_t CRH;
  __IO uint32_t IDR;
  __IO uint32_t ODR;
  __IO uint32_t BSRR;
  __IO uint32_t BRR;
  __IO uint32_t LCKR;
} GPIO_TypeDef;
```
Then each peripheral base address (GPIO A to GPIO G) is defined as pointers to the data structure:
```c
#define PERIPH_BASE ((uint32_t)0x40000000)
/*!< Peripheral base address in the bit-band region */
#define APB2PERIPH_BASE (PERIPH_BASE + 0x10000)
#define GPIOA_BASE (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE (APB2PERIPH_BASE + 0x1000)
#define GPIOD_BASE (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE (APB2PERIPH_BASE + 0x1800)
...
#define GPIOA ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE ((GPIO_TypeDef *) GPIOE_BASE)
...
```
When peripherals are declared using such a method, we can create functions that
can be used for each instance of the peripheral easily. For example, the code to reset
the GPIO port can be written as:
```c
void GPIO_reset(GPIO_TypeDef* GPIOx)
{
  // Set all pins as analog input mode
  GPIOx->CRL = 0; // Bit 0 to 7, all set as analog input
  GPIOx->CRH = 0; // Bit 8 to 15, all set as analog input
  GPIOx->ODR = 0; // Default output value is 0
  return;
}
```
To use this function, we just need to pass the peripheral base pointer to the function:
```c
GPIO_reset(GPIOA); /* Reset GPIO A */
GPIO_reset(GPIOB); /* Reset GPIO B */
```

## The Cortex microcontroller software interface standard (**CMSIS**)
**CMSIS-Core** (Cortex-M processor support
* a set of APIs for application or
middleware developers to access the features on the Cortex-Mprocessor regardless
of the microcontroller devices or toolchain used. Currently the CMSIS processor
support includes the Cortex-M0, Cortex-M0+, Cortex-M3, and Cortex-M4 processors
and SecurCore products like SC000 and SC300. Users of the Cortex-M1
can use the Cortex-M0 version because they share the same architecture.

**CMSIS-DSP library**： 
* in 2010 the CMSIS DSP library was released, supporting
many common DSP operations such as FFT and filters. The CMSIS-DSP is
intended to allow software developers to create DSP applications on Cortex-M
microcontrollers easily.

**CMSIS-SVD**:
* the CMSIS System View Description is an XML-based file
format to describe peripheral set in microcontroller products. Debug tool vendors
can then use the CMSIS SVD files prepared by the microcontroller vendors to
construct peripheral viewers quickly.

**CMSIS-RTOS**:
* an API specification for embedded OS
running on Cortex-M microcontrollers. This allows middleware and application
code to be developed for multiple embedded OS platforms, and allows better
reusability and portability.

**CMSIS-DAP**:
* the **CMSIS-DAP** (Debug Access Port) is a reference design for a
debug interface adaptor, which supports USB to JTAG/Serial protocol conversions.
This allows low-cost debug adaptors to be developed which work for
multiple development toolchains.

### Areas of standardization in **CMSIS-Core**: an **API**
From a software development point of view, the CMSIS-Core standardizes provides:
* **Standardized definitions** for the processor’s peripherals.
 * registers in **NVIC**
 * a system tick timer in the processor (**SysTick**)
 * an optional Memory Protection Unit (**MPU**)
 * various programmable registers in the System Control Block (**SCB**)
 * some software programmable registers related to debug features
* **Standardized functions** for
 * accessing processor’s features (Appendix E)
  * functions using **NVIC**
  * functions for accessing registers
   * direct access is possible but not portable
 * accessing special instructions easily
 * system exception handlers
 * system initialization
* Standardized software variables for clock speed information

In addition:
* A common platform for device-driver libraries
 * Each device-driver library has the same look and feel

### Organization of **CMSIS-Core**
The **CMSIS** files are integrated into device-driver library packages from microcontroller
vendors. Some of the files in the device-driver library are prepared by ARM
and are common to various microcontroller vendors. Other files are vendor/device
specific. 

In a general sense, we can define the **CMSIS** into multiple layers:
* Core Peripheral Access Layer: Name definitions, address definitions, and
helper functions to access core registers and core peripherals. 
 * This is processor specific and is provided by ARM.
* Device Peripheral Access Layer: Name definitions, address definitions of
peripheral registers, as well as system implementations including interrupt
assignments, exception vector definitions, etc. 
 * This is device specific (note: multiple devices from the same vendor might use the same file set).
* Access Functions for Peripherals: The driver code for peripheral accesses. 
 * This is vendor specific and is optional. You can choose to develop your application
using the peripheral driver code provided by the microcontroller vendor, or you
can program the peripherals directly if you prefer.

There is also a *proposed* additional layer for peripheral accesses:
* Middleware Access Layer: The idea is to develop a set of APIs for interfacing common peripherals
such as UART, SPI, and Ethernet. 
 * not existing yet. If this layer exists, developers of middleware
can develop their applications based on this layer to allow software to be ported
between devices easily.

The aim of **CMSIS** is to provide a common starting point, and the microcontroller vendors
can add additional functions if they prefer. 
* But software using these functions will need porting.

![cmsis-core](https://cloud.githubusercontent.com/assets/14265605/10638120/d5377a14-77ce-11e5-9ca1-d8075852e508.png)

### Use **CMSIS-Core**
![use-cmsis](https://cloud.githubusercontent.com/assets/14265605/10638228/77698bb0-77cf-11e5-8193-cf8bc8b99871.png)

* startup code in the form of assembly or C
 * Startup code is required for the starting sequence of the processor, and
it also includes the exception vector table definition that is required for interrupt
handling.
* `system_<device>.c`: device initialization code
 * `system_<device>.h`: its header
* `core_cm3.c` `core_cm4.c`: for **CMSIS**2.0 or older, processor-specific code
* `core_cm3.h` `core_cm4.h`: processor-specific headers
 
You can download the latest version of the CMSIS source package from [arm.com](http://www.arm.com/cmsis).

# Chapter 3: Techinical Overview

## General information

### Processor type
ARM Cortex-M are 32-bit **RISC** processors
* 32-bit registers
* 32-bit internal data path
* 32-bit bus interface

three-stage pipeline design
* instruction fetch
* decode
* execution

Harvard bus architecture
* allows simultaneous instruction fetches and data accesses.

The memory map is *unified*, which
means that although there can be multiple bus interfaces, there is only one 4GB
memory space. The memory space is used by the program code, data, peripherals,
and some of the debug support components inside the processors.

*Load-Store* architecture: This means data needs to be loaded from the memory, processed,
and then written back to memory using a number of separate instructions.

The details of the registers inside the
processors are commonly known as a programmer’s model.

### Processor architecture

### Instruction set
**Thumb-2** Technology: allows mixture of 16-bit and 32-bit instructions for high code density and high efficiency.

### Block diagram
![m3m4-block](https://cloud.githubusercontent.com/assets/14265605/10642653/5a16510a-77e4-11e5-875b-fd18b391e369.png)

Bus Interfaces on the Cortex-M3 and Cortex-M4 Processors:
* **I-CODE**: Primarily for program memory: Instruction fetch and vector fetch for
address 0x0 to 0x1FFFFFFF. Based on **AMBA** 3.0 AHB Lite bus
protocol.
* **D-CODE**: Primarily for program memory: Data and debugger accesses for
address 0x0 to 0x1FFFFFFF. Based on **AMBA** 3.0 AHB Lite bus
protocol.
* **System**: Primarily for RAM and peripherals: Any accesses from address
0x20000000 to 0xFFFFFFFF (apart from PPB regions). Based on
**AMBA** 3.0 AHB Lite bus protocol.
* **PPB**: External Private Peripheral Bus (PPB): For private debug components
on system level from address 0xE0040000 to 0xE00FFFFF. Based on
**AMBA** 3.0 APB protocol.
* **DAP**: Debug Access Port (DAP) interface: For debugger accesses generated
from the debug interface module to any memory locations including
system memory and debug components. Based on the ARM
CoreSight debug architecture.

The bus interfaces on the Cortex-M processors are 32-bit, and based on
the *Advanced Microcontroller Bus Architecture (**AMBA**)* standard. 
**AMBA** contains a collection of several bus protocol specifications.

### Memory system
The Cortex-M3/4 processors do not have memories
* no SRAM, no cache
* Instead, they come with a generic on-chip bus interface, so microcontroller vendors can add their own memory system to their design.
 * typically, the microcontroller vendor will need to add:
  * Program memory, typically flash
  * Data memory, typically SRAM
  * Peripherals

main bus interface protocol:
* **AHB Lite** (Advanced High-performance Bus)
* **APB** (Advanced Peripheral Bus)

### Interrupt and exception support
The address location of the **NVIC** is memory-mapped and is fixed.

## Features of Cotrex-M3/4

The majority of the Cortex-M microcontroller products use little endian.

If an **MPU**(Memory protection unit) is included,
applications can divide the memory space into a number of regions and define the
access permissions for each of them. When an access rule is violated, a fault exception
is generated and the fault exception handler will be able to analyze the problem
and, if possible, correct it.

By default the vector table is located at the beginning
of the memory space (address 0x0), but the vector table offset can be changed at
runtime if needed.

# Chapter 4: Architecture
Both ARMv7-M (Cortex-M3) and ARMv7E-M (Cortex-M4) features are documented in the same architecture specification document:
*the ARMv7-M Architecture Reference Manual*

No need to read the 1k-page manual, all you need are:
* a basic understanding of the programmer’s model
* how exceptions (such as interrupts) are handled
* the memory map
* how to use the peripherals
* how to use the software driver library files from the microcontroller vendors

## Programmer's model

### Operation modes and states
**Operation states**
* **Debug state**: When the processor is halted (e.g., by the debugger, or after hitting a
breakpoint), it enters debug state and stops executing instructions.
* **Thumb state**: If the processor is running program code (Thumb instructions), it
is in the Thumb state. Unlike classic ARM processors like ARM7TDMI, there
is no ARM state because the Cortex-M processors do not support the ARM instruction
set.

**Operation modes**
* **Handler mode**: When executing an exception handler such as an Interrupt Service
Routine (**ISR**). When in handler mode, the processor always has privileged
access level.
* **Thread mode**: When executing normal application code, the processor can be
either in privileged access level or unprivileged access level. This is controlled by
a special register called “CONTROL.”
![operation-states-and-mode](https://cloud.githubusercontent.com/assets/14265605/10656577/c8b1403c-7845-11e5-94ff-6b15cf8f2ca1.png)
* Software can switch the processor in privileged Thread mode to unprivileged Thread mode. However, it cannot switch itself back.
 * the processor has to use the exception mechanismto handle the switch-back.
* By default, the Cortex-M processors start in privileged Thread mode and in Thumb state

Besides the differences in memory access permission and access to several
special instructions, the programmer’s model of the privileged access level and
unprivileged access level are almost the same. Note that almost all of the **NVIC** registers
are privileged access only.

Thread mode has a separate shadowed stack pointer. Unprivileged Thread mode can be unused in simple applications.

### Registers (Registers in the register bank)
ARM uses ***Load-Store Architecture***: if data in
memory is to be processed, it has to be loaded from the memory to registers in
the **register bank**, processed inside the processor, and then written back to the memory,
if needed.


![register-bank](https://cloud.githubusercontent.com/assets/14265605/10656743/b5853a8e-7847-11e5-94a1-c6da3af6e438.png)

**R0** - **R12**: 
Due to the limited available space in the instruction set, many
16-bit instructions can only access the low registers. The high registers (R8 e R12)
can be used with 32-bit instructions, and a few with 16-bit instructions, like MOV
(move). The initial values of R0 to R12 are undefined.

**R13**, **Stack Pointer(SP)**:
R13 is the Stack Pointer. It is used for accessing the stack memory via PUSH and POP
operations. Physically there are two different Stack Pointers: the Main Stack Pointer
(MSP, or SP_main in some ARM documentation) is the default Stack Pointer. It
is selected after reset, or when the processor is in Handler Mode. The other Stack
Pointer is called the Process Stack Pointer (PSP, or SP_process in some ARM documentation). The PSP can only be used in Thread Mode. The selection of Stack
Pointer is determined by a special register called CONTROL, which will be explained
in section 4.2.3. In normal programs, only one of these Stack Pointers will be visible.

Both MSP and PSP are 32-bit, but the lowest two bits of the Stack Pointers (either
MSP or PSP) are always zero, and writes to these two bits are ignored. In ARM
Cortex-M processors, PUSH and POP are always 32-bit, and the addresses of the
transfers in stack operations must be aligned to 32-bit word boundaries.

For most cases, it is not necessary to use the PSP if the application doesn’t
require an embedded OS. Many simple applications can rely on the MSP completely.
The PSP is normally used when an embedded OS is involved, where the stack for the
OS kernel and application tasks are separated. The initial value of PSP is undefined,
and the initial value of MSP is taken from the first word of the memory during the
reset sequence.

**R14**, **Link register (LR)**:
This is used for holding the return address when calling a function or subroutine.
* If a function needs to call another function or subroutine, it needs to save the value of LR in the stack first.

Although the return address values in the Cortex-M processors are always even
(bit 0 is zero because the instructions must be aligned to half-word addresses), bit
0 of LR is readable and writeable. Some of the branch/call operations require that
bit zero of LR (or any register being used) be set to 1 to indicate Thumb state.

**R15**, **Program Counter (PC)**:
It is readable and writeable: a read returns the
current instruction address plus 4. Writing to PC
(e.g., using data transfer/processing instructions) causes a branch operation.

The instructions must be aligned to half-word or word addresses, the Least Significant Bit (LSB) of the PC is zero.

When using some of the branch/
memory read instructions to update the PC, you need to set the LSB of the new PC
value to 1 to indicate the Thumb state.
* In high-level programming
languages (including C, C++), the setting of LSB in branch targets is handled by
the compiler automatically.

![register-names](https://cloud.githubusercontent.com/assets/14265605/10656965/a10c529c-784a-11e5-9db3-594c832fd0d9.png)

### Special registers (Registers not in the register bank)
![special-register](https://cloud.githubusercontent.com/assets/14265605/10657024/6e273fe4-784b-11e5-8775-69800fd7511d.png)

These registers contain the processor status and define the operation
states and interrupt/exception masking.
* needed for development of and embedded OS, or when advanced interrupt masking features are needed.

Special registers are not memory mapped, and can be accessed using special register
access instructions such as `MSR` and `MRS`.
```
MRS <reg>, <special_reg>; Read special register into register
MSR <special_reg>, <reg>; write to special register
```
* **CMSIS-Core** also provides a number of C functions that can be used to access
special registers.

#### Program Status Registers
**PSR** is composed of:
* Application PSR (APSR)
* Execution PSR (EPSR)
* Interrupt PSR (IPSR)

These three registers can be accessed as one combined register,
referred to as xPSR in some documentation. In ARM assembler, when accessing
xPSR, the symbol PSR is used. For example:
```
MRS r0, PSR ; Read the combined program status word
MSR PSR, r0 ; Write combined program state word
```

You can also access an individual PSR. For example:
```
MRS r0, APSR ; Read Flag state into R0
MRS r0, IPSR ; Read Exception/Interrupt state
MSR APSR, r0 ; Write Flag state
```

The EPSR cannot be accessed by software code directly using MRS (read as zero) or MSR

The IPSR is read only and can be read from combined PSR (xPSR).

![psr](https://cloud.githubusercontent.com/assets/14265605/10657152/b539e494-784c-11e5-864e-7fe89c7386de.png)

![psr-bit-fields](https://cloud.githubusercontent.com/assets/14265605/10657200/582bfd68-784d-11e5-9d67-7f784b0eb8a5.png)

















