/*
 * fifofast_test.h
 *
 * Created: 08.11.2018 08:35:16
 *  Author: Dennis
 *
 * Description:
 * This file contains automated tests powered by unittrace.
 */ 


#ifndef FIFOFAST_TEST_H_
#define FIFOFAST_TEST_H_

#include "fifofast_demo.h"
#include "unittrace/unittrace.h"

//////////////////////////////////////////////////////////////////////////
// Function Declarations
//////////////////////////////////////////////////////////////////////////

void fifofast_test_macro_initial(void);
void fifofast_test_macro_write(uint8_t startvalue);
void fifofast_test_macro_peek(uint8_t startvalue);
void fifofast_test_macro_read(uint8_t startvalue);
void fifofast_test_macro_add(uint8_t startvalue);
void fifofast_test_macro_remove_lite(uint8_t startvalue);
void fifofast_test_macro_remove(uint8_t startvalue);
void fifofast_test_macro_rebase(uint8_t startvalue);

void fifofast_test_func_initial(fff_proto_t* fifo);
void fifofast_test_func_write(fff_proto_t* fifo, uint8_t startvalue);
void fifofast_test_func_peek(fff_proto_t* fifo, uint8_t startvalue);
void fifofast_test_func_remove_lite(fff_proto_t* fifo, uint8_t startvalue);
void fifofast_test_func_remove(fff_proto_t* fifo, uint8_t startvalue);

#endif /* FIFOFAST_TEST_H_ */