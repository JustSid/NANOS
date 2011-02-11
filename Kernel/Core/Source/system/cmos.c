//
//  cmos.c
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

#include "cmos.h"
#include "port.h"

uint8_t cmos_readData(uint8_t offset)
{
	uint8_t temp = inb(0x70);
	outb(0x70, (temp & 0x80) | (offset & 0x7F));
	return inb(0x71);
}

void cmos_setData(uint8_t offset, uint8_t data)
{
	uint8_t temp = inb(0x70);
	outb(0x70, (temp & 0x80) | (offset & 0x7F));
	outb(0x71, data);
}

void cmos_setRTCFlags(uint8_t flags)
{
	cmos_setData(CMOS_REGISTER_STATEB, flags);
}

void cmos_appendRTCFlags(uint8_t flags)
{
	uint8_t data = cmos_readData(CMOS_REGISTER_STATEB);
	data |= flags;
	cmos_setData(CMOS_REGISTER_STATEB, data);
}

void cmos_removeRTCFlags(uint8_t flags)
{
	uint8_t data = cmos_readData(CMOS_REGISTER_STATEB);
	data &= ~flags;
	cmos_setData(CMOS_REGISTER_STATEB, data);
}
