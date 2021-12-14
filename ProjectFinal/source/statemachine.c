/*
 * statemachine.c
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 * @brief  Has the code forfunctionality of Headlamp Beam Assistance Feature.
 *
 */

/* Header File */
#include "statemachine.h"

/* MACRO DEFS */
#define ZERO_THRESH                                 (0)
#define THRESH_POSITIVE								(35)
#define THRESH_NEGATIVE								(-35)
#define ERR_POS										(3)
#define ERR_NEG										(-3)

/* Variables */
acc_param_t mma_accl = {0,0,0,0,0};
char *state_str = NULL;

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
	else if((roll >= THRESH_NEGATIVE && roll < ZERO_THRESH) && (pitch >= ERR_NEG && pitch<= ERR_POS)){
		return BLUE;
	}
	else if((roll >= ERR_NEG && roll <= ERR_POS) && (pitch > ZERO_THRESH && pitch<=THRESH_POSITIVE)){
		return ORANGE;
	}
	else if((roll >= ERR_NEG && roll <= ERR_POS) && (pitch >= THRESH_NEGATIVE && pitch < ZERO_THRESH)){
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

	Colour_State state = CmdAuth();


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

		rgb_pwm_controller(0,255,0);
		state_str = "CAR IS MOVING TO RIGHT\r\n";

		break;

	case BLUE:

		rgb_pwm_controller(0,0,255);
		state_str = "CAR IS MOVING TO LEFT\r\n";

		break;

	case VIOLET:

		rgb_pwm_controller(165, 51, 255);
		state_str = "CAR IS MOVING DOWNHILL\r\n";

			break;

	case ORANGE:

		rgb_pwm_controller(255,147,51);
		state_str = "CAR IS MOVING UPHILL\r\n";

			break;

	case RED:

		rgb_pwm_controller(255,0,0);
		state_str = "CAR TOPPLED\r\n";

			break;

	case WHITE:

		rgb_pwm_controller(255,255,255);
		state_str = "CAR AT ZERO(CALIBRATED STATE)\r\n";

		break;
	}
}
