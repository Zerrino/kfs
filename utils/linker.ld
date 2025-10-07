
ENTRY(_start)

/*
PHDRS
{
	early PT_LOAD FLAGS(0x5);
	text  PT_LOAD FLAGS(0x5);
	data  PT_LOAD FLAGS(0x6);
	rdata PT_LOAD FLAGS(0x4);
}
*/

SECTIONS
{
	_kernel_start = .;

	. = 0x00000100;
	.multiboot ALIGN(0K):
	{
		*(.multiboot)
	}

	. = 0x00001000;
	
	.pagedir ALIGN(4K) :
	{
		*(.pagedir)
	}

	. = ALIGN(4K);
	
	.kernel_stack (NOLOAD) : ALIGN(16)
	{
		*(.kernel_stack)
		. = 0xa0000;
	}

	. = ALIGN(4K);

	.hardware (NOLOAD) : ALIGN(16)
	{
		*(.hardware)
		. = 0x00100000;
	}


	.boot ALIGN(4K) :
	{
		*(.boot)
	}

	. += 0xC0000000;

	.text ALIGN(4K) : AT (ADDR(.text) - 0xC0000000)
	{
		*(.text)
	}

	.rodata ALIGN(4K) : AT (ADDR(.rodata) - 0xC0000000)
	{
		*(.rodata)
	}

	.data ALIGN(4K) : AT (ADDR(.data) - 0xC0000000)
	{
		*(.data)
	}

	.bss ALIGN(4K) : AT (ADDR(.bss) - 0xC0000000)
	{
		*(COMMON)
		*(.bss)
		_end = .;
	}

	_kernel_end = .;
}
