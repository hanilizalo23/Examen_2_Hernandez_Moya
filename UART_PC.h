/*
 * \file 	UART_PC.c
 *		This is the source file to control the Serial Port
**		connected from the k64 to the PC
 *		
 *  Created on: 12/05/2022
 *      Author: Nelida Hernández
 */

#ifndef UART_PC_H_
#define UART_PC_H_


#include "fsl_uart.h"
#include "Menus.h"
//Macros for configuration
#define BAUD_RATE_PC 				115200U
#define UART_PC						UART0
#define UART_CLK_FREQ_PC 			CLOCK_GetFreq(UART0_CLK_SRC)
#define RX_RING_BUFFER_SIZE			20U
#define PRINT_DIGS					6U

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function configures the pins needed for the k64 to PC UART transmission.
 	  	 	 In this case, RX is PTB16 and TX is PTB17. It also configures the UART0
 	  	 	 with the default configuration.
 	 \param[in]  void
 	 \return void
 */
void UART_PC_configure_port(void);
/*!
 	 \brief	 This function transmits data through the serial port to the PC.
 	 \param[in]  const uint8_t *data, size_t length (of the data)
 	 \return void
 */
void UART_PC_write(uint8_t *data, size_t length);
/*!
 	 \brief	 This function allows to read data transmited data through the serial port
 	 from the PC.
 	 \param[in]  const uint8_t *data, size_t length (of the data)
 	 \return returns 'true' if a value has been read, or false if not
 */
void UART_PC_read(uint8_t *data, size_t length);
/*!
 	 \brief	 This function returns the interruption flag to know if something has been
 	 	 	 read from the UART.
 	 \param[in]  void
 	 \return returns 'true' if a value has been read, or 'false' if not
 */
uint8_t UART_PC_get_flag(void);
/*!
 	 \brief	 This function cleans the interruption flag.
 	 \param[in]  void
 	 \return void
 */
void UART_PC_clear_flag(void);
/*!
 	 \brief	 This function prints a date on the UART in the format hh:mm:ss
 	 \param[in]  time_store_t time = Variable where the time is saved
 	 \return void
 */


#endif /* UART_PC_H_ */
