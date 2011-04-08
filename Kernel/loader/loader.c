//
//  loader.c
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

#include "ELF.h"
#include "loader.h"

#include <system/string.h>
#include <system/console.h>
#include <memory/memory.h>

static ld_image *ld_firstImage = NULL;

// -------------------------------
// Helper functions
// -------------------------------

/**
 * Creates a void image with the given name
 **/
ld_image *ld_createVoidImage(const char *name)
{
	if(!name)
		return NULL;
	
	
	ld_image *image = mm_alloc(sizeof(ld_image));
	if(image)
	{
		image->name = (const char *)mm_alloc((strlen(name) + 1) * sizeof(char));
		if(!image->name)
		{
			mm_free(image);
			return NULL;
		}
		
		strcpy((char *)image->name, (char *)name);
		
		image->type		= ld_imageTypeUnknown;
		image->entry	= NULL;
		image->next		= NULL;
	}
	
	return image;
}


/**
 * Inserts a image into the linked list
 **/
void ld_insertImage(ld_image *image)
{
	if(!image)
		return;
	
	if(ld_firstImage)
	{
		ld_image *cimage = ld_firstImage;
		while(cimage)
		{
			if(cimage == image)
				return;
			
			if(!cimage->next)
			{
				cimage->next = image;
				break;
			}
			
			cimage = cimage->next;
		}
	}
	else 
		ld_firstImage = image;
}


ld_image *ld_imageWithName(const char *name)
{
	ld_image *image = ld_firstImage;
	
	while(image)
	{
		if(strcmp((char *)image->name, (char *)name) == 0)
			return image;
		
		image = image->next;
	}
	
	return NULL;
}


// -------------------------------
// ELF binaries
// -------------------------------

/**
 * Returns true if the given ptr points to an ELF binary
 **/
char ld_isELFBinary(void *ptr)
{
	elf_header *header = (elf_header *)ptr;
	return (header->magic == ELF_MAGIC) ? 1 : 0;
}

ld_image *ld_createELFImage(void *elfBinary, const char *name)
{
	if(!elfBinary || !ld_isELFBinary(elfBinary) || !name)
		return NULL;
	
	ld_image *image = ld_createVoidImage(name);
	if(image)
	{
		elf_header *header = (elf_header *)elfBinary;
		elf_program_header *pheader = (elf_program_header *)(((char *)header) + header->ph_offset);
		
		int i;
		for(i=0; i<header->ph_entry_count; i++, pheader++) 
		{
			void *dst = (void *)pheader->virt_addr;
			void *src = ((char *)elfBinary) + pheader->offset;
			
			if(pheader->type != ELF_LOAD) 
				continue;
			
			memset(dst, 0, pheader->mem_size);
			memcpy(dst, src, pheader->file_size); // TODO: Remove the evilness of this operation
		}
		
		image->entry = (ld_entry)header->entry;
		ld_insertImage(image);
	}
}


// -------------------------------
// Multiboot modules
// -------------------------------

ld_image *ld_createMultibootModuleImage(struct multiboot_module *module)
{
	ld_image	*image = NULL;
	char		*name  = module->mod_name;
	void		*start = module->mod_start;
	
	if(!name)
		return NULL;
	
	
	cn_printf("Loading module \"%s\"...", name);
	
	if(ld_isELFBinary(start))
	{
		image = ld_createELFImage(module->mod_start, (const char *)name);
	}
	
	
	cn_printf("%s\n", (image) ? "ok" : "failed");
	return image;
}
