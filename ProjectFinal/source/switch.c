/*
 * switch.c
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 */


#include "switch.h"

#define SWITCH_GPIO_PORT 						GPIOD
#define SWITCH_PIN								3
#define SWITCH_PIN_CTRL_REG						PORTD->PCR[SWITCH_PIN] /* PORTD PCR 3*/
#define SWITCH_SCGC5_MASK						SIM_SCGC5_PORTD_MASK
#define SWITCH_DATA_DIR_REG 					SWITCH_GPIO_PORT->PDDR
#define SWITCH_DATA_IN_REG						SWITCH_GPIO_PORT->PDIR
#define SWITCH_IFSR_REG							PORTD->ISFR

uint8_t switch_press_detected;

extern int while_running;

acclerometer_parameters_t mma_acc = {0,0,0,0,0,0};

/**
 * @prototype : void init_switch()
 * @brief : Initializing GPIO for switch input
 * @parameters : None
 * @return : None
 */
void init_switch()
{
	/* Enable clock source to the pin */
	SIM->SCGC5 |= SWITCH_SCGC5_MASK;

	/* Setting up port multiplexing for GPIO */
	SWITCH_PIN_CTRL_REG &= ~PORT_PCR_MUX_MASK;
	SWITCH_PIN_CTRL_REG |= PORT_PCR_MUX(1);

	/* Enable the pull select and pull enable */
	SWITCH_PIN_CTRL_REG |= PORT_PCR_PE(1) | PORT_PCR_PS(1);

	/* Enable the interrupt on any edge */
	SWITCH_PIN_CTRL_REG |= PORT_PCR_IRQC(11);

	/* Set direction to input */
	SWITCH_DATA_DIR_REG &= ~(1 << SWITCH_PIN);

	/* Set the priroity of switch */
	NVIC_EnableIRQ(PORTD_IRQn);

}
/**
 * @prototype : void PORTD_IRQHandler(void)
 * @brief : IRQ Handler for PORTD (GPIO PD3) pin
 * @parameters : None
 * @return : None
 */
void PORTD_IRQHandler(void)
{
	switch_press_detected = 0;

	if ((SWITCH_IFSR_REG & (1 << SWITCH_PIN)))
	{
		/* Debounce clearance */
		switch_press_detected = 0;

		if(	(SWITCH_DATA_IN_REG & (1 << SWITCH_PIN)) == 0)
		{
			switch_press_detected = 1;
		}
	}

	switch_cmd_process();

	SWITCH_IFSR_REG |= (1 << SWITCH_PIN); // Clearing the switch interrupt flag
}

/**
 * @prototype : void switch_cmd_process()
 * @brief : When switch is pressed, the headlamp assistance option is initiated or the roll/pitch values are displayed.
 * @parameters : None
 * @return : None
 */
void switch_cmd_process()
{
	read_full_xyz(&mma_acc);
	convert_xyz_to_roll_pitch(&mma_acc, 0);

	float roll = mma_acc.roll_val;
	float pitch = mma_acc.pitch_val;

	if((while_running != 1) && (switch_press_detected == 1)){

		printf("\r\n Initiating Headlamp Assistance \r\n");
		while_running = 1;
	}
	else if((while_running == 1) && (switch_press_detected == 1)){

		printf("THe values of ROLL and PITCH are: roll = %d, pitch = %d\r\n", (int)roll, (int)pitch);

	}
}
