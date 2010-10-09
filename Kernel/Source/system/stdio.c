//
//  stdio.c
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

#include "stdio.h"

char _buffer[64];

static inline char *strfmtint(unsigned long val, int base)
{
	if(base > 36)
		return "";
	
	if(val == 0)
		return "0";
	
	const char	*digits = "0123456789abcdefghijklmnopqrstuvwxyz";
	char		*p = _buffer + 64;
	
	int i = 0;
	*p = '\0';
	
	while(val)
	{
		*--p = digits[val % base];
		val /= base;
		
		i++;
	}
	
	return p;
}


int vsprintf(char *str, const char *format, va_list arg)
{
	int written = 0;
	
	while(*format != '\0')
	{
		if(*format == '%')
		{
			format ++;
			
			switch(*format)
			{
				case 'c':
				{
					char temp = va_arg(arg, int);
					str[written] = temp;
				}
					break;
					
				case 's':
				{
					char *string = va_arg(arg, char *);
					
					while(*string != '\0')
					{
						str[written] = *string;
						
						written ++;
						string ++;
					}
					
					str[written] = ' ';
					written --;
				}
					break;
					
					
				case 'i':
				{
					unsigned long	temp = va_arg(arg, unsigned long);
					char			*string = strfmtint(temp, 10);
					
					while(*string != '\0')
					{
						str[written] = *string;
						
						written ++;
						string ++;
					}
					
					str[written] = ' ';
					written --;
				}
					break;
					
					
				case 'p':
				{
					unsigned long	temp = (unsigned long)va_arg(arg, void *);
					char			*string = strfmtint(temp, 16);
					
					str[written] = '0';
					written ++;
					
					str[written] = 'x';
					written ++;
					
					while(*string != '\0')
					{
						str[written] = *string;
						
						written ++;
						string ++;
					}
					
					str[written] = ' ';
					written --;
				}
					break;
					
				case 'x':
				{
					unsigned long	temp = va_arg(arg, unsigned long);
					char			*string = strfmtint(temp, 16);
					
					while(*string != '\0')
					{
						str[written] = *string;
						
						written ++;
						string ++;
					}
					
					str[written] = ' ';
					written --;
					
				}
					break;
					
				case '%':
					str[written] = '%';					
					break;
					
				default:
					str[written] = '%';
					written ++;
					
					str[written] = *format;
					break;
			}
		}
		else 
		{
			str[written] = *format;
		}
		
		
		written ++;
		format ++;
	}
	
	str[written] = '\0';	
	return written;
}

int sprintf(char *dst, const char *format, ...)
{
	va_list param;
	va_start(param, format);
	
	int written = vsprintf(dst, format, param);
	
	va_end(param);
	
	return written;
}

