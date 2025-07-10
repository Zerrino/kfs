[bits 32]

global	IDT_Load
IDT_Load:
    push ebp             ; save old call frame
    mov ebp, esp         ; initialize new call frame

    ; load idt
    mov eax, [ebp + 8]
    lidt [eax]

    ; restore old call frame
    mov esp, ebp
    pop ebp
    ret

