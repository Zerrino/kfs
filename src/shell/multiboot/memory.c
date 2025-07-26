/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Zerrino <Zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:13:48 by zerrino           #+#    #+#             */
/*   Updated: 2025/07/26 23:42:10 by Zerrino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kernel.h"

static uint32_t	pageFrameMin;
static uint32_t	pageFrameMax;
static uint32_t	totalAlloc;
extern uint32_t initial_page_dir[1024];
uint32_t	mem_num_vpages;

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
	mem_num_vpages = 0;
	initial_page_dir[0] = 0;
	invalidate(0);
	initial_page_dir[1023] = ((uint32_t) initial_page_dir - KERNEL_START) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
	invalidate(0xFFFFF000);

	pm_init(physicalAllocStart, memHigh);

	ft_memset(pageDirs, 0, 0x1000 * NUM_PAGES_DIRS);
	ft_memset(pageDirUsed, 0, NUM_PAGES_DIRS);
}

uint32_t	*memGetCurrentPageDir()
{
	uint32_t	pd;

	asm volatile("mov %%cr3, %0": "=r"(pd));
	pd += KERNEL_START;

	return ((uint32_t*)pd);
}

void	memChangePageDir(uint32_t *pd)
{
	pd = (uint32_t *)(((uint32_t)pd) - KERNEL_START);
	asm volatile("mov %0, %%eax \n mov %%eax, %%cr3 \n" :: "m"(pd));
}

void	syncPageDirs()
{
	for (uint32_t i = 0; i < NUM_PAGES_DIRS; i++)
	{
		if (pageDirUsed[i])
		{
			uint32_t*	pageDir = pageDirs[i];

			for (uint32_t i = 768; i < 1023; i++)
			{
				pageDir[i] = initial_page_dir[i] & ~PAGE_FLAG_OWNER;
			}
		}
	}

}

void	memMapPage(uint32_t virtualAddr, uint32_t physicalAddr, uint32_t flags)
{
	uint32_t	*prevPageDir = 0;

	if (virtualAddr >= KERNEL_START)
	{
		prevPageDir = memGetCurrentPageDir();
		if (prevPageDir != initial_page_dir)
		{
			memChangePageDir(initial_page_dir);
		}
	}

	uint32_t	pdIndex = virtualAddr >> 22;
	uint32_t	ptIndex = virtualAddr >> 12 & 0x3ff;

	uint32_t*	pageDir = REC_PAGEDIR;
	uint32_t*	pt = REC_PAGETABLE(pdIndex);

	if (!(pageDir[pdIndex] & PAGE_FLAG_PRESENT))
	{
		uint32_t	ptPaddr = pmmAllocPageFrame();
		pageDir[pdIndex] = ptPaddr | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE | PAGE_FLAG_OWNER | flags;
		invalidate(virtualAddr);

		for (uint32_t i = 0; i < 1024; i++)
		{
			pt[i] = 0;
		}
	}

	pt[ptIndex] = physicalAddr | PAGE_FLAG_PRESENT | flags;
	mem_num_vpages++;
	invalidate(virtualAddr);

	if (prevPageDir != 0)
	{
		syncPageDirs();
		if (prevPageDir != initial_page_dir)
		{
			memChangePageDir(prevPageDir);
		}
	}
}

uint32_t	pmmAllocPageFrame()
{
	uint32_t	start = pageFrameMin / 8 + ((pageFrameMin & 7) != 0 ? 1:0);
	uint32_t	end   = pageFrameMax / 8 - ((pageFrameMax & 7) != 0 ? 1:0);

	for (uint32_t b = start; b < end; b++)
	{
		uint8_t	byte = physicalMemoryBitmap[b];
		if (byte == 0xff)
		{
			continue;
		}
		for (uint32_t i = 0; i < 8; i++)
		{
			bool	used = byte >> i & 1;

			if (!used)
			{
				byte ^= (-1 ^byte) & (1 << i);
				physicalMemoryBitmap[b] = byte;
				totalAlloc++;

				uint32_t	addr = (b * 8 * i) * 0x1000;
				return (addr);
			}
		}
	}
	return (0);
}
