/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phys_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/02 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

/* Global physical memory manager */
t_phys_mem_manager g_phys_mem_manager = {0};

/* Static bitmap storage */
static uint8_t phys_bitmap[BITMAP_SIZE];

/* Forward declarations */
static void phys_set_page_used(uint32_t page_addr);
static void phys_set_page_free(uint32_t page_addr);

/**
 * Initialize the physical memory manager
 * @param mem_size Total memory size in bytes
 */
void phys_mem_init(uint32_t mem_size)
{
    uint32_t i;
    uint32_t total_pages;
    
    /* Calculate total number of pages */
    total_pages = mem_size / PAGE_SIZE;
    if (total_pages > (MAX_PHYS_MEMORY / PAGE_SIZE))
        total_pages = MAX_PHYS_MEMORY / PAGE_SIZE;
    
    /* Initialize the physical memory manager structure */
    g_phys_mem_manager.bitmap = phys_bitmap;
    g_phys_mem_manager.total_pages = total_pages;
    g_phys_mem_manager.used_pages = 0;
    g_phys_mem_manager.free_pages = total_pages;
    g_phys_mem_manager.first_free_page = 0;
    
    /* Initialize bitmap - all pages are initially free (0) */
    for (i = 0; i < BITMAP_SIZE; i++)
        g_phys_mem_manager.bitmap[i] = 0;
    
    /* Mark the first 1MB as used (BIOS, kernel, etc.) */
    for (i = 0; i < (PHYS_MEM_START / PAGE_SIZE); i++)
        phys_set_page_used(i * PAGE_SIZE);
    
    terminal_writestring("Physical memory manager initialized\n");
    terminal_writestring("Total pages: ");
    printnbr(g_phys_mem_manager.total_pages, 10);
    terminal_writestring("\nFree pages: ");
    printnbr(g_phys_mem_manager.free_pages, 10);
    terminal_writestring("\n");
}

/**
 * Set a page as used in the bitmap
 * @param page_addr Physical address of the page
 */
static void phys_set_page_used(uint32_t page_addr)
{
    uint32_t page_index = page_addr / PAGE_SIZE;
    uint32_t byte_index = page_index / 8;
    uint32_t bit_index = page_index % 8;
    
    if (page_index >= g_phys_mem_manager.total_pages)
        return;
    
    /* Check if page was already used */
    if (!(g_phys_mem_manager.bitmap[byte_index] & (1 << bit_index))) {
        g_phys_mem_manager.bitmap[byte_index] |= (1 << bit_index);
        g_phys_mem_manager.used_pages++;
        g_phys_mem_manager.free_pages--;
    }
}

/**
 * Set a page as free in the bitmap
 * @param page_addr Physical address of the page
 */
static void phys_set_page_free(uint32_t page_addr)
{
    uint32_t page_index = page_addr / PAGE_SIZE;
    uint32_t byte_index = page_index / 8;
    uint32_t bit_index = page_index % 8;
    
    if (page_index >= g_phys_mem_manager.total_pages)
        return;
    
    /* Check if page was already free */
    if (g_phys_mem_manager.bitmap[byte_index] & (1 << bit_index)) {
        g_phys_mem_manager.bitmap[byte_index] &= ~(1 << bit_index);
        g_phys_mem_manager.used_pages--;
        g_phys_mem_manager.free_pages++;
        
        /* Update first free page optimization */
        if ((int)page_index < g_phys_mem_manager.first_free_page)
            g_phys_mem_manager.first_free_page = (int)page_index;
    }
}

/**
 * Check if a page is free
 * @param page_addr Physical address of the page
 * @return 1 if free, 0 if used
 */
int phys_is_page_free(uint32_t page_addr)
{
    uint32_t page_index = page_addr / PAGE_SIZE;
    uint32_t byte_index = page_index / 8;
    uint32_t bit_index = page_index % 8;
    
    if (page_index >= g_phys_mem_manager.total_pages)
        return 0;
    
    return !(g_phys_mem_manager.bitmap[byte_index] & (1 << bit_index));
}

