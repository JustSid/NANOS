//
//  syscall.c
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


#include "syscall.h"
#include "string.h"
#include "console.h"
#include "interrupt.h"

#include <memory/memory.h>
#include <loader/loader.h>
#include <scheduler/scheduler.h>

typedef struct sys_syscall
{
	uint32_t id;
	sys_execSyscall callback;
	
	struct sys_syscall *next;
} sys_syscall;


static sys_syscall *sys_first   = NULL;
static sys_syscall *sys_current = NULL; // General purpose iteration variable

uint32_t sys_registerSyscall(uint32_t type, sys_execSyscall callback)
{
	if(!callback)
		return SYSCALL_INVALID;
	
	if(type == SYSCALL_INVALID)
	{
		// TODO: Get a valid type id
	}
	
	sys_current = sys_first;
	while(sys_current)
	{
		if(sys_current->id == type)
		{
			// TODO: Get a valid type id
			break;
		}
		
		sys_current = sys_current->next;
	}
	
	
	sys_current = NULL;
	if(type == SYSCALL_INVALID)
		return SYSCALL_INVALID;
	
	sys_syscall *tsyscall = mm_heapAlloc(mm_kernelHeap(), sizeof(sys_syscall));
	if(!tsyscall)
		return SYSCALL_INVALID;
	
	tsyscall->id		= type;
	tsyscall->next		= NULL;
	tsyscall->callback	= callback;
	
	
	if(sys_first)
	{
		sys_current = sys_first;
		while(sys_current)
		{
			if(!sys_current->next)
			{
				sys_current->next = tsyscall;
				break;
			}
			
			sys_current = sys_current->next;
		}
	}
	else
	{
		sys_first = tsyscall;
	}
	
	return type;
}



ir_cpuState *syscallEx(ir_cpuState *state)
{
	uint32_t retVal = 0;
	uint32_t call = state->eax;
	
	ir_cpuState *returnState = state;
	
	sys_current = sys_first;
	while(sys_current)
	{
		if(sys_current->id == call)
		{
			retVal = sys_current->callback(state, &returnState);
			break;
		}
		
		sys_current = sys_current->next;
	}
	
	state->eax = retVal;
	return returnState;
}




uint32_t syscall0(uint32_t type)
{
	uint32_t retVal;
	
	__asm__ volatile("mov %1, %%eax;"
					 "int $0x30;" : "=a" (retVal) : "r" (type));
	
	return retVal;
}

uint32_t syscall1(uint32_t type, uint32_t arg1)
{
	uint32_t retVal = 0;
	
	__asm__ volatile("pushl %2;"
					 "movl %1, %%eax;"
					 "int $0x30;"
					 "add $0x4, %%esp;" : "=a" (retVal) : "r" (type), "r" (arg1));
	
	return retVal;
}

uint32_t syscall2(uint32_t type, uint32_t arg1, uint32_t arg2)
{
	uint32_t retVal = 0;
	
	__asm__ volatile("pushl %3;"
					 "pushl %2;"
					 "movl %1, %%eax;"
					 "int $0x30;"
					 "add $0x8, %%esp;" : "=a" (retVal) : "r" (type), "r" (arg1), "r" (arg2));
	
	return retVal;
}

uint32_t syscall3(uint32_t type, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
	uint32_t retVal = 0;
	
	__asm__ volatile("pushl %4;"
					 "pushl %3;"
					 "pushl %2;"
					 "movl %1, %%eax;"
					 "int $0x30;"
					 "add $0xC, %%esp;" : "=a" (retVal) : "r" (type), "r" (arg1), "r" (arg2), "r" (arg3));
	
	return retVal;
}


uint32_t sys_printExec(ir_cpuState *state, ir_cpuState **returnState)
{
	char *string = *(char **)(state->esp);
	cn_puts(string);
	
	return 0;
}

uint32_t sys_allocExec(ir_cpuState *state, ir_cpuState **returnState)
{
	sd_process *process = sd_getCurrentProcess();
	size_t size = *((size_t *)(state->esp));
	
	void *result = mm_heapAlloc(process->heap, size);
	return (uint32_t)result;
}

uint32_t sys_reallocExec(ir_cpuState *state, ir_cpuState **returnState)
{
	sd_process *process = sd_getCurrentProcess();
	
	void	*ptr = *(void **)(state->esp);
	size_t	size = *((size_t *)(state->esp + 4));
	
	
	void *result = mm_heapRealloc(process->heap, ptr, size);
	return (uint32_t)result;
}

uint32_t sys_freeExec(ir_cpuState *state, ir_cpuState **returnState)
{
	sd_process *process = sd_getCurrentProcess();
	void *ptr = *(void **)(state->esp);
	
	mm_heapFree(process->heap, ptr);
	return 0;
}

uint32_t sys_execExec(ir_cpuState *state, ir_cpuState **returnState)
{
	sd_pentry entry = *(sd_pentry *)(state->esp);
	sd_spawnProcess(entry);
	
	return 0;
}

uint32_t sys_threadExec(ir_cpuState *state, ir_cpuState **returnState)
{
	sd_tentry entry = *(sd_tentry *)(state->esp);
	return sd_attachThread(entry, sd_threadPriorityNormal);
}

uint32_t sys_processExec(ir_cpuState *state, ir_cpuState **returnState)
{
	char *name = *(char **)(state->esp);
	return ld_launchImage(name);
}

uint32_t sys_clsExec(ir_cpuState *state, ir_cpuState **returnState)
{
	cn_cls();
	return 0;
}



int sc_init()
{
	if(ir_installInterruptHandler(syscallEx, 0x30, 0x30) != 1)
		return 0;
	
	uint32_t rPrint, rAlloc, rRealloc, rFree, rExec, rThread, rProcess, rCls;	
	
	rPrint		= sys_registerSyscall(sys_print, sys_printExec);
	rAlloc		= sys_registerSyscall(sys_alloc, sys_allocExec);
	rRealloc	= sys_registerSyscall(sys_realloc, sys_reallocExec);
	rFree		= sys_registerSyscall(sys_free, sys_freeExec);
	rExec		= sys_registerSyscall(sys_exec, sys_execExec);
	rThread		= sys_registerSyscall(sys_thread, sys_threadExec);
	rProcess	= sys_registerSyscall(sys_process, sys_processExec);
	rCls		= sys_registerSyscall(sys_cls, sys_clsExec);
	
	if(rPrint == SYSCALL_INVALID || rAlloc == SYSCALL_INVALID || rRealloc == SYSCALL_INVALID || 
	   rFree == SYSCALL_INVALID || rFree == SYSCALL_INVALID || rExec == SYSCALL_INVALID || 
	   rThread == SYSCALL_INVALID || rProcess == SYSCALL_INVALID || rCls == SYSCALL_INVALID)
		return 0;
	
	return 1;
}
