//
//  time.h
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

#ifndef _TIME_H_
#define _TIME_H_

#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "malloc.h"
#include "cmos.h"
#include "taskd.h"
#include "syscall.h"
#include "video.h"

typedef struct
{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	
	uint8_t day_of_week;
	uint8_t day_of_month;
	
	uint8_t month;
	uint8_t year; // only the last two digits of the year. However, we have currently 2010, it's impossible that this kernel runs in 19xx and 21xx is far away. 
	// So, just add 2000 if you want the real year
} tm_date;

extern void tm_init();
extern void tm_pollTime();

extern void tm_drawDate(bool trueOrFalse);

extern tm_date *tm_currentDate();

#endif
