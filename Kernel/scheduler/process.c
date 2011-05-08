//
//  process.c
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

#include "process.h"
#include "thread.h"

#include "scheduler.h"

#include <system/debug.h>
#include <memory/memory.h>
#include <system/system.h>
#include <system/string.h>
#include <system/console.h>
#include <system/keyboard.h>

extern sd_thread *sd_createThread(sd_process *process, sd_pentry entry, sd_threadPriority priority, sd_threadPrivilege privilege);
extern void sd_setCurrentProcess(sd_process *process);
extern void sd_setFirstProcess(sd_process *process);

// Returns a unique process ID
uint32_t sd_processUID()
{
	uint32_t uid = 0;
	
	bool found;
	do {
		found = true;
		sd_process *process = sd_getFirstProcess();
		
		while(process)
		{
			if(process->pid == uid)
			{
				found = false;
				break;
			}
			
			process = process->next;
		}
		
		if(!found)
			uid ++;
		
	} while(!found);
	
	return uid;
}



uint32_t sd_spawnProcess(sd_pentry entry)
{
	return sd_spawnProcessWithContext(entry, NULL);
}

uint32_t sd_spawnProcessWithContext(sd_pentry entry, vmm_context *acontext)
{
	if(!entry)
		return SD_PID_INVALID;
	
	sd_disableScheduler();
	
	
	uint32_t processPid = SD_PID_INVALID;
	mm_heap *heap = mm_createHeap(5*(32*4096), NULL); // 5 Mb heap
	if(!heap)
	{
		db_logLevel(DB_LEVEL_WARNING, "Could not create process (no heap in place!)\n");
		
		sd_enableScheduler();
		return SD_PID_INVALID;
	}
	
	sd_process *process = (sd_process *)mm_heapAlloc(heap, sizeof(sd_process));
	if(process)
	{
		process->heap			= heap;
		process->name			= (char *)mm_heapAlloc(heap, 4 * sizeof(char));
		process->identifier		= (char *)mm_heapAlloc(heap, 4 * sizeof(char));
		
		if(!process->name || !process->identifier)
		{
			if(process->name)		mm_heapFree(heap, process->name);
			if(process->identifier) mm_heapFree(heap, process->identifier);
			
			mm_heapFree(heap, process);
			mm_destroyHeap(heap);
			
			db_logLevel(DB_LEVEL_WARNING, "Could not create process (out of memory!)\n");
			
			sd_enableScheduler();
			return SD_PID_INVALID;
		}
		
		strcpy(process->name, "???");
		strcpy(process->identifier, "???");
		
		process->pid = sd_processUID();
		process->ppid = (sd_getCurrentProcess()) ? sd_getCurrentProcess()->pid : 0;
		
		process->died		= false;
		process->next		= NULL;
		process->mainThread = NULL;
		
		
		sd_createThread(process, entry, sd_threadPriorityNormal, sd_threadPrivilegeRing3);
		if(!process->mainThread)
		{
			mm_heapFree(heap, process->name);
			mm_heapFree(heap, process->identifier);
			mm_heapFree(heap, process);
					
			
			db_logLevel(DB_LEVEL_WARNING, "Could not create process (out of memory!)\n");
			sd_enableScheduler();
			return SD_PID_INVALID;
		}
		
		process->currentThread = process->mainThread;
		if(sd_getFirstProcess())
		{
			sd_process *pprocess = sd_getFirstProcess();
			while(pprocess)
			{
				if(!pprocess->next)
				{
					pprocess->next = process;
					break;
				}
				
				pprocess = pprocess->next;
			}
		}
		else 
		{
			sd_setFirstProcess(process);
			sd_setCurrentProcess(process);
		}
		
		db_conditionalLog(st_isVerbose(), "Created process: %i (t:%p, t:%p, s:%p)\n", process->pid, process, process->currentThread, process->currentThread->state);
		processPid = process->pid;
	}
	
	sd_enableScheduler();
	return processPid;
}


void sd_destroyProcess(sd_process *process)
{
	sd_thread *thread = process->mainThread;
	while(thread)
	{
		sd_destroyThread(thread);
		thread = thread->next;
	}
	
	
	kb_removeKeyboardHook(process->pid); // Remove a possible keyboard hook
	mm_destroyHeap(process->heap); // Let the kernel reclaim the processes memory
}



sd_process *sd_processWithPid(uint32_t pid)
{
	sd_process *process = sd_getFirstProcess();
	while(process)
	{
		if(process->pid == pid && !process->died)
			return process;
		
		
		process = process->next;
	}
	
	return NULL;
}

uint32_t sd_getPid()
{
	sd_process *process = sd_getCurrentProcess();
	if(process)
		return process->pid;
	
	return SD_PID_INVALID;
}

uint32_t sd_getPpid()
{
	sd_process *process = sd_getCurrentProcess();
	if(process)
		return process->ppid;
	
	
	return SD_PID_INVALID;
}



bool sd_setName(char *name)
{
	sd_process *process = sd_getCurrentProcess();
	
	if(process)
	{
		size_t	size = strlen(name);
		char*	temp = mm_heapRealloc(process->heap, process->name, (size + 1) * sizeof(char));
		
		if(temp)
		{
			strcpy(temp, name);
			process->name = temp;
			
			return true;
		}
	}
	
	return false;
}

bool sd_setIdentifier(char *identifier)
{
	sd_process *process = sd_getCurrentProcess();
	
	if(process)
	{
		size_t	size = strlen(identifier);
		char*	temp = mm_heapRealloc(process->heap, process->identifier, (size + 1) * sizeof(char));
		
		if(temp)
		{
			strcpy(temp, identifier);
			process->identifier = temp;
			
			return true;
		}
	}
	
	return false;
}
