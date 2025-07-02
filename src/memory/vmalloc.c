/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/02 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

/* Virtual memory area list */
static t_vma *vma_list = NULL;
static uint32_t next_virt_addr = USER_SPACE_START;

/**
 * Find a free virtual memory area of given size
 * @param size Size in bytes
 * @return Virtual address, or 0 if not found
 */
static uint32_t find_free_vma(size_t size)
{
    t_vma *current = vma_list;
    uint32_t addr = USER_SPACE_START;
    uint32_t aligned_size = align_to_page(size);
    
    /* If no VMAs exist, start from beginning */
    if (!current)
        return addr;
    
    /* Check gap before first VMA */
    if (current->start_addr >= addr + aligned_size)
        return addr;
    
    /* Check gaps between VMAs */
    while (current->next) {
        uint32_t gap_start = current->end_addr;
        uint32_t gap_end = current->next->start_addr;
        
        if (gap_end - gap_start >= aligned_size)
            return gap_start;
        
        current = current->next;
    }
    
    /* Check gap after last VMA */
    addr = current->end_addr;
    if (addr + aligned_size <= USER_SPACE_END)
        return addr;
    
    return 0; /* No suitable area found */
}

/**
 * Create a new VMA
 * @param start_addr Start virtual address
 * @param size Size in bytes
 * @param flags Memory flags
 * @return Pointer to VMA, or NULL on failure
 */
static t_vma *create_vma(uint32_t start_addr, size_t size, uint32_t flags)
{
    t_vma *new_vma;
    t_vma *current;
    t_vma *prev = NULL;
    
    /* Allocate VMA structure */
    new_vma = (t_vma *)kmalloc(sizeof(t_vma));
    if (!new_vma) {
        kernel_error("Failed to allocate VMA structure");
        return NULL;
    }
    
    /* Initialize VMA */
    new_vma->start_addr = start_addr;
    new_vma->end_addr = start_addr + align_to_page(size);
    new_vma->flags = flags;
    new_vma->next = NULL;
    
    /* Insert VMA in sorted order */
    current = vma_list;
    while (current && current->start_addr < start_addr) {
        prev = current;
        current = current->next;
    }
    
    if (prev) {
        prev->next = new_vma;
        new_vma->next = current;
    } else {
        new_vma->next = vma_list;
        vma_list = new_vma;
    }
    
    return new_vma;
}

/**
 * Find VMA containing given address
 * @param addr Virtual address
 * @return Pointer to VMA, or NULL if not found
 */
static t_vma *find_vma(uint32_t addr)
{
    t_vma *current = vma_list;
    
    while (current) {
        if (addr >= current->start_addr && addr < current->end_addr)
            return current;
        current = current->next;
    }
    
    return NULL;
}

/**
 * Remove VMA from list
 * @param vma VMA to remove
 */
static void remove_vma(t_vma *vma)
{
    t_vma *current = vma_list;
    t_vma *prev = NULL;
    
    if (!vma)
        return;
    
    /* Find VMA in list */
    while (current && current != vma) {
        prev = current;
        current = current->next;
    }
    
    if (!current)
        return; /* VMA not found */
    
    /* Remove from list */
    if (prev)
        prev->next = current->next;
    else
        vma_list = current->next;
    
    /* Free VMA structure */
    kfree(vma);
}

/**
 * Allocate virtual memory
 * @param size Size in bytes
 * @return Virtual address, or NULL on failure
 */
void *vmalloc(size_t size)
{
    uint32_t virt_addr;
    uint32_t phys_addr;
    uint32_t pages_needed;
    uint32_t i;
    t_vma *vma;
    
    if (size == 0)
        return NULL;
    
    /* Find free virtual memory area */
    virt_addr = find_free_vma(size);
    if (!virt_addr) {
        kernel_error("No free virtual memory area available");
        return NULL;
    }
    
    /* Calculate pages needed */
    pages_needed = (align_to_page(size)) / PAGE_SIZE;
    
    /* Allocate physical pages and map them */
    for (i = 0; i < pages_needed; i++) {
        phys_addr = phys_alloc_page();
        if (!phys_addr) {
            kernel_error("Failed to allocate physical page for vmalloc");
            /* Clean up already allocated pages */
            for (uint32_t j = 0; j < i; j++) {
                uint32_t cleanup_virt = virt_addr + (j * PAGE_SIZE);
                uint32_t cleanup_phys = paging_get_physical_addr(g_current_directory, cleanup_virt);
                if (cleanup_phys) {
                    paging_unmap_page(g_current_directory, cleanup_virt);
                    phys_free_page(cleanup_phys);
                }
            }
            return NULL;
        }
        
        if (paging_map_page(g_current_directory, virt_addr + (i * PAGE_SIZE), 
                           phys_addr, PAGE_PRESENT | PAGE_WRITABLE) != 0) {
            kernel_error("Failed to map virtual page");
            phys_free_page(phys_addr);
            /* Clean up already allocated pages */
            for (uint32_t j = 0; j < i; j++) {
                uint32_t cleanup_virt = virt_addr + (j * PAGE_SIZE);
                uint32_t cleanup_phys = paging_get_physical_addr(g_current_directory, cleanup_virt);
                if (cleanup_phys) {
                    paging_unmap_page(g_current_directory, cleanup_virt);
                    phys_free_page(cleanup_phys);
                }
            }
            return NULL;
        }
        
        /* Zero the page */
        zero_page((void *)(virt_addr + (i * PAGE_SIZE)));
    }
    
    /* Create VMA */
    vma = create_vma(virt_addr, size, PAGE_PRESENT | PAGE_WRITABLE);
    if (!vma) {
        kernel_error("Failed to create VMA");
        /* Clean up allocated pages */
        for (i = 0; i < pages_needed; i++) {
            uint32_t cleanup_virt = virt_addr + (i * PAGE_SIZE);
            uint32_t cleanup_phys = paging_get_physical_addr(g_current_directory, cleanup_virt);
            if (cleanup_phys) {
                paging_unmap_page(g_current_directory, cleanup_virt);
                phys_free_page(cleanup_phys);
            }
        }
        return NULL;
    }
    
    return (void *)virt_addr;
}

