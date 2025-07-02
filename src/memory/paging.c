/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paging.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/02 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

/* Global page directories */
t_page_directory *g_kernel_directory = 0;
t_page_directory *g_current_directory = 0;

/* Static storage for kernel page directory */
static t_page_directory kernel_page_directory __attribute__((aligned(4096)));

/**
 * Initialize the paging system
 */
void paging_init(void)
{
    uint32_t i;
    uint32_t phys_addr;
    
    terminal_writestring("Initializing paging system...\n");
    
    /* Initialize kernel page directory */
    g_kernel_directory = &kernel_page_directory;
    
    /* Clear the page directory */
    for (i = 0; i < TABLES_PER_DIR; i++) {
        g_kernel_directory->tables[i].present = 0;
        g_kernel_directory->tables[i].writable = 1;
        g_kernel_directory->tables[i].user = 0;
        g_kernel_directory->tables[i].pwt = 0;
        g_kernel_directory->tables[i].pcd = 0;
        g_kernel_directory->tables[i].accessed = 0;
        g_kernel_directory->tables[i].reserved = 0;
        g_kernel_directory->tables[i].page_size = 0;
        g_kernel_directory->tables[i].ignored = 0;
        g_kernel_directory->tables[i].frame = 0;
    }
    
    /* Identity map the first 4MB (kernel space) */
    for (i = 0; i < 1024; i++) {
        phys_addr = i * PAGE_SIZE;
        paging_map_page(g_kernel_directory, phys_addr, phys_addr, 
                       PAGE_PRESENT | PAGE_WRITABLE);
    }
    
    /* Set current directory */
    g_current_directory = g_kernel_directory;
    
    terminal_writestring("Kernel identity mapping complete\n");
}

/**
 * Enable paging
 */
void paging_enable(void)
{
    uint32_t page_dir_phys;
    
    if (!g_kernel_directory) {
        kernel_fatal("Kernel page directory not initialized");
        return;
    }
    
    /* Get physical address of page directory */
    page_dir_phys = (uint32_t)g_kernel_directory;
    
    terminal_writestring("Enabling paging with directory at 0x");
    printnbr(page_dir_phys, 16);
    terminal_writestring("\n");
    
    /* Load page directory and enable paging */
    load_page_directory(page_dir_phys);
    enable_paging_asm();
    
    terminal_writestring("Paging enabled successfully\n");
}

/**
 * Disable paging
 */
void paging_disable(void)
{
    disable_paging_asm();
    terminal_writestring("Paging disabled\n");
}

/**
 * Create a new page directory
 * @return Pointer to new page directory, or NULL on failure
 */
t_page_directory *paging_create_directory(void)
{
    t_page_directory *dir;
    uint32_t phys_addr;
    uint32_t i;
    
    /* Allocate physical page for directory */
    phys_addr = phys_alloc_page();
    if (!phys_addr) {
        kernel_error("Failed to allocate page for directory");
        return NULL;
    }
    
    dir = (t_page_directory *)phys_addr;
    
    /* Clear the directory */
    for (i = 0; i < TABLES_PER_DIR; i++) {
        dir->tables[i].present = 0;
        dir->tables[i].writable = 1;
        dir->tables[i].user = 0;
        dir->tables[i].pwt = 0;
        dir->tables[i].pcd = 0;
        dir->tables[i].accessed = 0;
        dir->tables[i].reserved = 0;
        dir->tables[i].page_size = 0;
        dir->tables[i].ignored = 0;
        dir->tables[i].frame = 0;
    }
    
    return dir;
}

/**
 * Destroy a page directory
 * @param dir Page directory to destroy
 */
void paging_destroy_directory(t_page_directory *dir)
{
    uint32_t i;
    t_page_table *table;
    
    if (!dir || dir == g_kernel_directory) {
        kernel_warning("Cannot destroy kernel directory or NULL directory");
        return;
    }
    
    /* Free all page tables */
    for (i = 0; i < TABLES_PER_DIR; i++) {
        if (dir->tables[i].present) {
            table = (t_page_table *)(dir->tables[i].frame << 12);
            phys_free_page((uint32_t)table);
        }
    }
    
    /* Free the directory itself */
    phys_free_page((uint32_t)dir);
}

/**
 * Switch to a different page directory
 * @param dir Page directory to switch to
 */
void paging_switch_directory(t_page_directory *dir)
{
    if (!dir) {
        kernel_error("Cannot switch to NULL directory");
        return;
    }
    
    g_current_directory = dir;
    set_cr3((uint32_t)dir);
    flush_tlb();
}

/**
 * Get or create a page table for a given directory index
 * @param dir Page directory
 * @param dir_index Directory index (0-1023)
 * @return Pointer to page table, or NULL on failure
 */
