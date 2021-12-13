/*
 * touch_slider.h
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 */

#ifndef TOUCH_SLIDER_H_
#define TOUCH_SLIDER_H_

/*******************************************************************************
 Header Files
*******************************************************************************/
#include <MKL25Z4.H>
#include <stdio.h>
/*******************************************************************************
 Function Prototype
*******************************************************************************/
void init_touchsensor();
void TSI0_IRQHandler (void);
void start_touch_scanning();


#endif /* TOUCH_SLIDER_H_ */
