//
//  memory.h
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

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "multiboot.h"
#include "stdint.h"

#define MM_FRAGMENT_FREE	1
#define MM_FRAGMENT_USED	2
#define MM_FRAGMENT_WIRED	4

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initializes the physical memory manager using the informations from the multiboot info.
 @return The number of free memory chunks. An value of zero indicates an error.
 **/
extern size_t mm_init(struct multiboot_info *bootinfo);

/**
 * Prints the current memory fragments into the console
 **/
extern void mm_dumpMemory();

/**
 * Allocates a new chunk of memory with the given size and returns a pointer to it
 **/
extern void *mm_alloc(size_t size);
	
/**
 * Marks the memory as free, the memory must be allocated prior to its freeing.
 **/
extern void mm_free(void *ptr);

/**
 * Defragments the heap. Gets automatically called when a task exits
 **/
extern void mm_defrag();
	
/**
 * Clears all memory allocated by the task with the given id.
 @remark The task must already been terminated before you can call this function
 **/
extern void mm_clear(uint32_t pid);

#ifdef __cplusplus
}
#endif
	
#endif
