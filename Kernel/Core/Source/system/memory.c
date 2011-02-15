//
//  memory.c
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

#include "memory.h"
#include "console.h"
#include "scheduler.h"
#include "string.h"

/**
 * Represents one block of memory
 **/
typedef struct mm_memoryFragment
{
	void	*start;
	size_t	size;
	
	uint32_t flags;
	uint32_t pid;
	
	struct mm_memoryFragment *next;
	struct mm_memoryFragment *last;
} mm_memoryFragment;

extern void	kernelBegin;	// Marks the beginning of the kernel (set by linker)
extern void	kernelEnd;		// Marks the end of the kernel (also set by linker)

static mm_memoryFragment	*firstFragment;
static mm_memoryFragment	*lastFragment;
static size_t				mm_fragments = 0;

size_t mm_init(struct multiboot_info *bootinfo)
{
	cn_puts("Initializing memory manager...");
	
	struct multiboot_mmap *currMap = bootinfo->mbs_mmap_addr;
	struct multiboot_mmap *lastMap = (void *)((uintptr_t)bootinfo->mbs_mmap_addr + bootinfo->mbs_mmap_length);
	
	size_t heapSize	= 0;
	while(currMap < lastMap) // Iterate through all memory chunks
	{		
		// We are only looking for free chunks
		if(currMap->type == 1)
		{
			uintptr_t	baseAddr = currMap->base;
			uintptr_t	currAddr = currMap->base;
			uintptr_t	lastAddr = currMap->base + currMap->length;
			
			while(currAddr < lastAddr)
			{
				currAddr		+= 0x1000; // Count the size of the fragment
				heapSize		+= 0x1000;
			}
			
			uintptr_t kBegin	= (uintptr_t)&kernelBegin;
			uintptr_t kEnd		= (uintptr_t)&kernelEnd;
			
			if(baseAddr == 0x0)
				baseAddr = 0x1000; // Skip the first 4 kB to avoid having NULL as fragment
			
			if(!lastFragment)
			{
				// This is the first fragment
				if(kBegin >= baseAddr && kEnd <= lastAddr)
				{
					// The kernel is inside this fragment, wee need to create two fragments
					lastFragment	= (mm_memoryFragment *)kEnd + 0x100;
					firstFragment	= lastFragment;
					
					lastFragment->start = (void *)baseAddr;
					lastFragment->size	= kEnd - baseAddr;
					lastFragment->flags = (MM_FRAGMENT_USED | MM_FRAGMENT_WIRED);
					
					lastFragment->next	= (mm_memoryFragment *)lastFragment + sizeof(mm_memoryFragment); // The next fragment comes right after this fragment
					lastFragment->last	= NULL;
					
					mm_memoryFragment *temp = lastFragment;
					
					// The second fragment
					lastFragment = lastFragment->next;
					lastFragment->start = lastFragment + sizeof(mm_memoryFragment);
					lastFragment->size	= lastAddr - (uintptr_t)lastFragment->start;
					lastFragment->flags = MM_FRAGMENT_FREE;
					
					lastFragment->last = temp;
					lastFragment->next = NULL;
					
					mm_fragments += 2;
				}
				else 
				{
					lastFragment	= (mm_memoryFragment *)baseAddr;
					firstFragment	= lastFragment;
					
					uintptr_t fragmentStart = (uintptr_t)lastFragment;
					lastFragment->start = (void *)(fragmentStart + sizeof(mm_memoryFragment));
					lastFragment->size	= lastAddr - (uintptr_t)lastFragment->start;
					lastFragment->flags = MM_FRAGMENT_FREE;
					
					lastFragment->next	= NULL;
					lastFragment->last	= NULL;
					
					mm_fragments ++;
				}
			}
			else 
			{
				// Place a new fragment after the last one
				if(kBegin >= baseAddr && kEnd <= lastAddr)
				{
					// The kernel is inside this fragment, wee need to create two fragments
					mm_memoryFragment *temp = lastFragment;
					lastFragment->next		= (mm_memoryFragment *)kEnd + 0x100;
					lastFragment			= lastFragment->next;
					
					lastFragment->start = (void *)baseAddr;
					lastFragment->size	= kEnd - baseAddr;
					lastFragment->flags = (MM_FRAGMENT_USED | MM_FRAGMENT_WIRED);
					
					lastFragment->next	= (mm_memoryFragment *)lastFragment + sizeof(mm_memoryFragment); // The next fragment comes right after this fragment
					lastFragment->last	= temp;
					
					temp = lastFragment;
					lastFragment = lastFragment->next;
					lastFragment->start = lastFragment + sizeof(mm_memoryFragment);
					lastFragment->size	= lastAddr - (uintptr_t)lastFragment->start;
					lastFragment->flags = MM_FRAGMENT_FREE;
					
					lastFragment->last = temp;
					lastFragment->next = NULL;
					
					mm_fragments += 2;
				}
				else 
				{
					mm_memoryFragment *temp = lastFragment;
					lastFragment->next		= (mm_memoryFragment *)baseAddr;
					lastFragment			= lastFragment->next;
					
					uintptr_t fragmentStart = (uintptr_t)baseAddr;
					lastFragment->start = (void *)(fragmentStart + sizeof(mm_memoryFragment));
					lastFragment->size	= lastAddr - (uintptr_t)lastFragment->start;
					lastFragment->flags = MM_FRAGMENT_FREE;
					
					lastFragment->next = NULL;
					lastFragment->last = temp;
					
					mm_fragments ++;
				}

			}
		}
		
		currMap ++;
	}
	
	// We are done :)
	cn_puts("ok\n");
	
	return mm_fragments;
}

