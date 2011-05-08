//
//  scheduler.c
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


#include "scheduler.h"
#include "process.h"

#include <system/assert.h>
#include <system/debug.h>

#include <system/stdio.h>
#include <system/string.h>

#include <system/cmos.h>
#include <system/port.h>
#include <system/gdt.h>
#include <system/console.h>
#include <system/system.h>
#include <system/syscall.h>
#include <system/keyboard.h>
#include <video/video.h>

#include <memory/memory.h>

static bool sd_enabled = true;

static sd_process *sd_firstProcess = NULL;
static sd_process *sd_currentProcess = NULL;

// Entry point for new threads
void sd_threadEntry()
{
	if(sd_currentProcess && sd_currentProcess->currentThread)
	{
		sd_thread *thread = sd_currentProcess->currentThread;
		
		thread->entry(); // Call the process entry function, when this function returns has the thread already finished its work
		thread->died = true; // So mark it as dead
		
		while(1){} // Wait until the thread got removed from the internal list
	}
}


// Collects all died threads and purges them from the scheduler list
void sd_cleanUp()
{
	sd_process *process		= sd_firstProcess;
	sd_process *pprocess	= NULL;
	
	while(process)
	{
		sd_thread *thread	= process->mainThread;
		sd_thread *pthread	= NULL;
		
		while(thread)
		{
			if(thread->died)
			{
				if(thread == process->mainThread)
				{
					// When the main thread dies, the whole process is dead
					process->died = true;
					break;
				}
				
				
				if(process->currentThread == thread)
					process->currentThread = process->mainThread;
				
				sd_thread *diedThread = thread;
				
				thread = thread->next;
				pthread->next = thread;
				
				sd_destroyThread(diedThread);
				continue;
			}
			
			pthread = thread;
			thread = thread->next;
		}
		
		
		
		// Clean up the died process
		if(process->died)
		{
			sd_process *diedProcess = process;
			
			if(diedProcess == sd_firstProcess)
			{
				process = sd_firstProcess = process->next;
				pprocess = NULL;
			}
			else 
			{
				process = process->next;
				pprocess->next = process;
			}

			
			db_conditionalLog(st_isVerbose(), "Task %i (%s) exited\n", diedProcess->pid, diedProcess->name);
			sd_destroyProcess(diedProcess);
			continue;
		}
		
		pprocess = process;
		process  = process->next;
	}
}





ir_cpuState *sd_schedule(ir_cpuState *state)
{
	uint32_t intr = (state) ? state->intr : 0;
	
	if(!sd_enabled)
	{
		if(intr > 0)
		{
			outb(0x70, 0x0C);
			inb(0x71);
		}
		
		return state;
	}

	sd_disableScheduler();
	
	sd_process *process = sd_currentProcess;	
	sd_cleanUp();
	
	if(process != sd_currentProcess)
		process = NULL;
	
	
	assert(sd_currentProcess);
	assert(sd_currentProcess->currentThread);
	
	
	if(state) // Save the current CPU state
		memcpy(sd_currentProcess->currentThread->state, state, sizeof(ir_cpuState));
	
	
	sd_thread *currentThread = sd_currentProcess->currentThread;
	currentThread->usedTicks ++;
	
	if(currentThread->usedTicks >= currentThread->grantedTicks || currentThread->wantsSleep)
	{		
		currentThread->usedTicks = 0;
		currentThread->wantsSleep = false;
		
		// TODO: Re-evaluate the granted ticks based on the priority and used ticks
		
		sd_currentProcess->currentThread = sd_currentProcess->currentThread->next;
		if(!sd_currentProcess->currentThread)
			sd_currentProcess->currentThread = sd_currentProcess->mainThread;
		
		
		kb_clearKeyboardHook(sd_currentProcess->pid); // Reset the keyboards index
		sd_currentProcess = sd_currentProcess->next;
		
		if(!sd_currentProcess)
			sd_currentProcess = sd_firstProcess;
	}
	
	
	
	if(intr == 0x28)
	{
		outb(0x70, 0x0C);
		inb(0x71);
	}
	
	if(sd_currentProcess && sd_currentProcess->currentThread && sd_currentProcess->currentThread->state)
	{
		gdt_setTSSEntry((uint32_t)(sd_currentProcess->currentThread->state + 1), 1);
		//vmm_activateContext(sd_currentProcess->context);
		
		sd_enableScheduler();
		return sd_currentProcess->currentThread->state;
	}
	
	
	sd_enableScheduler();
	return state; // Normally this point shouldn't be reached, but if it does, there is a small chance that it will return NULL and cause a kernel panic!
}

ir_cpuState *sd_violentKill() // Marks the current process as dead and purges it from the internal list, no questions asked
{
	if(sd_currentProcess)
	{
		sd_currentProcess->died = true;
		return sd_schedule(NULL);
	}
	
	return NULL;
}


void sd_printProcesses()
{
	sd_process *process = sd_firstProcess;
	if(process)
	{
		cn_printf("Pid          Name                         Threads\n");
		
		while(process) 
		{
			int threadCount = 0;
			sd_thread *thread = process->mainThread;
			
			while(thread)
			{
				threadCount ++;
				thread = thread->next;
			}
			
			
			
			char string[33];
			int length;
			
			length = sprintf(string, "%i", (int)process->pid);
			cn_puts(string);
			
			int i, rest = 13 - length;
			for(i=0; i<rest; i++)
				cn_putc(' ');
			
			
			
			length = sprintf(string, "%s", process->name);
			cn_puts(string);
			
			
			rest = 29 - length;
			for(i=0; i<rest; i++)
				cn_putc(' ');
			
			
			
			cn_printf("%i\n", threadCount);			
			process = process->next;
		}
	}
}



