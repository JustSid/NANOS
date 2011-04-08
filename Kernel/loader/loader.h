//
//  loader.h
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

#ifndef _LOADER_H_
#define _LOADER_H_

#include <boot/multiboot.h>

/**
 @file loader.h
 @brief Defines structs which represent format independent images as well as functions to load binaries of different formats.
 *
 * The main purpose of this file is to get an abstraction layer between executable binaries with a whatsoever format and the scheduler.
 * The loading of the binaries from eg. disk, memory or whatever isn't the purpose of this module, but the loading of the actual data into the intermediate format that can be executed
 * directly by the scheduler.
 **/

/**
 * Type used as entry point into binaries
 **/
typedef void (*ld_entry)();

/**
 * The type of an image
 **/
typedef enum
{
	/**
	 * Used to represent a binary in an unknown format
	 **/
	ld_imageTypeUnknown,
	
	/**
	 * Used to represent a binary in the ELF (x86) format.
	 **/
	ld_imageTypeELF
} ld_imageType;

/**
 * Struct used to represent a loaded binary that can be directly executed
 **/
typedef struct ld_image
{
	/**
	 * Name of the image.
	 **/
	const char *name;
	void *payload; // Reserved for future use
	
	/**
	 * The entry point of the image. Must be set to something tht can be directly jumped into by eg. calling ld_image->entry();
	 **/
	ld_entry entry;
	/**
	 * The type of the image.
	 **/
	ld_imageType type;
	
	/**
	 * The next element in the linked list.
	 **/
	struct ld_image *next;
} ld_image;



/**
 * Returns the first entry of the linked list which name matches the given one.
 @param name The name of the image
 @return Pointer to the image or NULL if no image could be found.
 **/
extern ld_image *ld_imageWithName(const char *name);

/**
 * Loads an ELF binary.
 @param elfBinary Pointer to an memory area of kind struct elf_header.
 @param name The name of the new image, must not be NULL
 @return The newly created image or NULL if the binary or the name was invalid (or something else went wrong).
 **/
extern ld_image *ld_createELFImage(void *elfBinary, const char *name);

/**
 * Loads a multiboot module
 @param module Pointer to a valid multiboot module
 @return The newly created image or NULL if the module was invalid.
 **/
extern ld_image *ld_createMultibootModuleImage(struct multiboot_module *module);

#endif
