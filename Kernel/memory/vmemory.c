//
//  vmemory.c
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

#include <system/string.h>
#include <system/console.h>

#include "vmemory.h"
#include "pmemory.h"

extern void	kernelBegin;	// Marks the beginning of the kernel (set by linker)
extern void	kernelEnd;		// Marks the end of the kernel (also set by linker)

static vmm_context *vmm_kernelContext = NULL;
static vmm_context *vmm_currentContext = NULL;

vmm_context *vmm_getKernelContext()
{
	return vmm_kernelContext;
}

vmm_context *vmm_getCurrentContext()
{
	return vmm_currentContext;
}



vmm_context *vmm_createContext()
{
	vmm_context *context = (vmm_context *)pmm_alloc();
    if(context)
    {
        context->pageDirectory = pmm_alloc();
        if(!context->pageDirectory)
        {
            pmm_free(context);
            return NULL;
        }
        
		sd_mutexInit(&context->mutex);
		
		
		context->__contextPage = vmm_getFreePage(context);
		vmm_mapPage(context, (uintptr_t)context, context->__contextPage, false);
	
		context->__pageDirPage = vmm_getFreePage(context);
		vmm_mapPage(context, (uintptr_t)context->pageDirectory, context->__pageDirPage, false);
		
		
        memset(context->pageDirectory, 0, 4096);
		
		if(vmm_kernelContext)
		{
			context->__kernelPage = vmm_getFreePage(context);
			vmm_mapPage(context, (uintptr_t)vmm_kernelContext, context->__kernelPage, false); // Map the kernel context into this context
		}
		else 
		{
			context->__kernelPage = context->__contextPage; // As there is no other context, we can safely assume that we are creating the kernels context.
		}

    }
    
    return context;
}

void vmm_destroyContext(vmm_context *context)
{
	sd_mutexLock(&context->mutex); // The mutext won't be unlocked
	
	int i;
	for(i=0; i<1024; i++)
	{
		if(context->pageDirectory[i] & VMM_PAGETABLEFLAG_PRESENT) 
		{
			uint32_t *table = (uint32_t *)(context->pageDirectory[i] & ~0xFFF);
			pmm_free(table);	
		}
	}
	
	pmm_free(context->pageDirectory);
	pmm_free(context);
}




uintptr_t vmm_getFreePage(vmm_context *context)
{
	sd_mutexLock(&context->mutex);
	
	uintptr_t address = 0x0;
	while(1)
	{
		uint32_t index = (address / 4096) / 1024;
		if(!(context->pageDirectory[index] & VMM_PAGETABLEFLAG_PRESENT)) 
		{
			sd_mutexUnlock(&context->mutex);
			return address;
		}
		
		address += 0x1000;
	}
	
	// This point will never be reached
	return 0x0;
}



bool vmm_mapPage(vmm_context *context, uintptr_t virtAddress, uintptr_t physAddress, bool userspace)
{
	if((((uint32_t)virtAddress | (uint32_t)physAddress) & 0xFFF))
        return false;
	
	sd_mutexLock(&context->mutex);
	
    uint32_t index          = virtAddress / 4096;
    uint32_t directoryIndex = index / 1024;
    uint32_t tableIndex     = index % 1024;
    uint32_t flags			= VMM_PAGETABLEFLAG_PRESENT | VMM_PAGETABLEFLAG_WRITEABLE;
	
	if(userspace)
		flags |= VMM_PAGETABLEFLAG_USERSPACE;
	
	
    uint32_t *table;
    if(context->pageDirectory[directoryIndex] & VMM_PAGETABLEFLAG_PRESENT) 
	{
        table = (uint32_t *)(context->pageDirectory[directoryIndex] & ~0xFFF);
	}
    else
    {
        table = (uint32_t *)pmm_alloc();
        if(!table)
		{
			sd_mutexUnlock(&context->mutex);
            return false;
		}
		
		// Avoid a deadlock
		sd_mutexUnlock(&context->mutex);
		vmm_mapPage(context, (uintptr_t)table, (uintptr_t)table, true); // Map the physical memory
        sd_mutexLock(&context->mutex); // Lock the mutex again
		
        memset(table, 0, 1024);
        context->pageDirectory[directoryIndex] = ((uint32_t)table) | flags;
    }
    
	if(table)
	{
		table[tableIndex] = physAddress | flags;
		__asm__ volatile("invlpg %0" : : "m" (*(char *)virtAddress));
	}
    
	sd_mutexUnlock(&context->mutex);
    return true;
}

