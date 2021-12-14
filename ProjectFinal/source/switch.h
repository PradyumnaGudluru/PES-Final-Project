/*
 * switch.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 * @brief  Has the code for basic functionality of switch GPIO Interrupts.
 * @References 1) Lecture slides of Howdy Pierce
 *  			2) https://github.com/alexander-g-dean/ESF/tree/master/NXP
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include "MKL25Z4.h"
#include <stdio.h>
#include <stdint.h>
#include "mma8451.h"
#include "led_tmp.h"

void init_switch();
void switch_cmd_process();

#endif /* SWITCH_H_ */
