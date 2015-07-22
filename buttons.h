#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "state_machine.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include <stdbool.h>

// Types
typedef void(*ButtonFunc_T)(void);

typedef struct Button{
	uint16_t pin;					// the GPIO pin assigned to the button
	bool isBeingDebounced;			// true if button has triggered debounce conditions
	bool isPressed; 				// true if button is currently pressed
	bool isLongPress;				// true if button has received a long press
	ButtonFunc_T shortPress_func;	// function pointer to the short press behavior
	ButtonFunc_T longPress_func; 	// function pointer to the long press behavior
}Button_T;

// Globals
extern volatile State_T GState;
extern volatile Button_T GBtn_Music;
extern volatile Button_T GBtn_Hour;
extern volatile Button_T GBtn_Minute;
extern volatile Button_T GBtn_Time;
extern volatile Button_T GBtn_Alarm;
#define NUM_BUTTONS (5)

// Constants
static const GPIO_TypeDef* button_bank = GPIOC;
static const Button_T* buttons[NUM_BUTTONS] = {
		&GBtn_Music,
		&GBtn_Hour,
		&GBtn_Minute,
		&GBtn_Time,
		&GBtn_Alarm
};


// some debug junk
#define DEBUG_BUTTON_TIMERS 0
static FunctionalState fastTimerToggle = DISABLE;

/*
 * Buttons_Init
 *
 *  input: 	none
 * 	output: none
 * 	descr: 	Initializes GPIOC pins for button handling in pull down mode.
 * 			TIM3 is initialized for 100ms (button debouncing)
 * 			TIM7 is initialized for 1s (long press detection)
 */
void Buttons_Init();

/*
 * Buttons_AssignStateFromStableInput
 *
 *  input:	Button_T *button
 *  			The button that provide the state behavior
 *  output:	none
 *  descr:	decides which debounce just occurred: an initial rising edge; the falling
 *  		edge from a short press, or; the falling edge from a long press.
 *  		If a short press occurred, its associated function will be invoked.
 */
void Buttons_AssignStateFromStableInput(volatile Button_T *button);

/*
 * TIM3_IRQHandler
 *
 *  input:	none
 *  output:	none
 *  descr:	Handler for the 100ms debounce timer. It polls all buttons to
 *  		determine if they triggered the interrupt before clearing
 *  		the interrupt and shutting off.
 */
void TIM3_IRQHandler();

/*
 * Buttons_PollAllButtons
 *
 * input:	none
 * output:	none
 * descr:	Debounces all the buttons in polling style. Provides a single
 * 			point of access for main() into the debouncing functions.
 */
void Buttons_PollAllButtons();

/*
 * Buttons_Debounce
 *
 *  input:	Button_T *button
 * 				The button to be debounced
 * 	output:	none
 * 	descr:	Uses the button's press flag, pin status, timer status and debounce status
 * 			to determine if debounce timers need to be started, stopped or reset.
 * 			Very complicated. Much convolution.
 */
void Buttons_Debounce(volatile Button_T *button);

/*
 * TIM7_IRQHandler
 *
 *  input:	none
 *  output:	none
 *  descr:	Handler for the 1s long press event. Handler activates on timer expiration
 *  		and invokes the long press function for whichever button is in the long
 *  		press state. Timer shuts off after the handler is complete.
 */
void TIM7_IRQHandler(void);

/*
 * Buttons_AssignStateFromLongPress
 *
 *  intput:	Button_T *button
 *  			The button to be checked for the long press state
 *  output:	none
 *  descr:	Used by the long press handler (TIM7) to determine which button
 *  		is in the long press state. The long press function
 *  		is invoked at this point.
 */
void Buttons_AssignStateFromLongPress(volatile Button_T *button);

/*
 * Buttons_SetTimerState
 *
 *  input:	TIM_TypeDef *TIMx
 *  			The timer being acted upon
 *  		FunctionalState newState
 *  			The new state for the timer
 *  			ENABLE or DISABLE
 *  output:	none
 *  descr:	Puts the timer into the new state and unconditionally
 *  		resets the timer counter.
 */
void Buttons_SetTimerState(TIM_TypeDef *TIMx, FunctionalState newState);

/*
 * Buttons_GetTimerState
 *
 *  input:	TIM_TypeDef *TIMx
 *  			The timer whose state we would really like to know
 *  output:	FunctionalState
 *  			The current activation status of the timer
 *  			Returns ENABLE or DISABLE
 *  descr:	Retrieves the timer activation state by reading the appropriate
 *  		memory address and masking the value.
 */
FunctionalState Buttons_GetTimerState(TIM_TypeDef *TIMx);

void ButtonFunc_Disabled();
void ButtonFunc_ToggleRedLED();
void ButtonFunc_ToggleBlueLED();
void ButtonFunc_ToggleOrangeLED();
void ButtonFunc_ToggleGreenLED();
void ButtonFunc_SwapFunctions();

void ButtonFunc_DisplayRTC();

void ButtonFunc_ToggleHourFormat();

void ButtonFunc_GetNewTime();

void ButtonFunc_SetNewTime();

void ButtonFunc_IncrementMinutes();

void ButtonFunc_IncrementHours();

void ButtonFunc_ToggleAlarm();

void ButtonFunc_GetAlarmTime();

void ButtonFunc_SetAlarmTime();

#endif /* BUTTONS_H_ */
