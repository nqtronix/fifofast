/*
 * fifofast.h
 *
 * Created: 23.10.2017 06:45:20
 *  Author: Dennis aka nqtronix (github.com/nqtronix)
 *
 * Description:
 * Most libraries for data structures are designed for dynamically allocating memory, which can't be
 * efficiently used on small, low-cost, embedded devices.
 * This library implements the common fifo data structure based on function-like macros, which
 * allows the code to be generic for all data types and buffer sizes. All memory is allocated at
 * compile time. As a side effect the execution time for most functions and RAM memory consumption
 * is reduced, but the program size slightly increased.
 *
 * Some programs may need to access multiple fifos, e.g. to distribute measured ADC values to their
 * corresponding handler functions. For theses applications you can create fifos, which can be safely
 * passed by pointers (see _fff_create_p(...)). Although these fifos can be access by all
 * function-like macros, they can't be as generic and are limited to a depth of 256 and an element
 * size of 256 bytes.
 *
 * If any fifo is accessed from ISRs make sure all access to the involved fifo from normal code is
 * atomic! This is important to prevent glitches in the middle of an operation!
 */ 

#ifndef FIFOFAST_H_
#define FIFOFAST_H_

#include <stdint.h>

// include reduced list of macros if full list is not provided
#ifndef MACROS_H_
	#include "helper/macros.h"
#endif
#ifndef MACROS_MATH_H_
	#include "helper/macros_math.h"
#endif


//////////////////////////////////////////////////////////////////////////
// General Info
//////////////////////////////////////////////////////////////////////////

// version numbering is based on "Semantic Versioning 2.0.0" (semver.org)
#define FIFOFAST_VERSION_MAJOR		0
#define FIFOFAST_VERSION_MINOR		2
#define FIFOFAST_VERSION_PATCH		0
#define FIFOFAST_VERSION_SUFFIX		
#define FIFOFAST_VERSION_META		

//////////////////////////////////////////////////////////////////////////
// Check requirements
//////////////////////////////////////////////////////////////////////////

#ifndef __GNUC__
	#error fifofast.h requires "compound statments" and "typeof" offered by a GNU C/ GCC compiler!
#endif

#ifndef __OPTIMIZE__
	#pragma message "fifofast.h is intended to be compiled with optimisation and will run VERY SLOWLY without!"
#endif


//////////////////////////////////////////////////////////////////////////
// Data Structures (for inline functions only)
//////////////////////////////////////////////////////////////////////////

typedef struct
{
	const uint8_t data_size;	// bytes per element in data array
	const uint8_t mask;			// (max amount of elements in data array) - 1
	uint8_t read;				// index from which to read next element
	uint8_t write;				// index to which to write next element
	uint8_t level;				// possible writes without further checking
	uint8_t data[];				// data storage array
} fff8_t;

typedef struct
{
	const uint16_t data_size;	// bytes per element in data array
	const uint16_t mask;		// (max amount of elements in data array) - 1
	uint16_t read;				// index from which to read next element
	uint16_t write;				// index to which to write next element
	uint16_t level;				// possible writes without further checking
	uint8_t data[];				// data storage array
} fff16_t;

typedef struct
{
	const uint32_t data_size;	// bytes per element in data array
	const uint32_t mask;		// (max amount of elements in data array) - 1
	uint32_t read;				// index from which to read next element
	uint32_t write;				// index to which to write next element
	uint32_t level;				// possible writes without further checking
	uint8_t data[];				// data storage array
} fff32_t;


//////////////////////////////////////////////////////////////////////////
// internal macros (_fff__*)
//////////////////////////////////////////////////////////////////////////

// returns the structure name matching to given ID without the keyword 'struct'
#define _fff__name_struct(_id)			CAT(fff_, _id, _s)
#define _fff__name_structp8(_id)		CAT(fff_, _id, _sp8)
#define _fff__name_structp16(_id)		CAT(fff_, _id, _sp16)

// returns matching type for internal index values; fifo contrains are automatically applied
#define _fff__get_type(_depth)			typeof(_cast_min(_limit_lo(_depth,4)-1))
#define _fff__sizeof_data(_id)			sizeof(((struct _fff__name_struct(_id)*)0)->data[0])
#define _fff__sizeof_array(_id)			_sizeof_array(((struct _fff__name_struct(_id)*)0)->data)

