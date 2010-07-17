//
//  taskd.h
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

#ifndef _TASKD_H_
#define _TASKD_H_

#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"

#include "interruptManager.h"
#include "gdt.h"

#include "console.h"
#include "video.h"
#include "kernel.h"

extern void td_init();
extern void td_spawnProcess(void *entryPoint);
extern void td_exit(int state);

extern uint32_t td_currentPid();

extern void td_lock();
extern void td_unlock();

typedef struct td_process
{
	uint8_t			pstack[4096];
	uint32_t		pid;
	
	ir_cpu_state	*pstate;
	
	struct td_process *next;
} td_process;

#endif
