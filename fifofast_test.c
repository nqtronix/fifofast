/*
 * fifofast_test.c
 *
 * Created: 08.11.2018 08:34:58
 *  Author: Dennis
 */ 

#include "fifofast_test.h"


//////////////////////////////////////////////////////////////////////////
// Test Macros
//////////////////////////////////////////////////////////////////////////

void fifofast_test_macro_initial()
{
	UT_ASSERT(_fff_mem_depth(fifo_uint8)	== 4);		// constant
	UT_ASSERT(_fff_mem_mask(fifo_uint8)		== 3);		// constant = 0b11
	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 0);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 4);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
}

void fifofast_test_macro_write(uint8_t startvalue)
{
	// add 2 values with the fast '_lite' variant (after init we know it is empty)
	// use unusual values (NOT 0, 1, 2 ...) to decrease the likelihood of false positives
	_fff_write_lite(fifo_uint8, startvalue+0);
	_fff_write_lite(fifo_uint8, startvalue+1);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 2);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 2);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+0);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+1);
	UT_BREAK();


	// add 3 values to demonstrate overflow safety
	_fff_write(fifo_uint8, startvalue+2);
	_fff_write(fifo_uint8, startvalue+3);
	_fff_write(fifo_uint8, startvalue+4);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 4);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		!= 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+0);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+1);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== startvalue+2);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== startvalue+3);
	
	_fff_reset(fifo_uint8);
	
	// Test reset once only
	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 0);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 4);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
}

void fifofast_test_macro_peek(uint8_t startvalue)
{
	_fff_write_lite(fifo_uint8, startvalue+0);
	_fff_write_lite(fifo_uint8, startvalue+1);
	_fff_write_lite(fifo_uint8, startvalue+2);
	
	// modify existing value (index 0 and 2) with _fff_peek()
	_fff_peek(fifo_uint8, 0) = startvalue+3;
	_fff_peek(fifo_uint8, 2) = startvalue+4;
	
	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 3);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 1);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+3);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+1);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== startvalue+4);
	
	// demonstrate "out of bounds" safety (no over-read/ over-write possible)
	UT_ASSERT(_fff_peek(fifo_uint8, 4)		== startvalue+3);
	
	_fff_reset(fifo_uint8);
}

void fifofast_test_macro_read(uint8_t startvalue)
{
	_fff_write_lite(fifo_uint8, startvalue+0);
	_fff_write_lite(fifo_uint8, startvalue+1);
	_fff_write_lite(fifo_uint8, startvalue+2);
	
	// read 2 values with the fast '_lite' variant (we know we have more than two entries)
	UT_ASSERT(_fff_read_lite(fifo_uint8)	== startvalue+0);
	UT_ASSERT(_fff_read_lite(fifo_uint8)	== startvalue+1);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 1);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 3);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+2);

	// read 2 values to demonstrate overflow safety
	UT_ASSERT(_fff_read(fifo_uint8)			== startvalue+2);
	UT_ASSERT(_fff_read(fifo_uint8)			== 0x00);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 0);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 4);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	_fff_reset(fifo_uint8);
}

void fifofast_test_macro_add(uint8_t startvalue)
{
	__attribute__ ((unused)) volatile uint8_t* dbg_p0 = 0;
	__attribute__ ((unused)) volatile uint8_t* dbg_p1 = 0;
	__attribute__ ((unused)) volatile uint8_t* dbg_p2 = 0;
	
	// add 2 values with the fast '_lite' variant (after init we know it is empty)
	dbg_p0 = _fff_add_lite(fifo_uint8);
	dbg_p1 = _fff_add_lite(fifo_uint8);

	UT_ASSERT(dbg_p0 != 0);
	UT_ASSERT(dbg_p1 != 0);

	if(dbg_p0 != 0) *dbg_p0 = startvalue+0;
	if(dbg_p1 != 0) *dbg_p1 = startvalue+1;

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 2);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 2);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
		
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+0);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+1);


	// add 3 values to demonstrate overflow safety
	dbg_p0 = _fff_add(fifo_uint8);
	dbg_p1 = _fff_add(fifo_uint8);
	dbg_p2 = _fff_add(fifo_uint8);

	UT_ASSERT(dbg_p0 != 0);
	UT_ASSERT(dbg_p1 != 0);
	UT_ASSERT(dbg_p2 == 0);		// the fifo is full, so a null pointer must be returned

	if(dbg_p0 != 0) *dbg_p0 = startvalue+2;
	if(dbg_p1 != 0) *dbg_p1 = startvalue+3;
	if(dbg_p2 != 0) *dbg_p2 = startvalue+4;

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 4);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		!= 0);
		
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+0);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+1);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== startvalue+2);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== startvalue+3);
	
	_fff_reset(fifo_uint8);
}

