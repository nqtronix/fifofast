<h1 align="center" style="font-weight: bold; margin-top: 20px; margin-bottom: 20px;">unittrace</h4>

<h4 align="center"> A simple testing and debugging tool for MCUs inspired by <a href="http://www.jera.com/techinfo/jtns/jtn002.html" target="_blank">MinUnit</a>.</h4>
 
<p align="center">
	<a href="#changelog"><img src="https://img.shields.io/github/release-pre/nqtronix/unittrace.svg" alt="release: NA"></a>
    <a href="#about"><img src="https://img.shields.io/badge/language-C_(GCC_5.4.0)-blue.svg" alt="language: C GCC (5.4.0)"></a>
    <a href="#about"><img src="https://img.shields.io/badge/platform-MCUs, AVR8-blue.svg" alt="platform: MCUs, AVR8"></a>
	<a href="#about"><img src="https://img.shields.io/badge/status-maintained-green.svg" alt="status: maintained"></a>
	<a href="https://github.com/nqtronix/unittrace/issues"><img src="https://img.shields.io/github/issues/nqtronix/unittrace.svg" alt="issues: NA"></a>
	<a href="#license"><img src="https://img.shields.io/github/license/nqtronix/unittrace.svg" alt="license: NA"></a>
</p>

<p align="center">
  <a href="#getting-started">Getting Started</a> •
  <a href="#documentation">Documentation</a> •
  <a href="#under-the-hood">Under the Hood</a> •
  <a href="#support">Need Help?</a> •
  <a href="#about">About</a> •
  <a href="#credits-and-references">Credits</a>
</p>



## Introduction
Testing is an important part of writing code, especially if the code is meant to be re-used for other projects. On small MCUs testing can be quite tricky as the memory is limited and errors can't be easily reported with `printf()` or similar.

**unittrace** provides basic testing functionality and is designed with the limits of MCUs in mind. It runs directly on the hardware and thus can catch errors other test software can't. Fetch the results through a debugger or with a function.

<br>

## Key Features
 
 - **lightweight:** absolute minimum Flash & RAM usage
 - **runtime test:** catches both software and hardware errors
 - **static memory**: no malloc overhead
 
<br>

## Limitations

 - on AVR8 MCUs the maximum usable flash memory is 128KB or 64K words, as the address must fit into the 16bit pointer. On 32bit MCUs pointers are always 32bit and therefore there is no upper limit.
 
<br>

## Usage Example
**unittrace** is ridiculously simple to use. You can place the `ut_assert` functions wherever you want:

