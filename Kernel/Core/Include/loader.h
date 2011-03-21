//
//  loader.h
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

#ifndef _LOADER_H_
#define _LOADER_H_

#include "multiboot.h"

typedef void (*ld_entry)();

typedef struct ld_image
{
	const char *name;
	ld_entry entry;
	
	struct ld_image *next;
} ld_image;

// Returns the first image with the given name, or NULL
extern ld_image *ld_imageWithName(const char *name);

extern ld_image *ld_loadELFImage(void *image, const char *name);
extern ld_image *ld_loadMultibootModule(struct multiboot_module *module);

#endif
