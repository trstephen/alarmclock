/*******************************************************************************
 file: 		buttons.c
 author:	T. Stephen
 date: 		9 July, 2015
 descr:		Provides button debouncing with a mixture of polling and timer
 			interrupts. It is able to discriminate between a short button press
			and a long (greater than one second) press. Each button has a
			shortPress_func and a longPress_func which are set by the state
			machine. All button actions are defined in buttons.c and are prefixed
			ButtonFunc_
 ******************************************************************************/
#include "audioMP3.h"
#include "buttons.h"
#include "clock_display.h"
#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_syscfg.h"

/****************
*	Global Variables
*****************/
extern volatile RTC_TimeTypeDef GNewTime;
extern volatile RTC_AlarmTypeDef GAlarm;
extern volatile ClockDisplay_T GClockDisplay;
extern volatile State_T GState;
extern volatile Button_T GBtn_Music;
extern volatile Button_T GBtn_Hour;
extern volatile Button_T GBtn_Minute;
extern volatile Button_T GBtn_Time;
extern volatile Button_T GBtn_Alarm;
extern volatile int mp3PlayingFlag;
extern volatile int exitMp3;

/****************
*	Constants
*****************/
// make it easier to iterate over all buttons
#define NUM_BUTTONS (5)
static const Button_T* buttons[NUM_BUTTONS] = {
		&GBtn_Music,
		&GBtn_Hour,
		&GBtn_Minute,
		&GBtn_Time,
		&GBtn_Alarm
};
static const GPIO_TypeDef* button_bank = GPIOC;

/****************
*	Functions
*****************/
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

	// enable the interrupts
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
		/* enable debounce and long press timers and set button state flags */
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
		/* reset debounce and long press timers */
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
		/* enable debounce timer */
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
		/* reset debounce timer */
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
		int i;
		for (i = 0; i < NUM_BUTTONS; i++)
		{
			Buttons_AssignStateFromStableInput(buttons[i]);
		}

		Buttons_SetTimerState(TIM3, DISABLE);
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
		int i;
		for (i = 0; i < NUM_BUTTONS; i++)
		{
			Buttons_AssignStateFromLongPress(buttons[i]);
		}

		// Disable the timer and reset the count
		Buttons_SetTimerState(TIM7, DISABLE);
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
	TIM_Cmd(TIMx, newState);
	TIM_SetCounter(TIMx, 0);
}


FunctionalState Buttons_GetTimerState(TIM_TypeDef *TIMx)
{
	FunctionalState returnState = DISABLE; // until challenged

	if ((TIMx->CR1 & TIM_CR1_CEN) == TIM_CR1_CEN)
	{
		returnState = ENABLE;
	}

	return returnState;
}

void ButtonFunc_Disabled()
{
	/* do nothing */
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
	// start setting the new time from the current time value
	GNewTime = ClockDisplay_UpdateFromRTC();

	GState.nextState = GET_NEW_TIME;
}

void ButtonFunc_SetNewTime()
{
	RTC_SetTime(RTC_Format_BCD, &GNewTime);

	GState.nextState = DISPLAY_RTC;
}

void ButtonFunc_IncrementMinutes()
{
	GNewTime.RTC_Minutes += 0x01;

	// adapted from timekeeping.c

	//allows 9 to roll over to 0 and 60 to 0
	 if((GNewTime.RTC_Minutes & 0x0F) >= 0xA)
	 {
		 GNewTime.RTC_Minutes = (GNewTime.RTC_Minutes & 0x70 ) + 0x10;

		 if((GNewTime.RTC_Minutes & 0x70 ) >= 0x60 )
		 {
			 GNewTime.RTC_Minutes = 0x00;
		 }
	 }
}

void ButtonFunc_IncrementHours()
{
	GNewTime.RTC_Hours += 0x01;

	// adapted from timekeeping.c

	 //9 rolls over to 0
	 if((GNewTime.RTC_Hours & 0x0F) >= 0xA)
	 {
		 GNewTime.RTC_Hours = (GNewTime.RTC_Hours & 0x30 ) + 0x10;

	 }

	 //if incrementing passes 12 -> toggles am/pm
	 if(GNewTime.RTC_Hours == 0x12 )
	 {
		 GNewTime.RTC_H12 ^= 0x40;
	 }

	 //if incrementing hits hour 13, sets to 1 oclock
	 if(GNewTime.RTC_Hours >= 0x13 )
	 {
		 GNewTime.RTC_Hours = 0x01;
	 }
}

void ButtonFunc_ToggleAlarm()
{
	if (GState.isAlarmSet == true)
	{
		GState.isAlarmSet = false;
		RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
		RTC_AlarmCmd(RTC_Alarm_B, DISABLE);
	}
	else
	{
		GState.isAlarmSet = true;
		RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
	}
}

void ButtonFunc_GetAlarmTime()
{
	// start setting the new time from the current alarm time
	GNewTime = GAlarm.RTC_AlarmTime;

	GState.nextState = GET_ALARM_TIME;
}

void ButtonFunc_SetAlarmTime()
{
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

	GAlarm.RTC_AlarmTime = GNewTime;
	GAlarm.RTC_AlarmTime.RTC_Seconds = 0x00;
	GAlarm.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &GAlarm);

	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

	GState.isAlarmSet = true;

	GState.nextState = DISPLAY_RTC;
}

void ButtonFunc_Snooze()
{
	Audio_Stop();

	// turn off the main alarm
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	RTC_AlarmCmd(RTC_Alarm_B, DISABLE);

	// set the snooze alarm for now + 10sec
	RTC_AlarmTypeDef snoozeAlarm = GAlarm;
	RTC_GetTime(RTC_HourFormat_12, &snoozeAlarm.RTC_AlarmTime);

	snoozeAlarm.RTC_AlarmTime.RTC_Seconds += 0x010;

	// adapted from timekeeping.c
	//allows 9 to roll over to 0 and 60 to 0
	 if((snoozeAlarm.RTC_AlarmTime.RTC_Seconds & 0x0F) >= 0xA)
	 {
		 snoozeAlarm.RTC_AlarmTime.RTC_Seconds = (snoozeAlarm.RTC_AlarmTime.RTC_Seconds & 0x70 ) + 0x10;

		 if((snoozeAlarm.RTC_AlarmTime.RTC_Seconds & 0x70 ) >= 0x60 )
		 {
			 snoozeAlarm.RTC_AlarmTime.RTC_Seconds = 0x00;
		 }
	 }
	 // TODO: BUG - the rollover code does not increment the entire number so a snooze could "wrap around"
	 // and go off just before the original alarm time.

	 RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_B, &snoozeAlarm);
	 RTC_AlarmCmd(RTC_Alarm_B, ENABLE);

	GState.nextState = DISPLAY_RTC;
}

void ButtonFunc_DisableAlarm()
{
	Audio_Stop();

	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	RTC_AlarmCmd(RTC_Alarm_B, DISABLE);

	GState.isAlarmSet = false;
	GState.nextState = DISPLAY_RTC;
}


void ButtonFunc_ToggleMusic()
{
	if (mp3PlayingFlag == 0)
	{
		Audio_Start();
	}
	else
	{
		Audio_Stop();
	}
}

void ButtonFunc_ToggleAuxInput()
{
	if (mp3PlayingFlag == 1)
	{
		exitMp3 = 1;
		mp3PlayingFlag = 0;
		// LM386 stays ON
	}
	else
	{
		GPIO_ToggleBits(GPIOD, GPIO_Pin_6);
	}
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
