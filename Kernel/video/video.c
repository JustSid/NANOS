//
//  video.c
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

#include "video.h"
#include <system/port.h>
#include <system/stdint.h>

char *vd_videoMemory = (char *)0xb8000;

void vd_cls()
{
	int i;
	for(i=0; i<(VD_SIZE_X * VD_SIZE_Y) * 2; i++)
		vd_videoMemory[i] = (char)0;
}

void vd_setChar(int x, int y, char c)
{
	int offset = (y * VD_SIZE_X + x) * 2;
	vd_videoMemory[offset] = c;
}

void vd_setAttribute(int x, int y, char a)
{
	int offset = (y * VD_SIZE_X + x) * 2;
	vd_videoMemory[offset+1] = a;
}

void vd_copyLine(int dst, int src)
{
	int i;
	for(i=0; i<VD_SIZE_X; i++)
	{
		int offsetDst = (dst * VD_SIZE_X + i) * 2;
		int offsetSrc = (src * VD_SIZE_X + i) * 2;
		
		vd_videoMemory[offsetDst] = vd_videoMemory[offsetSrc];
		vd_videoMemory[offsetDst + 1] = vd_videoMemory[offsetSrc + 1];
	}
}

void vd_scrollLine()
{
	int i;
	for(i=0; i<VD_SIZE_Y-1; i++)
	{
		vd_copyLine(i, i+1);
	}
	
	for(i=0; i<VD_SIZE_X; i++)
	{
		int offset = ((VD_SIZE_Y - 1) * VD_SIZE_X + i) * 2;
		vd_videoMemory[offset] = 0;
		vd_videoMemory[offset + 1] = 0;
	}
}

void vd_writeString(int startX, int startY, char *string, char color)
{
	int i = 0;
	int x = startX;
	int y = startY;
	
	while(string[i] != '\0')
	{
		vd_setChar(x, y, string[i]);
		vd_setAttribute(x, y, color);		
		
		x++;
		i++;
		
		if(x >= VD_SIZE_X)
		{
			x = 0;
			y++;
		}
	}
}

void vd_setCursor(int x, int y)
{
	uint16_t pos = (y * VD_SIZE_X) + x;
	
	outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
  
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char )((pos >> 8) & 0xFF));
}
