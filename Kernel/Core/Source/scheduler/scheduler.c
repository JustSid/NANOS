//
//  scheduler.c
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


#include "debug.h"
#include "assert.h"
#include "scheduler.h"
#include "cmos.h"
#include "string.h"
#include "port.h"
#include "gdt.h"
#include "console.h"
#include "memory.h"
#include "stdio.h"
#include "system.h"

#define SD_THREAD_STACK_SIZE 4096

static sd_task *sd_firstTask	= NULL;
static sd_task *sd_currentTask	= NULL;
static bool		sd_threadDied	= false;
static bool		sd_enabled		= true;

void sd_threadEntry();

sd_thread *sd_createThread(sd_task *task, void (*entry)(), uint8_t priority, uint16_t stackSize)
{
	if(!task || !entry || stackSize == 0)
		return NULL;
	
	if(priority > SD_THREAD_PRIORITY_TOO_MUCH)
		priority = SD_THREAD_PRIORITY_DEFAULT;
	
	sd_thread *thread = (sd_thread *)mm_alloc(sizeof(sd_task));
	if(thread)
	{
		thread->stack = (uint8_t *)mm_alloc(stackSize * sizeof(uint8_t));
		thread->state = (ir_cpuState *)mm_alloc(sizeof(ir_cpuState));
		
		if(!thread->stack || !thread->state)
		{
			if(!thread->stack) mm_free(thread->stack);
			if(!thread->state) mm_free(thread->state);
			
			mm_free(thread);
			
			db_conditionalLogLevel(DB_LEVEL_WARNING, st_isVerbose(), "Could not create thread for %i (out of memory!)\n", task->pid);
			return NULL;
		}
		
		thread->state->eax = 0;
		thread->state->ebx = 0;
		thread->state->ecx = 0;
		thread->state->edx = 0;
		thread->state->esi = 0;
		thread->state->edi = 0;
		thread->state->ebp = 0;
		
		thread->state->eip = (uint32_t)sd_threadEntry;
		thread->state->esp = (uint32_t)thread->stack + stackSize;
		
		thread->state->cs	= 0x18 | 0x03;
		thread->state->ss	= 0x20 | 0x03;
		
		thread->state->eflags = 0x200;		
		
		thread->priority	= priority;
		thread->timeLeft	= 1 + (priority * 2);
		thread->timeSleep	= 0;
		
		thread->entry	= entry;		
		thread->next	= NULL;
		
		if(task->main_thread)
		{
			sd_thread *_thread = task->main_thread;
			while(_thread)
			{
				if(!_thread->next)
				{
					_thread->next	= thread;
					thread->id		= _thread->id + 1;
					break;
				}
				
				_thread = _thread->next;
			}
		}
		else 
		{
			task->main_thread = thread;
			thread->id = 0;
		}		
	}
	
	return thread;
}

uint32_t sd_attachThread(void (*entry)())
{
	if(sd_currentTask)
	{
		sd_thread *thread = sd_createThread(sd_currentTask, entry, SD_THREAD_PRIORITY_DEFAULT, SD_THREAD_STACK_SIZE);
		
		if(thread) 
			return thread->id;
	}
	
	return SD_TID_INVALID;
}

int sd_threadRunning(uint32_t threadId)
{
	if(sd_currentTask)
	{
		sd_thread *thread = sd_currentTask->main_thread;
		while(thread)
		{
			if(thread->id == threadId && !thread->died) 
				return 1;
			
			thread = thread->next;
		}
	}
	
	return 0;
}

void sd_threadSetPriority(uint8_t priority)
{
	if(sd_currentTask)
	{
		if(priority > SD_THREAD_PRIORITY_TOO_MUCH)
			priority = SD_THREAD_PRIORITY_DEFAULT;
		
		sd_thread *thread = sd_currentTask->current_thread;
		thread->priority = priority;
	}
}

void sd_destroyThread(sd_thread *thread)
{
	if(thread)
	{
		mm_free(thread->stack);
		mm_free(thread->state);
		mm_free(thread);
	}
}





