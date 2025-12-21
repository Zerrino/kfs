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

section .pagedir_info

section .kernel_stack
global stack_bottom
global stack_top

stack_bottom:
	resb 0x8e000
stack_top:

section .boot
	global _start
	extern kernel_main

_start:




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

;align 4096
inital_page_table:
;%assign n 0
;%rep 1024
;	DD (n << 12) | 00000011b
;%assign n n+1
;%endrep

second_page_table:
;%assign n 1024
;%rep 1024;
;	DD (n << 12) | 00000011b
;%assign n n+1
;%endrep

;DD 9

segment .freememory
global freemem_start
freemem_start:
	resb 0x10

section .bss
	global g_memory_limit
	g_memory_limit: resb 0x4
