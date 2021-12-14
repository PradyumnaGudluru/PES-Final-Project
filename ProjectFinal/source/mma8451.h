/*
 * mma8451.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 *
 * @brief  Has the code for basic functionality of Accelerometer sensor and has roll and pitch
 * 		calculating functions.
 * @References 1) Lecture slides of Howdy Pierce
 *  			2) https://github.com/alexander-g-dean/ESF/tree/master/NXP
 *  			3) MMA8451Q datasheet
 */

#ifndef MMA8451_H_
#define MMA8451_H_

#include <MKL25Z4.H>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "i2c.h"

typedef struct acc_val
{
	int x;
	int y;
	int z;
	float roll_val;
	float pitch_val;
}acc_param_t;


int init_mma(void);
void read_full_xyz(acc_param_t *acc);
void convert_xyz_to_roll_pitch(acc_param_t *acc_val,uint8_t update_ref);


#endif /* MMA8451_H_ */