bool vmm_mapPageRange(vmm_context *context, uintptr_t virtAddress, uintptr_t physAddress, size_t range, bool userspace)
{
	// TODO: Atomic mapping?!
	if((((uint32_t)virtAddress | (uint32_t)physAddress) & 0xFFF))
        return false;
	
	
	size_t location = 0x0;
	while(location < range)
	{
		cn_printf("Mapping %x", location);
		
		bool result = vmm_mapPage(context, virtAddress + location, physAddress + location, userspace);
		if(!result)
		{
			// Bad, bad, bad
			return false;
		}
		
		location += 0x1000;
	}
	
	return true;
}



bool vmm_unmapPage(vmm_context *context, uintptr_t virtAddress)
{
	sd_mutexLock(&context->mutex);
	
	if((uint32_t)virtAddress & 0xFFF)
        return false;
	
	uint32_t index          = virtAddress / 4096;
    uint32_t directoryIndex = index / 1024;
	
	if(context->pageDirectory[directoryIndex] & VMM_PAGETABLEFLAG_PRESENT) 
	{
        context->pageDirectory[directoryIndex] &= ~0xFFF;
		
		sd_mutexUnlock(&context->mutex);
		return true;
	}
	
	sd_mutexUnlock(&context->mutex);
	return false;
}

uintptr_t vmm_getPhysicalAddress(vmm_context *context, uintptr_t virtAddress)
{	
	if((uint32_t)virtAddress & 0xFFF)
        return 0x0;
	
	sd_mutexLock(&context->mutex);	
	uint32_t index          = virtAddress / 4096;
    uint32_t directoryIndex = index / 1024;
    uint32_t tableIndex     = index % 1024;
	
	uint32_t *table;
    if(context->pageDirectory[directoryIndex] & VMM_PAGETABLEFLAG_PRESENT) 
	{
        table = (uint32_t *)(context->pageDirectory[directoryIndex] & ~0xFFF);
		uintptr_t address = table[tableIndex];
		
		address &= ~(VMM_PAGETABLEFLAG_PRESENT | VMM_PAGETABLEFLAG_WRITEABLE | VMM_PAGETABLEFLAG_USERSPACE);
		sd_mutexUnlock(&context->mutex);
		return address;
	}
	
	sd_mutexUnlock(&context->mutex);
	return 0x0;
}




void vmm_activateContext(vmm_context *context)
{
	sd_mutexLock(&context->mutex);
	
	if(context != vmm_currentContext)
	{
		vmm_currentContext = context;
		__asm__ volatile("mov %0, %%cr3" : : "r" (context->pageDirectory));
	}
	
	sd_mutexUnlock(&context->mutex);
}




int vmm_init()
{
	cn_puts("Test");
    vmm_kernelContext = vmm_createContext();
	if(!vmm_kernelContext)
		return 0;
	
	cn_puts("Test2");
	uintptr_t start = (uintptr_t)&kernelBegin;
	uintptr_t end	= (uintptr_t)&kernelEnd;
	
	size_t		kernelSize = (size_t)(end - start);
	pmm_heap	*heap = pmm_getHeapMap();
	
	
	cn_puts("Context");
	vmm_mapPageRange(vmm_kernelContext, (uintptr_t)vmm_kernelContext, (uintptr_t)vmm_kernelContext, sizeof(vmm_context), true); // Map the kernels virtual memory context
	cn_puts("Heap");
	vmm_mapPageRange(vmm_kernelContext, (uintptr_t)heap, (uintptr_t)heap, sizeof(pmm_heap), false); // Map the heap
	
	cn_puts("Kernel");
	vmm_mapPageRange(vmm_kernelContext, start, start, kernelSize, false); // Map the kernel
	cn_puts("Video");
	vmm_mapPageRange(vmm_kernelContext, 0xB8000, 0xB8000, (25 * 80 * 2), false); // Map the video memory
	
	
	
	//vmm_mapPageRange(vmm_kernelContext, 0, 0, 4096 * 1024, false);
	cn_puts("Activate");
    vmm_activateContext(vmm_kernelContext);
	
	cn_puts("Activate 2");
	uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= (1 << 31);
    __asm__ volatile("mov %0, %%cr0" : : "r" (cr0));
	
	cn_puts("Done");
	return 1;
}
