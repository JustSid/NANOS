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
#include <memory/memory.h>
#include "interrupt.h"

#include <scheduler/scheduler.h>

typedef struct sys_syscall
{
	uint32_t id;
	
	sys_fillSyscall fill;
	sys_execSyscall exec;
	
	struct sys_syscall *next;
} sys_syscall;


static syscallEvent sys_event;
static sys_syscall *sys_first   = NULL;
static sys_syscall *sys_current = NULL; // General purpose iteration variable

ir_cpuState *syscallEx(uint32_t interrupt, ir_cpuState *state)
{	
	#pragma unused(interrupt)
	
	if(sys_current)
	{
		sys_event.cstate = state;
		sys_current->exec(&sys_event);
		
		if(sys_event.state)
			return sys_event.state;
	}
	
	return state;
}


int32_t syscall(uint32_t type, ...)
{
	memset(&sys_event, 0, sizeof(syscallEvent));
	
	sys_current = sys_first;
	while(sys_current)
	{
		if(sys_current->id == type)
			break;
		
		sys_current = sys_current->next;
	}
	
	if(sys_current)
	{
		if(sys_current->fill)
		{
			va_list args;
			va_start(args, type);
			
			sys_current->fill(&sys_event, args);
			
			va_end(args);
		}
		
		sys_event.pid  = sd_getPid();
		sys_event.type = type;
		
		__asm__ volatile("int $0x30");
		return sys_event.retVal;
	}
	
	return -1;
}

uint32_t sys_registerSyscall(uint32_t type, sys_fillSyscall fill, sys_execSyscall exec)
{
	if(!exec)
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
	
	sys_syscall *tsyscall = mm_alloc(sizeof(sys_syscall));
	if(!tsyscall)
		return SYSCALL_INVALID;
	
	tsyscall->id	= type;
	tsyscall->next	= NULL;
	
	tsyscall->fill = fill;
	tsyscall->exec = exec;
	
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




ir_cpuState *mapSyscall(uint32_t interrupt, ir_cpuState *state)
{
	#pragma unused(interrupt)
	
	state->eax = (uint32_t)&syscall;
	return state;
}



// Default syscalls
void sys_printFill(syscallEvent *event, va_list args)
{
	event->member[0].pointer_type = va_arg(args, char *);
}

void sys_printExec(syscallEvent *event)
{
	char *string = event->member[0].pointer_type;
	if(string)
	{
		cn_puts(string);
		event->retVal = 0;
	}
	else
		event->retVal = -1;
}


// --

int sc_init()
{
	if(ir_installInterruptHandler(syscallEx, 0x30, 0x30) != 1 || ir_installInterruptHandler(mapSyscall, 0x31, 0x31) != 1)
		return 0;
	
	uint32_t rPrint;	
	rPrint = sys_registerSyscall(sys_print, sys_printFill, sys_printExec);
	
	if(rPrint == SYSCALL_INVALID)
		return 0;
	
	return 1;
}
