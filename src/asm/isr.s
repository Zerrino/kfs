[bits 32]

extern ISR_Handler

%macro ISR_NOERROR 1

global	ISR%1:
ISR%1:
	push	0
	push	%1
	jmp		isr_common

%endmacro

%macro ISR_ERRORCODE 1

global	ISR%1:
ISR%1:

	push	%1
	jmp		isr_common

%endmacro

; error code 8 10 11 12 13 14 17 21 29 30

%assign n 0
%rep 256
	%if  (n = 8)  | (n = 10) | (n = 11) | (n = 12) | (n = 13) | (n = 14) | (n = 17) | (n = 21) | (n = 29) | (n = 30)
		ISR_ERRORCODE n
	%else
		ISR_NOERROR n
	%endif
%assign n n+1
%endrep


isr_common:
    pusha               ; pushes in order: eax, ecx, edx, ebx, esp, ebp, esi, edi

    xor eax, eax        ; push ds
    mov ax, ds
    push eax

    mov ax, 0x10        ; use kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp            ; pass pointer to stack to C, so we can access all the pushed information
    call ISR_Handler
    add esp, 4

    pop eax             ; restore old segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 8
    iret
