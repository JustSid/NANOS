//
//  pmemory.h
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

#ifndef _PMEMORY_H_
#define _PMEMORY_H_

#include <boot/multiboot.h>

#include <system/stdint.h>
#include <system/stdbool.h>

#include <scheduler/mutex.h>

#define MM_HEAPSIZE 32768

typedef struct
{
	sd_mutex mutex;
	uint32_t heap[MM_HEAPSIZE];
} pmm_heap;


extern int pmm_init(struct multiboot_info *bootinfo);
extern pmm_heap *pmm_getHeapMap();

extern void pmm_mark(void *page, bool used);

extern void *pmm_alloc();
extern void pmm_free(void *page);

#endif
