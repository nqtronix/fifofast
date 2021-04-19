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

#include "fifofast_demo.h"
#include "fifofast_test.h"


int main(void)
{
	//////////////////////////////////////////////////////////////////////////
	// Test Environment
	//////////////////////////////////////////////////////////////////////////
	
	// initialize all fifos
    __attribute__ ((unused)) _fff_init(fifo_uint8);
	__attribute__ ((unused)) _fff_init_p(fifo_uint8p);
	__attribute__ ((unused)) _fff_init(fifo_int16);
	__attribute__ ((unused)) _fff_init(fifo_frame);
	__attribute__ ((unused)) _fff_init_a(fifo_array, 5);
	UT_BREAK();

	// Note: only fifo_uint8 is tested here as uint8 are most easy to work with
	// after each change (or set of changes) call all returning functions
	//
	// The compiler MAY optimize these test strongly, and may not generate all assembler code.
	// To debug the tests, please disable optimization.
	fifofast_test_macro_initial();
	fifofast_test_macro_write(0x10);
	fifofast_test_macro_peek(0x20);
	fifofast_test_macro_read(0x30);
	fifofast_test_macro_add(0x40);
	fifofast_test_macro_remove_lite(0x50);
	fifofast_test_macro_remove(0x60);
	fifofast_test_macro_rebase(0x70);
	fifofast_test_macro_multiple_write(0x80);
	
	fifofast_test_func_initial((fff_proto_t*)&fifo_uint8p);
	fifofast_test_func_write((fff_proto_t*)&fifo_uint8p, 0x80);
	fifofast_test_func_peek((fff_proto_t*)&fifo_uint8p, 0x90);
	fifofast_test_func_remove_lite((fff_proto_t*)&fifo_uint8p, 0xa0);
	fifofast_test_func_remove((fff_proto_t*)&fifo_uint8p, 0xb0);
	
	UT_BREAK();


	//////////////////////////////////////////////////////////////////////////
	// Demonstrate usage of fifo array
	//////////////////////////////////////////////////////////////////////////

	// macros have been proved to work, so to see the result just look into the SRAM directly
	// These two loops copy the printable chars into the sram, each fifo gets a section of 16
	// consecutive characters.
	uint8_t data = ' ';
	for (uint8_t fifo_nr = 0; fifo_nr < _sizeof_array(fifo_array); fifo_nr++)
	{
		for (uint8_t idx = 0; idx < _fff_mem_depth(fifo_array[0]); idx++)
		{
			_fff_write(fifo_array[fifo_nr], data);
			data += 1;
		}
	}


	// End simulation
	UT_BREAK();
    while (1);
}