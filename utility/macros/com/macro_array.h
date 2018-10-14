/*
 * macro_array.h
 *
 * Created: 14.10.2018 14:38:19
 *  Author: Dennis
 */ 


#ifndef MACRO_ARRAY_H_
#define MACRO_ARRAY_H_


// like sizeof(), but returns the element count of a const array
#define _sizeof_array(_array)	(sizeof(_array)/sizeof(_array[0]))

// returns the length of a string
#define _sizeof_str(_string)	((sizeof(_string)/sizeof(_string[0]))-sizeof(_string[0]))


// Allows access of nth byte in uint16_t, uint32_t, ...
#define _get_nth_element(_type, input, n)		((_type)(input>>(n*sizeof(_type))))

// returns the index of an array that matches the pointer. the pointer MUST point to an element of
// the array, checks are NOT performed
#define _array_pointer2idx(_array_id, pointer)	((pointer-_array_id)/sizeof(_array_id[0]))


#endif /* MACRO_ARRAY_H_ */