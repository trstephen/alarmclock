
#include "stm32f4xx_rtc.h"
#include "buttons.h"

#ifndef MAIN_H_
#define MAIN_H_

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
