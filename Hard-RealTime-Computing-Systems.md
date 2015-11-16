# Chap 1: A General View

Laws on real-time systems:
* Murphy’s General Law
 * If something can go wrong, it will go wrong.
* Murphy’s Constant
 * Damage to an object is proportional to its value.
* Naeser’s Law
 * One can make something bomb-proof, not jinx-proof.
* Troutman Postulates
 * 1. Any software bug will tend to maximize the damage.
 * 2. The worst software bug will be discovered six months after the field test.
* Green’s Law
 * If a system is designed to be tolerant to a set of faults, there will always exist
an idiot so skilled to cause a nontolerated fault.
 * Corollary: Dummies are always more skilled than measures taken to keep them from
harm.
* Johnson’s First Law
 * If a system stops working, it will do it at the worst possible time.
* Sodd’s Second Law
 * Sooner or later, the worst possible combination of circumstances will happen.
 * Corollary: A system must always be designed to resist the worst possible combination
of circumstances.

## The meaning of *real-time*
***Time***: the correctness of the system depends not only on the logical
result of the computation but also on the time at which the results are produced.

***Real***: the reaction of the systems to external events must occur
*during* their evolution.

The system time is measured using the time in the real-world controlled environment (external time).
* the environment is always an essential component of any real-time system.

![real-time-control-system](https://cloud.githubusercontent.com/assets/14265605/11172068/da258b56-8bc4-11e5-8e95-fbbdb73f4b09.png)

The objective of fast computing is to minimize the average response
time of a given set of tasks, whereas the ***objective*** of ***real-time*** computing is to meet the individual
timing requirement of each task.
* Hence, rather than being fast, a real-time computing system should be predictable.

## Real-time tasks
**Hard**: A real-time task is said to be hard if producing the results after its deadline
may cause catastrophic consequences on the system under control.

**Firm**: A real-time task is said to be firm if producing the results after its deadline
is useless for the system, but does not cause any damage.

**Soft**: A real-time task is said to be soft if producing the results after its deadline
has still some utility for the system, although causing a performance degradation.

## Features of Real-time Systems
**Timeliness**
**Predictability**
**Efficiency**: most real-time systems are embedded systems.
**Robustness**: Real-time systems need to survive peak-load conditions.
**Fault tolerance**: Single H/S failures should not crash the whole system.
**Maintainability**: Hard real-time systems last a long time.

### Achieving predictability
*Non-deterministic factors*: 
* CPU: prefetcher, piplining, cache, DMA(CPU and I/O share the same bus, depending on how the system handles when I/O transfer and CPU memory cycle happen at the same time. *cycle stealing*: hard to predictibility. CPU waits DMA. *time-slice method*: more expensive but predictable.).
* OS: scheduling algorithm, synchronization mechanism, the types of semaphores, the memory management policy, the communication semantics, the interrupt handling mechanism.


#### Handling interrupts
Interrupts due to I/O requests: must be able to be handled w/o unbounded delay.

#### System calls
Each kernel primitive should be *preemptable*.

#### Semaphores
Non-real-time system semaphores suffer from ***priority inversion***.
* ***Basic Priority Inheritance***, ***Priority Ceiling*** and ***Stack Resource Policy*** policies are the rescue.
 * needs a substantial mod of the kernel.

#### Memory Management
*demand paging* is not suitable for real-time systems.
* design choice for balancing *predictability* against *flexibility*

#### Programming Languages
*Dynamic data structures*: predictablity cannot be determined at compile time.
*Recursions*: allowing recursion invalids schedulability analysis.
*loops*: time-bound or not? if not, then troubles.

*Real-time Euclid*, *Real-time Cocurrent C*.

> ***Real-time systems must be analyzable and predictable.***

# Chap 2: Basic Concepts

