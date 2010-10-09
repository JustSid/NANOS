//
//  ps2.h
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

#ifndef _PS2_H_
#define _PS2_H_

#include "stdint.h"
#include "interrupt.h"
#include "keymap.h"
#include "keyboard.h"
#include "port.h"

#define PS_KEY_SCROLL_LOCK	0x01
#define PS_KEY_NUM_LOCK		0x02
#define PS_KEY_CAPS_LOCK	0x04

#define PS_KEY_SHIFT	0x01
#define PS_KEY_NUM		0x02
#define PS_KEY_ALT_GR	0x0c
#define PS_KEY_STRG		0x04
#define PS_KEY_ALT		0x08

#define _SCANBUFFER_

void ps_init();

#endif
