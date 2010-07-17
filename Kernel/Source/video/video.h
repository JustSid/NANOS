//
//  video.h
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

#ifndef _VIDEO_H_
#define _VIDEO_H_

#define VIDEO_SIZE_X 80
#define VIDEO_SIZE_Y 25

#define VIDEO_COLOR_BLACK		0x0
#define VIDEO_COLOR_BLUE		0x1
#define VIDEO_COLOR_GREEN		0x2
#define VIDEO_COLOR_CYAN		0x3
#define VIDEO_COLOR_RED			0x4
#define VIDEO_COLOR_MAGENTA		0x5
#define VIDEO_COLOR_BROWN		0x6
#define VIDEO_COLOR_LGRAY		0x7
#define VIDEO_COLOR_DGRAY		0x8
#define VIDEO_COLOR_LBLUE		0x9
#define VIDEO_COLOR_LGREEN		0xA
#define VIDEO_COLOR_LCYAN		0xB
#define VIDEO_COLOR_LRED		0xC
#define VIDEO_COLOR_LMAGENTA	0xD
#define VIDEO_COLOR_YELLOW		0xE
#define VIDEO_COLOR_WHITE		0xF

#define VIDEO_COLOR_TEXT VIDEO_COLOR_LGRAY

#include "port.h"

extern void vd_clearScreen();
extern void vd_setAttribute(int x, int y, char attribute);
extern void vd_setChar(int x, int y, char c);
extern void vd_setCursor(int x, int y);

#endif

