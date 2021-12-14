/*
 * switch.c
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 *   @brief  Has the code for basic functionality of switch GPIO Interrupts.
 * @References 1) Lecture slides of Howdy Pierce
 *  			2) https://github.com/alexander-g-dean/ESF/tree/master/NXP
 */

/* Header File*/
#include "switch.h"

/* MACRO DEFS */
#define SWITCH_PIN								3
#define SWITCH_PIN_OFF							4

/* Variables */
uint8_t switch_flag;

extern int while_running;

acc_param_t mma_acc = {0,0,0,0,0};

/**
 * @prototype : void init_switch()
 * @brief : Initializing GPIO for switch input
 * @parameters : None
 * @return : None
 */
void init_switch()
{
	/* Enable clock source to the pin */
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	/* Setting up port multiplexing for GPIO */
	PORTD->PCR[SWITCH_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[SWITCH_PIN] |= PORT_PCR_MUX(1);

	/* Enable the pull select and pull enable */
	PORTD->PCR[SWITCH_PIN] |= PORT_PCR_PE(1) | PORT_PCR_PS(1);

	/* Enable the interrupt on any edge */
	PORTD->PCR[SWITCH_PIN] |= PORT_PCR_IRQC(10);

	/* Set direction to input */
	GPIOD->PDDR &= ~(1 << SWITCH_PIN);

	/*Select GPIO and enable pull-up resistors and interrupts on falling edge*/
	PORTD->PCR[SWITCH_PIN_OFF] = PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(10);
	/*Make pin input*/
	PTD->PDDR &= ~(1 << SWITCH_PIN_OFF);

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
	switch_flag = 0;

	if ((PORTD->ISFR & (1 << SWITCH_PIN)))
	{
		/* Debounce clearance */
		switch_flag = 0;

		if(	(GPIOD->PDIR & (1 << SWITCH_PIN)) == 0)
		{
			switch_flag = 1;
		}
	}

	else if((PORTD->ISFR & (1 << SWITCH_PIN_OFF))){
		while_running = 0;
		rgb_pwm_controller(0,0,0);
	}

	switch_cmd_process();

	PORTD->ISFR |= (1 << SWITCH_PIN); // Clearing the switch interrupt flag
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

	if((while_running != 1) && (switch_flag == 1)){

		printf("\r\n Initiating Headlamp Assistance \r\n");
		while_running = 1;
	}
	else if((while_running == 1) && (switch_flag == 1)){

		printf("THe values of ROLL and PITCH are: roll = %d, pitch = %d\r\n", (int)roll, (int)pitch);

	}
}