static t_page_table *paging_get_page_table(t_page_directory *dir, uint32_t dir_index)
{
    t_page_table *table;
    uint32_t phys_addr;
    uint32_t i;
    
    if (dir_index >= TABLES_PER_DIR)
        return NULL;
    
    /* Check if table already exists */
    if (dir->tables[dir_index].present) {
        return (t_page_table *)(dir->tables[dir_index].frame << 12);
    }
    
    /* Allocate new page table */
    phys_addr = phys_alloc_page();
    if (!phys_addr) {
        kernel_error("Failed to allocate page for page table");
        return NULL;
    }
    
    table = (t_page_table *)phys_addr;
    
    /* Clear the page table */
    for (i = 0; i < PAGES_PER_TABLE; i++) {
        table->pages[i].present = 0;
        table->pages[i].writable = 1;
        table->pages[i].user = 0;
        table->pages[i].pwt = 0;
        table->pages[i].pcd = 0;
        table->pages[i].accessed = 0;
        table->pages[i].dirty = 0;
        table->pages[i].pat = 0;
        table->pages[i].global = 0;
        table->pages[i].ignored = 0;
        table->pages[i].frame = 0;
    }
    
    /* Set up directory entry */
    dir->tables[dir_index].present = 1;
    dir->tables[dir_index].writable = 1;
    dir->tables[dir_index].user = 0;
    dir->tables[dir_index].frame = phys_addr >> 12;
    
    return table;
}

/**
 * Map a virtual address to a physical address
 * @param dir Page directory
 * @param virt_addr Virtual address
 * @param phys_addr Physical address
 * @param flags Page flags
 * @return 0 on success, -1 on failure
 */
int paging_map_page(t_page_directory *dir, uint32_t virt_addr, uint32_t phys_addr, uint32_t flags)
{
    uint32_t dir_index;
    uint32_t table_index;
    t_page_table *table;
    
    if (!dir) {
        kernel_error("Cannot map page: directory is NULL");
        return -1;
    }
    
    /* Align addresses to page boundaries */
    virt_addr = page_align_down(virt_addr);
    phys_addr = page_align_down(phys_addr);
    
    /* Get indices */
    dir_index = get_page_index(virt_addr);
    table_index = get_table_index(virt_addr);
    
    /* Get or create page table */
    table = paging_get_page_table(dir, dir_index);
    if (!table) {
        kernel_error("Failed to get page table for mapping");
        return -1;
    }
    
    /* Check if page is already mapped */
    if (table->pages[table_index].present) {
        kernel_warning("Page already mapped, remapping");
    }
    
    /* Set up page table entry */
    table->pages[table_index].present = (flags & PAGE_PRESENT) ? 1 : 0;
    table->pages[table_index].writable = (flags & PAGE_WRITABLE) ? 1 : 0;
    table->pages[table_index].user = (flags & PAGE_USER) ? 1 : 0;
    table->pages[table_index].pwt = (flags & PAGE_WRITE_THROUGH) ? 1 : 0;
    table->pages[table_index].pcd = (flags & PAGE_CACHE_DISABLE) ? 1 : 0;
    table->pages[table_index].global = (flags & PAGE_GLOBAL) ? 1 : 0;
    table->pages[table_index].frame = phys_addr >> 12;
    
    /* Flush TLB for this address */
    flush_tlb_single(virt_addr);
    
    return 0;
}

/**
 * Unmap a virtual address
 * @param dir Page directory
 * @param virt_addr Virtual address to unmap
 */
void paging_unmap_page(t_page_directory *dir, uint32_t virt_addr)
{
    uint32_t dir_index;
    uint32_t table_index;
    t_page_table *table;
    
    if (!dir) {
        kernel_error("Cannot unmap page: directory is NULL");
        return;
    }
    
    virt_addr = page_align_down(virt_addr);
    dir_index = get_page_index(virt_addr);
    table_index = get_table_index(virt_addr);
    
    /* Check if page table exists */
    if (!dir->tables[dir_index].present)
        return;
    
    table = (t_page_table *)(dir->tables[dir_index].frame << 12);
    
    /* Clear page table entry */
    table->pages[table_index].present = 0;
    table->pages[table_index].frame = 0;
    
    /* Flush TLB for this address */
    flush_tlb_single(virt_addr);
}

/**
 * Get physical address for a virtual address
 * @param dir Page directory
 * @param virt_addr Virtual address
 * @return Physical address, or 0 if not mapped
 */
uint32_t paging_get_physical_addr(t_page_directory *dir, uint32_t virt_addr)
{
    uint32_t dir_index;
    uint32_t table_index;
    uint32_t offset;
    t_page_table *table;
    
    if (!dir)
        return 0;
    
    dir_index = get_page_index(virt_addr);
    table_index = get_table_index(virt_addr);
    offset = get_page_offset(virt_addr);
    
    /* Check if page table exists */
    if (!dir->tables[dir_index].present)
        return 0;
    
    table = (t_page_table *)(dir->tables[dir_index].frame << 12);
    
    /* Check if page is present */
    if (!table->pages[table_index].present)
        return 0;
    
    return (table->pages[table_index].frame << 12) + offset;
}
