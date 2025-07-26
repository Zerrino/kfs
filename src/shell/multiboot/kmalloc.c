/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Zerrino <Zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 22:41:07 by Zerrino           #+#    #+#             */
/*   Updated: 2025/07/26 22:59:47 by Zerrino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kernel.h"

static uint32_t	heapStart;
static uint32_t	heapSize;
static uint32_t	threshold;
static bool		kmallocInitalized = false;


void	kmallocInit(uint32_t initialHeapSize)
{
	heapStart = KERNEL_MALLOC;
	heapSize = 0;
	threshold = 0;
	kmallocInitalized = true;

	changeHeapSize(initialHeapSize);
}

void	changeHeapSize(int newSize)
{
	int	oldPageTop = CEIL_DIV(heapSize, 0x1000);
	int	newPageTop = CEIL_DIV(newSize, 0x1000);

	int	diff = newPageTop - oldPageTop;

	for (int i = 0; i < diff; i++)
	{
		uint32_t phys = pmmAllocPageFrame();
		memMapPage(KERNEL_MALLOC + oldPageTop * 0x1000 + i * 0x1000, phys, PAGE_FLAG_WRITE);
	}
}
