
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
