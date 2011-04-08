//
//  syscall.h
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

#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include "stdint.h"
#include "interrupt.h"

#define sys_print	0
#define sys_sleep	1
#define sys_kill	2

#define SYSCALL_MEMBER 8
#define SYSCALL_INVALID UINT32_MAX
	
typedef union
{
	int64_t int64_type;
	int32_t int32_type;
	int16_t int16_type;
	int8_t  int8_type;
	
	float  float_type;
	double double_type;
	
	void *pointer_type;
} syscallMember;

typedef struct
{
	uint32_t pid;
	uint32_t type;
	
	syscallMember member[SYSCALL_MEMBER];
	int32_t retVal;
	
	ir_cpuState *cstate;
	ir_cpuState *state;
} syscallEvent;


typedef void (*sys_fillSyscall)(syscallEvent *event, va_list args);
typedef void (*sys_execSyscall)(syscallEvent *event);
	
extern uint32_t sys_registerSyscall(uint32_t type, sys_fillSyscall fill, sys_execSyscall exec);
	
extern int32_t syscall(uint32_t type, ...);
extern int sc_init();

#endif
