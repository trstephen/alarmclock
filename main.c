//This program uses the RTC to display time on a 4 digit 7 segment display
//When the alarm triggers, it plays mp3 files through a USB connected on the
//micro USB port

#include "audioMP3.h"
#include "buttons.h"
#include "clock_display.h"
#include "main.h"
#include "misc.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

//structures
RCC_ClocksTypeDef RCC_Clocks;
GPIO_InitTypeDef	initStruct;
TIM_TimeBaseInitTypeDef TIM_InitStruct;
NVIC_InitTypeDef NVIC_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;


//function prototypes
void configuration(void);


//global variables
int interruptOccurred = 0;


extern volatile int exitMp3 = 0;
extern volatile int mp3PlayingFlag = 0;
extern volatile int snoozeMemory = 0;

volatile uint16_t currentClockSegment = DIGIT_H10;


int main(void)
{
  configuration();

  // enable 1s timer
//  TIM_Cmd(TIM7, ENABLE);

  // set audio enable pin to LOW to turn on LM386
  GPIO_ResetBits(GPIOD, GPIO_Pin_6);

  while ( 1 )
  {
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

	//double checks that interrupt has occurred
	if( TIM_GetITStatus( TIM5, TIM_IT_Update ) != RESET )
	{
		ClockDisplay_TimeTest();

		Buttons_PollAllButtons();

		//clears interrupt flag
	     TIM5->SR = (uint16_t)~TIM_IT_Update;
    }
}

//// 1s timer handler
//void TIM7_IRQHandler(void)
//{
//	if( TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET )
//	{
//
////		Buttons_AssignLongPress();
//
//		if(GBtn_Music.isPressed == true)
//		{
//			GBtn_Music.isLongPress = true;
//		}
//		else if(GBtn_Hour.isPressed == true)
//		{
//			GBtn_Hour.isLongPress = true;
//		}
//		else if(GBtn_Minute.isPressed == true)
//		{
//			GBtn_Minute.isLongPress = true;
//		}
//		else if(GBtn_Time.isPressed == true)
//		{
//			GBtn_Time.isLongPress = true;
//		}
//		else if(GBtn_Alarm.isPressed == true)
//		{
//			GBtn_Alarm.isLongPress = true;
//		}
//
//		// Disable the timer and reset the count
//		TIM_Cmd(TIM7, DISABLE);
//		TIM_SetCounter(TIM7, 0);
//
//		/* Do the long press function */
//		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
//
//		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
//	}
//}

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

// PinC6-9 Handler
/*void EXTI9_5_IRQHandler(void)
{
	if( EXTI_GetITStatus(GBtn_Music.interruptLine) != RESET)
	{
		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);

		EXTI_ClearITPendingBit(GBtn_Music.interruptLine);
	}
	else if( EXTI_GetITStatus(GBtn_Hour.interruptLine) != RESET)
	{
		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);

		EXTI_ClearITPendingBit(GBtn_Hour.interruptLine);
	}
	else if( EXTI_GetITStatus(GBtn_Minute.interruptLine) != RESET)
	{
		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);

		EXTI_ClearITPendingBit(GBtn_Minute.interruptLine);
	}
//	else if( EXTI_GetITStatus(GBtn_Time.interruptLine) != RESET)
//	{
//		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
//
//		EXTI_ClearITPendingBit(GBtn_Time.interruptLine);
//	}
}*/

// PinC10 Handler
/*
 void EXTI15_10_IRQHandler(void)
{
	if( EXTI_GetITStatus(GBtn_Alarm.interruptLine) != RESET)
	{
		// If an edge change is detected, it could be...

		// a leading edge
		if (GBtn_Alarm.isPressed == false)
		{
			GBtn_Alarm.isPressed = true;

			 Start timer
			TIM_Cmd(TIM7, ENABLE);
		}
		// the falling edge of a short press
		else if (  GBtn_Alarm.isPressed == true
				&& TIM_GetCounter(TIM7) > 150
				&& GBtn_Alarm.isLongPress == false )
		{
			GBtn_Alarm.isPressed = false;

			 Stop timer
			TIM_Cmd(TIM7, DISABLE);
			TIM_SetCounter(TIM7, 0);
//			TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

			 signal a short press
			GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
		}
		// the falling edge of a long press
		else if (  GBtn_Alarm.isPressed == true
				&& GBtn_Alarm.isLongPress == true )
		{
			GBtn_Alarm.isPressed = false;
			GBtn_Alarm.isLongPress = false;

			 take no action, since the long press timer handled the state change
			GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
		}

		EXTI_ClearITPendingBit(GBtn_Alarm.interruptLine);
	}
}
*/

//configures the clocks, gpio, alarm, interrupts etc.
void configuration(void)
{
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

	  //set the time displayed on power up to 12PM
	  myclockTimeStruct.RTC_H12 = RTC_H12_PM;
	  myclockTimeStruct.RTC_Hours = 0x012;
	  myclockTimeStruct.RTC_Minutes = 0x00;
	  myclockTimeStruct.RTC_Seconds = 0x00;
	  RTC_SetTime(RTC_Format_BCD, &myclockTimeStruct);


	  //sets alarmA for 12:00AM, date doesn't matter
	  AlarmStruct.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
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
