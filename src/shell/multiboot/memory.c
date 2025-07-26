/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Zerrino <Zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:13:48 by zerrino           #+#    #+#             */
/*   Updated: 2025/07/25 15:48:44 by Zerrino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kernel.h"

static uint32_t	pageFrameMin;
static uint32_t	pageFrameMax;
static uint32_t	totalAlloc;
extern uint32_t initial_page_dir[1024];


#define NUM_PAGES_DIRS 256
#define NUM_PAGES_FRAMES (0x10000000 / 0x1000 / 8)

uint8_t	physicalMemoryBitmap[NUM_PAGES_FRAMES / 8]; // later dynamic bit array

static uint32_t	pageDirs[NUM_PAGES_DIRS][1024] __attribute__((aligned(4096)));
static uint8_t	pageDirUsed[NUM_PAGES_DIRS];

void	pm_init(uint32_t memLow, uint32_t memHigh)
{
	pageFrameMin = CEIL_DIV(memLow, 0x1000);
	pageFrameMax = memHigh / 0x1000;
	totalAlloc = 0;

	ft_memset(physicalMemoryBitmap, 0, sizeof(physicalMemoryBitmap));
}

void	invalidate(uint32_t vaddr)
{
	asm	volatile("invlpg %0" :: "m"(vaddr));
}

void	initMemory(uint32_t memHigh, uint32_t physicalAllocStart)
{
	initial_page_dir[0] = 0;
	invalidate(0);
	initial_page_dir[1023] = ((uint32_t) initial_page_dir - KERNEL_START) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
	invalidate(0xFFFFF000);

	pm_init(physicalAllocStart, memHigh);

	ft_memset(pageDirs, 0, 0x1000 * NUM_PAGES_DIRS);
	ft_memset(pageDirUsed, 0, NUM_PAGES_DIRS);
}

