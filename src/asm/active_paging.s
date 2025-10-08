[bits 32]

global activate_paging



activate_paging:

	mov		ecx, [esp+4]
	mov		cr3, ecx

	mov		ecx, cr4
	or		ecx, 0 ; PAE EXTENSION FLAG
	mov		cr4, ecx

	mov		ecx, cr0
	or		ecx, 0x80000000
	mov		cr0, ecx

    ret