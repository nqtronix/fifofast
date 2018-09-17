/*
 * fifofast.c
 *
 * Created: 11.11.2017 14:20:46
 *  Author: Dennis aka nqtronix (github.com/nqtronix)
 */ 

#include "fifofast.h"


extern inline uint8_t	fff_wrap(fff8_t *fifo, uint8_t idx) __attribute__((__always_inline__));
extern inline uint8_t	fff_data_size(fff8_t *fifo) __attribute__((__always_inline__));

extern inline uint8_t	fff_is_empty(fff8_t *fifo) __attribute__((__always_inline__));
extern inline uint8_t	fff_is_full(fff8_t *fifo) __attribute__((__always_inline__));
extern inline uint8_t	fff_mem_used(fff8_t *fifo) __attribute__((__always_inline__));
extern inline uint8_t	fff_mem_free(fff8_t *fifo) __attribute__((__always_inline__));

extern inline void		fff_flush(fff8_t *fifo) __attribute__((__always_inline__));
extern inline void		fff_remove(fff8_t *fifo, uint8_t amount) __attribute__((__always_inline__));
extern inline void		fff_write(fff8_t *fifo, void *data) __attribute__((__always_inline__));
extern inline void		fff_write_safe(fff8_t *fifo, void *data) __attribute__((__always_inline__));

extern inline void*		fff_peek_read(fff8_t *fifo, uint8_t idx) __attribute__((__always_inline__));
extern inline void		fff_peek_write(fff8_t *fifo, uint8_t idx, void *data) __attribute__((__always_inline__));