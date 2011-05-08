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


#include "process.h"
#include "syscall.h"

uint32_t processCreate(char *name)
{
	return syscall1(sys_process, (uint32_t)name);
}

uint32_t threadCreate(threadEntry entry)
{
	return syscall1(sys_thread, (uint32_t)entry);
}

void sleep()
{
	syscall0(sys_sleep);
}

void cls()
{
	syscall0(sys_cls);
}



void setName(char *name)
{
	if(!name)
		return;
	
	syscall3(sys_setInfo, sys_setInfoFlagName, getPid(), (uint32_t)name);
}

void setIdentifier(char *identifier)
{
	if(!identifier)
		return;
	
	syscall3(sys_setInfo, sys_setInfoFlagIdentifier, getPid(), (uint32_t)identifier);
}


uint32_t getPid()
{
	static uint32_t cachedPid = PID_INVALID; // Cache the pid as its often used and usually doesn't change
	if(cachedPid == PID_INVALID)
	{
		cachedPid = syscall2(sys_getInfo, sys_getInfoFlagPid, PID_INVALID);
	}
	
	return cachedPid;
}

uint32_t getNextPid(uint32_t pid)
{
	return syscall2(sys_getInfo, sys_getInfoFlagNextPid, pid);
}

char *getName(uint32_t pid)
{
	return (char *)syscall2(sys_getInfo, sys_getInfoFlagName, pid);
}

char *getIdentifier(uint32_t pid)
{
	return (char *)syscall2(sys_getInfo, sys_getInfoFlagIdentifier, pid);
}

uint32_t getThreadCount(uint32_t pid)
{
	return syscall2(sys_getInfo, sys_getInfoFlagThreads, pid);
}

uint32_t getAllocatedMemory(uint32_t pid)
{
	return syscall2(sys_getInfo, sys_getInfoFlagMemory, pid);
}

void hookKeyboard(void *characterBuffer, size_t bufferSize)
{
	if(characterBuffer && bufferSize > 0)
	{
		syscall3(sys_keyboard, 0, (uint32_t)characterBuffer, (uint32_t)bufferSize);
	}
}

void unhookKeyboard()
{
	syscall1(sys_keyboard, 1);
}
