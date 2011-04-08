//
//  keymap.c
//  NANOS
//
//  Created by Muffel
//  Copyright (c) 2011 by Muffel
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

#include <memory/memory.h>

#include "keymap.h"
#include "string.h"
#include "stdint.h"
#include "console.h"

static km_layout *km_current_layout = NULL;

km_layout *km_createLayout(const char *name)
{
	km_layout *temp = (km_layout *)mm_alloc(sizeof(km_layout));
	
	if(temp)
	{
		temp->name = (char *)mm_alloc((strlen(name) + 1) * sizeof(char));
		if(!temp->name)
		{
			mm_free(temp);
			return NULL;
		}
		
		strcpy(temp->name, name);
		
		int i;
		for(i=0; i<128; i++)
		{
			temp->layout_noCaps[i]	= 0;
			temp->layout_caps[i]	= 0;
		}
	}
	
	return temp;
}

km_layout *km_copy(km_layout *layout)
{
	km_layout *temp = km_createLayout(layout->name);
	
	if(temp)
	{
		int i;
		for(i=0; i<128; i++)
		{
			temp->layout_noCaps[i] = layout->layout_noCaps[i];
			temp->layout_caps[i] = layout->layout_caps[i];
		}
	}
	
	return temp;
}

void km_setLayout(km_layout *layout)
{
	if(km_current_layout)
	{
		mm_free(km_current_layout->name);
		mm_free(km_current_layout);
	}
	
	km_current_layout = km_copy(layout);
}

km_layout *km_currentLayout()
{
	return km_current_layout;
}
