/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/02 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H

#include <stdint.h>
#include <stddef.h>

/* ──────────── Memory Constants ──────────── */
#define PAGE_SIZE           4096        /* 4KB pages */
#define PAGE_ALIGN_MASK     0xFFFFF000  /* Page alignment mask */
#define PAGE_OFFSET_MASK    0x00000FFF  /* Page offset mask */
#define PAGES_PER_TABLE     1024        /* Pages per page table */
#define TABLES_PER_DIR      1024        /* Page tables per directory */

/* Memory layout constants */
#define KERNEL_START        0x00100000  /* 1MB - Kernel start */
#define KERNEL_HEAP_START   0x00400000  /* 4MB - Kernel heap start */
#define KERNEL_HEAP_SIZE    0x00400000  /* 4MB - Kernel heap size */
#define USER_SPACE_START    0x40000000  /* 1GB - User space start */
#define USER_SPACE_END      0xC0000000  /* 3GB - User space end */

/* Physical memory constants */
#define PHYS_MEM_START      0x00100000  /* 1MB - Start of usable physical memory */
#define MAX_PHYS_MEMORY     0x10000000  /* 256MB - Maximum physical memory */
#define BITMAP_SIZE         (MAX_PHYS_MEMORY / PAGE_SIZE / 8) /* Bitmap size in bytes */

/* ──────────── Page Directory/Table Entry Flags ──────────── */
#define PAGE_PRESENT        0x001       /* Page is present in memory */
#define PAGE_WRITABLE       0x002       /* Page is writable */
#define PAGE_USER           0x004       /* Page is accessible by user */
#define PAGE_WRITE_THROUGH  0x008       /* Write-through caching */
#define PAGE_CACHE_DISABLE  0x010       /* Cache disabled */
#define PAGE_ACCESSED       0x020       /* Page was accessed */
#define PAGE_DIRTY          0x040       /* Page was written to (only for page table entries) */
#define PAGE_SIZE_4MB       0x080       /* 4MB page size (only for page directory entries) */
#define PAGE_GLOBAL         0x100       /* Global page */

/* ──────────── Memory Management Structures ──────────── */

/* Page directory entry */
typedef struct s_page_dir_entry {
    uint32_t present    : 1;    /* Page present in memory */
    uint32_t writable   : 1;    /* Read/write permission */
    uint32_t user       : 1;    /* User/supervisor */
    uint32_t pwt        : 1;    /* Page write through */
    uint32_t pcd        : 1;    /* Page cache disable */
    uint32_t accessed   : 1;    /* Page accessed */
    uint32_t reserved   : 1;    /* Reserved (set to 0) */
    uint32_t page_size  : 1;    /* Page size (0 = 4KB, 1 = 4MB) */
    uint32_t ignored    : 4;    /* Ignored */
    uint32_t frame      : 20;   /* Frame address (bits 12-31) */
} __attribute__((packed)) t_page_dir_entry;

/* Page table entry */
typedef struct s_page_table_entry {
    uint32_t present    : 1;    /* Page present in memory */
    uint32_t writable   : 1;    /* Read/write permission */
    uint32_t user       : 1;    /* User/supervisor */
    uint32_t pwt        : 1;    /* Page write through */
    uint32_t pcd        : 1;    /* Page cache disable */
    uint32_t accessed   : 1;    /* Page accessed */
    uint32_t dirty      : 1;    /* Page dirty */
    uint32_t pat        : 1;    /* Page attribute table */
    uint32_t global     : 1;    /* Global page */
    uint32_t ignored    : 3;    /* Ignored */
    uint32_t frame      : 20;   /* Frame address (bits 12-31) */
} __attribute__((packed)) t_page_table_entry;

/* Page table */
typedef struct s_page_table {
    t_page_table_entry pages[PAGES_PER_TABLE];
} __attribute__((packed)) t_page_table;

/* Page directory */
typedef struct s_page_directory {
    t_page_dir_entry tables[TABLES_PER_DIR];
} __attribute__((packed)) t_page_directory;

