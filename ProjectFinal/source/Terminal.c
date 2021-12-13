/*
 * Terminal.c
 *
 * Created on: 10-Nov-2021
 * Author: Pradyumna
 * @brief  Has the code for terminal functions.
 * @References 1) Lecture Slides of Howdy Pierce
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include "Terminal.h"

/**
 * @prototype    author()
 * @brief   This function is called when the command is author and prints the name of author.
 */
void author(char* command){
	printf("Ram\r\n");
}

/**
 * @prototype    author()
 * @brief   This function is called when the command is dump and prints the hexdump of the memory requested.

void dump(char* command){
	uint32_t Start;
	size_t Len;
	if(sscanf(command, "dump %x %i", &Start, &Len) == 2){
		hexdump((void*) Start, Len);
	}
} */

/**
 * @prototype    unknown()
 * @brief   This function is called when the command doesn't match with any of the available commands.
 */
void unknown(char* command){
	printf("Unknown Command: %s\r\n",command);
}

/**
 * @prototype    help()
 * @brief   This function is called when the command is help and prints available commands on source code.
 */
void help(const char* cmd){

	printf("BreakfastSerial Commands\n\r\n\r");
	printf("Author : Display Name of Author \n\r");
	printf("\n\r");

	printf("Dump <location> <length> : Displays HexDump of requested length from given address\n\r");
	printf("\n\r");

	printf("Help : Displays available commands\n\r");
	printf("\n\r");

	return;
}
/*
void info(const char* cmd){

	printf("VERSION_TAG %s \r\n BUILD_MACHINE %s \r\n BUILD_DATE %s \r\n", VERSION_TAG, BUILD_MACHINE, BUILD_DATE);
}*/


/**
 * @prototype    lexical_cmdprocess()
 * @brief   This function takes the command from the terminal as input and adjusts as per the readable command. Then based on
 * 			the set of available commands, it calls the command calling functions.
 */
void  lexical_cmdprocess(char *cmd)
{

	char scan_buff[3000] = {0};
	char* str_temp_scan = &scan_buff[0];
	char* str_temp_cmd = &cmd[0];
	int check_space = 1;
	char cmdcmp[20];

	while(*str_temp_cmd != '\0')
	{
		if(isspace(*str_temp_cmd))
		{
			if(check_space)
			{
				str_temp_cmd++;
			}
			else
			{
				*str_temp_scan++ = ' ';
				check_space= 1;
			}
		}
		else
		{
			*str_temp_scan++ = tolower(*str_temp_cmd++);
			check_space = 0;
		}
	}

	*str_temp_cmd = '\0';

	sscanf(scan_buff, "%s", cmdcmp);


	if(strcmp(cmdcmp, "author") == 0)
	{
		author(scan_buff);
		str_temp_scan = &scan_buff[0];
		str_temp_cmd = &cmd[0];
		return;
	}

	else if(strcmp(cmdcmp, "dump") == 0)
	{
		//dump(scan_buff);
		str_temp_scan = &scan_buff[0];
		str_temp_cmd = &cmd[0];
		return;
	}

	else if(strcmp(cmdcmp, "help") == 0){
		help(scan_buff);
		str_temp_scan = &scan_buff[0];
		str_temp_cmd = &cmd[0];
		return;
	}

	else
	{
		unknown(scan_buff);
		str_temp_scan = &scan_buff[0];
		str_temp_cmd = &cmd[0];
		return;
	}


}
