/*
 * fifofast_demo.h
 *
 * Created: 08.11.2018 08:46:08
 *  Author: Dennis
 *
 * Description:
 * This file declares all fifos used by the demo. If declared in a header, they can be accessed by
 * all files, that include this header. In this particular case it is needed for 'fifofast_test.c'.
 * If a fifo does NOT need to be access from multiple files (i.e. if used as a buffer for ISRs),
 * they can be declared in a c file.
 */ 


#ifndef FIFOFAST_DEMO_H_
#define FIFOFAST_DEMO_H_

#include "fifofast.h"


// declare a fifo with 4 elements of type 'uint8_t' with the name 'fifo_uint8'
_fff_declare(uint8_t __attribute__((aligned(4))), fifo_uint8, 4);

// declare same fifo as above, but it can be passed by pointer to functions
_fff_declare_p(uint8_t, fifo_uint8p, 4);

// declare a fifo with 8 elements (6 elements is not possible, so it is automatically bumped to 8)
// of type 'int_16' with the name 'fifo_uint16'
_fff_declare(int16_t, fifo_int16, 6);

// fifofast also supports more complicated data such as frames used for serial data transmission.
// It is often useful to access the data not only in binary format ('raw'), but also as a struct
// ('header'). In this case the header contains a variable >1 byte, which needs to be stored aligned
// on most architectures. However the union is treaded by default as a 'uint8_t' array, so no
// alignment is enforced. To do this manually, GCC supports the attribute 'aligned(n)'. See
// documentation of GCC for more details.
typedef struct
{
	uint16_t control;
	uint8_t length;
	uint8_t id;
	uint8_t data[];
} header_t;
typedef union __attribute__((aligned(4), packed))
{
	uint8_t raw[32];
	header_t header;
} frame_u;

// declare a fifo to store 4 elements of the typedef'd union 'frame_u'
_fff_declare(frame_u, fifo_frame, 4);

// declare an array (indicated by the suffix _a) of 5 fifos with 16 elements each.
_fff_declare_a(uint8_t, fifo_array, 16, 5);


#endif /* FIFOFAST_DEMO_H_ */