/*
 * macros_math.h
 *
 * Created: 01.06.2017 08:11:08
 *  Author: Dennis aka nqtronix (github.com/nqtronix)
 *
 * Description
 * This file contains a subset of my macro collection which is required for fifiofast.c/.h to function.
 */ 


#ifndef MACROS_MATH_H_
#define MACROS_MATH_H_


// like sizeof(), but returns the element count of a const array
#define _sizeof_array(_array)	(sizeof(_array)/sizeof(_array[0]))

// By default all literals are interpreted as a int, whose size varies between systems
// This macro forces minimum memory usage by casting a literal to the smallest data type suitable.
// Unsigned types are proffered.
#define _cast_min(_literal)																			\
	(__builtin_choose_expr((_literal)>0,															\
	__builtin_choose_expr((_literal)<=UINT8_MAX, (uint8_t) (_literal),								\
	__builtin_choose_expr((_literal)<=UINT16_MAX, (uint16_t) (_literal),							\
	__builtin_choose_expr((_literal)<=UINT32_MAX, (uint32_t) (_literal), (uint64_t) (_literal)))),	\
	__builtin_choose_expr((_literal)>=INT8_MIN, (int8_t) (_literal),								\
	__builtin_choose_expr((_literal)>=INT16_MIN, (int16_t) (_literal),								\
	__builtin_choose_expr((_literal)>=INT32_MIN, (int32_t) (_literal), (int64_t) (_literal))))))

// returns the absolute value
#define _abs(arg)				((arg) >= 0? (arg) : -(arg))

// returns a value not smaller or larger than the limit
#define _limit(arg, lo, hi)		((arg)<lo? lo : (arg)>hi? hi : (arg))
#define _limit_lo(arg, lo)		((arg)<lo? lo : (arg))
#define _limit_hi(arg, hi)		((arg)>hi? hi : (arg))


// returns the mathematical function log2(x), rounded down to nearest integer
// this marco-like function does not contain a loop and can be used eg. for if(log2(x)>y)
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

#define _log2_8(_n)		 ((_n) >= 1<<7 ? 7 \
						: (_n) >= 1<<6 ? 6 \
						: (_n) >= 1<<5 ? 5 \
						: (_n) >= 1<<4 ? 4 \
						: (_n) >= 1<<3 ? 3 \
						: (_n) >= 1<<2 ? 2 \
						: (_n) >= 1<<1 ? 1 \
						: 0)


#endif /* MACROS_MATH_H_ */