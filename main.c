//This program uses the RTC to display time on a 4 digit 7 segment display
//When the alarm triggers, it plays mp3 files through a USB connected on the
//micro USB port

#include "audioMP3.h"
#include "buttons.h"
#include "clock_display.h"
#include "main.h"
#include "misc.h"
#include "state_machine.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

/*
 * Leftover junk from the demo code
 */
RTC_TimeTypeDef		myclockTimeStruct;
RTC_AlarmTypeDef	AlarmStruct;
RTC_AlarmTypeDef 	alarmMemory;

//global variables
int interruptOccurred = 0;


extern volatile int exitMp3 = 0;
extern volatile int mp3PlayingFlag = 0;
extern volatile int snoozeMemory = 0;

/*
 * My code starts here
*/
extern volatile State_T GState = {
	.currentState = DISPLAY_RTC,
	.nextState = DISPLAY_RTC,
	.isAlarmSet = false
};

volatile ClockDisplay_T GClockDisplay = {
	.getTime_func = ClockDisplay_UpdateFromRTC,
	.hourFormat = RTC_HourFormat_12,
	.currentSegment = DIGIT_H10,
	.isColonBlinking = true,
	.isDisplayBlinking = true,
	.blinkCounter = 0
};

// Global variables for buttons
volatile Button_T GBtn_Music = {
	.pin = GPIO_Pin_6,
	.isBeingDebounced = false,
	.isPressed = false,
	.isLongPress = false,
	.shortPress_func = ButtonFunc_Disabled,
	.longPress_func = ButtonFunc_Disabled
};

volatile Button_T GBtn_Hour = {
	.pin = GPIO_Pin_7,
	.isBeingDebounced = false,
	.isPressed = false,
	.isLongPress = false,
	.shortPress_func = ButtonFunc_ToggleBlueLED,
	.longPress_func = ButtonFunc_ToggleOrangeLED
};

volatile Button_T GBtn_Minute = {
	.pin = GPIO_Pin_8,
	.isBeingDebounced = false,
	.isPressed = false,
	.isLongPress = false,
	.shortPress_func = ButtonFunc_SwapFunctions,
	.longPress_func = ButtonFunc_Disabled
};

volatile Button_T GBtn_Time = {
	.pin = GPIO_Pin_9,
	.isBeingDebounced = false,
	.isPressed = false,
	.isLongPress = false,
	.shortPress_func = ButtonFunc_ToggleHourFormat,
	.longPress_func = ButtonFunc_GetNewTime
};

volatile Button_T GBtn_Alarm = {
	.pin = GPIO_Pin_10,
	.isBeingDebounced = false,
	.isPressed = false,
	.isLongPress = false,
	.shortPress_func = ButtonFunc_ToggleAlarm,
	.longPress_func = ButtonFunc_GetAlarmTime
};

int main(void)
{
	configuration();

	// set audio enable pin to LOW to turn on LM386
	GPIO_ResetBits(GPIOD, GPIO_Pin_6);



	while ( 1 )
	{

//		State_UpdateState();

		mp3PlayingFlag = 1;
		audioToMp3();
	}
}

//timer interrupt handler that is called at a rate of 500Hz
//this function gets the time and displays it on the 7 segment display
//it also checks for button presses, debounces, and handles each case
void TIM5_IRQHandler(void)
{
	int previousState = 0;

#if DEBUG_BUTTON_TIMERS
	TIM_Cmd(TIM7, ENABLE);
#endif

	//double checks that interrupt has occurred
	if( TIM_GetITStatus( TIM5, TIM_IT_Update ) != RESET )
	{
		ClockDisplay_UpdateTime();

#if !DEBUG_BUTTON_TIMERS
		Buttons_PollAllButtons();
#endif

		State_UpdateState();

		//clears interrupt flag
	     TIM5->SR = (uint16_t)~TIM_IT_Update;
    }
}

//alarm A interrupt handler
//when alarm occurs, clear all the interrupt bits and flags
//then set the flag to play mp3
void RTC_Alarm_IRQHandler(void)
{

	//resets alarm flags and sets flag to play mp3
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF);
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		EXTI_ClearITPendingBit(EXTI_Line17);
		interruptOccurred = 1;
	}
}


void configuration(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	GPIO_InitTypeDef initStruct;
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	RTC_InitTypeDef	myclockInitTypeStruct;
	RTC_TimeTypeDef initTime;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	//lets the system clocks be viewed
	RCC_GetClocksFreq(&RCC_Clocks);

	//enable peripheral clocks
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	//enable the RTC
	PWR_BackupAccessCmd(DISABLE);
	PWR_BackupAccessCmd(ENABLE);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);

	//Enable the LSI OSC
	RCC_LSICmd(ENABLE);

	//Wait till LSI is ready
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

	//enable the external interrupt for the RTC to use the Alarm
	/* EXTI configuration */
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	//set timer 5 to interrupt at a rate of 500Hz
	TIM_TimeBaseStructInit(&TIM_InitStruct);
	TIM_InitStruct.TIM_Period	=  8000;	// 500Hz
	TIM_InitStruct.TIM_Prescaler = 20;
	TIM_TimeBaseInit(TIM5, &TIM_InitStruct);

	// Enable the TIM5 global Interrupt
	NVIC_Init( &NVIC_InitStructure );
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

	//setup the RTC for 12 hour format
	myclockInitTypeStruct.RTC_HourFormat = RTC_HourFormat_12;
	myclockInitTypeStruct.RTC_AsynchPrediv = 127;
	myclockInitTypeStruct.RTC_SynchPrediv = 0x00FF;
	RTC_Init(&myclockInitTypeStruct);

	// initial time: 12:00 PM
	initTime.RTC_H12 = RTC_H12_PM;
	initTime.RTC_Hours = 0x012;
	initTime.RTC_Minutes = 0x00;
	initTime.RTC_Seconds = 0x00;
	RTC_SetTime(RTC_Format_BCD, &initTime);


	//sets alarmA for 12:00AM, date doesn't matter
	AlarmStruct.RTC_AlarmTime.RTC_H12 = RTC_H12_PM;
	AlarmStruct.RTC_AlarmTime.RTC_Hours = 0x12;
	AlarmStruct.RTC_AlarmTime.RTC_Minutes = 0x00;
	AlarmStruct.RTC_AlarmTime.RTC_Seconds = 0x00;
	AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
	RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&AlarmStruct);

	// Enable the Alarm global Interrupt
	NVIC_Init( &NVIC_InitStructure );
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

	Buttons_Init();

	Audio_Init();

	ClockDisplay_Init();

	//enables RTC alarm A interrupt
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	//enables timer interrupt
	TIM5->DIER |= TIM_IT_Update;

	//enables timer
	TIM5->CR1 |= TIM_CR1_CEN;
}
