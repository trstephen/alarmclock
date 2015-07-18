/******************************************************************
 * file: 	state_machine.c
 * author:	T. Stephen
 * date: 	17 July, 2015
 * descr:	encapsulates the state machine of the alarm clock
 *
 ******************************************************************/

#include "state_machine.h"
#include "stm32f4xx_gpio.h"

void State_ButtonDisabled()
{
	/* do nothing */
}

void State_ToggleRedLED()
{
	GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
}

void State_ToggleBlueLED()
{
	GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
}

void State_ToggleOrangeLED()
{
	GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
}
