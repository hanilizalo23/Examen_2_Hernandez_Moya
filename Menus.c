
/*
 * \file 	Menus.c
 *		This source file includes the functions to access to each menu/submenu
**		of the program
 *		
 *  Created on: 12/05/2022
 *      Author: Nelida Hernández
 */

#include "Menus.h"
#include "MK64F12.h"
#include "stdbool.h"
#include "fsl_uart.h"
#include "Bits.h"
#include "UART_PC.h"
#include "PIT.h"
#include <stdio.h>

static program_status_t g_actual_status[2] = {{MAIN_MENU, NONE,false},{MAIN_MENU, NONE,false}};
static uint8_t read_data[2];

//Strings for messages
uint8_t MainMenu[] = "\r\n1) Introducir secuencia\r\n2) PWM\r\n";

uint8_t ConfigureAll(void)
{
	UART_PC_configure_port();

	return(1);
}

flow_flags_t ReadTerminals(void)
{
	flow_flags_t flow_flag = {false, false};
	read_data[PC] = ASCII_NULL;
	UART_PC_read(&read_data[PC],1);
	delay_PIT(PIT_0, DELAY_READING);
	if((MAIN_MENU == g_actual_status[PC].stage) | (UART_PC_get_flag()) | g_actual_status[PC].continue_flow)
	{
		flow_flag.t1_flag = true;
	}

	UART_PC_clear_flag();
	return(flow_flag);
}

void ChooseStage(terminal_t terminal)
{
	switch(g_actual_status[terminal].stage)
	{
	case MAIN_MENU:
		Start_Menu(terminal);
		break;
	case CHOOSE_OPT:
		//If the ASCII code is between 48 and 57 (numbers), convert to the real number
		if((ASCII_0 <= read_data[terminal]) && (ASCII_9 >= read_data[terminal]))
		{
			Print_On_Terminal(terminal,&read_data[terminal],1);
			g_actual_status[terminal].submenu = read_data[terminal] - ASCII_0;
		}
	case SUBMENU:
	case SUBMENU_OUT:
		ChooseSubmenu(terminal);
		break;
	default:
		break;
	}
}

void ChooseSubmenu(terminal_t terminal)
{
	switch(g_actual_status[terminal].submenu){
	case INTROD_SCIA:
		break;
	case INTROD_VAL:

		break;
	default:
		break;
	}
}

void Start_Menu(terminal_t terminal)
{
	if(PC == terminal)
	{
		UART_PC_write(MainMenu,sizeof(MainMenu) - 1);
	}
	g_actual_status[terminal].stage = CHOOSE_OPT;
}

void Print_On_Terminal(terminal_t terminal, uint8_t *data, size_t length)
{
	if(PC == terminal)
	{
		UART_PC_write(data,length);
	}
}
