//
//  stdlib.c
//  libkernel
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

#include "stdlib.h"
#include "syscall.h"

void *malloc(size_t size)
{
	void *result = (void *)syscall1(sys_alloc, (uint32_t)size);
	return result;
}

void *realloc(void *ptr, size_t size)
{
	void *result = (void *)syscall2(sys_realloc, (uint32_t)ptr, (uint32_t)size);
	return result;
}

void free(void *ptr)
{
	syscall1(sys_free, (uint32_t)ptr);
}


int puts(const char *string)
{
	syscall1(sys_print, (uint32_t)string);
	return 0; // HACK!
}


// Start stub
extern int main(int argc, char *argv[]);

void _start()
{
	// TODO: Gather the default arguments from somewhere
	main(0, NULL);
	// TODO: Do something with the returned value!
}
