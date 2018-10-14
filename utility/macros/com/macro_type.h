/*
 * macro_type.h
 *
 * Created: 14.10.2018 14:34:03
 *  Author: Dennis
 *
 * Description:
 * Provides typ-related macros
 */


#ifndef MACRO_TYPE_H_
#define MACRO_TYPE_H_


// returns a value not smaller or larger than the limit
#define _limit(arg, lo, hi)		((arg)<lo? lo : (arg)>hi? hi : (arg))
#define _limit_lo(arg, lo)		((arg)<lo? lo : (arg))
#define _limit_hi(arg, hi)		((arg)>hi? hi : (arg))

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



#endif /* MACRO_TYPE_H_ */