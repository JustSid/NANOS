//
//  string.c
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

#include "string.h"

void *memcpy(void *dst0, const void *src0, size_t size)
{
	char *dst = (char *)dst0;
	char *src = (char *)src0;
	
	size_t i;
	for(i=0; i<size; i++)
		dst[i] = src[i];
	
	return dst0;
}

int memcmp(const void *src0, const void *src1, size_t size)
{
	char *_src1 = (char *)src0;
	char *_src2 = (char *)src1;
	
	size_t i;
	for(i=0; i<size; i++)
		if(_src1[i] != _src2[i]) return 1;
	
	return 0;
}

void *memset(void *dst0, int val, size_t size)
{
	char *dst = (char *)dst0;
	
	size_t i;
	for(i=0; i<size; i++)
		dst[i] = val;
	
	return dst0;
}


int strcmp(char *str1, char *str2)
{
	size_t i=0;
	while(str1[i] != '\0' || str2[i] != '\0')
	{
		if(str1[i] > str2[i])
		{
			return 1;
		}
		else if(str1[i] < str2[i])
		{
			return -1;
		}
		
		i++;
	}
	
	return 0;
}

int strncmp(char *str1, char *str2, size_t len)
{
	size_t i;
	for(i=0; i<len; i++)
	{
		if(str1[i] > str2[i])
		{
			return 1;
		}
		else if(str1[i] < str2[i])
		{
			return -1;
		}
	}
	
	return 0;
}


size_t strlen(const char *str)
{
	size_t i;
	for(i=0; str[i] != '\0'; i++)
	{
		
	}
	
	return i;
}

char *strcpy(char *dst, const char *src)
{
	size_t i;
	for(i=0; src[i] != '\0'; i++)
	{
		dst[i] = src[i];
	}
	
	dst[i] = '\0';
	
	return dst;
}

char *strncpy(char *dst, const char *src, size_t len)
{
	size_t	i;
	bool	copy = true;
	for(i=0; i<len; i++)
	{
		if(src[i] == '\0')
			copy = false;
		
		if(copy)
		{
			dst[i] = src[i];
		}
		else 
		{
			dst[i] = '\0';
		}

	}
	
	return dst;
}

char *strcat(char *dst, const char *src)
{
	size_t i = 0;
	size_t j = 0;
	
	while(dst[i] != '\0') 
	{
		i++;
	}
	
	while(src[j] != '\0')
	{
		dst[i] = src[j];
		
		i++;
		j++;
	}
	
	dst[i+1] = '\0';
	
	return dst;
}

char *strcatc(char *dst, char c)
{
	size_t i = 0;
	
	while(dst[i] != '\0') 
		i++;
	
	dst[i] = c;
	dst[i+1] = '\0';
	
	return dst;
}
