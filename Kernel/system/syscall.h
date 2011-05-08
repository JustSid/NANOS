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

#define sys_print		0
#define sys_sleep		1 // Scheduler.c
#define sys_kill		2 // Scheduler.c
#define sys_alloc		3
#define sys_realloc		4
#define sys_free		5
#define sys_exec		7
#define sys_thread		8 
#define sys_process		9
#define sys_getInfo		10 // Scheduler.c
#define sys_setInfo		11 // Scheduler.c
#define sys_keyboard	12 // keymap_def.c
#define sys_cls			13


// sys_getInfo flags:
#define sys_getInfoFlagName			0
#define sys_getInfoFlagIdentifier	1
#define sys_getInfoFlagPid			2
#define sys_getInfoFlagThreads		3
#define sys_getInfoFlagMemory		4
#define sys_getInfoFlagNextPid		5

// sys_setInfo flags:
#define sys_setInfoFlagName			0
#define sys_setInfoFlagIdentifier	1


#define SYSCALL_INVALID UINT32_MAX

typedef uint32_t (*sys_execSyscall)(ir_cpuState *state, ir_cpuState **returnState);
extern uint32_t sys_registerSyscall(uint32_t type, sys_execSyscall callback);
	

extern uint32_t syscall0(uint32_t type);
extern uint32_t syscall1(uint32_t type, uint32_t arg1);
extern uint32_t syscall2(uint32_t type, uint32_t arg1, uint32_t arg2);
extern uint32_t syscall3(uint32_t type, uint32_t arg1, uint32_t arg2, uint32_t arg3);

extern int sc_init();

#endif
