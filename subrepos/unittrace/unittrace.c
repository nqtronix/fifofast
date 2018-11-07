/*
 * unittrace.c
 *
 * Created: 29.10.2018 20:34:06
 *  Author: Dennis aka nqtronix (github.com/nqtronix)
 */ 


#include "unittrace.h"
#include <stddef.h>		// required for 'NULL'


//////////////////////////////////////////////////////////////////////////
// Variables
//////////////////////////////////////////////////////////////////////////

volatile void* unittrace_array[UNITTRACE_LIST_SIZE] = {[0 ... UNITTRACE_LIST_SIZE-1] = NULL};
ut_cnt_t unittrace_count = 0;


//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////

void ut_assert_manual(void* addr, uint8_t cond)
{
	// only store event, if condition is 'false' 
	if (cond == 0)
	{
		#ifndef UNITTRACE_USE_EXT_COUNTER
			// store event data into array, if it is not full yet
			if (unittrace_count < UNITTRACE_LIST_SIZE)
			{
				unittrace_array[unittrace_count] = addr;
				unittrace_count++;
			}
		
		#else
			// store event data into array, if it is not full yet
			if (unittrace_count < UNITTRACE_LIST_SIZE)
				unittrace_array[unittrace_count] = addr;
			
			// increment counter, reset if overflow detected
			// this odd code works with any positive integer type
			unittrace_count++;
			if (unittrace_count == 0)
				unittrace_count--;
		
		#endif
	}
}


void* ut_get_array(void)
{
	return unittrace_array;
}

ut_cnt_t ut_get_count(void)
{
	return unittrace_count;
}