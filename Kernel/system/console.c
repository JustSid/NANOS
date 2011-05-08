//
//  console.c
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


#include "console.h"

#include "stdio.h"
#include "stdint.h"
#include "system/string.h"
#include "interrupt.h"
#include "syscall.h"

#include <video/video.h>
#include <scheduler/scheduler.h>

static uint8_t cn_screenPos_x = 0;
static uint8_t cn_screenPos_y = 0;
static bool cn_cursorVisible = true;

char cn_textColor = VD_COLOR_LGRAY;

void cn_setTextColor(char color)
{
	cn_textColor = color;
}

void cn_cls()
{
	vd_cls();
	cn_screenPos_x = 0;
	cn_screenPos_y = 0;
}

void cn_setCursor(bool visible)
{
	cn_cursorVisible = visible;
	vd_setCursor(cn_screenPos_x, cn_screenPos_y + ((cn_cursorVisible) ? 0 : VD_SIZE_Y));
}


void cn_scrollLine()
{
	vd_scrollLine();
	
	cn_screenPos_y --;
	cn_screenPos_x = 0;
}

void cn_putc(char c)
{
	if(cn_screenPos_x >= VD_SIZE_X)
	{
		cn_screenPos_x = 0;
		cn_screenPos_y += 1;
	}
	
	if(c == '\n')
	{
		cn_screenPos_x = 0;
		cn_screenPos_y += 1;
		return;
	}
	
	if(cn_screenPos_y >= VD_SIZE_Y)
		cn_scrollLine();
	
	vd_setChar(cn_screenPos_x, cn_screenPos_y, c);
	vd_setAttribute(cn_screenPos_x, cn_screenPos_y, cn_textColor);
	
	cn_screenPos_x += 1;
	
	if(cn_cursorVisible)
		vd_setCursor(cn_screenPos_x, cn_screenPos_y);
}

void cn_puts(char *string)
{
	int i = 0;
	while(string[i] != '\0')
	{
		cn_putc(string[i]);
		i++;
	}
}

void cn_printf(const char *format, ...)
{
	va_list param;
	va_start(param, format);
	
	char temp[1024];
	vsprintf(temp, format, param);
	
	cn_puts(temp);
	va_end(param);
}

void cn_delchar()
{
	if(cn_screenPos_x > 0)
	{
		cn_screenPos_x --;
	}
	else 
	{
		cn_screenPos_x = 0;
		
		if(cn_screenPos_y > 0)
			cn_screenPos_y --;
	}
	
	vd_setChar(cn_screenPos_x, cn_screenPos_y, ' ');
	vd_setAttribute(cn_screenPos_x, cn_screenPos_y, cn_textColor);
}
