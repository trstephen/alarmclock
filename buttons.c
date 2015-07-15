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

void Buttons_Init()
{
	// www.stm32f4-discovery.com/2014/08/stm32f4-external-interrupts-tutorial

	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	// GPIOC clock enabled in main(), left in for parity with example code
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	// Enable clock for SYSCFG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	// Set input pins
	GPIO_StructInit( &GPIO_InitStruct );
	GPIO_InitStruct.GPIO_Pin = GBtn_Music.pin | GBtn_Hour.pin | GBtn_Minute.pin | GBtn_Time.pin | GBtn_Alarm.pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(button_bank, &GPIO_InitStruct);

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
