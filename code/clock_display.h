#ifndef CLOCK_DISPLAY_H_
#define CLOCK_DISPLAY_H_

#include "stm32f4xx_gpio.h"

/*
 * Digits represented by clock segments
 */
static const uint32_t digit_0 = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
static const uint32_t digit_1 = GPIO_Pin_7 | GPIO_Pin_8;
static const uint32_t digit_2 = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12;
static const uint32_t digit_3 = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12;
static const uint32_t digit_4 = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
static const uint32_t digit_5 = GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_12;
static const uint32_t digit_6 = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
static const uint32_t digit_7 = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
static const uint32_t digit_8 = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
static const uint32_t digit_9 = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
static const uint32_t digit_colon = GPIO_Pin_6 | GPIO_Pin_7;

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

#endif /* CLOCK_DISPLAY_H_ */
