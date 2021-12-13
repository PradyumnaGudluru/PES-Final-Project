/*
 * led_tmp.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 */

#ifndef LED_TMP_H_
#define LED_TMP_H_

#include "MKL25Z4.h"
#include "msec_timers.h"

#define PWM_PERIOD										255

void init_led_pwm();
void init_tpm();
void rgb_pwm_controller(uint8_t red, uint8_t green, uint8_t blue);
void red_led_on();
void green_led_on();
void blue_led_on();
void all_led_on();
void all_led_off();

#endif /* LED_TMP_H_ */
