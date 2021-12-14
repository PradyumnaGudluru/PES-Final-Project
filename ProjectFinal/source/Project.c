/*
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#include <math.h>
#include "sysclock.h"
#include "uart.h"
#include "cbfifo.h"
#include "commandprocessor.h"
#include "systick_timers.h"
#include "switch.h"
#include "led_tmp.h"
#include "i2c.h"
#include "mma8451.h"
#include "statemachine.h"
#include "commandprocessor.h"
#include "cbfifo_testcase.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */


#define	LOADVAL_BUFF_SIZE						50
#define ERROR								-1


int while_running = 0;

static void system_init();
static void app();

/*
* @prototype:  main
* @brief: Starts the application
* @input : None
* @return: None
*/
int main(void)
{
	/* System Initialization */
	system_init();

	/* test the modules */
	test_modules();

	/* Calling the headlamp application */
	app();

	return 0;
}

/*
* @prototype:  system_init
* @brief: Initialization of system
* @input : None
* @return: None
*/
static void system_init()
{
	/* System Clock Initialization */
	init_sysclock();

	/* Systick init */
	init_systick();

	/* TPM init */
	init_tpm();

	/* LED PWM init */
	init_led_pwm();

	/* UART init */
	init_UART0();

	/* switch init */
	init_switch();

	/* I2C module init */
	init_i2c();

	/* Sensor init */
	if (!init_mma())
	{
		/* Light red error LED */
		rgb_pwm_controller(1, 0, 0);
		while (1)
			;
		printf("ERROR initializing accelerometer sensor\r\n");
	}

	printf("Headlamp_Assistance Feature!\r\n");

	reset_timer();
}

/*
* @prototype:  app
* @brief: Application of headlamp assistance starts here
* @input : None
* @return: None
*/
static void app()
{

	int8_t ch;
	bool firstbackspace = true;
	uint8_t loadval[LOADVAL_BUFF_SIZE];

	memset(loadval,'\0',sizeof(loadval));
	uint8_t *val = loadval;

	printf("Press HELP for details\r\n");

	while(1)
	{
		printf("$ ");

		firstbackspace = true;

		while(1)
		{
			ch = getchar();

			if(ch != ERROR)
			{
				putchar(ch);

				if(ch != 0x08)
				{
					firstbackspace = false;
					*val++ = ch;
				}
				else
				{
					/* for backspace */
					if(firstbackspace == false)
					{
						printf(" \b");
						*val-- = ch;
					}
					else
					{
						putchar(' ');
					}
				}

				if(ch == '\r')
				{
					*val++ = '\n';
					*val++ = '\0';
					val = &loadval[0];
					ch = -1;
					break;
				}
			}


			/* Enters the state machine only when initiated by switch or command line */
			if(while_running == 1)
			state_color();
		}

		val = &loadval[0];

		/* Lexical analysis for the command */
		lexical_analysis(loadval);


	}

}
