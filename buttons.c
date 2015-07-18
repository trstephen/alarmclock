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
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"

void Buttons_Init()
{
	// www.stm32f4-discovery.com/2014/08/stm32f4-external-interrupts-tutorial

	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_InitStruct;

	// GPIOC clock enabled in main(), left in for parity with example code
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
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

/*
	Buttons_LinkPinToInterrupt(EXTI_PinSource6, GBtn_Music.interruptLine);
	Buttons_LinkPinToInterrupt(EXTI_PinSource7, GBtn_Hour.interruptLine);
	Buttons_LinkPinToInterrupt(EXTI_PinSource8, GBtn_Minute.interruptLine);
	Buttons_LinkPinToInterrupt(EXTI_PinSource9, GBtn_Time.interruptLine);
	Buttons_LinkPinToInterrupt(EXTI_PinSource10, GBtn_Alarm.interruptLine);

	// Add IRQ vector to NVIC (C5-9)
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStruct );

	// Add IRQ vector to NVIC (C10)
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStruct );
*/

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
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStruct );

	// enable timer interrupt
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

	// Debouncing timer, 10ms
	TIM_TimeBaseStructInit(&TIM_InitStruct);
	TIM_InitStruct.TIM_Prescaler = 26000; // 84 MHz -> 13 kHz
	TIM_InitStruct.TIM_Period	=  135;  // 13 kHz -> 10ms
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStruct );

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

}

void Buttons_LinkPinToInterrupt(uint8_t pinSource, uint32_t line)
{
	EXTI_InitTypeDef EXTI_InitStruct;

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, pinSource);

	EXTI_StructInit( &EXTI_InitStruct );
	EXTI_InitStruct.EXTI_Line = line;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init( &EXTI_InitStruct );
}

void Buttons_PollAllButtons()
{
	Buttons_Debounce(&GBtn_Music);
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
	if (button->isPressed == false
		&& pinState == SET
		&& debounceTimerState == DISABLE)
	{
		/* enable DB and LP timers */
		TIM_Cmd(TIM3, ENABLE);
		TIM_Cmd(TIM7, ENABLE);
	}
	// ripple on rising edge
	if (button->isPressed == false
		&& pinState == RESET
		&& debounceTimerState == ENABLE)
	{
		/* reset DB and LP timers */
		TIM_SetCounter(TIM3, 0);
		TIM_SetCounter(TIM7, 0);
	}
	// waiting for rise debounce to activate
	if (button->isPressed == false
		&& pinState == SET
		&& debounceTimerState == ENABLE)
	{
		/* do nothing */
	}
	// steady up
	if (button->isPressed == true
		&& pinState == SET
		&& debounceTimerState == DISABLE)
	{
		/* do nothing */
	}
	// falling edge
	if (button->isPressed == true
		&& pinState == RESET
		&& debounceTimerState == DISABLE)
	{
		/* enable DB timer */
		TIM_Cmd(TIM3, ENABLE);
	}
	// ripple on falling edge
	if (button->isPressed == true
		&& pinState == SET
		&& debounceTimerState == ENABLE)
	{
		/* reset DB timer */
		TIM_SetCounter(TIM3, 0);
	}
	// waiting for fall debounce to activate
	if (button->isPressed == true
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
		// Poll all the buttons to determine who caused the interrupt
		Buttons_AssignStateFromStableInput(&GBtn_Music);
		Buttons_AssignStateFromStableInput(&GBtn_Hour);
		Buttons_AssignStateFromStableInput(&GBtn_Minute);
		Buttons_AssignStateFromStableInput(&GBtn_Time);
		Buttons_AssignStateFromStableInput(&GBtn_Alarm);

		// Disable the timer and reset the count
		TIM_Cmd(TIM3, DISABLE);
		TIM_SetCounter(TIM3, 0);
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
	}
	// the button is released after a short press
	if (button->isPressed == true
			&& pinState == RESET
			&& button->isLongPress == false)
	{
		/* reset the press flag and do the short press function */
	}
	// the button is released after a long press
	if (button->isPressed == true
			&& pinState == RESET
			&& button->isLongPress == true)
	{
		/* reset flags but take no action */
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
		TIM_Cmd(TIM7, DISABLE);
		TIM_SetCounter(TIM7, 0);
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}
}

void Buttons_AssignStateFromLongPress(volatile Button_T *button)
{
	if (button->isPressed == true)
	{
		button->isLongPress = true;
		/* do the long press function */
		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
	}
}
