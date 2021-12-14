/*
 * statemachine.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 * @brief  Has the code forfunctionality of Headlamp Beam Assistance Feature.
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include <stdio.h>
#include <stdint.h>
#include "MKL25Z4.h"
#include "mma8451.h"
#include "led_tmp.h"
#include "systick_timers.h"
#include "commandprocessor.h"
#include "string.h"

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
