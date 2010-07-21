//
//  syscall.c
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

#include "syscall.h"

#include "taskd.h"
#include "time.h"

extern ir_cpu_state *td_fireRunloop(uint32_t intr, ir_cpu_state *state);

ir_cpu_state *_syscallEx(uint32_t intr, ir_cpu_state *state)
{
	syscall_type type = (syscall_type)state->eax;
	
	switch(type) 
	{
		case syscall_type_sleep:
			{
				return td_fireRunloop(intr, state);
			}
			break;

			
		case syscall_type_clock:
			{
				tm_pollTime();
			}
			break;
			
		default:
			{
				char reason[128];
				sprintf(reason, "Unknown syscall %i", type);
				
				k_panic(reason);
			}
			break;
	}
	
	return state;
}

void syscall(uint32_t type)
{
	__asm__ volatile("int $0x30" : : "a" (0));
}


void syscall_init()
{
	ir_addInterrupt(0x30, 0x30, _syscallEx);
}
