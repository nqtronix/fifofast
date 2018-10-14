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
// this marco-like function does not contain a loop and can be used eg. for if(log2(x)>y)
#define _log2_8(_n)		 ((_n) >= 1<<7 ? 7 \
						: (_n) >= 1<<6 ? 6 \
						: (_n) >= 1<<5 ? 5 \
						: (_n) >= 1<<4 ? 4 \
						: (_n) >= 1<<3 ? 3 \
						: (_n) >= 1<<2 ? 2 \
						: (_n) >= 1<<1 ? 1 \
						: 0)

#define _log2_16(_n)	 ((_n) >= 1<<15 ? 15 \
						: (_n) >= 1<<14 ? 14 \
						: (_n) >= 1<<13 ? 13 \
						: (_n) >= 1<<12 ? 12 \
						: (_n) >= 1<<11 ? 11 \
						: (_n) >= 1<<10 ? 10 \
						: (_n) >= 1<<9 ? 9 \
						: (_n) >= 1<<8 ? 8 \
						: (_n) >= 1<<7 ? 7 \
						: (_n) >= 1<<6 ? 6 \
						: (_n) >= 1<<5 ? 5 \
						: (_n) >= 1<<4 ? 4 \
						: (_n) >= 1<<3 ? 3 \
						: (_n) >= 1<<2 ? 2 \
						: (_n) >= 1<<1 ? 1 \
						: 0)

#define _log2_32(_n)	 ((_n) >= 1<<31 ? 31 \
						: (_n) >= 1<<30 ? 30 \
						: (_n) >= 1<<29 ? 29 \
						: (_n) >= 1<<28 ? 28 \
						: (_n) >= 1<<27 ? 27 \
						: (_n) >= 1<<26 ? 26 \
						: (_n) >= 1<<25 ? 25 \
						: (_n) >= 1<<24 ? 24 \
						: (_n) >= 1<<23 ? 23 \
						: (_n) >= 1<<22 ? 22 \
						: (_n) >= 1<<21 ? 21 \
						: (_n) >= 1<<20 ? 20 \
						: (_n) >= 1<<19 ? 19 \
						: (_n) >= 1<<18 ? 18 \
						: (_n) >= 1<<17 ? 17 \
						: (_n) >= 1<<16 ? 16 \
						: (_n) >= 1<<15 ? 15 \
						: (_n) >= 1<<14 ? 14 \
						: (_n) >= 1<<13 ? 13 \
						: (_n) >= 1<<12 ? 12 \
						: (_n) >= 1<<11 ? 11 \
						: (_n) >= 1<<10 ? 10 \
						: (_n) >= 1<<9 ? 9 \
						: (_n) >= 1<<8 ? 8 \
						: (_n) >= 1<<7 ? 7 \
						: (_n) >= 1<<6 ? 6 \
						: (_n) >= 1<<5 ? 5 \
						: (_n) >= 1<<4 ? 4 \
						: (_n) >= 1<<3 ? 3 \
						: (_n) >= 1<<2 ? 2 \
						: (_n) >= 1<<1 ? 1 \
						: 0)


#endif /* MACRO_MATH_H_ */