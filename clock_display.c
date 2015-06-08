/******************************************************************
 * file: 	clock_display.c
 * author:	T. Stephen
 * date: 	7 June, 2015
 * descr:	allows the alarm clock to display a time on the
 * 			4x7-segment display.
 *
 ******************************************************************/

#include "clock_display.h"

void ClockDisplay_GPIO_Init(GPIO_InitTypeDef* initStruct)
{
	//configure GPIO for digits
	GPIO_StructInit( initStruct );
	initStruct->GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	initStruct->GPIO_Speed = GPIO_Speed_2MHz;
	initStruct->GPIO_Mode = GPIO_Mode_OUT;
	initStruct->GPIO_OType = GPIO_OType_PP;
	initStruct->GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, initStruct);

	//configure GPIO for multiplexing
	GPIO_StructInit( initStruct );
	initStruct->GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	initStruct->GPIO_Speed = GPIO_Speed_2MHz;
	initStruct->GPIO_Mode = GPIO_Mode_OUT;
	initStruct->GPIO_OType = GPIO_OType_PP;
	initStruct->GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, initStruct);
}

void ClockDisplay_Test(uint16_t *currentClockSegment)
{
	GPIO_ResetBits(GPIOD, *currentClockSegment);
	GPIO_ResetBits(GPIOE, digit_8);  // clears all digits
	uint32_t currentDigit;

	switch (*currentClockSegment)
	{
		case GPIO_Pin_7:  // H10
			*currentClockSegment = GPIO_Pin_8;
			currentDigit = digit_1;
			break;
		case GPIO_Pin_8:  // H1
			*currentClockSegment = GPIO_Pin_9;
			currentDigit = digit_2;
			break;
		case GPIO_Pin_9:  // :
			*currentClockSegment = GPIO_Pin_10;
			currentDigit = digit_colon;
			break;
		case GPIO_Pin_10: // M10
			*currentClockSegment = GPIO_Pin_11;
			currentDigit = digit_3;
			break;
		case GPIO_Pin_11: // M1
			*currentClockSegment = GPIO_Pin_7;
			currentDigit = digit_4;
			break;
		default:
			*currentClockSegment = GPIO_Pin_7;
			currentDigit = digit_8;
			break;
	}

	// find the new time digit, but it's always 8 for now ;)
	GPIO_SetBits(GPIOE, currentDigit);
	GPIO_SetBits(GPIOD, currentClockSegment);
}
