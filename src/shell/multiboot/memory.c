/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:13:48 by zerrino           #+#    #+#             */
/*   Updated: 2025/10/09 14:11:27 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kernel.h"

uint32_t    *page_directory = (uint32_t *)0x00001000;
uint32_t    *page_directory_info = (uint32_t *)0x00002000;
uint32_t    *page_tables = (uint32_t *)0x00400000;

void    *getPage(void *vm_addr)
{
    uint32_t    pd_index = ((uint32_t)vm_addr >> 22);
    uint32_t    pt_index = ((uint32_t)vm_addr >> 12) & ((1<<10) - 1);
    terminal_writestring("\npd_index : 0x");
    printnbr(pd_index, 16);
    terminal_writestring("\npt_index : 0x");
    printnbr(pt_index, 16);
    terminal_writestring("\nvalue of the table index : 0x");
    printnbr(((uint32_t *)(page_directory[pd_index] & ~((1 << 12)-1)))[pt_index] , 16);
    terminal_writestring("\n");
    return 0;
}

void    initMemory()
{
    int         i;

    i = 0;

    while (i < 1024)
    {
        page_directory[i] = 0;
        if (i < 256)
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

   getPage((void *)0xC01000);
}