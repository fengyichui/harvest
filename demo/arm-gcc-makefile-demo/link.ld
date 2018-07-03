
/* Define memory regions. */
MEMORY
{
    rom (rx) :  ORIGIN = 0x08000000, LENGTH = 512K
    ram (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Define the entry point of the output file. */
ENTRY(reset_handler)
_system_stack_size = 0x1000;
_system_heap_size  = 0xA000;

/* Define sections. */
SECTIONS
{
    .vectors : { KEEP(*(.vectors)) } > rom    /* Vector table */
    .text : {
        *(.text*)    /* Program code */
        . = ALIGN(4);
        *(.rodata*)  /* Read-only data */
        . = ALIGN(4);
        _etext = .;
    } >rom

    /* .stack must be placed to fisrt 8K */
    .stack : {
        . = ALIGN(4);
        _stack = .;
        . = . + _system_stack_size;
        . = ALIGN(4);
        _estack = .;
    } > ram

    .heap : {
        . = ALIGN(4);
        __heap_base = .;
        . = . + _system_heap_size;
        . = ALIGN(4);
        __heap_limit = .;
    } > ram

    .data : {
        _data = .;
        *(.data*)    /* Read-write initialized data */
        . = ALIGN(4);
        _edata = .;
    } >ram AT >rom
    _data_loadaddr = LOADADDR(.data);

    .bss : {
        *(.bss*)    /* Read-write zero initialized data */
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >ram

    . = ALIGN(4);
    end = .;
}

