BITS 32
%include "include/gdt_segments.inc"

global gdt_flush

gdt_flush:
    ; Load GDT
    mov eax, [esp+4]    ; Get the pointer to the GDT, passed as a parameter
    lgdt [eax]          ; Load the GDT

    ; Update segment registers
    mov ax, GDT_KERNEL_DATA  ; Load kernel data segment selector
    mov ds, ax               ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ax, GDT_KERNEL_STACK ; Load kernel stack segment selector
    mov ss, ax               ; Set stack segment

    ; Far jump to update CS register
    ; This is crucial - we need to reload CS with a far jump
    jmp GDT_KERNEL_CODE:.flush  ; Jump to kernel code segment
.flush:
    ret
