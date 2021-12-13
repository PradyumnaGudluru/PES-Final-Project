/*
 * msec_timers.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 */

#ifndef MSEC_TIMERS_H_
#define MSEC_TIMERS_H_

#define TICK_FREQUENCY							(50)

typedef uint32_t ticktime_t;

void delay(uint32_t time_del);
void init_systick();
ticktime_t now();
void reset_timer();
ticktime_t get_timer();

void delay_ms(uint32_t time);

#endif /* MSEC_TIMERS_H_ */
