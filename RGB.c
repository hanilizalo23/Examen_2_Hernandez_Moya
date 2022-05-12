/**
       \file
       \brief
                    This is the source file for the colors needed in the main implementation.
       \author Nelida Paulina Hernández Moya
       \date 15/02/2022
*/

#include <stdio.h>
#include <stdint.h>
#include "MK64F12.h"
#include "GPIO.h"
#include "RGB.h"
#include "bits.h"
#include "PIT.h"

#define RESO 1U

//Variable to save a maximum of 10 colors for the sequence
static color_sequence_t g_colors_sequence [10] = {0};

typedef enum {seqmin, seqmax = 2}range_seq; /**Enum for the limits of the sequence*/
typedef enum {seqnw, seqsw2, seqsw3, seqsw23}number_seq; /**Enum for the values of the sequence*/
typedef enum {color0, color1, color2}number_color; /**Enum for the three colors of every sequence*/

/**Number of the pins for the reading*/
uint8_t sw2pin = 6;
uint8_t sw3pin = 4;

/**Variables for color and sequence*/
uint8_t color = 0;
uint8_t sequence = 0;

//Counter to get 1 second
static uint32_t g_counter_1s = 0x00;
//Counter for the sequence of submenu 3
static uint16_t g_sequence_counter = 0x00;
//Variable that stores the total value of colors in the sequence
static uint16_t g_sequence_total = 0x00;
//Stores the colors in the sequence to use them on callbacks.
static color_sequence_t *g_colors_seq = 0x00;
//Stores if the sequence is specified to be stopped
static continue_seq_t g_stop_flag = FALSE;

//RGB codes of the colors for the sequence
rgb_intensity_colors_t g_sequence_rgb_code[7] =
{
		{0,		0,		0	},			//NONE
		{0, 	0, 	 	255	},			//BLUE
		{255, 	0, 	 	0	},			//RED
		{0, 	255, 	0	},			//GREEN
		{255, 	255, 	0	},			//YELLOW
		{255, 	0, 		255	},			//PURPLE
		{255, 	255, 	255	},			//WHITE
};

/*For Sequence*/
void RGB_led_start_sequence(color_sequence_t *colors,uint8_t sequence_total,continue_seq_t stop)
{
	g_colors_seq = colors;
	g_sequence_total = sequence_total - RESO;
	g_stop_flag = stop;
	//Turn off the first color
	RGB_led_set_color(&g_sequence_rgb_code[colors[0]]);
	//Enables the interruption to get 1sec delay
	RGB_led_delay_1s_init();
}

void RGB_led_change_seq_color(void)
{
	//If the sequence must stop and this is the last color...
	if((g_stop_flag) && (g_sequence_total == g_sequence_counter))
	{
		g_sequence_counter = 0x00;
		RGB_led_stop_sequence();
	}
	else if((!g_stop_flag) && (g_sequence_total == g_sequence_counter))
	{
		g_sequence_counter = 0x00;
		RGB_led_set_color(&g_sequence_rgb_code[g_colors_seq[g_sequence_counter]]);
	}
	else
	{
		g_sequence_counter++;
		RGB_led_set_color(&g_sequence_rgb_code[g_colors_seq[g_sequence_counter]]);
	}
}

void SW3_cb(void) /**Setter for the callback of sw3 and both reading*/
{
	/**Starting the color of this sequence as the first*/
	color = color0;

	/**Variables for reading*/
	uint8_t sw2_read;
	uint8_t sw3_read;
	uint8_t sw_read;

	/**Reading the values of the switches*/
	sw2_read = GPIO_read_pin(GPIO_C, sw2pin);
	sw3_read = GPIO_read_pin(GPIO_A, sw3pin);

	/**An OR of the values*/
	sw_read = sw2_read | sw3_read;

	if (seqmin == sw_read) /**If the value is 0, it means, both switches are pressed, is the turn of the fourth sequence and cleaning the status*/
	{
		sequence = seqsw23;
		GPIO_clear_irq_status(GPIO_A);
		GPIO_clear_irq_status(GPIO_C);
	}
	else /**If the value is only for the switch 3, third sequence is going to run*/
	{
		sequence = seqsw3;
		GPIO_clear_irq_status(GPIO_A);
	}
}

void SW2_cb(void) /**Setter for the callback of sw2*/
{
	color = color0;
	sequence = seqsw2;
	GPIO_clear_irq_status(GPIO_C);
}

void PIT_cb(void) /**Callback for the PIT*/
{
	color++; /**Go switching colors*/

	if(seqmax < color) /**As long as the color sequence is behind the max limit, reset it*/
	{
		color = color0;
	}

	PIT_clear_interrupt_flag(); /**Cleaning the flag of PIT*/
}

/**Getters for the sequence and color*/

uint8_t get_sequence(void)
{
	return sequence;
}

uint8_t get_color(void)
{
	return color;
}

void green(void) /**Turn on the green LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_E, bit_26);
	GPIO_set_pin(GPIO_E, bit_26);
}

void blue(void) /**Turn on the blue LED, waiting and turning it off*/
{
    GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_21);
}

void purple(void) /**Turn on the purple LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);
}

void red(void) /**Turn on the red LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_B, bit_22);
}

void yellow(void) /**Turn on the yellow LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_E, bit_26);
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_E, bit_26);
}

void white(void) /**Turn on the white LED, waiting and turning it off*/
{
	GPIO_clear_pin(GPIO_B, bit_21);
	GPIO_clear_pin(GPIO_B, bit_22);
	GPIO_clear_pin(GPIO_E, bit_26);
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_E, bit_26);
}

void no_color(void) /**Turn on none color*/
{
	GPIO_set_pin(GPIO_B, bit_21);
	GPIO_set_pin(GPIO_B, bit_22);
	GPIO_set_pin(GPIO_E, bit_26);
}
