BITS 32
global gdt_flush

gdt_flush:
    ; Load GDT
    mov eax, [esp+4]    ; Get the pointer to the GDT, passed as a parameter
    lgdt [eax]          ; Load the GDT

    ; Update segment registers
    mov ax, 0x10        ; 0x10 is the offset in the GDT to our data segment (2nd entry)
    mov ds, ax          ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Far jump to update CS register
    ; This is crucial - we need to reload CS with a far jump
    jmp 0x08:.flush     ; 0x08 is the offset to our code segment (1st entry)
.flush:
    ret
