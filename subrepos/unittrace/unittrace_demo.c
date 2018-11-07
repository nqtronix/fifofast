/*
 * unittrace.c
 *
 * Created: 29.10.2018 20:32:06
 * Author : Dennis aka nqtronix (github.com/nqtronix)
 */ 


#include "unittrace.h"

int main(void)
{
	// using inline functions
	ut_assert(5 == 5);	// passes
	UT_BREAK();
	ut_assert(5 == 7);	// fails and code address gets stored in array
	UT_BREAK();
	
	// using macros (equivalently behavior to functions)
	UT_ASSERT(5 == 5);	// passes
	UT_BREAK();
	UT_ASSERT(5 == 7);	// fails and code address gets stored in array
	UT_BREAK();

	// Access generated data
	// You can access the data either with a debugger by watching the two global variables or use
	// them in your code. See pseudocode below:
	for (uint8_t cnt=0; cnt < unittrace_count; cnt++)
	{
		/*usedata(unittrace_array[cnt]);*/
	}

	// End test
	UT_BREAK();
    while (1);
}
