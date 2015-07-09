#ifndef CLOCK_DISPLAY_H_
#define CLOCK_DISPLAY_H_

#include "stm32f4xx_gpio.h"

/*
 * For sake of consistency, SetBits will be associated with activating a digit / segment
 * and ResetBits will turn it off. Since the display circuit is active low, unwanted (complementary)
 * digits / segments are included in the constants
 */

// Digits represented by clock segments (complement)
static const uint16_t display_0 = GPIO_Pin_12 | GPIO_Pin_13;
static const uint16_t display_1 = GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
static const uint16_t display_2 = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_13;
static const uint16_t display_3 = GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_13;
static const uint16_t display_4 = GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_13;
static const uint16_t display_5 = GPIO_Pin_7 | GPIO_Pin_10 |GPIO_Pin_13;
static const uint16_t display_6 = GPIO_Pin_7 | GPIO_Pin_13;
static const uint16_t display_7 = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
static const uint16_t display_8 = GPIO_Pin_13;
static const uint16_t display_9 = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_13;
static const uint16_t display_colon = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
static const uint16_t display_all = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;

/*
static const uint16_t display_numbers[12] = {
		GPIO_Pin_12 | GPIO_Pin_13,
		GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13,
		display_2,
		display_3,
		display_4,
		display_5,
		display_6,
		display_7,
		display_8,
		display_9,
		display_colon,
		display_all
};

typedef enum {
	Display_0 = 0,
	Display_1,
	Display_2,
	Display_3,
	Display_4,
	Display_5,
	Display_6,
	Display_7,
	Display_8,
	Display_9,
	Display_colon,
	Display_all
}ClockDisplayT_Numbers;
*/

// Digits on the clock display
// #defines are used because switch statements need a runtime constant (static const aren't runtime constants :( )
#define DIGIT_H10 GPIO_Pin_11
#define DIGIT_H01 GPIO_Pin_10
#define DIGIT_COLON GPIO_Pin_9
#define DIGIT_M10 GPIO_Pin_8
#define DIGIT_M01 GPIO_Pin_7
static const uint16_t digit_all = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;

// Global variables
extern volatile uint16_t currentClockSegment;

/*
 * ClockDisplay_GPIO_Init
 *
 * input: 	a GPIO init struct. Not necessary, since it will be cleared before
 * 		  	it is used, but this maintains symmetry with the other init calls
 * 		  	in main.
 * 	output: null
 * 	descr: 	Initializes GPIO pins for controlling the digit selection diodes
 * 			(GPIOE) and the segment selection diodes (GPIOD).
 */
void ClockDisplay_GPIO_Init(GPIO_InitTypeDef* initStruct);

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
