/*
 *
 *  Created on: 10 jul 2012
 *      Author: BenjaminVe
 */

#ifndef AUDIOMP3_H_
#define AUDIOMP3_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include "stm32f4xx_it.h"
#include "usb_hcd_int.h"
#include "usbh_usr.h"
#include "usbh_core.h"
#include "usbh_msc_core.h"




// Function prototypes
void TimingDelay_Decrement(void);
void Delay(volatile uint32_t nTime);
int audioToMp3(void);

// Set the global flags to start/stop playback from audioToMp3()
// which lives in the mail while(1) loop. Hacky, but better than
// refactoring audioToMp3();
void Audio_Start();
void Audio_Stop();

//volatile int snoozeButtonFlag;
volatile int snoozeMemory;
volatile int exitMp3;
volatile int mp3PlayingFlag;

#endif /* AUDIOMP3_H_ */
