
#include "stm32f4xx_rtc.h"
#include "buttons.h"

#ifndef MAIN_H_
#define MAIN_H_


#define TIMEBUTTON 0xB20		//(GPIOC->IDR & GPIO_Pin_4)
#define TIMEHOURBUTTON 0xA20	//(GPIOC->IDR & GPIO_Pin_8)
#define TIMEMINBUTTON 0x320		//(GPIOC->IDR & GPIO_Pin_11)
#define ALARMBUTTON 0x930		//(GPIOC->IDR & GPIO_Pin_9)
#define MODEBUTTON 0xA30//0xB32		//(GPIOC->IDR & GPIO_Pin_6)
#define BUTTON		0xB10		//(GPIOC->IDR & GPIO_Pin_5)
#define SNOOZEBUTTON 0x330//0xB70		//(GPIOC->IDR & GPIO_Pin_1)
#define ALARMHOURBUTTON 0x830
#define ALARMMINBUTTON 0x130
#define NOBUTTON 0xB30


//RTC structures
RTC_InitTypeDef		myclockInitTypeStruct;
RTC_TimeTypeDef		myclockTimeStruct;
RTC_AlarmTypeDef	AlarmStruct;
RTC_AlarmTypeDef 	alarmMemory;

// Martial those global variables!
extern volatile Button_T GBtn_Music;
extern volatile Button_T GBtn_Hour;
extern volatile Button_T GBtn_Minute;
extern volatile Button_T GBtn_Time;
extern volatile Button_T GBtn_Alarm;

#endif /* MAIN_H_ */
