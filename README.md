# fifofast


## Introduction
On microcontrollers (MCUs) the application program often has to collect and process data from multiple sources such as sensors, ADCs or external communication interfaces. Since truly parallel handling of all tasks is not possible, it is reasonable to buffer incoming data until the previous task is completed.

A simple fifo is best suited for this purpose. Likely you have written your own implementation at least once during your studies. It turns out that although the common textbook approach is great for PCs, it is unnecessary bloated for MCUs. It still might be ok for your applications, but if you try to handle >1kB/s on an AVR controller (or similar), you might notice a performance hit.


## Problems of the textbook approach

### Dynamic memory allocation
Without question dynamic memory allocation is crucial for the flexibility PCs offer. On MCUs however the memory requirements are usually known at compile time and functions like `malloc` add unnecessary overhead.

### Excessive pointer use
Even on 8bit MCUs the pointer size is usually 16bit, so loading and manipulating pointers takes multiple cycles. On top of that the AVR8 series only has 3 register pairs that can be used for pointers. 

### Function calls
A regular function call on an AVR8 MCU takes about 10 to 20 additional cycles, depending on the complexity of the function. If a function is called from an ISR, the compiler adds additional `push` and `pop` instructions, which roughly doubles the cycles required. Long ISRs can result in missed interrupts and limit the minimum allowed time between two interrupts of the same kind.

### Excessive use of conditions
On AVR8 MCUs a if statement take up to 4 cycles to process, on MCUs with a pipeline the effects are even worse. Therefore it is reasonable to limit the use to as little as possible.

### Fixed data type
The few fifo implementations which use static memory allocation are limited to one data type only. If you have to support another data type the "solution" is to copy & paste all code and change the data type as well as a few names.


## Summary

fifofast is a fifo for C/C++ based on macros with the aim to solve all the problems mentioned above. The basic idea is to create 2ⁿ ring buffers as introduced on the german [mikroconterller.net wiki](https://www.mikrocontroller.net/articles/FIFO#2n-Ringpuffer_-_die_schnellste_L.C3.B6sung) to reduce the amount of if-statements required. The macros create all required C code, which is then further optimised by the compiler. The resulting assembler code has minimum overhead and is very fast¹ on almost every target architecture.

¹ no scientific performance comparisons have been done, yet


## Features

### MIT License
The great thing about basic code is that it can be used over and over in all kinds of projects without the need to re-invent the wheel every time. With fifofast I want to give you not only a basic, yet highly efficient building block, but also the freedom to do use it the way you need to without all that legal hassle. If this code has become a part of your projrct, I'd appreciate a mention and a link back to this page so your users can benifit, too. Thanks!

### Static Memory
fifofast is 100% based on static memory, which makes it more efficient on embedded systems than dynamic implementations.

### Uses function-like macros
Function-like macros behave a lot like inline functions: the code is pasted wherever the function-like macro is called. This significantly increases the execution speed if called from ISRs. In addition macros allow more flexibility and improved compiler optimisation.

### Supports Generic Data
fifofast supports all types with any size including custom structures as long as they are typedef-d. Each fifo can store 2^n elements depending on your requirements.

### Minimal RAM required
In the basic implementation from the inspiration the last element is never used. This is problematic for larger data types, i.e. if you want to buffer complete frames. fifofast uses a 3rd index variable (single byte) to circumvent this issue. In addition the size of the index variables is selected based on the size of the fifo.

### Easy to use
All functions you expect from a fifo are implemented in fifofast. In addition you get special functions like `peek` which allows to access the memory like an array without changing the contents. To try each function-like macro on its own, edit the included file fifofast_demo.c and inspect it with the simulator and debugger.

### Safe
The macros are designed to prevent common run-time errors such as array overflow and type mismatch. This is mostly done by clever design or by letting the compiler run into an error/ warning if possible. For the few macros that do require additional instructions for run-time checks, faster `_lite`-variants are provided.

### Extension: Pointable fifos (WIP)
Occasionally you may wish to pass a pointer of a fifo to a function, which is not possible with the standard fifos implemented. Instead you can create a pointable fifo, which requires some additional RAM but can be accessed by the provided inline functions (and with the regular macros, too).

### Supports Debugging
In AtmelStudio each fifo can be inspected in the debugger, just like any other struct.


## Requirements, Limits, Downsides

### Fifo size
The fifo size is limited to 2ⁿ elements to make use of the fast wrapping functionality. Other sizes will be automatically rounded up. Pointable fifos are limited to 256 elements with a size of up to 256 bytes each.

### ISR-safe access
If a fifo is accessed from an ISR, it is recommended to make all accesses form regular code atomic. Otherwise glitches may occur with some function combinations (further investigation required).

### Programm memory usage
Each function-like macro pastes new code at its location. Compared to a function-based fifo the program memory usage (flash) is higher.

### GNU C/ GCC compiler
Some macros require compiler extensions, namely _compound statetments_ and _typeof_. You may be able to get away without them if you are willing to give up on some flexibility (and change the code yourself).

### Compiler optimisation
The macros contain a lot of literals and arithmetics with them. All of this extra code will be optimised away automatically unless optimisation is disabled. In that case the resulting assembler code will be incredible slow, so do not use the `-o` setting!


## Documentation
Since this is an early development version the documentation is currently only included as comments in the source file(s). You may use the fifofast_demo.c as example code and/or to test each macro with the simulator and debugger.


## Author's Note (The current state of this code)
The first version of this fifo was created about a year ago. Since then I've used the macros for multiple projects and different MCU architectures (AVR8, SAMG and SAML) and it seems to be working fine. As of 0.2.0 all macros used in the file fifofast_demo.c work as expected and can be safely used in your programms. The more complex macros `_fff_read...()`, `_fff_write...()` and `_fff_add...()` can possibly further optimized, but their use will likely remain the same. For now, inline functions and pointable fifos have been removed (but will be added again in the future).

Despite being "uncomplete" I decided to publish the code I've written so far, so you can start to use it right now. Besides bugfixes and optimisations I plan on adding a few additional features, although only when I need them for other projects.
