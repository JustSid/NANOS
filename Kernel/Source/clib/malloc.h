//
//  malloc.h
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

#ifndef _MALLOC_H_
#define _MALLOC_H_

#include "kernel.h"

#include "stdint.h"
#include "string.h"

#include "mmap.h"
#include "console.h"

#define M_FLAG_FREE  0 // The memory is free, nuff said
#define M_FLAG_USED  1 // If this flag isnt set, the node is free
#define M_FLAG_WIRED 2 // The memory is wired which means that it is used by the kernel

#define M_FLAG_ALL   UINT8_MAX

extern void *malloc(size_t size);
extern void *calloc(size_t count, size_t size);

extern void *realloc(void *ptr, size_t size);

extern void free(void *ptr);

extern size_t m_getOccupiedSize(uint8_t flag);

#endif
