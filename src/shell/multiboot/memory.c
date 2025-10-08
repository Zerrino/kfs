/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:13:48 by zerrino           #+#    #+#             */
/*   Updated: 2025/10/08 15:45:17 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kernel.h"

void    initMemory()
{
	uint32_t    *page_directory = (uint32_t *)0x00001000;
	uint32_t    *page_tables = (uint32_t *)0x00400000;
    int         i;

    i = 0;

    while (i < 1024)
    {
        page_directory[i] = 0;
        if (i < 2)
            page_directory[i] = (uint32_t)(page_tables + ((1024*4) * i)) | 0b00000011;
        i++;
    }

    i = 0;
    while (i < 2048)
    {
        page_tables[i] = (i << 12) | 0b00000011;
        i++;
    }
    activate_paging(page_directory);
}