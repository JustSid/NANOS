//
//  thread.h
//  NANOS
//
//  Created by Sidney Just
//  Copyright (c) 2011 by Sidney Just
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

#ifndef _THREAD_H_
#define _THREAD_H_

#include <system/stdint.h>
#include <system/stdbool.h>
#include <system/interrupt.h>

typedef enum
{
	sd_threadPriorityIdle,
	sd_threadPriorityLowest,
	sd_threadPriorityNormal,
	sd_threadPriorityHighest,
	sd_threadPriorityTimeCritical,
	
	sd_threadPriorityDynamic
} sd_threadPriority;

typedef enum
{
	sd_threadPrivilegeRing0,
	sd_threadPrivilegeRing3
} sd_threadPrivilege;

typedef void (*sd_tentry)();

typedef struct sd_thread
{
	uint8_t	id;
	uint8_t	*stack;
	
	uint8_t	priority;
	
	uint8_t	usedTicks;
	uint8_t grantedTicks;
	
	ir_cpuState *state;
	
	sd_threadPrivilege	privilege;
	sd_tentry			entry;
	
	bool died;
	bool wantsSleep;
	
	uint8_t tickDiff[5]; // Difference between used and granted ticks over the last five scheduler cycles
	
	struct sd_thread *next;
} sd_thread;

#define SD_TID_INVALID UINT32_MAX

extern uint32_t sd_attachThread(sd_tentry entry, sd_threadPriority priority);
extern void sd_destroyThread(sd_thread *thread);

extern sd_thread *sd_currentThread();
extern sd_thread *sd_threadWithId(uint32_t id);

extern void sd_threadSetPriority(sd_threadPriority priority);

#endif
