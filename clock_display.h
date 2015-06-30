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

// Digits on the clock display
// #defines are used because switch statements need a runtime constant (static const aren't runtime constants :( )
#define DIGIT_H10 GPIO_Pin_11
#define DIGIT_H01 GPIO_Pin_10
#define DIGIT_COLON GPIO_Pin_9
#define DIGIT_M10 GPIO_Pin_8
#define DIGIT_M01 GPIO_Pin_7
static const uint16_t digit_all = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;

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
 * ClockDisplay_Clear
 *
 * input:	none
 * ouput:	none
 * descr:	Sets all clock segments and digits to the OFF (low) position
 */
void ClockDisplay_Clear();

#endif /* CLOCK_DISPLAY_H_ */
