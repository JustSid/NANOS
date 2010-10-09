ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    kernelBegin = .;

    .text : {
        *(multiboot)
        *(.text)
    }
    .data ALIGN(4096) : {
        *(.data)
    }
    .rodata ALIGN(4096) : {
        *(.rodata)
    }
    .bss ALIGN(4096) : {
        *(.bss)
    }

    . = ALIGN(4096);
    kernelEnd = .;
}

