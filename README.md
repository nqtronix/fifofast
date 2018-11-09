
<h1 align="center" style="font-weight: bold; margin-top: 20px; margin-bottom: 20px;">fifofast</h4>

<h3 align="center" style="font-weight: bold; margin-top: 20px; margin-bottom: 20px;">A fast, generic fifo for MCUs.</h4>

<p align="center">
	<a href="#changelog"><img src="https://img.shields.io/github/release-pre/nqtronix/fifofast.svg" alt="release: NA"></a>
    <a href="#about"><img src="https://img.shields.io/badge/language-C_(GCC_5.4.0)-blue.svg" alt="language: C GCC (5.4.0)"></a>
    <a href="#about"><img src="https://img.shields.io/badge/platform-MCUs, AVR8-blue.svg" alt="platform: MCUs, AVR8"></a>
	<a href="#about"><img src="https://img.shields.io/badge/status-active-brightgreen.svg" alt="status: active"></a>
	<a href="https://github.com/nqtronix/fifofast/issues"><img src="https://img.shields.io/github/issues/nqtronix/fifofast.svg" alt="issues: NA"></a>
	<a href="#license"><img src="https://img.shields.io/github/license/nqtronix/fifofast.svg" alt="license: NA"></a>
</p>

<p align="center">
  <a href="#getting-started">Getting Started</a> •
  <a href="#documentation">Documentation</a> •
  <a href="#under-the-hood">Under the Hood</a> •
  <a href="#support">Need Help?</a> •
  <a href="#contribute">Contribute</a> •
  <a href="#about">About</a> •
  <a href="#credits">Credits</a>
</p>


## Introduction
First-In-First-Out (FIFO) buffers are one of the most used data structures, especially on micro controllers (MCUs) to handle read time data input/output. Although there are countless of implementations, there wasn't a single one that is well optimized for MCUs.

**fifofast** was specifically designed to consume as little CPU time and SRAM as possible, while providing more versatility and features than typical implementations. It is ideally suited to buffer serial data, ADC measurement results or other data of any kind.

<br>

## Key Features
 - **generic data:** fifofast supports **_any_** data type, even custom typedef'd ones
 - **static memory:** no additional overhead through dynamic memory management
 - **inline support:** speeds up execution, especially from ISRs (Interrupt Service Routines)
 - **minimal RAM:** a typical fifo has only **3 _byte_** management overhead
 - **easy to use:** all typical fifo functions are implemented (and they work like you expect)
 - **supports debugging:** with the build-in debugger of Atmel Studio 7
 - **well documented:** extensive use of comments within the source code

<br>

## Limititations
- **Fifo size:**<br>
  The fifo size is limited to 2ⁿ elements to make use of the fast wrapping functionality. Other sizes will be automatically rounded up. Pointable fifos are limited to 256 elements with a size of up to 256 bytes each.
   
- **Programm memory usage:**<br>
  Each function-like macro or inline function pastes new code at its location. Compared to a regular function-based fifo the program memory usage (flash) is higher.

<br>

## Minimal Code Example
The declaration of a fifo is slightly different to support generic types, but they are accessed just like you'd expect. This is the minimum code example that can be compiled:

```c
#include "fifofast.h"

// declare a fifo with 16 elements of type 'int_8' with the name 'fifo_uint8'
_fff_declare(int8_t, fifo_int8, 16);

int main(void)
{
	// volatile prevents the compiler from optimizing the variable away
	volatile int8_t tmp;

	// write a value to the fifo
	_fff_write(fifo_int8, -42);

	// read back the value from the fifo
	tmp = _fff_read(fifo_int8);

	// 'tmp' contains now the value '-42'
	while(1);
}
```

You find this a variation of this snippet and much more in [`fifofast_test.c`](fifofast_test.c).

<br>

