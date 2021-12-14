/*
 * mma8451.c
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 * @brief  Has the code for basic functionality of Accelerometer sensor and has roll and pitch
 * 		calculating functions.
 * @References 1) Lecture slides of Howdy Pierce
 *  			2) https://github.com/alexander-g-dean/ESF/tree/master/NXP
 *  			3) MMA8451Q datasheet
 */
/* Header File*/
#include "mma8451.h"

/* MACRO DEFS */
#define MMA_ADDR 								0x3A
#define REG_XHI 								0x01
#define COUNTS_PER_G 							(4096.0)
#define M_PI 									(3.14159265)
#define REG_CTRL1  								0x2A
#define SET										1

/* Variables */
extern int cap_tap_detected;
extern float roll_calibrate, pitch_calibrate;

/**
 * @prototype : int init_mma()
 * @brief : initializing accelerometer sensor.
 * @parameters : Takes input as accelerometer parameter
 * @return: None
 */
int init_mma()
{
	/* set active mode, 14 bit sampling and 800 Hz ODR value */
	i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x01);
	return 1;
}

/**
 * @prototype : void read_full_xyz(acc_param_t *acc_val)
 * @brief : reading the values of x, y and z
 * @parameters : Takes input as accelerometer parameter
 * @return: None
 */
void read_full_xyz(acc_param_t *acc_val)
{
	int i;
	uint8_t data[6];
	int16_t temp[3];

	i2c_start();
	i2c_read_setup(MMA_ADDR , REG_XHI);

	/* Read 5 bytes in repeated mode */
	for(i = 0 ; i < 5; i++)
	{
		data[i] = i2c_repeated_read(0);
	}

	/* Read last byte ending repeated mode */
	data[i] = i2c_repeated_read(1);

	/* calculate the final value for x,y,z
	 * and store it in the respective buffer */
	for(i = 0; i < 3; i++)
	{
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	/* Align the data for 14 bits */
	acc_val->x= temp[0]/4;
	acc_val->y = temp[1]/4;
	acc_val->z = temp[2]/4;

}

/**
 * @prototype : void convert_xyz_to_roll_pitch(acc_param_t *acc_val,uint8_t update_ref)
 * @brief : Converts x, y and z values to roll values and pitch values.
 * @parameters : Takes input as accelerometer parameter and updated to reference
 * @return: None
 */
void convert_xyz_to_roll_pitch(acc_param_t *acc_val,uint8_t update_ref)
{
	float ax = (acc_val->x)/COUNTS_PER_G;
	float ay = (acc_val->y)/COUNTS_PER_G;
	float az = (acc_val->z)/COUNTS_PER_G;

	acc_val->roll_val= atan2(ay, az)*180/M_PI;
	acc_val->pitch_val = atan2(ax, sqrt(ay*ay + az*az))*180/M_PI;

	acc_val->roll_val= acc_val->roll_val - roll_calibrate;
	acc_val->pitch_val = acc_val->pitch_val - pitch_calibrate;
}
