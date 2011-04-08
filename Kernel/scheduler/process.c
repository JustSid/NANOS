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

extern sd_thread *sd_createThread(sd_process *process, sd_pentry entry, sd_threadPriority priority, sd_threadPrivilege privilege);
extern void sd_setCurrentProcess(sd_process *process);
extern void sd_setFirstProcess(sd_process *process);

// Returns a unique thread ID for the given process
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
	
	
	vmm_context *context;
	
	if(!acontext)
	{
		context = vmm_createContext();
		if(!context)
			return SD_PID_INVALID;
	}
	else
		context = acontext;
	
	
	sd_process *process = (sd_process *)mm_alloc(sizeof(sd_process));
	if(process)
	{
		process->name			= (char *)mm_allocContext(4 * sizeof(char), context);
		process->identifier		= (char *)mm_allocContext(4 * sizeof(char), context);
		process->context		= context;
		process->ownedContext	= (acontext) ? false : true;
		
		if(!process->name || !process->identifier || !process->context)
		{
			if(process->name)		mm_free(process->name);
			if(process->identifier) mm_free(process->identifier);
			
			mm_free(process);
			
			if(process->ownedContext)
				vmm_destroyContext(process->context);
			
			db_logLevel(DB_LEVEL_WARNING, "Could not create process (out of memory!)\n");
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
			mm_free(process->name);
			mm_free(process->identifier);
			mm_free(process);
			
			if(process->ownedContext)
				vmm_destroyContext(process->context);
			
			db_logLevel(DB_LEVEL_WARNING, "Could not create process (out of memory!)\n");
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
		
		db_conditionalLog(st_isVerbose(), "Created task: %i (t:%p, t:%p, s:%p)\n", process->pid, process, process->currentThread, process->currentThread->state);
		return process->pid;
	}
	
	return SD_PID_INVALID;
}


void sd_destroyProcess(sd_process *process)
{
	sd_thread *thread = process->mainThread;
	while(thread)
	{
		sd_destroyThread(thread);
		thread = thread->next;
	}
	
	
	vmm_context *context = (process->ownedContext) ? process->context : NULL;
	
	mm_free(process->name);
	mm_free(process->identifier);
	mm_free(process);
	
	if(context)
		vmm_destroyContext(context);
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
	if(sd_getCurrentProcess())
	{
		return sd_getCurrentProcess()->pid;
	}
	
	return SD_PID_INVALID;
}

uint32_t sd_getPpid()
{
	if(sd_getCurrentProcess())
	{
		return sd_getCurrentProcess()->ppid;
	}
	
	return SD_PID_INVALID;
}



bool sd_setName(char *name)
{
	if(sd_getCurrentProcess())
	{
		size_t	size = strlen(name);
		char*	temp = mm_alloc((size + 1) * sizeof(char));
		
		if(temp)
		{
			strcpy(temp, name);
			
			mm_free(sd_getCurrentProcess()->name);
			sd_getCurrentProcess()->name = temp;
		}
	}
	
	return false;
}

bool sd_setIdentifier(char *identifier)
{
	if(sd_getCurrentProcess())
	{
		size_t	size = strlen(identifier);
		char*	temp = mm_alloc((size + 1) * sizeof(char));
		
		if(temp)
		{
			strcpy(temp, identifier);
			
			mm_free(sd_getCurrentProcess()->identifier);
			sd_getCurrentProcess()->identifier = temp;
		}
	}
	
	return false;
}
