/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:00:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 17:06:51 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kfs.h"

void kmem_init(void)
{
    t_mem_block *initial_block;
    g_kernel_heap.start_addr = KERNEL_HEAP_START;
    g_kernel_heap.end_addr = KERNEL_HEAP_START + KERNEL_HEAP_SIZE;
    g_kernel_heap.current_end = KERNEL_HEAP_START;
    g_kernel_heap.first_block = NULL;
    initial_block = (t_mem_block *)g_kernel_heap.start_addr;
    initial_block->size = 0;
    initial_block->is_free = 1;
    initial_block->next = NULL;
    initial_block->prev = NULL;
    g_kernel_heap.first_block = initial_block;
    g_kernel_heap.current_end = g_kernel_heap.start_addr + sizeof(t_mem_block);
}

static int expand_heap(size_t size)
{
    uint32_t pages_needed;
    uint32_t new_end;
    uint32_t i;
    uint32_t virt_addr;
    uint32_t phys_addr;

    if (!g_current_directory) {
        kernel_error("expand_heap called without page directory");
        return -1;
    }

    pages_needed = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    new_end = g_kernel_heap.current_end + (pages_needed * PAGE_SIZE);
    if (new_end > g_kernel_heap.end_addr) {
        kernel_error("Kernel heap exhausted");
        return -1;
    }
    
    for (i = 0; i < pages_needed; i++) {
        virt_addr = g_kernel_heap.current_end + (i * PAGE_SIZE);
        phys_addr = phys_alloc_page();
        
        if (!phys_addr) {
            kernel_error("Failed to allocate physical page for heap expansion");
            return -1;
        }
        
        if (paging_map_page(g_current_directory, virt_addr, phys_addr, PAGE_PRESENT | PAGE_WRITABLE) != 0) {
            phys_free_page(phys_addr);
            kernel_error("Failed to map page for heap expansion");
            return -1;
        }

        zero_page((void *)virt_addr);
    }
    
    g_kernel_heap.current_end = new_end;
    return 0;
}

static t_mem_block *find_free_block(size_t size)
{
    t_mem_block *current = g_kernel_heap.first_block;
    
    while (current) {
        if (current->is_free && current->size >= size) 
            return current;

        current = current->next;
    }
    
    return NULL;
}

static void split_block(t_mem_block *block, size_t size)
{
    t_mem_block *new_block;
    size_t remaining_size;
    
    if (!block || block->size < size + MIN_BLOCK_SIZE)
        return;
    
    remaining_size = block->size - size - sizeof(t_mem_block);
    new_block = (t_mem_block *)((uint8_t *)block + sizeof(t_mem_block) + size);
    new_block->size = remaining_size;
    new_block->is_free = 1;
    new_block->next = block->next;
    new_block->prev = block;
    if (block->next)
        block->next->prev = new_block;
    block->next = new_block;
    block->size = size;
}

static void merge_free_blocks(t_mem_block *block)
{
    if (!block || !block->is_free)
        return;
    
    while (block->next && block->next->is_free) {
        t_mem_block *next_block = block->next;
        block->size += sizeof(t_mem_block) + next_block->size;
        block->next = next_block->next;
        if (next_block->next)
            next_block->next->prev = block;
    }
    
    if (block->prev && block->prev->is_free) {
        t_mem_block *prev_block = block->prev;
        prev_block->size += sizeof(t_mem_block) + block->size;
        prev_block->next = block->next;
        if (block->next)
            block->next->prev = prev_block;
    }
}

void *kmalloc(size_t size)
{
    t_mem_block *block;
    uint32_t total_size;

    if (size == 0)
        return NULL;

    if (g_kernel_heap.start_addr == 0) {
        kernel_error("kmalloc called before heap initialization");
        return NULL;
    }

    size = (size + 3) & ~3;
    block = find_free_block(size);
    if (!block) {
        total_size = sizeof(t_mem_block) + size;
        
        if (g_kernel_heap.current_end + total_size > g_kernel_heap.end_addr) 
            if (expand_heap(total_size) != 0)
                return NULL;
        
        block = (t_mem_block *)g_kernel_heap.current_end;
        block->size = size;
        block->is_free = 0;
        block->next = NULL;
        block->prev = NULL;
        if (g_kernel_heap.first_block && g_kernel_heap.first_block->size > 0) {
            t_mem_block *last = g_kernel_heap.first_block;
            while (last->next)
                last = last->next;
            last->next = block;
            block->prev = last;
        } else 
            g_kernel_heap.first_block = block;
        
        g_kernel_heap.current_end += sizeof(t_mem_block) + size;
    } else {
        block->is_free = 0;
        split_block(block, size);
    }
    
    return (void *)((uint8_t *)block + sizeof(t_mem_block));
}

void kfree(void *ptr)
{
    t_mem_block *block;
    
    if (!ptr)
        return;
    
    block = (t_mem_block *)((uint8_t *)ptr - sizeof(t_mem_block));
    if ((uint32_t)block < g_kernel_heap.start_addr || (uint32_t)block >= g_kernel_heap.current_end) {
        kernel_warning("Attempted to free invalid pointer");
        return;
    }
    
    if (block->is_free) {
        kernel_warning("Attempted to free already freed memory");
        return;
    }
    
    block->is_free = 1;
    merge_free_blocks(block);
}

size_t ksize(void *ptr)
{
    t_mem_block *block;
    
    if (!ptr)
        return 0;
    
    block = (t_mem_block *)((uint8_t *)ptr - sizeof(t_mem_block));
    if ((uint32_t)block < g_kernel_heap.start_addr || (uint32_t)block >= g_kernel_heap.current_end) 
        return 0;
    
    if (block->is_free)
        return 0;
    
    return block->size;
}

void *kbrk(intptr_t increment)
{
    uint32_t old_end = g_kernel_heap.current_end;
    uint32_t new_end = old_end + increment;
    
    if (increment == 0)
        return (void *)old_end;
    
    if (increment > 0) {
        if (new_end > g_kernel_heap.end_addr) {
            kernel_error("Heap expansion would exceed limit");
            return NULL;
        }
        
        if (expand_heap(increment) != 0)
            return NULL;
    } 
	else {
        kernel_warning("Heap shrinking not implemented");
        return NULL;
    }
    
    return (void *)old_end;
}

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
    
    if (new_size <= old_size)
        return ptr;
    
    new_ptr = kmalloc(new_size);
    if (!new_ptr)
        return NULL;
    
    copy_size = (old_size < new_size) ? old_size : new_size;
    memcpy_asm(new_ptr, ptr, copy_size);
    kfree(ptr);
    return new_ptr;
}
