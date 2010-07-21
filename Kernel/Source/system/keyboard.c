//
//  keyboard.c
//  NANOS
//
//  Created by Muffel
//  Copyright © 2010 by Muffel
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


int in_ansi = 0;
char ansibuffer[256];

#ifdef _SCANBUFFER_
// stores the last inputs
int in_scan = 0;
char scanbuffer[16];
#endif

// flags whether  capslock etc. are checked // aslo for the keyboard LEDS not supported
char lockflags;


void InsertToANSIBuffer( char ch )  // untested
{
	int i;
	if( ansibuffer[in_ansi - 1] == '\n' )
	{
		// if the last input was enter
		// clear the buffer
		for( i=0 ; i < 256 ; i++ )
		{
			ansibuffer[i] = 0;
		}
	}
	
	if(in_ansi < 256)
	{
		ansibuffer[in_ansi] = ch;
		in_ansi++;
	}
	else
	{
		for( i=0 ; i < 255 ; i++ )
		{
			ansibuffer[ i ] = ansibuffer[ i + 1 ];
		}
		ansibuffer[256] = ch;
	}
}

// is a escaped sequenz is found
char escaped = 0;
// which keys are pressed SCHIFT NUMLOCK STRG ALT
char shift = 0;
#define SHIFT 0x01
#define NUM 0x02
#define ALT_GR 0x0c
#define STRG 0x04
#define ALT 0x08

void DefaultScan( unsigned char scanned)
{
	unsigned char letter;
	if( shift & SHIFT)
	{
		// look in the tabelle with capslock
		letter = kbl_de_cap[scanned];
		if( letter != 0 ) // no special key function
		{
			// how to work with the characters insert your stuff
			cn_putc( (signed char)letter ); // put it into the console
			InsertToANSIBuffer( letter );  // put it into the ANSII buffer
		}
	}
	else
	{
		letter = kbl_de[scanned];	if( letter != 0 ) // no special key function
		{
			// how to work with the characters insert your stuff
			cn_putc( (signed char)letter ); 
			InsertToANSIBuffer( letter );
		}
	}
	
}




ir_cpu_state *keyboard_callback(uint32_t interrupt, ir_cpu_state *state)
{
	unsigned char scancode;
	// get the scancode
	scancode = inb(0x60);	
	
	if(scancode == 0xe0) // if there is a escaped sequence next byte has a diffrent function
	{
		escaped = 1; 
		cn_puts("escaped\n"); // testing how to deal it has to be implemented
	}
	else
	{
		if(escaped == 0) // if its not escaped
		{
			if(scancode & 0x80)  // if the key is released
			{
				
				switch(scancode)
				{
					case 0xaa:
						//cn_puts("l small break\n");
						shift &= ~SHIFT;
						break;
					case 0xb6:
						//cn_puts("r small break\n");
						shift &= ~SHIFT;
						break;
					case 0x9d:
						shift &= ~STRG;
						break;
					case 0xb8:
						shift &= ~ALT;
						break;
					default:
						//cn_puts("break\n");
						// normale breaks nicht behandeln
						break;
				};
			}
			else
			{
				// if the key is pressed
				switch(scancode)
				{
					case 0x2a:
						//cn_puts("l big make\n");
						shift |= SHIFT;
						break;
					case 0x36:
						shift |= SHIFT;
						//cn_puts("r big make\n");
						break;
					case 0x1d:
						shift |= STRG;
						break;
					case 0x38:
						shift |= ALT;
						break;
					default:
						//cn_puts("make\n");
						DefaultScan(scancode);
						break;
				};
			}
		}
		else
		{
			// what to do if the last was escaped
			escaped = 0;
		}
	}
	
	
	outb(0x20, 0x20);  // preparing for next interupt  necassary??
	return state;
	
}

// to call in the main function
void init_keyboard()
{
	
	cn_puts( "Init Keyboard..." );
	char command;
	// wait for free input
	
	// set to interupt handler ( IRQ1 )
	ir_addInterrupt(0x21, 0x21, keyboard_callback);
	
	// wait for free input
	while((inb(0x64)&2)!=0){}
	// change scancode command
	command = 0xf0;
	outb( 0x64 , command );
	
	// wait for free input
	while((inb(0x64)&2)!=0){}
	// parameter scancode 2
	command = 0x02;
	outb( 0x60 , command );
	
	cn_puts( "[OK] \n" );
}

char GetLastInput()// untested
{
	if(in_ansi >= 256) return 0;
	return ansibuffer[in_ansi];
}