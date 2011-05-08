//
//  keyboard.c
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

#include "keyboard.h"
#include "keymap.h"
#include "string.h"

#include <memory/memory.h>
#include <system/syscall.h>

static unsigned char	kb_lastCharacter = '\0';
static kb_callback_map *kb_first_callback = NULL;

kb_callback_map *kb_callbackForPid(uint32_t pid)
{
	kb_callback_map *map = kb_first_callback;
	while(map)
	{
		if(map->pid == pid)
			return map;
		
		map = map->next;
	}
	
	return NULL;
}


void kb_addKeyboardHook(uint32_t pid, void *buffer, size_t bufferSize)
{
	kb_callback_map *map = kb_callbackForPid(pid);
	if(map)
	{
		// Update the old callback map
		map->buffer = (char *)buffer;
		map->size = bufferSize;
		map->index = 0;
		
		memset(map->buffer, 0, map->size);
		return;
	}
	
	
	map = (kb_callback_map *)mm_heapAlloc(mm_kernelHeap(), sizeof(kb_callback_map));
	if(map)
	{
		if(!kb_first_callback)
		{
			kb_first_callback = map;
		}
		else 
		{
			kb_callback_map *temp = kb_first_callback;
			while(temp)
			{
				if(!temp->next)
					temp->next = map;
				
				temp = temp->next;
			}
		}
		
		
		map->pid = pid;
		map->buffer = (char *)buffer;
		map->size = bufferSize;
		map->index = 0;
		map->next = NULL;
		
		memset(map->buffer, 0, map->size);
	}
}

void kb_removeKeyboardHook(uint32_t pid)
{
	kb_callback_map *map = kb_first_callback;
	kb_callback_map *lmap = NULL;
	
	while(map)
	{
		if(map->pid == pid)
		{
			if(!lmap)
			{
				kb_first_callback = map->next;
			}
			else 
			{
				lmap->next = map->next;
			}
			
			
			mm_heapFree(mm_kernelHeap(), map);
			break;
		}
		
		lmap = map;
		map = map->next;
	}
}

void kb_clearKeyboardHook(uint32_t pid)
{
	kb_callback_map *map = kb_callbackForPid(pid);
	if(map)
	{
		map->index = 0;
		memset(map->buffer, 0, map->size);
	}
}

void kb_keyDown(unsigned char key)
{
	if(kb_first_callback)
	{
		kb_callback_map *map = kb_first_callback;
		while(map)
		{
			if(map->index < map->size)
			{				
				map->buffer[map->index] = key;
				map->index ++;
			}
			
			map = map->next;
		}
	}
}
