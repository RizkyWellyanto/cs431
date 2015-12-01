# Introduction to CS 431
CS431 at the University of Illinois at Urbana-Champaign, taught by Dr. Lui Sha who is famous for his [Generalized Rate Monotonic Scheduling Theory](), covers the following topics:
* Microcontroller Programming
* Signal Processing
  * Fourier Analysis
  * Filtering
* Serial communication
* PID control and Dynamical System
* Inter-process Communication
* Task Scheduling
* Priority Servers
* Resource Access Protocols
* Real-Time Operation Systems
* Real-Time Networking

# Lab work
10 individual labs amount to a full-fledged embedded control system: [The AmazingBallSystem]().
* The hardware: 
  * a linux work station
  * an oscilloscope
  * a dsPIC33F board is set up with 
  * a joystick, 
  * an LCD display and 
  * a touch-screen platform whose two tilting angles are controlled by two separate servos
  * a metal ball rolling on the touch-screen platform, which is programmed to control the movement of the metal ball.
* IDE: MPLAB X
* Debugger: ICD 3
* the [μC/OS-II](http://micrium.com/rtos/ucosii/overview/) real-time kernel is used.
* at the end of the semester, the system will be able to do something like [this](). More importantly, the underlying source code must meet requirements of [Hard Real-Time Computing Systems](http://www.amazon.com/Hard-Real-Time-Computing-Systems-Applications/dp/1461406757/ref=sr_1_1?ie=UTF8&qid=1449008254&sr=8-1&keywords=hard+realtime+computing+system).
