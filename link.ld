SECTIONS
{
    . = 0x80000;
    .text : { KEEP(*(.text.boot)) *(.text .text.* .gnu.linkonce.t*) }
    .rodata : { *(.rodata .rodata.* .gnu.linkonce.r*) }
    PROVIDE(_data = .);
    .data : { *(.data .data.* .gnu.linkonce.d*) }
    . = ALIGN(0x8);
    __bss_start = .;
    .bss : {*(.bss .bss.*)}
    __bss_end = .;
    _end = .;

}
__bss_size = (__bss_end - __bss_start)>>3;
