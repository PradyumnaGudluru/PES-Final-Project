/*
 * cb_fifo.h
 *
 * Created on: 10-Nov-2021
 * Author: Pradyumna
 * @brief  Has the code for circular buffer with 256 bytes.
 * @References 1) Embedded Systems Fundamentals with ARm Cortex-M based Microcontrollers by Alexander G. Dean
 * 			   2) Lecture Slides of Howdy Pierce
 */

#ifndef CBFIFO_H_
#define CBFIFO_H_

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define STRIDE 256

typedef struct {
	size_t cb_data[STRIDE];
	int read, write;
	int max;
	bool full;
	int length;
}cb_fifo_t;


void cbfifo_init(cb_fifo_t * f);



 size_t cbfifo_enqueue(cb_fifo_t * f, void *buf , size_t nbyte);



 size_t cbfifo_dequeue(cb_fifo_t * f, void *buf , size_t nbyte);


bool cbfifo_full(cb_fifo_t * f);

size_t cbfifo_capacity(cb_fifo_t * cb_fifo);

int cbfifo_size(cb_fifo_t * f);

size_t cbfifo_length(cb_fifo_t * cb_fifo);


#endif /* CBFIFO_H_ */
