//
//  loader.c
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

#include "console.h"
#include "loader.h"
#include "memory.h"
#include "string.h"
#include "ELF.h"

static ld_image *firstImage = NULL;

ld_image *ld_loadELFImage(void *image, const char *name)
{
	if(!image || !name)
		return NULL;
	
	ld_image *img = mm_alloc(sizeof(ld_image));
	
	if(img)
	{
		elf_header *header = (elf_header *)image;
		elf_program_header *pheader;
		
		if(header->magic != ELF_MAGIC)
		{
			mm_free(img);
			return NULL;
		}
		
		
		img->name = (const char *)mm_alloc((strlen(name) + 1) * sizeof(char));
		if(!img->name)
		{
			mm_free(img);
			return NULL;
		}
		
		strcpy((char *)img->name, name);		
		pheader = (elf_program_header *)(((char *)image) + header->ph_offset);
		
		int i;
		for(i=0; i<header->ph_entry_count; i++, pheader++) 
		{
			void *dest = (void *)pheader->virt_addr;
			void *src = ((char *)image) + pheader->offset;
			
			if(pheader->type != ELF_LOAD) 
				continue;
			
			memset(dest, 0, pheader->mem_size);
			memcpy(dest, src, pheader->file_size);
		}
		
		img->entry = (ld_entry)header->entry;
		img->next = NULL;
		
		if(firstImage)
		{
			ld_image *__image = firstImage;
			while(__image)
			{
				if(!__image->next)
				{
					__image->next = img;
					break;
				}
				
				__image = __image->next;
			}
		}
		else 
			firstImage = img;
	}
	
	return img;
}

ld_image *ld_loadMultibootModule(struct multiboot_module *module)
{
	ld_image *image = NULL;
	
	char *name = module->mod_name;
	name = (strlen(name) <= 0) ? "anonymous" : name;
	
	cn_printf("Loading module \"%s\"...", name);
	image = ld_loadELFImage(module->mod_start, (const char *)name);
	
	cn_printf("%s\n", (image) ? "ok" : "failed");
	return image;
}



ld_image *ld_imageWithName(const char *name)
{
	ld_image *image = firstImage;
	
	while(image)
	{
		if(strcmp((char *)image->name, (char *)name) == 0)
			return image;
		
		image = image->next;
	}
	
	return NULL;
}
