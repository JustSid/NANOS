//
//  idt.h
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

#ifndef _IDT_H_
#define _IDT_H_

#include "stdbool.h"
#include "stdint.h"

#define IDT_FLAG_INTERRUPT_GATE		0xe
#define IDT_FLAG_PRESENT			0x80
#define IDT_FLAG_RING0				0x00
#define IDT_FLAG_RING3				0x60

#define IDT_ENTRIES					256

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Sets an given IDT entry
 **/
extern void idt_setEntry(int index, void (*fn)(), unsigned int selector, int flags);
	
/**
 * Commits all current IDT entries. Similia to gdt_commitEntries, but for the IDT
 **/
extern void idt_commitEntries();

/**
 * Initializes the PIC
 **/
extern void idt_picInit();
	
/**
 * Loads all default IDT entries, initializes the pic and commits the entries, depending on commitImmediately
 **/
extern void idt_defaults(bool commitImmediately);

#ifdef __cplusplus
}
#endif
	
#endif