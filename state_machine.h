/*******************************************************************************
 	file: 	state_machine.h
 	author:	T. Stephen
 	date: 	17 July, 2015
	descr:	Provides variables to maintain the machine state. When the state
			changes, the button functions and clock behavior are changed.
			Other activities which do not require buttons to be remapped are
			encapsulated in a ButtonFunc_.
 ******************************************************************************/
#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <stdbool.h>

/****************
*	Enumerators
*****************/
enum states  {
		DISPLAY_RTC = 0,
		GET_NEW_TIME,
		GET_ALARM_TIME,
		ALARM_ACTIVE
};

/****************
*	Typedefs
*****************/
typedef void(*StateFunc_T)(void);

typedef struct State {
	enum states currentState;	// present state of the machine
	enum states nextState;		// set by button functions or other triggers
	bool isAlarmSet;					// condensation of a machine state to a flag which prevents duplication of states
}State_T;

/****************
*	Prototypes
*****************/
/*
	State_UpdateState

	input:	none
	output:	may change button and display behavior
	descr:	Changes the button functions and clock display target for the current
					machine state.
*/
void State_UpdateState();

/*
	State_DisplayRTC

	input:	none
	output:	changes button and display behavior
	descr:	Updates button functions and displays the RTC value on the clock.
*/
void State_DisplayRTC();

/*
	State_GetNewTime

	input:	none
	output:	changes button and display behavior
	descr:	Updates button functions and displays the intended RTC time on the
					clock.
*/
void State_GetNewTime();

/*
	State_GetAlarmTime

	input:	none
	output:	changes button and display behavior
	descr:	Updates button functions and displays the intended Alarm A time on the
					clock.
*/
void State_GetAlarmTime();

void State_AlarmActive();

void State_PlayMP3();
void State_PlayAux();

#endif /* STATE_MACHINE_H_ */
