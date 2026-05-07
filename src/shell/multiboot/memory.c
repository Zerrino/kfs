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

uint32_t    *page_directory = (uint32_t *)0x00001000;
uint32_t    *page_directory_info = (uint32_t *)0x00002000;
uint32_t    *page_tables = (uint32_t *)0x00400000;


uint32_t    kernel_mem_ptr = 0x00800000;

uint32_t    kernel_heap_break = 0x10000000;
uint32_t    kernel_heap_limit = 0x40000000;

uint32_t    user_heap_break = 0x40000000;
uint32_t    user_heap_limit = 0xE0000000;

uint32_t    kfree_ptr = 0x0;
uint32_t    vfree_ptr = 0x0;

void    *get_physaddr(void *virtualaddr)
{
    uint32_t    pd_index = ((uint32_t)virtualaddr >> 22);
    uint32_t    pt_index = ((uint32_t)virtualaddr >> 12) & ((1<<10)-1);
    return (void *)(((uint32_t *)(page_directory[pd_index] & ~((1 << 12)-1)))[pt_index] & ~((1 << 12)-1));
}

void    map_page(void *physaddr, void *virtualaddr, unsigned int flags)
{
    uint32_t    pd_index = ((uint32_t)virtualaddr >> 22);
    uint32_t    pt_index = ((uint32_t)virtualaddr >> 12) & ((1<<10) - 1);

    ((uint32_t *)(page_directory[pd_index] & ~((1 << 12)-1)))[pt_index] = (uint32_t)physaddr | (flags & ((1 << 12)-1)) | 1;
}

void    unmap_page(void *virtualaddr)
{
    uint32_t    pd_index = ((uint32_t)virtualaddr >> 22);
    uint32_t    pt_index = ((uint32_t)virtualaddr >> 12) & ((1<<10) - 1);

    ((uint32_t *)(page_directory[pd_index] & ~((1 << 12)-1)))[pt_index] = 0;
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
    khdr_t *h;

    h = ((khdr_t *)ptr) - 1;
    if (h->magic != 0xCAFEBABE && h->magic != (0xCAFEBABE | 1))
        return (0);
    return (h->size);
}

void    kfree(void *ptr)
{
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
    int i;

    i = 0;

    while (i < 1024)
    {
        page_directory[i] = (uint32_t)(&page_tables[i << 10]) | PD_FLAG_PRESENT | PD_FLAG_R_AND_W;
        i++;
    }
    i = 0;
    while (i < 1024)
    {
        page_directory_info[i] = 0;
        i++;
    }
    i = 0;
    while (i < (1024 << 10))
    {
        page_tables[i] = 0;
        if (i < (2 << 10))
            page_tables[i] = (i << 12) | PT_FLAG_PRESENT | PT_FLAG_R_AND_W;
        i++;
    }
    activate_paging(page_directory);
}
