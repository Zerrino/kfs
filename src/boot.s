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
	
section .pagedir
global inital_page_table
global second_page_table
global initial_page_dir


align 4096
initial_page_dir:
	DD (inital_page_table)  + 00000011b	 ; 4KB
	DD (second_page_table)  + 00000011b	 ; 4KB
	TIMES 256-2 DD 0 ; Kernel Space

	TIMES 768-0 DD 0 ; User Space

section .kernel_stack
global stack_bottom
global stack_top

stack_bottom:
	resb 0x9e000
stack_top:

section .boot
	global _start
	extern kernel_main

_start:


	mov		ecx, (initial_page_dir)
	mov		cr3, ecx

	mov		ecx, cr4
	or		ecx, 0 ; PAE EXTENSION FLAG
	mov		cr4, ecx

	mov		ecx, cr0
	or		ecx, 0x80000000
	mov		cr0, ecx

	jmp		kernel_m
section .text

kernel_m:
	mov		esp, stack_top
	push	ebx
	push	eax
	call kernel_main
	cli
	pop		eax
	pop		ebx
	.hang:
		hlt
		jmp .hang





section .page_tables

align 4096
inital_page_table:
%assign n 0
%rep 1024
	DD (n << 12) | 00000011b
%assign n n+1
%endrep

second_page_table:
%assign n 1024
%rep 1024
	DD (n << 12) | 00000011b
%assign n n+1
%endrep


segment .freememory
global freemem_start
freemem_start:
	resb 0x10