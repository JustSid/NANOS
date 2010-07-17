//
//  video.c
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

#include "video.h"

char *vd_videoMem = (char *)0xb8000;

void vd_clearScreen()
{
	int i;
	for(i=0; i<(VIDEO_SIZE_X * VIDEO_SIZE_Y) * 2; i++)
	{
		vd_videoMem[i] = (char)0;
	}
}

void vd_setAttribute(int x, int y, char attribute)
{
	if(x <= VIDEO_SIZE_X && y < VIDEO_SIZE_Y)
	{
		int offset = (y * VIDEO_SIZE_X + x) * 2;
		vd_videoMem[offset+1] = attribute;
	}
}

void vd_setChar(int x, int y, char c)
{
	if(x <= VIDEO_SIZE_X && y < VIDEO_SIZE_Y)
	{
		int offset = (y * VIDEO_SIZE_X + x) * 2;
		vd_videoMem[offset] = c;
	}
}

void vd_setCursor(int x, int y)
{
	uint16_t pos = (y * VIDEO_SIZE_X) + x;
	
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos >> 8);
    
    outb(0x3D4, 0x0E);
    outb(0x3D5, pos);
}
