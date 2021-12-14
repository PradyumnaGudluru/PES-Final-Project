/*
 * led_tmp.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 * @brief  Has the code for basic functionality of LED by TPM LED Switching ON functions.
 * @References 1) Lecture slides of Howdy Pierce
 *  			2) https://github.com/alexander-g-dean/ESF/tree/master/NXP
 */

#ifndef LED_TMP_H_
#define LED_TMP_H_

#include "MKL25Z4.h"
#include "systick_timers.h"

#define PWM_PERIOD										255

void init_led_pwm();
void init_tpm();
void rgb_pwm_controller(uint8_t red, uint8_t green, uint8_t blue);

#endif /* LED_TMP_H_ */
