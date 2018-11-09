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

#include <stdint.h>		// required for data types (uint8_t, uint16_t, ...)
#include <stddef.h>		// required for "NULL"
#include <string.h>		// required for memcopy

// include required macros
// by default, the project's macros will be used, they have to be located at:
// "utility/macros/..."
//
// If your project has no global macros, you can re-direct GCC to the included files:
// a) in Atmel Studio edit project properties (ALT+F7 OR "Project -> <project name> Properties...")
// b) Go to "Toolchain / AVR/GNU C Compiler / Directories"
// c) add the relative path to: "../src/subrepo/fifofast/" or wherever this code is located. DONE!
//
// YOU DO NOT need to change the include(s) below.
#include "utility/macros/mpl/macro_cat.h"
#include "utility/macros/com/macro_array.h"
#include "utility/macros/com/macro_math.h"
#include "utility/macros/com/macro_type.h"


//////////////////////////////////////////////////////////////////////////
// User Config
//////////////////////////////////////////////////////////////////////////

// defines the maximum depth of pointable fifos. The performance of fifofast drops with increased
// maximum depth in discrete steps, especially on 8bit MCUs. 32bit MCUs (such as ARM cortex) are
// mostly unaffected, except they require slightly more RAM.
//
// depth			| performance
// -----------------+-------------
//    4 <= x <= 128	| best
//         x == 256	| ok
//  512 <= x		| slow
#define FIFOFAST_MAX_DEPTH_POINTABLE	128


//////////////////////////////////////////////////////////////////////////
// General Info
//////////////////////////////////////////////////////////////////////////

// version numbering is based on "Semantic Versioning 2.0.0" (semver.org)
#define FIFOFAST_VERSION_MAJOR		0
#define FIFOFAST_VERSION_MINOR		6
#define FIFOFAST_VERSION_PATCH		0
#define FIFOFAST_VERSION_SUFFIX		
#define FIFOFAST_VERSION_META		

// For all development versions (0.x.x) the patch version is increased whenever a function was renamed

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

// extra #defines prevent VAssitX from marking the type red (because it doesn't understand 'typeof')
#define FIFOFAST_INDEX_T _type_min(FIFOFAST_MAX_DEPTH_POINTABLE-1)
#define FIFOFAST_LEVEL_T _type_min(FIFOFAST_MAX_DEPTH_POINTABLE)
typedef FIFOFAST_INDEX_T fff_index_t;
typedef FIFOFAST_LEVEL_T fff_level_t;

typedef struct
{
	const fff_index_t data_size;	// bytes per element in data array
	const fff_index_t mask;			// (max amount of elements in data array) - 1
	fff_index_t read;				// index from which to read next element
	fff_index_t write;				// index to which to write next element
	fff_level_t level;				// possible writes without further checking
	uint8_t data[];					// data storage array
} fff_proto_t;


//////////////////////////////////////////////////////////////////////////
// internal macros (_FFF_*)
//////////////////////////////////////////////////////////////////////////

// returns the structure name matching to given ID without the keyword 'struct'
#define _FFF_NAME_STRUCT(_id)			CAT(fff_, _id, _s)

// returns matching type for internal index values; fifo contrains are automatically applied
#define _FFF_GET_TYPE(_depth)			_type_min(_limit_lo(_depth,4)-1)
#define _FFF_SIZEOF_DATA(_id)			sizeof(((struct _FFF_NAME_STRUCT(_id)*)0)->data[0])
#define _FFF_SIZEOF_ARRAY(_id)			_sizeof_array(((struct _FFF_NAME_STRUCT(_id)*)0)->data)

// returns the length of the data array; fifo contrains are automatically applied
#define	_FFF_GET_ARRAYDEPTH8(_depth)	_limit((1<<(1+_log2((uint8_t)_depth-1))), 4, UINT8_MAX+1)
#define	_FFF_GET_ARRAYDEPTH16(_depth)	_limit((1<<(1+_log2((uint16_t)_depth-1))), 4, (uint16_t)INT16_MAX+1)


//////////////////////////////////////////////////////////////////////////
// user macros (_fff_*)
//////////////////////////////////////////////////////////////////////////

// all function-like macros are suitable for ANY fifo, independent of data type or size. 

// declares semi-anonymous fifofast structure
// semi-anonymous means it appears anonymous for the user as it is derived from the '_id' whenever
// needed, but is not anonymous on compiler level. This has the additional benefit of VAssisX and
// debugging (inspecting variables during runtime with an external debugger) working as usual. The
// fifo shall only be access through macros or functions provided in this file.
//
// The variant _fff_declare_p(...) declares a structure which includes the size of each element and
// the maximum amount of elements and can thus be used by the inline functions. They still can be
// accessed with all function-like macros provided for some speed gain. 
//
// The variant_fff_declare_pa(...) declares an array with structures as declared by _fff_declare_p(...).
// 
//
// _id:		C conform identifier
// _type:	any C type except pointers and structs. To store pointers or structs use typedef first
// _depth:	maximum amount of elements, which can be stored in the fifo. Only values of 2^n are
//			possible. If another value is passed the next larger value will be automatically
//			selected.
//			Additional 3/6/10 bytes are used for fifos with with <= 2^8/ <= 2^16 / <= 2^31 elements.

