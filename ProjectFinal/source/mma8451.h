/*
 * mma8451.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 */

#ifndef MMA8451_H_
#define MMA8451_H_

#include <MKL25Z4.H>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "i2c.h"

typedef struct accl_param
{
	int x;
	int y;
	int z;
	float roll_val;
	float pitch_val;
	float ref_roll_val;
}acclerometer_parameters_t;


int init_mma(void);
void read_full_xyz(acclerometer_parameters_t *acc);
void convert_xyz_to_roll_pitch(acclerometer_parameters_t *accl_param,uint8_t update_ref);


#endif /* MMA8451_H_ */
