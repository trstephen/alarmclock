/*******************************************************************************
 file: 		buttons.h
 author:	T. Stephen
 date: 		9 July, 2015
 descr:		Provides button debouncing with a mixture of polling and timer
 					interrupts. It is able to discriminate between a short button press
					and a long (greater than one second) press. Each button has a
					shortPress_func and a longPress_func which are set by the state
					machine. All button actions are defined in buttons.c and are prefixed
					ButtonFunc_
 ******************************************************************************/

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "state_machine.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include <stdbool.h>

/****************
*	Typedefs
*****************/
typedef void(*ButtonFunc_T)(void);

typedef struct Button{
	uint16_t pin;						// the GPIO pin assigned to the button
	bool isBeingDebounced;	// true if button has triggered debounce conditions
	bool isPressed; 				// true if button is currently pressed
	bool isLongPress;				// true if button has received a long press
	ButtonFunc_T shortPress_func;	// function pointer to the short press behavior
	ButtonFunc_T longPress_func; 	// function pointer to the long press behavior
}Button_T;

/****************
*	Prototypes
*****************/
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
 * Buttons_PollAllButtons
 *
 * input:	none
 * output:	none
 * descr:	Debounces all the buttons in polling style. Provides a single
 * 			point of access for main() into the debouncing functions.
 */
void Buttons_PollAllButtons();

/*
	Buttons_Debounce

	input:	Button_T *button
						the button to be debounced
	output:	none
	descr:	Checks the current button value and timer status and compares it with
					the button's flags to determine which state it is in. Resetting the
					timers on a ripple debounces the input. Button state flags are updated
					when a stable input is determined.
 */
void Buttons_Debounce(volatile Button_T *button);

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

void ButtonFunc_DisplayRTC();

/*
	ButtonFunc_Disabled

	input:	none
	output:	none
	descr:	has no effect. Somewhat more explicit than setting NULL as the
					button function when no action is desired. Also, NULL was
					throwing errors at me so this is easier~
*/
void ButtonFunc_Disabled();

/*
	ButtonFunc_ToggleHourFormat

	input:	none
	output:	none
	descr:	Toggles the flag that tells the clock display to represent the time
					in 24h format.
*/
void ButtonFunc_ToggleHourFormat();

/*
	ButtonFunc_GetNewTime

	input:	none
	output:	none
	descr:	Changes the machine state to GET_NEW_TIME
*/
void ButtonFunc_GetNewTime();


/*
	ButtonFunc_SetNewTime

	input:	none
	output:	none
	descr:	Writes the new time to the RTC and changes the machine state
					to DISPLAY_RTC
*/
void ButtonFunc_SetNewTime();

/*
	ButtonFunc_IncrementMinutes

	input:	none
	output:	none
	descr:	Increases the time by 1 minute and handles rollover.
*/
void ButtonFunc_IncrementMinutes();

/*
	ButtonFunc_IncrementHours

	input:	none
	output:	none
	descr:	Increases the time by 1 hour and handles rollover.
*/
void ButtonFunc_IncrementHours();

/*
	ButtonFunc_ToggleAlarm

	input:	none
	output:	none
	descr:	Toggles the state flag and enables/disables the wakeup alarm (Alarm A)
*/
void ButtonFunc_ToggleAlarm();

/*
	ButtonFunc_GetAlarmTime

	input:	none
	output:	none
	descr:	Changes the machine state to GET_ALARM_TIME
*/
void ButtonFunc_GetAlarmTime();

/*
	ButtonFunc_SetAlarmTime

	input:	none
	output:	none
	descr:	Updates the alarm time and enables the wakeup alarm (A).
					Machine state is changed to DISPLAY_RTC
*/
void ButtonFunc_SetAlarmTime();

/*
	ButtonFunc_Snooze

	input:	none
	output:	none
	descr:	Changes the machine state to DISPLAY_RTC but sets the snooze alarm (B)
					to now + 10 min.
*/
void ButtonFunc_Snooze();

/*
	ButtonFunc_DisableAlarm

	input:	none
	output:	none
	descr:	Disables wakeup alarm and snooze alarm.
*/
void ButtonFunc_DisableAlarm();

/*
	ButtonFunc_ToggleMusic

	input:	none
	output:	none
	descr:	Starts and stops music playback from the mp3 decoder.
*/
void ButtonFunc_ToggleMusic();

// some debug functions
void ButtonFunc_ToggleRedLED();
void ButtonFunc_ToggleBlueLED();
void ButtonFunc_ToggleOrangeLED();
void ButtonFunc_ToggleGreenLED();
void ButtonFunc_SwapFunctions();

#endif /* BUTTONS_H_ */