#define _fff_declare(_type, _id, _depth)								\
struct _FFF_NAME_STRUCT(_id) {											\
	_FFF_GET_TYPE(_depth) read;											\
	_FFF_GET_TYPE(_depth) write;										\
	_FFF_GET_TYPE(_depth+1) level;										\
	_type data[_FFF_GET_ARRAYDEPTH16(_depth)];							\
} _id

#define _fff_declare_p(_type, _id, _depth)								\
struct _FFF_NAME_STRUCT(_id) {											\
	const fff_index_t data_size;										\
	const fff_index_t mask;												\
	fff_index_t read;													\
	fff_index_t write;													\
	fff_level_t level;													\
	_type data[_FFF_GET_ARRAYDEPTH8(_depth)];							\
} _id

// declares an array with '_size' fifos. '_size' can be any positive integer.
#define _fff_declare_a(_type, _id, _depth, _size)		_fff_declare(_type, _id, _depth) [_size]
#define _fff_declare_pa(_type, _id, _depth, _size)		_fff_declare_p(_type, , _depth) [_size]

// TODO: make '_FFF_GET_ARRAYDEPTH8(_depth)' dependent on fff_index_t


// initializes the fifo with the name '<_id>'
// This initialization function is technically identical to the term "definition" in c, but to
// prevent confusion with "#define" it has been named '_fff_init()'. Since it is a definition it
// can be only called once. Use '_fff_reset()' to reset any fifo back to it's original state.
//
// The variants '_fff_init_p(_id)' and '_fff_init_pa(_id, _arraysize)' are intended for the
// respective declarations.
#define _fff_init(_id)													\
struct _FFF_NAME_STRUCT(_id) _id =										\
{																		\
	0,																	\
	0,																	\
	0,																	\
	{}																	\
}

#define _fff_init_a(_id, _arraysize)									\
struct _FFF_NAME_STRUCT(_id) _id [] =									\
{[0 ... _arraysize-1] = {												\
	0,																	\
	0,																	\
	0,																	\
	{}																	\
}}

#define _fff_init_p(_id)												\
struct _FFF_NAME_STRUCT(_id) _id =										\
{																		\
	_FFF_SIZEOF_DATA(_id),												\
	_FFF_SIZEOF_ARRAY(_id)-1,											\
	0,																	\
	0,																	\
	0,																	\
	{}																	\
}

#define _fff_init_pa(_id, _arraysize)									\
struct _FFF_NAME_STRUCT(_id) _id [] =									\
{[0 ... _arraysize-1] = {												\
	_FFF_SIZEOF_DATA(_id),												\
	_FFF_SIZEOF_ARRAY(_id)-1,											\
	0,																	\
	0,																	\
	0,																	\
	{}																	\
}}


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
#define _fff_is_full(_id)				(_id.level > _fff_mem_mask(_id))

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
#define _fff_mem_free(_id)				(_fff_mem_depth(_id) - _id.level)


// clears/ resets buffer completely
// _id:		C conform identifier
#define _fff_reset(_id)					do{_id.read=0; _id.write=0; _id.level=0;} while (0)

	
// removes a certain number of elements or less, if not enough elements are available.
// This function is especially useful after data has been used by _fff_peek(...)
// _id:		C conform identifier
// amount:	Amount of elements which will be removed, amount >= 0 (positive integer)
#define _fff_remove(_id, amount)								\
do{																\
	typeof(_id.level) _amount = amount;							\
	if(amount > _id.level)										\
		_amount = _id.level;									\
	_fff_remove_lite(_id, _amount);								\
}while(0)
					
// removes a certain number of elements. The user must ensure that the given amount of elements can
// be removed; 0 and _fff_depth(_id) are invallid amounts! If you require argument checking use 
// _fff_remove().
// This function is especially useful after data has been used by _fff_peek(...)
// _id:		C conform identifier
// amount:	Amount of elements which will be removed; must be 0 <= amount <= _fff_mem_level(_id);
#define _fff_remove_lite(_id, amount)							\
do{																\
	_id.level -= amount;										\
	_id.read = _fff_wrap(_id, _id.read+amount);					\
}while(0)				


