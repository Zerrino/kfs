/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pages.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 16:40:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/24 16:40:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

static uint32_t placement_address = 0;

t_mempage_directory *kernel_directory = NULL;
t_mempage_directory *current_directory = NULL;

void *kmem_early_alloc(uint32_t size, uint32_t align)
{
	uint32_t addr;

	if (align && (placement_address & (PAGE_SIZE - 1)))
		placement_address = (placement_address + PAGE_SIZE) & ~(PAGE_SIZE - 1);
	addr = placement_address;
	placement_address += size;
	return (void *)addr;
}

static void paging_identity_map(uint32_t start, uint32_t end)
{
	uint32_t addr = start;

	while (addr < end)
	{
		t_mempage *page = create_page(addr, kernel_directory);
		alloc_frame_at(page, addr, 1, 1);
		addr += PAGE_SIZE;
	}
}

t_mempage *get_page(uint32_t address, t_mempage_directory *dir)
{
	uint32_t addr = address / PAGE_SIZE;
	uint32_t table_idx = addr / 1024;

	if (dir->tables[table_idx])
		return &dir->tables[table_idx]->pages[addr % 1024];
	return NULL;
}

t_mempage *create_page(uint32_t address, t_mempage_directory *dir)
{
	uint32_t addr = address / PAGE_SIZE;
	uint32_t table_idx = addr / 1024;

	if (dir->tables[table_idx] == NULL)
	{
		t_mempage_table *table = (t_mempage_table *)kmem_early_alloc(sizeof(t_mempage_table), 1);
		ft_memset(table, 0, sizeof(t_mempage_table));
		dir->tables[table_idx] = table;
		dir->tables_phys[table_idx] = ((uint32_t)table) | PT_FLAG_PRESENT | PT_FLAG_R_AND_W;
	}
	return &dir->tables[table_idx]->pages[addr % 1024];
}

void switch_page_directory(t_mempage_directory *dir)
{
	current_directory = dir;
	activate_paging((void *)dir->phys_addr);
}

void init_paging(void)
{
	extern uint32_t _kernel_end;
	uint32_t kernel_end = (uint32_t)&_kernel_end;
	uint32_t max_map;

	placement_address = (kernel_end + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
	init_frames();

	kernel_directory = (t_mempage_directory *)kmem_early_alloc(sizeof(t_mempage_directory), 1);
	ft_memset(kernel_directory, 0, sizeof(t_mempage_directory));
	kernel_directory->phys_addr = (uint32_t)kernel_directory->tables_phys;

	max_map = g_memory_limit;
	if (max_map == 0 || max_map > 0x01000000)
		max_map = 0x01000000;
	paging_identity_map(0, max_map);
	switch_page_directory(kernel_directory);

	for (uint32_t addr = KHEAP_START; addr < KHEAP_START + KHEAP_INITIAL_SIZE; addr += PAGE_SIZE)
	{
		t_mempage *page = create_page(addr, kernel_directory);
		alloc_frame(page, 1, 1);
	}
	kheap = create_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, KHEAP_MAX, 1, 0);
}
