/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:11:44 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/04 15:29:36 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_FUNCTIONS_H
# define MEMORY_FUNCTIONS_H

#include "../core/structs.h"

/* ──────────── Physical Memory Management Functions ──────────── */
void        phys_mem_init(uint32_t mem_size);
uint32_t    phys_alloc_page(void);
void        phys_free_page(uint32_t page_addr);
int         phys_is_page_free(uint32_t page_addr);
uint32_t    phys_get_free_pages(void);
uint32_t    phys_get_used_pages(void);
uint32_t    phys_alloc_pages(uint32_t num_pages);
void        phys_free_pages(uint32_t page_addr, uint32_t num_pages);

/* ──────────── Paging System Functions ──────────── */
void        paging_init(void);
void        paging_enable(void);
void        paging_disable(void);
t_page_directory *paging_create_directory(void);
void        paging_destroy_directory(t_page_directory *dir);
void        paging_switch_directory(t_page_directory *dir);
int         paging_map_page(t_page_directory *dir, uint32_t virt_addr, uint32_t phys_addr, uint32_t flags);
void        paging_unmap_page(t_page_directory *dir, uint32_t virt_addr);
uint32_t    paging_get_physical_addr(t_page_directory *dir, uint32_t virt_addr);
void        paging_flush_tlb(void);
void        paging_flush_tlb_single(uint32_t addr);

/* ──────────── Virtual Memory Management Functions ──────────── */
void        *vmalloc(size_t size);
void        vfree(void *ptr);
size_t      vsize(void *ptr);
void        *vbrk(intptr_t increment);
void        vmem_print_stats(void);

/* ──────────── Kernel Memory Management Functions ──────────── */
void        kmem_init(void);
void        *kmalloc(size_t size);
void        kfree(void *ptr);
size_t      ksize(void *ptr);
void        *kbrk(intptr_t increment);
void        *krealloc(void *ptr, size_t new_size);

/* ──────────── Kernel Panic System Functions ──────────── */
void        kernel_panic(t_panic_level level, const char *message);
void        kernel_warning(const char *message);
void        kernel_error(const char *message);
void        kernel_fatal(const char *message);
void        panic_init(void);
void        page_fault_handler(t_registers *regs);

/* ──────────── Assembly Memory Functions ──────────── */
void        load_page_directory(uint32_t page_dir_phys);
void        enable_paging_asm(void);
void        disable_paging_asm(void);
uint32_t    get_cr3(void);
void        set_cr3(uint32_t page_dir);
void        flush_tlb(void);
void        flush_tlb_single(uint32_t addr);
uint32_t    get_esp(void);
uint32_t    get_ebp(void);
void        memcpy_asm(void *dest, const void *src, size_t n);
void        memset_asm(void *s, int c, size_t n);
void        zero_page(void *page);
int         is_paging_enabled(void);
uint32_t    get_page_fault_addr(void);
void        enable_write_protection(void);
void        disable_write_protection(void);

/* ──────────── Memory Utility Functions ──────────── */
uint32_t    align_to_page(uint32_t addr);
uint32_t    page_align_down(uint32_t addr);
uint32_t    get_page_index(uint32_t addr);
uint32_t    get_table_index(uint32_t addr);
uint32_t    get_page_offset(uint32_t addr);

/* ──────────── Memory Debugging and Information Functions ──────────── */
void        mem_dump_page_directory(t_page_directory *dir);
void        mem_dump_page_table(t_page_table *table);
void        mem_print_stats(void);
void        mem_check_integrity(void);
void        format_size(uint32_t bytes, char *buffer, size_t buffer_size);

/* ──────────── Memory Validation Functions ──────────── */
int         is_valid_kernel_ptr(void *ptr);
int         is_valid_user_ptr(void *ptr);
int         is_valid_address_range(uint32_t start, size_t size, uint32_t min_addr, uint32_t max_addr);

/* ──────────── Master Memory Initialization ──────────── */
void        memory_init(uint32_t mem_size);

#endif /* MEMORY_FUNCTIONS_H */
