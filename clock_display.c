/******************************************************************
 * file: 	clock_display.c
 * author:	T. Stephen
 * date: 	7 June, 2015
 * descr:	allows the alarm clock to display a time on the
 * 			4x7-segment display.
 *
 ******************************************************************/

#include "clock_display.h"

void ClockDisplay_Init()
{
	GPIO_InitTypeDef initStruct;

	//configure GPIO for segments
	GPIO_StructInit( &initStruct );
	initStruct.GPIO_Pin = numbers[ALL];
	initStruct.GPIO_Speed = GPIO_Speed_2MHz;
	initStruct.GPIO_Mode = GPIO_Mode_OUT;
	initStruct.GPIO_OType = GPIO_OType_PP;
	initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &initStruct);

	//configure GPIO for digit multiplexing
	GPIO_StructInit( &initStruct );
	initStruct.GPIO_Pin = digit_all;
	initStruct.GPIO_Speed = GPIO_Speed_2MHz;
	initStruct.GPIO_Mode = GPIO_Mode_OUT;
	initStruct.GPIO_OType = GPIO_OType_PP;
	initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &initStruct);
}

void ClockDisplay_GPIO_Init_Oops(GPIO_InitTypeDef* initStruct)
{
	//configure GPIO for segments
	GPIO_StructInit( initStruct );
	initStruct->GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10;
	initStruct->GPIO_Speed = GPIO_Speed_2MHz;
	initStruct->GPIO_Mode = GPIO_Mode_OUT;
	initStruct->GPIO_OType = GPIO_OType_PP;
	initStruct->GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, initStruct);

	//configure GPIO for digit multiplexing
	GPIO_StructInit( initStruct );
	initStruct->GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
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
			currentDigit = numbers[1];
			break;
		case DIGIT_H10:
			*currentClockSegment = DIGIT_H01;
			currentDigit = numbers[2];
			break;
		case DIGIT_H01:
			*currentClockSegment = DIGIT_COLON;
			currentDigit = numbers[COLON];
			break;
		case DIGIT_COLON:
			*currentClockSegment = DIGIT_M10;
			currentDigit = numbers[3];
			break;
		case DIGIT_M10:
			*currentClockSegment = DIGIT_M01;
			currentDigit = numbers[4];
			break;
		default:
			*currentClockSegment = DIGIT_H10;
			currentDigit = numbers[8];
			break;
	}

	// find the new time digit, later~~~~
	GPIO_ResetBits(GPIOD, *currentClockSegment);
	GPIO_SetBits(GPIOE, currentDigit);
}

void ClockDisplay_Test_Oops(uint16_t *currentClockSegment, GPIO_TypeDef* GPIOx, uint16_t H10, uint16_t H01)
{
	GPIO_SetBits(GPIOx, H10 | H01);
	GPIO_ResetBits(GPIOE, numbers[ALL]);

	uint32_t currentDigit;

	if (*currentClockSegment == H01)
	{
		*currentClockSegment = H10;
		currentDigit = numbers[2];
	}
	else if (*currentClockSegment == H10)
	{
		*currentClockSegment = H01;
		currentDigit = numbers[1];
	}
	else
	{
		*currentClockSegment = H01;
		currentDigit = numbers[8];
	}

	GPIO_SetBits(GPIOE, currentDigit);
	GPIO_ResetBits(GPIOx, *currentClockSegment);
}

void ClockDisplay_TimeTest()
{
	ClockDisplay_Clear();

	switch (currentClockSegment)
	{
		case DIGIT_M01:
			currentClockSegment = DIGIT_H10;
			break;
		case DIGIT_H10:
			currentClockSegment = DIGIT_H01;
			break;
		case DIGIT_H01:
			currentClockSegment = DIGIT_COLON;
			break;
		case DIGIT_COLON:
			currentClockSegment = DIGIT_M10;
			break;
		case DIGIT_M10:
			currentClockSegment = DIGIT_M01;
			break;
		default:
			currentClockSegment = DIGIT_H10;
			break;
	}

	RTC_TimeTypeDef time;

	RTC_GetTime(RTC_HourFormat_12, &time);

	uint16_t currentDigit = ClockDisplay_AssignTimeDigit(&time);

	GPIO_ResetBits(digit_bank, currentClockSegment);
	GPIO_SetBits(display_bank, currentDigit);
}

uint16_t ClockDisplay_AssignTimeDigit(RTC_TimeTypeDef *time)
{
	uint16_t currentDigit = 0;

	// determine the current digit to display by masking
	// the appropriate RTC value
	switch (currentClockSegment) {
		case DIGIT_H10:
			currentDigit = time->RTC_Minutes & 0x0F0;
			break;
		case DIGIT_H01:
			currentDigit = time->RTC_Minutes & 0x00F;
			break;
		case DIGIT_COLON:
			currentDigit = numbers[COLON];
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

	// ensure that current digit is between 0 and 9 and assign
	// the appropriate display pins.
	currentDigit = currentDigit % 10;
	uint16_t displayPins = numbers[currentDigit];

	// detect the edge case where there's a leading 0 in H10
	// and clear the display value
	if ( (currentClockSegment == DIGIT_H10)
		&& (currentDigit == 0x0) )
		{
			displayPins = numbers[ALL]; // despite the awkward names, this illuminates no segments
		}

	return displayPins;
}

void ClockDisplay_Clear()
{
	GPIO_SetBits(digit_bank, digit_all);
	GPIO_ResetBits(display_bank, numbers[ALL]);
}
