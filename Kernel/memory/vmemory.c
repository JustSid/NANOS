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
	/*vmm_context *prevContext = vmm_currentContext;
	vmm_activateContext(vmm_kernelContext);
	
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
		
		
		for(int i=0; i<1024; i++)
		{
			uint32_t *table = (uint32_t *)pmm_alloc();
			
			memset(table, 0, 1024);
			context->pageDirectory[i] = (uint32_t)table;
		}
		
		
		
		vmm_mapPage(context, (uintptr_t)context, (uintptr_t)context, true);

		if(vmm_kernelContext)
		{
			vmm_mapPage(context, (uintptr_t)vmm_kernelContext, (uintptr_t)vmm_kernelContext, true); // Map the kernel context into this context
			vmm_mapPage(context, (uintptr_t)&vmm_currentContext, (uintptr_t)&vmm_currentContext, true); // Map the current contexts pointer into this context
		}
		if(prevContext)
		{
			vmm_mapPage(prevContext, (uintptr_t)context, (uintptr_t)context, true); // Map the new context into the old context
		}
    }
    
	vmm_activateContext(prevContext);*/
	vmm_context *context = (vmm_context *)0x1000;
    return context;
}

void vmm_destroyContext(vmm_context *context)
{
	/*sd_mutexLock(&context->mutex); // The mutext won't be unlocked
	

	for(int i=0; i<1024; i++)
	{
		uint32_t *table = (uint32_t *)(context->pageDirectory[i] & ~0xFFF);
		pmm_free(table);	
	}
	
	pmm_free(context->pageDirectory);
	pmm_free(context);*/
}





uintptr_t vmm_getFreePages(vmm_context *context, uint32_t pageCount)
{
	/*sd_mutexLock(&context->mutex);
	uint32_t i, j, freePages = 0, fPageBeginDirectory, fPageBeginTable;

	for(i=0; i<1024; i++)
	{
		if(freePages == 0)
			fPageBeginDirectory = i;
		
		if((context->pageDirectory[i] & VMM_PAGETABLEFLAG_PRESENT)) 
		{
			uint32_t *table = (uint32_t *)context->pageDirectory[i];
			
			for(j=(i == 0 ? 1 : 0); j<1024; j++)
			{
				if(!(table[j] & VMM_PAGETABLEFLAG_PRESENT))
				{
					if(freePages == 0)
						fPageBeginTable = j;
					
					freePages ++;
					
					if(freePages >= pageCount)
						break;
				}
				else
					freePages = 0;
			}
		}
		else 
		{
			if(i == 0)
			{
				fPageBeginTable = 1;
				freePages = 1024 - 1;
			}
			else 
			{
				if(freePages == 0)
					fPageBeginTable = 0;
				
				freePages += 1024;
			}
		}

		
		if(freePages >= pageCount)
			break;
	}
	
	if(freePages < pageCount)
	{
		cn_puts("vmm_getFreePages(), not enough free pages!");
		sd_mutexUnlock(&context->mutex);
		return 0x0;
	}
	
	sd_mutexUnlock(&context->mutex);
	return (uintptr_t)(fPageBeginDirectory << 22) + (fPageBeginTable << 12);*/
	
	return 0x1000;
}

uintptr_t vmm_getFreePage(vmm_context *context)
{
	return vmm_getFreePages(context, 1);
}




bool vmm_mapPage(vmm_context *context, uintptr_t virtAddress, uintptr_t physAddress, bool userspace)
{
	/*if((((uint32_t)virtAddress | (uint32_t)physAddress) & 0xFFF))
        return false;
	
	sd_mutexLock(&context->mutex);
	
    uint32_t index          = virtAddress / 4096;
    uint32_t directoryIndex = index / 1024;
    uint32_t tableIndex     = index % 1024;
    uint32_t flags			= VMM_PAGETABLEFLAG_PRESENT | VMM_PAGETABLEFLAG_WRITEABLE;
	
	if(userspace)
		flags |= VMM_PAGETABLEFLAG_USERSPACE;
	
	
	
	uint32_t *table = (uint32_t *)(context->pageDirectory[directoryIndex] & ~0xFFF);
	context->pageDirectory[directoryIndex] = ((uint32_t)table) | flags | VMM_PAGETABLEFLAG_USERSPACE;
	
	table[tableIndex] = physAddress | flags;
	__asm__ volatile("invlpg %0" : : "m" (*(char *)virtAddress));
    
	
	sd_mutexUnlock(&context->mutex);
    return true;*/
	
	return true;
}

bool vmm_mapPageRange(vmm_context *context, uintptr_t virtAddress, uintptr_t physAddress, size_t range, bool userspace)
{
	/*// TODO: Atomic mapping?!
	if((((uint32_t)virtAddress | (uint32_t)physAddress) & 0xFFF))
        return false;
	
	size_t offset = 0x0;
	while(offset < range)
	{
		bool result = vmm_mapPage(context, virtAddress + offset, physAddress + offset, userspace);
		if(!result)
		{
			// Bad, bad, bad
			return false;
		}
		
		offset += 0x1000;
	}*/
	
	return true;
}



bool vmm_unmapPage(vmm_context *context, uintptr_t virtAddress)
{
	/*sd_mutexLock(&context->mutex);
	
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
	return false;*/
	
	return true;
}

uintptr_t vmm_getPhysicalAddress(vmm_context *context, uintptr_t virtAddress)
{	
	/*if((uint32_t)virtAddress & 0xFFF)
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
	
	sd_mutexUnlock(&context->mutex);*/
	
	return 0x1000;
}




void vmm_activateContext(vmm_context *context)
{
	/*if(context == NULL)
		return;
	
	sd_mutexLock(&context->mutex);
	
	if(context != vmm_currentContext)
	{
		vmm_currentContext = context;
		//__asm__ volatile("mov %0, %%cr3" : : "r" (context->pageDirectory));
	}
	
	sd_mutexUnlock(&context->mutex);*/
}




int vmm_init()
{
    /*vmm_kernelContext = vmm_createContext();
	if(!vmm_kernelContext)
		return 0;
	
	uintptr_t start = (uintptr_t)&kernelBegin;
	uintptr_t end	= (uintptr_t)&kernelEnd;
	
	size_t		kernelSize = (size_t)(end - start);
	pmm_heap	*heap = pmm_getHeapMap();
	
	

	vmm_mapPageRange(vmm_kernelContext, (uintptr_t)heap, (uintptr_t)heap, sizeof(pmm_heap) + 0x1000, false); // Map the heap
	vmm_mapPageRange(vmm_kernelContext, start, start, kernelSize, false); // Map the kernel
	vmm_mapPageRange(vmm_kernelContext, 0xB8000, 0xB8000, (25 * 80 * 2), false); // Map the video memory
	
	
	
	vmm_mapPageRange(vmm_kernelContext, 0, 0, 4096 * 1024, false);
    vmm_activateContext(vmm_kernelContext);*/

	/*uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= (1 << 31);
    __asm__ volatile("mov %0, %%cr0" : : "r" (cr0));*/
	
	return 1;
}
