
SECTIONS {
    . = 0x80000;

    .text ALIGN(0x10) : { *(.text) }
    .data ALIGN(0x10) : { *(.data) }

    .bss ALIGN(0x10) : 
    {
        _bss_start = .;
        *(.bss)
        _bss_end = .;
    }
}
