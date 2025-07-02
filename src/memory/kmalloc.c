/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/02 00:00:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

/* Global kernel heap */
t_kernel_heap g_kernel_heap = {0};

/* Minimum block size (including header) */
#define MIN_BLOCK_SIZE (sizeof(t_mem_block) + 16)

/**
 * Initialize kernel memory management
 */
void kmem_init(void)
{
    t_mem_block *initial_block;
    
    /* Initialize kernel heap structure */
    g_kernel_heap.start_addr = KERNEL_HEAP_START;
    g_kernel_heap.end_addr = KERNEL_HEAP_START + KERNEL_HEAP_SIZE;
    g_kernel_heap.current_end = KERNEL_HEAP_START;
    g_kernel_heap.first_block = NULL;
    
    /* Create initial block */
    initial_block = (t_mem_block *)g_kernel_heap.start_addr;
    initial_block->size = 0;
    initial_block->is_free = 1;
    initial_block->next = NULL;
    initial_block->prev = NULL;
    
    g_kernel_heap.first_block = initial_block;
    g_kernel_heap.current_end = g_kernel_heap.start_addr + sizeof(t_mem_block);
    
    terminal_writestring("Kernel memory allocator initialized\n");
    terminal_writestring("Heap start: 0x");
    printnbr(g_kernel_heap.start_addr, 16);
    terminal_writestring("\nHeap size: ");
    printnbr(KERNEL_HEAP_SIZE / 1024, 10);
    terminal_writestring(" KB\n");
}

/**
 * Expand heap by allocating more pages
 * @param size Minimum size to expand by
 * @return 0 on success, -1 on failure
 */
static int expand_heap(size_t size)
{
    uint32_t pages_needed;
    uint32_t new_end;
    uint32_t i;
    uint32_t virt_addr;
    uint32_t phys_addr;

    /* Check if paging is enabled and directory is set */
    if (!g_current_directory) {
        kernel_error("expand_heap called without page directory");
        return -1;
    }

    /* Calculate pages needed */
    pages_needed = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    new_end = g_kernel_heap.current_end + (pages_needed * PAGE_SIZE);
    
    /* Check if we exceed heap limit */
    if (new_end > g_kernel_heap.end_addr) {
        kernel_error("Kernel heap exhausted");
        return -1;
    }
    
    /* Allocate and map pages */
    for (i = 0; i < pages_needed; i++) {
        virt_addr = g_kernel_heap.current_end + (i * PAGE_SIZE);
        phys_addr = phys_alloc_page();
        
        if (!phys_addr) {
            kernel_error("Failed to allocate physical page for heap expansion");
            return -1;
        }
        
        if (paging_map_page(g_current_directory, virt_addr, phys_addr, 
                           PAGE_PRESENT | PAGE_WRITABLE) != 0) {
            phys_free_page(phys_addr);
            kernel_error("Failed to map page for heap expansion");
            return -1;
        }
        
        /* Zero the new page */
        zero_page((void *)virt_addr);
    }
    
    g_kernel_heap.current_end = new_end;
    return 0;
}

/**
 * Find a free block of sufficient size
 * @param size Required size
 * @return Pointer to free block, or NULL if none found
 */
