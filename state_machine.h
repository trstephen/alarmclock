#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

typedef void(*StateFunc_T)(void);

void State_ButtonDisabled();
void State_ToggleRedLED();
void State_ToggleBlueLED();
void State_ToggleOrangeLED();
void State_ToggleGreenLED();

#endif /* STATE_MACHINE_H_ */
