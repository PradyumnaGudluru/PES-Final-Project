/*
 * statemachine.c
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 */


#include "statemachine.h"

#define ZERO_THRESH                                 (0)
#define THRESH_POSITIVE								(35)
#define THRESH_NEGATIVE								(-35)
#define ERR_POS										(3)
#define ERR_NEG										(-3)

acclerometer_parameters_t mma_accl = {0,0,0,0,0,0};
char *state_str = NULL;
#if 0
/*******************************************************************************
 Macros
*******************************************************************************/
#define SET_INDIVIDUAL_VALUE						(15)
#define FULL_ON										(10)
#define FULL_OFF									(0)
#define UPDATE_REF_LEVEL							(1)
#define MAINTAIN_REF_LEVEL							(0)
#define RANDOM_COLOR_UPPER_LIMIT					(5)
#define MSEC_750									(15*TICK_FREQUENCY)
#define MSEC_250									(5*TICK_FREQUENCY)
/*******************************************************************************
 Structures and Enums
*******************************************************************************/
typedef struct rgb_led_color
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}rgb_color_t;

typedef enum
{
    ANGLE_0 = 0,
    ANGLE_1 = 30,
	ANGLE_2 = 45,
	ANGLE_3 = 60,
	ANGLE_4 = 90
}eangles_detected;

/*******************************************************************************
 Global variables
*******************************************************************************/
rgb_color_t pwm_led_color = {0,0,0};
acclerometer_parameters_t mma_accl_param = {0,0,0,0,0,0};
extern uint8_t switch_press_detected;
extern int cap_tap_detected;
/*******************************************************************************
 Function Prototype
*******************************************************************************/

/*******************************************************************************
 Function definition
*******************************************************************************/
/*******************************************************************************
* @Function name: handle_calibrate_to_zero
* @Description: In this state the calibration (reference) is performed for DAG
* for reference 0 exact
* @input param: none
* @return: eDigitalAngleGaugeState - next state to be transitioned into
*******************************************************************************/
eDigitalAngleGaugeState handle_calibrate_to_zero()
{
	pwm_led_color.red = 0;
	pwm_led_color.green = 0;
	pwm_led_color.blue = SET_INDIVIDUAL_VALUE;

	rgb_pwm_controller(pwm_led_color.red,pwm_led_color.green,pwm_led_color.blue);

	read_full_xyz(&mma_accl_param);
	convert_xyz_to_roll_pitch(&mma_accl_param,MAINTAIN_REF_LEVEL);

	if(mma_accl_param.roll_val == 0.0)
	{
		convert_xyz_to_roll_pitch(&mma_accl_param,UPDATE_REF_LEVEL);

		if(mma_accl_param.ref_roll_val < 0)
		{
			mma_accl_param.ref_roll_val = mma_accl_param.ref_roll_val*(-1);
		}

		/* set the colors default to blue */
		pwm_led_color.red = 0;
		pwm_led_color.green = SET_INDIVIDUAL_VALUE;
		pwm_led_color.blue = 0;

		/* set the colors to the desired as per the angle under consideration*/
		rgb_pwm_controller(pwm_led_color.red,pwm_led_color.green,pwm_led_color.blue);
		/* wait for 750 msec for detection */
		reset_timer();
		while(get_timer() < MSEC_750);

		/* set the colors default to blue */
		pwm_led_color.red = 0;
		pwm_led_color.green = 0;
		pwm_led_color.blue = 0;
		rgb_pwm_controller(pwm_led_color.red,pwm_led_color.green,pwm_led_color.blue);

		/* wait for 250 msec for detection */
		reset_timer();
		while(get_timer() < MSEC_250);


		printf("Reference value is %d\r\n",(int)mma_accl_param.ref_roll_val);
		printf("Roll value is %d\r\n",(int)mma_accl_param.roll_val);
		printf("Calibration done for ZERO\r\n");

		set_reference_angle((int)mma_accl_param.ref_roll_val);

		return ANGLE_DISPLAY_STATE;
	}

	return INIT_STATE;

}
/*******************************************************************************
* @Function name: handle_calibrate_state
* @Description: In this state the calibration (reference) is performed for DAG
* @input param: none
* @return: eDigitalAngleGaugeState - next state to be transitioned into
*******************************************************************************/
eDigitalAngleGaugeState handle_calibrate_state()
{
	pwm_led_color.red = 0;
	pwm_led_color.green = 0;
	pwm_led_color.blue = 0;

	if(cap_tap_detected)
	{
		cap_tap_detected = 0;
		printf("CS::CALIBRATE STATE CAP TAP detected\r\n");

		read_full_xyz(&mma_accl_param);
		convert_xyz_to_roll_pitch(&mma_accl_param,UPDATE_REF_LEVEL);

		if(mma_accl_param.ref_roll_val < 0)
		{
			mma_accl_param.ref_roll_val = mma_accl_param.ref_roll_val*(-1);
		}

		set_reference_angle((int)mma_accl_param.ref_roll_val);

		return ANGLE_DISPLAY_STATE;
	}

	rgb_pwm_controller(pwm_led_color.red,pwm_led_color.green,pwm_led_color.blue);

	return CALIBRATE_STATE;
}

