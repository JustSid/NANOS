//
//  keymap.h
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


// keyboard layout deutsch no shift/caps_lock
unsigned char kbl_de[128] = {
	0 , // error code 0	
	0 , // escape		01
	'1', // 1 !		02	
	'2', // 1 !03
	'3', // 1 !04
	'4', // 1 !05
	'5', // 1 !06
	'6', // 1 !07
	'7', // 1 !08
	'8', // 1 !09
	'9', // 1 !0a
	'0', // 1 !0b
	223, // 1 !0c
	39, // ' hochkomata '` 0d
	'\b',// Backspace 0e
	'\t',// tab 0f
	'q', // q 10
	'w', // q 11
	'e', // q 12
	'r', // q 13
	't', // q 14
	'z', // q 15
	'u', // q 16
	'i', // q 17
	'o', // q 18
	'p', // q 19
	252, // q 1a
	'+', // q 1b
	'\n',// return 1c   e0 für Ziffernblock
	0 ,  // strg left 1d  e0 für rechts
	'a', // q  30 1e 	
	's', // q 1f
	'd', // q 20 e0 für mute
	'f', // q 21
	'g', // q 22 e0 play/pause
	'h', // q 23 
	'j', // q 24 e0 stopdruck
	'k', // q 25
	'l', // q 26
	246, // q 27
	228, // q 28
	'^', // q 29
	0 , // shift left 2a
	'#', // # ' 2b
	'y', // q 2c
	'x', // q 2d 
	'c', // q 2e e0 leiser
	'v', // q 2f
	'b', // q 30 e0 lauter
	'n', // q 31
	'm', // q 32 e0 www
	',', // q 33
	'.', // q 34
	'-', // q 35 e0 / numpad
	0 , // shift right 36
	'*', //	37 e0 druck
	0 , // alt left e0 rechts 38
	32, // leertaste  39  3a 01
	0 , // CAPS-LOCK / Feststell taste
	0 , // f1- f10 3b
	0 , // f1- f10 3c
	0 , // f1- f10 3d
	0 , // f1- f10 3e
	0 , // f1- f10 3f
	0 , // f1- f10 40
	0 , // f1- f10 41
	0 , // f1- f10 42
	0 , // f1- f10 43
	0 , // f1- f10 44
	0 , // num lock 45
	0 , //   scroll lock  46 e0 ctrl+ break
	0 , //   7 numpad 47 e0 pos1
	0 , //   8 numpad 48 e0 hoch
	0 , //   9 numpad 49 e0 bildhoch
	'-', //   - numpad 4a 
	0 , //   4 numpad 4b e0 links
	0 , //   5 numpad 4c 
	0 , //   6 numpad 4d e0 rechts
	'+', //   + numpad 4e 
	0 , //   1 numpad 4f e0 ende
	0 , //   2 numpad 50 e0 runter
	0 , //   3 numpad 51 e0 bild runter
	0 , //   0 numpad 52 einfg
	0 , //entf numpad 53 entf
	0 , // ALT + sysRq 54
	0 , // 56
	0 , // 57  f11
	0 , // 58  f12
	0 , // 59
	0 , // 5a
	0 , // 5b
	0 , // 5c rest undefined
};

// keyboard layout deutsch with shift/caps_lock
unsigned char kbl_de_cap[128] = { 
	0 , // error code 0	
	0 , // escape		01
	'!', // 1 !		02	
	'"', // 1 !03
	'§', // 1 !04
	'$', // 1 !05
	'%', // 1 !06
	'&', // 1 !07
	'/', // 1 !08
	'(', // 1 !09
	')', // 1 !0a
	'=', // 1 !0b
	'?', // 1 !0c
	'`', // ' hochkomata '` 0d
	'\b',// Backspace 0e
	'\t',// tab 0f
	'Q', // q 10
	'W', // q 11
	'E', // q 12
	'R', // q 13
	'T', // q 14
	'Z', // q 15
	'U', // q 16
	'I', // q 17
	'O', // q 18
	'P', // q 19
	220, // q 1a
	'*', // q 1b
	'\n',// return 1c   e0 für Ziffernblock
	0 ,  // strg left 1d  e0 für rechts
	'A', // q  30 1e 	
	'S', // q 1f
	'D', // q 20 e0 für mute
	'F', // q 21
	'G', // q 22 e0 play/pause
	'H', // q 23 
	'J', // q 24 e0 stopdruck
	'K', // q 25
	'L', // q 26
	212, // q 27
	196, // q 28
	'o', // q 29
	0 , // shift left 2a
	'>', // # ' 2b
	'Y', // q 2c
	'X', // q 2d 
	'C', // q 2e e0 leiser
	'V', // q 2f
	'B', // q 30 e0 lauter
	'N', // q 31
	'M', // q 32 e0 www
	';', // q 33
	':', // q 34
	'_', // q 35 e0 / numpad
	0 , // shift right 36
	'*', //	37 e0 druck
	0 , // alt left e0 rechts 38
	32, // leertaste  39  3a 01
	0 , // CAPS-LOCK / Feststell taste
	0 , // f1- f10 3b
	0 , // f1- f10 3c
	0 , // f1- f10 3d
	0 , // f1- f10 3e
	0 , // f1- f10 3f
	0 , // f1- f10 40
	0 , // f1- f10 41
	0 , // f1- f10 42
	0 , // f1- f10 43
	0 , // f1- f10 44
	0 , // num lock 45
	0 , //   scroll lock  46 e0 ctrl+ break
	'7', //   7 numpad 47 e0 pos1
	'8', //   8 numpad 48 e0 hoch
	'9', //   9 numpad 49 e0 bildhoch
	'-', //   - numpad 4a 
	'4', //   4 numpad 4b e0 links
	'5', //   5 numpad 4c 
	'6', //   6 numpad 4d e0 rechts
	'+', //   + numpad 4e 
	'1', //   1 numpad 4f e0 ende
	'2', //   2 numpad 50 e0 runter
	'3', //   3 numpad 51 e0 bild runter
	'0', //   0 numpad 52 einfg
	',', //entf numpad 53 entf
	0 , // ALT + sysRq 54
	0 , // 56
	0 , // 57  f11
	0 , // 58  f12
	0 , // 59
	0 , // 5a
	0 , // 5b
	0 , // 5c rest undefined
};
