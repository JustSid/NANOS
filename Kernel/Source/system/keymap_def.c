//
//  keymap_def.c
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

#include "keymap_def.h"
#include "stdint.h"
#include "console.h"

static km_layout *layout_deDE = NULL;

void km_create_deDE()
{
	layout_deDE = km_createLayout("deDE");
	if(layout_deDE)
	{
		layout_deDE->layout_noCaps[0] = 0;
		layout_deDE->layout_noCaps[1] = 0;
		layout_deDE->layout_noCaps[2] = '1';
		layout_deDE->layout_noCaps[3] = '2';
		layout_deDE->layout_noCaps[4] = '3';
		layout_deDE->layout_noCaps[5] = '4';
		layout_deDE->layout_noCaps[6] = '5';
		layout_deDE->layout_noCaps[7] = '6';
		layout_deDE->layout_noCaps[8] = '7';
		layout_deDE->layout_noCaps[9] = '8';
		layout_deDE->layout_noCaps[10] = '9';
		layout_deDE->layout_noCaps[11] = '0';
		layout_deDE->layout_noCaps[12] = 223;
		layout_deDE->layout_noCaps[13] = 39;
		layout_deDE->layout_noCaps[14] = '\b';
		layout_deDE->layout_noCaps[15] = '\t';
		layout_deDE->layout_noCaps[16] = 'q';
		layout_deDE->layout_noCaps[17] = 'w';
		layout_deDE->layout_noCaps[18] = 'e';
		layout_deDE->layout_noCaps[19] = 'r';
		layout_deDE->layout_noCaps[20] = 't';
		layout_deDE->layout_noCaps[21] = 'z';
		layout_deDE->layout_noCaps[22] = 'u';
		layout_deDE->layout_noCaps[23] = 'i';
		layout_deDE->layout_noCaps[24] = 'o';
		layout_deDE->layout_noCaps[25] = 'p';
		layout_deDE->layout_noCaps[26] = 25;
		layout_deDE->layout_noCaps[27] = '+';
		layout_deDE->layout_noCaps[28] = '\n';
		layout_deDE->layout_noCaps[29] = 0;
		layout_deDE->layout_noCaps[30] = 'a';
		layout_deDE->layout_noCaps[31] = 's';
		layout_deDE->layout_noCaps[32] = 'd';
		layout_deDE->layout_noCaps[33] = 'f';
		layout_deDE->layout_noCaps[34] = 'g';
		layout_deDE->layout_noCaps[35] = 'h';
		layout_deDE->layout_noCaps[36] = 'j';
		layout_deDE->layout_noCaps[37] = 'k';
		layout_deDE->layout_noCaps[38] = 'l';
		layout_deDE->layout_noCaps[39] = 246;
		layout_deDE->layout_noCaps[40] = 228;
		layout_deDE->layout_noCaps[41] = '^';
		layout_deDE->layout_noCaps[42] = 0;
		layout_deDE->layout_noCaps[43] = '#';
		layout_deDE->layout_noCaps[44] = 'y';
		layout_deDE->layout_noCaps[45] = 'x';
		layout_deDE->layout_noCaps[46] = 'c';
		layout_deDE->layout_noCaps[47] = 'v';
		layout_deDE->layout_noCaps[48] = 'b';
		layout_deDE->layout_noCaps[49] = 'n';
		layout_deDE->layout_noCaps[50] = 'm';
		layout_deDE->layout_noCaps[51] = ',';
		layout_deDE->layout_noCaps[52] = '.';
		layout_deDE->layout_noCaps[53] = '-';
		layout_deDE->layout_noCaps[54] = 0;
		layout_deDE->layout_noCaps[56] = '*';
		layout_deDE->layout_noCaps[57] = 0;
		layout_deDE->layout_noCaps[58] = ' ';
		layout_deDE->layout_noCaps[59] = 0;
		layout_deDE->layout_noCaps[60] = 0;
		layout_deDE->layout_noCaps[61] = 0;
		layout_deDE->layout_noCaps[62] = 0;
		layout_deDE->layout_noCaps[63] = 0;
		layout_deDE->layout_noCaps[64] = 0;
		layout_deDE->layout_noCaps[65] = 0;
		layout_deDE->layout_noCaps[66] = 0;
		layout_deDE->layout_noCaps[67] = 0;
		layout_deDE->layout_noCaps[68] = 0;
		layout_deDE->layout_noCaps[69] = 0;
		layout_deDE->layout_noCaps[70] = 0;
		layout_deDE->layout_noCaps[71] = 0;
		layout_deDE->layout_noCaps[72] = 0;
		layout_deDE->layout_noCaps[73] = 0;
		layout_deDE->layout_noCaps[74] = 0;
		layout_deDE->layout_noCaps[75] = '-';
		layout_deDE->layout_noCaps[76] = 0;
		layout_deDE->layout_noCaps[77] = 0;
		layout_deDE->layout_noCaps[78] = 0;
		layout_deDE->layout_noCaps[79] = '+';
		layout_deDE->layout_noCaps[80] = 0;
		layout_deDE->layout_noCaps[81] = 0;
		layout_deDE->layout_noCaps[82] = 0;
		layout_deDE->layout_noCaps[83] = 0;
		layout_deDE->layout_noCaps[84] = 0;
		layout_deDE->layout_noCaps[85] = 0;
		layout_deDE->layout_noCaps[86] = 0;
		layout_deDE->layout_noCaps[87] = 0;
		layout_deDE->layout_noCaps[88] = 0;
		layout_deDE->layout_noCaps[89] = 0;
		layout_deDE->layout_noCaps[90] = 0;
		layout_deDE->layout_noCaps[91] = 0;
		layout_deDE->layout_noCaps[92] = 0;
		layout_deDE->layout_noCaps[93] = 0;
		layout_deDE->layout_noCaps[94] = 0;
		layout_deDE->layout_noCaps[95] = 0;
		layout_deDE->layout_noCaps[96] = 0;
		layout_deDE->layout_noCaps[97] = 0;
		layout_deDE->layout_noCaps[98] = 0;
		layout_deDE->layout_noCaps[99] = 0;
		layout_deDE->layout_noCaps[100] = 0;
		layout_deDE->layout_noCaps[101] = 0;
		layout_deDE->layout_noCaps[102] = 0;
		layout_deDE->layout_noCaps[103] = 0;
		layout_deDE->layout_noCaps[104] = 0;
		layout_deDE->layout_noCaps[105] = 0;
		layout_deDE->layout_noCaps[106] = 0;
		layout_deDE->layout_noCaps[107] = 0;
		layout_deDE->layout_noCaps[108] = 0;
		layout_deDE->layout_noCaps[109] = 0;
		layout_deDE->layout_noCaps[110] = 0;
		layout_deDE->layout_noCaps[111] = 0;
		layout_deDE->layout_noCaps[112] = 0;
		layout_deDE->layout_noCaps[113] = 0;
		layout_deDE->layout_noCaps[114] = 0;
		layout_deDE->layout_noCaps[115] = 0;
		layout_deDE->layout_noCaps[116] = 0;
		layout_deDE->layout_noCaps[117] = 0;
		layout_deDE->layout_noCaps[118] = 0;
		layout_deDE->layout_noCaps[119] = 0;
		layout_deDE->layout_noCaps[120] = 0;
		layout_deDE->layout_noCaps[121] = 0;
		layout_deDE->layout_noCaps[122] = 0;
		layout_deDE->layout_noCaps[123] = 0;
		layout_deDE->layout_noCaps[124] = 0;
		layout_deDE->layout_noCaps[125] = 0;
		layout_deDE->layout_noCaps[126] = 0;
		layout_deDE->layout_noCaps[127] = 0;
		
		// Caps
		layout_deDE->layout_caps[0] = 0;
		layout_deDE->layout_caps[1] = 0;
		layout_deDE->layout_caps[2] = '!';
		layout_deDE->layout_caps[3] = '"';
		layout_deDE->layout_caps[4] = '§';
		layout_deDE->layout_caps[5] = '$';
		layout_deDE->layout_caps[6] = '%';
		layout_deDE->layout_caps[7] = '&';
		layout_deDE->layout_caps[8] = '/';
		layout_deDE->layout_caps[9] = '(';
		layout_deDE->layout_caps[10] = ')';
		layout_deDE->layout_caps[11] = '=';
		layout_deDE->layout_caps[12] = '?';
		layout_deDE->layout_caps[13] = '`';
		layout_deDE->layout_caps[14] = '\b';
		layout_deDE->layout_caps[15] = '\t';
		layout_deDE->layout_caps[16] = 'Q';
		layout_deDE->layout_caps[17] = 'W';
		layout_deDE->layout_caps[18] = 'E';
		layout_deDE->layout_caps[19] = 'R';
		layout_deDE->layout_caps[20] = 'T';
		layout_deDE->layout_caps[21] = 'Z';
		layout_deDE->layout_caps[22] = 'U';
		layout_deDE->layout_caps[23] = 'I';
		layout_deDE->layout_caps[24] = 'O';
		layout_deDE->layout_caps[25] = 'P';
		layout_deDE->layout_caps[26] = 220;
		layout_deDE->layout_caps[27] = '*';
		layout_deDE->layout_caps[28] = '\n';
		layout_deDE->layout_caps[29] = 0;
		layout_deDE->layout_caps[30] = 'A';
		layout_deDE->layout_caps[31] = 'S';
		layout_deDE->layout_caps[32] = 'D';
		layout_deDE->layout_caps[33] = 'F';
		layout_deDE->layout_caps[34] = 'G';
		layout_deDE->layout_caps[35] = 'H';
		layout_deDE->layout_caps[36] = 'J';
		layout_deDE->layout_caps[37] = 'K';
		layout_deDE->layout_caps[38] = 'L';
		layout_deDE->layout_caps[39] = 212;
		layout_deDE->layout_caps[40] = 196;
		layout_deDE->layout_caps[41] = '°';
		layout_deDE->layout_caps[42] = 0;
		layout_deDE->layout_caps[43] = '>';
		layout_deDE->layout_caps[44] = 'Y';
		layout_deDE->layout_caps[45] = 'X';
		layout_deDE->layout_caps[46] = 'C';
		layout_deDE->layout_caps[47] = 'V';
		layout_deDE->layout_caps[48] = 'B';
		layout_deDE->layout_caps[49] = 'N';
		layout_deDE->layout_caps[50] = 'M';
		layout_deDE->layout_caps[51] = ';';
		layout_deDE->layout_caps[52] = ':';
		layout_deDE->layout_caps[53] = '_';
		layout_deDE->layout_caps[54] = 0;
		layout_deDE->layout_caps[56] = '*';
		layout_deDE->layout_caps[57] = ' ';
		layout_deDE->layout_caps[58] = ' ';
		layout_deDE->layout_caps[59] = 0;
		layout_deDE->layout_caps[60] = 0;
		layout_deDE->layout_caps[61] = 0;
		layout_deDE->layout_caps[62] = 0;
		layout_deDE->layout_caps[63] = 0;
		layout_deDE->layout_caps[64] = 0;
		layout_deDE->layout_caps[65] = 0;
		layout_deDE->layout_caps[66] = 0;
		layout_deDE->layout_caps[67] = 0;
		layout_deDE->layout_caps[68] = 0;
		layout_deDE->layout_caps[69] = 0;
		layout_deDE->layout_caps[70] = 0;
		layout_deDE->layout_caps[71] = 0;
		layout_deDE->layout_caps[72] = '7';
		layout_deDE->layout_caps[73] = '8';
		layout_deDE->layout_caps[74] = '9';
		layout_deDE->layout_caps[75] = '-';
		layout_deDE->layout_caps[76] = '4';
		layout_deDE->layout_caps[77] = '5';
		layout_deDE->layout_caps[78] = '6';
		layout_deDE->layout_caps[79] = '+';
		layout_deDE->layout_caps[80] = '1';
		layout_deDE->layout_caps[81] = '2';
		layout_deDE->layout_caps[82] = '3';
		layout_deDE->layout_caps[83] = '0';
		layout_deDE->layout_caps[84] = ',';
		layout_deDE->layout_caps[85] = 0;
		layout_deDE->layout_caps[86] = 0;
		layout_deDE->layout_caps[87] = 0;
		layout_deDE->layout_caps[88] = 0;
		layout_deDE->layout_caps[89] = 0;
		layout_deDE->layout_caps[90] = 0;
		layout_deDE->layout_caps[91] = 0;
		layout_deDE->layout_caps[92] = 0;
		layout_deDE->layout_caps[93] = 0;
		layout_deDE->layout_caps[94] = 0;
		layout_deDE->layout_caps[95] = 0;
		layout_deDE->layout_caps[96] = 0;
		layout_deDE->layout_caps[97] = 0;
		layout_deDE->layout_caps[98] = 0;
		layout_deDE->layout_caps[99] = 0;
		layout_deDE->layout_caps[100] = 0;
		layout_deDE->layout_caps[101] = 0;
		layout_deDE->layout_caps[102] = 0;
		layout_deDE->layout_caps[103] = 0;
		layout_deDE->layout_caps[104] = 0;
		layout_deDE->layout_caps[105] = 0;
		layout_deDE->layout_caps[106] = 0;
		layout_deDE->layout_caps[107] = 0;
		layout_deDE->layout_caps[108] = 0;
		layout_deDE->layout_caps[109] = 0;
		layout_deDE->layout_caps[110] = 0;
		layout_deDE->layout_caps[111] = 0;
		layout_deDE->layout_caps[112] = 0;
		layout_deDE->layout_caps[113] = 0;
		layout_deDE->layout_caps[114] = 0;
		layout_deDE->layout_caps[115] = 0;
		layout_deDE->layout_caps[116] = 0;
		layout_deDE->layout_caps[117] = 0;
		layout_deDE->layout_caps[118] = 0;
		layout_deDE->layout_caps[119] = 0;
		layout_deDE->layout_caps[120] = 0;
		layout_deDE->layout_caps[121] = 0;
		layout_deDE->layout_caps[122] = 0;
		layout_deDE->layout_caps[123] = 0;
		layout_deDE->layout_caps[124] = 0;
		layout_deDE->layout_caps[125] = 0;
		layout_deDE->layout_caps[126] = 0;
		layout_deDE->layout_caps[127] = 0;
	}
}


//static km_layout *layout_enEN = NULL;

void km_init()
{
	cn_puts("Initializing keyboards...");
	
	km_create_deDE();
	km_setLayout(layout_deDE);
	
	cn_puts("ok\n");
}
