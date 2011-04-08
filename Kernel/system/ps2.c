//
//  ps2.c
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

#include "ps2.h"
#include "keymap.h"
#include "stdbool.h"
#include "console.h"

static bool		ps_escaped = false;
static uint8_t	ps_keyModifier = 0;

void ps_scanKey(unsigned char scanned)
{
	unsigned char	letter;
	km_layout		*layout = km_currentLayout();
	
	if(!layout)
		return;
	
	if(ps_keyModifier & PS_KEY_SHIFT)
	{
		
		letter = layout->layout_caps[scanned];
		if(letter != 0)
		{
			kb_keyDown(letter);
		}
	}
	else
	{
		letter = layout->layout_noCaps[scanned];	
		if(letter != 0)
		{
			kb_keyDown(letter);
		}
	}
}

ir_cpuState *ps_ir_callback(uint32_t interrupt, ir_cpuState *state)
{
	unsigned char scancode = inb(0x60);
	
	if(scancode == 0xe0)
	{
		ps_escaped = true;
	}
	else 
	{
		if(!ps_escaped)
		{
			if(scancode & 0x80) 
			{
				// Key down
				switch(scancode)
				{
					case 0xaa:
						ps_keyModifier &= ~PS_KEY_SHIFT;
						break;
					case 0xb6:
						ps_keyModifier &= ~PS_KEY_SHIFT;
						break;
					case 0x9d:
						ps_keyModifier &= ~PS_KEY_STRG;
						break;
					case 0xb8:
						ps_keyModifier &= ~PS_KEY_ALT;
						break;
					default:
						break;
				};
			}
			else
			{
				// Key up
				switch(scancode)
				{
					case 0x2a:
						ps_keyModifier |= PS_KEY_SHIFT;
						break;
					case 0x36:
						ps_keyModifier |= PS_KEY_SHIFT;
						break;
					case 0x1d:
						ps_keyModifier |= PS_KEY_STRG;
						break;
					case 0x38:
						ps_keyModifier |= PS_KEY_ALT;
						break;
					default:
						ps_scanKey(scancode);
						break;
				};
			}
		}
		else 
		{
			ps_escaped = false;
		}
	}

	outb(0x20, 0x20);
	return state;
}

void ps_init()
{
	cn_puts("Initializing PS/2 controller...");
	ir_installInterruptHandler(ps_ir_callback, 0x21, 0x21);
    
	while((inb(0x64) & 2) != 0) {}
	outb(0x64, 0xf0);
	
	while((inb(0x64) & 2) != 0) {}
	outb(0x60 , 0x02);
	
	cn_puts("ok\n");
}
