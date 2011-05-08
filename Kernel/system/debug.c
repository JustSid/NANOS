//
//  debug.c
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

#include "stdio.h"
#include "stdint.h"
#include "debug.h"
#include "panic.h"
#include "string.h"
#include "console.h"

static DB_LOGGING_LEVEL __loggingLevel = DB_LEVEL_INFORMATION;

void db_setLoggingLevel(DB_LOGGING_LEVEL level)
{
	__loggingLevel = level;
}

DB_LOGGING_LEVEL db_getLoggingLevel()
{
	return __loggingLevel;
}


char *db_lastPathComponent(char *path)
{
	uint32_t i = strlen(path);
	while(i > 0)
	{
		if(path[i] == '\\' || path[i] == '/')
			return &path[i];
		
		i--;
	}
	
	return path;
}

void db_verboseLog(DB_LOGGING_LEVEL level, char *file, int line, char *format, ...)
{
	if(__loggingLevel > DB_LEVEL_NONE && __loggingLevel >= level)
	{
		va_list args;
		va_start(args, format);
		
		char temp[1024];
		vsprintf(temp, format, args);
		
		va_end(args);
		
		
		if(level == DB_LEVEL_PANIC)
		{
			panic("%s:%i: %s", db_lastPathComponent(file), line, temp);
			return; // Unneeded but anyway...
		}
		
		
		char temp2[1024];
		sprintf(temp2, "%s:%i: %s", db_lastPathComponent(file), line, temp);
		cn_puts(temp2);
	}
}
