#ifndef CLOCK_DISPLAY_H_
#define CLOCK_DISPLAY_H_

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rtc.h"
#include <stdbool.h>

typedef RTC_TimeTypeDef(*TimeUpdateFunc_T)(void);

typedef struct ClockDisplay{
	TimeUpdateFunc_T getTime_func;
	uint32_t hourFormat;
	uint16_t currentSegment;
	bool isColonBlinking;
	bool isDisplayBlinking;
	uint16_t blinkCounter;
}ClockDisplay_T;

// helper enumerator so we can use indexing for numbers[] but still
// access the colon and all segments values which have no obvious index
enum extraNumbers {
		COLON = 10,
		AM_PM,
		ALL,
};

// a sum of pins needed to display numbers 0-9. Array index corresponds to display number
static const uint16_t numbers[13] = {
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
/* ALL */	GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13
};
static const GPIO_TypeDef* display_bank = GPIOE;

// Digits on the clock display
// #defines are used because switch statements need a runtime constant (static const aren't runtime constants :( )
#define DIGIT_H10 (GPIO_Pin_11)
#define DIGIT_H01 (GPIO_Pin_10)
#define DIGIT_COLON (GPIO_Pin_9)
#define DIGIT_M10 (GPIO_Pin_8)
#define DIGIT_M01 (GPIO_Pin_7)
static const uint16_t digit_all = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
static const GPIO_TypeDef* digit_bank = GPIOD;

// Global variables
extern volatile ClockDisplay_T GClockDisplay;

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
 * ClockDisplay_AssignTimeDigit
 *
 * input:	RTC_TimeTypeDef* time
 * 				current value of the RTC clock
 * 	output:	uint16_t
 * 				the pins needed to display the digit that corresponds to the current clock segment
 * 	descr:	Extracts the corresponding digit from time for the requested clock segment
 * 			and returns the pins needed to display it
 */
uint16_t ClockDisplay_AssignTimeDigit(RTC_TimeTypeDef *time);

uint16_t ClockDisplay_AssignTimeDigitMinSec(RTC_TimeTypeDef *time);

/*
 * ClockDisplay_Clear
 *
 * input:	none
 * ouput:	none
 * descr:	Sets all clock segments and digits to the OFF (low) position
 */
void ClockDisplay_Clear();

void ClockDisplay_UpdateTime();

void ClockDisplay_IncrementClockSegment();

uint16_t ClockDisplay_DetermineBlinkBehavior(uint16_t displayPins);

RTC_TimeTypeDef ClockDisplay_UpdateFromRTC();
RTC_TimeTypeDef ClockDisplay_UpdateFromTimeSet();
RTC_TimeTypeDef ClockDisplay_UpdateFromAlarmSet();

#endif /* CLOCK_DISPLAY_H_ */
