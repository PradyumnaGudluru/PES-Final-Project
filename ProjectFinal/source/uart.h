/*
 * uart.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 * @brief  Has the code for UART initialization and terminal calling function.
 * @References 1) Lecture slides of Howdy Pierce
 * 			   2) https://github.com/alexander-g-dean/ESF/tree/master/NXP/Misc/Touch%20Sense/TSI/src
 */

#ifndef UART_H_
#define UART_H_

#include <MKL25Z4.H>
#include <stdint.h>
#include <stdio.h>
#include "cbfifo.h"
#include "sysclock.h"

void init_UART0();
int __sys_write(int handle, char *buf, int size);
int __sys_readc(void);

void Terminal();

#endif /* UART_H_ */
