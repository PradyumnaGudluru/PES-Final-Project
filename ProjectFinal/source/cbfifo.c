/*
 * cb_fifo.c
 *
 * Created on: 10-Nov-2021
 * Author: Pradyumna
 * @brief  Has the code for circular buffer with 256 bytes.
 * @References : 1) Embedded Systems Fundamentals with ARm Cortex-M based Microcontrollers by Alexander G. Dean
 * 				 2) Lecture Slides of Howdy Pierce
 */
#include "cbfifo.h"
#include "MKL25Z4.h"

/**
 * @prototype    cbfifo_init()
 * @brief   This function gets the initialization done for the cbfifo.
 */
void cbfifo_init(cb_fifo_t * cb_fifo)
{

	for (int i=0; i<STRIDE; i++)
	{
		cb_fifo->cb_data[i] = '_';
	}
	cb_fifo->write = 0;
	cb_fifo->read = 0;
	cb_fifo->full = false;
	cb_fifo->length = 0;

}

/**
 * @prototype    cbfifo_capacity()
 * @brief   This function takes the input of cbfifo for an instance and returns the maximum capacity
 * 			available with the instance of FIFO.
 */
size_t cbfifo_capacity(cb_fifo_t * cb_fifo){

	return STRIDE;
}

/**
 * @prototype    cbfifo_empty()
 * @brief   This function takes the input of cbfifo for an instance and returns the empty condition of the FIFO.
 */
bool cbfifo_empty(cb_fifo_t * cb_fifo){

	return ((cb_fifo->write == cb_fifo->read)&&(!cb_fifo->full));
}

/**
 * @prototype    cbfifo_size()
 * @brief   This function takes the input of cbfifo for an instance and returns the size of the FIFO.
 */
int cbfifo_size(cb_fifo_t * cb_fifo){

	return cb_fifo->length;
}

/**
 * @prototype    cbfifo_length()
 * @brief   This function takes the input of cbfifo for an instance and returns the computed length of the FIFO.
 */
size_t cbfifo_length(cb_fifo_t * cb_fifo){

	size_t size_len=0;

	if(cb_fifo->full){
		size_len = STRIDE;
	}
	else if(cb_fifo->write >= cb_fifo->read){
		size_len = cb_fifo->write - cb_fifo->read;
	}
	else{
		size_len = STRIDE - (cb_fifo->read - cb_fifo->write);
	}

	return size_len;
}

/**
 * @prototype    cbfifo_full()
 * @brief   This function takes the input of cbfifo for an instance and returns the full condition of the FIFO.
 */
bool cbfifo_full(cb_fifo_t * cb_fifo){

	return (cbfifo_length(cb_fifo) == STRIDE);
}

/**
 * @prototype    cbfifo_enqueue()
 * @brief   This function takes the input of cbfifo for an instance with the buffer to be enqueued and the size of the buffer
 *  and returns the number of bytes enqueued to the FIFO.
 */
size_t cbfifo_enqueue(cb_fifo_t * cb_fifo,  void *buf , size_t nbyte){
 uint32_t mask;
	if(buf==NULL){
	     return (size_t)-1;
	}


	if(cb_fifo->full){
	    return 0;
	}


	//int result;
	  unsigned char *enq_buf=(unsigned char*)buf;
	  size_t enqueued=0;

	  for(int i=0;i<nbyte; i++){
	    // result=f->full;

	     if(cb_fifo->full){
	       break;
	     }

	     else{

	      cb_fifo->cb_data[cb_fifo->write]=*enq_buf++;
	      //src++;

	      mask = __get_PRIMASK(); // protecting operation from preemption; saving the mask value
	      __disable_irq(); // Disabling the interrupts

	      cb_fifo->write++;
	      enqueued++;

	      __set_PRIMASK(mask); // After completion of update, enabling the interrupt masking
	      //i++;
	      cb_fifo->write=(cb_fifo->write)%STRIDE;
	      if(cb_fifo->write==cb_fifo->read){
	    	  cb_fifo->full=1;
	      }
	     }

	  }

	cb_fifo->length=cb_fifo->length+enqueued;

	return enqueued;

}

/**
 * @prototype    cbfifo_dequeue()
 * @brief   This function takes the input of cbfifo for an instance with the buffer to be dequeued into and the
 * 			size of the buffer and returns the number of bytes dequeued to the FIFO.
 */
size_t cbfifo_dequeue(cb_fifo_t * cb_fifo, void *buf , size_t nbyte){
	uint32_t mask;
    if(buf==NULL){
      return (size_t)-1;
    }

    unsigned char *deq_buf=(unsigned char*)buf;

   //int result;

   size_t dequeued=0;

   for(int i=0; i<nbyte; i++){
       //result=fifo_Empty(f);

      if(cbfifo_empty(cb_fifo)){
       cb_fifo->read=0;
       cb_fifo->write=0;
        break;
      }

     else{
        *deq_buf++=cb_fifo->cb_data[cb_fifo->read];
        //deq_buf++;

        mask = __get_PRIMASK(); // protecting operation from preemption; saving the mask value
       __disable_irq(); // Disabling the interrupts

        cb_fifo->read++;
        dequeued++;

        __set_PRIMASK(mask); // After completion of update, enabling the interrupt masking

        //i++;
        cb_fifo->read=(cb_fifo->read)%STRIDE;
        cb_fifo->full=0;
      }
   }

cb_fifo->length=cb_fifo->length - dequeued;


return dequeued;

}


