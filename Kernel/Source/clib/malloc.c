//
//  malloc.c
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

#include "malloc.h"

extern MMHEAPSTRUCT *mm_getHeap();

typedef struct M_HEAP_NODE
{
	uintptr_t	*pointer;
	size_t		size;
	
	uint8_t flags;
	
	struct M_HEAP_NODE *next;
} M_HEAP_NODE;

M_HEAP_NODE *m_firstNode; // The first node is a fixed node which points to the memory allocated by the kernel
// The node after the first node points to the free memory and will be sliced into pieces when new memory get's allocated

// Examples:
// Before the first malloc:
// <--- m_firstNode --- ><---          Free           --->
// [.......kernel.......][                               ]
// 
// After the first malloc:
// <--- m_firstNode --- ><--- X ---><---    Free      --->
// [.......kernel.......][.........][                    ]
// 
// After a free:
// <--- m_firstNode --- ><--- X ---><---    Free      --->
// [.......kernel.......][         ][                    ]
// 
// 

void malloc_init()
{
	MMHEAPSTRUCT *mm_heap = mm_getHeap();
	
	m_firstNode = (void *)(mm_heap->kernelPtr + mm_heap->kernelSize + 0x0100 + sizeof(M_HEAP_NODE));
	mm_heap->kernelSize += 0x0100 + sizeof(M_HEAP_NODE);
	
	m_firstNode->pointer = mm_heap->kernelPtr;
	m_firstNode->size	 = mm_heap->kernelSize + sizeof(M_HEAP_NODE);
	m_firstNode->flags   = M_FLAG_USED | M_FLAG_WIRED;
	m_firstNode->next	 = (void *)mm_heap->kernelPtr + m_firstNode->size - sizeof(M_HEAP_NODE);
	
	m_firstNode->next->pointer	= (void *)m_firstNode->pointer + m_firstNode->size + sizeof(M_HEAP_NODE);
	m_firstNode->next->size		= (mm_heap->mmapSize - m_firstNode->size);
	m_firstNode->next->flags	= 0;
	m_firstNode->next->next		= NULL;
}

// This operation is really expensive and should only invoked if it's really needed.
// Eg. the malloc and realloc functions call this function if they can't allocate enough memory.
void m_defrag()
{
	M_HEAP_NODE	*node = m_firstNode;
	
	while(node && node->next)
	{
		if(!(node->flags & M_FLAG_USED) && !(node->next->flags & M_FLAG_USED))
		{
			node->flags |= node->next->flags;
			node->size += node->next->size - sizeof(M_HEAP_NODE);
			
			node->next = node->next->next;
			
			continue;
		}
		
		node = node->next;
	}
}

void *malloc(size_t size)
{
	if(size <= 0)
		return NULL;
	
	M_HEAP_NODE	*node = m_firstNode;
	
	while(node)
	{
		if(!(node->flags & M_FLAG_USED))
		{
			if(node->size - sizeof(M_HEAP_NODE) >= size)
				break;
		}
		
		node = node->next;
	}
	
	static bool failed = false;
	
	if(node)
	{
		failed = false;
		
		if(node->size - sizeof(M_HEAP_NODE) == size)
		{
			node->flags |= M_FLAG_USED;
			return node->pointer;
		}
		
		if(node->next == NULL)
		{
			size_t lastSize = node->size;
			
			node->size = size + sizeof(M_HEAP_NODE);
			node->next = (void *)node->pointer + size;
			
			node->next->flags = 0;
			node->next->pointer = (void *)node->pointer + node->size + sizeof(M_HEAP_NODE);
			node->next->size = lastSize - node->size;
			node->next->next = NULL;
			
			node->flags |= M_FLAG_USED;
			
			return node->pointer;
		}
		
		M_HEAP_NODE *temp	= node->next;
		size_t lastSize		= node->size;
		
		node->size = size + sizeof(M_HEAP_NODE);
		node->next = (void *)node->pointer + size;
		
		node->next->flags = temp->flags;
		node->next->pointer = (void *)node->pointer + node->size + sizeof(M_HEAP_NODE);
		node->next->size = lastSize - (size + sizeof(M_HEAP_NODE));
		node->next->next = temp;
		
		node->flags |= M_FLAG_USED;
		
		return node->pointer;
	}
	
	if(!failed) 
	{
		failed = true;
		
		m_defrag();
		return malloc(size);
	}
	
	return NULL;
}

void *calloc(size_t count, size_t size)
{
	void *ptr = malloc(count * size);
	if(ptr)
	{
		memset(ptr, 0, count * size);
	}
	return ptr;
}

void *realloc(void *ptr, size_t size)
{
	if(!ptr && size == 0)
	{
		return NULL;
	}
	else if(!ptr)
	{
		return malloc(size);
	}
	else if(size == 0)
	{
		free(ptr);
		return NULL;
	}
	
	M_HEAP_NODE *node = m_firstNode;
	while(node && node->pointer != ptr)
	{
		node = node->next;
	}
	
	if(node)
	{
		void *temp = malloc(size);
		
		if(temp)
		{
			memcpy(temp, node->pointer, node->size - sizeof(M_HEAP_NODE));
			free(node->pointer);
		}
		
		return temp;
	}
	
	return NULL;
}

void free(void *ptr)
{
	M_HEAP_NODE	*node = m_firstNode;
	
	while(node && node->pointer != ptr)
	{
		node = node->next;
	}
	
	
	if(node && !(node->flags & M_FLAG_WIRED))
	{
		node->flags &= ~M_FLAG_USED; // Mark the memory as free
	}
	else 
	{
		cn_puts("Pointer being freed was not allocated!\n");
	}
}

size_t m_getOccupiedSize(uint8_t flag)
{
	size_t size = 0;
	
	M_HEAP_NODE *node = m_firstNode;
	while(node) 
	{
		if(flag == M_FLAG_ALL)
		{
			size += (node->size - sizeof(M_HEAP_NODE));
		}
		else 
		{
			switch(flag) 
			{
				case M_FLAG_FREE:
					if(node->flags == 0)
					{
						size += (node->size - sizeof(M_HEAP_NODE));
					}
					break;
					
				case M_FLAG_USED:
					if(node->flags & M_FLAG_USED)
					{
						size += (node->size - sizeof(M_HEAP_NODE));
					}
					break;
					
				case M_FLAG_WIRED:
					if(node->flags & M_FLAG_WIRED)
					{
						size += (node->size - sizeof(M_HEAP_NODE));
					}
					break;
			}
		}
		
		node = node->next;
	}
	
	return size;
}
