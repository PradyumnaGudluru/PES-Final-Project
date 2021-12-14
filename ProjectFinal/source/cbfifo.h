/*
 * cbfifo.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 */

#ifndef CBFIFO_H_
#define CBFIFO_H_

/*******************************************************************************
Header files
*******************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "MKL25Z4.h"
/*******************************************************************************
Macros
*******************************************************************************/
#define CBFIFO_SIZE             		(256)
/*******************************************************************************
Structures
*******************************************************************************/
typedef struct sCircularBuffer
{
    uint8_t cbBuf[CBFIFO_SIZE];       /* Circular buffer in place */
    uint16_t size;                    /* Size in power of 2 */
    uint16_t read;                    /* Read position 0 to size-1 */
    uint16_t write;                   /* Write position 0 to size-1 */
    int16_t tracker;                  /* To track the position */
}CB_t;
#if 0
/*******************************************************************************
Function declarations
*******************************************************************************/
void cbfifo_init(CB_t *cbfifo , size_t size);
size_t cbfifo_enqueue(CB_t *cbfifo,void *buf, size_t nbyte);
size_t cbfifo_dequeue(CB_t *cbfifo,void *buf, size_t nbyte);
size_t cbfifo_length(CB_t *cbfifo);
size_t cbfifo_capacity(CB_t *cbfifo);
void cbfifo_dump_state(CB_t *cbfifo);
void DiagnosticMessage(CB_t *cbfifo);
int cbfifo_full(CB_t *cbfifo);
#endif
/*
 * cb_fifo.h
 *
 * Created on: 10-Nov-2021
 * Author: Pradyumna
 * @brief  Has the code for circular buffer with 256 bytes.
 * @References 1) Embedded Systems Fundamentals with ARm Cortex-M based Microcontrollers by Alexander G. Dean
 * 			   2) Lecture Slides of Howdy Pierce
 */


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
