/******************************************************************
 * file: 	buttons.c
 * author:	T. Stephen
 * date: 	9 July, 2015
 * descr:	handles signals from external buttons on the alarm
 *
 ******************************************************************/
#include "buttons.h"
#include "clock_display.h"
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
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
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
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
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
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStruct );

//	Buttons_SetTimerState(TIM7, ENABLE);
//	Buttons_SetTimerState(TIM3, ENABLE);

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

}

void Buttons_PollAllButtons()
{
	int i;
	for (i = 0; i < NUM_BUTTONS; ++i)
	{
		Buttons_Debounce(buttons[i]);
	}
}

void Buttons_Debounce(volatile Button_T *button)
{
	uint8_t pinState = GPIO_ReadInputDataBit(button_bank, button->pin);
	FunctionalState debounceTimerState = Buttons_GetTimerState(TIM3);

	// a button may be...
	// steady down
	if (button->isPressed == false
		&& pinState == Bit_RESET
		&& debounceTimerState == DISABLE)
	{
		/* do nothing */
	}
	// rising edge
	else if (button->isPressed == false
		&& pinState == Bit_SET
		&& debounceTimerState == DISABLE)
	{
		/* enable DB and LP timers and set button's debounce flag to true*/
		Buttons_SetTimerState(TIM3, ENABLE);
		Buttons_SetTimerState(TIM7, ENABLE);
		button->isBeingDebounced = true;
	}
	// down, but the timer is debouncing another button
	else if (button->isPressed == false
		&& pinState == Bit_RESET
		&& debounceTimerState == ENABLE
		&& button->isBeingDebounced == false)
	{
		/* Do nothing */
	}
	// ripple on rising edge
	else if (button->isPressed == false
		&& pinState == Bit_RESET
		&& debounceTimerState == ENABLE
		&& button->isBeingDebounced == true)
	{
		/* reset DB and LP timers */
		TIM_SetCounter(TIM3, 0);
		TIM_SetCounter(TIM7, 0);
	}
	// waiting for rise debounce to activate
	else if (button->isPressed == false
		&& pinState == Bit_SET
		&& debounceTimerState == ENABLE)
	{
		/* do nothing */
	}
	// steady up
	else if (button->isPressed == true
		&& pinState == Bit_SET
		&& debounceTimerState == DISABLE)
	{
		/* do nothing */
	}
	// falling edge
	else if (button->isPressed == true
		&& pinState == Bit_RESET
		&& debounceTimerState == DISABLE)
	{
		/* enable DB timer */
		Buttons_SetTimerState(TIM3, ENABLE);
		button->isBeingDebounced = true;
	}
	// up, but the timer is debouncing another button
	else if (button->isPressed == true
		&& pinState == Bit_SET
		&& debounceTimerState == ENABLE
		&& button->isBeingDebounced == false)
	{
		/* Do nothing */
	}
	// ripple on falling edge
	else if (button->isPressed == true
		&& pinState == Bit_SET
		&& debounceTimerState == ENABLE
		&& button->isBeingDebounced == true)
	{
		/* reset DB timer */
		TIM_SetCounter(TIM3, 0);
	}
	// waiting for fall debounce to activate
	else if (button->isPressed == true
		&& pinState == Bit_RESET
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
#if DEBUG_BUTTON_TIMERS
		State_ToggleBlueLED();
#else
		int i;
		for (i = 0; i < NUM_BUTTONS; i++)
		{
			Buttons_AssignStateFromStableInput(buttons[i]);
		}

		Buttons_SetTimerState(TIM3, DISABLE);
#endif
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void Buttons_AssignStateFromStableInput(volatile Button_T *button)
{
	// because we are polling the buttons, we have to keep track of
	// which button caused the debounce to initiate and then only
	// do the behavior associate for that button
	if (button->isBeingDebounced == true)
	{
		uint8_t pinState = GPIO_ReadInputDataBit(button_bank, button->pin);

		// the timer tiggered because...
		// the button is steady after being pressed
		if (button->isPressed == false
				&& pinState == Bit_SET)
		{
			/* flag the button as pressed */
			button->isPressed = true;
		}
		// the button is released after a short press
		else if (button->isPressed == true
				&& pinState == Bit_RESET
				&& button->isLongPress == false)
		{
			/* reset the press flag and do the short press function */
			button->isPressed = false;
			button->shortPress_func();
		}
		// the button is released after a long press
		else if (button->isPressed == true
				&& pinState == Bit_RESET
				&& button->isLongPress == true)
		{
			/* reset flags but take no action */
			button->isPressed = false;
			button->isLongPress = false;
		}

		button->isBeingDebounced = false;
	}
}

// long press timer handler
void TIM7_IRQHandler(void)
{
	if( TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET )
	{
#if DEBUG_BUTTON_TIMERS
		fastTimerToggle = fastTimerToggle == DISABLE ? ENABLE : DISABLE;
		Buttons_SetTimerState(TIM3, fastTimerToggle);
		State_ToggleOrangeLED();
#else
		int i;
		for (i = 0; i < NUM_BUTTONS; i++)
		{
			Buttons_AssignStateFromLongPress(buttons[i]);
		}

		// Disable the timer and reset the count
		Buttons_SetTimerState(TIM7, DISABLE);
#endif
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

	}
}

void Buttons_AssignStateFromLongPress(volatile Button_T *button)
{
	if (button->isPressed == true)
	{
		/* set flag and do the long press function */
		button->isLongPress = true;
		button->longPress_func();
	}
}

void Buttons_SetTimerState(TIM_TypeDef *TIMx, FunctionalState newState)
{
//	TIM_ITConfig(TIMx, TIM_IT_Update, newState);
	TIM_Cmd(TIMx, newState);
	TIM_SetCounter(TIMx, 0);
}


FunctionalState Buttons_GetTimerState(TIM_TypeDef *TIMx)
{
	FunctionalState returnState = DISABLE;

	if ((TIMx->CR1 & TIM_CR1_CEN) == TIM_CR1_CEN)
	{
		returnState = ENABLE;
	}

	return returnState;
}

void ButtonFunc_DisplayRTC()
{

}

void ButtonFunc_Disabled()
{
	/* do nothing */
}

void ButtonFunc_ToggleRedLED()
{
	GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
}

void ButtonFunc_ToggleBlueLED()
{
	GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
}

void ButtonFunc_ToggleOrangeLED()
{
	GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
}

void ButtonFunc_ToggleGreenLED()
{
	GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
}

void ButtonFunc_SwapFunctions()
{
	if (GBtn_Alarm.shortPress_func == ButtonFunc_ToggleBlueLED)
	{
		GBtn_Alarm.shortPress_func = ButtonFunc_ToggleRedLED;
	}
	else
	{
		GBtn_Alarm.shortPress_func = ButtonFunc_ToggleBlueLED;
	}

	if (GBtn_Alarm.longPress_func == ButtonFunc_ToggleOrangeLED)
	{
		GBtn_Alarm.longPress_func = ButtonFunc_ToggleGreenLED;
	}
	else
	{
		GBtn_Alarm.longPress_func = ButtonFunc_ToggleOrangeLED;
	}
}

void ButtonFunc_ToggleHourFormat()
{
	if (GClockDisplay.hourFormat == RTC_HourFormat_12)
	{
		GClockDisplay.hourFormat = RTC_HourFormat_24;
	}
	else
	{
		GClockDisplay.hourFormat = RTC_HourFormat_12;
	}
}

void ButtonFunc_GetNewTime()
{
	GState.nextState = GET_NEW_TIME;
}

void ButtonFunc_SetNewTime()
{
	RTC_SetTime(RTC_Format_BCD, &GNewTime);

	GState.nextState = DISPLAY_RTC;
}

void ButtonFunc_IncrementMinutes()
{
	RTC_TimeTypeDef *time;

	switch (GState.currentState) {
		case GET_NEW_TIME:
			time = &GNewTime;
			break;
		case GET_ALARM_TIME:
			/* set it to the alarm time struct */
			break;
		default:
			/* just bail out, man */
			return;
	}

	time->RTC_Minutes += 0x01;

	// adapted from timekeeping.c

	//allows 9 to roll over to 0 and 60 to 0
	 if((time->RTC_Minutes & 0x0F) >= 0xA)
	 {
		 time->RTC_Minutes = (time->RTC_Minutes & 0x70 ) + 0x10;

		 if((time->RTC_Minutes & 0x70 ) >= 0x60 )
		 {
			 time->RTC_Minutes = 0x00;
		 }
	 }
}

void ButtonFunc_IncrementHours()
{
	RTC_TimeTypeDef *time;

	switch (GState.currentState) {
		case GET_NEW_TIME:
			time = &GNewTime;
			break;
		case GET_ALARM_TIME:
			/* set it to the alarm time struct */
			break;
		default:
			/* just bail out, man */
			return;
	}

	time->RTC_Hours += 0x01;

	// adapted from timekeeping.c

	 //9 rolls over to 0
	 if((time->RTC_Hours & 0x0F) >= 0xA)
	 {
		 time->RTC_Hours = (time->RTC_Hours & 0x30 ) + 0x10;

	 }

	 //if incrementing passes 12 -> toggles am/pm
	 if(time->RTC_Hours == 0x12 )
	 {
		 time->RTC_H12 ^= 0x40;
	 }

	 //if incrementing hits hour 13, sets to 1 oclock
	 if(time->RTC_Hours >= 0x13 )
	 {
		 time->RTC_Hours = 0x01;
	 }
}