// returns the length of the data array; fifo contrains are automatically applied
#define	_fff__get_arraydepth8(_depth)	_limit((1<<(1+_log2_8((uint8_t)_depth-1))), 4, UINT8_MAX+1)
#define	_fff__get_arraydepth16(_depth)	_limit((1<<(1+_log2_16((uint16_t)_depth-1))), 4, (uint16_t)INT16_MAX+1)


//////////////////////////////////////////////////////////////////////////
// user macros (_fff_*)
//////////////////////////////////////////////////////////////////////////

// all function-like macros are suitable for ANY fifo, independent of data type or size. 

// declares or initializes (defines) semi-anonymous fastfifo structure
// semi-anonymous means it appears anonymous for the user as it is derived from the '_id' whenever
// needed, but is not anonymous on compiler level. This has the additional benefit of VAssisX and
// debugging (inspecting variables during runtime with an external debugger) working as usual.
//
// The variant _fff_create_p(...) creates a structure, which can be used by the included
// inline function, as the size of each element and the maximum amount are stored as well. They
// still can be accessed with all function-like macros provided for some speed gain. 
//
// _prefix: if required C conform keywords such as volatile can be inserted here. If not required,
//			leave empty.
// _id:		C conform identifier
// _type:	any C type except pointers and structs. To store pointers or structs use typedef first
// _depth:	maximum amount of elements, which can be stored in the fifo. Only values of 2^n are
//			possible. If another value is passed the next larger value will be automatically
//			selected.
//			Additional 2/4/8 bytes are used for fifos with with <= 2^8/ <= 2^16 / <= 2^31 elements.


// creates a structure for the given parameters with the name 'fff_<_id>_s'
#define _fff_declare(_type, _id, _depth)								\
struct _fff__name_struct(_id) {											\
	_fff__get_type(_depth) read;										\
	_fff__get_type(_depth) write;										\
	_fff__get_type(_depth) level;										\
	_type data[_fff__get_arraydepth16(_depth)];							\
} _id

#define _fff_declare_p8(_type, _id, _depth)								\
struct _fff__name_structp8(_id) {										\
	const uint8_t data_size;											\
	const uint8_t mask;													\
	uint8_t read;														\
	uint8_t write;														\
	uint8_t level;														\
	_type data[_fff__get_arraydepth8(_depth)];							\
} _id

// initializes the fifo with the name '<_id>'
#define _fff_init(_id)													\
struct _fff__name_struct(_id) _id =										\
{																		\
	0,																	\
	0,																	\
	0,																	\
	{}																	\
}

// initializes the point-able fifo with the name '<_id>'
#define _fff_init_p8(_id)												\
struct _fff__name_structp8(_id) _id =									\
{																		\
	_fff__sizeof_data(_id),												\
	(_fff__sizeof_array(_id)-1),										\
	0,																	\
	0,																	\
	0,																	\
	{}																	\
}


// masks a given index value based on a given fifo
// This macro is used to simplify other marcos below; the end user will likely never need it
// _id:		C conform identifier
// idx:		the index value to mask. MUST be larger than -_sizeof_array(_id.data)
#define _fff_wrap(_id, idx)				((idx) & _fff_mem_mask(_id))

// returns the maximum amount of data elements which can be stored in the fifo
// The returned value is calculated at compile time and thus a constant. No atomic access is needed.
// NOTE: For a fifo of size 256 a "256" is returned and thus does not fit into uint8_t
// _id:		C conform identifier
#define _fff_mem_depth(_id)				(_sizeof_array(_id.data))
#define _fff_mem_mask(_id)				(_sizeof_array(_id.data)-1)

// returns the amount of bytes per data element
// The returned value is calculated at compile time and thus a constant. No atomic access is needed.
// _id:		C conform identifier
#define _fff_data_size(_id)				(sizeof(_id.data[0]))

// returns !0 if empty
#define _fff_is_empty(_id)				(_id.level == 0)

// returns !0 if full
#define _fff_is_full(_id)				((_id.write == _id.read) && (_id.level == _fff_mem_mask(_id)))

