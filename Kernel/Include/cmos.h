//
//  cmos.h
//  NANOS
//
//  Created by Sidney Just
//  Copyright (c) 2010 by Sidney Just
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

#ifndef _CMOS_H_
#define _CMOS_H_

#include "stdint.h"

#define CMOS_REGISTER_SECOND	0x00
#define CMOS_REGISTER_MINUTE	0x02
#define CMOS_REGISTER_HOUR		0x04
#define CMOS_REGISTER_WDAY		0x06
#define CMOS_REGISTER_DMONTH	0x07
#define CMOS_REGISTER_MONTH		0x08
#define CMOS_REGISTER_YEAR		0x09

#define CMOS_REGISTER_STATEA	0x0A
#define CMOS_REGISTER_STATEB	0x0B

extern uint8_t	cmos_readData(uint8_t offset);
extern void		cmos_setData(uint8_t offset, uint8_t data);

#define CMOS_RTC_FLAG_SUMMERTIME	1 
#define CMOS_RTC_FLAG_24HOUR		2
#define CMOS_RTC_FLAG_BINARY		4
#define CMOS_RTC_FLAG_FREQUENCY		8
#define CMOS_RTC_FLAG_INTERRUPT		16
#define CMOS_RTC_FLAG_ALARM			32
#define CMOS_RTC_FLAG_PERIODICINT	64
#define CMOS_RTC_FLAG_NOUPDATE		128

extern void cmos_setRTCFlags(uint8_t flags);
extern void cmos_appendRTCFlags(uint8_t flags);
extern void cmos_removeRTCFlags(uint8_t flags);

#endif
