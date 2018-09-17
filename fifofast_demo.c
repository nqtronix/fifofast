/*
 * fifofast_demo.c
 *
 * Created: 17.09.2018 19:21:17
 *  Author: Dennis aka nqtronix (github.com/nqtronix)
 *
 * Description:
 * This file contains some demo code to try out the provided macros. Simply run it in the simulator
 * and inspect the step-by-step changes with the debugger.
 */ 

#include <stdio.h>
#include "fifofast.h"

// all declarations CAN be moved into a seperate header file just like any oother declaration

// declare a fifo with 4 elements of type 'uint8_t' with the name 'fifo_uint8'
_fff_declare(uint8_t, fifo_uint8, 4);

// declare a fifo with 8 elements (6 elements is not possible, so it is automatically bumped to 8)
// of type 'int_16' with the name 'fifo_uint16'
_fff_declare(int16_t, fifo_int16, 6);

// declare a fifo to store 4 elements of the typedef'd struct 'frame_t'
typedef struct  
{
	uint16_t id;
	uint8_t length;
	uint8_t data[32];
} frame_t;
_fff_declare(frame_t, fifo_frame, 4);


int main(void)
{
	// initialize all fifos
    _fff_init(fifo_uint8);
	_fff_init(fifo_int16);
	_fff_init(fifo_frame);

	// 'volatile' allows inspection during debugging
	volatile uint8_t dbg0 = 0;
	volatile uint8_t dbg1 = 0;
	volatile uint8_t dbg2 = 0;
	volatile uint8_t dbg3 = 0;

	// add 3 values with the fast '_lite' variant (after init we know it is empty)
	_fff_write_lite(fifo_uint8, 0x73);
	_fff_write_lite(fifo_uint8, 0x74);
	_fff_write_lite(fifo_uint8, 0x75);
	dbg0 = _fff_peek(fifo_uint8, 0);	// = 0x73
	dbg1 = _fff_peek(fifo_uint8, 1);	// = 0x74
	dbg2 = _fff_peek(fifo_uint8, 2);	// = 0x75
	dbg3 = _fff_peek(fifo_uint8, 3);	// = "empty" = any value

	// add 2 values to demonstrate overflow safety
	_fff_write(fifo_uint8, 0x76);
	_fff_write(fifo_uint8, 0x77);
	dbg0 = _fff_peek(fifo_uint8, 0);	// = 0x73
	dbg1 = _fff_peek(fifo_uint8, 1);	// = 0x74
	dbg2 = _fff_peek(fifo_uint8, 2);	// = 0x75
	dbg3 = _fff_peek(fifo_uint8, 3);	// = 0x76

	

	// End simulation
    while (1);
}

