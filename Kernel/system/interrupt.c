//
//  interrupt.c
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

#include "interrupt.h"

#include <memory/memory.h>

#include "port.h"
#include "gdt.h"
#include "idt.h"

#include "stdio.h"
#include "string.h"

#include "debug.h"
#include "console.h"
#include "panic.h"

#include <scheduler/scheduler.h>
#include <scheduler/mutex.h>

static bool ir_interruptsEnabled = true;
static sd_mutex __ir_stateMutex;

extern ir_cpuState *sd_violentKill();

void ir_enableInterrupts()
{
	sd_mutexLock(&__ir_stateMutex);
	ir_interruptsEnabled = true;
	
	__asm__ volatile ("sti");
	outb(0x70, inb(0x70) & 0x7F);
	sd_mutexUnlock(&__ir_stateMutex);
}

void ir_disableInterrupts()
{
	sd_mutexLock(&__ir_stateMutex);
	ir_interruptsEnabled = false;
	
	__asm__ volatile ("cli");
	outb(0x70, inb(0x70) | 0x80);
	sd_mutexUnlock(&__ir_stateMutex);
}

typedef struct ir_interruptHandler
{
	// The "listening" area
	uint32_t begin;
	uint32_t end;
	
	// Callback
	ir_interruptHandle	interruptHandle;
	
	struct ir_interruptHandler *next;
} ir_interruptHandler;




static ir_interruptHandler	*firstHandler	= NULL;
static ir_cpuState			*lastState		= NULL;

int ir_installInterruptHandler(ir_interruptHandle interruptHandle, uint32_t handleBegin, uint32_t handleEnd)
{
	if(!interruptHandle)
		return -1;
	
	ir_interruptHandler *handler = mm_alloc(sizeof(ir_interruptHandler));
	if(handler)
	{
		handler->begin	= handleBegin;
		handler->end	= handleEnd;

		handler->interruptHandle	= interruptHandle;
		handler->next = NULL;
		
		if(!firstHandler)
		{
			firstHandler = handler;
		}
		else 
		{
			ir_interruptHandler *_handler = firstHandler;
			while(_handler) 
			{
				if(!_handler->next)
				{
					_handler->next = handler;
					return 1;
				}
				
				_handler = _handler->next;
			}
		}
		
		return 1;
	}
	
	return 0;
}


ir_cpuState *ir_handleInterrupt(ir_cpuState *state)
{
	ir_interruptHandler *handler		= firstHandler;
	ir_cpuState			*returnState	= NULL;
		
	lastState = state;
	
	while(handler)
	{
		if(state->intr >= handler->begin && state->intr <= handler->end)
			returnState = handler->interruptHandle(state);
		
		handler = handler->next;
	}
	
	if(returnState)
	{
		return returnState;
	}
	
	
	
	char panicReason[256]; // Oh array, y u so large?
	sprintf(panicReason, "Unhandled interrupt %i occured!", (unsigned int)state->intr);
	panic(panicReason);

	return NULL;
}


ir_cpuState *ir_defaultHandler(ir_cpuState *state)
{
	uint32_t interrupt = state->intr;
	
	if(interrupt >= 0x28)
		outb(0xa0, 0x20);
	
	outb(0x20, 0x20);
	return state;
}

ir_cpuState *ir_exceptionHandler(ir_cpuState *state)
{	
	uint32_t exception = state->intr;
	
	switch(exception)
	{
		case 0:
			cn_printf("Division by zero in task %i", sd_getPid());
			break;

		case 1:
			cn_printf("Debug exception in task %i", sd_getPid());
			break;
			
		case 2:
			cn_printf("Non Maskable Interrupt in task %i", sd_getPid());
			break;
			
		case 4:
			cn_printf("Overflow in task %i.", sd_getPid());
			break;
			
		case 5:
			cn_printf("Bound range exceeded in task %i.", sd_getPid());
			break;
			
		case 6:
		{
			cn_printf("Invalid Opcode in task %i.", sd_getPid());
			
			return sd_violentKill();
		}
			break;
			
		case 13:
		{
			int pid = sd_getPid();
			cn_printf("General Protection Fault occurred in %i (%s).\n", pid, sd_processWithPid(pid)->name);
			
			return sd_violentKill();
		}
			break;
			
		case 15:
		{
			int pid = sd_getPid();
			cn_printf("Segmentation Fault occurred in %i (%s).\n", pid, sd_processWithPid(pid)->name);
			
			return sd_violentKill();
		}
			break;
			
		default:
			panic("Unhandled exception %i occured in %i!", (unsigned int)exception, sd_getPid());
			break;
	}
	
	
	cn_putc('\n');
	return state;
}

ir_cpuState *ir_lastCPUState()
{
	return lastState;
}



int ir_init()
{
	cn_puts("Initializing interrupt handler...");
	
	sd_mutexInit(&__ir_stateMutex);	
	if(ir_installInterruptHandler(ir_defaultHandler, 0x20, 0x2f) != 1)
		return 0;
	
	if(ir_installInterruptHandler(ir_exceptionHandler, 0x0, 0x12) != 1)
		return 0;
	
	gdt_defaults(true);
	idt_defaults(true);
	
	if(!ir_interruptsEnabled)
		ir_disableInterrupts();
	
	cn_puts("ok\n");
	return 1;
}
