//
//  entry.c
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

#include "multiboot.h"

#include "memory.h"
#include "interrupt.h"
#include "system.h"
#include "syscall.h"
#include "scheduler.h"

#include "console.h"
#include "panic.h"
#include "debug.h"

#include "time.h"
#include "keymap_def.h"
#include "ps2.h"
#include "stdio.h"

#define VersionMajor 0
#define VersionMinor 2
#define VersionPatch 0
#define VersionCreate(major, minor, patch) (((major) << 16) | ((minor) << 8) | (patch))
#define VersionCurrent VersionCreate(VersionMajor, VersionMinor, VersionPatch)

// High-Level boot entry
void boot(struct multiboot_info *bootinfo)
{
	cn_cls();
	char nanosWelcome[255];
	sprintf(nanosWelcome, "NANOS build from %s %s. Version: %i.%i.%i:%i\n", __DATE__, __TIME__, VersionMajor, VersionMinor, VersionPatch, VersionCurrent);
	
	cn_puts(nanosWelcome);
	cn_puts("Here be dragons!\n\n");
	
	ir_disableInterrupts();
	
	if(mm_init(bootinfo) == 0)
		panic("Error while initializing the memory manager!");
	
	if(ir_init() == 0)
		panic("Error while initializing the interrupt handler!");
	
	if(st_init() == 0)
		panic("Errow while initializing the system!");
	
	if(sd_init() == 0)
		panic("Error while initializing the scheduler!");
	
	ps_init();
	km_init();
	
	cn_printf("\n\n");
	ir_enableInterrupts();
	
	while(1) {__asm__ volatile ("hlt");}
}