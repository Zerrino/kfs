[bits 32]
%include "include/gdt_segments.inc"

extern ISR_Handler

%macro ISR_NOERROR 1

global	ISR%1
ISR%1:
	cli
	push	0
	push	%1
	jmp		isr_common

%endmacro

%macro ISR_ERRORCODE 1

global	ISR%1
ISR%1:
	cli
	push	%1
	jmp		isr_common

%endmacro

; error code 8 10 11 12 13 14 17 21 29 30

%assign n 0
%rep 256
	%if  (n == 8)  | (n == 10) | (n == 11) | (n == 12) | (n == 13) | (n == 14) | (n == 17) | (n == 21) | (n == 29) | (n == 30)
		ISR_ERRORCODE n
	%else
		ISR_NOERROR n
	%endif
%assign n n+1
%endrep


isr_common:

	pusha

	xor eax, eax
	mov ax, ds
	push eax

	xor eax, eax
	mov ax, es
	push eax

	xor eax, eax
	mov ax, fs
	push eax

	xor eax, eax
	mov ax, gs
	push eax

	mov ax, GDT_KERNEL_DATA
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	push esp
	call ISR_Handler
	add esp, 4

	pop eax
	mov gs, ax

	pop eax
	mov fs, ax

	pop eax
	mov es, ax

	pop eax
	mov ds, ax

	popa
	add esp, 8
	iret
