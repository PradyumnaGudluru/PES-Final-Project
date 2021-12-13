/*
 * commandprocessor.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 */

#ifndef COMMANDPROCESSOR_H_
#define COMMANDPROCESSOR_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "mma8451.h"

void acceptinput();
void lexical_analysis(uint8_t *cmdProessor);
void set_reference_angle(int ref_rcvd );

#endif /* COMMANDPROCESSOR_H_ */
