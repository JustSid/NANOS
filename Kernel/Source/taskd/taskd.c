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

void td_spawnProcess(void *entryPoint)
{
	td_locked = true;
	td_process *process = (td_process *)malloc(sizeof(td_process));

	if(process)
	{
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
		
		// Ring 3 State
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
		
		process->pid = pid;
		process->pstate = (void *)(process->pstack + 4096) - sizeof(ir_cpu_state);
		*process->pstate = state;
		
		process->next = NULL;
		
		//cn_printf("Spawning process %p with pid %i\n", entryPoint, pid);
	}
	
	td_locked = false;
}

ir_cpu_state *td_fireRunloop(uint32_t intr, ir_cpu_state *state)
{
	if(td_locked)
		return state;
	
	if(!first_process)
		return NULL;
	
	if(current_process && !td_processDied)
	{
		current_process->pstate = state;
		current_process = current_process->next;
	}
	
	if(!current_process)
	{
		current_process = first_process;
	}
	
	//cn_printf("Switching to process %i\n", current_process->pid);
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

uint32_t td_currentPid()
{
	if(current_process)
	{
		return current_process->pid;
	}
	
	return 0;
}

void td_lock()
{
	td_locked = true;
}

void td_unlock()
{
	td_locked = false;
}

int td_runLoop(int argc, char *argv[])
{
	while(1)
	{
		
	}
	
	return EXIT_SUCCESS;
}

void td_init()
{
	cn_puts("Initializing taskd...");
	
	td_spawnProcess(td_runLoop);
	ir_addInterrupt(0x20, 0x20, td_fireRunloop);
	
	cn_puts("[ok]\n");
}
