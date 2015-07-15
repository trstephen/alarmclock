#ifndef CLOCK_DISPLAY_H_
#define CLOCK_DISPLAY_H_

#include "stm32f4xx_gpio.h"

/*
 * For sake of consistency, SetBits will be associated with activating a digit / segment
 * and ResetBits will turn it off. Since the display circuit is active low, unwanted (complementary)
 * digits / segments are included in the constants
 */


static const GPIO_TypeDef* display_bank = GPIOE;

// helper enumerator so we can use indexing for numbers[] but still
// access the colon and all segments values which have no obvious index
enum extraNumbers {
		COLON = 10,
		ALL,
};

// a sum of pins needed to display numbers 0-9. Array index corresponds to display number
static const uint16_t numbers[12] = {
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
/* ALL */	GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13
};

// Digits on the clock display
// #defines are used because switch statements need a runtime constant (static const aren't runtime constants :( )
#define DIGIT_H10 GPIO_Pin_11
#define DIGIT_H01 GPIO_Pin_10
#define DIGIT_COLON GPIO_Pin_9
#define DIGIT_M10 GPIO_Pin_8
#define DIGIT_M01 GPIO_Pin_7
static const uint16_t digit_all = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
static const GPIO_TypeDef* digit_bank = GPIOD;

// Global variables
extern volatile uint16_t currentClockSegment;

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
 * ClockDisplay_Test
 *
 * input:	uint16_t* currentClockSegment
 * 				the pin connected to the transistor that turns an entire
 * 				clock digit on or off
 * output: 	null
 * descr:	Displays "12:34" on the clock
 */
void ClockDisplay_Test(uint16_t *currentClockSegment);

/*
 * ClockDisplay_TimeTest
 *
 * input:	none
 * output:	none
 * descr:	Takes decodes the current time for myClockStruct (global) and displays it
 */
void ClockDisplay_TimeTest();

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

/*
 * ClockDisplay_Clear
 *
 * input:	none
 * ouput:	none
 * descr:	Sets all clock segments and digits to the OFF (low) position
 */
void ClockDisplay_Clear();

void ClockDisplay_GPIO_Init_Oops(GPIO_InitTypeDef* initStruct);

void ClockDisplay_Test_Oops(uint16_t *currentClockSegment, GPIO_TypeDef* GPIOx, uint16_t H10, uint16_t H01);

#endif /* CLOCK_DISPLAY_H_ */
