# A General View

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