## Getting Started
This section is written especially for everyone who is **not familiar** with the used tools. If you run into problems, please [ask for clarification](#get-help).<br>

### Step 1: Software and Tools
 - [**Atmel Studio 7.0**][tool-atmel-studio-7-0]** (Build 1931) [free]**<br>
   The installer contains all tools you need to open, edit, compile, simulate and flash this code. If you favor another development tool, feel free to use it instead. (But please understand that I can not provide any support).
 - **An AVR8 ISP/JTAG programmer [optional]**<br>
   To program AVR8 MCUs I use the [AVR Dragon][tool-avr-dragon]. It can be also used as a debugger and is available within Atmel Studio by default.

### Step 2: Download fifofast
 - Clone this repository or hit [Download][git-download] and extract the .zip file.

### Step 3: Browse the project
 - **Open the project in Atmel Studio:**<br>
   Either double click `fifofast.atsln` or open Atmel Studio and select "File -> Open -> Project/Solution..."

 - **Open any file of your interest:**<br>
   Select the file in the top right window "Solution Explorer". If the window is not visible, open it by pressing `CTRL + ALT + L` or selecting "View -> Solution Explorer" from the menu.

### Step 4: Run the demo
 - **Compile the demo code:**<br>
   Press `F7` or select "Build -> Build Solution" from the menu
 
 - **Run the demo code in the simulator:**<br>
   Press `CTRL + F5` or select "Debug -> Start Debugging and Break" from the menu
 
 - **Place breakpoints:**<br>
   Left-Click on the lightgrey area left of the code to place or remove a breakpoint. Select lines with the comment "easy breakpoint".
   
 - **View variable values:**<br>
   When the code is paused, hover over any variable to display its value. Alternately you can "Right-Click -> Watch" to display the value in the "watch window".
 
 - **Run Code:**<br>
   Press `F5` to run to the next breakpoint or `F10` to execute one step.
 
### Step 5: Going further
 - **Changing the target device:**<br>
   Press `ALT + F7` and select "Device -> Change device..." to select your desired MCU.
 
 - **Program a real device:**<br>
   Connect your programmer, press `ALT + F7` and select "Tool". Choose your tool, your programming interface and wire up your MCU. Press `CTRL + ALT + F7` to flash the code to the MCU. Non-official programmers are not supported by Atmel Studio.

<br>

## Documentation

### API

To keep the documentation up-to-date with the least hassle, all configuration options, functions and their arguments are explained in a comment right in front of the declaration. See `fifofast.h` for more information. This section will be updated as soon as this project hits version 1.0.0.

<br>

### Pointable Fifos
Since release 0.6.0 you can create special fifos which can be referenced by a pointer. They are created very similar to normal fifos, but they have a `_p` as a suffix:

```c
// declare pointable fifo
_fff_declare_p(uint8_t, fifo_uint8p, 4);

//init pointable fifo
_fff_init_p(fifo_uint8p);
```

<br>

To access such a fifo you have two options:

1. Pass its pointer to one of the implemented functions OR
2. Use its identifier in a macro

Generic data in C can only be archived with pointers and type casts. This can be very confusing, so let me demonstrate it with examples:

```c
// 'fifo_uint8_p' has its own type depending on data type and depth, but its header looks like fff_proto_t,
// which is the only type excepted by the functions. Therefore you need to convert the pointer first:
uint8_t tmp_is_empty = fff_is_empty((fff_proto_t*)&fifo_uint8_p);

// alternatively you can create a new temporal pointer like this:
fff_proto_t* fifo_pointer = (fff_proto_t*)&fifo_uint8_p;

// to write you need to pass a pointer to the data location. NO type check can be performed!
uint8_t tmp_value = 42;
fff_write(fifo_pointer, &tmp_value);

// if you read data, you will only receive a void pointer. This needs to be cast into a pointer of the right
// type and then de-referenced:
uint8_t tmp_read = *(uint8_t*)fff_peek_read(fifo_pointer, 0);   // returns '42'
```

Type conversions are often considered to be an _evil_ feature of C, as it hides all type mismatches. To reduce the chance of bug, **only use these inline functions where absolutly required!**


<br>

### FIFO Arrays
For some applications you may need multiple identical fifos which can be selected with an index.

To create a fifo array, declare its structure first:
```c
// declare an array (suffix _a) of fifos with 16 elements each.
_fff_declare_a(uint8_t, fifo_array, 16, 5);
```
Next initialize it and specify the amount of fifos you need:
```c
// initialize an array (suffix _a) of 5 fifos.
_fff_init_a(fifo_array, 5);
```
Now you can access each fifo like this:
```c
// write to the fifo at index 'fifo_nr' the value 'data'
_fff_write(fifo_array[fifo_nr], data);
```
<br>

### Aligned Data
Because **fifofast** supports any data type, it may also be used to store frames for a serial data transmission. It is often useful to access the data not only in binary (`raw`) format, but also as a struct (`header`):
```c
typedef struct  
{
    uint16_t control;
    uint8_t length;
    uint8_t id;
    uint8_t data[];
} header_t;

typedef union __attribute__((aligned(2), packed))
{
    uint8_t raw[32];
    header_t header;
} frame_u;
```
In this case the header variable `control` is 2 bytes large and must be stored aligned
on most architectures. The union however is treated by default as a `uint8_t` array, so no alignment is enforced. To do this manually, GCC supports the [`__attribute__((aligned(n)))`][doc-gcc-alignment]. If a struct or union is declared like this, it will be correctly stored in the fifo.

To align any non-typedef'd data, you can declare a fifo like this:
```c
_fff_declare(uint8_t __attribute__((aligned(4))), fifo_uint8, 4);
```

<br>

## Under the Hood
To use **fifofast** you don't need to know its inner workings. This chapter is for those who seek to understand and learn.


### The Typical Implementation
To get the best performance most fifos are based on an array for data storage. New elements are always placed at the next free index. When the fifo is read, the element of the earliest written index is returned. Example:

	empty array:
	┌───┬───┬───┬───┬───┬───┬───┬───┐
	│   │   │   │   │   │   │   │   │
	└───┴───┴───┴───┴───┴───┴───┴───┘

	write 4 elements:
	┌───┬───┬───┬───┬───┬───┬───┬───┐
	│ a │ b │ c │ d │   │   │   │   │
	└───┴───┴───┴───┴───┴───┴───┴───┘

	read 2 elements:
	┌───┬───┬───┬───┬───┬───┬───┬───┐
	│   │   │ c │ d │   │   │   │   │
	└───┴───┴───┴───┴───┴───┴───┴───┘


After the array was filled at least once, new data must be added to the very first location. This is called a [circular buffer (wiki)][wiki-circular-buffer]

	after some time (example):
	┌───┬───┬───┬───┬───┬───┬───┬───┐
	│   │   │   │   │   │ e │ f │ g │
	└───┴───┴───┴───┴───┴───┴───┴───┘

    write 1 element:
	┌───┬───┬───┬───┬───┬───┬───┬───┐
	│ h │   │   │   │   │ e │ f │ g │
	└───┴───┴───┴───┴───┴───┴───┴───┘

To detect this overflow the straight forward approach is to use `if(index > array_size){index = 0;}`. This comparison has to be done for _every_ fifo access. Branches take typically more cycles than arithmetic instruction, especially if a [instruction pipeline (wiki)][wiki-pipelining] is used within the MCU.

<br>

### 2ⁿ Ring Buffer
If the array has a length of 2ⁿ elements, this `if` can be replaced with a faster logic instruction.

```c
// increment the write index by 1
index = ((index+1) & (array_size-1));
```

Let's take the example above and observe the bahavior:

```c
// start values
uint8_t array_size  = 8;
uint8_t index       = 5;

// first increment                v equivilant binary          v decimal result
index = ((5+1) & (8-1));   // == (0b0110 & 0b0111) == 0b110 == 6
index = ((6+1) & (8-1));   // == (0b0111 & 0b0111) == 0b111 == 7
index = ((7+1) & (8-1));   // == (0b1000 & 0b0111) == 0b000 == 0
index = ((0+1) & (8-1));   // == (0b0001 & 0b0111) == 0b001 == 1
```

As you can see, the logic operation works exactly as we want! Of course you don't have to deal with this detail, **fifofast** takes care of that for you. For more info look in the [resource](#resources) section.

<br>

### Generic Data
Support for generic data types is not a part of C so **fifofast** has to use a creative work-around with macros. The key are the `_fff_declate(...)` macros:

```c
#define _fff_declare(_type, _id, _depth)            \
struct _FFF_NAME_STRUCT(_id)                        \
{                                                   \
    _FFF_GET_TYPE(_depth) read;                     \
    _FFF_GET_TYPE(_depth) write;                    \
    _FFF_GET_TYPE(_depth) level;                    \
    _type data[_FFF_GET_ARRAYDEPTH16(_depth)];      \
} _id
```

Each macro "call" declares a new struct with an individual type and identifier. Therefore each of these struct can have members of a different size. However all structs have identical member identifiers, so if the `_id` known, a member can be accessed with `_id.member` like a normal struct. The compiler knows all datatypes used within the structure and generates appropiate code.

<br>

## Support

### Get Help

**Something doesn't work as expected?** No worries! Just open up a new issue in the [GitHub issue tracker][git-issues]. Please provide all information to reproduce your problem. If you don't have a GitHub account (and can't be bothered to create one,) you can [contact](#contact) me directly.

### Contribute

**Spotted an error?** [Open an issue][git-issues] or submit a pull request.

There is no CONTRIBUTING.md yet, sorry. Contributions will inherit the [license](#license) of this project. If you have any questions, just ask.

<br>

## About
### Status
**This project is currently classified as <a href="https://github.com/nqtronix/git-template/blob/master/badges.md#project-status"><img src="https://img.shields.io/badge/status-active-brightgreen.svg" alt="status: active"></a> :**<br>
_The developers are working on new features, optimisations, documentation or another part of the project. The code will be kept in working condition by updating dependencies, fixing bugs and solving issues with a high priority._

The first version of this fifo was created about a year ago. Since then I've used the macros successfully for multiple projects and different MCU architectures (AVR8, SAMG and SAML). fifofast is activly used for upcoming projects and will receive additional features whenever I need them.

### Known Issues
 - **Non-Atomic Glitches:**<br>
   Accessing the same fifo from normal _and_ ISR code can cause glitches with some function combinations. To prevent this, put the normal code in an [atomic block][doc-gcc-atomic] if the fifo is also accessed in an ISR.
 
### Planned Features

- none (for now)

### Changelog
This project uses [**Semantic Versioning 2.0.0**][semver.org]. During initial development (0.x.x versions) any _major_ increase is substituted with a _minor_ increase (0.1.0->0.2.0 instead of 0.1.0->1.0.0).

The message of each commit contains detailed information about the changes made. The list below is a summary about all significant improvements.

 - **0.5.0 (latest)**
   - testing now automated with the brand new [unittrace][git-repo-unittrace].
   - _finally_ polished up this readme :tada:
 - **0.4.0**
   - array with several fifos now possible
   - aligned data example provided
 - **0.3.0**
   - complete _and successful_ test of all macros (this time for real)
 - **0.2.0**
   - ~~complete~~ test of all macros
   - changed fifo struct to improve readability
   - MIT License added to git
 - **0.1.0**
   - initial commit

### Contact

If you haven't done so already, please check out [Get Help](#get-help) for the fastest possible help on your issue. Alternatively you can find my public email address on my [profile][git-profile].

<br>


## Credits

### Projects Used

 - [**git-template**][git-repo-git-template] - _A simple and clean git repository template._<br>

 - [**unittrace**][git-repo-unittrace] - _A simple testing and debugging tool for MCUs inspired by MinUnit_<br>
Specifically written for this project, because testing became to annoying.

<br>

### Related Projects

 - none (yet)
 
Want yours to be listed here, too? Create a merge request or [**get in touch**](#get-help).

<br>

### Additional Resources
 - [**2ⁿ circular buffer explanation (German)**][article-2n-buffer] or [English (with google translate)][article-2n-buffer-eng]
 
 - [**GCC: Atomically and Non-Atomically Executed Code Blocks**][doc-gcc-atomic]
 
 - [**GCC: Specifying Attributes of Variables**][doc-gcc-alignment]

<br>

## License
This project is proudly licensed under the [MIT license][git-license].

The MIT license was chosen to give you the freedom to use this project in any way you want, while protecting all contributors from legal claims. Good code works, great code works for everyone. If this code has become a part of one of your projects, a link back to us would be highly appreciated. Thanks!

<!-- Links -->

[semver.org]:https://semver.org/

[git-readme]:README.md
[git-license]:LICENSE.md
[git-profile]:https://github.com/nqtronix
[git-issues]:https://github.com/nqtronix/fifofast/issues
[git-download]:https://github.com/nqtronix/fifofast/archive/master.zip

[git-repo-unittrace]:https://github.com/nqtronix/unittrace/
[git-repo-git-template]:https://github.com/nqtronix/git-template/

[doc-gcc-atomic]:https://www.nongnu.org/avr-libc/user-manual/group__util__atomic.html
[doc-gcc-alignment]:https://gcc.gnu.org/onlinedocs/gcc-5.4.0/gcc/Variable-Attributes.html
[article-2n-buffer]:https://www.mikrocontroller.net/articles/FIFO#2n-Ringpuffer_-_die_schnellste_L.C3.B6sung
[article-2n-buffer-eng]:https://translate.google.com/translate?sl=de&tl=en&js=y&prev=_t&hl=de&ie=UTF-8&u=https%3A%2F%2Fwww.mikrocontroller.net%2Farticles%2FFIFO%232n-Ringpuffer_-_die_schnellste_L.C3.B6sung&edit-text=&act=url

[wiki-pipelining]:https://en.wikipedia.org/wiki/Instruction_pipelining
[wiki-circular-buffer]:https://en.wikipedia.org/wiki/Circular_buffer

[tool-atmel-studio-7-0]:https://www.microchip.com/mplab/avr-support/atmel-studio-7
[tool-avr-dragon]:https://www.microchip.com/Developmenttools/ProductDetails/ATAVRDRAGON
