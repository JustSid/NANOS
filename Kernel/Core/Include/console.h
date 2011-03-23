//
//  console.h
//  NANOS
//
//  Created by Sidney Just
//  Copyright (c) 2010 by Sidney Just
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

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "video.h"
#include "stdarg.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Sets the text color for all following texts
 @param color The new color for the text, values are described in video.h
 @default VD_COLOR_LGRAY
 **/
extern void cn_setTextColor(char color);

/**
 * Clears all attributes and texts from the screen
 **/
extern void cn_cls();

/**
 * Writes the given character at the position of the cursor onto the screen
 **/
extern void cn_putc(char c);

/**
 * Writes the given null terminated CString at the position of the cursor onto the screen
 **/
extern void cn_puts(char *string);

/**
 * Prints the string onto the screen by first evaluating the format string + parameters and then using syscall(sys_print, string) to print the string onto the screen.
 **/
extern void cn_printf(const char *format, ...);

/**
 * Scrolls one line up
 @remark Buggy!
 **/
extern void cn_scrollLine();
	
/**
 * Sets the cursor visibility
 @param visible true if the cursor should be visible, otherwise false
 **/
extern void cn_setCursor(bool visible);
extern void cn_updateCursor();

/**
 * Deletes the character at the cursor position and shifts the cursor one character back
 **/
extern void cn_delchar();

#ifdef __cplusplus
}
#endif
	
#endif


