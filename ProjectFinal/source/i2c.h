/*
 * i2c.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 *
 * @brief  Has the code for basic functionality of I2C with read and write functions.
 *  @References 1) Lecture slides of Howdy Pierce
 *  			2) https://github.com/alexander-g-dean/ESF/tree/master/NXP
 */

#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

void init_i2c(void);
void i2c_start(void);
void i2c_read_setup(uint8_t dev, uint8_t address);
uint8_t i2c_repeated_read(uint8_t);
//uint8_t i2c_read_byte(uint8_t dev, uint8_t address);
void i2c_write_byte(uint8_t dev, uint8_t address, uint8_t data);


#endif /* I2C_H_ */
