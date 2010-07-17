//
//  interruptManager.c
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

#include "interruptManager.h"

static ir_cpu_state *_ir_lastState = NULL; // Last known CPU state. Gets refreshed if an interrupt is fired

static ir_interrupt_entry *first_entry	= NULL; 
static ir_interrupt_entry *last_entry	= NULL;

ir_interrupt_entry *ir_addInterrupt(uint32_t start, uint32_t end, ir_cpu_state *(*callback)(uint32_t intr, ir_cpu_state *state))
{
	if(!callback)
		return NULL;
	
	if(start > end)
	{
		uint32_t temp = end;
		
		end = start;
		start = temp;
	}
	
	if(!first_entry)
	{
		last_entry = (ir_interrupt_entry *)malloc(sizeof(ir_interrupt_entry));
		first_entry = last_entry;
	}
	else 
	{
		last_entry->next = (ir_interrupt_entry *)malloc(sizeof(ir_interrupt_entry));
		last_entry = last_entry->next;
	}	
	
	last_entry->intr_start = start;
	last_entry->intr_end = end;
	last_entry->callback = callback;
	last_entry->next = NULL;
	
	return last_entry;
}


ir_cpu_state *ir_handleInterrupt(ir_cpu_state *state)
{
	ir_cpu_state *nextState = state;
	_ir_lastState			= state;

	if(first_entry)
	{		
		ir_interrupt_entry *entry = first_entry;
		while(entry)
		{
			if(state->intr >= entry->intr_start && state->intr <= entry->intr_end)
			{
				nextState = entry->callback(state->intr, state);
				
				if(nextState && nextState != state)
				{
					return nextState;
				}
			}
			
			entry = entry->next;
		}
		
		if(nextState)
			return nextState;
	}
	
	char message[255];
	sprintf(message, "Unhandled interrupt %x", (unsigned int)state->intr);
	k_panic(message);
	
	return nextState; // We will never reach this point, however, to avoid compiler warnings, we have this return here
}

ir_cpu_state *ir_lastState()
{
	return _ir_lastState;
}


ir_cpu_state *ir_defaultHandle(uint32_t intr, ir_cpu_state *state)
{
	if(intr >= 0x28)
		outb(0xa0, 0x20);
	
	outb(0x20, 0x20);
	
	return state; 
}

void ir_init()
{
	cn_puts("Initializing interrupt manager...");
	
	ir_addInterrupt(0x20, 0x2f, ir_defaultHandle);
	
	gdt_defaults(true);
	idt_defaults(true);
	
	cn_puts("[ok]\n");
}