sd_process *sd_getCurrentProcess()
{
	return sd_currentProcess;
}

sd_process *sd_getFirstProcess()
{
	return sd_firstProcess;
}


void sd_setCurrentProcess(sd_process *process)
{
	sd_currentProcess = process;
}

void sd_setFirstProcess(sd_process *process)
{
	sd_firstProcess = process;
}


sd_thread *sd_getCurrentThread()
{
	return (sd_currentProcess) ? sd_currentProcess->currentThread : NULL;
}




// Scheduler state
static sd_mutex __sd_stateMutex;

void sd_enableScheduler()
{
	sd_mutexLock(&__sd_stateMutex);
	sd_enabled = true;
	sd_mutexUnlock(&__sd_stateMutex);
}

void sd_disableScheduler()
{
	sd_mutexLock(&__sd_stateMutex);
	sd_enabled = false;
	sd_mutexUnlock(&__sd_stateMutex);
}


// Syscalls
uint32_t sd_sleepExec(ir_cpuState *state, ir_cpuState **returnState)
{
	if(sd_currentProcess)
	{
		sd_currentProcess->currentThread->wantsSleep = true;
		
		ir_cpuState *newState = sd_schedule(state);
		*returnState = newState;
	}
	
	return 0;
}



uint32_t sd_killExec(ir_cpuState *state, ir_cpuState **returnState)
{
	uint32_t pid = *((uint32_t *)(state->esp));

	if(sd_currentProcess->pid == pid)
	{
		sd_currentProcess->died = true;
		
		ir_cpuState *newState = sd_schedule(state);
		*returnState = newState;
	}
	else 
	{
		sd_process *process = sd_firstProcess;
		
		while(process)
		{
			if(process->pid == pid)
			{
				process->died = true; // Just mark the process as dead and let the scheduler collect the body on its next schedule cycle
				break;
			}
			
			process = process->next;
		}
	}
	
	return 0;
}

uint32_t sd_getInfoExec(ir_cpuState *state, ir_cpuState **returnState)
{
	uint32_t flag = *((uint32_t *)(state->esp));	
	uint32_t pid = *((uint32_t *)(state->esp + 4));

	sd_process *process = sd_processWithPid(pid);
	if(process)
	{
		switch(flag)
		{
			case sys_getInfoFlagName:
				return (uint32_t)process->name;
				break;
				
			case sys_getInfoFlagIdentifier:
				return (uint32_t)process->identifier;
				break;
				
			case sys_getInfoFlagPid:
				return process->pid;
				break;
				
			case sys_getInfoFlagThreads:
			{
				uint32_t threadCount = 0;
				sd_thread *thread = process->mainThread;
				
				while(thread)
				{
					threadCount ++;
					thread = thread->next;
				}
				
				return threadCount;
			}
				break;
				
			case sys_getInfoFlagMemory:
			{
				mm_heap *heap = process->heap;
				return (uint32_t)heap->allocatedBytes;
			}
				break;
				
			case sys_getInfoFlagNextPid:
			{
				sd_process *next = process->next;
				return (next) ? next->pid : SD_PID_INVALID;

			}
				break;
				
			default:
				break;
		}
	}
	
	if(flag == sys_getInfoFlagPid)
	{
		// Special case
		return (uint32_t)(sd_currentProcess) ? sd_currentProcess->pid : SD_PID_INVALID;
	}
	
	return 0;
}

uint32_t sd_setInfoExec(ir_cpuState *state, ir_cpuState **returnState)
{
	uint32_t flag = *((uint32_t *)(state->esp));
	uint32_t pid = *((uint32_t *)(state->esp + 4));

	sd_process *process = sd_processWithPid(pid);
	if(process)
	{
		bool result = false;
		
		switch(flag) 
		{
			case sys_setInfoFlagName:
			{
				char *string = *((char **)(state->esp + 8));
				result = sd_setName(string);
			}
				break;
				
			case sys_setInfoFlagIdentifier:
			{
				char *string = *((char **)(state->esp + 8));
				result = sd_setIdentifier(string);
			}
				break;
				
			default:
				break;
		}
		
		return !result; // 0 means success, 1 means failure
	}
	
	return -1;
}



// -------
void kernelTask();

int sd_init()
{
	cn_printf("Initializing scheduler...");

	sd_mutexInit(&__sd_stateMutex);
	if(sd_spawnProcessWithContext(kernelTask, vmm_getKernelContext()) == SD_PID_INVALID || !ir_installInterruptHandler(sd_schedule, 0x28, 0x28))
		return 0;
	
	uint32_t sSleep, sKill, rGetInfo, rSetInfo;
	
	sSleep		= sys_registerSyscall(sys_sleep, sd_sleepExec);
	sKill		= sys_registerSyscall(sys_kill, sd_killExec);
	rGetInfo	= sys_registerSyscall(sys_getInfo, sd_getInfoExec);
	rSetInfo	= sys_registerSyscall(sys_setInfo, sd_setInfoExec);
	
	if(sSleep == SYSCALL_INVALID || sKill == SYSCALL_INVALID)
		return 0;
	
	
	sd_currentProcess = sd_firstProcess;
	// The very first task is the kernel task, however, the sd_spawnTask() spins up an temporary ring 3 task. The kernel spins up the real kernel task by just calling the kernelTask()
	// function in void boot(). Once the scheduler does its first step, it assumes that it comes right out of the real kernel task (or just a few calls before the kernel spins it up)
	// the scheduler will overwrite the first tasks state with the ring 0 kernel task state.
	
	
	cmos_appendRTCFlags(CMOS_RTC_FLAG_PERIODICINT);	
	cn_printf("ok\n");
	
	return 1;
}
