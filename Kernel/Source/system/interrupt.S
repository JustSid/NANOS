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

// There are two types of interrupts, the one which set a error number and the one which doesn't set one

// This get's called if the CPU doesn't sets a error number
.macro idt_stub nr
.global idt_stub_\nr
idt_stub_\nr:
    pushl $0 // We set 0 as the error number. This makes the handling easier cause we don't need to differentiate between the interrupts that set a error number and the one which don't
    pushl $\nr
    jmp ir_preHandler // we call the pre handler
.endm

.macro idt_stub_error_code nr
.global idt_stub_\nr
idt_stub_\nr:
    pushl $\nr // This was a interrupt with an error number. We don't need to set one
    jmp ir_preHandler // let the pre handler to the rest
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

idt_stub 32 // Timer
idt_stub 33 // PS/2 1
idt_stub 34 // Cascade
idt_stub 35 // RS-232
idt_stub 36 // RS 232
idt_stub 37 // LPT 2
idt_stub 38 // Floppy 
idt_stub 39 // LPT 1
idt_stub 40 // Real Time Clock
idt_stub 41 // 
idt_stub 42 // IDE/ATAPI/ATA
idt_stub 43 // IDE/ATAPI/ATA
idt_stub 44 // PS/2 2
idt_stub 45 // FPU
idt_stub 46 // primary IDE/ATAPI/ATA
idt_stub 47 // secondary IDE/ATAPI/ATA

idt_stub 48


// !---- Pre Handler ---- ! //
// The pre handler gets called before our high level interrupt handler gets called. So we don't falsify the CPU state
ir_preHandler:
	// Push the current CPU state
	push %ebp
    push %edi
    push %esi
    push %edx
    push %ecx
    push %ebx
    push %eax

	// Reload the kernel data segment
	mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es

    push %esp 
    call ir_handleInterrupt // Calls our high-level handler 
    mov %eax, %esp // Switch the stack. We have switched the CPU state context, so we need to switch the stack too.

	// Reloads the user data segment
	mov $0x23, %ax
    mov %ax, %ds
    mov %ax, %es

	// Pop the state
    pop %eax
    pop %ebx
    pop %ecx
    pop %edx
    pop %esi
    pop %edi
    pop %ebp

    add $8, %esp // Removes the error number from the stack.
    iret
