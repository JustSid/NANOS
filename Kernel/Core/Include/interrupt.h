//
//  interrupt.h
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

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "stdint.h"

typedef struct 
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
} ir_cpuState;

#ifdef __cplusplus
extern "C" {
#endif

typedef ir_cpuState *(*ir_interruptHandle)(uint32_t interrupt, ir_cpuState *state);

extern void ir_enableInterrupts();
extern void ir_disableInterrupts();

extern int ir_installInterruptHandler(ir_interruptHandle interruptHandle, uint32_t handleBegin, uint32_t handleEnd);
extern ir_cpuState *ir_handleInterrupt(ir_cpuState *state);

extern int ir_init();
	
#ifdef __cplusplus
}
#endif

#endif