/**
 * Free virtual memory
 * @param ptr Virtual address to free
 */
void vfree(void *ptr)
{
    uint32_t virt_addr = (uint32_t)ptr;
    t_vma *vma;
    uint32_t pages;
    uint32_t i;
    uint32_t phys_addr;
    
    if (!ptr)
        return;
    
    /* Find VMA */
    vma = find_vma(virt_addr);
    if (!vma) {
        kernel_warning("Attempted to free invalid virtual address");
        return;
    }
    
    /* Check if address is start of VMA */
    if (virt_addr != vma->start_addr) {
        kernel_warning("Attempted to free middle of virtual memory area");
        return;
    }
    
    /* Calculate pages to free */
    pages = (vma->end_addr - vma->start_addr) / PAGE_SIZE;
    
    /* Unmap and free physical pages */
    for (i = 0; i < pages; i++) {
        uint32_t page_virt = vma->start_addr + (i * PAGE_SIZE);
        phys_addr = paging_get_physical_addr(g_current_directory, page_virt);
        
        if (phys_addr) {
            paging_unmap_page(g_current_directory, page_virt);
            phys_free_page(phys_addr);
        }
    }
    
    /* Remove VMA */
    remove_vma(vma);
}

/**
 * Get size of virtual memory allocation
 * @param ptr Virtual address
 * @return Size in bytes, or 0 if invalid
 */
size_t vsize(void *ptr)
{
    uint32_t virt_addr = (uint32_t)ptr;
    t_vma *vma;
    
    if (!ptr)
        return 0;
    
    vma = find_vma(virt_addr);
    if (!vma || virt_addr != vma->start_addr)
        return 0;
    
    return vma->end_addr - vma->start_addr;
}

/**
 * Expand virtual memory space (sbrk-like for virtual memory)
 * @param increment Bytes to add (positive) or remove (negative)
 * @return Previous end address, or NULL on failure
 */
void *vbrk(intptr_t increment)
{
    uint32_t old_addr = next_virt_addr;
    uint32_t new_addr = old_addr + increment;
    
    if (increment == 0)
        return (void *)old_addr;
    
    if (increment > 0) {
        /* Expanding virtual space */
        if (new_addr > USER_SPACE_END) {
            kernel_error("Virtual space expansion would exceed limit");
            return NULL;
        }
        
        /* Check for conflicts with existing VMAs */
        t_vma *current = vma_list;
        while (current) {
            if ((old_addr < current->end_addr) && (new_addr > current->start_addr)) {
                kernel_error("Virtual space expansion conflicts with existing VMA");
                return NULL;
            }
            current = current->next;
        }
        
        next_virt_addr = new_addr;
    } else {
        /* Shrinking virtual space - not implemented for safety */
        kernel_warning("Virtual space shrinking not implemented");
        return NULL;
    }
    
    return (void *)old_addr;
}

/**
 * Print virtual memory statistics
 */
void vmem_print_stats(void)
{
    t_vma *current = vma_list;
    uint32_t total_vmas = 0;
    uint32_t total_size = 0;
    
    terminal_writestring("\n=== Virtual Memory Statistics ===\n");
    
    while (current) {
        total_vmas++;
        total_size += (current->end_addr - current->start_addr);
        
        if (total_vmas <= 5) { /* Show first 5 VMAs */
            terminal_writestring("VMA ");
            printnbr(total_vmas, 10);
            terminal_writestring(": 0x");
            printnbr(current->start_addr, 16);
            terminal_writestring(" - 0x");
            printnbr(current->end_addr, 16);
            terminal_writestring(" (");
            printnbr((current->end_addr - current->start_addr) / 1024, 10);
            terminal_writestring(" KB)\n");
        }
        
        current = current->next;
    }
    
    if (total_vmas > 5) {
        terminal_writestring("... and ");
        printnbr(total_vmas - 5, 10);
        terminal_writestring(" more VMAs\n");
    }
    
    terminal_writestring("Total VMAs: ");
    printnbr(total_vmas, 10);
    terminal_writestring("\nTotal virtual memory: ");
    printnbr(total_size / 1024, 10);
    terminal_writestring(" KB\n");
    terminal_writestring("Next virtual address: 0x");
    printnbr(next_virt_addr, 16);
    terminal_writestring("\n================================\n\n");
}
