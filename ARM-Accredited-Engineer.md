# Chapter 1: Introduction to ARM Cortex-M Processors

The ISA in Cortex-M: **Thumb ISA**
* based on **Thumb-2** technology which supports a mixture of 16-bit and 32-bit instructions.

Harvard bus architecture with unified memory space(***Modified Harward architecture***): instructions and data use the same address space. 
* Harvard architecture: physically *separate* storage and signal pathways for *instructions* and *data*. 
 * In other words, a memory address does not uniquely identify a storage location (as it does in a Von Neumann machine); you also need to know the memory space (instruction or data) to which the address belongs.
* Modified Harvard architecture: allows the contents of the instruction memory to be accessed as if it were data.
 * loosen the strict separation between code and data, while still supporting the higher performance concurrent data and instruction access of the Harvard architecture.
* Von Neumann architecture: same address space - code can also be accessed and treated the same as data, and vice versa. 

