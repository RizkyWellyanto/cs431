# Chap 1: A General View

Laws on real-time systems:
* Murphy’s General Law
 * If something can go wrong, it will go wrong.
* Murphy’s Constant
 * Damage to an object is proportional to its value.
* Naeser’s Law
 * One can make something bomb-proof, not jinx-proof.
* Troutman Postulates
 * 1. Any software bug will tend to maximize the damage.C
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
## Terminologies
![ready-queue](https://cloud.githubusercontent.com/assets/14265605/11217737/7056be58-8d17-11e5-88dd-429b0e1af754.png)

![concepts](https://cloud.githubusercontent.com/assets/14265605/11225529/43697ccc-8d40-11e5-819a-01faabaa49c6.png)

## Task Constraints
* Timing constraints
* Precedence constraints
* Resource constraints

Waiting state caused by resource constraints:
![waiting-state](https://cloud.githubusercontent.com/assets/14265605/11218104/23bcc338-8d19-11e5-9e3f-6df713a99785.png)
* a task *leaves* **waiting state** *to* the **ready state** for the scheduler to schedule according to ***priority***.

## Scheduling problems
* Tasks (Γ) (Jobs, J): J = {J1, J2, ..., Jn}
* Processors: P = {P1, P2, ..., Pm}
* Resources: R = {R1, R2, ..., Rs}

***Scheduling*** means assigning processors from P and resources from R to tasks from Γ in order to complete all tasks under the specified constraints.
* is **[NP-complete](https://en.wikipedia.org/wiki/NP_(complexity))**
 * **NP**: *Nondeterministic Polynomial time*: cannot be solved by a *deterministic Turing machine* (the set of rules prescribes at most one action to be performed for any given situation. ); CAN be solved by a *Non-deterministic Turing machine*.
 * **NP-complete** problem: whose solutions are sufficient to deal with any other **NP** problem in polynomial time. 
   * A decision problem Q is said to be **NP-hard** if all problems in **NP** are polynomially transformable to Q, but we cannot show that Q ∈ **NP**.
 * NP is the class of decision problems that the proof of a "YES" cannot be solved in polynomial time by a deterministic Turing machine.

## Scheduling Algorithms
*Preemptive* vs. *Non-preemptive*

*Static* vs. *Dynamic*

*Off-line* vs. *Online*

*Optimal* vs. *Heuristic*

***Guarantee-based Algorithm***: if taking a new task would voilate the guarantee of meeting all deadlines, the new task is rejected:
![guarantee-based-scheduling](https://cloud.githubusercontent.com/assets/14265605/11224834/99234782-8d3c-11e5-8853-fffad09091f6.png)

***Best-effort Algorithm***: may be OK for soft real-time systems

## Metrics for performance evaluation
The design of Cost functions:
![cost-functions](https://cloud.githubusercontent.com/assets/14265605/11224915/f2beec88-8d3c-11e5-89f0-975c1433df83.png)

## Scheduling Anomalies

Those seemingly benign "improvements" can lead to deadline misses: *Anomalies* in real-time systems: 
Number of processors increased, computation times reduced, Precedence constraints weakened, faster CPU.

# Chap 3: APERIODIC TASK SCHEDULING

## Earliest Due Date (EDD): tasks arrive at the same time: *1*|*sync*|*Lmax*
* uniprocessor, tasks are synchronous (arrive at the same time), no preemption, minimize the maximum of lateness.

Theorem: Given a set of n independent tasks, any algorithm that
executes the tasks in order of nondecreasing deadlines is optimal with respect to minimizing
the maximum lateness.

Time complexity: nlogn (sorting on priorities)

notes: the optimality of the EDD algorithm cannot guarantee the feasibility of the
schedule for any task set. It only guarantees that if a feasible schedule exists for a task
set, then EDD will find it.

## Earliest Deadline First (EDF): tasks with arbitrary arrival times: *1*|*preem*|*Lmax*

Theorem: Given a set of n independent tasks with arbitrary arrival times,
any algorithm that at any instant executes the task with the earliest absolute deadline
among all the ready tasks is optimal with respect to minimizing the maximum lateness.

Time complexity: nlogn

EDF is optimal in the sense of feasibility.
* if a feasible schedule exists, EDF can find it.
* also minimizes max_lateness.
### Feasibility Guarantee Checks
`c_i(t)`: the remaining worst-case exe time of task `J_i`.
the worst-case finishing tiime of `J_i`: `f_i = Sum(1,i,c_k(t))`.
The schedulability can be guaranteed by the following conditions: `Any i = 1, ..., n, Sum(1,i,c_k(t)) <= d_i`.

**Feasibility Guarantee**: checked dynamically if tasks have dynamic activations
```
Algorithm: EDF_guarantee(J_vector, J_new)
{
  J_vector.push_back(J_new);
  J_vector.sort_prio(); // ordered by deadline
  t = current_time();
  f_0 = 0;
  foreach J_i in J_vector {
    f_i = f_(i-1) + c_i(t);
    if(f_i > d_i)
      return(UNFEASIBLE);
  }
  return(FEASIBLE);
}
```

EDF is no longer optimal in terms of feasibility if tasks cannot be preempted.
* if a feasible schedule exists, EDF cannot guarantee to fiind it.
* but if *non-idle algorithm* is not allowed, EDF is still the optimal.
  * *non-idle*: permits the processor to be idle when there are active jobs.

## Non-preemptive Scheduling
NP-hard problem. Exhaustive searching is not practical. 

### Bratley's Algorithm: *1*|*NO_PREEM*|*FEASIBLE*
Uses additional info. to prune the search tree and reduce the complexity.
* a pruning technique to determine when a current search can be reasonably abandoned.
* In practice, abandon the search when:
  * the addition of any node to the current path causes a missed deadline.
  * a feasible schedule is found at the current path.
* improves average running time.

e.g.
![pruning](https://cloud.githubusercontent.com/assets/14265605/11294068/ef1fd5e8-8f25-11e5-9da6-cfe2c338a318.png)
* the worst-case complexity is still `O(n*n!)`.
* Bratley's algorithm can only be used in *off_line* mode when all task parameters are known.
  * usually a *time-triggered system*: the resulting chedule, the *task activation list*, can be stored in data structure.

### The Spring Algorithm: the search is driven by a *heuristic function* (*H*)
*H* is applied to each of the tasks that remain to be scheduled. *H* is flexible:
* if *H = a_i (arrival time)*: FirstComeFirstServe.
* if *H = C_i*: ShortestJobFirst.
* if *H = d_i*: EDF.

*Backtracking* can be used to continue the search after a failure.

## Scheduling with precedence constraints