```c
int main(void)
{
	// using inline functions
	ut_assert(5 == 5);	// passes
	ut_assert(5 == 7);	// fails and code address gets stored in array
	
	// using macros (equivalently behavior to functions)
	UT_ASSERT(5 == 5);	// passes
	UT_ASSERT(5 == 7);	// fails and code address gets stored in array
    
    // set a breakpoint
	UT_BREAK();
    while (1);
}
```
[](#interpreting-the-data)

<br>

## Getting Started
This section is written especially for everyone who is **not familiar** with the used tools. If you run into problems, please [ask for clarification](#get-help).<br>

### Step 1: Software and Tools
 - [**Atmel Studio 7.0**][tool-atmel-studio-7-0]** (Build 1931) [free**]<br>
   The installer contains all tools you need to open, edit, compile, simulate and flash this code. If you favor another development tool, feel free to use it instead. (But please understand that I can not provide any support).
 - **An AVR8 ISP/JTAG programmer [optional]**<br>
   To program AVR8 MCUs I use the [AVR Dragon][tool-avr-dragon]. It can be also used as a debugger and is available within Atmel Studio by default.

### Step 2: Download unittrace
 - Clone this repository or hit [Download][git-download] and extract the .zip file.

### Step 3: Browse the project
 - **Open the project in Atmel Studio:**<br>
   Either double click `unittrace.atsln` or open Atmel Studio and select "File -> Open -> Project/Solution..."

 - **Open any file of your interest:**<br>
   Select the file in the top right window "Solution Explorer". If the window is not visible, open it by pressing `CTRL + ALT + L` or selecting "View -> Solution Explorer" from the menu.

### Step 4: Run the demo
 - **Compile the demo code:**<br>
   Press `F7` or select "Build -> Build Solution" from the menu
 
 - **Run the demo code in the simulator:**<br>
   Press `CTRL + F5` or select "Debug -> Start Debugging and Break" from the menu
 
 - **Place breakpoints:**<br>
   Left-Click on the light grey area left of the code to place or remove a breakpoint. Select lines with the comment "easy breakpoint".
   
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

### Interpreting the Data

The results of the test can be accessed at any time, even if the test is not fully complete. The global variable `unittrace_count` contains the number of failed asserts, the array `unittrace_array` contains pointers to the location of the failed instruction in the flash memory. Both variables can be read at runtime or in the debugger with right-click -> watch.

However due to limitations of the build-in debugger there is no direct way to jump to the corresponding line of code. Instead you have to:

1. Start the debug session
2. Right-click on the source code -> open disassembly
3. Scroll and search the correct line
4. Right-click on the line -> Go to source code

This section will be updated, if I figure out a better way.

<br>

### API

To keep the documentation up-to-date with the least hassle, all configuration options, functions and their arguments are explained in a comment right in front of the declaration. See `unittrace.h` for more information. This section will be updated as soon as this project hits version 1.0.0.

<br>

## Under the Hood
I possibly should note that I've never used any "professional" unit testing library, so this may be unlike the unit testing you are used to. The code was written from scratch and inspired by [MinUnit][tool-minunit].

> Of course, if you have access to a full-featured testing framework like JUnit, by all means use it. But if you don't, you can still use a simple framework like MinUnit, or whip up your own in a few hours. There's no excuse for not unit testing.

<br>

### Development History

I didn't want to use `printf` or any UART equivalent, but the debugger instead. This is not as easy as it seems. Here are the ideas I've tried:

<br>

#### Idea 1
Create a global `static void*` pointer and one for each assert macro. All shall be initialized with `NULL`. Whenever the assert condition is false, the local pointer gets the value from the global one and the global is updated with a reference to the local one. After multiple failed asserted this generates a linked list.

Problems:
1. The Atmel Studio 7 Debugger can't jump to the location of a variable, so one can not tell which assert failed.
2. After multiple de-referencing a pointer like this multiple times the 'watch-window' gets progressively slower.

<br>

#### Idea 2
Create an array of structs in SRAM, each containing:
- pointer to a String in FLASH containing the filename and path
- uint16_t with the line number of the failed assert
New struct shall be added whenever an assert fails. Filename and line number can be accessed with the macros `__FILE__` and `__LINE__`, respectively.

Problem:<br>
To show strings in the watch window, the suffix `,s` must be added manually. If the suffix is added to a struct array like this, ALL contents are interpreted as string.

<br>

#### Idea 3
Instead of saving filename and line number individually, put them in one string into flash. Now whenever a assert fails, store a pointer to the corresponding string in the array.

Problem:<br>
The debugger can't access the data of a FLASH pointer address, if the address is stored in SRAM (or at least I couldn't figure out how). In short: you can't mix SRAM and FLASH pointer as one expression in the debugger.

<br>

#### Idea 4
Instead of generating a list of human readable strings, store the physical location of the assert macro in a SRAM array. The location in the program memory is a 16 bit pointer, so the amount of SRAM required is pretty low. Also this solution requires almost no flash memory, except the few instructions to per assert.

Downside:<br>
Although it is easy to get the value of the pointer in the array, you can not jump to the corresponding location in the C source code. See section [Interpreting the Data](#interpreting-the-data) for more details.

As of now idea 4 is the only one that works, so it is the implemented solution. Please [tell me](#contribute) if you know a more straightforward solution.

<br>

## Support

### Get Help

**Something doesn't work as expected?** No worries! Just open up a new issue in the [GitHub issue tracker][git-issues]. Please provide all information to reproduce your problem. If you don't have a GitHub account (and can't be bothered to create one,) you can [contact](#contact) me directly.

<br>

### Contribute

**Spotted an error?** [Open an issue][git-issues] or submit a pull request.

There is no CONTRIBUTING.md yet, sorry. Contributions will inherit the [license](#license) of this project. If you have any questions, just ask.

<br>

## About
### Status
**This project is currently classified as** <a href="https://github.com/nqtronix/git-template/blob/master/badges.md#project-status"><img src="https://img.shields.io/badge/status-maintained-green.svg" alt="status: maintained"></a><br>
_The developers intend to keep the code in working condition by updating dependencies, fixing bugs and solving issues._

As my testing needs increase I will likely add the functionality I need.

<br>

### Changelog
This project uses [**Semantic Versioning 2.0.0**][semver.org]. During initial development (0.x.x versions) any _major_ increase is substituted with a _minor_ increase (0.1.0->0.2.0 instead of 0.1.0->1.0.0).

The message of each commit contains detailed information about the changes made. The list below is a summary about all significant improvements.

 - **0.1.0 (latest)** <br>
   - initial release

<br>

### Contact

If you haven't done so already, please check out [Get Help](#get-help) for the fastest possible help on your issue. Alternatively you can find my public email address on my [profile][git-profile].

<br>

## Credits and References

### Projects Used
- [**MinUnit**][tool-minunit] - _a minimal unit testing framework for C_<br>
  Great inspiration, thanks!

- [**git-template**][git-repo-git-template] - _A simple and clean git repository template._<br>

<br>

### Related Projects

 - none (yet)
 
Want yours to be listed here, too? Create a merge request or [**get in touch**](#get-help).

<br>

### Additional Resources

- [**Catch2**][git-repo-catch2] by [@catchorg][at-catchorg]<br>
  A header-only test framework. Found it by chance and it seems like a great choice for PC applications.


<br>

## License
This project is proudly licensed under the [MIT license][git-license].

The MIT license was chosen to give you the freedom to use this project in any way you want, while protecting all contributors from legal claims. Good code works, great code works for everyone. If this code has become a part of one of your projects, a link back to us would be highly appreciated. Thanks!

<!-- Links -->

[git-readme]:README.md
[git-license]:LICENSE.md
[git-profile]:https://github.com/nqtronix
[git-issues]:https://github.com/nqtronix/unittrace/issues
[git-download]:https://github.com/nqtronix/unittrace/archive/master.zip

[git-repo-git-template]:https://github.com/nqtronix/git-template

[semver.org]:semver.org

[tool-atmel-studio-7-0]:https://www.microchip.com/mplab/avr-support/atmel-studio-7
[tool-avr-dragon]:https://www.microchip.com/Developmenttools/ProductDetails/ATAVRDRAGON
[tool-minunit]:http://www.jera.com/techinfo/jtns/jtn002.html

[at-catchorg]:https://github.com/catchorg
[git-repo-catch2]:https://github.com/catchorg/Catch2