void fifofast_test_macro_remove_lite(uint8_t startvalue)
{
	// fill with any data (macros have been proved to work before)
	_fff_write_lite(fifo_uint8, startvalue+0);
	_fff_write_lite(fifo_uint8, startvalue+1);
	_fff_write_lite(fifo_uint8, startvalue+2);
	_fff_write_lite(fifo_uint8, startvalue+3);
	// fifo is now full

		
	// _remove 2 (Test case: fifo full before macro)
	_fff_remove_lite(fifo_uint8, 2);
		
	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 2);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 2);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
		
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+2);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+3);


	// _remove 1 (Test case: fifo not full before and not empty after macro)
	_fff_remove_lite(fifo_uint8, 1);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 1);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 3);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
		
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+3);
	
	
	// _remove 1 (Test case: fifo empty after macro)
	_fff_remove_lite(fifo_uint8, 1);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 0);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 4);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	_fff_reset(fifo_uint8);
}

void fifofast_test_macro_remove(uint8_t startvalue)
{
	// fill with any data (macros have been proved to work before)
	_fff_write_lite(fifo_uint8, startvalue+0);
	_fff_write_lite(fifo_uint8, startvalue+1);
	_fff_write_lite(fifo_uint8, startvalue+2);
	_fff_write_lite(fifo_uint8, startvalue+3);
	// fifo is now full
	
	
	// _remove 0 (Test case: amount == 0 elements)
	_fff_remove(fifo_uint8, 0);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 4);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		!= 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+0);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+1);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== startvalue+2);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== startvalue+3);


	// _remove 4 (Test case: amount == _fff_mem_level() w/ fifo full)
	_fff_remove(fifo_uint8, 4);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 0);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 4);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
}

void fifofast_test_macro_rebase(uint8_t startvalue)
{
	//////////////////////////////////////////////////////////////////////////
	// Test 1: no rebase needed
	
	// fill with any data (macros have been proved to work before)
	_fff_write_lite(fifo_uint8, startvalue+0);
	_fff_write_lite(fifo_uint8, startvalue+1);
	_fff_write_lite(fifo_uint8, startvalue+2);
	_fff_write_lite(fifo_uint8, startvalue+3);
	
	_fff_rebase(fifo_uint8);
	
	// Confirm it is exactly like before:
	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 4);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		!= 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+0);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+1);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== startvalue+2);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== startvalue+3);
	
	// confirm that the element 0 is at array index 0
	UT_ASSERT(&_fff_peek(fifo_uint8, 0)		== &fifo_uint8.data[0]);
	
	_fff_reset(fifo_uint8);
	
	
	//////////////////////////////////////////////////////////////////////////
	// Test 3: move 2 elements
		
	// fill with any data (macros have been proved to work before)
	_fff_write_lite(fifo_uint8, startvalue+0);
	_fff_write_lite(fifo_uint8, startvalue+1);
	_fff_write_lite(fifo_uint8, startvalue+2);
	_fff_write_lite(fifo_uint8, startvalue+3);
		
	_fff_read_lite(fifo_uint8);
	_fff_write_lite(fifo_uint8, startvalue+4);
		
	_fff_rebase(fifo_uint8);
		
	// Confirm it is exactly like before:
	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 4);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		!= 0);
		
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+1);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+2);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== startvalue+3);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== startvalue+4);
		
	// confirm that the element 0 is at array index 0
	UT_ASSERT(&_fff_peek(fifo_uint8, 0)		== &fifo_uint8.data[0]);
		
	_fff_reset(fifo_uint8);
	
	
	//////////////////////////////////////////////////////////////////////////
	// Test 3: move 2 elements
	
	// fill with any data (macros have been proved to work before)
	_fff_write_lite(fifo_uint8, startvalue+0);
	_fff_write_lite(fifo_uint8, startvalue+1);
	_fff_write_lite(fifo_uint8, startvalue+2);
	_fff_write_lite(fifo_uint8, startvalue+3);
	
	_fff_read_lite(fifo_uint8);
	_fff_read_lite(fifo_uint8);
	_fff_write_lite(fifo_uint8, startvalue+4);
	_fff_write_lite(fifo_uint8, startvalue+5);
	
	_fff_rebase(fifo_uint8);
	
	// Confirm it is exactly like before:
	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 4);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		!= 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+2);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+3);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== startvalue+4);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== startvalue+5);
	
	// confirm that the element 0 is at array index 0
	UT_ASSERT(&_fff_peek(fifo_uint8, 0)		== &fifo_uint8.data[0]);
	
	_fff_reset(fifo_uint8);
	
	
	//////////////////////////////////////////////////////////////////////////
	// Test 2: move 3 elements
	
	// fill with any data (macros have been proved to work before)
	_fff_write_lite(fifo_uint8, startvalue+0);
	_fff_write_lite(fifo_uint8, startvalue+1);
	_fff_write_lite(fifo_uint8, startvalue+2);
	_fff_write_lite(fifo_uint8, startvalue+3);
	
	_fff_read_lite(fifo_uint8);
	_fff_read_lite(fifo_uint8);
	_fff_read_lite(fifo_uint8);
	_fff_write_lite(fifo_uint8, startvalue+4);
	_fff_write_lite(fifo_uint8, startvalue+5);
	_fff_write_lite(fifo_uint8, startvalue+6);
	
	_fff_rebase(fifo_uint8);
	
	// Confirm it is exactly like before:
	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 4);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		!= 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+3);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+4);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== startvalue+5);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== startvalue+6);
	
	// confirm that the element 0 is at array index 0
	UT_ASSERT(&_fff_peek(fifo_uint8, 0)		== &fifo_uint8.data[0]);
	
	_fff_reset(fifo_uint8);
}