uint32_t sd_spawnTask(void (*entry)())
{
	if(!entry)
		return SD_PID_INVALID;
	
	sd_task *task = (sd_task *)mm_alloc(sizeof(sd_task));
	if(task)
	{
		task->name = (char *)mm_alloc(4 * sizeof(char)); // "???" as name. 3 Characters + \0
		if(!task->name)
		{
			mm_free(task);
			return SD_PID_INVALID;
		}
		
		strcpy(task->name, "???");
		
		// Search for a new pid
		uint32_t pid = 0;
		bool     found;
		
		do {
			found = true;
			
			sd_task *_task = sd_firstTask;
			while(_task) 
			{
				if(_task->pid == pid)
				{
					found = false;
					break;
				}
				
				_task = _task->next;
			}
			
			if(!found)
				pid += 1;
			
			if(pid == SD_PID_INVALID)
			{
				mm_free(task);
				return SD_PID_INVALID;
			}
			
		} while(!found);
		
		
		task->pid			= pid;
		task->parent		= (sd_currentTask) ? sd_currentTask->pid : SD_PID_INVALID;
		task->died			= false;		
		task->next			= NULL;
		task->main_thread	= NULL;
		
		
		sd_createThread(task, entry, 0, SD_THREAD_STACK_SIZE);
		if(!task->main_thread)
		{
			mm_free(task);
			
			db_conditionalLogLevel(DB_LEVEL_WARNING, st_isVerbose(), "Could not create task (out of memory!)\n");
			return SD_PID_INVALID;
		}
		
		task->current_thread = task->main_thread;
		
		if(sd_firstTask)
		{
			sd_task *_task = sd_firstTask;
			while(_task)
			{
				if(!_task->next)
				{
					_task->next = task;
					break;
				}
				
				_task = _task->next;
			}
		}
		else 
			sd_firstTask = task;
		
		db_conditionalLog(st_isVerbose(), "Created task: %i (t:%p, t:%p, s:%p)\n", task->pid, task, task->current_thread, task->current_thread->state);
		return task->pid;
	}
	
	return SD_PID_INVALID;
}

void sd_destroyTask(sd_task *task)
{
	sd_thread *thread = task->main_thread;
	while(thread)
	{
		sd_destroyThread(thread);
		thread = thread->next;
	}
	
	mm_free(task->name);
	mm_free(task);
}






void sd_threadEntry()
{
	if(sd_currentTask && sd_currentTask->current_thread)
	{
		sd_thread *thread = sd_currentTask->current_thread;
		
		thread->entry();
		thread->died	= true;
		sd_threadDied	= true;		
		
		while(1){} // Wait until the thread really dies
	}
}


// Collects all died threads and purges them from the scheduler list
void sd_cleanUp()
{
	sd_task *task	= sd_firstTask;
	sd_task *ptask	= NULL;
	
	while(task)
	{
		sd_thread *thread	= task->main_thread;
		sd_thread *pthread	= NULL;
		
		while(thread)
		{
			if(thread->died)
			{
				if(thread == task->main_thread)
				{
					task->died = true;
					break;
				}
				
				sd_thread *_thread = thread;
				
				thread = thread->next;
				pthread->next = thread;
				
				sd_destroyThread(_thread);
				continue;
			}
			
			
			pthread = thread;
			thread = thread->next;
		}
		
		
		
		
		if(task->died)
		{
			if(task == sd_firstTask)
			{
				sd_firstTask = task->next;
				
				task  = sd_firstTask;
				ptask = NULL;
			}
			
			
			if(task == sd_currentTask)
			{
				sd_currentTask = task->next;
				
				if(!sd_currentTask)
					sd_currentTask = sd_firstTask;
			}
			
			
			
			sd_task *_task = task;
			
			task = task->next;
			
			if(ptask)
				ptask->next = task;
			
			
			db_conditionalLog(st_isVerbose(), "Task %i (%s) exited\n", _task->pid, _task->name);
			sd_destroyTask(_task);
			continue;
		}
		
		ptask = task;
		task = task->next;
	}
}


