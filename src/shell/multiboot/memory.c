/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:13:48 by zerrino           #+#    #+#             */
/*   Updated: 2025/12/23 18:50:02 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kernel.h"

uint32_t    kernel_mem_ptr = 0x00800000;

uint32_t    kernel_heap_break = 0x10000000;
uint32_t    kernel_heap_limit = 0x40000000;

uint32_t    user_heap_break = 0x40000000;
uint32_t    user_heap_limit = 0xE0000000;

uint32_t    kfree_ptr = 0x0;
uint32_t    vfree_ptr = 0x0;

void    *get_physaddr(void *virtualaddr)
{
    t_mempage *page = get_page((uint32_t)virtualaddr, current_directory);
    uint32_t offset = (uint32_t)virtualaddr & (PAGE_SIZE - 1);

    if (!page || page->frame == 0)
        return 0;
    return (void *)((page->frame * PAGE_SIZE) + offset);
}

void    map_page(void *physaddr, void *virtualaddr, unsigned int flags)
{
    t_mempage *page = create_page((uint32_t)virtualaddr, current_directory);
    int is_kernel = (flags & PT_FLAG_USER) == 0;
    int is_writeable = (flags & PT_FLAG_R_AND_W) != 0;

    if (!page)
        return;
    alloc_frame_at(page, (uint32_t)physaddr, is_kernel, is_writeable);
}

void    unmap_page(void *virtualaddr)
{
    t_mempage *page = get_page((uint32_t)virtualaddr, current_directory);

    if (!page)
        return;
    free_frame(page);
}

void    mmap(void *physaddr, void *virtualaddr, uint32_t size, unsigned int flags)
{
    uint32_t    i;
    uint32_t    size_n;

    size_n = (size + ((1 << 12) - 1)) >> 12;
    i = 0;
    while (i < size_n)
    {
        map_page(physaddr, virtualaddr, flags);
        physaddr = (void *)((uint8_t *)physaddr + (1 << 12));
        virtualaddr = (void *)((uint8_t *)virtualaddr + (1 << 12));
        i++;
    }
}

void    *vbrk(uint32_t increment)
{
    uint32_t    old;

    increment = (increment + (1 << 12) - 1) & ~((1 << 12) - 1);
    if (kernel_mem_ptr > g_memory_limit - increment)
        return (0);
    if (user_heap_break > user_heap_limit - increment)
        return (0);
    mmap((void *)kernel_mem_ptr, (void *)user_heap_break, increment, PT_FLAG_PRESENT | PT_FLAG_R_AND_W | PT_FLAG_USER);
    kernel_mem_ptr += increment;
    old = user_heap_break;
    user_heap_break += increment;
    return ((void *)old);
}

void    *vmalloc(uint32_t size)
{
    uint32_t    total;
    khdr_t      *h;

    size = (size + 15) & ~(15);
    total = size + sizeof(khdr_t);
    if (vfree_ptr != 0)
    {
        while (vfree_ptr < user_heap_break)
        {
            h = (khdr_t *)vfree_ptr;
            if (h->magic == (0xBABECAFE | 1) && h->size >= size)
            {
                h->magic = 0xBABECAFE;
                return ((void *)(h + 1));
            }
            if (h->magic != 0 && h->magic != 0xBABECAFE && h->magic != (0xBABECAFE | 1))
                break;
            vfree_ptr += sizeof(h) + h->size;
        }
    }
    h = vbrk(total);
    if (!h)
        return (0);
    h->size = size;
    h->magic = 0xBABECAFE;
    return ((void *)(h + 1));
}

uint32_t vsize(void *ptr)
{
    khdr_t *h;

    h = ((khdr_t *)ptr) - 1;
    if (h->magic != 0xBABECAFE && h->magic != (0xBABECAFE | 1))
        return (0);
    return (h->size);
}

void    vfree(void *ptr)
{
    khdr_t      *h;
    uint32_t    i;

    if (vsize(ptr) == 0)
        return ;
    h = ((khdr_t *)ptr) - 1;
    if (h->magic != 0xBABECAFE)
        return ;
    h->magic |= 1;
    vfree_ptr = (uint32_t)h;
    i = 0;
    while (i < h->size)
    {
        ((uint8_t *)ptr)[i] = 0;
        i++;
    }
}


void    *kbrk(uint32_t increment)
{
    uint32_t    old;

    increment = (increment + (1 << 12) - 1) & ~((1 << 12) - 1);
    if (kernel_mem_ptr > g_memory_limit - increment)
        return (0);
    if (kernel_heap_break > kernel_heap_limit - increment)
        return (0);
    mmap((void *)kernel_mem_ptr, (void *)kernel_heap_break, increment, PT_FLAG_PRESENT | PT_FLAG_R_AND_W);
    kernel_mem_ptr += increment;
    old = kernel_heap_break;
    kernel_heap_break += increment;
    return ((void *)old);
}

void    *kmalloc(uint32_t size)
{
    if (kheap != NULL)
        return heap_alloc(size, 0, kheap);
    uint32_t    total;
    khdr_t      *h;

    size = (size + 15) & ~(15);
    total = size + sizeof(khdr_t);
    if (kfree_ptr != 0)
    {
        while (kfree_ptr < kernel_heap_break)
        {
            h = (khdr_t *)kfree_ptr;
            if (h->magic == (0xCAFEBABE | 1) && h->size >= size)
            {
                h->magic = 0xCAFEBABE;
                return ((void *)(h + 1));
            }
            if (h->magic != 0 && h->magic != 0xCAFEBABE && h->magic != (0xCAFEBABE | 1))
                break;
            kfree_ptr += sizeof(h) + h->size;
        }
    }
    h = kbrk(total);
    if (!h)
        return (0);
    h->size = size;
    h->magic = 0xCAFEBABE;
    return ((void *)(h + 1));
}

uint32_t ksize(void *ptr)
{
    if (kheap != NULL)
        return heap_block_size(ptr);
    khdr_t *h;

    h = ((khdr_t *)ptr) - 1;
    if (h->magic != 0xCAFEBABE && h->magic != (0xCAFEBABE | 1))
        return (0);
    return (h->size);
}

void    kfree(void *ptr)
{
    if (kheap != NULL)
    {
        heap_free(ptr, kheap);
        return ;
    }
    khdr_t      *h;
    uint32_t    i;

    if (ksize(ptr) == 0)
        return ;
    h = ((khdr_t *)ptr) - 1;
    if (h->magic != 0xCAFEBABE)
        return ;
    h->magic |= 1;
    kfree_ptr = (uint32_t)h;
    i = 0;
    while (i < h->size)
    {
        ((uint8_t *)ptr)[i] = 0;
        i++;
    }
}

void    user_test(void)
{

    while (1)
        ;
}

void    initMemory()
{
    extern uint32_t _kernel_end;

    kernel_mem_ptr = ((uint32_t)&_kernel_end + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    init_paging();
}
