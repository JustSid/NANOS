//
//  memory.h
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

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <boot/multiboot.h>

#include <system/stdint.h>
#include <system/stdbool.h>

#include "pmemory.h"
#include "vmemory.h"

struct mm_heap;

#define MM_ALLOCATION_FLAG_FREE 1
#define MM_ALLOCATION_FLAG_USED 2

typedef struct mm_allocation
{
	void *begin;
	size_t size;
	uint8_t flags;
	
	struct mm_allocation *next;
} mm_allocation;

typedef struct
{
	size_t size;
	size_t allocatedBytes;
	size_t allocations;
	
	vmm_context *contexts[2]; // Two initial contexts, kernel and process
	uint8_t contextCount;
	
	mm_allocation *firstAllocation;
} mm_heap;


extern int mm_init(struct multiboot_info *bootinfo);


extern mm_heap *mm_createHeap(size_t size, vmm_context *context);
extern mm_heap *mm_kernelHeap();
extern void mm_destroyHeap(mm_heap *heap);

extern void *mm_heapAlloc(mm_heap *heap, size_t size);
extern void *mm_heapRealloc(mm_heap *heap, void *ptr, size_t newSize);
extern void mm_heapFree(mm_heap *heap, void *ptr);


extern void *mm_alloc(size_t size);
extern void *mm_realloc(void *ptr, size_t size);
extern void mm_free(void *ptr);

#endif
