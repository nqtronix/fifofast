/*
 * macro_math.h
 *
 * Created: 14.10.2018 14:34:32
 *  Author: Dennis
 *
 * Description:
 * Provides math operations, which can be solved at compile time, given the inputs are literals.
 * If used during runtime, these macros might be significant slower than the usual implementations.
 */


#ifndef MACRO_MATH_H_
#define MACRO_MATH_H_




//////////////////////////////////////////////////////////////////////////
// Comparisons
//////////////////////////////////////////////////////////////////////////

// returns 0 if argument is not power of two
#define _is_power_of_two(arg)						((arg) && !((arg) & ((arg) - 1)))

// returns 0 if both arguments have the same sign
#define _is_sign_opposite(arg1, arg2)				(((arg1) ^ (arg2)) < 0)


//////////////////////////////////////////////////////////////////////////
// Calc
//////////////////////////////////////////////////////////////////////////

// returns the absolute value
#define _abs(arg)				((arg) >= 0? (arg) : -(arg))

// shifts 'input' left or right by 'shiftAmount' , but unlike the C standard operators << and >>
// negative values are allowed and result in a shift of the opposite direction
#define _shift_left(arg, shiftAmount)		\
	(shiftAmount > 0 ? arg << shiftAmount : arg >> (-shiftAmount))

#define _shift_right(arg, shiftAmount)		\
	(shiftAmount > 0 ? arg >> shiftAmount : arg << (-shiftAmount))

// returns the mathematical function log2(x), rounded down to nearest integer
// this marco-like function makes use of a GCC build-in function and can be used eg. for if(log2(x)>y)
// If a literal is passed to this function, the result is calculated at compile time and stored in flash.
#define _log2(n)	((unsigned) (8*sizeof (unsigned long long) - __builtin_clzll((n)) - 1))



#endif /* MACRO_MATH_H_ */