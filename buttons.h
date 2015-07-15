#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "stm32f4xx_exti.h"
#include "stm32f4xx_gpio.h"
#include <stdbool.h>

// Define the pins for each switch
#define SWITCH_1 (GPIO_Pin_6)
#define SWITCH_2 (GPIO_Pin_7)
#define SWITCH_3 (GPIO_Pin_8)
#define SWITCH_4 (GPIO_Pin_9)
#define SWITCH_5 (GPIO_Pin_10)
static const GPIO_TypeDef* button_bank = GPIOC;

typedef  struct
{
	uint16_t pin;			// the GPIO pin assigned to the button
	uint32_t interruptLine; // button attached to this line
	bool isPressed; 		// true if button is currently pressed
	bool isLongPress;		// true if button has received a long press
}Button_T;

// Global variables for buttons
static volatile Button_T GBtn_Music = {
	.pin = GPIO_Pin_6,
	.interruptLine = EXTI_Line6,
	.isPressed = false,
	.isLongPress = false
};

static volatile Button_T GBtn_Hour = {
	.pin = GPIO_Pin_7,
	.interruptLine = EXTI_Line7,
	.isPressed = false,
	.isLongPress = false
};

static volatile Button_T GBtn_Minute = {
	.pin = GPIO_Pin_8,
	.interruptLine = EXTI_Line8,
	.isPressed = false,
	.isLongPress = false
};

static volatile Button_T GBtn_Time = {
	.pin = GPIO_Pin_9,
	.interruptLine = EXTI_Line9,
	.isPressed = false,
	.isLongPress = false
};

static volatile Button_T GBtn_Alarm = {
	.pin = GPIO_Pin_10,
	.interruptLine = EXTI_Line10,
	.isPressed = false,
	.isLongPress = false
};



/*
 * Buttons_Init
 *
 *  input: 	none
 * 	output: null
 * 	descr: 	Initializes GPIOC pins for button handling. Set as NOPULL since the
 * 			pull-down resistors are on the board. Meh.
 * 			Connects Switch 1-4 to EXTI9-5 and Switch 5 to EXTI15-10.
 * 			The handlers are in main().
 */
void Buttons_Init();

/*
 * Buttons_LinkPinToInterrupt
 *
 *  input:	uint8_t pinSource
 *  			The pin to associate with the interrupt. Form EXTI_PinSourcex
 *  		uint32_t line
 *  			Interrupt line to tie to the pin. Form EXTI_Linex
 *  output:	none
 *  descr:	Configures EXTI for a given pin and line
 */
void Buttons_LinkPinToInterrupt(uint8_t pinSource, uint32_t line);

#endif /* BUTTONS_H_ */
