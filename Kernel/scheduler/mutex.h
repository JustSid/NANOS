//
//  muthex.h
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

#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <system/stdint.h>

/**
 @file mutex.h
 @brief Defines a mutex object and the appropriate functions to work with it.
 *
 * Defines a mutex object which is tightly bound to the scheduler as well as functions needed for locking and unlocking a mutex.
 **/

struct sd_process;
struct sd_thread;

/**
 @brief Mutex object
 *
 * A mutex object contains of a state variable (locked) which can be either true or false (representing locked and unlocked), and also of a process and a thread.
 * The thread and process are used to make sure that no other thread but the one who locked the mutex can unlock it.
 @remark Never ever set a value directly!
 **/
typedef struct sd_mutex
{
	/**
	 * Value used to determine the mutexs locked state
	 @remark MUST be written atomically.
	 **/
	uint8_t locked;
	
	/**
	 * The process used to lock the mutex, or NULL
	 **/
	struct sd_process	*currentProcess;
	/**
	 * The thread used to lock the mutex, or NULL
	 **/
	struct sd_thread	*currentThread;
} sd_mutex;

/**
 * Initializes a mutex.
 @remark Always call this function when creating a new mutex, unless you created the mutex via a calloc like allocation and can assure that everything is NULL!
 @param mutex The mutex that you want to initialze
 **/
extern void sd_mutexInit(sd_mutex *mutex);

/**
 * Locks the given mutex
 @remark If the mutex is already locked, the thread that tries to lock the mutex will resign all its granted CPU cycles and check again if the mutex is locked after the scheduler granted it more cycles.
 @remark Trying to lock a mutex a second time before unlocking it will result in a deadlock.
 @param mutex The mutex you want to lock.
 **/
extern void sd_mutexLock(sd_mutex *mutex);

/**
 * Unlocks the given mutex.
 @remark Only the thread that locked the mutex can unlock it!
 @param mutex The mutex you want to unlock.
 **/
extern void sd_mutexUnlock(sd_mutex *mutex);

#endif