/* Physical memory manager structure */
typedef struct s_phys_mem_manager {
    uint8_t *bitmap;            /* Bitmap for tracking allocated pages */
    uint32_t total_pages;       /* Total number of pages */
    uint32_t used_pages;        /* Number of used pages */
    uint32_t free_pages;        /* Number of free pages */
    int first_free_page;        /* Index of first free page (optimization, -1 if none) */
} t_phys_mem_manager;

/* Virtual memory area structure */
typedef struct s_vma {
    uint32_t start_addr;        /* Start virtual address */
    uint32_t end_addr;          /* End virtual address */
    uint32_t flags;             /* Memory flags */
    struct s_vma *next;         /* Next VMA in list */
} t_vma;

/* Memory allocation block header */
typedef struct s_mem_block {
    size_t size;                /* Size of the block */
    int is_free;                /* 1 if free, 0 if allocated */
    struct s_mem_block *next;   /* Next block in list */
    struct s_mem_block *prev;   /* Previous block in list */
} t_mem_block;

/* Kernel heap structure */
typedef struct s_kernel_heap {
    uint32_t start_addr;        /* Heap start address */
    uint32_t end_addr;          /* Heap end address */
    uint32_t current_end;       /* Current heap end */
    t_mem_block *first_block;   /* First memory block */
} t_kernel_heap;

/* Panic levels */
typedef enum e_panic_level {
    PANIC_LEVEL_WARNING = 0,    /* Warning - system can continue */
    PANIC_LEVEL_ERROR = 1,      /* Error - system should stop */
    PANIC_LEVEL_FATAL = 2       /* Fatal - system must halt immediately */
} t_panic_level;

/* ──────────── Function Declarations ──────────── */

/* Physical Memory Management */
void        phys_mem_init(uint32_t mem_size);
uint32_t    phys_alloc_page(void);
void        phys_free_page(uint32_t page_addr);
int         phys_is_page_free(uint32_t page_addr);
uint32_t    phys_get_free_pages(void);
uint32_t    phys_get_used_pages(void);
uint32_t    phys_alloc_pages(uint32_t num_pages);
void        phys_free_pages(uint32_t page_addr, uint32_t num_pages);

/* Paging System */
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

/* Virtual Memory Management */
void        *vmalloc(size_t size);
void        vfree(void *ptr);
size_t      vsize(void *ptr);
void        *vbrk(intptr_t increment);
void        vmem_print_stats(void);

/* Kernel Memory Management */
void        kmem_init(void);
void        *kmalloc(size_t size);
void        kfree(void *ptr);
size_t      ksize(void *ptr);
void        *kbrk(intptr_t increment);
void        *krealloc(void *ptr, size_t new_size);

/* Kernel Panic System */
void        kernel_panic(t_panic_level level, const char *message);
void        kernel_warning(const char *message);
void        kernel_error(const char *message);
void        kernel_fatal(const char *message);
void        panic_init(void);
void        page_fault_handler(t_registers *regs);

/* Assembly functions (implemented in ASM) */
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

/* Utility functions */
uint32_t    align_to_page(uint32_t addr);
uint32_t    page_align_down(uint32_t addr);
uint32_t    get_page_index(uint32_t addr);
uint32_t    get_table_index(uint32_t addr);
uint32_t    get_page_offset(uint32_t addr);

/* Memory debugging and information */
void        mem_dump_page_directory(t_page_directory *dir);
void        mem_dump_page_table(t_page_table *table);
void        mem_print_stats(void);
void        mem_check_integrity(void);
void        format_size(uint32_t bytes, char *buffer, size_t buffer_size);

/* Global variables */
extern t_phys_mem_manager g_phys_mem_manager;
extern t_page_directory *g_kernel_directory;
extern t_page_directory *g_current_directory;
extern t_kernel_heap g_kernel_heap;

#endif /* MEMORY_H */
