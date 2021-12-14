/*
 * i2c.c
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 *  @brief  Has the code for basic functionality of I2C with read and write functions.
 *  @References 1) Lecture slides of Howdy Pierce
 *  			2) https://github.com/alexander-g-dean/ESF/tree/master/NXP
 */

/* Header File*/
#include <MKL25Z4.H>
#include "i2c.h"

/*MACRO DEFS*/
#define I2C_M_START 						I2C0->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP  						I2C0->C1 &= ~I2C_C1_MST_MASK
#define I2C_M_RSTART 						I2C0->C1 |= I2C_C1_RSTA_MASK

#define I2C_TRAN							I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC								I2C0->C1 &= ~I2C_C1_TX_MASK

#define BUSY_ACK 	    					while(I2C0->S & 0x01)
#define TRANS_COMP							while(!(I2C0->S & 0x80))
#define I2C_WAIT 							i2c_wait();

#define NACK 	        					I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK           						I2C0->C1 &= ~I2C_C1_TXAK_MASK

#define ONBOARD_ACCELEROMETER_SCL			24
#define ONBOARD_ACCELEROMETER_SDA			25

#define RESET								0
#define SET									1

/*Variable*/
int g_lock_detect = 0;

/**
 * @prototype : void init_i2c(void)
 * @brief : initializing I2C module
 * @parameters : None
 * @return: None
 */
void init_i2c(void)
{
	/* Send clock to the i2c peripheral */
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	/* Set pins for I2c functionality with appropriate
	 * mux for i2c functionality  */
	PORTE->PCR[ONBOARD_ACCELEROMETER_SCL] |= PORT_PCR_MUX(5);
	PORTE->PCR[ONBOARD_ACCELEROMETER_SDA] |= PORT_PCR_MUX(5);

	/* Set to 100K baud rate
	 * Multiplication factor is set to one */
 	I2C0->F = (I2C_F_ICR(0x10) | I2C_F_MULT(0));

	/* Enables the I2C module */
	I2C0->C1 |= (I2C_C1_IICEN_MASK);

	/* Select high drive mode */
	I2C0->C2 |= (I2C_C2_HDRS_MASK);
}

/**
 * @prototype : void i2c_busy(void)
 * @brief : I2C busy function
 * @parameters : None
 * @return: None
 */
void i2c_busy(void)
{
	/* Start Signal transmission */
	g_lock_detect = RESET;

	/* Disables the I2c communication */
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;

	i2c_start();

	/* Enables the I2c communication */
	I2C0->C1 |=  I2C_C1_IICEN_MASK;

	/* Set MASTER mode and
	 * Set transmit (TX) mode */
	I2C0->C1 |= I2C_C1_MST_MASK;
	I2C0->C1 |= I2C_C1_TX_MASK;

	/* The data to be transmitted is written in this register */
	I2C0->D = 0xFF;

	while ((I2C0->S & I2C_S_IICIF_MASK) == 0U)
	{
		 /* wait for interrupt to indicate
		  * that the data is sent on the bus */
	}

	/* clear interrupt bit in the status register
	 * after sending data */
	I2C0->S |= I2C_S_IICIF_MASK;

	/* Clear arbitration error flag*/
	I2C0->S |= I2C_S_ARBL_MASK;

	/* Send start */
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;

	/* Set MASTER mode and
	 * Set transmit (TX) mode */
	I2C0->C1 |= I2C_C1_TX_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;

	/* Enables the I2c communication */
	I2C0->C1 |= I2C_C1_IICEN_MASK;

	/* Wait until start is send
	 * Send stop and disable
	 * I2C communication */
	I2C0->C1 &= ~I2C_C1_IICEN_MASK;
	I2C0->C1 |= I2C_C1_MST_MASK;

	/* set SLAVE mode
	 * set Rx */
	I2C0->C1 &= ~I2C_C1_MST_MASK;
	I2C0->C1 &= ~I2C_C1_TX_MASK;

	/* Enable I2c communication */
	I2C0->C1 |= I2C_C1_IICEN_MASK;

	/* Clear interrupt flag*/
	I2C0->S |= I2C_S_IICIF_MASK;

	/* Clear arbitration error */
	I2C0->S |= I2C_S_ARBL_MASK;

	g_lock_detect = RESET;
}

/**
 * @prototype : void i2c_wait(void)
 * @brief : I2C wait function till the transfer completes and ack receives.
 * @parameters : None
 * @return: None
 */
