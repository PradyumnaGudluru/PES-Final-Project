/*
 * led_tmp_testcase.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 */

#ifndef LED_TMP_TESTCASE_H_
#define LED_TMP_TESTCASE_H_

/*******************************************************************************
 Header Files
*******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include "MKL25Z4.h"
#include "led_tmp.h"
#include "msec_timers.h"
/*******************************************************************************
 Function declarations
*******************************************************************************/
void test_led_tpm(uint8_t pwm_period);

#endif /* LED_TMP_TESTCASE_H_ */