// returns the current fill level of the fifo (the amount of elements that can be read)
// Note that this macro is meant to check if access through _fff_peek(...) is possible. If the fifo
// is completely full (which should never happen normally), it will sill return (depth-1) instead!
// Use _fff_is_full(...) to determine whether the fifo is full.
// _id: C conform identifier
#define _fff_mem_level(_id)				(_id.level)

// returns the current free space of the fifo (the amount of elements that can be written)
// Note that this macro is meant to check if repeated writes are possible. If the fifo
// is almost full (depth-1, should only rarely happen normally), it will return 0.
// Use !_fff_is_full(...) to determine whether can accept further elements.
// _id: C conform identifier
#define _fff_mem_free(_id)				(_fff_mem_mask(_id) - _id.level)


// clears/ resets buffer completely
// _id:		C conform identifier
#define _fff_reset(_id)					do{_id.read=0; _id.write=0; _id.level=0;} while (0)


// TODO: remove is broken, reason unknown; details:
// _fff_remove(_id, amount) should have the same effect as while(amount){_fff_read(_id);amount--}

// removes a certain number of elements or less, if not enough elements are available.
// This function is especially useful after data has been used by _fff_peek(...)
// _id:		C conform identifier
// amount:	Amount of elements which will be removed; must be amount > 0;
// #define _fff_remove_broken(_id, amount)								\
// do{																\
// 	uint8_t _amount = (amount);									\
// 	if(_fff_is_full(_id))										\
// 	{															\
// 		_id.min_w++;											\
// 		_amount--;												\
// 	}															\
// 	if(_fff_mem_mask(_id)-_id.min_w > (_amount))	/*flush if equal*/		\
// 	{															\
// 		_id.min_w	+= _amount;									\
// 		_id.read	= _fff_wrap(_id, (_id.read+(_amount+1)));	\
// 	}															\
// 	else														\
// 		_fff_reset(_id);										\
// }while(0)


#define _fff_remove(_id, amount)								\
do{																\
	for (uint8_t _idx = amount; _idx > 0; _idx--)				\
		{_fff_read(_id);}										\
}while(0)
					

// #define _fff_remove_newest(_id, amount)							\
// do{																\
// 	for (uint8_t _idx = amount; _idx > 0; _idx--)				\
// 	{															\
// 		if(!_fff_is_empty(_id))									\
// 			_id.min_w++;										\
// 		_id.write = _fff_wrap(_id, (_id.write-1));				\
// 	}															\
// }while(0)					


// returns the next element from the fifo and removes it from the memory
// Use if(!_fff_is_empty(_id)) if amount of stored data is unknown
// _id: C conform identifier
#define _fff_read_lite(_id)										\
({																\
	typeof(_id.data[0])	_return;								\
	if(!_fff_is_full(_id))							\
		_id.level--;											\
																\
	_return = _id.data[_id.read];								\
	_id.read = _fff_wrap(_id, (_id.read+1));					\
	_return;													\
})

// returns the next element from the fifo and removes it from the memory
// if no element is available, 0 is returned
// _id: C conform identifier
#define _fff_read(_id)											\
({																\
	typeof(_id.data[0])	_return;								\
	if(!_fff_is_empty(_id))										\
		_return = _fff_read_lite(_id);							\
	else														\
		_return = 0;											\
	_return;													\
})	

				 
// adds an element to the fifo
// Use if(!_fff_is_full(_id)) if amount of stored data is unknown
// _id:		C conform identifier
// newdata:	data to be written
#define _fff_write_lite(_id, newdata)							\
do{																\
	_id.data[_id.write] = (newdata);							\
	_id.write = _fff_wrap(_id, (_id.write+1));					\
	if(_id.level != _fff_mem_mask(_id))							\
		_id.level++;											\
}while(0)

// adds an element to the fifo, if space is available
// if full element will be dismissed
// _id:		C conform identifier
// newdata:	data to be written
#define _fff_write(_id, newdata)								\
do{																\
	if(!_fff_is_full(_id))										\
		_fff_write_lite(_id, newdata);							\
}while(0)

// #define _fff_write_bulk(_id, cnt, pointer)
// do{
// 
// 
// }while(0);

