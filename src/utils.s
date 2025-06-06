BITS 32
	global irq1_handler
	global load_idt
	extern keyboard_handler
	global irq0_handler
irq0_handler:
    pusha
    mov al, 0x20
    out 0x20, al   ; EOI pour IRQ0 (PIC maître)
    popa
    iretd

irq1_handler:
	; keyboard_handler(t_kernel *kernel)
    pusha
    call keyboard_handler
    mov al, 0x20        ; envoyer EOI au PIC
    out 0x20, al
    popa
    iretd

load_idt:
    mov eax, [esp+4]   ; EAX = pointeur vers idt_ptr
    lidt [eax]         ; charger IDT depuis la structure pointée
    ret
