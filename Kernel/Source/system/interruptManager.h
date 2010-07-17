//
//  interruptManager.h
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


#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "console.h"
#include "stdint.h"
#include "kernel.h"
#include "malloc.h"
#include "string.h"

#include "gdt.h"
#include "idt.h"

typedef struct ir_cpu_state
{
    uint32_t   eax;
    uint32_t   ebx;
    uint32_t   ecx;
    uint32_t   edx;
    uint32_t   esi;
    uint32_t   edi;
    uint32_t   ebp;
	
    uint32_t   intr;
    uint32_t   error;
	
    uint32_t   eip;
    uint32_t   cs;
    uint32_t   eflags;
    uint32_t   esp;
    uint32_t   ss;
} ir_cpu_state;

typedef struct ir_interrupt_entry
{
	uint32_t intr_start;
	uint32_t intr_end;
	
	ir_cpu_state *(*callback)(uint32_t intr, ir_cpu_state *state); 
	
	struct ir_interrupt_entry *next;
} ir_interrupt_entry;

extern void ir_init();

extern ir_interrupt_entry *ir_addInterrupt(uint32_t start, uint32_t end, ir_cpu_state *(*callback)(uint32_t intr, ir_cpu_state *state));

extern ir_cpu_state *ir_handleInterrupt(ir_cpu_state *state);
extern ir_cpu_state *ir_lastState();

#endif
