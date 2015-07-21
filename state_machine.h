#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

//#include "buttons.h"
#include "stm32f4xx_gpio.h"

typedef void(*StateFunc_T)(void);

typedef struct State {
	uint8_t currentState; // member of states enum
	uint8_t nextState;
}State_T;

enum states  {
		DISPLAY_RTC = 0,
		GET_NEW_TIME,
		GET_ALARM_TIME,
		PLAY_MP3,
		PLAY_AUX,
		ALARM_ACTIVE
};

void State_UpdateState();

//void State_AssignNewFunctionsToButtons(Button_T* button, ButtonFunc_T shortPressFunc, ButtonFunc_T longPressFunc);

void State_DisplayRTC();

void State_ToggleHourFormat();

void State_GetNewTime();

void State_GetAlarmTime();

void State_PlayMP3();

void State_PlayAux();

void State_AlarmActive();

#endif /* STATE_MACHINE_H_ */
