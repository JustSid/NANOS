//
//  process.c
//  libkernel
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

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "stdint.h"
#define PID_INVALID UINT32_MAX


typedef void (*threadEntry)();

extern uint32_t processCreate(char *name);
extern uint32_t threadCreate(threadEntry entry);
extern void sleep();
extern void cls();


extern void setName(char *name);
extern void setIdentifier(char *identifier);

extern uint32_t getPid();
extern uint32_t getNextPid(uint32_t pid);

extern char *getName(uint32_t pid);
extern char *getIdentifier(uint32_t pid);
extern uint32_t getThreadCount(uint32_t pid);
extern uint32_t getAllocatedMemory(uint32_t pid);

extern void hookKeyboard(void *characterBuffer, size_t bufferSize);
extern void unhookKeyboard();

#endif
