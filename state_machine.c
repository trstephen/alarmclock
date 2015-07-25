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

extern volatile Button_T GBtn_Music;
extern volatile Button_T GBtn_Hour;
extern volatile Button_T GBtn_Minute;
extern volatile Button_T GBtn_Time;
extern volatile Button_T GBtn_Alarm;
extern volatile ClockDisplay_T GClockDisplay;
extern volatile RTC_TimeTypeDef GNewTime;
extern volatile RTC_AlarmTypeDef GAlarm;
volatile State_T GState;

/* Placing the prototype here resolves a circular dependency between buttons.h and state_machine.h */
void State_AssignNewFunctionsToButtons(volatile Button_T* button, ButtonFunc_T shortPressFunc, ButtonFunc_T longPressFunc);

void State_UpdateState()
{
	if (GState.currentState != GState.nextState)
	{
		GState.currentState = GState.nextState;

		switch (GState.currentState) {
			case DISPLAY_RTC:
				State_DisplayRTC();
				break;
			case GET_NEW_TIME:
				State_GetNewTime();
				break;
			case GET_ALARM_TIME:
				State_GetAlarmTime();
				break;
			case PLAY_MP3:
				State_PlayMP3();
				break;
			case PLAY_AUX:
				State_PlayAux();
				break;
			case ALARM_ACTIVE:
				State_AlarmActive();
				break;
			default:
				GState.currentState = DISPLAY_RTC;
				GState.nextState = DISPLAY_RTC;
				State_DisplayRTC();
				break;
		}
	}
}

void State_AssignNewFunctionsToButtons(volatile Button_T *button, ButtonFunc_T shortPressFunc, ButtonFunc_T longPressFunc)
{
	button->shortPress_func = shortPressFunc;
	button->longPress_func = longPressFunc;
}

void State_DisplayRTC()
{
	GClockDisplay.getTime_func = ClockDisplay_UpdateFromRTC;
	GClockDisplay.isColonBlinking = true;
	GClockDisplay.isDisplayBlinking = false;

	State_AssignNewFunctionsToButtons(&GBtn_Music, ButtonFunc_Disabled, ButtonFunc_Disabled);
	State_AssignNewFunctionsToButtons(&GBtn_Hour, ButtonFunc_Disabled, ButtonFunc_Disabled);
	State_AssignNewFunctionsToButtons(&GBtn_Minute, ButtonFunc_Disabled, ButtonFunc_Disabled);
	State_AssignNewFunctionsToButtons(&GBtn_Time, ButtonFunc_ToggleHourFormat, ButtonFunc_GetNewTime);
	State_AssignNewFunctionsToButtons(&GBtn_Alarm, ButtonFunc_ToggleAlarm, ButtonFunc_GetAlarmTime);
}

void State_GetNewTime()
{
	// start setting the new time from the current time value
	GNewTime = ClockDisplay_UpdateFromRTC();

	GClockDisplay.getTime_func = ClockDisplay_UpdateFromTimeSet;
	GClockDisplay.isColonBlinking = false;
	GClockDisplay.isDisplayBlinking = false;

	State_AssignNewFunctionsToButtons(&GBtn_Music, ButtonFunc_Disabled, ButtonFunc_Disabled);
	State_AssignNewFunctionsToButtons(&GBtn_Hour, ButtonFunc_IncrementHours, ButtonFunc_IncrementHours);
	State_AssignNewFunctionsToButtons(&GBtn_Minute, ButtonFunc_IncrementMinutes, ButtonFunc_IncrementMinutes);
	State_AssignNewFunctionsToButtons(&GBtn_Time, ButtonFunc_SetNewTime, ButtonFunc_SetNewTime);
	State_AssignNewFunctionsToButtons(&GBtn_Alarm, ButtonFunc_Disabled, ButtonFunc_Disabled);

}

void State_GetAlarmTime()
{
	// start setting the new time from the current alarm time
	GNewTime = GAlarm.RTC_AlarmTime;

	GClockDisplay.getTime_func = ClockDisplay_UpdateFromTimeSet;
	GClockDisplay.isColonBlinking = false;
	GClockDisplay.isDisplayBlinking = false;

	State_AssignNewFunctionsToButtons(&GBtn_Music, ButtonFunc_Disabled, ButtonFunc_Disabled);
	State_AssignNewFunctionsToButtons(&GBtn_Hour, ButtonFunc_IncrementHours, ButtonFunc_IncrementHours);
	State_AssignNewFunctionsToButtons(&GBtn_Minute, ButtonFunc_IncrementMinutes, ButtonFunc_IncrementMinutes);
	State_AssignNewFunctionsToButtons(&GBtn_Time, ButtonFunc_Disabled, ButtonFunc_Disabled);
	State_AssignNewFunctionsToButtons(&GBtn_Alarm, ButtonFunc_SetAlarmTime, ButtonFunc_SetAlarmTime);
}

void State_PlayMP3()
{

}

void State_PlayAux()
{

}

void State_AlarmActive()
{
	/* start music playing */

	GClockDisplay.isDisplayBlinking = true;

	State_AssignNewFunctionsToButtons(&GBtn_Music, ButtonFunc_Disabled, ButtonFunc_Disabled);
	State_AssignNewFunctionsToButtons(&GBtn_Hour, ButtonFunc_Disabled, ButtonFunc_Disabled);
	State_AssignNewFunctionsToButtons(&GBtn_Minute, ButtonFunc_Disabled, ButtonFunc_Disabled);
	State_AssignNewFunctionsToButtons(&GBtn_Time, ButtonFunc_Disabled, ButtonFunc_Disabled);
	State_AssignNewFunctionsToButtons(&GBtn_Alarm, ButtonFunc_Snooze, ButtonFunc_DisableAlarm);
}
