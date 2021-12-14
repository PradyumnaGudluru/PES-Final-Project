/*
 * commandprocessor.c
 *
 *  Created on: 12-Dec-2021
 *      Author: Pradyumna
 */


#include "commandprocessor.h"

#define NO_OF_COMMANDS 					(sizeof(commands)/sizeof(commands[0]))

static void author_command_handler(const char *datareceived);
static void help_command_handler(const char *datareceived);
static void unknown_command_handler(const char *datareceived);
static void display_command_handler(const char *datareceived);
static void initiate_command_handler(const char *datareceived);
static void calibrate_command_handler(const char *datareceived);

int reference_angle = 0;
int ref_setting_status = 0;
float roll_calibrate, pitch_calibrate = 0;
extern int while_running;

acclerometer_parameters_t mma_acc_al = {0,0,0,0,0,0};

typedef void (*command_handler_t)(const char* datareceived);

typedef struct
{
	const char* commandname;
	command_handler_t handlername;
	const char* commandparameters;
}command_table_t;

static const command_table_t commands[] =
{
	{"author"  ,author_command_handler     ,"Print the author\r\n	 <no parameters>"									},
	{"display" ,display_command_handler    ,"Print the roll and pitch values\r\n	 <no parameters>"					    },
	{"help"	   ,help_command_handler	   ,"Print this help message\r\n	 <no parameters>"							},
	{"init"	   ,initiate_command_handler	   ,"initiates the process\r\n	 <no parameters>"							},
	{"calib"	,calibrate_command_handler	   ,"calibrates the angle roll and pitch\r\n	 <no parameters>"		},
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
	uint8_t tempcounter = 0;

	bool spacedetected = true;

	char tempbuf[200] = {0};
	char *tempptr = tempbuf;
	char commandreceived[25];
	memset(commandreceived,'\0',sizeof(commandreceived));

	/* find end of string */
	for(end = input;*end != '\0';end++)
	{
		tempcounter++;
	}

	for(p = input; p < end;p++)
	{
		if((*p == ' '))
		{
			if(!spacedetected)
			{
				/* inserting space only once after the
				 * end of the string.
				 * Extra spaces are removed.
				 * Will be used further in sscanf to tokenize */
				*tempptr++ = ' ';
				/* Do nothing as space is to be ignored */
			}

			spacedetected = true;
		}
		else
		{
			/* Enter when a character is detected */
			*tempptr++ = *p;

			spacedetected = false;
		}
	}

	*tempptr++ = '\0';

	tempptr = &tempbuf[0];

	/* extract the first string  for command detection */
	sscanf(&tempbuf[0],"%s",commandreceived);

	for(uint8_t currentcommand = 0; currentcommand < NO_OF_COMMANDS;currentcommand++)
	{
		if(strcasecmp(commandreceived,commands[currentcommand].commandname) == 0)
		{
			commands[currentcommand].handlername(tempbuf);
			return;
		}
	}

	/* if the command is not recognized
	 * call the below handler */
	unknown_command_handler(tempbuf);
	return;
}

/**
 * @prototype : static void author_command_handler(const char *datareceived)
 * @brief : Prints name of the author
 * @parameters : received command
 * @return: None
 */
static void author_command_handler(const char *datareceived)
{
	printf("\r\n Ram \r\n");
}

/**
 * @prototype : static void help_command_handler(const char *datareceived)
 * @brief : Prints all the commands available for functioning
 * @parameters : received command
 * @return: None
 */
static void help_command_handler(const char *datareceived)
{
	printf("\r\nHeadLamp Beam Assistance Feature options: \r\nOptions available \r\n");

	/*for(uint8_t cmdlen = 0; cmdlen < NO_OF_COMMANDS; cmdlen++)
	{
		printf("%s\r\n\t %s\r\n",commands[cmdlen].commandname,
							commands[cmdlen].commandparameters);
	}*/

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
 * @prototype : static void unknown_command_handler(const char *datareceived)
 * @brief : Unknown command processing function
 * @parameters : received command
 * @return: None
 */
static void unknown_command_handler(const char *datareceived)
{
	if(strcasecmp(datareceived,"\r\n") == 0)
	{
		/* handles the new line printing when enter key is pressed */
		printf("\r\n");
	}
	else
	{
		printf("\r\nUnknown command: %s",datareceived);
	}
}

/**
 * @prototype : static void display_command_handler(const char *datareceived)
 * @brief : Prints the roll and pitch of the sensor
 * @parameters : received command
 * @return: None
 */
static void display_command_handler(const char *datareceived)
{
	read_full_xyz(&mma_acc_al);
	convert_xyz_to_roll_pitch(&mma_acc_al, 0);

	float roll = mma_acc_al.roll_val;
	float pitch = mma_acc_al.pitch_val;

	printf("THe values of ROLL and PITCH are: roll = %d, pitch = %d\r\n", (int)roll, (int)pitch);

}

/**
 * @prototype : void set_reference_angle(int ref_rcvd )
 * @brief : Sets up the reference angle
 * @parameters : reference received angle
 * @return: None
 */
void set_reference_angle(int ref_rcvd )
{
	reference_angle = ref_rcvd;
	ref_setting_status = 1;
}


/**
 * @prototype : static void initiate_command_handler(const char *datareceived)
 * @brief : Initiates the headlamp assistance feature.
 * @parameters : received command
 * @return: None
 */
static void initiate_command_handler(const char *datareceived)
{
	if(while_running !=1){
	printf("\r\n Initiating Headlamp Assistance \r\n");

	while_running = 1;
	}
}

/**
 * @prototype : static void calibrate_command_handler(const char *datareceived)
 * @brief : Calibrates the roll and pitch for the headlamp assistance feature.
 * @parameters : received command
 * @return: None
 */
static void calibrate_command_handler(const char *datareceived)
{
	read_full_xyz(&mma_acc_al);
	convert_xyz_to_roll_pitch(&mma_acc_al, 0);
	roll_calibrate =  mma_acc_al.roll_val;
	pitch_calibrate = mma_acc_al.pitch_val;

	printf("\r\n Calibration Values: \r\n");
	printf("roll_calibrate = %d,  pitch_calibrate = %d\r\n", (int)roll_calibrate, (int)pitch_calibrate);
}
