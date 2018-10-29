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


// returns smallest type for given integer
#define _type_min(_integer)		typeof(_type_cast_min(_integer))

// By default all literals are interpreted as a int, whose size varies between systems
// This macro forces minimum memory usage by casting a literal to the smallest data type suitable.
// Unsigned types are proffered.
#define _type_cast_min(_integer)																			\
	(__builtin_choose_expr((_integer)>0,															\
	__builtin_choose_expr((_integer)<=UINT8_MAX, (uint8_t) (_integer),								\
	__builtin_choose_expr((_integer)<=UINT16_MAX, (uint16_t) (_integer),							\
	__builtin_choose_expr((_integer)<=UINT32_MAX, (uint32_t) (_integer), (uint64_t) (_integer)))),	\
	__builtin_choose_expr((_integer)>=INT8_MIN, (int8_t) (_integer),								\
	__builtin_choose_expr((_integer)>=INT16_MIN, (int16_t) (_integer),								\
	__builtin_choose_expr((_integer)>=INT32_MIN, (int32_t) (_integer), (int64_t) (_integer))))))



#endif /* MACRO_TYPE_H_ */