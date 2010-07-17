//
//  mmap.c
//  NANOS
//
//  Created by Sidney Just
//  Copyright © 2010 by Sidney Just
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

#include "mmap.h"

// Information about the heap
MMHEAPSTRUCT *mm_heap;

// Ptr where the kernel are. This is set by the linker
extern void	kernelBegin;
extern void	kernelEnd;

extern void malloc_init();

MMHEAPSTRUCT *mm_getHeap()
{
	return mm_heap;
}

void mm_init(struct multiboot_info *bootInfo)
{
	cn_puts("Initializing memory...");
	
	// We want to store the heap information inside the heap, so we look at which adress the kernel ends
	// The mm_heap pointer will point to the end + 0x1000, to avoid memory troubles, we extend the kernel end by the size of the MMHEAPSTRUCT + 0x1000 so we can mark the space as used
 	void *_kernelSpaceEnd = &kernelEnd;
	mm_heap = _kernelSpaceEnd + 0x1000;
	_kernelSpaceEnd += sizeof(MMHEAPSTRUCT) + 0x1000;
	
	// As we now have a space for our struct, we save all values about the heap
	mm_heap->bootInfo = bootInfo;
	
	mm_heap->kernelPtr	= &kernelBegin;
	mm_heap->kernelSize = (uintptr_t)(_kernelSpaceEnd - mm_heap->kernelPtr);
	
	mm_heap->mmapBegin	= bootInfo->mbs_mmap_addr;
	mm_heap->mmapEnd	= bootInfo->mbs_mmap_addr + bootInfo->mbs_mmap_length;
	
	mm_heap->mmapSize = 0;
	while(mm_heap->mmapBegin < mm_heap->mmapEnd) 
	{
		if(mm_heap->mmapBegin->type == 1) 
		{
			uintptr_t adr		= mm_heap->mmapBegin->base;
			uintptr_t end_addr	= adr + mm_heap->mmapBegin->length;
			
			mm_heap->mmapSize += end_addr - adr;
		}
		mm_heap->mmapBegin++;
	}
	
	mm_heap->mmapBegin = bootInfo->mbs_mmap_addr; // Whe have incremented the pointer in the loop above, so we set it back to it's real value
	
	malloc_init();	
	cn_puts("[ok]\n");
}

size_t mm_availableMemory()
{
	return mm_heap->mmapSize;
}

size_t mm_wiredMemory()
{
	return mm_heap->kernelSize;
}
