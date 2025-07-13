/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiboot.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerrino <zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 03:26:33 by zerrino           #+#    #+#             */
/*   Updated: 2025/07/13 04:19:33 by zerrino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kernel.h"

#define KERNEL_PHYS_BASE 0x0        /* tant que paging = identité        */

pmm_t pmm = {0};

void	*phys_to_virt(uintptr_t p)
{
	return (void *)(p + KERNEL_PHYS_BASE);
}


static inline void push(uint32_t page_num)
{
	if (pmm.stack_top >= MAX_PAGES) kernelPanic();
	pmm.stack[pmm.stack_top++] = page_num;
}

void *pmm_alloc_page(void)
{
	if (pmm.stack_top == 0) return NULL;          /* plus de RAM libre */
	uint32_t page_num = pmm.stack[--pmm.stack_top];
	return (void *)(page_num << 12);
}

void pmm_free_page(void *page)
{
	push(((uintptr_t)page) >> 12);
}

void add_region_to_pmm(uint64_t base, uint64_t length)
{
	uint64_t first = (base + PAGE_SIZE-1) & ~(PAGE_SIZE-1);   /* align up */
	uint64_t last  = (base + length) & ~(PAGE_SIZE-1);        /* align dn */

	for (uint64_t p = first; p < last; p += PAGE_SIZE)
		push(p >> 12);       /* ajoute à la pile -> page DISPONIBLE */
}

void parse_multiboot1(const multiboot_info_t *mbi)
{
	if (mbi->flags & (1 << 0))
	{
		uint32_t total_kib = mbi->mem_lower + mbi->mem_upper;
		terminal_writestring("RAM : ");
		printnbr(total_kib, 10);
		terminal_writestring("\n");
	}
	if (mbi->flags & (1 << 2))
	{
		const char *cmd = phys_to_virt(mbi->cmdline);
		terminal_writestring("cmdline: ");
		terminal_writestring(cmd);
		terminal_writestring("\n");
	}
	if (mbi->flags & (1 << 3))
	{
		const multiboot_module_t *mods = phys_to_virt(mbi->mods_addr);
		for (uint32_t i = 0; i < mbi->mods_count; ++i)
		{
			const char *name = phys_to_virt(mods[i].string);
			terminal_writestring("module ");
			printnbr(i, 10);
			terminal_writestring(" ");
			terminal_writestring(name);
			terminal_writestring(" ");
			printnbr(mods[i].mod_start, 16);
			terminal_writestring(" ");
			printnbr(mods[i].mod_end, 16);
			terminal_writestring("\n");
		}
	}
	if (mbi->flags & (1 << 6))
	{
		const uint8_t *scan = phys_to_virt(mbi->mmap_addr);
		const uint8_t *end  = scan + mbi->mmap_length;
		while (scan < end)
		{
			const multiboot_memory_map_t *e = (const void *)scan;
			if (e->type == 1)
			{
				add_region_to_pmm(e->addr, e->len);
			}
			scan += e->size + 4;
		}
	}
	terminal_writestring("page non used : ");
	printnbr(pmm.stack_top, 10);
	terminal_writestring(" - page used : ");
	printnbr(MAX_PAGES - pmm.stack_top, 10);
	terminal_writestring("\n");

	terminal_writestring("page non used : ");
	printnbr(pmm.stack_top, 10);
	terminal_writestring(" - page used : ");
	printnbr(MAX_PAGES - pmm.stack_top, 10);
	terminal_writestring("\n");

	if (mbi->flags & (1 << 9))
	{
		terminal_writestring("bootloader : ");
		terminal_writestring(phys_to_virt(mbi->boot_loader_name));
		terminal_writestring("\n");
	}
}
