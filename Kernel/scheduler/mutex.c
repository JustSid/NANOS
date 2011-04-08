//
//  mutex.c
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

#include "mutex.h"
#include "process.h"
#include "thread.h"

#include "scheduler.h"

#include <system/syscall.h>

void sd_mutexInit(sd_mutex *mutex)
{
	mutex->locked = false;
	
	mutex->currentThread  = NULL;
	mutex->currentProcess = NULL;
}



void sd_mutexLock(sd_mutex *mutex)
{
	// Checks the mutexes state and sets a new one atomically!
	__asm__ volatile("movb $1, %%cl;"
					 "jmp tryLock;"
					 "waitLock: pushl 4(%%ebp);"
					 "pushl $1;"
					 //"call _syscall;" // Syscall 1 is the syscall which resigns a threads resources!
					 "addl $4, %%esp;"
					 "tryLock: xorb %%al, %%al;"
					 "lock cmpxchgb %%cl, (%0);"
					 "jnz waitLock;" : : "D" (&mutex->locked) : "eax", "ecx");

	mutex->currentProcess = sd_getCurrentProcess();
	mutex->currentThread  = sd_getCurrentThread();
}

void sd_mutexUnlock(sd_mutex *mutex)
{
	// Check if the mutex has been locked and if so, if it was locked by the current thread/process pair
	if((mutex->locked) && (mutex->currentProcess->identifier == sd_getCurrentProcess()->identifier) && (mutex->currentThread->id == sd_getCurrentThread()->id))
	{
		mutex->currentProcess = NULL;
		mutex->currentThread  = NULL;
		
		mutex->locked = false; // Mark the mutex as unlocked...
	}
}
