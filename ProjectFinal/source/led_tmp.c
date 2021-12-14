/*
 * led_tmp.c
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 * @brief  Has the code for basic functionality of LED by TPM LED Switching ON functions.
 * @References 1) Lecture slides of Howdy Pierce
 *  			2) https://github.com/alexander-g-dean/ESF/tree/master/NXP
 */



#include "led_tmp.h"

/* TPM related initialization */
#define RED_LED_CHANNEL							0
#define GREEN_LED_CHANNEL						1
#define BLUE_LED_CHANNEL						1

/* RED LED configuration */
#define RED_LED_PIN								18

/* GREEN LED configuration */
#define GREEN_LED_PIN							19

/* BLUE LED configuration */
#define BLUE_LED_PIN							1


static void init_tpm0(uint32_t period);
static void init_tpm2(uint32_t period);
static void init_led(uint8_t led_channel);

/**
 * @prototype : void init_led_pwm()
 * @brief : initializing LED PWM
 * @parameters : None
 * @return: None
 */
void init_led_pwm()
{
	/* Send the Clock to the GPIO Port for Red, Green and Blue LED */
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

	/* Set the appropriate mux for the pwm functionality */
	init_led(RED_LED_PIN);
	init_led(GREEN_LED_PIN);
	init_led(BLUE_LED_PIN);
}

/**
 * @prototype : void init_tpm()
 * @brief : initializing TPM
 * @parameters : None
 * @return: None
 */
void init_tpm()
{
	/* Configure TPM-0 and TPM-2 */
	SIM->SCGC6 |= (SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK);

	/* Set clock source for tpm: 24 MHz */
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

	/* Init tpm - 0 specific initialization */
	init_tpm0(PWM_PERIOD);

	/* Init tpm - 2 specific initialization */
	init_tpm2(PWM_PERIOD);
}

/**
 * @prototype : static void init_tpm0(uint32_t period)
 * @brief : initializing TPM0
 * @parameters : Takes input period value
 * @return: None
 */
static void init_tpm0(uint32_t period)
{
	/* Load the counter and mod */
	TPM0->MOD = period;

	/* Set TPM count direction to up with a divide by 2 pre-scaler */
	TPM0->SC = TPM_SC_PS(1);

	/* Continue operation in debug mode */
	TPM0->CONF |= TPM_CONF_DBGMODE(3);

	/* Set channel 1(BLUE) to edge-aligned low-true PWM */
	TPM0->CONTROLS[BLUE_LED_CHANNEL].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);

	/* Set initial duty cycle to 0  (OFF)*/
	TPM0->CONTROLS[BLUE_LED_CHANNEL].CnV = 0;

	/* Start TPM-0 */
	TPM0->SC |= TPM_SC_CMOD(1);
}

/**
 * @prototype : static void init_tpm2(uint32_t period)
 * @brief : initializing TPM2
 * @parameters : Takes input period value
 * @return: None
 */
static void init_tpm2(uint32_t period)
{
	/* Load the counter and mod */
	TPM2->MOD = period;

	/* Set TPM count direction to up with a divide by 2 pre-scaler */
	TPM2->SC =  TPM_SC_PS(1);

	/* Continue operation in debug mode */
	TPM2->CONF |= TPM_CONF_DBGMODE(3);

	/* Set channel 0 (RED) to edge-aligned low-true PWM */
	TPM2->CONTROLS[RED_LED_CHANNEL].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);

	/* Set initial duty cycle to 0  (OFF)*/
	TPM2->CONTROLS[RED_LED_CHANNEL].CnV = 0;

	/* Set channel 1(GREEN) to edge-aligned low-true PWM */
	TPM2->CONTROLS[GREEN_LED_CHANNEL].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);

	/* Set initial duty cycle to 0 (OFF) */
	TPM2->CONTROLS[GREEN_LED_CHANNEL].CnV = 0;

	/* Start TPM-2 */
	TPM2->SC |= TPM_SC_CMOD(1);
}

/**
 * @prototype : void rgb_pwm_controller(uint8_t red_pwm, uint8_t green_pwm, uint8_t blue_pwm)
 * @brief : PWM working of RGB color
 * @parameters : Takes input as red, green and blue values
 * @return: None
 */
void rgb_pwm_controller(uint8_t red_pwm, uint8_t green_pwm, uint8_t blue_pwm)
{
	TPM2->CONTROLS[RED_LED_CHANNEL].CnV = red_pwm;
	TPM2->CONTROLS[GREEN_LED_CHANNEL].CnV = green_pwm;
	TPM0->CONTROLS[BLUE_LED_CHANNEL].CnV = blue_pwm;
}

/**
 * @prototype : static void init_led(uint8_t led_channel)
 * @brief : Initialize LED for Red, Green and Blue
 * @parameters : Takes input as LED channel
 * @return: None
 */
static void init_led(uint8_t led_channel)
{
	switch(led_channel)
	{
		case RED_LED_PIN:
		{
			/* Set up Port Multiplexing as it is needed
			 * for PWM feature  for RED LED */
			PORTB->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
			PORTB->PCR[RED_LED_PIN] |= PORT_PCR_MUX(3);
			break;
		}
		case GREEN_LED_PIN:
		{
			/* Set up Port Multiplexing as it is needed
			 * for PWM feature  for GREEN LED */
			PORTB->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
			PORTB->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(3);
			break;
		}
		case BLUE_LED_PIN:
		{
			/* Set up Port Multiplexing as it is needed
			 * for PWM feature  for BLUE LED */
			PORTD->PCR[BLUE_LED_PIN] &= ~PORT_PCR_MUX_MASK;
			PORTD->PCR[BLUE_LED_PIN] |= PORT_PCR_MUX(4);
			break;
		}
		default:
		{
			break;
		}
	}
}
