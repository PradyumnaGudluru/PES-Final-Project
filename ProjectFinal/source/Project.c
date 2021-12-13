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
#include "msec_timers.h"
#include "switch.h"
#include "touch_slider.h"
#include "led_tmp.h"
#include "i2c.h"
#include "mma8451.h"
#include "statemachine.h"
#include "commandprocessor.h"
#include "testmodules.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */


#define	ACCUM_BUFF_SIZE						50
#define ERROR								-1

//static eDigitalAngleGaugeState current_state;

int while_running = 0;

static void system_init();
static void app();
void digital_angle_gauge_state_machine();
//void state_change_detector(eDigitalAngleGaugeState current_state,eDigitalAngleGaugeState new_state);



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
	//test_modules();

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

	/* TSI init */
	init_touchsensor();

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
	printf("Press HELP for details\r\n");
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

	int8_t ch1;
	bool firstbackspace = true;
	uint8_t accumulate[ACCUM_BUFF_SIZE];

	memset(accumulate,'\0',sizeof(accumulate));
	uint8_t *bufptr = accumulate;

	//printf("Please finalize your ground reference to 0 by trying to align FRDM parallel to ground\r\n");
	//printf("Once aligned the LED will change to green and then go off!\r\n");

	//current_state = INIT_STATE;

	while(1)
	{
		printf("$ ");

		firstbackspace = true;

		while(1)
		{
			ch1 = getchar();

			if(ch1 != ERROR)
			{
				putchar(ch1);

				if(ch1 != 0x08)
				{
					firstbackspace = false;
					*bufptr++ = ch1;
				}
				else
				{
					/* for backspace */
					if(firstbackspace == false)
					{
						printf(" \b");
						*bufptr-- = ch1;
					}
					else
					{
						putchar(' ');
					}
				}

				if(ch1 == '\r')
				{
					*bufptr++ = '\n';
					*bufptr++ = '\0';
					bufptr = &accumulate[0];
					ch1 = -1;
					break;
				}
			}

			/* start scanning for touch sensor
			 * detection
			start_touch_scanning();*/

			/* Enters the state machine only when initiated by switch or command line */
			if(while_running == 1)
			state_color();
		}

		bufptr = &accumulate[0];

		/* Lexical analysis for the command */
		lexical_analysis(accumulate);


	}

}



#if 0
/*******************************************************************************
* @Function name: traffic_light_state
* @Description: main code which handles all the state transactions
* @input param: none
* @return: none
* @references: elevator.c examples
*******************************************************************************/
void digital_angle_gauge_state_machine()
{
	eDigitalAngleGaugeState new_state;

	new_state = current_state;

	switch (current_state)
	{
		case INIT_STATE:
			new_state = handle_calibrate_to_zero();
			break;

		case CALIBRATE_STATE:
			new_state = handle_calibrate_state();
			break;

		case ANGLE_DISPLAY_STATE:
			new_state = handle_angle_gauge_state();
			break;

		default:
			break;
	}

	if (new_state != current_state)
	{
		/* Only when a new state is detected */
		state_change_detector(current_state,new_state);

		current_state = new_state;
	}
}

/*******************************************************************************
* @Function name: state_change_detector
* @Description: Logs the sequence of states in which the state machine is executed
* @input param 1: current state of the machine
* @input param 2: current state of the machine
* @return: none
* @references: elevator.c examples
*******************************************************************************/
void state_change_detector(eDigitalAngleGaugeState current_state,eDigitalAngleGaugeState new_state)
{
	char buf[300];
	uint8_t calibreate = 0;
	uint8_t angle_display = 0;

	const char *current_str;
	const char *next_str;

	/* executing through below sequence to track
	 *  the state machine flow */
	switch(current_state)
	{
		case INIT_STATE:
		{
			current_str = "INIT_STATE";
			break;
		}
		case CALIBRATE_STATE:
		{
			current_str = "CALIBRATE_STATE";
			break;
		}
		case ANGLE_DISPLAY_STATE:
		{
			current_str = "ANGLE_DISPLAY_STATE";
			break;
		}
	}

	switch (new_state)
	{
		case INIT_STATE:
		{
			next_str = "INIT_STATE";
			break;
		}
		case CALIBRATE_STATE:
		{
			calibreate = 1;
			next_str = "CALIBRATE_STATE";
			break;
		}
		case ANGLE_DISPLAY_STATE:
		{
			angle_display = 1;
			next_str = "ANGLE_DISPLAY_STATE";
			break;
		}
	}

	snprintf(buf,sizeof(buf),"CS::%s, NS::%s\r\n",current_str, next_str);
	printf(buf);

	if(calibreate == 1)
	{
		printf("Press touch slider to set new reference!\r\n");
	}

	if(angle_display == 1)
	{
		printf("Roll FRDM to detect 30,45,60,90 degree angle\r\n");
		printf("Press button between PTD3 and ground to reset the reference level\r\n");
	}

	calibreate = 0;
	angle_display = 0;

}

#endif