void fifofast_test_macro_write_multiple(uint8_t startvalue) {
	uint8_t multidata[4] = {startvalue + 3, startvalue + 4, startvalue + 5, startvalue + 6};

	// initialize fifo with some data
	_fff_write_lite(fifo_uint8, startvalue+0);
	_fff_write_lite(fifo_uint8, startvalue+1);
	_fff_write_lite(fifo_uint8, startvalue+2);
	_fff_remove_lite(fifo_uint8, 2);

	// write test data, case: all data fits
	_fff_write_multiple(fifo_uint8, multidata, 3);

	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+2);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+3);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== startvalue+4);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== startvalue+5);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 4);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 1);
	
	_fff_reset(fifo_uint8);
	
	// re-initialize fifo with some data
	_fff_write_lite(fifo_uint8, startvalue+7);
	_fff_write_multiple(fifo_uint8, multidata, 3);
	
	// write test data, case: NOT all data fits (overflow is discarded)
	_fff_write_multiple(fifo_uint8, multidata, 4);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+7);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+3);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== startvalue+4);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== startvalue+5);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 4);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 1);
}

//////////////////////////////////////////////////////////////////////////
// Test Functions
//////////////////////////////////////////////////////////////////////////

void fifofast_test_func_initial(fff_proto_t* fifo)
{
	//UT_ASSERT(fff_mem_depth(fifo)			== 4);		// constant
	UT_ASSERT(fff_mem_mask(fifo)			== 3);		// constant = 0b11
	UT_ASSERT(fff_mem_level(fifo)			== 0);
	UT_ASSERT(fff_mem_free(fifo)			== 4);
	UT_ASSERT(fff_is_empty(fifo)			!= 0);
	UT_ASSERT(fff_is_full(fifo)				== 0);
}

void fifofast_test_func_write(fff_proto_t* fifo, uint8_t startvalue)
{
	// add 2 values with the fast '_lite' variant (after init we know it is empty)
	// use unusual values (NOT 0, 1, 2 ...) to decrease the likelihood of false positives
	uint8_t tmp0 = startvalue+0;
	uint8_t tmp1 = startvalue+1;
	fff_write_lite(fifo, &tmp0);
	fff_write_lite(fifo, &tmp1);

	UT_ASSERT(fff_mem_level(fifo)			== 2);
	UT_ASSERT(fff_mem_free(fifo)			== 2);
	UT_ASSERT(fff_is_empty(fifo)			== 0);
	UT_ASSERT(fff_is_full(fifo)				== 0);
	
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 0)		== startvalue+0);
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 1)		== startvalue+1);


	// add 3 values to demonstrate overflow safety
	uint8_t tmp2 = startvalue+2;
	uint8_t tmp3 = startvalue+3;
	uint8_t tmp4 = startvalue+4;
	fff_write(fifo, &tmp2);
	fff_write(fifo, &tmp3);
	fff_write(fifo, &tmp4);

	UT_ASSERT(fff_mem_level(fifo)			== 4);
	UT_ASSERT(fff_mem_free(fifo)			== 0);
	UT_ASSERT(fff_is_empty(fifo)			== 0);
	UT_ASSERT(fff_is_full(fifo)				!= 0);
	
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 0)		== startvalue+0);
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 1)		== startvalue+1);
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 2)		== startvalue+2);
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 3)		== startvalue+3);
	
	fff_reset(fifo);
	
	// Test reset once only
	UT_ASSERT(fff_mem_level(fifo)			== 0);
	UT_ASSERT(fff_mem_free(fifo)			== 4);
	UT_ASSERT(fff_is_empty(fifo)			!= 0);
	UT_ASSERT(fff_is_full(fifo)				== 0);
}

