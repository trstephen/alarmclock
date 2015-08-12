/******************************************************************
 	file: 	clock_display.c
 	author:	T. Stephen
 	date: 	7 June, 2015
 	descr:	Handles all behavior for the LED clock display. It translates a
			RTC_TimeTypeDef into the correct combination of display segments.
			Since the RTC clock is always in 12 hour mode, the display library
			creates 24 hour mode by altering the display of the RTC clock rather
			than its value or operating mode. GClockDisplay.getTime_func can
			be changed by the state machine to display the current RTC value or
			the value for time or alarm setting.
 ******************************************************************/

#include "clock_display.h"
#include "state_machine.h"

/****************
*	Global Variables
*****************/
volatile RTC_TimeTypeDef GNewTime;
extern volatile ClockDisplay_T GClockDisplay;
extern volatile RTC_AlarmTypeDef GAlarm;
extern volatile State_T GState;

/****************
*	Enumerators
*****************/
// helper enumerator so we can use indexing for numbers[] but still
// access the colon and all segments values which have no obvious index
enum extraNumbers {
		COLON = 10,
		AM_PM,
		ALARM,
		NONE,
};

/****************
*	Constants
*****************/
// a sum of pins needed to display numbers 0-9. Array index corresponds to display number
// usage:
//		display[7] returns all of the segments needed to display a 7 on the clock
static const uint16_t numbers[14] = {
/* 0 */ 	GPIO_Pin_12 | GPIO_Pin_13,
/* 1 */ 	GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13,
/* 2 */ 	GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_13,
/* 3 */ 	GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_13,
/* 4 */ 	GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_13,
/* 5 */		GPIO_Pin_7 | GPIO_Pin_10 |GPIO_Pin_13,
/* 6 */ 	GPIO_Pin_7 | GPIO_Pin_13,
/* 7 */ 	GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13,
/* 8 */ 	GPIO_Pin_13,
/* 9 */ 	GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_13,
/* COLON */ GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13,
/* AM_PM */ GPIO_Pin_13,
/* ALARM */ GPIO_Pin_8,
/* NONE */	GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13
};
static const GPIO_TypeDef* digit_bank = GPIOD;
static const GPIO_TypeDef* display_bank = GPIOE;

// Digits on the clock display
static const uint16_t digit_all = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;

/****************
*	Functions
*****************/
void ClockDisplay_Init()
{
	GPIO_InitTypeDef initStruct;

	//configure GPIO for segments
	GPIO_StructInit( &initStruct );
	initStruct.GPIO_Pin = numbers[NONE];
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

	GPIO_SetBits(display_bank, currentDigit);
	GPIO_ResetBits(digit_bank, GClockDisplay.currentSegment);
}

void ClockDisplay_Clear()
{
	GPIO_SetBits(digit_bank, digit_all);
	GPIO_ResetBits(display_bank, numbers[NONE]);
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
	// the RTC library can't toggle nicely between 12/24 formats
	// so we'll always work in 12h and add +12h to visually
	// correct for 24h format
	if (GClockDisplay.hourFormat == RTC_HourFormat_24)
	{
		ClockDisplay_AdjustFor24HMode(time);
	}

	uint16_t currentDigit = 0;

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
		displayPins = numbers[NONE];
	}

	// turn on the AM/PM indicator
	if (GClockDisplay.currentSegment == DIGIT_M01
			&& GClockDisplay.hourFormat == RTC_HourFormat_12
			&& time->RTC_H12 == RTC_H12_PM)
	{
		displayPins = displayPins & ~(numbers[AM_PM]);
	}

	// alarm active indicator
	if (GClockDisplay.currentSegment == DIGIT_COLON
			&& GState.isAlarmSet == true)
	{
		displayPins = displayPins & ~(numbers[ALARM]);
	}

	displayPins = ClockDisplay_DetermineBlinkBehavior(displayPins);

	return displayPins;
}

void ClockDisplay_AdjustFor24HMode(RTC_TimeTypeDef *time)
{
	// midnight = 0h00
	if (time->RTC_H12 == RTC_H12_AM
			&& time->RTC_Hours == 0x012)
	{
		time->RTC_Hours = 0x00;
	}

	// shift 12->11pm to 12h->23h
	if (time->RTC_H12 == RTC_H12_PM)
	{
		switch (time->RTC_Hours) {
			case 0x012:
				time->RTC_Hours = 0x012;
				break;
			case 0x01:
				time->RTC_Hours = 0x013;
				break;
			case 0x02:
				time->RTC_Hours = 0x014;
				break;
			case 0x03:
				time->RTC_Hours = 0x015;
				break;
			case 0x04:
				time->RTC_Hours = 0x016;
				break;
			case 0x05:
				time->RTC_Hours = 0x017;
				break;
			case 0x06:
				time->RTC_Hours = 0x018;
				break;
			case 0x07:
				time->RTC_Hours = 0x019;
				break;
			case 0x08:
				time->RTC_Hours = 0x020;
				break;
			case 0x09:
				time->RTC_Hours = 0x021;
				break;
			case 0x010:
				time->RTC_Hours = 0x022;
				break;
			case 0x011:
				time->RTC_Hours = 0x023;
				break;
			default:
				time->RTC_Hours = 0x088;
				break;
		}
	}
}

uint16_t ClockDisplay_DetermineBlinkBehavior(uint16_t displayPins)
{
	// since the display is being updated at 500Hz, toggling the blink
	// state every ~250 ticks will  give the appearance of a 1s period
	GClockDisplay.blinkCounter = ++GClockDisplay.blinkCounter % 500;

	// blink the entire display
	if (GClockDisplay.isDisplayBlinking == true
			&& GClockDisplay.blinkCounter < 250)
	{
		displayPins = numbers[NONE];
	}
	// blink the colon but not the alarm indicator
	else if (GClockDisplay.currentSegment == DIGIT_COLON
			&& GClockDisplay.blinkCounter < 250
			&& GState.isAlarmSet == true
			&& GClockDisplay.isColonBlinking == true)
	{
		displayPins = numbers[NONE] & ~(numbers[ALARM]);
	}
	// just blink the colon
	else if (GClockDisplay.currentSegment == DIGIT_COLON
			&& GClockDisplay.blinkCounter < 250
			&& GState.isAlarmSet == false
			&& GClockDisplay.isColonBlinking == true)
	{
		displayPins = numbers[NONE];
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
