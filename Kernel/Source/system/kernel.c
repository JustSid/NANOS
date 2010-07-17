//
//  kernel.c
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

void k_panic(const char *reason)
{
	cn_cls();
	cn_setCursor(false);
	
	cn_setTextColor(VIDEO_COLOR_RED);
	cn_puts("Kernel panic!\n");
	
	cn_setTextColor(VIDEO_COLOR_TEXT);
	cn_printf("Reason: \"%s\"\n", reason);
	
	ir_cpu_state *state = ir_lastState(); // We load the old CPU state here. Loading the current CPU state here, would show the CPU state for this function
	// But it normally panics in other functions ;)
	
	if(state)
	{
		cn_printf("\nLast known CPU State:\n");
		cn_printf("   %%eax: %p\n", state->eax);
		cn_printf("   %%ebx: %p\n", state->ebx);
		cn_printf("   %%ecx: %p\n", state->ecx);
		cn_printf("   %%edx: %p\n", state->edx);
		cn_printf("   %%esi: %p\n", state->esi);
		cn_printf("   %%edi: %p\n", state->edi);
		cn_printf("   %%ebp: %p\n", state->ebp);
	}
	else 
	{
		cn_printf("Could not load the last known CPU state");
	}

	
	cn_puts("\nKernel halted\n");
	
	while(1) 
	{
		asm volatile("cli; hlt");
		// Stop da train :3
	}
}
