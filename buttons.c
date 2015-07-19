/******************************************************************
 * file: 	buttons.c
 * author:	T. Stephen
 * date: 	9 July, 2015
 * descr:	handles signals from external buttons on the alarm
 *
 ******************************************************************/
#include "buttons.h"
#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"

void Buttons_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// Set input pins
	GPIO_StructInit( &GPIO_InitStruct );
	GPIO_InitStruct.GPIO_Pin = GBtn_Music.pin | GBtn_Hour.pin | GBtn_Minute.pin | GBtn_Time.pin | GBtn_Alarm.pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(button_bank, &GPIO_InitStruct);

	// Long press timer, 1s
	// numbers were tuned with a scope. General (close) form is
	// 		period = clock speed / (prescaler * period)
	TIM_TimeBaseStructInit(&TIM_InitStruct);
	TIM_InitStruct.TIM_Prescaler = 260000; // 84 MHz -> 1.3 kHz
	TIM_InitStruct.TIM_Period	=  1350;  // 1.3 kHz -> 1 s
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM7, &TIM_InitStruct);

	// Set up timer interrupt handling
	NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init( &NVIC_InitStruct );

	// Debouncing timer, 10ms
	TIM_TimeBaseStructInit(&TIM_InitStruct);
	TIM_InitStruct.TIM_Prescaler = 26000; // 84 MHz -> 13 kHz
	TIM_InitStruct.TIM_Period	=  135;  // 13 kHz -> 10ms
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init( &NVIC_InitStruct );

}

void Buttons_PollAllButtons()
{
	Buttons_Debounce(&GBtn_Music);
	Buttons_Debounce(&GBtn_Hour);
	Buttons_Debounce(&GBtn_Minute);
	Buttons_Debounce(&GBtn_Time);
	Buttons_Debounce(&GBtn_Alarm);
}

void Buttons_Debounce(volatile Button_T *button)
{
	uint8_t pinState = GPIO_ReadInputDataBit(button_bank, button->pin);
	uint8_t debounceTimerState = TIM3->CR1; // control register: ENABLE or DISABLE

	// a button may be...
	// steady down
	if (button->isPressed == false
		&& pinState == RESET
		&& debounceTimerState == DISABLE)
	{
		/* do nothing */
	}
	// rising edge
	else if (button->isPressed == false
		&& pinState == SET
		&& debounceTimerState == DISABLE)
	{
		/* enable DB and LP timers */
		Buttons_SetTimerActivity(TIM3, ENABLE);
		Buttons_SetTimerActivity(TIM7, ENABLE);
	}
	// ripple on rising edge
	else if (button->isPressed == false
		&& pinState == RESET
		&& debounceTimerState == ENABLE)
	{
		/* reset DB and LP timers */
		TIM_SetCounter(TIM3, 0);
		TIM_SetCounter(TIM7, 0);
	}
	// waiting for rise debounce to activate
	else if (button->isPressed == false
		&& pinState == SET
		&& debounceTimerState == ENABLE)
	{
		/* do nothing */
	}
	// steady up
	else if (button->isPressed == true
		&& pinState == SET
		&& debounceTimerState == DISABLE)
	{
		/* do nothing */
	}
	// falling edge
	else if (button->isPressed == true
		&& pinState == RESET
		&& debounceTimerState == DISABLE)
	{
		/* enable DB timer */
		Buttons_SetTimerActivity(TIM3, ENABLE);
	}
	// ripple on falling edge
	else if (button->isPressed == true
		&& pinState == SET
		&& debounceTimerState == ENABLE)
	{
		/* reset DB timer */
		TIM_SetCounter(TIM3, 0);
	}
	// waiting for fall debounce to activate
	else if (button->isPressed == true
		&& pinState == RESET
		&& debounceTimerState == ENABLE)
	{
		/* do nothing */
	}
}

// debounce timer handler
void TIM3_IRQHandler(void)
{
	if( TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET )
	{
//		// Poll all the buttons to determine who caused the interrupt
		Buttons_AssignStateFromStableInput(&GBtn_Music);
		Buttons_AssignStateFromStableInput(&GBtn_Hour);
		Buttons_AssignStateFromStableInput(&GBtn_Minute);
		Buttons_AssignStateFromStableInput(&GBtn_Time);
		Buttons_AssignStateFromStableInput(&GBtn_Alarm);

		Buttons_SetTimerActivity(TIM3, DISABLE);

		State_ToggleBlueLED();
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void Buttons_AssignStateFromStableInput(volatile Button_T *button)
{
	uint8_t pinState = GPIO_ReadInputDataBit(button_bank, button->pin);

	// the timer tiggered because...
	// the button is steady after being pressed
	if (button->isPressed == false
			&& pinState == SET)
	{
		/* flag the button as pressed */
		button->isPressed = true;
	}
	// the button is released after a short press
	if (button->isPressed == true
			&& pinState == RESET
			&& button->isLongPress == false)
	{
		/* reset the press flag and do the short press function */
		button->isPressed = false;
		button->shortPress_func;
	}
	// the button is released after a long press
	if (button->isPressed == true
			&& pinState == RESET
			&& button->isLongPress == true)
	{
		/* reset flags but take no action */
		button->isPressed = false;
		button->isLongPress = false;
	}
}

// long press timer handler
void TIM7_IRQHandler(void)
{
	if( TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET )
	{
		Buttons_AssignStateFromLongPress(&GBtn_Music);
		Buttons_AssignStateFromLongPress(&GBtn_Hour);
		Buttons_AssignStateFromLongPress(&GBtn_Minute);
		Buttons_AssignStateFromLongPress(&GBtn_Time);
		Buttons_AssignStateFromLongPress(&GBtn_Alarm);

		// Disable the timer and reset the count
		Buttons_SetTimerActivity(TIM7, DISABLE);

		State_ToggleRedLED();
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

	}
}

void Buttons_AssignStateFromLongPress(volatile Button_T *button)
{
	if (button->isPressed == true)
	{
		/* set flag and do the long press function */
		button->isLongPress = true;
		button->longPress_func;
	}
}

void Buttons_SetTimerActivity(TIM_TypeDef *TIMx, FunctionalState newState)
{
	TIM_ITConfig(TIMx, TIM_IT_Update, newState);
	TIM_Cmd(TIMx, newState);
	TIM_SetCounter(TIMx, 0);
}
