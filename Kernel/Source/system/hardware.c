//
//  hardware.h
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


#include "hardware.h"
#include "console.h"
#include "stdint.h"
#include "string.h"
#include "memory.h"

static hw_identifier *currentHardware = NULL;

hw_identifier *hw_currentHardware()
{
	return currentHardware;
}

void cpuid(uint32_t selector, uint32_t *data)
{
	asm volatile ("pushl %%ebx \n"
				  "cpuid \n"
				  "movl %%ebx, %1 \n"
				  "popl %%ebx \n" : "=a" (data[0]), "=r" (data[1]), "=c" (data[2]), "=d" (data[3]) : "a" (selector) : "cc");
}

int hw_checkHardware()
{
	cn_printf("Initializing hardware...");
	
	currentHardware = (hw_identifier *)mm_alloc(sizeof(hw_identifier));
	if(!currentHardware)
		return 0;
	
	uint32_t reg[4];
	cpuid(0, reg);
	
	currentHardware->cpuVendor = processorVendorUnknown;	
	currentHardware->sse2	= false;
	currentHardware->sse3	= false;
	currentHardware->mmx	= false;
	
	cn_printf("ok\n");
	return 1;
}
