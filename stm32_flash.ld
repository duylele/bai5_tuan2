MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

/* Khai báo đỉnh Stack ở cuối vùng RAM */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    .text :
    {
        _stext = .;
        KEEP(*(.isr_vector))
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        
        KEEP (*(.init))
        KEEP (*(.fini))

        /* Gom nhóm các phần mở rộng ARM để không bị lỗi overlap với .data */
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        
        _etext = .;
    } >FLASH

    .data : AT (_etext)
    {
        _sdata = .;
        *(.data)
        *(.data*)
        _edata = .;
    } >RAM

    .bss :
    {
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >RAM

    . = ALIGN(8);
    _end = .;
    PROVIDE(end = .);
}