/*******************************************************************************
* @Function name: handle_angle_gauge_state
* @Description: In this state the desired angle value is obtained
* @input param: none
* @return: eDigitalAngleGaugeState - next state to be transitioned into
*******************************************************************************/
eDigitalAngleGaugeState handle_angle_gauge_state()
{
	int final_compare_val;

	read_full_xyz(&mma_accl_param);
	convert_xyz_to_roll_pitch(&mma_accl_param,MAINTAIN_REF_LEVEL);

	if(mma_accl_param.roll_val < 0)
	{
		mma_accl_param.roll_val = mma_accl_param.roll_val*(-1);
	}

	/* default color when a particular angle is not there */
	pwm_led_color.red = 0;
	pwm_led_color.green = 0;
	pwm_led_color.blue = 0;
	rgb_pwm_controller(pwm_led_color.red,pwm_led_color.green,pwm_led_color.blue);

	final_compare_val =  (int)(mma_accl_param.roll_val - mma_accl_param.ref_roll_val);
	angle_detect_color_assign(final_compare_val);

	if(switch_press_detected)
	{
		printf("CS::ANGLE_DISPLAY_STATE SWITCH TAP detected\r\n");
		switch_press_detected = 0;
		return CALIBRATE_STATE;
	}

	return ANGLE_DISPLAY_STATE;
}

/*******************************************************************************
* @Function name: angle_detect_color_assign
* @Description: In this state the the color for the angle obtained is displayed
* @input param: none
* @return:
*******************************************************************************/
void angle_detect_color_assign(int color_assigner)
{
	switch(color_assigner)
	{
		case ANGLE_1:
		{
			printf("30 degree detected\r\n");
			pwm_led_color.red = SET_INDIVIDUAL_VALUE;
			pwm_led_color.green = 0;
			pwm_led_color.blue = 0;
			break;
		}
		case ANGLE_2:
		{
			printf("45 degree detected\r\n");
			pwm_led_color.red = 0;
			pwm_led_color.green = SET_INDIVIDUAL_VALUE;
			pwm_led_color.blue = 0;
			break;
		}
		case ANGLE_3:
		{
			printf("60 degree detected\r\n");
			pwm_led_color.red = 0;
			pwm_led_color.green = 0;
			pwm_led_color.blue = SET_INDIVIDUAL_VALUE;
			break;
		}
		case ANGLE_4:
		{
			printf("90 degree detected\r\n");
			pwm_led_color.red = FULL_ON;
			pwm_led_color.green = FULL_ON;
			pwm_led_color.blue = FULL_ON;
			break;
		}
		default:
		{
			break;
		}
	}

	if((pwm_led_color.red > 0)
				|| (pwm_led_color.green > 0 )||
						(pwm_led_color.blue > 0))
	{
		/* set the colors to the desired as per the angle under consideration*/
		rgb_pwm_controller(pwm_led_color.red,pwm_led_color.green,pwm_led_color.blue);
		/* wait for 750 msec for detection */
		reset_timer();
		while(get_timer() < MSEC_750);

		/* set the colors default to blue */
		pwm_led_color.red = 0;
		pwm_led_color.green = 0;
		pwm_led_color.blue = 0;
		rgb_pwm_controller(pwm_led_color.red,pwm_led_color.green,pwm_led_color.blue);

		/* wait for 250 msec for detection */
		reset_timer();
		while(get_timer() < MSEC_250);
	}

}
#endif

