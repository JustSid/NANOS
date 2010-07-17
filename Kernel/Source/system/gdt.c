//
//  gdt.c
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

#include "gdt.h"

static uint64_t gdt_table[GDT_ENTIRES];
static uint32_t gdt_tss[32] = {0, 0, 0x10};

void gdt_setEntry(int index, unsigned int base, unsigned int limit, int flags)
{
	if(index < 0 || index >= GDT_ENTIRES)
		return;
	
    gdt_table[index] =  limit  & 0xffffLL;
    gdt_table[index] |= (base  & 0xffffffLL)	<< 16;
    gdt_table[index] |= (flags & 0xffLL)		<< 40;
	gdt_table[index] |= ((limit >> 16) & 0xfLL)		<< 48;
    gdt_table[index] |= ((flags >> 8 ) & 0xffLL)	<< 52;
    gdt_table[index] |= ((base >>  24) & 0xffLL)	<< 56;
}

void gdt_commitEntries()
{
	struct 
	{
        uint16_t	limit;
        void		*pointer;
    } __attribute__((packed)) 
	
	gdtp = 
	{
        .limit		= GDT_ENTIRES * 8 - 1,
        .pointer	= gdt_table,
    };	
	
	
	asm volatile("lgdt	%0" : : "m" (gdtp));
	
	asm volatile("mov	$0x10,  %ax;"
				 "mov	%ax,    %ds;"
				 "mov	%ax,    %es;"
				 "mov	%ax,    %ss;"
				 "ljmp	$0x8,   $.1;"
				 ".1:");
	
	asm volatile("ltr %%ax" : : "a" (5 << 3));
}

void gdt_defaults(bool commitImmediately)
{
	gdt_setEntry(0, 0, 0, 0);
	gdt_setEntry(1, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT | GDT_FLAG_CODESEG | GDT_FLAG_4K | GDT_FLAG_PRESENT | GDT_FLAG_RING0); // Ring 0 code segment
	gdt_setEntry(2, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT | GDT_FLAG_DATASEG | GDT_FLAG_4K | GDT_FLAG_PRESENT | GDT_FLAG_RING0); // Ring 0 data segment
	gdt_setEntry(3, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT | GDT_FLAG_CODESEG | GDT_FLAG_4K | GDT_FLAG_PRESENT | GDT_FLAG_RING3); // Ring 3 code segment
    gdt_setEntry(4, 0, 0xfffff, GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT | GDT_FLAG_DATASEG | GDT_FLAG_4K | GDT_FLAG_PRESENT | GDT_FLAG_RING3); // Ring 3 code segment
	
	gdt_setEntry(5, (uint32_t)gdt_tss, sizeof(gdt_tss), GDT_FLAG_TSS | GDT_FLAG_PRESENT | GDT_FLAG_RING3);
	
	if(commitImmediately)
	{
		gdt_commitEntries();
	}
}

void gdt_setTSSEntry(uint32_t entry)
{
	gdt_tss[1] = entry;
}
