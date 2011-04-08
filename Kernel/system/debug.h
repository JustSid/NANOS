//
//  debug.h
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


#ifndef _DEBUG_H_
#define _DEBUG_H_

typedef enum
{
	DB_LEVEL_NONE,
	
	DB_LEVEL_INFORMATION,
	DB_LEVEL_WARNING,
	DB_LEVEL_ERROR,
	DB_LEVEL_PANIC // Logs the message as syscall
} DB_LOGGING_LEVEL;

extern void db_setLoggingLevel(DB_LOGGING_LEVEL level);
extern DB_LOGGING_LEVEL db_getLoggingLevel();

extern void db_verboseLog(DB_LOGGING_LEVEL level, char *file, int line, char *format, ...);
extern char *db_lastPathComponent(char *path); // Returns the last path component. eg: "/foo/bar/baz.file" -> "baz.file". Works with Windows and UNIX style paths
// Ownership note: The returned pointer points to path + offset!

#define db_log(...) db_verboseLog(DB_LEVEL_INFORMATION, __FILE__, __LINE__, __VA_ARGS__)
#define db_logLevel(level, ...) db_verboseLog(level, __FILE__, __LINE__, __VA_ARGS__)

#define db_conditionalLog(condition, ...) ((condition) ? db_verboseLog(DB_LEVEL_INFORMATION, __FILE__, __LINE__, __VA_ARGS__) : ((void) 0))
#define db_conditionalLogLevel(level, condition, ...) ((condition) ? db_verboseLog(level, __FILE__, __LINE__, __VA_ARGS__) : ((void) 0))

#endif
