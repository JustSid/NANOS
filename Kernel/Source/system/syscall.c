//
//  syscall.c
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


#include "syscall.h"
#include "memory.h"
#include "panic.h"
#include "stdint.h"
#include "interrupt.h"
#include "console.h"

extern ir_cpuState *td_schedule(uint32_t intr, ir_cpuState *state);
extern ir_cpuState *td_kill(uint32_t pid);

ir_cpuState *syscallEx(uint32_t interrupt, ir_cpuState *state)
{	
	syscall_types type = state->eax;
	switch(type)
	{
		case sys_sleep:
		{
			
		}
			
		case sys_exit:
		{
			
		}
					
		default:
			break;
	}
	
	return state;
}


int64_t syscall(syscall_types type, ...)
{
	asm volatile ("int $0x30" : : "a" (type));
	return 0;
}


int sc_init()
{
	cn_printf("Initializing system...");
	
	if(ir_installInterruptHandler(syscallEx, 0x30, 0x30) != 1)
		return 0;
	
	cn_printf("ok\n");
	return 1;
}
