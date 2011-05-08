//
//  memory.c
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

#include "memory.h"

#include <system/console.h>
#include <system/debug.h>
#include <system/stdio.h>
#include <system/string.h>
#include <loader/loader.h>
#include <scheduler/scheduler.h>

static sd_mutex __mm_memoryMutex;
static mm_heap *kernelHeap;

mm_heap *mm_kernelHeap()
{
	return kernelHeap;
}

mm_heap *mm_createHeap(size_t size, vmm_context *context)
{
	sd_mutexLock(&__mm_memoryMutex);
	if(size < 4096)
		size = 4096;
	
	uint32_t pages = size / 4096;
	mm_heap *heap = pmm_allocPages(pages);
	
	if(heap)
	{
		memset(heap, 0, size); // Clean the memory area

		heap->size = size;
		heap->allocatedBytes = 0;
		heap->allocations = 1;
		
		
		uintptr_t allocationPtr = (uintptr_t)heap;
		allocationPtr += sizeof(mm_heap); // The first allocation begins directly behind the heap structure
		
		heap->firstAllocation = (mm_allocation *)allocationPtr;		
		
		
		mm_allocation *allocation = (mm_allocation *)allocationPtr;
		allocation->flags = MM_ALLOCATION_FLAG_FREE;
		allocation->begin = (void *)(allocationPtr + sizeof(mm_allocation));
		allocation->size  = heap->size - (sizeof(mm_allocation) + sizeof(mm_heap));
		allocation->next  = NULL; 
	}
	else
		db_logLevel(DB_LEVEL_WARNING, "Could not create heap (out of memory!)\n");
	
	sd_mutexUnlock(&__mm_memoryMutex);
	return heap;
}

void mm_destroyHeap(mm_heap *heap)
{
	uint32_t pages = heap->size / 4096;
	pmm_freePages(heap, pages);
}


void *mm_heapAlloc(mm_heap *heap, size_t size)
{
	if(!heap)
		return NULL;
	
	size_t appendBytes = 5; // TODO: Add a hint flag that allows different sizes!
	
	mm_allocation *allocation = heap->firstAllocation;
	while(allocation)
	{
		// Look for a free memory chunk large enough to hold the allocation
		if((allocation->flags & MM_ALLOCATION_FLAG_FREE) && allocation->size >= size + appendBytes)
		{
			if(allocation->size + appendBytes >= size + 12 + sizeof(mm_allocation)) // Check if there is enough place for another allocation
			{
				size_t allocationSize = size + appendBytes;
				size_t appendAllocationSize = allocation->size - (allocationSize + sizeof(mm_allocation));
				
				uintptr_t appendAllocationPtr = (uintptr_t)allocation->begin;
				appendAllocationPtr += size + appendBytes;
				
				mm_allocation *appendAllocation = (mm_allocation *)appendAllocationPtr;
				appendAllocation->flags = MM_ALLOCATION_FLAG_FREE;
				appendAllocation->begin = (void *)(appendAllocationPtr + sizeof(mm_allocation));
				appendAllocation->size  = appendAllocationSize;
				appendAllocation->next  = allocation->next;
				
				
				allocation->size = allocationSize;
				allocation->next = appendAllocation;
			}
			
			
			allocation->flags &= ~MM_ALLOCATION_FLAG_FREE;
			allocation->flags |= MM_ALLOCATION_FLAG_USED;
			
			heap->allocations ++;
			heap->allocatedBytes += allocation->size;

			return allocation->begin;
		}
		
		allocation = allocation->next;
	}
	
	return NULL;
}

void *mm_heapRealloc(mm_heap *heap, void *ptr, size_t newSize)
{
	mm_allocation *allocation = heap->firstAllocation;
	while(allocation)
	{
		if(allocation->begin == ptr)
		{
			if(newSize == allocation->size || newSize < allocation->size)
				return ptr;
			
			if(newSize > allocation->size)
			{
				void *newPtr = mm_heapAlloc(heap, newSize);
				memcpy(newPtr, ptr, allocation->size);
				
				mm_heapFree(heap, ptr);
				return newPtr;
			}
		}
		
		allocation = allocation->next;
	}
	
	return NULL;
}

void mm_heapFree(mm_heap *heap, void *ptr)
{
	if(!heap)
		return;
	
	mm_allocation *allocation = heap->firstAllocation;
	while(allocation)
	{
		if(allocation->begin == ptr)
		{
			allocation->flags &= ~MM_ALLOCATION_FLAG_USED;
			allocation->flags |= MM_ALLOCATION_FLAG_FREE;
			
			heap->allocations --;
			heap->allocatedBytes -= allocation->size;
			
			return;
		}
		
		allocation = allocation->next;
	}
}





void *mm_alloc(size_t size)
{
	sd_process *process = sd_getCurrentProcess();
	return mm_heapAlloc((process) ? process->heap : kernelHeap, size);
}

void *mm_realloc(void *ptr, size_t size)
{
	sd_process *process = sd_getCurrentProcess();
	return mm_heapRealloc((process) ? process->heap : kernelHeap, ptr, size);
}

void mm_free(void *ptr)
{
	sd_process *process = sd_getCurrentProcess();
	mm_heapFree((process) ? process->heap : kernelHeap, ptr);
}




extern void pmm_markPages(void *page, bool used, uint32_t pageCount); // HACK!
int mm_init(struct multiboot_info *bootinfo)
{
	cn_puts("Initializing memory manager...");
	
	if(!pmm_init(bootinfo) || !vmm_init())
		return 0;
	
	ld_markImages(bootinfo); // Placed here to avoid that the kernelHeap overwrites modules memory
	// REMOVE once virtual memory is enabled and the issue is solved
	
	
	sd_mutexInit(&__mm_memoryMutex);
	kernelHeap = mm_createHeap(10*(32*4096), vmm_getKernelContext()); // Create a 10 mB heap	
	
	if(!kernelHeap)
		return 0;
	
	cn_puts("ok\n");
	return 1;
}
