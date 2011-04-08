//
//  pmemory.c
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

#include <scheduler/scheduler.h>
#include <system/string.h>
#include <system/console.h>

#include "pmemory.h"

extern void	kernelBegin;	// Marks the beginning of the kernel (set by linker)
extern void	kernelEnd;		// Marks the end of the kernel (also set by linker)

static pmm_heap __pmm_heapMap;
static pmm_heap *pmm_heapMap = NULL;

pmm_heap *pmm_getHeapMap()
{
	return pmm_heapMap;
}


int pmm_init(struct multiboot_info *bootinfo)
{
	cn_puts("FOOOO");
	struct multiboot_mmap *mmapBegin	= bootinfo->mbs_mmap_addr;
	struct multiboot_mmap *mmapEnd		= (void *)((uintptr_t)bootinfo->mbs_mmap_addr + bootinfo->mbs_mmap_length);
	
	struct multiboot_mmap *mmap = mmapBegin;
	pmm_heapMap = &__pmm_heapMap;
	
	// Initialize the heap map
	memset(&pmm_heapMap->heap, 0, sizeof(MM_HEAPSIZE));
	sd_mutexInit(&pmm_heapMap->mutex);
	
	
	// Mark the default stuff
	while(mmap < mmapEnd) 
	{
		if(mmap->type == 1) 
		{
			uintptr_t addressStart	= mmap->base;
			uintptr_t addressEnd	= addressStart + mmap->length;
			
			uintptr_t address = addressStart;
			
			while(address < addressEnd) 
			{
				pmm_mark((void *)address, false);
				address += 0x1000;
			}
		}
		
		
		mmap++;
	}
	
	// Mark the kernel as allocated
	uintptr_t address = (uintptr_t)&kernelBegin;
	
	while(address < (uintptr_t)&kernelEnd) 
	{
		pmm_mark((void *)address, true);
		address += 0x1000;
	}
	
	cn_puts("Bar");
	pmm_mark(NULL, true);
	return 1;
}



void pmm_mark(void *page, bool used)
{
	sd_mutexLock(&pmm_heapMap->mutex);	
	uintptr_t index = (uintptr_t)page / 4096;
	
	if(used)
	{
		pmm_heapMap->heap[index / 32] &= ~(1 << (index % 32));
	}
	else 
	{
		pmm_heapMap->heap[index / 32] |= (1 << (index % 32));
	}
	
	sd_mutexUnlock(&pmm_heapMap->mutex);
}



void *pmm_alloc()
{
	sd_mutexLock(&pmm_heapMap->mutex);
	
	int i, j;
	for(i=0; i<MM_HEAPSIZE; i++)
	{
		if(pmm_heapMap->heap[i] != 0)
		{
			for(j=0; j<32; j++)
			{
				if(pmm_heapMap->heap[i] & (1 << j)) 
				{
                    pmm_heapMap->heap[i] &= ~(1 << j);
					sd_mutexUnlock(&pmm_heapMap->mutex);
					
                    return (void *)((i * 32 + j) * 4096);
                }
			}
		}
	}
	
	sd_mutexUnlock(&pmm_heapMap->mutex);
	return NULL;
}

void pmm_free(void *page)
{
	pmm_mark(page, false);
}
