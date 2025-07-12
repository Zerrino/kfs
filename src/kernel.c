/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerrino <zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:54:32 by alexafer          #+#    #+#             */
/*   Updated: 2025/07/12 23:56:18 by zerrino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

t_kernel	kernel = {0};

/* ──────────── Kernel Execution Stack (GDT-integrated) ──────────── */
uint8_t kernel_execution_stack[KERNEL_EXECUTION_STACK_SIZE] __attribute__((aligned(16)));
uint32_t *kernel_stack_top = (uint32_t*)((uint8_t*)kernel_execution_stack + KERNEL_EXECUTION_STACK_SIZE);

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

	DisableInterrupts();
	gdt_install();
	IDT_Initialize();
	ISR_Initialize();
	IRQ_Initialize();
	keyboard_init();
	EnableInterrupts();
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

void kernel_main(uint32_t magic, uint32_t multiboot_info_ptr)
{

	kernel.terminal_buffer = (uint16_t *)VGA_MEMORY;
	(void)multiboot_info_ptr;
	terminal_initialize();
	switch_to_kernel_stack();
	printnbr(magic, 16);

	while (1)
		__asm__ volatile ("hlt");
}
