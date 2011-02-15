//
//  keyboard.c
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

#include "keyboard.h"
#include "memory.h"
#include "keymap.h"

static unsigned char	kb_lastCharacter = '\0';
static kb_callback_map *kb_first_callback = NULL;

kb_callback_map *kb_addKeyboardHook()
{
	kb_callback_map *temp = (kb_callback_map *)mm_alloc(sizeof(kb_callback_map));
	
	if(temp)
	{
		if(!kb_first_callback)
		{
			kb_first_callback = temp;
		}
		else 
		{
			kb_callback_map *map = kb_first_callback;
			
			while(map->next)
			{
				map = map->next;
			}
			
			map->next = temp;
		}
		
		temp->key_down = NULL;
		temp->next = NULL;
	}
	
	return temp;
}

void kb_removeKeyboardHook(kb_callback_map *rmap)
{
	kb_callback_map *map = kb_first_callback;
	kb_callback_map *lmap = NULL;
	
	while(map)
	{
		if(map == rmap)
		{
			if(!lmap)
			{
				kb_first_callback = map->next;
				mm_free(map);
				
				return;
			}
			else 
			{
				rmap->next = map->next;
				mm_free(map);
				
				return;
			}
		}
		
		lmap = map;
		map = map->next;
	}
}

void kb_keyDown(unsigned char key)
{
	if(kb_first_callback)
	{
		kb_callback_map *map = kb_first_callback;
		while(map)
		{
			if(map->key_down)
			{
				map->key_down(key);
			}
			
			map = map->next;
		}
	}
}