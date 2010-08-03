//
//  console.c
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

#include "console.h"

static int cn_screenPos_x = 0;
static int cn_screenPos_y = 0;
static bool cn_cursorVisible = true;

char cn_textColor = VIDEO_COLOR_TEXT;

void cn_setTextColor(char color)
{
	cn_textColor = color;
}

void cn_cls()
{
	vd_clearScreen();
	cn_screenPos_x = 0;
	cn_screenPos_y = 0;
}

void cn_setCursor(bool visible)
{
	cn_cursorVisible = visible;
	
	if(cn_cursorVisible == false)
	{
		vd_setCursor(cn_screenPos_x, cn_screenPos_y + VIDEO_SIZE_Y);
	}
	else 
	{
		vd_setCursor(cn_screenPos_x, cn_screenPos_y);
	}

}

void cn_scrollLine()
{
	int	i = 0;
	int	j = (1 * VIDEO_SIZE_X + 0) * 2;
	
	char *video_memory = (char *)0xb8000;
	
	while(j < (cn_screenPos_y * VIDEO_SIZE_X + VIDEO_SIZE_X) * 2)
	{
		video_memory[i] = video_memory[j];
		video_memory[i+1] = video_memory[j+1];
		
		i += 2;
	}
	
	cn_screenPos_y -= 1;
}

void cn_putc(char c)
{
	if(cn_screenPos_x >= VIDEO_SIZE_X)
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
	
	if(cn_screenPos_y >= VIDEO_SIZE_Y)
	{
		cn_cls();
	}
	
	vd_setChar(cn_screenPos_x, cn_screenPos_y, c);
	vd_setAttribute(cn_screenPos_x, cn_screenPos_y, cn_textColor);
	
	cn_screenPos_x += 1;
}

void cn_puts(char *string)
{
	int i = 0;
	while(string[i] != '\0')
	{
		cn_putc(string[i]);
		i++;
	}
	
	vd_setCursor(cn_screenPos_x, cn_screenPos_y);
}

void cn_printn(unsigned long x, int base)
{
	if(x == 0)
	{
		cn_putc('0');
		return;
	}
	
	const char	*digits = "0123456789abcdefghijklmnopqrstuvwxyz";
	char		*p;
	char		buf[65];
	
	if(base > 36)
		return;
	
	p = buf + 64;
	*p = '\0';
	
	while(x)
	{
		*--p = digits[x % base];
		x /= base;
	}
	
	cn_puts(p);
}

void cn_printf(const char *format, ...)
{
	va_list param;
	va_start(param, format);
	
	int i=0;
	while(*format != '\0')
	{
		if(*format == '%')
		{
			format++;
			i++;
			
			switch(*format)
			{
				case 'c':
				{
					char temp = va_arg(param, int);
					cn_putc(temp);
					break;
				}
					
				case 's':
				{
					char *temp = va_arg(param, char *);
					cn_puts(temp);
					break;
				}
					
				case 'i':
				{
					unsigned long temp = va_arg(param, unsigned long);
					cn_printn(temp, 10);
					break;
				}
					
				case 'p':
				{
					unsigned long temp = va_arg(param, unsigned long);
					cn_puts("0x");
					cn_printn(temp, 16);
					break;
				}
					
				case '%':
					cn_putc('%');
					break;
					
				default:
					cn_putc('%');
					cn_putc(*format);
					break;
			}
		} 
		else 
		{
			cn_putc(*format);
		}
		
		format++;
		i++;
	}
	
	
	vd_setCursor(cn_screenPos_x, cn_screenPos_y);
	va_end(param);
}
