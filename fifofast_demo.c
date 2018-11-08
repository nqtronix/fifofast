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

#include "fifofast.h"
#include "unittrace/unittrace.h"

// all declarations CAN be moved into a separate header file just like any other declaration

// declare a fifo with 4 elements of type 'uint8_t' with the name 'fifo_uint8'
_fff_declare(uint8_t __attribute__((aligned(4))), fifo_uint8, 4);

// declare a fifo with 8 elements (6 elements is not possible, so it is automatically bumped to 8)
// of type 'int_16' with the name 'fifo_uint16'
_fff_declare(int16_t, fifo_int16, 6);

// fifofast also supports more complicated data such as frames used for serial data transmission.
// It is often useful to access the data not only in binary format ('raw'), but also as a struct
// ('header'). In this case the header contains a variable >1 byte, which needs to be stored aligned
// on most architectures. However the union is treaded by default as a 'uint8_t' array, so no
// alignment is enforced. To do this manually, GCC supports the attribute 'aligned(n)'. See
// documentation of GCC for more details.
typedef struct  
{
	uint16_t control;
	uint8_t length;
	uint8_t id;
	uint8_t data[];
} header_t;
typedef union __attribute__((aligned(4), packed))
{
	uint8_t raw[32];
	header_t header;
} frame_u;

// declare a fifo to store 4 elements of the typedef'd union 'frame_u'
_fff_declare(frame_u, fifo_frame, 4);

// NOT the total amount of fifos!
// declare an array (indicated by the suffix _a) of 5 fifos with 16 elements each.

// create 5 fifos within the fifo array
__attribute__ ((unused)) _fff_init_a(fifo_array, 5);

