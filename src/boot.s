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
align 4096
global initial_page_4kb_dir
initial_page_4kb_dir:
	DD (inital_page_table - 0xC0000000)  + 00000011b	 ; 4KB
	TIMES 768-1 DD 0

	DD (inital_page_table - 0xC0000000)  + 00000011b	 ; 4KB
	DD (second_page_table - 0xC0000000)  + 00000011b	 ; 4KB
	TIMES 256-2 DD 0

section .kernel_stack
stack_bottom:
	resb 0x9e000
stack_top:

section .hardware
	;resb 0x100000 - 0x000a2000

section .bss
align 16
	global stack_bottom
	global stack_top
;stack_bottom:
;	resb 16384 * 8
;stack_top:

section .boot
	global _start
	extern kernel_main

_start:


	mov		ecx, (initial_page_4kb_dir)
	mov		cr3, ecx

	mov		ecx, cr4
	or		ecx, 0 ; PAE EXTENSION FLAG
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
	pop		eax
	pop		ebx
	.hang:
		hlt
		jmp .hang

section .data

align 4096
global initial_page_dir ; 1024 P
initial_page_dir:
	DD 10000011b			 ; 4MB
	TIMES 768-1 DD 0

	DD (0 << 22) | 10000011b ; 4MB
	TIMES 256-1 DD 0


align 4096
inital_page_table:
%assign n 0
%rep 1024
	DD (n << 12) | 00000011b
%assign n n+1
%endrep

align 4096
second_page_table:
%assign n 1024
%rep 1024
	DD (n << 12) | 00000011b
%assign n n+1
%endrep