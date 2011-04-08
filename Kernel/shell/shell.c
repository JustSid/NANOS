//
//  shell.c
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


#include <loader/loader.h>
#include <scheduler/scheduler.h>

#include <system/stdio.h>
#include <system/string.h>
#include <system/console.h>
#include <system/keyboard.h>
#include <memory/memory.h>
#include <system/system.h>
#include <system/syscall.h>
#include <system/console.h>

static char sh_lastKey = ' ';
static kb_callback_map *sh_callback = NULL;

static int  sh_index = 0;
static char sh_input[255];

void keydown(unsigned char input)
{
	if(input == '\b')
	{
		if(sh_index > 0)
		{
			cn_delchar();
			
			sh_index --;	
			sh_input[sh_index] = '\0';
			
			sh_lastKey = ' ';
		}
		else 
		{
			sh_input[0]	= '\0';			
			sh_lastKey  = ' ';
		}

		//cn_printf("%i: %s\n", sh_index, sh_input);
		return;
	}
	
	
	if(sh_index < 254)
	{
		cn_printf("%c", input);
		sh_lastKey = (char)input;
		
		sh_input[sh_index] = input;
		sh_input[sh_index + 1] = '\0';
		sh_index ++;
	}
	else 
	{
		sh_lastKey = '\n';
	}
}

void shell_main()
{
	int i;
	sd_setName("Mishelle");
	sd_setIdentifier("krn.nanos.mishelle");
	
	for(i=0; i<80; i++)
		cn_printf("-");
	
	cn_printf("NANOS shell\nDon't panic!\nType \"help\" for a list of commands\n\n");
	sh_callback = kb_addKeyboardHook();
	sh_callback->key_down = keydown;
	
	while(1)
	{
		sh_input[0]	= '\0';
		sh_index	= 0;
		
		
		sh_lastKey = ' ';
		cn_printf("> ");
		
		while(sh_lastKey != '\n') 
		{
			syscall(sys_sleep); // Give CPU time back to the Kernel
		}
		
		sh_input[sh_index-1] = '\0';
		
		
		ld_image *image = ld_imageWithName(sh_input);
		if(image)
		{
			uint32_t pid = sd_spawnProcess(image->entry);
			
			while(sd_processWithPid(pid) != NULL)
				syscall(sys_sleep);
		}
		else if(strcmp(sh_input, "cls") == 0)
		{
			cn_cls();
			continue;
		}
		else if(strcmp(sh_input, "help") == 0)
		{
			cn_printf("help    - Displays this message\n");
			cn_printf("cls     - Clears the screen\n");
			cn_printf("mem     - Prints the memory fragmentation\n");
			cn_printf("tasks   - Prints the current process list\n");
			cn_printf("verbose - Toggles the verbosity of the system on or off\n");
			cn_printf("echo    - Prints the given message on the screen\n");
			cn_printf("exit    - Exits the shell\n");
		}
		else if(strcmp(sh_input, "exit") == 0)
		{
			kb_removeKeyboardHook(sh_callback);
			sh_callback = NULL;
			
			return;
		}
		else if(strcmp(sh_input, "tasks") == 0)
		{
			sd_printProcesses();
		}
		else if(strcmp(sh_input, "mem") == 0)
		{
			//mm_dumpMemory();
		}
		else if(strcmp(sh_input, "verbose") == 0)
		{
			st_setVerbosity(!st_isVerbose());
		}
		else if(strncmp(sh_input, "echo ", 5) == 0)
		{
			char *output = &sh_input[5];
			cn_printf("   %s", output);
		}
		else
			cn_printf("   Unknown command %s", sh_input);

		
		cn_putc('\n');	
	}
}