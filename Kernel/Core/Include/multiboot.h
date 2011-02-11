//
//  multiboot.h
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

#ifndef _MULTIBOOT_H_
#define _MULTIBOOT_H_

#include "stdint.h"

struct multiboot_info 
{
    uint32_t    mbs_flags;
    uint32_t    mbs_mem_lower;
    uint32_t    mbs_mem_upper;
    uint32_t    mbs_bootdevice;
    uint32_t    mbs_cmdline;
    uint32_t    mbs_mods_count;
    void		*mbs_mods_addr;
    uint32_t    mbs_syms[4];
    uint32_t    mbs_mmap_length;
    void		*mbs_mmap_addr;
} __attribute__((packed));

struct multiboot_mmap 
{
    uint32_t    entry_size;
    uint64_t    base;
    uint64_t    length;
    uint32_t    type;
} __attribute__((packed));

struct multiboot_moduleb
{
	void		*mod_start;
	void		*mod_end;
	void		*mod_name;
	uint32_t	reserved;
} __attribute__((packed));

#endif