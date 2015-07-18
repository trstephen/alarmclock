#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "state_machine.h"
#include "stm32f4xx_gpio.h"
#include <stdbool.h>

// Constants
static const GPIO_TypeDef* button_bank = GPIOC;

// Types
typedef struct Button{
	uint16_t pin;					// the GPIO pin assigned to the button
	bool isPressed; 				// true if button is currently pressed
	bool isLongPress;				// true if button has received a long press
	StateFunc_T shortPress_func;	// function pointer to the short press behavior
	StateFunc_T longPress_func; 	// function pointer to the long press behavior
}Button_T;

// Globals
extern volatile Button_T GBtn_Music;
extern volatile Button_T GBtn_Hour;
extern volatile Button_T GBtn_Minute;
extern volatile Button_T GBtn_Time;
extern volatile Button_T GBtn_Alarm;

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

void Buttons_AssignStateFromStableInput(volatile Button_T *button);

void TIM3_IRQHandler();

void Buttons_PollAllButtons();

void Buttons_Debounce(volatile Button_T *button);

void TIM7_IRQHandler(void);

void Buttons_AssignStateFromLongPress(volatile Button_T *button);

#endif /* BUTTONS_H_ */