int main(void)
{
	//////////////////////////////////////////////////////////////////////////
	// Test Environment
	//////////////////////////////////////////////////////////////////////////

	// 'volatile' prevents compiler from removing these "unused" variables and thus allows
	// inspection during debugging.

	// for "_fff_add...()" macros only
	__attribute__ ((unused)) volatile uint8_t* dbg_p0 = 0;
	__attribute__ ((unused)) volatile uint8_t* dbg_p1 = 0;
	__attribute__ ((unused)) volatile uint8_t* dbg_p2 = 0;
	__attribute__ ((unused)) volatile uint8_t* dbg_p3 = 0;

	
	// initialize all fifos
    __attribute__ ((unused)) _fff_init(fifo_uint8);
	__attribute__ ((unused)) _fff_init(fifo_int16);
	__attribute__ ((unused)) _fff_init(fifo_frame);
	UT_BREAK();


	// Note: only fifo_uint8 is tested here as uint8 are most easy to work with
	// after each change (or set of changes) call all returning functions
	UT_ASSERT(_fff_mem_depth(fifo_uint8)	== 4);		// constant
	UT_ASSERT(_fff_mem_mask(fifo_uint8)		== 3);		// constant = 0b11
	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 0);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 3);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	UT_BREAK();


	//////////////////////////////////////////////////////////////////////////
	// Test Cases WRITE
	//////////////////////////////////////////////////////////////////////////

	// add 2 values with the fast '_lite' variant (after init we know it is empty)
	// use unusual values (NOT 0, 1, 2 ...) to decrease the likelihood of false positives
	_fff_write_lite(fifo_uint8, 0x73);
	_fff_write_lite(fifo_uint8, 0x74);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 2);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 1);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== 0x73);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== 0x74);
	UT_BREAK();


	// add 3 values to demonstrate overflow safety
	_fff_write(fifo_uint8, 0x75);
	_fff_write(fifo_uint8, 0x76);
	_fff_write(fifo_uint8, 0x77);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 3);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		!= 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== 0x73);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== 0x74);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== 0x75);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== 0x76);
	UT_BREAK();


	//////////////////////////////////////////////////////////////////////////
	// Test Case PEEK
	//////////////////////////////////////////////////////////////////////////

	// modify existing value (index 0 and 2) with _fff_peek()
	_fff_peek(fifo_uint8, 0) = 0x53;	
	_fff_peek(fifo_uint8, 2) = 0x55;
	
	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 3);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		!= 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== 0x53);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== 0x74);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== 0x55);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== 0x76);
	// demonstrate "out of bounds" safety (no over-read/ over-write possible)
	UT_ASSERT(_fff_peek(fifo_uint8, 4)		== 0x53);
	
	UT_BREAK();


	//////////////////////////////////////////////////////////////////////////
	// Test Cases READ
	//////////////////////////////////////////////////////////////////////////

	// read 2 values with the fast '_lite' variant (we know we have written at least to entries)
	UT_ASSERT(_fff_read_lite(fifo_uint8)	== 0x53);
	UT_ASSERT(_fff_read_lite(fifo_uint8)	== 0x74);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 2);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 1);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== 0x55);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== 0x76);
	UT_BREAK();


	// read 3 values t demonstrate overflow safety
	UT_ASSERT(_fff_read(fifo_uint8)			== 0x55);
	UT_ASSERT(_fff_read(fifo_uint8)			== 0x76);
	UT_ASSERT(_fff_read(fifo_uint8)			== 0x00);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 0);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 3);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	UT_BREAK();


	//////////////////////////////////////////////////////////////////////////
	// Test Cases ADD
	//////////////////////////////////////////////////////////////////////////

	// add 2 values with the fast '_lite' variant (after init we know it is empty)
	// use unusual values (NOT 0, 1, 2 ...) to decrease the likelihood of false positives
	dbg_p0 = _fff_add_lite(fifo_uint8);
	dbg_p1 = _fff_add_lite(fifo_uint8);

	if(dbg_p0 != 0) *dbg_p0 = 0x3a;
	if(dbg_p1 != 0) *dbg_p1 = 0x3b;

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 2);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 1);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== 0x3a);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== 0x3b);
	UT_BREAK();


	// add 3 values to demonstrate overflow safety
	dbg_p0 = _fff_add(fifo_uint8);
	dbg_p1 = _fff_add(fifo_uint8);
	dbg_p2 = _fff_add(fifo_uint8);

	if(dbg_p0 != 0) *dbg_p0 = 0x3c;
	if(dbg_p1 != 0) *dbg_p1 = 0x3d;
	if(dbg_p2 != 0) *dbg_p2 = 0x3e;

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 3);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		!= 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== 0x3a);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== 0x3b);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== 0x3c);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== 0x3d);
	UT_BREAK();


	//////////////////////////////////////////////////////////////////////////
	// Test Case RESET
	//////////////////////////////////////////////////////////////////////////

	// only header is reset to its initial values which marks all present data as "empty" (but data
	// is not reset to 0)
	_fff_reset(fifo_uint8);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 0);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 3);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	UT_BREAK();


	//////////////////////////////////////////////////////////////////////////
	// Test Case REMOVE_LITE
	//////////////////////////////////////////////////////////////////////////

	// fill with any data (macros have been proved to work before)
	_fff_write_lite(fifo_uint8, 0x23);
	_fff_write_lite(fifo_uint8, 0x24);
	_fff_write_lite(fifo_uint8, 0x25);
	_fff_write_lite(fifo_uint8, 0x26);
	// fifo is now full
	
	// _remove 2 (Test case: fifo full before macro)
	_fff_remove_lite(fifo_uint8, 2);
	
	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 2);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 1);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== 0x25);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== 0x26);
	UT_BREAK();


	// _remove 1 (Test case: fifo not full before and not empty after macro)
	_fff_remove_lite(fifo_uint8, 1);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 1);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 2);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== 0x26);
	UT_BREAK();
	

	// _remove 1 (Test case: fifo empty after macro)
	_fff_remove_lite(fifo_uint8, 1);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 0);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 3);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	UT_BREAK();


	//////////////////////////////////////////////////////////////////////////
	// Test Case REMOVE
	//////////////////////////////////////////////////////////////////////////

	// fill with any data (macros have been proved to work before)
	_fff_write_lite(fifo_uint8, 0x13);
	_fff_write_lite(fifo_uint8, 0x14);
	_fff_write_lite(fifo_uint8, 0x15);
	_fff_write_lite(fifo_uint8, 0x16);
	// fifo is now full
	
	
	// _remove 0 (Test case: amount <= 0 elements)
	_fff_remove(fifo_uint8, 0);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 3);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		!= 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== 0x13);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== 0x14);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== 0x15);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== 0x16);
	UT_BREAK();


	// _remove 4 (Test case: amount > _fff_mem_level() w/ fifo full)
	_fff_remove(fifo_uint8, 4);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 1);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 2);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== 0x16);
	UT_BREAK();


	// _remove 4 (Test case: amount > _fff_mem_level() w/ fifo not full)
	_fff_remove(fifo_uint8, 4);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 0);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 3);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
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
    while (1) asm volatile ("nop");
}