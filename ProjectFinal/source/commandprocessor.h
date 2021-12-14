/*
 * commandprocessor.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 *  @brief  Has the code for Command processing of terminal interface commands and lexical analysis.
 *  @References 1) Lecture slides of Howdy Pierce
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

#endif /* COMMANDPROCESSOR_H_ */
