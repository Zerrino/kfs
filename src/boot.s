%define ALIGN    (1 << 0)
%define MEMINFO  (1 << 1)
%define FLAGS    (ALIGN | MEMINFO)
%define MAGIC    0x1BADB002
%define CHECKSUM -(MAGIC + FLAGS)

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

	dd 0, 0, 0, 0, 0          ; Adresse header, load_addr, load_end_addr, bss_end_addr, entry_addr
	dd 0
	dd 800
	dd 600
	dd 32

section .bss
align 16
	global stack_bottom
	global stack_top
stack_bottom:
	resb 16384 * 8
stack_top:

section .boot
	global _start
	extern kernel_main

_start:


	mov		ecx, (initial_page_dir - 0xC0000000)
	mov		cr3, ecx

	mov		ecx, cr4
	or		ecx, 0x10
	mov		cr4, ecx

	mov		ecx, cr0
	or		ecx, 0x80000000
	mov		cr0, ecx

	jmp		higher_half


section .text
higher_half:

	mov		esp, stack_top
	push	ebx
	push	eax
	call kernel_main
	cli
	.hang:
		hlt
		jmp .hang

section .data
align 4096
global initial_page_dir
initial_page_dir:
	DD 10000011b
	TIMES 768-1 DD 0

	DD (0 << 22) | 10000011b
	DD (1 << 22) | 10000011b
	DD (2 << 22) | 10000011b
	DD (3 << 22) | 10000011b
	TIMES 256-4 DD 0