// returns the next element from the fifo and removes it from the memory
// Use if(!_fff_is_empty(_id)) if amount of stored data is unknown
// _id: C conform identifier
#define _fff_read_lite(_id)										\
({																\
	typeof(_id.data[0])	_return;								\
	_id.level--;												\
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
		_return = *((typeof(_id.data[0])*)NULL);				\
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
	_id.level++;												\
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


// adds an element to the fifo, but does not write any data to it. instead, a pointer to the data
// section is returned. The caller may write up to _fff_data_size(_id) bytes to this location.
// Use if(!_fff_is_full(_id)) if amount of stored data is unknown
// _id: C conform identifier
#define _fff_add_lite(_id)										\
({																\
	typeof(&_id.data[0]) _return = & _id.data[_id.write];		\
	_id.write = _fff_wrap(_id, (_id.write+1));					\
	_id.level++;												\
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
		_return = (typeof(&_id.data[0]))NULL;					\
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



// internal functions
static inline fff_index_t fff_wrap(fff_proto_t *fifo, fff_index_t idx) __attribute__((__always_inline__));
static inline void* fff_data_p(fff_proto_t *fifo, fff_index_t idx) __attribute__((__always_inline__));

// these functions behave as their corresponding macros, so please refer to their description
// for infos on usage.
static inline fff_index_t	fff_mem_mask(fff_proto_t *fifo) __attribute__((__always_inline__));
static inline fff_level_t	fff_mem_level(fff_proto_t *fifo) __attribute__((__always_inline__));
static inline fff_index_t	fff_mem_free(fff_proto_t *fifo) __attribute__((__always_inline__));

static inline fff_index_t	fff_data_size(fff_proto_t *fifo) __attribute__((__always_inline__));
static inline uint8_t	fff_is_empty(fff_proto_t *fifo) __attribute__((__always_inline__));
static inline uint8_t	fff_is_full(fff_proto_t *fifo) __attribute__((__always_inline__));

static inline void		fff_reset(fff_proto_t *fifo) __attribute__((__always_inline__));
static inline void		fff_remove(fff_proto_t *fifo, fff_level_t amount) __attribute__((__always_inline__));
static inline void		fff_remove_lite(fff_proto_t *fifo, fff_level_t amount) __attribute__((__always_inline__));
static inline void		fff_write(fff_proto_t *fifo, void *data) __attribute__((__always_inline__));
static inline void		fff_write_lite(fff_proto_t *fifo, void *data) __attribute__((__always_inline__));

static inline void*		fff_peek_read(fff_proto_t *fifo, fff_index_t idx) __attribute__((__always_inline__));
static inline void		fff_peek_write(fff_proto_t *fifo, fff_index_t idx, void *data) __attribute__((__always_inline__));


//////////////////////////////////////////////////////////////////////////
// Inline functions
//////////////////////////////////////////////////////////////////////////

// Inline functions MUST be defined in the .h, not in the .c file to work correctly!

// auxiliary functions
static inline fff_index_t fff_wrap(fff_proto_t *fifo, fff_index_t idx)
{
	return (idx & fifo->mask);
}
static inline void* fff_data_p(fff_proto_t *fifo, fff_index_t idx)
{
	return &(fifo->data[idx * fifo->data_size]);
}
static inline fff_index_t fff_mem_mask(fff_proto_t *fifo)
{
	return (fifo->mask);
}
static inline fff_index_t fff_data_size(fff_proto_t *fifo)
{
	return fifo->data_size;
}

//
static inline uint8_t fff_is_empty(fff_proto_t *fifo)
{
	return (fifo->level == 0);
}
static inline uint8_t fff_is_full(fff_proto_t *fifo)
{
	return (fifo->level > fifo->mask);
}
static inline fff_level_t fff_mem_level(fff_proto_t *fifo)
{
	return (fifo->level);
}
static inline fff_index_t fff_mem_free(fff_proto_t *fifo)
{
	return (fifo->mask - fifo->level + 1);
}

//
static inline void fff_reset(fff_proto_t *fifo)
{
	fifo->read	= 0;
	fifo->write	= 0;
	fifo->level = 0;
}


static inline void fff_remove(fff_proto_t *fifo, fff_level_t amount)
{
	if (amount > fifo->level)
		amount = fifo->level;
	fff_remove_lite(fifo, amount);
}
static inline void fff_remove_lite(fff_proto_t *fifo, fff_level_t amount)
{
	fifo->level -= amount;
	fifo->read = fff_wrap(fifo, fifo->read + amount);
}

static inline void fff_write(fff_proto_t *fifo, void *data)
{
	if (!fff_is_full(fifo))
	fff_write_lite(fifo,data);
}
static inline void fff_write_lite(fff_proto_t *fifo, void *data)
{
	memcpy(fff_data_p(fifo, fifo->write), data, fifo->data_size);
	fifo->write = fff_wrap(fifo, fifo->write+1);
	fifo->level++;
}

// the peek function MUST be split into two to work as a normal c function
// BOTH function STILL refer to the top (read) end of the fifo
static inline void* fff_peek_read(fff_proto_t *fifo, fff_index_t idx)
{
	return fff_data_p(fifo, fff_wrap(fifo, fifo->read+idx));
}
static inline void fff_peek_write(fff_proto_t *fifo, fff_index_t idx, void *data)
{
	memcpy(fff_data_p(fifo, fff_wrap(fifo, fifo->read+idx)), data, fifo->data_size);
}


#endif /* FIFOFAST_H_ */