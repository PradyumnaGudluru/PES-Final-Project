/*
 * uart.c
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 * @brief  Has the code for UART initialization and terminal calling function.
 * @References 1) Lecture slides of Howdy Pierce
 * 			   2) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Misc/Touch%20Sense/TSI/src
 */

#include "uart.h"

#define UART_OVERSAMPLE_RATE 						(16)
#define BAUD_RATE									(38400)		/* sets the baud rate */
#define STOP_BIT									(1)			/* Ensures 2 stop bits */
#define PARITY										(0)			/* No parity */
#define DATA_SIZE									(0)			/* Data size is 8 */

cb_fifo_t CB_TX_FIFO, CB_RX_FIFO;

/* BEGIN - UART0 Device Driver
	Code created by Shannon Strutz
	Date : 5/7/2014
	Licensed under CC BY-NC-SA 3.0
	http://creativecommons.org/licenses/by-nc-sa/3.0/
	Modified by Alex Dean 9/13/2016
*/
/**
 * @prototype : __sys_write(int handle, char *buf, int size)
 * @brief : sys write functionality - enqueues data from UART
 * @parameters : Takes the handle character and size as input
 * @return : success/ failure status
 */
int __sys_write(int handle, char *buf, int size)
{
	if(buf == NULL)
	{
		return -1;
	}

	/* Check for the queue to be empty */
	while(cbfifo_full(&CB_TX_FIFO))
	{
		;
	}

	cbfifo_enqueue(&CB_TX_FIFO,buf,size);

	/* enable the transmit interrupt */
	if(!(UART0->C2 & UART0_C2_TIE_MASK))
	{
		UART0->C2 |= UART0_C2_TIE(1);
	}

	return 0;
}

/**
 * @prototype :  __sys_readc(void)
 * @brief : sys read functionality - dequeues data from UART
 * @parameters : None
 * @return : data on successful dequeue
 */
int __sys_readc(void)
{
	char data_byte;

	/* dequeue one byte at a time */
	if(cbfifo_dequeue(&CB_RX_FIFO,&data_byte,1) != 1)
	{
		return -1;
	}
	else
	{
		return data_byte;
	}
}


/**
 * @prototype :  init_UART0()
 * @brief : This function gets the initialization done for the UART0.
 * @parameters : None
 * @return : none
 */
void init_UART0()
{
	uint16_t sbr;

	/* Enable clock gating for UART0 and Port A */
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	/* Make sure transmitter and receiver are disabled before initialization */
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	/* Set UART clock to 24 MHz clock */
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);

	/* Set pins to UART0 Rx and Tx */
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	/* Set baud rate and over-sampling ratio */
	sbr = (uint16_t)((SYSCLOCK_FREQUENCY)/(BAUD_RATE * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	/* select two stop bit */
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOP_BIT) | UART0_BDH_LBKDIE(0);

	/* Don't enable loopback mode, use 8 data bit mode, don't use parity */
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(DATA_SIZE) | UART0_C1_PE(PARITY) | UART0_C1_PT(0);

	/* Don't invert transmit data, don't enable interrupts for errors */
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	/* Clear error flags */
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	/* Send LSB first, do not invert received data */
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	/*Buffer Init & enable UART interrupt*/
		cbfifo_init(&CB_TX_FIFO);
		cbfifo_init(&CB_RX_FIFO);

	/* Enable UART-0 interrupt */
	NVIC_EnableIRQ(UART0_IRQn);

	/* Enable receive interrupts but not transmit interrupts yet */
	UART0->C2 |= UART_C2_RIE(1);

	/* Enable UART receiver and transmitter */
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

}

/* END - UART0 Device Driver
	Code created by Shannon Strutz
	Date : 5/7/2014
	Licensed under CC BY-NC-SA 3.0
	http://creativecommons.org/licenses/by-nc-sa/3.0/
	Modified by Alex Dean 9/13/2016
*/
/**
 * @prototype    UART0_IRQHandler()
 * @brief   This function gets the Interrupt Request at the time of UART serial input or output to be printed to the terminal.
 * 			This updates the cbfifo enqueue and dequeue at respective intervals.
 */
void UART0_IRQHandler(void)
{
	uint8_t ch;

	/* Check for any errors in the interrupt sequence */
	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK |
		UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
		/* Clear error flags */
		UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

		/* read the data register to clear RDRF */
		ch = UART0->D;
	}

	/* Receiver interrupt checked */
	if (UART0->S1 & UART0_S1_RDRF_MASK)
	{
		/* received a character from the interrupt */
		ch = UART0->D;

		/* Store it in the buffer */
		cbfifo_enqueue(&CB_RX_FIFO,&ch,1);
	}

	/* Transmitter interrupt enabled */
	if ( (UART0->C2 & UART0_C2_TIE_MASK) &&
			(UART0->S1 & UART0_S1_TDRE_MASK) )
	{
		if(cbfifo_dequeue(&CB_TX_FIFO,&ch,1))
		{
			/* transmit the byte */
			UART0->D = ch;
		}
		else
		{
			/* queue is empty so disable transmitter interrupt */
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}

void Terminal()
{

	char loadval[640];
	char* val = &loadval[0];

	uint8_t char_in;
	while(char_in != 0x0D){
		while (cbfifo_size(&CB_RX_FIFO) == 0);
		cbfifo_dequeue(&CB_RX_FIFO, &char_in, 1);
		putchar(char_in);
		if (char_in != 0x0D || char_in != 0x0A){
			if(char_in != 0x08){
				*val = (char)char_in;
				val++;
			}
			else{
				printf(" \b");
				val--;

			}
		}

		if (!(UART0->C2 & UART0_C2_TIE_MASK)){
			UART0->C2 |= UART0_C2_TIE(1);
		}
		if(char_in == '\r'){
			char_in = 0x0A;
			printf("\r\n");
			break;
		}

	}
	*val = '\0';

	lexical_cmdprocess(loadval);
}
