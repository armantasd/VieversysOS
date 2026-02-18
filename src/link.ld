ENTRY(_start)

VIR_OFF = 0xFFFFFFFFFFDF5000;

SECTIONS {
    . = 0xb0d0;	
    .boottext : {
        *(.boottext)
    }
    .gdt : {
        *(.gdt)
    }
    . += 0xFFFFFFFFFFDF5000;
    .text : AT(ADDR(.text) - 0xFFFFFFFFFFDF5000){
        *(.text)
    }
    .rodata : AT(ADDR(.rodata) - 0xFFFFFFFFFFDF5000){
        *(.rodata)
    }
    .data : AT(ADDR(.data) - 0xFFFFFFFFFFDF5000){
        *(.data)
    }
    .bss : AT(ADDR(.bss) - 0xFFFFFFFFFFDF5000){
        *(.bss)
        *(COMMON)
    }
}
