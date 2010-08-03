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

static tm_date *_tm_current_date = NULL;
static bool		_tm_drawDate = true;

void tm_pollTime()
{
	if(_tm_current_date)
	{
		uint8_t temp = _tm_current_date->hour;
		
		_tm_current_date->second	= cmos_readData(CMOS_REGISTER_SECOND);
		_tm_current_date->minute	= cmos_readData(CMOS_REGISTER_MINUTE);
		_tm_current_date->hour		= cmos_readData(CMOS_REGISTER_HOUR);
		
		if(temp != _tm_current_date->hour) // Update the week, month, year etc. only every hour.
		{
			_tm_current_date->day_of_week	= cmos_readData(CMOS_REGISTER_WDAY);
			_tm_current_date->day_of_month	= cmos_readData(CMOS_REGISTER_DMONTH);
			
			_tm_current_date->month = cmos_readData(CMOS_REGISTER_MONTH);
			_tm_current_date->year	= cmos_readData(CMOS_REGISTER_YEAR);
		}
	}
}

tm_date *tm_current_date()
{
	return _tm_current_date;
}

void tm_drawDate(bool trueOrFalse)
{
	_tm_drawDate = trueOrFalse;
}

void timed()
{
	while(1)
	{
		syscall(syscall_type_clock); // Refresh the clock
		
		if(_tm_drawDate)
		{
			char temp[12];
			
			memset(temp, 0, 12);
			sprintf(temp, "%i:%i:%i", _tm_current_date->hour, _tm_current_date->minute, _tm_current_date->second);
			
			int i;
			int j = strlen(temp);
			int k = 0;
			
			for(i=0; i<12; i++)
			{
				int pos = VIDEO_SIZE_X - (12 - i);
				
				if(i >= 12 - j)
				{
					vd_setChar(pos, 0, temp[k]);
					k++;
				}
				else 
				{
					vd_setChar(pos, 0, ' ');
				}

				vd_setAttribute(pos, 0, VIDEO_COLOR_WHITE);
			}
		}
	}
}

void tm_init()
{
	_tm_current_date = (tm_date *)malloc(sizeof(tm_date));
	
	cmos_setRTCFlags(CMOS_RTC_FLAG_24HOUR | CMOS_RTC_FLAG_BINARY);
	td_spawnProcess(timed, TD_TASK_PRIORITY_DEFAULT);
}
