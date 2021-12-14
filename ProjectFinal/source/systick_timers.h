/*
 * systick_timers.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 * @brief  Has the code for basic functionality of Systick functionality.
 * @References 1) Lecture slides of Howdy Pierce
 *  			2) https://github.com/alexander-g-dean/ESF/tree/master/NXP
 */

#ifndef SYSTICK_TIMERS_H_
#define SYSTICK_TIMERS_H_

#define TICK_TIME							(50)

typedef uint32_t ticktime_t;

void delay(uint32_t time_del);
void init_systick();
ticktime_t now();
void reset_timer();
ticktime_t get_timer();

void delay_ms(uint32_t time);

#endif /* SYSTICK_TIMERS_H_ */
