ENTRY(_start)                  /* Define entry point explicitly */

SECTIONS
{
    . = 0x1000;                /* Load address */

    .text : {
        *(.text)               /* All code sections */
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(COMMON)
        *(.bss)
    }
}
