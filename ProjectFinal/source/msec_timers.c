/*
 * msec_timers.c
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 */

#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include "msec_timers.h"

#define TICKS_50MSEC							(1200000)			//24 MhZ

ticktime_t g_tickcount = 0;
ticktime_t g_program_start;
ticktime_t g_timer_start;

/**
 * @prototype : static ticktime_t time_msec()
 * @brief : Initialization systick
 * @parameters : None
 * @return: returns the time in milliseconds
 */
static ticktime_t time_msec()
{
	return g_tickcount*TICK_FREQUENCY;
}

/**
 * @prototype : void init_systick()
 * @brief : Initialization systick
 * @parameters : None
 * @return: None
 */
void init_systick()
{
	/* Disable SysTick during setup */
	SysTick->CTRL = 0;

	/* The value to reload when the counter reaches 0 */
	SysTick->LOAD = TICKS_50MSEC;

	/* Set NVIC Priority for the timer */
	NVIC_SetPriority(SysTick_IRQn,3);

	/* The val register is initialized with 0 */
	SysTick->VAL = 0;

	/* Enable SysTick with core (processor) clock and using interrupt */
	SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk
					| SysTick_CTRL_TICKINT_Msk);

	g_program_start = g_timer_start = time_msec();
}

/**
 * @prototype : void reset_timer()
 * @brief : Reset the timer to zero based on requirement.
 * @parameters : None
 * @return: None
 */
void reset_timer()
{
	g_timer_start = time_msec();
}

/**
 * @prototype : ticktime_t now()
 * @brief : For getting the exact timer value from reset
 * @parameters : None
 * @return: gets the timer value from reset
 */
ticktime_t now()
{
	return time_msec() - g_program_start;
}

/**
 * @prototype : ticktime_t get_timer()
 * @brief : For getting the exact timer value from reset
 * @parameters : None
 * @return: gets the timer value from reset
 */
ticktime_t get_timer()
{
	return time_msec() - g_timer_start;
}

/**
 * @prototype : void SysTick_Handler()
 * @brief : Is called at every overflow as an interrupt with priority 2. Calculates the tick, by how much time has passed out till run.
 * @parameters : None
 * @return: None
 */
void SysTick_Handler()
{
	g_tickcount++;
}

/**
 * @prototype : void delay_ms(uint32_t time)
 * @brief : Calculates for the delay in msec as per systick values.
 * @parameters : Takes input as delay required and works for the while loop in delay.
 * @return : None
 */
void delay_ms(uint32_t time)
{
  uint32_t delay_Start_tick = g_tickcount;
  while (((g_tickcount - delay_Start_tick)*TICK_FREQUENCY) < time);
}
