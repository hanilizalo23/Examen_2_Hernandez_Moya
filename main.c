/**
**	\file 	main.c
**	\brief
**		Main source file of the project. This program displays a menu on UART terminal
**		and executes any of the following tasks according to the user's choice (input):
**			1) Sequency
**			2) RGB
 *  Created on: 12/05/2022
 *      Author: Nelida Hernández
**/

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "Menus.h"
#include "fsl_uart.h"

int main(void) {

	ConfigureAll();
	flow_flags_t flag_flow;
    while(1) {
    	//Constantly read one character in terminal
    	flag_flow = ReadTerminals();

    	//Do something according to the stage of the program
    	if(flag_flow.t1_flag)
    	{
    		ChooseStage(PC);
    	}
    }
    return 0 ;
}
