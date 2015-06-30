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
	//configure GPIO for segments
	GPIO_StructInit( initStruct );
	initStruct->GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	initStruct->GPIO_Speed = GPIO_Speed_2MHz;
	initStruct->GPIO_Mode = GPIO_Mode_OUT;
	initStruct->GPIO_OType = GPIO_OType_PP;
	initStruct->GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, initStruct);

	//configure GPIO for digit multiplexing
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
	ClockDisplay_Clear();

	uint32_t currentDigit;

	switch (*currentClockSegment)
	{
		case DIGIT_M01:
			*currentClockSegment = DIGIT_H10;
			currentDigit = display_1;
			break;
		case DIGIT_H10:
			*currentClockSegment = DIGIT_H01;
			currentDigit = display_2;
			break;
		case DIGIT_H01:
			*currentClockSegment = DIGIT_COLON;
			currentDigit = display_colon;
			break;
		case DIGIT_COLON:
			*currentClockSegment = DIGIT_M10;
			currentDigit = display_1;
			break;
		case DIGIT_M10:
			*currentClockSegment = DIGIT_M01;
			currentDigit = display_2;
			break;
		default:
			*currentClockSegment = DIGIT_H10;
			currentDigit = display_8;
			break;
	}

	// find the new time digit, later~~~~
	GPIO_ResetBits(GPIOD, *currentClockSegment);
	GPIO_SetBits(GPIOE, currentDigit);
}

void ClockDisplay_Clear()
{
	GPIO_SetBits(GPIOD, digit_all);
	GPIO_ResetBits(GPIOE, display_all);
}
