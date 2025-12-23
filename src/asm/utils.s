
[bits 32]

global crash_me

crash_me:
	int		50
	mov		eax, 0
	div		eax
	ret


global EnableInterrupts
EnableInterrupts:
	sti
	ret

global DisableInterrupts
DisableInterrupts:
	cli
	ret

global i686_outb
i686_outb:
	[bits 32]
	mov dx, [esp + 4]
	mov al, [esp + 8]
	out dx, al
	ret

global i686_inb
i686_inb:
	[bits 32]
	mov dx, [esp + 4]
	xor eax, eax
	in al, dx
	ret


global enter_user_mode
enter_user_mode:
    mov     ax, 0x23
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    mov     eax, [esp+4]      ; user_stack
    mov     ebx, [esp+8]      ; entry

    push    dword 0x23        ; SS user
    push    eax               ; ESP user

    pushfd
    or      dword [esp], 0x200    ; IF

    push    dword 0x1B        ; CS user
    push    ebx               ; EIP user

    iretd
