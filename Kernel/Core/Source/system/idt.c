//
//  idt.c
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

#include "idt.h"
#include "port.h"

static long long unsigned int idt_table[IDT_ENTRIES];

void idt_setEntry(int index, void (*fn)(), unsigned int selector, int flags)
{
	if(index < 0 || index >= IDT_ENTRIES)
		return;
	
    unsigned long int handler = (unsigned long int)fn;
	
    idt_table[index] = handler			& 0xffffLL;
    idt_table[index] |= (selector		& 0xffffLL)	<< 16;
    idt_table[index] |= (flags			& 0xffLL)	<< 40;
    idt_table[index] |= ((handler>> 16) & 0xffffLL)	<< 48;
}

void idt_commitEntries()
{
	struct 
	{
        unsigned short int	limit;
        void				*pointer;
    } __attribute__((packed)) 
	
	idtp = 
	{
        .limit		= IDT_ENTRIES * 8 - 1,
        .pointer	= idt_table,
    };
	
	__asm__ volatile("lidt %0" : : "m" (idtp));
    __asm__ volatile("sti");
}

void idt_picInit()
{
	// Master
    outb(0x20, 0x11); 
    outb(0x21, 0x20); 
    outb(0x21, 0x04); 
    outb(0x21, 0x01);
	
	// Slave
    outb(0xa0, 0x11);
    outb(0xa1, 0x28); 
    outb(0xa1, 0x02);
    outb(0xa1, 0x01);
	
    outb(0x20, 0x0);
    outb(0xa0, 0x0);
}


extern void idt_stub_0();
extern void idt_stub_1();
extern void idt_stub_2();
extern void idt_stub_3();
extern void idt_stub_4();
extern void idt_stub_5();
extern void idt_stub_6();
extern void idt_stub_7();
extern void idt_stub_8();
extern void idt_stub_9();
extern void idt_stub_10();
extern void idt_stub_11();
extern void idt_stub_12();
extern void idt_stub_13();
extern void idt_stub_14();
extern void idt_stub_15();
extern void idt_stub_16();
extern void idt_stub_17();
extern void idt_stub_18();

extern void idt_stub_32();
extern void idt_stub_33();
extern void idt_stub_34();
extern void idt_stub_35();
extern void idt_stub_36();
extern void idt_stub_37();
extern void idt_stub_38();
extern void idt_stub_39();
extern void idt_stub_40();
extern void idt_stub_41();
extern void idt_stub_42();
extern void idt_stub_43();
extern void idt_stub_44();
extern void idt_stub_45();
extern void idt_stub_46();
extern void idt_stub_47();

extern void idt_stub_48();

void idt_defaults(bool commitImmediately)
{
    idt_picInit();
	
    idt_setEntry(0,  idt_stub_0,  0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(1,  idt_stub_1,  0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(2,  idt_stub_2,  0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(3,  idt_stub_3,  0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING3 | IDT_FLAG_PRESENT);
    idt_setEntry(4,  idt_stub_4,  0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(5,  idt_stub_5,  0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(6,  idt_stub_6,  0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(7,  idt_stub_7,  0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(8,  idt_stub_8,  0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(9,  idt_stub_9,  0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(10, idt_stub_10, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(11, idt_stub_11, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(12, idt_stub_12, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(13, idt_stub_13, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(14, idt_stub_14, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(15, idt_stub_15, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(16, idt_stub_16, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(17, idt_stub_17, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(18, idt_stub_18, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	
    idt_setEntry(32, idt_stub_32, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
    idt_setEntry(33, idt_stub_33, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_setEntry(34, idt_stub_34, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_setEntry(35, idt_stub_35, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_setEntry(36, idt_stub_36, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_setEntry(37, idt_stub_37, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_setEntry(38, idt_stub_38, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_setEntry(39, idt_stub_39, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_setEntry(40, idt_stub_40, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_setEntry(41, idt_stub_41, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_setEntry(42, idt_stub_42, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_setEntry(43, idt_stub_43, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_setEntry(44, idt_stub_44, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_setEntry(45, idt_stub_45, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_setEntry(46, idt_stub_46, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_setEntry(47, idt_stub_47, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	
    idt_setEntry(48, idt_stub_48, 0x8, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING3 | IDT_FLAG_PRESENT);
	
	if(commitImmediately)
	{
		idt_commitEntries();
	}
}
