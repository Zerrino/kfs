
ENTRY(_start)


PHDRS
{
	multiboot		PT_LOAD FLAGS(0x4);
	pagedir			PT_LOAD FLAGS(0x6);
	kernel_stack	PT_LOAD FLAGS(0x6);
	hardware		PT_LOAD FLAGS(0x6);
	boot			PT_LOAD FLAGS(0x5);


	text  			PT_LOAD FLAGS(0x5);
	data  			PT_LOAD FLAGS(0x6);
	rdata 			PT_LOAD FLAGS(0x4);
}


SECTIONS
{
	_kernel_start = .;

	. = 0x00000100;
	.multiboot ALIGN(0K):
	{
		*(.multiboot)
	} : multiboot

	. = 0x00001000;
	
	.pagedir ALIGN(4K) :
	{
		*(.pagedir)
	} : pagedir

	/*. = ALIGN(4K); */
	
	.kernel_stack (NOLOAD) : ALIGN(16)
	{
		*(.kernel_stack)
		. = 0xa0000 - 0x2000;
	} : kernel_stack
	/*. = ALIGN(4K); */

	.hardware (NOLOAD) : ALIGN(16)
	{
		. = 0x00100000 - 0xa0000;
		*(.hardware)
	} : hardware


	.boot ALIGN(4K) :
	{
		*(.boot)
	} : boot

	. += 0xC0000000;

	.text ALIGN(4K) : AT (ADDR(.text) - 0xC0000000)
	{
		*(.text)
	} : text

	.rodata ALIGN(4K) : AT (ADDR(.rodata) - 0xC0000000)
	{
		*(.rodata)
	} : rdata

	.data ALIGN(4K) : AT (ADDR(.data) - 0xC0000000)
	{
		*(.data)
	} : data

	.bss ALIGN(4K) : AT (ADDR(.bss) - 0xC0000000)
	{
		*(COMMON)
		*(.bss)
		_end = .;
	} : data

	_kernel_end = .;
}
