//
//  video.h
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

#ifndef _VIDEO_H_
#define _VIDEO_H_

/* The default video size, 80x25 characters */
#define VD_SIZE_X 80
#define VD_SIZE_Y 25

/* The default video colors */
#define VD_COLOR_BLACK		0x0
#define VD_COLOR_BLUE		0x1
#define VD_COLOR_GREEN		0x2
#define VD_COLOR_CYAN		0x3
#define VD_COLOR_RED		0x4
#define VD_COLOR_MAGENTA	0x5
#define VD_COLOR_BROWN		0x6
#define VD_COLOR_LGRAY		0x7
#define VD_COLOR_DGRAY		0x8
#define VD_COLOR_LBLUE		0x9
#define VD_COLOR_LGREEN		0xA
#define VD_COLOR_LCYAN		0xB
#define VD_COLOR_LRED		0xC
#define VD_COLOR_LMAGENTA	0xD
#define VD_COLOR_YELLOW		0xE
#define VD_COLOR_WHITE		0xF

/**
 * Clears the complete screen
 **/
extern void vd_cls();

extern void vd_setChar(int x, int y, char c);
extern void vd_setAttribute(int x, int y, char a);

extern void vd_scrollLine();

extern void vd_writeString(int startX, int startY, char *string, char color);
extern void vd_setCursor(int x, int y);

#endif
