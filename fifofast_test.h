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

#include "unittrace/unittrace.h"

//////////////////////////////////////////////////////////////////////////
// Function Declarations
//////////////////////////////////////////////////////////////////////////

void fifofast_test_initial(void);
void fifofast_test_write(uint8_t startvalue);
void fifofast_test_peek(uint8_t startvalue);
void fifofast_test_read(uint8_t startvalue);
void fifofast_test_add(uint8_t startvalue);
void fifofast_test_remove_lite(uint8_t startvalue);
void fifofast_test_remove(uint8_t startvalue);


#endif /* FIFOFAST_TEST_H_ */