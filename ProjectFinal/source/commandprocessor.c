/*
 * commandprocessor.c
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 *
 *  @brief  Has the code for Command processing of terminal interface commands and lexical analysis.
 *  @References 1) Lecture slides of Howdy Pierce
 *
 */


#include "commandprocessor.h"

#define COMMAND_COUNT 					(sizeof(commands)/sizeof(commands[0]))

static void author_command_handler(const char *data);
static void help_command_handler(const char *data);
static void unknown_command_handler(const char *data);
static void display_command_handler(const char *data);
static void initiate_command_handler(const char *data);
static void calibrate_command_handler(const char *data);

int reference_angle = 0;
int ref_setting_status = 0;
float roll_calibrate, pitch_calibrate = 0;
extern int while_running;

acc_param_t mma_acc_al = {0,0,0,0,0};

typedef void (*command_handler_t)(const char* data);

typedef struct
{
	const char* commandname;
	command_handler_t handlername;
	const char* commandparameters;
}command_table_t;

static const command_table_t commands[] =
{
	{"author"  ,author_command_handler     ,"Print the author\r\n	 <no parameters>"									},
	{"display" ,display_command_handler    ,"Print the roll and pitch values\r\n	 <no parameters>"					},
	{"help"	   ,help_command_handler	   ,"Print this help message\r\n	 <no parameters>"							},
	{"init"	   ,initiate_command_handler	   ,"initiates the process\r\n	 <no parameters>"							},
	{"calib"	,calibrate_command_handler	   ,"calibrates the angle roll and pitch\r\n	 <no parameters>"			},
};

/**
 * @prototype : void lexical_analysis(uint8_t *input)
 * @brief : For performing lexical analysis
 * @parameters : takes input as the string
 * @return: None
 */

void lexical_analysis(uint8_t *input)
{
	uint8_t *p = input;
	uint8_t *end;
	uint8_t temp_flag = 0;

	bool space_flag = true;

	char temp_buf[200] = {0};
	char *temp = temp_buf;
	char commandreceived[25];
	memset(commandreceived,'\0',sizeof(commandreceived));

	for(end = input;*end != '\0';end++)
	{
		temp_flag++;
	}

	for(p = input; p < end;p++)
	{
		if((*p == ' '))
		{
			if(!space_flag)
			{

				*temp++ = ' ';

			}

			space_flag = true;
		}
		else
		{

			*temp++ = *p;

			space_flag = false;
		}
	}

	*temp++ = '\0';

	temp = &temp_buf[0];

	sscanf(&temp_buf[0],"%s",commandreceived);

	for(uint8_t currentcommand = 0; currentcommand < COMMAND_COUNT;currentcommand++)
	{
		if(strcasecmp(commandreceived,commands[currentcommand].commandname) == 0)
		{
			commands[currentcommand].handlername(temp_buf);
			return;
		}
	}

	unknown_command_handler(temp_buf);
	return;
}

/**
 * @prototype : static void author_command_handler(const char *data)
 * @brief : Prints name of the author
 * @parameters : received command
 * @return: None
 */
static void author_command_handler(const char *data)
{
	printf("\r\n Ram \r\n");
}

/**
 * @prototype : static void help_command_handler(const char *data)
 * @brief : Prints all the commands available for functioning
 * @parameters : received command
 * @return: None
 */
static void help_command_handler(const char *data)
{
	printf("\r\nHeadLamp Beam Assistance Feature options: \r\nOptions available \r\n");

	printf("Author : Display Name of Author \n\r");
	printf("\n\r");

	printf("Display : Print the roll and pitch values\r\n");
	printf("\n\r");

	printf("Help : Displays available commands\n\r");
	printf("\n\r");

	printf("init : initiates the process\n\r");
	printf("\n\r");

	printf("calib : calibrates the angle roll and pitch\n\r");
	printf("\n\r");
}

/**
 * @prototype : static void unknown_command_handler(const char *data)
 * @brief : Unknown command processing function
 * @parameters : received command
 * @return: None
 */
static void unknown_command_handler(const char *data)
{
	if(strcasecmp(data,"\r\n") == 0)
	{

		printf("\r\n");
	}
	else
	{
		printf("\r\nUnknown command: %s",data);
	}
}

/**
 * @prototype : static void display_command_handler(const char *data)
 * @brief : Prints the roll and pitch of the sensor
 * @parameters : received command
 * @return: None
 */
static void display_command_handler(const char *data)
{
	read_full_xyz(&mma_acc_al);
	convert_xyz_to_roll_pitch(&mma_acc_al, 0);

	float roll = mma_acc_al.roll_val;
	float pitch = mma_acc_al.pitch_val;

	printf("THe values of ROLL and PITCH are: roll = %d, pitch = %d\r\n", (int)roll, (int)pitch);

}

/**
 * @prototype : static void initiate_command_handler(const char *data)
 * @brief : Initiates the headlamp assistance feature.
 * @parameters : received command
 * @return: None
 */
static void initiate_command_handler(const char *data)
{
	if(while_running !=1){
	printf("\r\n Initiating Headlamp Assistance \r\n");

	while_running = 1;
	}
}

/**
 * @prototype : static void calibrate_command_handler(const char *data)
 * @brief : Calibrates the roll and pitch for the headlamp assistance feature.
 * @parameters : received command
 * @return: None
 */
static void calibrate_command_handler(const char *data)
{
	read_full_xyz(&mma_acc_al);
	convert_xyz_to_roll_pitch(&mma_acc_al, 0);
	roll_calibrate =  mma_acc_al.roll_val;
	pitch_calibrate = mma_acc_al.pitch_val;

	printf("\r\n Calibration Values: \r\n");
	printf("roll_calibrate = %d,  pitch_calibrate = %d\r\n", (int)roll_calibrate, (int)pitch_calibrate);
}
