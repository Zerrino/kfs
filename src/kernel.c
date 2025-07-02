/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerrino <zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:54:32 by alexafer          #+#    #+#             */
/*   Updated: 2025/07/02 18:27:59 by zerrino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

t_kernel	kernel = {0};

void terminal_initialize()
{
	for (size_t i = 0; i < NB_SCREEN; i++)
	{
		kernel.screens[i].color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
		for (size_t y = 0; y < VGA_HEIGHT * NB_SCROLL; y++) {
			for (size_t x = 0; x < VGA_WIDTH; x++) {
				const size_t index = y * VGA_WIDTH + x;
				kernel.screens[i].content[index] = vga_entry(' ', kernel.screens[i].color);
			}
		}
		kernel.screen_index = i;
		shell_initialize();
		kernel.screens[i].shell_mode = 1;
	}
	kernel.screen_index = 0;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			kernel.terminal_buffer[index] = kernel.screens[kernel.screen_index].content[index];
		}
	}
	IDT_Initialize();
	ISR_Initialize();
	IRQ_Initialize();
	DisableInterrupts();
	gdt_install();
	EnableInterrupts();
	stack_push(0xCAFEBABE);
	stack_push(0x12345678);
}

void terminal_offset(uint16_t offset)
{
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			kernel.terminal_buffer[index] = vga_entry(kernel.screens[kernel.screen_index].content[index + (VGA_WIDTH * offset)], kernel.screens[kernel.screen_index].color);
		}
	}
}

void terminal_restore()
{
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			kernel.terminal_buffer[index] = vga_entry(kernel.screens[kernel.screen_index].content[index + (VGA_WIDTH * kernel.screens[kernel.screen_index].offset)], kernel.screens[kernel.screen_index].color);
		}
	}
}

void kernel_main(void)
{
	kernel.terminal_buffer = (uint16_t *)VGA_MEMORY;

	terminal_initialize();



	/* Test stack functionality */
	stack_push(0xDEADBEEF);
	stack_push(0xCAFEBABE);
	stack_push(0x12345678);
	terminal_writestring("Kernel stack operational\n");

	/* KFS-3 Memory Management Features */
	terminal_writestring("\nInitializing KFS-3 memory management...\n");
	DisableInterrupts(); /* Disable interrupts during memory initialization */

	/* Initialize physical memory manager (assume 16MB for now) */
	phys_mem_init(16 * 1024 * 1024);

	/* Initialize paging system */
	paging_init();

	/* Initialize kernel memory allocator */
	kmem_init();

	/* Initialize panic system */
	panic_init();

	/* Enable paging */
	paging_enable();

	EnableInterrupts(); /* Re-enable interrupts */
	terminal_writestring("Memory management system initialized\n");

	terminal_writestring("\nAll systems ready!\n");
	terminal_writestring("Use arrow keys to navigate, Ctrl+Shift+arrows for screens\n");
	terminal_writestring("Type to enter shell mode, ESC to exit shell\n\n");

	/* Initialize shell */
	shell_initialize();

	while (1)
	{
		__asm__ volatile ("hlt");
	}
}
