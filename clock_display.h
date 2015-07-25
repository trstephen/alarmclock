/******************************************************************
 	file: 	clock_display.h
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
#ifndef CLOCK_DISPLAY_H_
#define CLOCK_DISPLAY_H_

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rtc.h"
#include <stdbool.h>

/****************
*	Typedefs
*****************/
typedef RTC_TimeTypeDef(*TimeUpdateFunc_T)(void);

typedef struct ClockDisplay{
	TimeUpdateFunc_T getTime_func;	// gives the time to be displayed
	uint32_t hourFormat;			// 12 or 24 hour format
	uint16_t currentSegment;	// the clock segment currently displaying a number
	bool isColonBlinking;			// true if the colon will blink at 1Hz; indicates RTC display
	bool isDisplayBlinking;		// true if the entire display blinks at 1Hz
	uint16_t blinkCounter;		// controls 1Hz blink behavior
	bool isAlarmActive;				// true if the wakeup alarm is enabled
}ClockDisplay_T;

/****************
*	Prototypes
*****************/
/*
 * ClockDisplay_Init
 *
 * input: 	null
 * 	output: null
 * 	descr: 	Initializes GPIO pins for controlling the digit selection diodes
 * 			(GPIOE) and the segment selection diodes (GPIOD).
 */
void ClockDisplay_Init();

/*
	ClockDisplay_UpdateTime

	input:	none
	output:	none
	descr:	Displays a time on the clock display one segment at a time.
*/
void ClockDisplay_UpdateTime();

/*
 * ClockDisplay_Clear
 *
 * input:	none
 * ouput:	none
 * descr:	Sets all clock segments and digits to the OFF (low) position
 */
void ClockDisplay_Clear();

/*
	ClockDisplay_IncrementClockSegment

	input:	none
	output:	none
	descr:	Move the active clock segment to the right and rollover minutes 01
					to hours 10;
*/
void ClockDisplay_IncrementClockSegment();

/*
 * ClockDisplay_AssignTimeDigit
 *
 * input:	RTC_TimeTypeDef* time
 * 				the time to be displayed
 * 	output:	uint16_t
 * 				the pins needed to display the digit that corresponds to the current clock segment
 * 	descr:	Extracts the corresponding digit from time for the requested clock segment
 * 			and returns the pins needed to display it
 */
uint16_t ClockDisplay_AssignTimeDigit(RTC_TimeTypeDef *time);

/*
	ClockDisplay_AdjustFor24HMode

	input:	RTC_TimeTypeDef* time
						the time to be displayed
	output:	*time is altered
	descr:	changes the PM hours to their 24 hour counterpars
*/
void ClockDisplay_AdjustFor24HMode(RTC_TimeTypeDef *time);

/*
	ClockDisplay_AdjustFor24HMode

	input:	uint16_t displayPins
						pins for displaying the current digit
	output:	uint16_t
						may return pins needed to clear out display
	descr:	clears the display digit for the lower half of a 1s period to simulate
					blinking with a 1Hz frequency. Allows the alarm indicator on the colon
					digit to display independently of the colon blinking.
*/
uint16_t ClockDisplay_DetermineBlinkBehavior(uint16_t displayPins);

/*
	ClockDisplay_UpdateFromRTC

	input:	none
	output:	RTC_TimeTypeDef
						The current value of the RTC in 12 hour format
	descr:	Wrapper for the RTC_GetTime function that lets it be used as a
					TimeUpdateFunc_T.
*/
RTC_TimeTypeDef ClockDisplay_UpdateFromRTC();

/*
	ClockDisplay_UpdateFromTimeSet

	input:	none
	output:	RTC_TimeTypeDef
						The current value of time setting variable
	descr:	Allows the static variable for setting the time to be used as a
					TimeUpdateFunc_T.
*/
RTC_TimeTypeDef ClockDisplay_UpdateFromTimeSet();

#endif /* CLOCK_DISPLAY_H_ */
