//
//  scheduler.h
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


#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "stdint.h"
#include "stdbool.h"
#include "interrupt.h"

typedef struct sd_thread
{
	int8_t		priority;
	uint8_t		*stack;
	
	uint8_t		timeLeft;
	uint8_t		timeMax;
	uint16_t	timeSleep;
	
	uint8_t		id;
	
	ir_cpuState *state;
	void		(*entry)();
	
	bool died;
	
	struct sd_thread *next;
} sd_thread;

typedef struct sd_task
{
	uint32_t	pid;
	uint32_t	parent;
	
	sd_thread		*main_thread;
	sd_thread		*current_thread;
	
	char *name;
	bool died;
	struct sd_task *next;
} sd_task;

#define SD_PID_INVALID UINT32_MAX
#define SD_TID_INVALID UINT32_MAX

#define SD_THREAD_PRIORITY_DEFAULT	1 // 3 scheduler cycles
#define SD_THREAD_PRIORITY_BETTER	2 // 5 scheduler cycles
#define SD_THREAD_PRIORITY_TOO_MUCH 10 // 21 scheduler cycles


#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t sd_spawnTask(void (*entry)());
extern int sd_init();

extern void		sd_nameTask(char *name);
extern uint32_t sd_attachThread(void (*entry)());
extern int		sd_threadRunning(uint32_t threadId);
extern void		sd_threadSetPriority(uint8_t priority); // Something between SD_THREAD_PRIORITY_DEFAULT and SD_THREAD_PRIORITY_TOO_MUCH

extern sd_task *sd_taskWithPid(uint32_t pid);
extern ir_cpuState *sd_kill(uint32_t pid);
extern uint32_t sys_getpid();
extern uint32_t sys_getppid();

extern void sd_printTasks();

extern void sd_enableScheduler();
extern void sd_disableScheduler();
	
#ifdef __cplusplus
}
#endif

#endif