ir_cpuState *sd_step(uint32_t intr, ir_cpuState *state)
{
	if(!sd_enabled)
	{
		if(intr > 0)
		{
			outb(0x70, 0x0C);
			inb(0x71);
		}
		
		return state;
	}
	
	
	
	sd_task *ptask = sd_currentTask;
	sd_cleanUp();
	
	if(ptask != sd_currentTask)
		state = NULL;
	
	
	assert(sd_currentTask);
	assert(sd_currentTask->current_thread);
	
	if(state) // Save the current CPU state
		memcpy(sd_currentTask->current_thread->state, state, sizeof(ir_cpuState));


	sd_currentTask->current_thread->timeLeft --;
	if(sd_currentTask->current_thread->timeLeft == 0) // The thread ran out of time, switch to the next one
	{
		sd_currentTask->current_thread->timeLeft = 1 + (sd_currentTask->current_thread->priority * 2);
		
		sd_currentTask->current_thread = sd_currentTask->current_thread->next;
		
		if(!sd_currentTask->current_thread)
			sd_currentTask->current_thread = sd_currentTask->main_thread;
		
		
		// Now that the task has its next thread, switch to the next task and make it the current task
		sd_currentTask = sd_currentTask->next;
		
		if(!sd_currentTask)
			sd_currentTask = sd_firstTask;
	}

	
	if(intr > 0)
	{
		outb(0x70, 0x0C);
		inb(0x71);
	}
	
	if(sd_currentTask && sd_currentTask->current_thread && sd_currentTask->current_thread->state)
	{
		gdt_setTSSEntry((uint32_t)(sd_currentTask->current_thread->state + 1), 1);
		return sd_currentTask->current_thread->state;
	}
	
	
	return state; // Normally this point shouldn't be reached, and there is a small chance that it will return NULL and cause a kernel panic!
}





ir_cpuState *sd_kill(uint32_t pid)
{
	sd_task *task = sd_taskWithPid(pid);
	if(task)
	{
		task->died = true;
		sd_threadDied = true;
		
		mm_clear(pid);
		return sd_step(0, NULL);
	}
	
	return NULL;
}

void sd_nameTask(char *name)
{
	if(sd_currentTask)
	{
		char *oldname = sd_currentTask->name;		
		sd_currentTask->name = (char *)mm_alloc((strlen(name) + 1) * sizeof(char));
		
		if(sd_currentTask->name) 
		{
			strcpy(sd_currentTask->name, name);
			mm_free(oldname);
		}
		else 
			sd_currentTask->name = oldname;
	}
}

sd_task *sd_taskWithPid(uint32_t pid)
{
	if(sd_firstTask)
	{
		sd_task *task = sd_firstTask;
		while(task)
		{
			if(task->pid == pid && !task->died)
				return task;
			
			task = task->next;
		}
	}
	
	return NULL;
}

uint32_t sys_getpid()
{
	return sd_currentTask ? sd_currentTask->pid : SD_PID_INVALID;
}

uint32_t sys_getppid()
{
	return sd_currentTask ? sd_currentTask->parent : SD_PID_INVALID;
}


// Debug process dump function
void sd_printTasks()
{
	sd_task *task = sd_firstTask;
	if(task)
	{
		cn_printf("Pid          Name                         Threads\n");
		
		while(task) 
		{
			int threadCount = 0;
			sd_thread *thread = task->main_thread;
			
			while(thread)
			{
				threadCount ++;
				thread = thread->next;
			}
			
			
			char string[33];
			int length;
			
			length = sprintf(string, "%i", (int)task->pid);
			cn_puts(string);
			
			int i, rest = 13 - length;
			for(i=0; i<rest; i++)
				cn_putc(' ');
			
			
			
			length = sprintf(string, "%s", task->name);
			cn_puts(string);
			
			
			rest = 29 - length;
			for(i=0; i<rest; i++)
				cn_putc(' ');
			
			
			
			cn_printf("%i\n", threadCount);			
			task = task->next;
		}
	}
}


// Scheduler state
void sd_enableScheduler()
{
	sd_enabled = true;
}

void sd_disableScheduler()
{
	sd_enabled = false;
}


// -------
void kernelTask();

int sd_init()
{
	cn_printf("Initializing scheduler...");

	if(sd_spawnTask(kernelTask) == SD_PID_INVALID || !ir_installInterruptHandler(sd_step, 0x28, 0x28))
		return 0;
	
	sd_currentTask = sd_firstTask;
	// The very first task is the kernel task, however, the sd_spawnTask() spins up an temporary ring 3 task. The kernel spins up the real kernel task by just calling the kernelTask()
	// function in void boot(). Once the scheduler does its first step, it assumes that it comes right out of the real kernel task (or just a few calls before the kernel spins it up)
	// the scheduler will overwrite the first tasks state with the ring 0 kernel task state.
	
	
	cmos_appendRTCFlags(CMOS_RTC_FLAG_PERIODICINT);	
	cn_printf("ok\n");
	
	return 1;
}
