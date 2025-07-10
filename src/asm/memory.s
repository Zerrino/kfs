[BITS 32]

; Export functions
global load_page_directory
global enable_paging_asm
global disable_paging_asm
global get_cr3
global set_cr3
global flush_tlb
global flush_tlb_single

; Load page directory into CR3 register
; void load_page_directory(uint32_t page_dir_phys)
load_page_directory:
    push ebp
    mov ebp, esp
    
    mov eax, [ebp + 8]      ; Get page directory physical address
    mov cr3, eax            ; Load into CR3 register
    
    pop ebp
    ret

; Enable paging by setting the PG bit in CR0
; void enable_paging_asm(void)
enable_paging_asm:
    push ebp
    mov ebp, esp
    
    mov eax, cr0            ; Get current CR0 value
    or eax, 0x80000000      ; Set PG bit (bit 31)
    mov cr0, eax            ; Enable paging
    
    pop ebp
    ret

; Disable paging by clearing the PG bit in CR0
; void disable_paging_asm(void)
disable_paging_asm:
    push ebp
    mov ebp, esp
    
    mov eax, cr0            ; Get current CR0 value
    and eax, 0x7FFFFFFF     ; Clear PG bit (bit 31)
    mov cr0, eax            ; Disable paging
    
    pop ebp
    ret

; Get current page directory from CR3
; uint32_t get_cr3(void)
get_cr3:
    mov eax, cr3            ; Return CR3 value
    ret

; Set page directory in CR3
; void set_cr3(uint32_t page_dir)
set_cr3:
    push ebp
    mov ebp, esp
    
    mov eax, [ebp + 8]      ; Get page directory address
    mov cr3, eax            ; Set CR3 register
    
    pop ebp
    ret

; Flush entire TLB by reloading CR3
; void flush_tlb(void)
flush_tlb:
    mov eax, cr3            ; Get current CR3 value
    mov cr3, eax            ; Reload CR3 to flush TLB
    ret

; Flush single page from TLB
; void flush_tlb_single(uint32_t addr)
flush_tlb_single:
    push ebp
    mov ebp, esp
    
    mov eax, [ebp + 8]      ; Get virtual address
    invlpg [eax]            ; Invalidate page in TLB
    
    pop ebp
    ret

; Additional memory-related assembly functions

; Get current stack pointer
; uint32_t get_esp(void)
global get_esp
get_esp:
    mov eax, esp
    ret

; Get current base pointer
; uint32_t get_ebp(void)
global get_ebp
get_ebp:
    mov eax, ebp
    ret

; Memory copy function (optimized)
; void memcpy_asm(void *dest, const void *src, size_t n)
global memcpy_asm
memcpy_asm:
    push ebp
    mov ebp, esp
    push esi
    push edi
    
    mov edi, [ebp + 8]      ; dest
    mov esi, [ebp + 12]     ; src
    mov ecx, [ebp + 16]     ; n
    
    cld                     ; Clear direction flag
    rep movsb               ; Copy bytes
    
    pop edi
    pop esi
    pop ebp
    ret

; Memory set function (optimized)
; void memset_asm(void *s, int c, size_t n)
global memset_asm
memset_asm:
    push ebp
    mov ebp, esp
    push edi
    
    mov edi, [ebp + 8]      ; s
    mov eax, [ebp + 12]     ; c
    mov ecx, [ebp + 16]     ; n
    
    cld                     ; Clear direction flag
    rep stosb               ; Set bytes
    
    pop edi
    pop ebp
    ret

; Zero memory function (optimized for page clearing)
; void zero_page(void *page)
global zero_page
zero_page:
    push ebp
    mov ebp, esp
    push edi
    
    mov edi, [ebp + 8]      ; page address
    mov ecx, 1024           ; 4096 bytes / 4 = 1024 dwords
    xor eax, eax            ; zero value
    
    cld                     ; Clear direction flag
    rep stosd               ; Set dwords to zero
    
    pop edi
    pop ebp
    ret

; Check if paging is enabled
; int is_paging_enabled(void)
global is_paging_enabled
is_paging_enabled:
    mov eax, cr0            ; Get CR0 value
    and eax, 0x80000000     ; Check PG bit
    shr eax, 31             ; Shift to get 0 or 1
    ret

; Get page fault address from CR2
; uint32_t get_page_fault_addr(void)
global get_page_fault_addr
get_page_fault_addr:
    mov eax, cr2            ; Return CR2 value (page fault address)
    ret

; Enable write protection (WP bit in CR0)
; void enable_write_protection(void)
global enable_write_protection
enable_write_protection:
    mov eax, cr0
    or eax, 0x10000         ; Set WP bit (bit 16)
    mov cr0, eax
    ret

; Disable write protection (WP bit in CR0)
; void disable_write_protection(void)
global disable_write_protection
disable_write_protection:
    mov eax, cr0
    and eax, 0xFFFEFFFF     ; Clear WP bit (bit 16)
    mov cr0, eax
    ret
