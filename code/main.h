
#include "stm32f4xx_rtc.h"
#include "buttons.h"

#ifndef MAIN_H_
#define MAIN_H_

// global variables
volatile RTC_AlarmTypeDef GAlarm;

int main(void);
void configuration(void);


#endif /* MAIN_H_ */