/*
* @prototype:  CmdAuth()
* @brief: Command Authentication for the color change of headlight based on the roll and pitch of the car.
* @input : None
* @return: Color/ state of color (enum value)
*/
int CmdAuth(){

	read_full_xyz(&mma_accl);
	convert_xyz_to_roll_pitch(&mma_accl, 0);

	float roll = mma_accl.roll_val;
	float pitch = mma_accl.pitch_val;

   // printf("cmdAuth: roll = %f, pitch = %f\r\n", roll, pitch);

	if(((int)roll == ZERO_THRESH) && ((int)pitch == ZERO_THRESH)){
		return WHITE;
	}
	else if ((roll > ZERO_THRESH && roll<=THRESH_POSITIVE) && (pitch >= ERR_NEG && pitch <= ERR_POS)){
		return GREEN;
	}
	else if((roll > THRESH_NEGATIVE && roll < ZERO_THRESH) && (pitch >= ERR_NEG && pitch<= ERR_POS)){
		return BLUE;
	}
	else if((roll >= ERR_NEG && roll <= ERR_POS) && (pitch > ZERO_THRESH && pitch<=THRESH_POSITIVE)){
		return ORANGE;
	}
	else if((roll >= ERR_NEG && roll <= ERR_POS) && (pitch > THRESH_NEGATIVE && pitch < ZERO_THRESH)){
		return VIOLET;
	}
	else{
		return RED;
	}
}

/*
* @prototype:  state_color()
* @brief: State machine for the color change.
* @input : None
* @return: none
*/
void state_color(){

	float roll = mma_accl.roll_val;
		float pitch = mma_accl.pitch_val;
		//int blink = 0;

	Colour_State state = CmdAuth();

    //printf("state_colour: roll = %f, pitch = %f\r\n", roll, pitch);

	if(get_timer() > 3000){
		reset_timer();
		printf("\r\n roll = %d, pitch = %d\r\n", (int)roll, (int)pitch);
		printf("STATE : \r");
		for(int i =0;i<strlen(state_str);i++){
		printf("%c", state_str[i]);
		}
		printf("\n");
	}

	switch(state){

	case GREEN:
		//PWM_Config_GREEN();
		//LED_control(0,255,0);
		rgb_pwm_controller(0,255,0);
		state_str = "CAR IS MOVING TO RIGHT\r\n";
		//printf("GREEN STATE \r\n");
		//printf("state_colour: roll = %d, pitch = %d\r\n", (int)roll, (int)pitch);

		break;

	case BLUE:
		//PWM_Config_BLUE();
		//LED_control(0,0,255);
		rgb_pwm_controller(0,0,255);
		state_str = "CAR IS MOVING TO LEFT\r\n";
		//printf("BLUE STATE \r\n");
		//printf("state_colour: roll = %d, pitch = %d\r\n", (int)roll, (int)pitch);

		break;

	case VIOLET:
			//PWM_Config_VIOLET();
		rgb_pwm_controller(165, 51, 255);
		state_str = "CAR IS MOVING DOWNHILL\r\n";
		//printf("VIOLET STATE \r\n");
		//printf("state_colour: roll = %d, pitch = %d\r\n", (int)roll, (int)pitch);

			break;

	case ORANGE:
			//PWM_Config_ORANGE();
		rgb_pwm_controller(255,147,51);
		state_str = "CAR IS MOVING UPHILL\r\n";
		//printf("ORANGE STATE \r\n");
		//printf("state_colour: roll = %d, pitch = %d\r\n", (int)roll, (int)pitch);

			break;

	case RED:
			//PWM_Config_RED();

		rgb_pwm_controller(255,0,0);
		state_str = "CAR TOPPLED\r\n";
		//printf("RED STATE \r\n");
		//printf("state_colour: roll = %d, pitch = %d\r\n", (int)roll, (int)pitch);

			break;

	case WHITE:

		rgb_pwm_controller(255,255,255);
		state_str = "CAR AT ZERO(CALIBRATED STATE)\r\n";

		break;
	}
}
