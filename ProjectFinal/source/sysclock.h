/*
 * sysclock.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 *
 * sysclock.h - configuration routines for KL25Z system clock
 *
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 */

#ifndef SYSCLOCK_H_
#define SYSCLOCK_H_


#define SYSCLOCK_FREQUENCY 									(24000000) /* 24 Mhz */
/*
 * Initializes the system clock. You should call this first in your
 * program.
 */
void init_sysclock();

#endif /* SYSCLOCK_H_ */
