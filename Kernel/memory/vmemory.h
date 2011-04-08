//
//  vmemory.h
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

#ifndef _VMEMORY_H_
#define _VMEMORY_H_

#include <system/stdint.h>
#include <system/stdbool.h>

#include <scheduler/mutex.h>

#define VMM_PAGETABLEFLAG_PRESENT    1
#define VMM_PAGETABLEFLAG_WRITEABLE  2
#define VMM_PAGETABLEFLAG_USERSPACE  4

typedef struct
{
	sd_mutex mutex;
	uint32_t *pageDirectory;
	
	uintptr_t __contextPage;
	uintptr_t __pageDirPage;
	uintptr_t __kernelPage;
} vmm_context;


extern int vmm_init();


extern vmm_context *vmm_getKernelContext();
extern vmm_context *vmm_getCurrentContext();

extern vmm_context *vmm_createContext();
extern void vmm_destroyContext(vmm_context *context);

extern uintptr_t vmm_getFreePage(vmm_context *context);

extern bool vmm_mapPage(vmm_context *context, uintptr_t virtAddress, uintptr_t physAddress, bool userspace);
extern bool vmm_mapPageRange(vmm_context *context, uintptr_t virtAddress, uintptr_t physAddress, size_t range, bool userspace);

extern bool vmm_unmapPage(vmm_context *context, uintptr_t virtAddress);

extern uintptr_t vmm_getPhysicalAddress(vmm_context *context, uintptr_t virtAddress);

extern void vmm_activateContext(vmm_context *context);

#endif
