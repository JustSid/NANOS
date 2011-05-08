//
//  Shell.c
//  Programs
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

#include <libkernel.h>

char keyboardBuffer[64];
char input[256];
uint32_t inputIndex = 0;


int main(int argc, char *argv[])
{
	//setName("Mishelle");
	//setIdentifier("krn.nanos.mishelle");
	
	// Hook the keyboard
	memset(input, 0, 256);
	hookKeyboard(keyboardBuffer, 64);
	

	// Print a vertical line and then the welcome text
	puts("\n");
	
	int i;
	for(i=0; i<80; i++)
		puts("-");
	
	puts("NANOS shell\nDon't panic!\nType \"help\" for a list of commands\n\n");
	puts("> ");	
	
	while(1)
	{
		strncpy(&input[inputIndex], keyboardBuffer, 256-inputIndex); // Write the last key presses into the input buffer
		puts(keyboardBuffer); // And also write the keypresses to the screen
		
		inputIndex += strlen(keyboardBuffer);
		
		
		if(input[inputIndex-1] != '\n')
		{
			sleep();
			continue;
		}
		
		input[inputIndex-1] = '\0'; // Strip the \n out of the input, otherwise the strcmp wouldn't work!
		
		uint32_t pid = processCreate(input);
		if(pid != PID_INVALID)
		{
			// Okay, the input was really the name of an image, lets wait until the program finished
			while(getName(pid) != NULL) // HACK because there is currently no other way to look if a process is alive other than to poll one of its available info
			{
				sleep(); // Give the Kernel the CPU time back
			}
		}
		else if(strcmp(input, "help") == 0)
		{
			// \n\n because of a bug, a single \n at the end isn't recognized
			printf("help    - Displays this message\n\n");
			printf("cls     - Clears the screen\n\n");
			printf("proc    - Prints the current process list\n\n");
			printf("echo    - Prints the given message on the screen\n\n");
			printf("exit    - Exits the shell\n\n");
		}
		else if(strcmp(input, "cls") == 0)
		{
			memset(input, 0, 256);
			memset(keyboardBuffer, 0, 64);
			inputIndex = 0;
			
			cls();
			puts("> ");
			continue; // Avoid the newline
		}
		else if(strcmp(input, "exit") == 0)
		{
			return 0;
		}
		else if(strcmp(input, "proc") == 0)
		{
			// Pid 0 is the kernel task, thus its always a existing process (unless something went very wrong, in which case the output doesn't matter anymore)
			pid = 0;
			do
			{
				char *name, *identifier;
				uint32_t threads, memory;
				
				name		= getName(pid);
				identifier	= getIdentifier(pid);
				
				threads = getThreadCount(pid);
				memory	= getAllocatedMemory(pid);
				
				
				printf("%s (%s). Pid: %i Threads: %i Memory: %ib\n", name, identifier, (int)pid, (int)threads, (int)memory);
				pid = getNextPid(pid);
			} while(pid != PID_INVALID);
		}
		else if(strncmp(input, "echo ", 5) == 0)
		{
			char *output = &input[5];
			printf("   %s", output);
		}
		else
			printf("   Unknown command '%s'", input);

		
		// Avoid that the old input is appended again to the next input
		memset(input, 0, 256);
		memset(keyboardBuffer, 0, 64);
		inputIndex = 0;
		
		puts("\n> ");
	}
	
	return 0;
}