void fifofast_test_func_peek(fff_proto_t* fifo, uint8_t startvalue)
{
	uint8_t tmp0 = startvalue+0;
	uint8_t tmp1 = startvalue+1;
	uint8_t tmp2 = startvalue+2;
	fff_write_lite(fifo, &tmp0);
	fff_write_lite(fifo, &tmp1);
	fff_write_lite(fifo, &tmp2);
	
	// modify existing value (index 0 and 2) with _fff_peek()
	uint8_t tmp3 = startvalue+3;
	uint8_t tmp4 = startvalue+4;
	fff_peek_write(fifo, 0, &tmp3);
	fff_peek_write(fifo, 2, &tmp4);
	
	UT_ASSERT(fff_mem_level(fifo)			== 3);
	UT_ASSERT(fff_mem_free(fifo)			== 1);
	UT_ASSERT(fff_is_empty(fifo)			== 0);
	UT_ASSERT(fff_is_full(fifo)				== 0);
	
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 0)		== startvalue+3);
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 1)		== startvalue+1);
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 2)		== startvalue+4);
	
	// demonstrate "out of bounds" safety (no over-read/ over-write possible)
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 4)		== startvalue+3);
	
	fff_reset(fifo);
}

void fifofast_test_func_remove_lite(fff_proto_t* fifo, uint8_t startvalue)
{
	// fill with any data (macros have been proved to work before)
	uint8_t tmp0 = startvalue+0;
	uint8_t tmp1 = startvalue+1;
	uint8_t tmp2 = startvalue+2;
	uint8_t tmp3 = startvalue+3;
	fff_write_lite(fifo, &tmp0);
	fff_write_lite(fifo, &tmp1);
	fff_write_lite(fifo, &tmp2);
	fff_write_lite(fifo, &tmp3);
	// fifo is now full

	
	// _remove 2 (Test case: fifo full before macro)
	fff_remove_lite(fifo, 2);
	
	UT_ASSERT(fff_mem_level(fifo)			== 2);
	UT_ASSERT(fff_mem_free(fifo)			== 2);
	UT_ASSERT(fff_is_empty(fifo)			== 0);
	UT_ASSERT(fff_is_full(fifo)				== 0);
	
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 0)		== startvalue+2);
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 1)		== startvalue+3);


	// _remove 1 (Test case: fifo not full before and not empty after macro)
	fff_remove_lite(fifo, 1);

	UT_ASSERT(fff_mem_level(fifo)			== 1);
	UT_ASSERT(fff_mem_free(fifo)			== 3);
	UT_ASSERT(fff_is_empty(fifo)			== 0);
	UT_ASSERT(fff_is_full(fifo)				== 0);
	
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 0)		== startvalue+3);
	
	
	// _remove 1 (Test case: fifo empty after macro)
	fff_remove_lite(fifo, 1);

	UT_ASSERT(fff_mem_level(fifo)			== 0);
	UT_ASSERT(fff_mem_free(fifo)			== 4);
	UT_ASSERT(fff_is_empty(fifo)			!= 0);
	UT_ASSERT(fff_is_full(fifo)				== 0);
	
	fff_reset(fifo);
}

void fifofast_test_func_remove(fff_proto_t* fifo, uint8_t startvalue)
{
	// fill with any data (macros have been proved to work before)
	uint8_t tmp0 = startvalue+0;
	uint8_t tmp1 = startvalue+1;
	uint8_t tmp2 = startvalue+2;
	uint8_t tmp3 = startvalue+3;
	fff_write_lite(fifo, &tmp0);
	fff_write_lite(fifo, &tmp1);
	fff_write_lite(fifo, &tmp2);
	fff_write_lite(fifo, &tmp3);
	// fifo is now full
	
	
	// _remove 0 (Test case: amount == 0 elements)
	fff_remove(fifo, 0);

	UT_ASSERT(fff_mem_level(fifo)			== 4);
	UT_ASSERT(fff_mem_free(fifo)			== 0);
	UT_ASSERT(fff_is_empty(fifo)			== 0);
	UT_ASSERT(fff_is_full(fifo)				!= 0);
	
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 0)		== startvalue+0);
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 1)		== startvalue+1);
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 2)		== startvalue+2);
	UT_ASSERT(*(uint8_t*)fff_peek_read(fifo, 3)		== startvalue+3);


	// _remove 4 (Test case: amount == _fff_mem_level() w/ fifo full)
	fff_remove(fifo, 4);

	UT_ASSERT(fff_mem_level(fifo)			== 0);
	UT_ASSERT(fff_mem_free(fifo)			== 4);
	UT_ASSERT(fff_is_empty(fifo)			!= 0);
	UT_ASSERT(fff_is_full(fifo)				== 0);

}
