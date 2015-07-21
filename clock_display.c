/******************************************************************
 * file: 	clock_display.c
 * author:	T. Stephen
 * date: 	7 June, 2015
 * descr:	allows the alarm clock to display a time on the
 * 			4x7-segment display.
 *
 ******************************************************************/

#include "clock_display.h"

static const GPIO_TypeDef* digit_bank = GPIOD;
static const GPIO_TypeDef* display_bank = GPIOE;

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
	GPIO_Init(display_bank, &initStruct);

	//configure GPIO for digit multiplexing
	GPIO_StructInit( &initStruct );
	initStruct.GPIO_Pin = digit_all;
	initStruct.GPIO_Speed = GPIO_Speed_2MHz;
	initStruct.GPIO_Mode = GPIO_Mode_OUT;
	initStruct.GPIO_OType = GPIO_OType_PP;
	initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(digit_bank, &initStruct);
}

void ClockDisplay_UpdateTime()
{
	ClockDisplay_Clear();

	ClockDisplay_IncrementClockSegment();

	RTC_TimeTypeDef time = GClockDisplay.getTime_func();

	uint16_t currentDigit = ClockDisplay_AssignTimeDigit(&time);
//	uint16_t currentDigit = ClockDisplay_AssignTimeDigitMinSec(&time);

	GPIO_SetBits(display_bank, currentDigit);
	GPIO_ResetBits(digit_bank, GClockDisplay.currentSegment);
}

void ClockDisplay_Clear()
{
	GPIO_SetBits(digit_bank, digit_all);
	GPIO_ResetBits(display_bank, numbers[ALL]);
}

void ClockDisplay_IncrementClockSegment()
{
	switch (GClockDisplay.currentSegment)
	{
		case DIGIT_M01:
			GClockDisplay.currentSegment = DIGIT_H10;
			break;
		case DIGIT_H10:
			GClockDisplay.currentSegment = DIGIT_H01;
			break;
		case DIGIT_H01:
			GClockDisplay.currentSegment = DIGIT_COLON;
			break;
		case DIGIT_COLON:
			GClockDisplay.currentSegment = DIGIT_M10;
			break;
		case DIGIT_M10:
			GClockDisplay.currentSegment = DIGIT_M01;
			break;
		default:
			GClockDisplay.currentSegment = DIGIT_H10;
			break;
	}
}

uint16_t ClockDisplay_AssignTimeDigit(RTC_TimeTypeDef *time)
{
	uint16_t currentDigit = 0;

	// the RTC library can't toggle nicely between 12/24 formats
	// so we'll always work in 12h and add +12h to visually
	// correct for 24h format
	if (GClockDisplay.hourFormat == RTC_HourFormat_24
			&& time->RTC_H12 == RTC_H12_PM)
	{
		time->RTC_Hours = time->RTC_Hours + 0x012;
	}
	// display midnight as 0:00
	else if (GClockDisplay.hourFormat == RTC_HourFormat_24
			&& time->RTC_H12 == RTC_H12_AM
			&& time->RTC_Hours == 0x0)
	{
		time->RTC_Hours = time->RTC_Hours - 0x012;
	}

	// determine the current digit to display by masking
	// the appropriate RTC value
	switch (GClockDisplay.currentSegment) {
		case DIGIT_H10:
			currentDigit = time->RTC_Hours & 0x0F0;
			currentDigit = currentDigit >> 4;
			break;
		case DIGIT_H01:
			currentDigit = time->RTC_Hours & 0x00F;
			break;
		case DIGIT_COLON:
			currentDigit = COLON;
			break;
		case DIGIT_M10:
			currentDigit = time->RTC_Minutes & 0x0F0;
			currentDigit = currentDigit >> 4;
			break;
		case DIGIT_M01:
			currentDigit = time->RTC_Minutes & 0x00F;
			break;
		default:
			currentDigit = 6; // hail satan!
			break;
	}

	uint16_t displayPins = numbers[currentDigit];

	// detect the edge case where there's a leading 0 in H10
	// and clear the display value
	if ( GClockDisplay.currentSegment == DIGIT_H10
			&& currentDigit == 0x0)
	{
		displayPins = numbers[ALL]; // despite the awkward names, this illuminates no segments
	}

	// turn on the AM/PM indicator
	if (GClockDisplay.currentSegment == DIGIT_M01
			&& GClockDisplay.hourFormat == RTC_HourFormat_12
			&& time->RTC_H12 == RTC_H12_PM)
	{
		displayPins = displayPins & ~(numbers[AM_PM]);
	}

	displayPins = ClockDisplay_DetermineBlinkBehavior(displayPins);

	return displayPins;
}

uint16_t ClockDisplay_AssignTimeDigitMinSec(RTC_TimeTypeDef *time)
{
	uint16_t currentDigit = 0;

	switch (GClockDisplay.currentSegment) {
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

	uint16_t displayPins = numbers[currentDigit];

	if ( (GClockDisplay.currentSegment == DIGIT_H10)
		&& (currentDigit == 0x0) )
		{
			displayPins = numbers[ALL]; // despite the awkward names, this illuminates no segments
		}

	return displayPins;
}

uint16_t ClockDisplay_DetermineBlinkBehavior(uint16_t displayPins)
{
	// since the display is being updated at 500Hz, toggling the blink
	// state every ~250 ticks will  give the appearance of a 1s period
	GClockDisplay.blinkCounter = ++GClockDisplay.blinkCounter % 500;

	if (GClockDisplay.isDisplayBlinking == true
			&& GClockDisplay.blinkCounter < 250)
	{
		displayPins = numbers[ALL];
	}
	else if (GClockDisplay.currentSegment == DIGIT_COLON
			&& GClockDisplay.isColonBlinking == true
			&& GClockDisplay.blinkCounter < 250 )
	{
		displayPins = numbers[ALL];
	}

	return displayPins;
}

RTC_TimeTypeDef ClockDisplay_UpdateFromRTC()
{
	RTC_TimeTypeDef time;

	RTC_GetTime(RTC_HourFormat_12, &time);

	return time;
}

RTC_TimeTypeDef ClockDisplay_UpdateFromTimeSet()
{
	return GNewTime;
}
