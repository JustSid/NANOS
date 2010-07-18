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
#include "console.h"
#include "kernel.h"
#include "malloc.h"
#include "string.h"

#include "gdt.h"
#include "idt.h"

static ir_cpu_state *_ir_lastState = NULL; // Last known CPU state. Gets refreshed if an interrupt is fired

static ir_interrupt_entry *first_entry	= NULL; // The list of interrupt listener, this is the first element
static ir_interrupt_entry *last_entry	= NULL; // The last element of the list. Every new listener will be attached to this element and than this entry becomes the new element

// Adds a listener for a specific interrupt or a range of interrupts. start and end determine the range of interrupts (eg. 0x20 to 0x2f) and callback is a C function that gets called
// if a interrupt matches the specified range.
// Example for an callback:
/*
ir_cpu_state *some_callback(uint32_t interrupt, ir_cpu_state *state)
{
	// interrupt contains the number of the interrupt (the same as state->intr)
	// state contains the state of the CPU by the time the interrupt occured.
 
	// You need to return a new CPU state (eg. a stored one), or you can return the current CPU state if you don't need to switch the CPU state
	// If you return NULL and no other interrupt listener "catches" the interrupt, the kernel will panic! However, you should return NULL if your function failed.
	//
	// Keep in mind that there is a responder chain, so there can be multiple listeners for the same interrupt. If you return a new CPU state, no other callbacks will be fired,
	// but your CPU state will be the new state. If you return NULL or the current state, the other callbacks will be also fired, until there is a new CPU state or no other listener that can
	// handle the interrupt!
}
 */
ir_interrupt_entry *ir_addInterrupt(uint32_t start, uint32_t end, ir_cpu_state *(*callback)(uint32_t intr, ir_cpu_state *state))
{
	if(!callback)
		return NULL; // You need a callback
	
	if(start > end) // If start is greate than end, both will be switched. Start must be always <= end!
	{
		uint32_t temp = end;
		
		end = start;
		start = temp;
	}
	
	if(!first_entry) 
	{
		// We have no entrys yet, so we create new one. This is the first and last element, as it is the only one
		last_entry = (ir_interrupt_entry *)malloc(sizeof(ir_interrupt_entry));
		first_entry = last_entry;
	}
	else 
	{
		last_entry->next = (ir_interrupt_entry *)malloc(sizeof(ir_interrupt_entry)); // We attach a new element to the last element
		last_entry = last_entry->next;  // And then set the last element to the new element. So last_entry always points to the last element
	}	
	
	// Set the interrupt range and the callback
	last_entry->intr_start = start;
	last_entry->intr_end = end;
	last_entry->callback = callback;
	last_entry->next = NULL; // No next element. This is the last entry!
	
	return last_entry;
}

void ir_removeInterrupt(ir_interrupt_entry *entry)
{
	ir_interrupt_entry *temp = first_entry;
	while(temp && temp->next != entry) // Search for the entry that the user wants to delete
	{
		temp = temp->next;
	}
	
	if(temp)
	{
		temp->next = entry->next; // Same as temp->next = temp->next->next, as temp->next is entry
		free(entry);
	}
}


// A interrupt occured, we need to handle it. If we can't handle the interrupt, the kernel panics.
ir_cpu_state *ir_handleInterrupt(ir_cpu_state *state)
{
	ir_cpu_state *nextState = state;
	_ir_lastState			= state; // We set the last known state. If the kernel panics, it will print this state on the screen.

	if(first_entry)
	{		
		ir_interrupt_entry *entry = first_entry;
		while(entry) // Iterate through all known listener
		{
			if(state->intr >= entry->intr_start && state->intr <= entry->intr_end) 
			{
				// The range of the listener matches with the fired interrupt
				nextState = entry->callback(state->intr, state); // we call the callback and ask for a new CPU state
				
				if(nextState && nextState != state)
				{
					// The callback returned a new CPU state, so we will return this and make this to the new CPU state
					return nextState;
				}
			}
			
			// The interrupt couldn't be handled yet (or the listener returned the old CPU state), so we pick up the next element in the list
			entry = entry->next;
		}
		
		if(nextState)
			return nextState;
	}
	
	// We couldn't handle the interrupt. Everything we can do now is to shut down the kernel
	char message[255];
	sprintf(message, "Unhandled interrupt %x", (unsigned int)state->intr);
	k_panic(message); // This will deadlock the CPU
	
	return nextState; // We will never reach this point, however, to avoid compiler warnings, we have this return here
}

ir_cpu_state *ir_lastState()
{
	return _ir_lastState;
}


// The default handle. This will handle all interrupts from 0x20 to 0x2f
ir_cpu_state *ir_defaultHandle(uint32_t intr, ir_cpu_state *state)
{
	if(intr >= 0x28)
		outb(0xa0, 0x20);
	
	outb(0x20, 0x20);
	return state; // We have no new CPU state, so we return just the default state
}


void ir_init()
{
	cn_puts("Initializing interrupt manager...");
	
	ir_addInterrupt(0x20, 0x2f, ir_defaultHandle); // Setup the default interrupt handle before we receive any interrupt.
	 
	gdt_defaults(true); // Setups the global descriptor table and reloads it (this will also setup the TSS)
	idt_defaults(true); // Setups the interrupt descriptor table and reloads it
	
	cn_puts("[ok]\n"); // We are done :)
}
