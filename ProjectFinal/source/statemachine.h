/*
 * statemachine.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include <stdio.h>
#include <stdint.h>
#include "MKL25Z4.h"
#include "mma8451.h"
#include "led_tmp.h"
#include "msec_timers.h"
#include "commandprocessor.h"
#include "string.h"
#if 0
/*******************************************************************************
 Structures
*******************************************************************************/
typedef enum
{
	INIT_STATE = 0,					/* In this state the calibration for zero is performed */
    CALIBRATE_STATE,				/* In this state the calibration (reference) is performed */
    ANGLE_DISPLAY_STATE 			/* In this state the angles are displayed */
}eDigitalAngleGaugeState;
/*******************************************************************************
 Function declarations
*******************************************************************************/
eDigitalAngleGaugeState handle_calibrate_to_zero();
eDigitalAngleGaugeState handle_calibrate_state();
eDigitalAngleGaugeState handle_angle_gauge_state();
void angle_detect_color_assign(int color_assigner);
#endif

void state_color();
int CmdAuth();

typedef enum {
	GREEN,
	BLUE,
	VIOLET,
	ORANGE,
	RED,
	WHITE
}Colour_State;

#endif /* STATEMACHINE_H_ */
