ENTRY(_start)

SECTIONS {
    . = 0xb0cd;
    .text : {
        *(.text)
    }
    .rodata : {
        *(.rodata)
    }
    .data : {
        *(.data)
    }
    .bss : {
        *(.bss)
        *(COMMON)
    }
    .pgtables : ALIGN(4096) {
        *(.pgtables)
   	}
}
