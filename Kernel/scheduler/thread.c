//
//  thread.c
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

#include "thread.h"
#include "process.h"

#include "scheduler.h"

#include <system/debug.h>
#include <memory/memory.h>

#define SD_THREAD_STACK_SIZE 4096

extern void sd_threadEntry(); // scheduler.c

// Returns a unique thread ID for the given process
uint32_t sd_threadUID(sd_process *process)
{
	uint32_t uid = 0;
	
	bool found;
	do {
		found = true;
		sd_thread *thread = process->mainThread;
		
		while(thread)
		{
			if(thread->id == uid)
			{
				found = false;
				break;
			}
			
			thread = thread->next;
		}
		
		if(!found)
			uid ++;
		
	} while(!found);
	
	return uid;
}

sd_thread *sd_createThread(sd_process *process, sd_tentry entry, sd_threadPriority priority, sd_threadPrivilege privilege)
{
	if(!process || !entry)
		return NULL;
	
	sd_thread *thread = (sd_thread *)mm_allocContext(sizeof(sd_thread), process->context);
	if(thread)
	{
		thread->stack = (uint8_t *)mm_allocContext(SD_THREAD_STACK_SIZE * sizeof(uint8_t), process->context);
		thread->state = (ir_cpuState *)mm_allocContext(sizeof(ir_cpuState), process->context);
		
		if(!thread->stack || !thread->state)
		{
			if(thread->stack) mm_free(thread->stack);
			if(thread->state) mm_free(thread->state);
			
			mm_free(thread);
			db_logLevel(DB_LEVEL_WARNING, "Could not create thread for %i (out of memory!)\n", process->pid);
		}
		
		thread->state->eax = 0;
		thread->state->ebx = 0;
		thread->state->ecx = 0;
		thread->state->edx = 0;
		thread->state->esi = 0;
		thread->state->edi = 0;
		thread->state->ebp = 0;
		
		thread->state->eip = (uint32_t)sd_threadEntry;
		thread->state->esp = (uint32_t)thread->stack + SD_THREAD_STACK_SIZE;
		
		if(privilege == sd_threadPrivilegeRing0)
		{
			thread->state->cs	= 0x18;
			thread->state->ss	= 0x20;
		}
		else
		if(privilege == sd_threadPrivilegeRing3)
		{
			thread->state->cs	= 0x18 | 0x03;
			thread->state->ss	= 0x20 | 0x03;				
		}
		
		thread->state->eflags = 0x200;	
		
		thread->priority		= priority;
		thread->usedTicks		= 0;
		thread->grantedTicks	= 2;
		
		thread->died		= false;
		thread->wantsSleep	= false;
		
		thread->tickDiff[0] = 0;
		thread->tickDiff[1] = 0;
		thread->tickDiff[2] = 0;
		thread->tickDiff[3] = 0;
		thread->tickDiff[4] = 0;
		
		thread->entry	= entry;		
		thread->next	= NULL;
		
		
		if(process->mainThread)
		{
			sd_thread *tthread = process->mainThread;
			while(tthread)
			{
				if(!tthread->next)
				{
					thread->id = sd_threadUID(process);
					tthread->next = thread;
					break;
				}
				
				tthread = thread->next;
			}
		}
		else 
		{
			process->mainThread		= thread;
			process->currentThread	= thread;
		}
	}
	
	return thread;
}

uint32_t sd_attachThread(sd_tentry entry, sd_threadPriority priority)
{
	if(sd_getCurrentProcess() && entry)
	{
		sd_thread *thread = sd_createThread(sd_getCurrentProcess(), entry, priority, sd_getCurrentProcess()->currentThread->privilege);
		if(thread)
			return thread->id;
	}
	
	return SD_TID_INVALID;
}


void sd_destroyThread(sd_thread *thread)
{
	if(thread && thread->died)
	{
		mm_free(thread->stack);
		mm_free(thread->state);
		
		mm_free(thread);
	}
}



sd_thread *sd_currentThread()
{
	if(sd_getCurrentProcess())
		return sd_getCurrentProcess()->currentThread;
	
	return NULL;
}

sd_thread *sd_threadWithId(uint32_t id)
{
	if(sd_getCurrentProcess())
	{
		sd_thread *thread = sd_getCurrentProcess()->mainThread;
		while(thread)
		{
			if(thread->id == id)
				return thread;
			
			thread = thread->next;
		}
	}
	
	return NULL;
}



void sd_threadSetPriority(sd_threadPriority priority)
{
	if(sd_getCurrentProcess())
	{
		sd_getCurrentProcess()->currentThread->priority = priority;
	}
}
