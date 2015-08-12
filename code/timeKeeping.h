

#ifndef TIMEKEEPING_H_
#define TIMEKEEPING_H_

#include "stm32f4xx_rtc.h"
#include "audioMP3.h"
#include "main.h"

extern RTC_TimeTypeDef		myclockTimeStruct;
extern RTC_AlarmTypeDef	AlarmStruct;

void timeHourCheck(void);
void timeMinuteCheck(void);
void alarmHourCheck(void);
void alarmMinuteCheck(void);

#endif /* TIMEKEEPING_H_ */
