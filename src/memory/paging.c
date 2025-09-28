/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paging.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:45:34 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 17:09:04 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kfs.h"

/* Static storage for kernel page directory */
static t_page_directory kernel_page_directory __attribute__((aligned(4096)));

void paging_init(void)
{
    uint32_t i;
    uint32_t phys_addr;
    
    g_kernel_directory = &kernel_page_directory;
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
        paging_map_page(g_kernel_directory, phys_addr, phys_addr,PAGE_PRESENT | PAGE_WRITABLE);
    }

    /* Map kernel heap area (4MB - 8MB) */
    for (i = 1024; i < 2048; i++) {
        phys_addr = phys_alloc_page();
        if (phys_addr) 
            paging_map_page(g_kernel_directory, i * PAGE_SIZE, phys_addr,PAGE_PRESENT | PAGE_WRITABLE);
        else {
            terminal_writestring("Warning: Failed to allocate page for heap mapping\n");
            break;
        }
    }
    
    g_current_directory = g_kernel_directory;
}

void paging_enable(void)
{    
    if (!g_kernel_directory) {
        kernel_fatal("Kernel page directory not initialized");
        return;
    }
}

void paging_disable(void)
{
    disable_paging_asm();
    terminal_writestring("Paging disabled\n");
}

t_page_directory *paging_create_directory(void)
{
    t_page_directory *dir;
    uint32_t phys_addr;
    uint32_t i;
    
    phys_addr = phys_alloc_page();
    if (!phys_addr) {
        kernel_error("Failed to allocate page for directory");
        return NULL;
    }
    
    dir = (t_page_directory *)phys_addr;
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

void paging_destroy_directory(t_page_directory *dir)
{
    uint32_t i;
    t_page_table *table;
    
    if (!dir || dir == g_kernel_directory) {
        kernel_warning("Cannot destroy kernel directory or NULL directory");
        return;
    }
    
    for (i = 0; i < TABLES_PER_DIR; i++) {
        if (dir->tables[i].present) {
            table = (t_page_table *)(dir->tables[i].frame << 12);
            phys_free_page((uint32_t)table);
        }
    }
    
    phys_free_page((uint32_t)dir);
}

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

static t_page_table *paging_get_page_table(t_page_directory *dir, uint32_t dir_index)
{
    t_page_table *table;
    uint32_t phys_addr;
    uint32_t i;
    
    if (dir_index >= TABLES_PER_DIR)
        return NULL;
    
    if (dir->tables[dir_index].present) 
        return (t_page_table *)(dir->tables[dir_index].frame << 12);
    
    phys_addr = phys_alloc_page();
    if (!phys_addr) {
        kernel_error("Failed to allocate page for page table");
        return NULL;
    }
    
    table = (t_page_table *)phys_addr;
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
    
    dir->tables[dir_index].present = 1;
    dir->tables[dir_index].writable = 1;
    dir->tables[dir_index].user = 0;
    dir->tables[dir_index].frame = phys_addr >> 12;
    return table;
}

int paging_map_page(t_page_directory *dir, uint32_t virt_addr, uint32_t phys_addr, uint32_t flags)
{
    uint32_t dir_index;
    uint32_t table_index;
    t_page_table *table;
    
    if (!dir) {
        kernel_error("Cannot map page: directory is NULL");
        return -1;
    }
    
    virt_addr = page_align_down(virt_addr);
    phys_addr = page_align_down(phys_addr);
    dir_index = get_page_index(virt_addr);
    table_index = get_table_index(virt_addr);
    table = paging_get_page_table(dir, dir_index);
    if (!table) {
        kernel_error("Failed to get page table for mapping");
        return -1;
    }
    
    if (table->pages[table_index].present)
        kernel_warning("Page already mapped, remapping");
    
    table->pages[table_index].present = (flags & PAGE_PRESENT) ? 1 : 0;
    table->pages[table_index].writable = (flags & PAGE_WRITABLE) ? 1 : 0;
    table->pages[table_index].user = (flags & PAGE_USER) ? 1 : 0;
    table->pages[table_index].pwt = (flags & PAGE_WRITE_THROUGH) ? 1 : 0;
    table->pages[table_index].pcd = (flags & PAGE_CACHE_DISABLE) ? 1 : 0;
    table->pages[table_index].global = (flags & PAGE_GLOBAL) ? 1 : 0;
    table->pages[table_index].frame = phys_addr >> 12;
    flush_tlb_single(virt_addr);
    return 0;
}

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
    if (!dir->tables[dir_index].present)
        return;
    
    table = (t_page_table *)(dir->tables[dir_index].frame << 12);
    table->pages[table_index].present = 0;
    table->pages[table_index].frame = 0;
    flush_tlb_single(virt_addr);
}

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
    if (!dir->tables[dir_index].present)
        return 0;
    
    table = (t_page_table *)(dir->tables[dir_index].frame << 12);
    if (!table->pages[table_index].present)
        return 0;
    
    return (table->pages[table_index].frame << 12) + offset;
}
