//This code is used with the AlarmClockMp3 project
//it is called when incrementing the time manually and
//it ensures the time values stay within the bounds of a 12 hour clock

#include "timeKeeping.h"

void timeHourCheck(void)
{
	 //9 rolls over to 0
	 if((myclockTimeStruct.RTC_Hours & 0x0F) >= 0xA)
	 {
		 myclockTimeStruct.RTC_Hours = (myclockTimeStruct.RTC_Hours & 0x30 ) + 0x10;

	 }

	 //if incrementing passes 12 -> toggles am/pm
	 if(myclockTimeStruct.RTC_Hours == 0x12 )
	 {
		 myclockTimeStruct.RTC_H12 ^= 0x40;
	 }

 //if incrementing hits hour 13, sets to 1 oclock
	 if(myclockTimeStruct.RTC_Hours >= 0x13 )
	 {
		 myclockTimeStruct.RTC_Hours = 0x01;
	 }

}

void timeMinuteCheck(void)
{
	//allows 9 to roll over to 0 and 60 to 0
	 if((myclockTimeStruct.RTC_Minutes & 0x0F) >= 0xA)
	 {
		 myclockTimeStruct.RTC_Minutes = (myclockTimeStruct.RTC_Minutes & 0x70 ) + 0x10;
		 if((myclockTimeStruct.RTC_Minutes & 0x70 ) >= 0x60 )
		 {
			 myclockTimeStruct.RTC_Minutes = 0x00;
		 }
	 }
}

void alarmHourCheck(void)
{
			//9 rolls over to 0
			 if((AlarmStruct.RTC_AlarmTime.RTC_Hours & 0x0F) >= 0xA)
			 {
				 AlarmStruct.RTC_AlarmTime.RTC_Hours = (AlarmStruct.RTC_AlarmTime.RTC_Hours & 0x30 ) + 0x10;

			 }

			 //if incrementing passes 12 -> toggles am/pm
			 if(AlarmStruct.RTC_AlarmTime.RTC_Hours == 0x12 )
			 {
				 AlarmStruct.RTC_AlarmTime.RTC_H12 ^= 0x40;
			 }

			 //if incrementing hits hour 13, sets to 1 oclock
			 if(AlarmStruct.RTC_AlarmTime.RTC_Hours >= 0x13 )
			 {
				  AlarmStruct.RTC_AlarmTime.RTC_Hours = 0x01;
			 }

}

void alarmMinuteCheck(void)
{
	 //allows 9 to roll over to 0 and 60 to 0
	 if((AlarmStruct.RTC_AlarmTime.RTC_Minutes & 0x0F) >= 0xA)
	 {
		 AlarmStruct.RTC_AlarmTime.RTC_Minutes = (AlarmStruct.RTC_AlarmTime.RTC_Minutes & 0x70 ) + 0x10;
		 if((AlarmStruct.RTC_AlarmTime.RTC_Minutes & 0x70 ) >= 0x60 )
		 {
			 AlarmStruct.RTC_AlarmTime.RTC_Minutes = 0x00;
		 }
	 }
}
