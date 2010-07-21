//
//  time.c
//  NANOS
//
//  Created by Sidney Just
//  Copyright © 2010 by Sidney Just
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
//  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
//  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "time.h"

static uint8_t tm_clock_second = 0;
static uint8_t tm_clock_minute = 0;
static uint8_t tm_clock_hour = 0;

void tm_pollTime()
{
	tm_clock_second = cmos_readData(CMOS_REGISTER_SECOND);
	tm_clock_minute = cmos_readData(CMOS_REGISTER_MINUTE);
	tm_clock_hour	= cmos_readData(CMOS_REGISTER_HOUR);
}

void tm_init()
{
	cmos_setRTCFlags(CMOS_RTC_FLAG_24HOUR | CMOS_RTC_FLAG_BINARY);
	
	tm_pollTime();
	td_spawnProcess(timed, TD_TASK_PRIORITY_DEFAULT);
}

void timed()
{
	while(1)
	{
		syscall(syscall_type_clock); // Refresh the clock
		
		char temp[12]; // Time format xx:xx:xx = 8 characters + \0. However, if there is an error with the CMOS or the RTC, it's possible that we have something like: xxx:xxx:xxx = 11 + \0!
		memset(temp, 0, 12);
		
		sprintf(temp, "%i:%i:%i", tm_clock_hour, tm_clock_minute, tm_clock_second);
		
		int i;
		int j = strlen(temp);
		for(i=0; i<j; i++)
		{
			int pos = VIDEO_SIZE_X - (j - i);
			
			vd_setChar(pos, 0, temp[i]);
			vd_setAttribute(pos, 0, VIDEO_COLOR_WHITE);
		}
	}
}
