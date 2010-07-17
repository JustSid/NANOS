//
//  interrupt.S
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

.extern ir_handleInterrupt

.macro idt_stub nr
.global idt_stub_\nr
idt_stub_\nr:
    pushl $0
    pushl $\nr
    jmp ir_preHandler
.endm

.macro idt_stub_error_code nr
.global idt_stub_\nr
idt_stub_\nr:
    pushl $\nr
    jmp ir_preHandler
.endm

idt_stub 0
idt_stub 1
idt_stub 2
idt_stub 3
idt_stub 4
idt_stub 5
idt_stub 6
idt_stub 7
idt_stub_error_code 8
idt_stub 9
idt_stub_error_code 10
idt_stub_error_code 11
idt_stub_error_code 12
idt_stub_error_code 13
idt_stub_error_code 14
idt_stub 15
idt_stub 16
idt_stub_error_code 17
idt_stub 18

idt_stub 32
idt_stub 33

idt_stub 48

ir_preHandler:
	push %ebp
    push %edi
    push %esi
    push %edx
    push %ecx
    push %ebx
    push %eax

    push %esp
    call ir_handleInterrupt
    mov %eax, %esp

    pop %eax
    pop %ebx
    pop %ecx
    pop %edx
    pop %esi
    pop %edi
    pop %ebp

    add $8, %esp
    iret
