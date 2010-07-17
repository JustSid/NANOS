//
//  mmap.h
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

#ifndef _MMAP_H_
#define _MMAP_H_

#include "stdint.h"
#include "stdbool.h"

#include "console.h"
#include "multiboot.h"

extern void mm_init(struct multiboot_info *bootInfo);
extern size_t mm_availableMemory();
extern size_t mm_wiredMemory();

typedef struct
{
	void	*kernelPtr;
	size_t	kernelSize;
	
	struct multiboot_mmap	*mmapBegin;
	struct multiboot_mmap	*mmapEnd;
	size_t	mmapSize; // The size of the available RAM in byte
	
	struct multiboot_info *bootInfo;
} MMHEAPSTRUCT;


#endif