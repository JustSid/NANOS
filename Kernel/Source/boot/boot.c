//
//  boot.c
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

#include "kernel.h"
#include "syscall.h"

#include "multiboot.h"
#include "video.h"
#include "console.h"

#include "mmap.h"

#include "interruptManager.h"

#include "taskd.h"
#include "time.h"
#include "test.h"

#include "keyboard.h"

void boot_x86(struct multiboot_info *bootInfo)
{
	// Fasten your seatbelt and be ready to buy new hardware :)
	cn_cls(); // Clear the screen to remove unwanted artifacts
	
	// Print the Welcome message
	cn_setTextColor(VIDEO_COLOR_CYAN); // We color the message with CYAN
	cn_printf("NANOS - Build from %s %s\n", __DATE__, __TIME__);
	cn_setTextColor(VIDEO_COLOR_RED); // And then set the color back to the default text color
	cn_puts("Here be dragons!\n\n"); // Some words of warning (:
	cn_setTextColor(VIDEO_COLOR_TEXT); // And then set the color back to the default text color
	
	int x;
	for(x=0; x<5; x++)
	{
		// Color "NANOS"
		vd_setAttribute(x, 0, (char)VIDEO_COLOR_WHITE);
	}	
	
	// Actual boot 
	mm_init(bootInfo);	// Initialize the memory mapping
	ir_init();			// Initialize the interrupt controller
	syscall_init();		// Allows the use of syscalls
	td_init();			// Kickof the task daemon	
	tm_init();			// Launch the time daemon
	init_keyboard();
	
	/*// Print the memory footprint
	cn_puts("Memory footprint:\n");
	cn_printf("   Wired: %i Kb\n", m_getOccupiedSize(M_FLAG_WIRED)	/ 1024);
	cn_printf("   Used:  %i Kb\n", m_getOccupiedSize(M_FLAG_USED)	/ 1024);
	cn_printf("   Free:  %i Kb\n", m_getOccupiedSize(M_FLAG_FREE)	/ 1024);
	cn_printf("   All:   %i Kb\n", m_getOccupiedSize(M_FLAG_ALL)	/ 1024);*/
	
	cn_putc('\n');
	
	td_spawnProcess(taskA, TD_TASK_PRIORITY_DEFAULT);
	td_spawnProcess(taskB, TD_TASK_PRIORITY_DEFAULT);
}
