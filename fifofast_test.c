/*
 * fifofast_test.c
 *
 * Created: 08.11.2018 08:34:58
 *  Author: Dennis
 */ 

#include "fifofast_demo.h"
#include "fifofast_test.h"

void fifofast_test_initial()
{
	UT_ASSERT(_fff_mem_depth(fifo_uint8)	== 4);		// constant
	UT_ASSERT(_fff_mem_mask(fifo_uint8)		== 3);		// constant = 0b11
	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 0);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 3);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
}

void fifofast_test_write(uint8_t startvalue)
{
	// add 2 values with the fast '_lite' variant (after init we know it is empty)
	// use unusual values (NOT 0, 1, 2 ...) to decrease the likelihood of false positives
	_fff_write_lite(fifo_uint8, startvalue+0);
	_fff_write_lite(fifo_uint8, startvalue+1);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 2);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 1);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+0);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+1);
	UT_BREAK();


	// add 3 values to demonstrate overflow safety
	_fff_write(fifo_uint8, startvalue+2);
	_fff_write(fifo_uint8, startvalue+3);
	_fff_write(fifo_uint8, startvalue+4);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 3);
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
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 3);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
}

void fifofast_test_peek(uint8_t startvalue)
{
	_fff_write_lite(fifo_uint8, startvalue+0);
	_fff_write_lite(fifo_uint8, startvalue+1);
	_fff_write_lite(fifo_uint8, startvalue+2);
	
	// modify existing value (index 0 and 2) with _fff_peek()
	_fff_peek(fifo_uint8, 0) = startvalue+3;
	_fff_peek(fifo_uint8, 2) = startvalue+4;
	
	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 3);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+3);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+1);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== startvalue+4);
	
	// demonstrate "out of bounds" safety (no over-read/ over-write possible)
	UT_ASSERT(_fff_peek(fifo_uint8, 4)		== startvalue+3);
	
	_fff_reset(fifo_uint8);
}

void fifofast_test_read(uint8_t startvalue)
{
	_fff_write_lite(fifo_uint8, startvalue+0);
	_fff_write_lite(fifo_uint8, startvalue+1);
	_fff_write_lite(fifo_uint8, startvalue+2);
	
	// read 2 values with the fast '_lite' variant (we know we have more than two entries)
	UT_ASSERT(_fff_read_lite(fifo_uint8)	== startvalue+0);
	UT_ASSERT(_fff_read_lite(fifo_uint8)	== startvalue+1);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 1);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 2);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+2);

	// read 2 values to demonstrate overflow safety
	UT_ASSERT(_fff_read(fifo_uint8)			== startvalue+2);
	UT_ASSERT(_fff_read(fifo_uint8)			== 0x00);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 0);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 3);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	_fff_reset(fifo_uint8);
}

void fifofast_test_add(uint8_t startvalue)
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
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 1);
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

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 3);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		!= 0);
		
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+0);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+1);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== startvalue+2);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== startvalue+3);
	
	_fff_reset(fifo_uint8);
}

void fifofast_test_remove_lite(uint8_t startvalue)
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
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 1);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
		
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+2);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+3);


	// _remove 1 (Test case: fifo not full before and not empty after macro)
	_fff_remove_lite(fifo_uint8, 1);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 1);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 2);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
		
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+3);
	
	
	// _remove 1 (Test case: fifo empty after macro)
	_fff_remove_lite(fifo_uint8, 1);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 0);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 3);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	_fff_reset(fifo_uint8);
}

void fifofast_test_remove(uint8_t startvalue)
{
	// fill with any data (macros have been proved to work before)
	_fff_write_lite(fifo_uint8, startvalue+0);
	_fff_write_lite(fifo_uint8, startvalue+1);
	_fff_write_lite(fifo_uint8, startvalue+2);
	_fff_write_lite(fifo_uint8, startvalue+3);
	// fifo is now full
	
	
	// _remove 0 (Test case: amount <= 0 elements)
	_fff_remove(fifo_uint8, 0);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 3);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		!= 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+0);
	UT_ASSERT(_fff_peek(fifo_uint8, 1)		== startvalue+1);
	UT_ASSERT(_fff_peek(fifo_uint8, 2)		== startvalue+2);
	UT_ASSERT(_fff_peek(fifo_uint8, 3)		== startvalue+3);


	// _remove 4 (Test case: amount > _fff_mem_level() w/ fifo full)
	_fff_remove(fifo_uint8, 4);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 1);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 2);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		== 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
	
	UT_ASSERT(_fff_peek(fifo_uint8, 0)		== startvalue+3);


	// _remove 4 (Test case: amount > _fff_mem_level() w/ fifo not full)
	_fff_remove(fifo_uint8, 4);

	UT_ASSERT(_fff_mem_level(fifo_uint8)	== 0);
	UT_ASSERT(_fff_mem_free(fifo_uint8)		== 3);
	UT_ASSERT(_fff_is_empty(fifo_uint8)		!= 0);
	UT_ASSERT(_fff_is_full(fifo_uint8)		== 0);
}