void i2c_wait(void)
{
	g_lock_detect = 0;

	/* A counter of 200 is chosen till the bus
	 * is considered to be free. Once the 200 mark crosses,
	 * the bus is declared to be busy */
	while(((I2C0->S & I2C_S_IICIF_MASK) == 0) & (g_lock_detect < 200))
	{
		/* increment as the pending interrupt is accumulating */
		g_lock_detect++;
	}

	/* declare busy busy as the pending interrupt count has
	 * crossed 200 */
	if (g_lock_detect >= 200)
	{
		i2c_busy();
	}

	/* write 1 to clear the interrupt pending flag */
	I2C0->S |= I2C_S_IICIF_MASK;
}

/**
 * @prototype : void i2c_start()
 * @brief : I2C transfer start function
 * @parameters : None
 * @return: None
 */
void i2c_start()
{
	/*set to transmit mode */
	I2C_TRAN;

	/*send start bit */
	I2C_M_START;
}

/**
 * @prototype : void i2c_read_setup(uint8_t dev, uint8_t address)
 * @brief : I2C data setup for register and data
 * @parameters : data and the register address
 * @return: None
 */
void i2c_read_setup(uint8_t dev, uint8_t address)
{
	/* send dev address to request data
	 * from the device under consideration	*/
	I2C0->D = dev;
	/* wait for the transfer to
	 * complete on the bus*/
	I2C_WAIT

	/* send read address	*/
	I2C0->D =  address;
	/* wait for the transfer to
	 * complete on the bus*/
	I2C_WAIT

	/*repeated start */
	I2C_M_RSTART;

	/* send 7-bit dev address and set
	 * 8th bit to be read mode indication */
	I2C0->D = (dev|0x1);
	/* wait for the transfer to
	 * complete on the bus*/
	I2C_WAIT

	/* set to receive mode as
	 * it will require to now
	 * read data from the device once setup
	 * is complete  */
	I2C_REC;

}

/**
 * @prototype : uint8_t i2c_repeated_read(uint8_t isLastRead)
 * @brief : read a byte from the I2C
 * @parameters :  the device address to be sent for communication
 * @return: byte data read
 */

uint8_t i2c_repeated_read(uint8_t isLastRead)
{
	uint8_t data;

	g_lock_detect = 0;

	/* only for the last byte
	 * read a NACK has to be sent
	 * otherwise rest of the data
	 * received from the device
	 * a ACK has to be sent */
	if(isLastRead)
	{
		/* set NACK after read	*/
		NACK;
	}
	else
	{
		/* ACK after read	*/
		ACK;
	}

	/* Dummy read to clear register */
	data = I2C0->D;

	/* Wait for read completion */
	I2C_WAIT

	/* check if it is the last
	 * read byte */
	if(isLastRead)
	{
		/* a stop has to be sent
		 * before teh last byte is read */
		I2C_M_STOP;
	}

	/*read data	*/
	data = I2C0->D;

	return  data;
}

/**
 * @prototype : uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
 * @brief : reading a single byte from the address
 * @parameters :  The device address and the register address
 * @return: byte data read
 */
uint8_t i2c_read_byte(uint8_t dev, uint8_t address)
{
	uint8_t data;

	/* send start bit s*/
	i2c_start();

	/*send dev address	*/
	I2C0->D = dev;
	/*wait for completion */
	I2C_WAIT

	/*send read address	*/
	I2C0->D =  address;
	/*wait for completion */
	I2C_WAIT

	/*repeated start */
	I2C_M_RSTART;
	/*send dev address (read)	*/
	I2C0->D = (dev|0x1);
	/*wait for completion */
	I2C_WAIT

	/*set to receive mode */
	I2C_REC;
	 /*set NACK after read	*/
	NACK;

	/*dummy read */
	data = I2C0->D;
	/*wait for completion */
	I2C_WAIT

	/*send stop	*/
	I2C_M_STOP;
	/*read data	*/
	data = I2C0->D;

	return data;
}

/**
 * @prototype : void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
 * @brief : writing a single byte of data to the device
 * @parameters :  The device address and the register address
 * @return: byte data read
 */

void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data)
{
	i2c_start();

	/*send dev address	*/
	I2C0->D = dev;
	/*wait for ack */
	I2C_WAIT

	/*send write address	*/
	I2C0->D =  address;
	/*wait for ack */
	I2C_WAIT

	/*send data	*/
	I2C0->D = data;
	/*wait for ack */
	I2C_WAIT

	/*send stop bit */
	I2C_M_STOP;

}
