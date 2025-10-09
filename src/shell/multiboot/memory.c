/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:13:48 by zerrino           #+#    #+#             */
/*   Updated: 2025/10/09 15:47:38 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kernel.h"

uint32_t    *page_directory = (uint32_t *)0x00001000;
uint32_t    *page_directory_info = (uint32_t *)0x00002000;
uint32_t    *page_tables = (uint32_t *)0x00400000;

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

void    initMemory()
{
    int         i;

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

    terminal_writestring("\nvalue of the table index : 0x");
    printnbr((uint32_t)get_physaddr((void *)0xc00000), 16);
    terminal_writestring("\n");

    char x[7] __attribute__((aligned(0x1000))) = "\nTest\n";
    char *test = (char *)0xb0000000;
    map_page((void *)(&x), (void*)0xb0000000, PT_FLAG_PRESENT | PT_FLAG_R_AND_W);
    terminal_writestring(test);
}