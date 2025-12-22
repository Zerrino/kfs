/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:13:48 by zerrino           #+#    #+#             */
/*   Updated: 2025/12/22 19:57:10 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kernel.h"

uint32_t    *page_directory = (uint32_t *)0x00001000;
uint32_t    *page_directory_info = (uint32_t *)0x00002000;
uint32_t    *page_tables = (uint32_t *)0x00400000;
uint32_t    kernel_heap_break = 0x00800000;
uint32_t    kfree_ptr = 0x0;

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

void    *kbrk(uint32_t increment)
{
    uint32_t    old;

    increment = (increment + (1 << 12) - 1) & ~((1 << 12) - 1);
    if (kernel_heap_break > g_memory_limit - increment)
        return (0);
    old = kernel_heap_break;
    mmap((void *)old, (void *)old, increment, PT_FLAG_PRESENT | PT_FLAG_R_AND_W);
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

    terminal_writestring("\nvalue of the memory limit : 0x");
    printnbr((uint32_t)kernel_heap_break, 16);
    terminal_writestring("\n");

    char *test = (char *)kmalloc(sizeof(char) * 20);

    test[0] = 's';
    test[1] = 'a';
    test[2] = 'l';
    test[3] = 'u';
    test[4] = 't';
    test[5] = '\0';

    terminal_writestring("\nvalue of the memory limit : 0x");
    printnbr((uint32_t)kernel_heap_break, 16);
    terminal_writestring("\n");
    terminal_writestring(test);
    kfree(test);
    test = (char *)kmalloc(sizeof(char) * 10);
    test[0] = 't';
    test[1] = 'e';
    test[2] = 's';
    test[3] = 't';
    test[4] = 't';
    test[5] = '\0';
    terminal_writestring(test);
    terminal_writestring("\nvalue of the memory limit : 0x");
    printnbr((uint32_t)kernel_heap_break, 16);
    terminal_writestring("\n");
}
