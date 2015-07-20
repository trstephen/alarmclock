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

void ClockDisplay_UpdateTime()
{
	ClockDisplay_Clear();

	ClockDisplay_IncrementClockSegment();

	RTC_TimeTypeDef time = clockDisplay.getTime_func();

	uint16_t currentDigit = ClockDisplay_AssignTimeDigit(&time);

	GPIO_ResetBits(digit_bank, clockDisplay.currentSegment);
	GPIO_SetBits(display_bank, currentDigit);
}

void ClockDisplay_Clear()
{
	GPIO_SetBits(digit_bank, digit_all);
	GPIO_ResetBits(display_bank, numbers[ALL]);
}

void ClockDisplay_IncrementClockSegment()
{
	switch (clockDisplay.currentSegment)
	{
		case DIGIT_M01:
			clockDisplay.currentSegment = DIGIT_H10;
			break;
		case DIGIT_H10:
			clockDisplay.currentSegment = DIGIT_H01;
			break;
		case DIGIT_H01:
			clockDisplay.currentSegment = DIGIT_COLON;
			break;
		case DIGIT_COLON:
			clockDisplay.currentSegment = DIGIT_M10;
			break;
		case DIGIT_M10:
			clockDisplay.currentSegment = DIGIT_M01;
			break;
		default:
			clockDisplay.currentSegment = DIGIT_H10;
			break;
	}
}

uint16_t ClockDisplay_AssignTimeDigit(RTC_TimeTypeDef *time)
{
	uint16_t currentDigit = 0;

	// determine the current digit to display by masking
	// the appropriate RTC value
	switch (clockDisplay.currentSegment) {
		case DIGIT_H10:
			currentDigit = time->RTC_Minutes & 0x0F0;
			currentDigit = currentDigit >> 4;
			break;
		case DIGIT_H01:
			currentDigit = time->RTC_Minutes & 0x00F;
			break;
		case DIGIT_COLON:
			currentDigit = COLON;
			break;
		case DIGIT_M10:
			currentDigit = time->RTC_Seconds & 0x0F0;
			currentDigit = currentDigit >> 4;
			break;
		case DIGIT_M01:
			currentDigit = time->RTC_Seconds & 0x00F;
			break;
		default:
			currentDigit = 6;
			break;
	}

	// ensure that current digit is between 0 and 9 and assign
	// the appropriate display pins.
//	currentDigit = currentDigit % 10; // good idea, but this kills digit[10] = COLON
	uint16_t displayPins = numbers[currentDigit];

	// detect the edge case where there's a leading 0 in H10
	// and clear the display value
	if ( (clockDisplay.currentSegment == DIGIT_H10)
		&& (currentDigit == 0x0) )
		{
			displayPins = numbers[ALL]; // despite the awkward names, this illuminates no segments
		}

	return displayPins;
}

RTC_TimeTypeDef ClockDisplay_UpdateFromRTC()
{
	RTC_TimeTypeDef time;

	RTC_GetTime(clockDisplay.hourFormat, &time);

	return time;
}
