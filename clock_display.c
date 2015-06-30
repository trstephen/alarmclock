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

void ClockDisplay_TimeTest(uint16_t *currentClockSegment)
{
	ClockDisplay_Clear();

	RTC_TimeTypeDef time;

	RTC_GetTime(RTC_HourFormat_12, &time);

	switch (*currentClockSegment)
	{
		case DIGIT_M01:
			*currentClockSegment = DIGIT_H10;
			break;
		case DIGIT_H10:
			*currentClockSegment = DIGIT_H01;
			break;
		case DIGIT_H01:
			*currentClockSegment = DIGIT_COLON;
			break;
		case DIGIT_COLON:
			*currentClockSegment = DIGIT_M10;
			break;
		case DIGIT_M10:
			*currentClockSegment = DIGIT_M01;
			break;
		default:
			*currentClockSegment = DIGIT_H10;
			break;
	}

	uint16_t currentDigit = ClockDisplay_AssignTimeDigit(currentClockSegment, &time);

	GPIO_ResetBits(GPIOD, *currentClockSegment);
	GPIO_SetBits(GPIOE, currentDigit);
}

uint16_t ClockDisplay_AssignTimeDigit(uint16_t *currentClockSegment, RTC_TimeTypeDef *time)
{
	uint16_t currentDigit = 0;

	switch (*currentClockSegment) {
		case DIGIT_H10:
			currentDigit = time->RTC_Minutes & 0x0F0;
			break;
		case DIGIT_H01:
			currentDigit = time->RTC_Minutes & 0x00F;
			break;
		case DIGIT_COLON:
			currentDigit = display_colon;
			break;
		case DIGIT_M10:
			currentDigit = time->RTC_Seconds & 0x0F0;
			break;
		case DIGIT_M01:
			currentDigit = time->RTC_Seconds & 0x00F;
			break;
		default:
			currentDigit = time->RTC_Seconds & 0x00F;
			break;
	}

	uint16_t displayPins = 0;

	switch (currentDigit) {
		case 0:
			displayPins = display_0;
			break;
		case 1:
			displayPins = display_1;
			break;
		case 2:
			displayPins = display_2;
			break;
		case 3:
			displayPins = display_3;
			break;
		case 4:
			displayPins = display_4;
			break;
		case 5:
			displayPins = display_5;
			break;
		case 6:
			displayPins = display_6;
			break;
		case 7:
			displayPins = display_7;
			break;
		case 8:
			displayPins = display_8;
			break;
		case 9:
			displayPins = display_9;
			break;
		default:
			break;
	}

	return displayPins;
}

void ClockDisplay_Clear()
{
	GPIO_SetBits(GPIOD, digit_all);
	GPIO_ResetBits(GPIOE, display_all);
}
