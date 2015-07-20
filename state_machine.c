/******************************************************************
 * file: 	state_machine.c
 * author:	T. Stephen
 * date: 	17 July, 2015
 * descr:	encapsulates the state machine of the alarm clock
 *
 ******************************************************************/
#include "buttons.h"
#include "clock_display.h"
#include "state_machine.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rtc.h"

extern volatile Button_T GBtn_Alarm;
extern volatile ClockDisplay_T GClockDisplay;

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

void State_ToggleGreenLED()
{
	GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
}

void State_SwapFunctions()
{
	if (GBtn_Alarm.shortPress_func == State_ToggleBlueLED)
	{
		GBtn_Alarm.shortPress_func = State_ToggleRedLED;
	}
	else
	{
		GBtn_Alarm.shortPress_func = State_ToggleBlueLED;
	}

	if (GBtn_Alarm.longPress_func == State_ToggleOrangeLED)
	{
		GBtn_Alarm.longPress_func = State_ToggleGreenLED;
	}
	else
	{
		GBtn_Alarm.longPress_func = State_ToggleOrangeLED;
	}
}

void State_ToggleHourFormat()
{
	if (GClockDisplay.hourFormat == RTC_HourFormat_12)
	{
		GClockDisplay.hourFormat = RTC_HourFormat_24;
	}
	else
	{
		GClockDisplay.hourFormat = RTC_HourFormat_12;
	}
}
