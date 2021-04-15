SECTIONS {
    . = 0x80000;

    .text ALIGN(0x10) : { *(.text) }
    .data ALIGN(0x10) : { *(.data) }

    .bss ALIGN(0x10) : 
    {
        _bss_start = .;
        *(.bss);
        *(COMMON);
        _bss_end = .;
    }

    .bootloader ALIGN(0X10) : 
    {
        _bootloader_start = ALIGN(0X10);
        *(.bootloader);
        _bootloader_end = ALIGN(0X10);
    }
}