static t_mem_block *find_free_block(size_t size)
{
    t_mem_block *current = g_kernel_heap.first_block;
    
    while (current) {
        if (current->is_free && current->size >= size) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

/**
 * Split a block if it's larger than needed
 * @param block Block to split
 * @param size Required size
 */
static void split_block(t_mem_block *block, size_t size)
{
    t_mem_block *new_block;
    size_t remaining_size;
    
    if (!block || block->size < size + MIN_BLOCK_SIZE)
        return;
    
    remaining_size = block->size - size - sizeof(t_mem_block);
    
    /* Create new block after current block */
    new_block = (t_mem_block *)((uint8_t *)block + sizeof(t_mem_block) + size);
    new_block->size = remaining_size;
    new_block->is_free = 1;
    new_block->next = block->next;
    new_block->prev = block;
    
    /* Update links */
    if (block->next)
        block->next->prev = new_block;
    block->next = new_block;
    block->size = size;
}

/**
 * Merge adjacent free blocks
 * @param block Block to merge with adjacent blocks
 */
static void merge_free_blocks(t_mem_block *block)
{
    if (!block || !block->is_free)
        return;
    
    /* Merge with next block if it's free */
    while (block->next && block->next->is_free) {
        t_mem_block *next_block = block->next;
        block->size += sizeof(t_mem_block) + next_block->size;
        block->next = next_block->next;
        if (next_block->next)
            next_block->next->prev = block;
    }
    
    /* Merge with previous block if it's free */
    if (block->prev && block->prev->is_free) {
        t_mem_block *prev_block = block->prev;
        prev_block->size += sizeof(t_mem_block) + block->size;
        prev_block->next = block->next;
        if (block->next)
            block->next->prev = prev_block;
    }
}

/**
 * Allocate kernel memory
 * @param size Size in bytes to allocate
 * @return Pointer to allocated memory, or NULL on failure
 */
void *kmalloc(size_t size)
{
    t_mem_block *block;
    uint32_t total_size;

    if (size == 0)
        return NULL;

    /* Check if heap is initialized */
    if (g_kernel_heap.start_addr == 0) {
        kernel_error("kmalloc called before heap initialization");
        return NULL;
    }

    /* Align size to 4-byte boundary */
    size = (size + 3) & ~3;
    
    /* Try to find existing free block */
    block = find_free_block(size);
    
    if (!block) {
        /* Need to expand heap */
        total_size = sizeof(t_mem_block) + size;
        
        /* Ensure we have enough space */
        if (g_kernel_heap.current_end + total_size > g_kernel_heap.end_addr) {
            if (expand_heap(total_size) != 0)
                return NULL;
        }
        
        /* Create new block at end of heap */
        block = (t_mem_block *)g_kernel_heap.current_end;
        block->size = size;
        block->is_free = 0;
        block->next = NULL;
        block->prev = NULL;
        
        /* Link to existing blocks */
        if (g_kernel_heap.first_block && g_kernel_heap.first_block->size > 0) {
            t_mem_block *last = g_kernel_heap.first_block;
            while (last->next)
                last = last->next;
            last->next = block;
            block->prev = last;
        } else {
            g_kernel_heap.first_block = block;
        }
        
        g_kernel_heap.current_end += sizeof(t_mem_block) + size;
    } else {
        /* Use existing free block */
        block->is_free = 0;
        split_block(block, size);
    }
    
    /* Return pointer to data (after header) */
    return (void *)((uint8_t *)block + sizeof(t_mem_block));
}

/**
 * Free kernel memory
 * @param ptr Pointer to memory to free
 */
void kfree(void *ptr)
{
    t_mem_block *block;
    
    if (!ptr)
        return;
    
    /* Get block header */
    block = (t_mem_block *)((uint8_t *)ptr - sizeof(t_mem_block));
    
    /* Validate block */
    if ((uint32_t)block < g_kernel_heap.start_addr || 
        (uint32_t)block >= g_kernel_heap.current_end) {
        kernel_warning("Attempted to free invalid pointer");
        return;
    }
    
    if (block->is_free) {
        kernel_warning("Attempted to free already freed memory");
        return;
    }
    
    /* Mark as free */
    block->is_free = 1;
    
    /* Merge with adjacent free blocks */
    merge_free_blocks(block);
}

/**
 * Get size of allocated memory block
 * @param ptr Pointer to allocated memory
 * @return Size of block, or 0 if invalid
 */
size_t ksize(void *ptr)
{
    t_mem_block *block;
    
    if (!ptr)
        return 0;
    
    block = (t_mem_block *)((uint8_t *)ptr - sizeof(t_mem_block));
    
    /* Validate block */
    if ((uint32_t)block < g_kernel_heap.start_addr || 
        (uint32_t)block >= g_kernel_heap.current_end) {
        return 0;
    }
    
    if (block->is_free)
        return 0;
    
    return block->size;
}

/**
 * Expand or shrink heap (sbrk-like function)
 * @param increment Bytes to add (positive) or remove (negative)
 * @return Previous heap end, or NULL on failure
 */
void *kbrk(intptr_t increment)
{
    uint32_t old_end = g_kernel_heap.current_end;
    uint32_t new_end = old_end + increment;
    
    if (increment == 0)
        return (void *)old_end;
    
    if (increment > 0) {
        /* Expanding heap */
        if (new_end > g_kernel_heap.end_addr) {
            kernel_error("Heap expansion would exceed limit");
            return NULL;
        }
        
        if (expand_heap(increment) != 0)
            return NULL;
    } else {
        /* Shrinking heap - not implemented for safety */
        kernel_warning("Heap shrinking not implemented");
        return NULL;
    }
    
    return (void *)old_end;
}

/**
 * Reallocate memory block
 * @param ptr Existing pointer (can be NULL)
 * @param new_size New size
 * @return Pointer to reallocated memory, or NULL on failure
 */
void *krealloc(void *ptr, size_t new_size)
{
    void *new_ptr;
    size_t old_size;
    size_t copy_size;
    
    if (!ptr)
        return kmalloc(new_size);
    
    if (new_size == 0) {
        kfree(ptr);
        return NULL;
    }
    
    old_size = ksize(ptr);
    if (old_size == 0) {
        kernel_warning("Attempted to realloc invalid pointer");
        return NULL;
    }
    
    /* If new size is same or smaller, just return existing pointer */
    if (new_size <= old_size)
        return ptr;
    
    /* Allocate new block */
    new_ptr = kmalloc(new_size);
    if (!new_ptr)
        return NULL;
    
    /* Copy data */
    copy_size = (old_size < new_size) ? old_size : new_size;
    memcpy_asm(new_ptr, ptr, copy_size);
    
    /* Free old block */
    kfree(ptr);
    
    return new_ptr;
}
