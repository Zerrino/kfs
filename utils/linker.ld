
ENTRY(_start)


PHDRS
{
	multiboot		PT_LOAD FLAGS(0x4);
	pagedir			PT_LOAD FLAGS(0x6);
	pagedir_info	PT_LOAD FLAGS(0x6);
	kernel_stack	PT_LOAD FLAGS(0x6);
	hardware		PT_LOAD FLAGS(0x6);


	kernel_code		PT_LOAD FLAGS(0x5);
	data 			PT_LOAD FLAGS(0x6);
	rdata			PT_LOAD FLAGS(0x4);

	page_tables 	PT_LOAD FLAGS(0x6);
	freemem			PT_LOAD FLAGS(0x6);
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
	.pagedir (NOLOAD) :
	{
		_pagedir_start = .;
		*(.pagedir)
		. += 0x1000;
	} : pagedir

	.pagedir_info (NOLOAD) :
	{
		*(.pagedir_info)
		. += 0x1000;
	} : pagedir_info

	.kernel_stack (NOLOAD) : ALIGN(16)
	{
		*(.kernel_stack)
		. = 0xa0000 - 0x3000;
		_end = .;
	} : kernel_stack

	.hardware (NOLOAD) : ALIGN(16)
	{
		. = 0x00100000 - 0xa0000;
		*(.hardware)
	} : hardware


	.boot ALIGN(4K) :
	{
		*(.boot)
	} : kernel_code

	.text ALIGN(4K) : AT (ADDR(.text))
	{
		*(.text)
	} : kernel_code

	.data ALIGN(4K) : AT (ADDR(.data))
	{
		*(.data)
	} : data

	.bss (NOLOAD) : AT (ADDR(.bss))
	{
		*(.bss)
	} : data

	.rodata ALIGN(4K) : AT (ADDR(.rodata))
	{
		*(.rodata)
	} : rdata


	. = 0x400000;

	.page_tables (NOLOAD) :
	{
		_pagetables_start = .;
		*(.page_tables)
		. += 0x400000;
	} : page_tables


	.freememory ALIGN(4K) : AT (ADDR(.freememory))
	{
		*(.freememory)
	} : freemem

	_kernel_end = .;
}