void mm_dumpMemory()
{
	cn_printf("Memory dump:\n");
	
	int i = 0;
	mm_memoryFragment *fragment = firstFragment;
	
	while(fragment)
	{
		uintptr_t fragmentEnd = (uintptr_t)fragment->start;
		fragmentEnd += fragment->size;
		cn_printf("Fragment %i/%i. From: %p to %x (%i b). Type: ", i+1, mm_fragments, fragment->start, fragmentEnd, fragment->size);
		
		if(fragment->flags & MM_FRAGMENT_FREE)
			cn_printf("FREE ");
		
		if(fragment->flags & MM_FRAGMENT_USED)
			cn_printf("USED ");
		
		if(fragment->flags & MM_FRAGMENT_WIRED)
			cn_printf("WIRED ");
		
		cn_printf("\n");
		
		i++;
		fragment = fragment->next;
	}
}

void *mm_alloc(size_t size)
{
	mm_memoryFragment *fragment = firstFragment;
	mm_memoryFragment *biggestFragment = NULL;
	
	// We are looking for the biggest fragment to allocate it
	while(fragment)
	{
		if(fragment->flags & MM_FRAGMENT_FREE && fragment->size >= size)
		{
			if(!biggestFragment)
			{
				biggestFragment = fragment;
				continue;
			}
			
			if(fragment->size >= biggestFragment->size)
				biggestFragment = fragment;
			
		}
		   
		   
		fragment = fragment->next;
	}
	
	
	if(biggestFragment)
	{
		// We found one fragment, lets mark it as used
		biggestFragment->flags &= ~MM_FRAGMENT_FREE;
		biggestFragment->flags |= MM_FRAGMENT_USED;
		biggestFragment->pid = sys_getpid();
		
		if(biggestFragment->size == size)
			return biggestFragment->start;
		
		// Prepare another fragment to appended to last fragment as it will be trimmed to fit the size
		uintptr_t fragmentAddr = (uintptr_t)biggestFragment->start;
		fragmentAddr += (sizeof(mm_memoryFragment) + size);
		
		mm_memoryFragment *appendFragment = (mm_memoryFragment *)fragmentAddr;
		
		size_t fragmentSize = (size_t)biggestFragment->start;
		fragmentSize += biggestFragment->size;
		fragmentSize -= (size_t)appendFragment->start;
		
		
		appendFragment->start	= appendFragment + sizeof(mm_memoryFragment);
		appendFragment->size	= fragmentSize;
		appendFragment->flags	= MM_FRAGMENT_FREE;
		
		appendFragment->next	= biggestFragment->next;
		appendFragment->last	= biggestFragment;
		
		// Append the prepared fragment
		biggestFragment->size = size;
		biggestFragment->next = appendFragment;
		
		mm_fragments ++;		
		return biggestFragment->start;
	}
	
	return NULL;
}

void mm_free(void *ptr)
{
	mm_memoryFragment *fragment = firstFragment;
	while(fragment) 
	{
		if(fragment->start == ptr)
		{
			if(fragment->flags & MM_FRAGMENT_WIRED)
				return; // We don't mark wired space as free!
			
			
			mm_fragments --;
			fragment->flags = MM_FRAGMENT_FREE; // Mark it as free so it can be allocated again
			fragment->pid	= SD_PID_INVALID;
			return;
		}
		
		fragment = fragment->next;
	}
}

void mm_defrag()
{
	mm_memoryFragment *fragment = firstFragment;
	while(fragment && fragment->next) 
	{
		if((fragment->flags & MM_FRAGMENT_FREE) && (fragment->next->flags & MM_FRAGMENT_FREE))
		{
			fragment->size += fragment->next->size + sizeof(mm_memoryFragment);
			fragment->next = fragment->next->next;
			
			mm_fragments --;
			continue;
		}
		
		fragment = fragment->next;
	}
}

void mm_clear(uint32_t pid)
{
	if(!sd_taskWithPid(pid))
	{
		mm_memoryFragment *fragment = firstFragment;
		while(fragment) 
		{
			if(fragment->pid == pid)
			{
				mm_free((void *)fragment->start);
			}
			
			fragment = fragment->next;
		}
		
		mm_defrag();
	}
}