/**
 * Find the first free page starting from a given index
 * @param start_index Index to start searching from
 * @return Index of first free page, or -1 if none found
 */
static int phys_find_free_page(uint32_t start_index)
{
    uint32_t i;
    
    for (i = start_index; i < g_phys_mem_manager.total_pages; i++) {
        if (phys_is_page_free(i * PAGE_SIZE))
            return i;
    }
    
    return -1;
}

/**
 * Allocate a physical page
 * @return Physical address of allocated page, or 0 if no pages available
 */
uint32_t phys_alloc_page(void)
{
    int page_index;
    uint32_t page_addr;
    
    if (g_phys_mem_manager.free_pages == 0) {
        kernel_error("No free physical pages available");
        return 0;
    }
    
    /* Find first free page */
    page_index = phys_find_free_page(g_phys_mem_manager.first_free_page);
    if (page_index == -1) {
        kernel_error("Failed to find free physical page");
        return 0;
    }
    
    /* Calculate physical address */
    page_addr = page_index * PAGE_SIZE;
    
    /* Mark page as used */
    phys_set_page_used(page_addr);
    
    /* Update first free page optimization */
    if ((int)page_index == g_phys_mem_manager.first_free_page) {
        g_phys_mem_manager.first_free_page = phys_find_free_page(page_index + 1);
        if (g_phys_mem_manager.first_free_page == -1)
            g_phys_mem_manager.first_free_page = (int)g_phys_mem_manager.total_pages;
    }
    
    return page_addr;
}

/**
 * Free a physical page
 * @param page_addr Physical address of the page to free
 */
void phys_free_page(uint32_t page_addr)
{
    /* Align address to page boundary */
    page_addr = page_align_down(page_addr);
    
    if (page_addr < PHYS_MEM_START) {
        kernel_warning("Attempted to free reserved physical page");
        return;
    }
    
    if (!phys_is_page_free(page_addr)) {
        phys_set_page_free(page_addr);
    } else {
        kernel_warning("Attempted to free already free physical page");
    }
}

/**
 * Get number of free pages
 * @return Number of free pages
 */
uint32_t phys_get_free_pages(void)
{
    return g_phys_mem_manager.free_pages;
}

/**
 * Get number of used pages
 * @return Number of used pages
 */
uint32_t phys_get_used_pages(void)
{
    return g_phys_mem_manager.used_pages;
}

/**
 * Allocate multiple contiguous physical pages
 * @param num_pages Number of pages to allocate
 * @return Physical address of first page, or 0 if allocation failed
 */
uint32_t phys_alloc_pages(uint32_t num_pages)
{
    uint32_t i, j;
    uint32_t start_page = 0;
    uint32_t consecutive_free = 0;
    
    if (num_pages == 0)
        return 0;
    
    if (num_pages == 1)
        return phys_alloc_page();
    
    if (g_phys_mem_manager.free_pages < num_pages) {
        kernel_error("Not enough free physical pages for allocation");
        return 0;
    }
    
    /* Find consecutive free pages */
    for (i = g_phys_mem_manager.first_free_page; i < g_phys_mem_manager.total_pages; i++) {
        if (phys_is_page_free(i * PAGE_SIZE)) {
            if (consecutive_free == 0)
                start_page = i;
            consecutive_free++;
            
            if (consecutive_free == num_pages) {
                /* Found enough consecutive pages, allocate them */
                for (j = start_page; j < start_page + num_pages; j++)
                    phys_set_page_used(j * PAGE_SIZE);
                
                return start_page * PAGE_SIZE;
            }
        } else {
            consecutive_free = 0;
        }
    }
    
    kernel_error("Failed to find consecutive free physical pages");
    return 0;
}

/**
 * Free multiple contiguous physical pages
 * @param page_addr Physical address of first page
 * @param num_pages Number of pages to free
 */
void phys_free_pages(uint32_t page_addr, uint32_t num_pages)
{
    uint32_t i;
    
    page_addr = page_align_down(page_addr);
    
    for (i = 0; i < num_pages; i++)
        phys_free_page(page_addr + (i * PAGE_SIZE));
}
