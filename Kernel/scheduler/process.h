//
//  process.h
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

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <memory/memory.h>

#include <system/stdint.h>
#include <system/stdbool.h>

#define SD_PID_INVALID UINT32_MAX

struct sd_thread;

typedef void (*sd_pentry)();

typedef struct sd_process
{
	uint32_t	pid;
	uint32_t	ppid;
	
	struct sd_thread	*mainThread;
	struct sd_thread	*currentThread;
	
	char *name;
	char *identifier;
	
	bool ownedContext;
	vmm_context *context;
	
	mm_heap *heap;
	
	bool died;
	struct sd_process *next;
} sd_process;

extern uint32_t sd_spawnProcess(sd_pentry entry);
extern uint32_t sd_spawnProcessWithContext(sd_pentry entry, vmm_context *acontext);

extern void sd_destroyProcess(sd_process *process);

extern sd_process	*sd_processWithPid(uint32_t pid);
extern uint32_t		sd_getPid();
extern uint32_t		sd_getPpid();

extern bool sd_setName(char *name);
extern bool sd_setIdentifier(char *identifier);

#endif
