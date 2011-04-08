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
#include <scheduler/scheduler.h>

static sd_mutex __mm_memoryMutex;

void *mm_allocContext(size_t size, vmm_context *context)
{
	if(!context || size == 0)
		return NULL;
	
	sd_mutexLock(&__mm_memoryMutex);
	
	void *ptr = pmm_alloc();
	if(ptr)
	{
		// Get a free page
		uintptr_t page = vmm_getFreePage(context);
		if(page)
		{
			// Map the memory
			bool result = vmm_mapPage(context, page, (uintptr_t)ptr, true);
			if(!result)
			{
				pmm_free(ptr);
				page = 0x0;
			}
			
			
			sd_mutexUnlock(&__mm_memoryMutex);
			return (void *)page;
		}
		
		pmm_free(ptr);
	}
	
	sd_mutexUnlock(&__mm_memoryMutex);
	return NULL;
}



void *mm_alloc(size_t size)
{
	sd_process *process = sd_getCurrentProcess();
	return mm_allocContext(size, (process) ? process->context : vmm_getKernelContext());
}

void mm_free(void *ptr)
{
	sd_mutexLock(&__mm_memoryMutex);
	
	sd_process  *process = sd_getCurrentProcess();
	vmm_context *context = (process) ? process->context : vmm_getKernelContext();
	
	
	uintptr_t memory = vmm_getPhysicalAddress(context, (uintptr_t)ptr);
	vmm_unmapPage(context, (uintptr_t)ptr);
	pmm_free((void *)memory);
	
	sd_mutexUnlock(&__mm_memoryMutex);
}




int mm_init(struct multiboot_info *bootinfo)
{
	cn_puts("Initializing memory manager...");
	
	if(!pmm_init(bootinfo) || !vmm_init())
		return 0;
	
	sd_mutexInit(&__mm_memoryMutex);
	cn_puts("ok\n");
	
	return 1;
}