// adds an element to the fifo, but does not write any data to it. instead, a pointer to the data
// section is returned. The caller may write up to _fff_data_size(_id) bytes to this location.
// Use if(!_fff_is_full(_id)) if amount of stored data is unknown
// _id: C conform identifier
#define _fff_add_lite(_id)										\
({																\
	typeof(&_id.data[0]) _return = & _id.data[_id.write];		\
	_id.write = _fff_wrap(_id, (_id.write+1));					\
	if(_id.level != _fff_mem_mask(_id))							\
		_id.level++;											\
	_return;													\
})

// like _fff_add_lite(_id), but checks if space is available before writing. Returns 'null' if full.
// _id: C conform identifier
#define _fff_add(_id)											\
({																\
	typeof(&_id.data[0]) _return;								\
	if(!_fff_is_full(_id))										\
		_return = _fff_add_lite(_id);							\
	else														\
		_return = NULL;											\
	_return;													\
})


// allows accessing the data of a fifo as an array without removing any elements
// Like any array this function can be used as a right or left site operant. Attempting to access
// more elements than currently in the buffer will return undefined data on read and will have no
// effect on write. Accidental read/write operations outside the assigned data space are not
// possible.
// This macro is NOT ATOMIC. If fifo "_id" is accessed within an ISR at least once, this macro MUST
// be placed within an atomic block outside of any ISR.
// _id:		C conform identifier
// idx:		Offset from the first element in the buffer
#define _fff_peek(_id, idx)				_id.data[_fff_wrap(_id, _id.read+(idx))]



// functions to allow use of pinter
// inline uint8_t fff_wrap(fff8_t *fifo, uint8_t idx)
// {
// 	return (idx & fifo->mask);
// }
// inline uint8_t fff_data_size(fff8_t *fifo)
// {
// 	return fifo->data_size;
// }
// 
// inline uint8_t fff_is_empty(fff8_t *fifo)
// {
// 	return ((fifo->mask == fifo->min_w) && (fifo->min_w != 0));
// }
// inline uint8_t fff_is_full(fff8_t *fifo)
// {
// 	return ((fifo->min_w == 0) && (fifo->write == fifo->read));
// }
// inline uint8_t fff_mem_used(fff8_t *fifo)
// {
// 	return (fifo->mask - fifo->min_w);
// }
// inline uint8_t fff_mem_free(fff8_t *fifo)
// {
// 	return (fifo->min_w);
// }
// 
// inline void fff_flush(fff8_t *fifo)
// {
// 	fifo->read	= 0;
// 	fifo->write	= 0;
// 	fifo->min_w	= fifo->mask;
// }
// 
// inline void fff_remove(fff8_t *fifo, uint8_t amount)
// {
// 	if(fff_is_full(fifo))
// 	{
// 		fifo->min_w--;
// 		amount--;
// 	}
// 	if(fifo->mask - fifo->min_w > amount)	// flush if equal
// 	{
// 		fifo->min_w	+= amount;
// 		fifo->read	= fff_wrap(fifo, (fifo->read + amount));
// 	}
// 	else
// 		fff_flush(fifo);
// }
// 
// inline void* fff_peek_read(fff8_t *fifo, uint8_t idx)
// {
// 	return &(fifo->data[fff_wrap(fifo, fifo->read + idx) * fifo->data_size]);
// }
// 
// // like fff_peek_read, but overwrites the stored elements
// inline void fff_peek_write(fff8_t *fifo, uint8_t idx, void *data)
// {
// 	// copy an element byte for byte into the fifo
// 	for (uint8_t i = fifo->data_size; i>0; i--)
// 	{
// 		fifo->data[fifo->read*fifo->data_size + i-1] = ((uint8_t*)data)[i-1];
// 	}
// }
// 
// // not tested yet
// inline void fff_write(fff8_t *fifo, void *data)
// {
// 	for (uint8_t i = fifo->data_size; i>0; i--)
// 	{
// 		fifo->data[fifo->write*fifo->data_size + i-1] = ((uint8_t*)data)[i-1];
// 	}
// 	fifo->write = fff_wrap(fifo, fifo->write+1);
// 	if(--fifo->min_w == 255)
// 		fifo->min_w = 0;
// }
// 
// 
// inline void fff_write_safe(fff8_t *fifo, void *data)
// {
// 	if (fff_is_full(fifo))
// 		return;
// 
// 	fff_write(fifo, data);
// }


#endif /* FIFOFAST_H_ */