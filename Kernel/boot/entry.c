//
//  entry.c
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

#include "multiboot.h"

#include <system/cmos.h>
#include <memory/memory.h>
#include <system/interrupt.h>
#include <system/system.h>
#include <system/syscall.h>

#include <system/debug.h>
#include <system/console.h>
#include <system/panic.h>
#include <system/string.h>

#include <system/stdio.h>
#include <system/ps2.h>
#include <system/keymap_def.h>

#include <scheduler/scheduler.h>
#include <loader/loader.h>

#define VersionMajor 0
#define VersionMinor 2
#define VersionPatch 10
#define VersionCreate(major, minor, patch) (((major) << 16) | ((minor) << 8) | (patch))
#define VersionCurrent VersionCreate(VersionMajor, VersionMinor, VersionPatch)

#define BareBoneBoot

extern void kernelTask();

// High-Level boot entry
void boot(struct multiboot_info *bootinfo)
{	
	cn_cls();
	char nanosWelcome[255];
	sprintf(nanosWelcome, "NANOS build from %s %s. Version: %i.%i.%i:%i\n", __DATE__, __TIME__, VersionMajor, VersionMinor, VersionPatch, VersionCurrent);
	
	cn_puts(nanosWelcome);
	cn_puts("Here be dragons!\n\n");
	
	ir_disableInterrupts();
	
	// Load the most basic components
	if(mm_init(bootinfo) == 0) // Memory manager
		panic("Error while initializing the memory manager!");
	
	if(ir_init() == 0) // Interrupt controller
		panic("Error while initializing the interrupt handler!");
	
	if(st_init() == 0) // System stuff (syscalls)
		panic("Errow while initializing the system!");
	
	
	#ifndef BareBoneBoot
	if(sd_init() == 0) // Scheduler and kernel task
		panic("Error while initializing the scheduler!");
	
	ps_init(); // PS/2 controller
	km_init(); // Keyboard defintions
	
	
	
	// Load the multiboot modules
	cn_printf("\nLoading %i module%s\n", bootinfo->mbs_mods_count, (bootinfo->mbs_mods_count == 1) ? "" : "s");
	struct multiboot_module *modules = bootinfo->mbs_mods_addr;
	
	uint32_t i;
    for(i=0; i<bootinfo->mbs_mods_count; i++) 
		ld_createMultibootModuleImage(&modules[i]);

	#endif
	cn_puts("\n\n");
	
	// And then reality kicks in...
	ir_enableInterrupts();
	
	#ifndef BareBoneBoot
	kernelTask(); // Spin up the real kernel task, see scheduler.c for more informations
	#else
	for(int i=0; i<5; i++)
	{
		uintptr_t freePage = vmm_getFreePage(vmm_getKernelContext());
		
		void *ptr = mm_alloc(10);
		strcpy((char *)ptr, "foobar");
		
		cn_printf("Free page: %p. Allocated: %p\n", freePage, ptr);
		mm_free(ptr);
	}
	
	cn_printf("Bare bone boot completed...");
	while(1) {__asm__("hlt;");}
	#endif
}
