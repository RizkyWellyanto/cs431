# [Self-clocking signal](https://en.wikipedia.org/wiki/Self-clocking_signal)
A self-clocking signal is one that can be decoded without the need for a separate clock signal or other source of synchronization. This is usually done by including embedded synchronization information within the signal, and adding constraints on the coding of the data payload such that false synchronization can easily be detected.

If a clock signal is embedded in the data transmission, there are two possibilities: 
1. the clock signals are sent at the same time as the data (isochronous)
2. at a different time (anisochronous).

## isochronous example: [Manchester coding](https://en.wikipedia.org/wiki/Manchester_code)
![Self-clocking Manchester coding](https://upload.wikimedia.org/wikipedia/commons/thumb/9/90/Manchester_encoding_both_conventions.svg/771px-Manchester_encoding_both_conventions.svg.png)

The data and clock cycles can be thought of as "adding up" to a combination, where both the clock cycle and the data can be retrieved from the transmitted signal.

## anisochronous example

Anisochronous self-clocking signals do not combine clock cycles and data transfer into one continuous signal. Instead, the transmission of clock cycles and data transmission is modulated. 

![Asynchronous serial communication](https://upload.wikimedia.org/wikipedia/commons/4/47/Puerto_serie_Rs232.png)

Instead of a common synchronization signal, the data stream contains synchronization information in form of start and stop signals, before and after each unit of transmission, respectively. The start signal prepares the receiver for arrival of data and the stop signal resets its state to enable triggering of a new sequence.

[Morse code](https://en.wikipedia.org/wiki/Morse_code) is one anisochronous example.

## [Non-return-to-zero](https://en.wikipedia.org/wiki/Non-return-to-zero)

# [Ring buffer](https://en.wikipedia.org/wiki/Circular_buffer)
## [The ring buffer](http://www.embedded.com/electronics-blogs/embedded-round-table/4419407/The-ring-buffer)
Just a fixed-size circular array. Good for buffering data streams.

# [UART](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver/transmitter)
A universal asynchronous receiver/transmitter, is a computer hardware device that translates data between parallel and serial forms.

Most serial communication must first be converted back into parallel form by a UART before they may be directly connected to a data bus.

# [CRC](https://en.wikipedia.org/wiki/Cyclic_redundancy_check)
A CRC-enabled device calculates (*hashes*) a short, fixed-length binary sequence, known as the check value or CRC, for each block of data to be sent or stored and appends it to the data, forming a *codeword*. When a codeword is received or read, the device either compares its check value with one freshly calculated from the data block, or equivalently, performs a CRC on the whole codeword and compares the resulting check value with an expected residue constant. If the check values do not match, then the block contains a data error. The device may take corrective action, such as rereading the block or requesting that it be sent again. Otherwise, the data is assumed to be error-free.

## CRC algorithm is based on [*cyclic code*](https://en.wikipedia.org/wiki/Cyclic_code)
In coding theory, a cyclic code is a block code, where the circular shifts of each codeword gives another word that belongs to the code. They are error-correcting codes that have algebraic properties that are convenient for efficient error detection and correction.

CRC just works: using its coding-decoding algorithm, we are able to correct errors. Think this as [magic](https://www.youtube.com/watch?v=ZJH0KT6c0B0).

# [Polling](http://www.electronics-base.com/useful-info/software-related/90-polling-vs-interrupt)
## Polling vs Interrupt
If the event of interest is:
* Asynchronous
 * timing critical
* Urgent
then an interrupt based handler would make sense.

If the event of interest is:
* Synchronous (i.e. you know when to expect it within a small window)
* Not Urgent (i.e. a slow polling interval has no ill effects)
* Frequent (i.e. majority of your polling cycles create a 'hit')
then polling might be a better fit.

There are many advantages of using Interrupts. The microcontroller can serve many devices. Each device can get service based on the priority assigned to it. The microcontroller can ignore (mask) a device request. The use of microcontroller is more efficient.

Polling is good for operations that are not dependent on exact timings. By using this you can create multitasking on microcontroller.

# [Memory-mapped I/O vs Port-mapped I/O](http://www.bogotobogo.com/Embedded/memory_mapped_io_vs_port_mapped_isolated_io.php)

Microprocessors normally use two methods to connect external devices: memory mapped or port mapped I/O. However, as far as the peripheral is concerned, both methods are really identical.

Memory mapped I/O is mapped into the same address space as program memory and/or user memory, and is accessed in the same way.

Port mapped I/O uses a separate, dedicated address space and is accessed via a dedicated set of microprocessor instructions.

## MMIO
The advantage to this method is that every instruction which can access memory can be used to manipulate an I/O device.

The disadvantage to this method is that the entire address bus must be fully decoded for every device. For example, a machine with a 32-bit address bus would require logic gates to resolve the state of all 32 address lines to properly decode the specific address of any device. This increases the cost of adding hardware to the machine.

## PMIO
From a software perspective, this is a slight disadvantage because more instructions are required to accomplish the same task. For instance, if we wanted to test one bit on a memory mapped port, there is a single instruction to test a bit in memory, but for ports we must read the data into a register, then test the bit.

## Transfer function in Control

## Laplace transform (control) and Fourier transform (signal processing)

# Lecture 16

## criticality vs. priority
Criticality: importance of the task
Priority: execution time.
* which one should start first

## real-time system requirements

1. Schedulable utilization

2. Worst-case response time for each of tasks

3. Stability (getting critical tasks done)

FAA forgids EDF scheduling because it aces at 1. but fails at 2. and 3.





