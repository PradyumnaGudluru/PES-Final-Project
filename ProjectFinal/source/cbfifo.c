/*
 * cbfifo.c
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 */
#if 0
/*******************************************************************************
Header files
*******************************************************************************/
#include "cbfifo.h"


/*******************************************************************************
Macros
*******************************************************************************/
#define ERROR                  					 	(-1)
#define START_CRITICAL_SECTION()					__disable_irq()
#define END_CRITICAL_SECTION(x)						__set_PRIMASK(x)
/*******************************************************************************
Function definitions
*******************************************************************************/
/*******************************************************************************
* @Function Name:cbfifo_enqueue
* @Description: Enqueues data onto the FIFO, up to the limit of the available
* FIFO capacity.
* @input param 1: Pointer to the data
* @input param 2: Max number of bytes to enqueue
* @input param 3: Queue under consideration
* @return: The number of bytes actually enqueued, which could be 0. In case
* of an error, returns -1.
*******************************************************************************/
size_t cbfifo_enqueue(CB_t *cbfifo,void *buf, size_t nbyte)
{
    if(NULL == buf)
    {
        return ERROR;
    }

    if(CBFIFO_SIZE < nbyte)
    {
        nbyte = CBFIFO_SIZE;
    }

    uint16_t NoOfBytesCopied = 0;
    uint16_t l_traverse = 0;
    uint16_t Fifofilled = 0;
    uint32_t mask_state = 0;

    for(l_traverse = 0;l_traverse < nbyte;l_traverse++)
    {
    	/* Read < Write */
        /* Do nothing */

        /* Write < Read */
    	if((cbfifo->read - cbfifo->write) == 1)
        {
            /* Condition where write should not overwrite
             * the existing buffer */
            return NoOfBytesCopied;
        }

    	/* Write == Read */
        if(cbfifo->read == cbfifo->write)
        {
            /* Both read and write are same value */
            Fifofilled = cbfifo_length(cbfifo);

            if((0 == cbfifo->write) && (0 == Fifofilled) && (cbfifo->tracker > 0))
            {
                /* Queue identifed to be full.Hence, return */
                return NoOfBytesCopied;
            }
        }

        mask_state = __get_PRIMASK();
        START_CRITICAL_SECTION();

        cbfifo->cbBuf[cbfifo->write] = *((uint8_t*)(buf+l_traverse));
        cbfifo->tracker++;
        cbfifo->write = (cbfifo->write+1)&(cbfifo->size-1);
        NoOfBytesCopied++;

        END_CRITICAL_SECTION(mask_state);

    }

    return NoOfBytesCopied;
}
/*******************************************************************************
* @Function Name: cbfifo_dequeue
* @Description: Attempts to remove ("dequeue") up to nbyte bytes of data from the
* FIFO. Removed data will be copied into the buffer pointed to by buf.
* @input param 1: Destination for the dequeued data
* @input param 2: Bytes of data requested
* @input param 3: Queue under consideration
* @return: The number of bytes actually copied, which will be between 0 and
* nbyte.
* To further explain the behavior: If the FIFO's current length is 24
* bytes, and the caller requests 30 bytes, cbfifo_dequeue should
* return the 24 bytes it has, and the new FIFO length will be 0. If
* the FIFO is empty (current length is 0 bytes), a request to dequeue
* any number of bytes will result in a return of 0 from
* cbfifo_dequeue.
*******************************************************************************/
size_t cbfifo_dequeue(CB_t *cbfifo,void *buf, size_t nbyte)
{
    if(NULL == buf)
    {
        return ERROR;
    }

    if(nbyte > CBFIFO_SIZE)
    {
        /* Since, size is greater than the
        * buffer capacity truncating*/
        nbyte = CBFIFO_SIZE;
    }

    uint16_t NoOfBytesCopied = 0;
    uint16_t l_traverse = 0;
    uint16_t Fifofilled = 0;
    uint32_t mask_state = 0;

    for(l_traverse = 0;l_traverse < nbyte; l_traverse++)
    {
    	/* Read < Write */
        /* Do nothing */

        /* Write < Read */
        /* Do nothing */

    	/* Write == Read */
        if(cbfifo->read == cbfifo->write)
        {
            /* Both read and write are same value */
            Fifofilled = cbfifo_length(cbfifo);

            if((cbfifo->read == 0) && (Fifofilled == 0) && (cbfifo->tracker == 0))
            {
                /* Empty fifo condition reached */
                return NoOfBytesCopied;
            }
            else if((0 < cbfifo->read) && (0 == Fifofilled) && (0 == cbfifo->tracker))
            {
                /* Empty fifo condition reached */
                return NoOfBytesCopied;
            }
            else
            {
                /* Do Nothing */
            }
        }

        mask_state = __get_PRIMASK();
        START_CRITICAL_SECTION();

        *((uint8_t*)(buf+l_traverse)) = cbfifo->cbBuf[cbfifo->read];
        cbfifo->tracker--;
        NoOfBytesCopied++;
        cbfifo->read = (cbfifo->read+1)&(cbfifo->size-1);

        END_CRITICAL_SECTION(mask_state);
    }

    return NoOfBytesCopied;
}
/*******************************************************************************
* @Function Name:cbfifo_length
* @Description: Returns the number of bytes currently on the FIFO.
* @input param: Queue under consideration
* @return: Number of bytes currently available to be dequeued from the FIFO
*******************************************************************************/
size_t cbfifo_length(CB_t *cbfifo)
{
    /* Optimized as per recommendation */
    return ((cbfifo->write - cbfifo->read) & (cbfifo->size -1));
}
/*******************************************************************************
* @Function Name:cbfifo_capacity
* @Description: This function Returns the FIFO's capacity
* @input param: Queue under consideration
* @return: The capacity, in bytes, for the FIFO
*******************************************************************************/
size_t cbfifo_capacity(CB_t *cbfifo)
{
    /* Only correction made in 1.1 revision */
    return cbfifo->size-1;
}
/*******************************************************************************
* @Function Name:cbfifo_dump_state (PART OF TEST FUNCTIONS )
* @Description: This function shows the visual rep of the Circular buffer
* @input param: Queue under consideration
* @return: None
*******************************************************************************/
void cbfifo_dump_state(CB_t *cbfifo)
{
    printf("---------------DUMP STATE------------------------\r\n");
    uint32_t l_traverse = 0;

    for(l_traverse = 0;l_traverse < CBFIFO_SIZE ;l_traverse++)
    {
        printf("%c",cbfifo->cbBuf[l_traverse]);
    }

    printf("\r\n");
}
/*******************************************************************************
* @Function Name:DiagnosticMessage (PART OF TEST FUNCTIONS )
* @Description: The locations of various pointers.
* @input param: Queue under consideration
* @return: None
*******************************************************************************/
void DiagnosticMessage(CB_t *cbfifo)
{
    printf("---------------DiagnosticMessage------------------------\r\n");
    printf("WRITE = %d\r\nREAD = %d\r\n",cbfifo->write,cbfifo->read);
    printf("LENGTH = %zu\r\n",cbfifo_length(cbfifo));
    printf("TRACKER = %d\r\n",cbfifo->tracker);
    printf("########################################################\r\n\n");
}

/*******************************************************************************
* @Function: cbfifo_full
* @Description:Check if the queue is full
* @input param: Queue under consideration
* @return: yes/no
*******************************************************************************/
int cbfifo_full(CB_t *cbfifo)
{
	/* size -1 is checked as one byte is wasted to
	 * ensure write does not cross read	 */
	if(cbfifo_length(cbfifo) == (CBFIFO_SIZE - 1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*******************************************************************************
* @Function: cbfifo_init
* @Description: Init the queue
* @input param 1: Queue under consideration
* @input param 2: size to init with
* @return: yes/no
*******************************************************************************/
void cbfifo_init(CB_t *cbfifo , size_t size)
{
	cbfifo->size = size;
	cbfifo->read = 0;
	cbfifo->write = 0;
	cbfifo->tracker = 0;
}

#endif

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


