//
//  taskd.c
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

#include "taskd.h"

static td_process *first_process	= NULL;
static td_process *last_process		= NULL;
static td_process *current_process	= NULL;

static bool td_locked		= false;
static bool td_processDied	= false;

static uint32_t td_frame = 0;
static int16_t	td_locks = 0; // The number of locks. If this is zero, the taskd isn't locked


void td_spawnProcess(void *entryPoint, uint8_t priority)
{
	if(!entryPoint)
	{
		return;
	}
	
	if(priority > TD_TASK_PRIORITY_CRITICAL || priority < TD_TASK_PRIORITY_IDLE)
	{
		priority = TD_TASK_PRIORITY_DEFAULT;
	}
	
	td_locked = true;
	td_process *process = (td_process *)malloc(sizeof(td_process));

	if(process)
	{
		process->pstack = (uint8_t *)malloc(4096 * sizeof(uint8_t)); 
		
		if(!process->pstack)
		{
			free(process);
			return;
		}
		
		process->pstate = (ir_cpu_state *)malloc(sizeof(ir_cpu_state));
		
		if(!process->pstate)
		{
			free(process->pstack);
			free(process);
			
			return;
		}
		
		
		if(!first_process)
		{
			first_process = process;
			last_process  = first_process;
		}
		else 
		{
			last_process->next = process;
			last_process = last_process->next;
		}
		
		
		// Looks for an unique pid, this will be later used to identifie a process
		uint32_t pid = 0;
		bool     found = false;
		
		while(!found)
		{
			found = true;
			
			td_process *temp = first_process;
			while(temp) 
			{
				if(process != temp)
				{
					if(temp->pid == pid)
					{
						found = false;
						break;
					}
				}
				
				temp = temp->next;
			}
			
			if(!found)
				pid += 1;
			
			if(pid == UINT32_MAX)
			{
				return;
			}
		}
		
		
		// Ring 3 CPU State
		struct ir_cpu_state state = 
		{
			.eax = 0,
			.ebx = 0,
			.ecx = 0,
			.edx = 0,
			.esi = 0,
			.edi = 0,
			.ebp = 0,
			
			.eip	= (uint32_t)entryPoint,
			.esp	= (uint32_t)(process->pstack + 4096),
			
			.cs		= 0x18 | 0x03,
			.ss		= 0x20 | 0x03,
			.eflags = 0x200,
		};
		
		// Process setup
		process->pid = pid;
		process->priority = priority;
		*process->pstate = state;
		
		process->next = NULL;
	}
	
	td_locked = false;
}

// -------------------------------
// Task management:
// Manages the task switches and process exits.
// -------------------------------

ir_cpu_state *td_fireRunloop(uint32_t intr, ir_cpu_state *state)
{
	if(td_locked) // The taskd is locked, so we don't want to switch the current context
		return state;
	
	if(!first_process)
		return NULL;
	
	if(current_process && !td_processDied)
	{
		if((td_frame % current_process->priority) == 0)
		{
			current_process->pstate = state;
			current_process = current_process->next;

			//cn_printf("Switched to process %i", current_process->pid);
		}
		
		td_frame += 1;
	}
	
	if(!current_process)
		current_process = first_process;
	
	td_processDied = false;
	
	gdt_setTSSEntry((uint32_t)(current_process->pstate + 1));
	return current_process->pstate;
}

void td_taskExit()
{
	td_process *temp = first_process;
	while(temp->next && temp->next != current_process) 
	{
		temp = temp->next;
	}
	
	if(temp)
	{
		free(current_process->pstack);
		free(current_process);
		
		td_processDied = true;
		
		temp->next = temp->next->next;		
		current_process = temp->next;
	}
}

void td_exit(int state)
{
	td_locked = true;
	td_taskExit();
	td_locked = false;
}

// -------------------------------
// Runtime:
// Controls the locking and provides functions for processes
// -------------------------------

void td_lock()
{
	td_locks += 1;
	td_locked = true;
}

void td_unlock()
{
	td_locks -= 1;
	
	if(td_locks <= 0)
	{
		td_locked = false;
		td_locks = 0;
	}
}

uint32_t td_currentPid()
{
	if(current_process)
	{
		return current_process->pid;
	}
	
	return 0;
}

void td_runLoop()
{
	while(1)
	{
		
	}
}

// -------------------------------
// Init
// -------------------------------

void td_init()
{
	cn_puts("Initializing taskd...");
	
	td_spawnProcess(td_runLoop, TD_TASK_PRIORITY_IDLE); // We need at least one process.
	ir_addInterrupt(0x20, 0x20, td_fireRunloop);
	
	cn_puts("[ok]\n");
}
