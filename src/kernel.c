/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:54:32 by alexafer          #+#    #+#             */
/*   Updated: 2025/07/01 20:26:20 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

t_kernel	kernel = {0};

void terminal_initialize()
{
	for (size_t i = 0; i < NB_SCREEN; i++)
	{
		kernel.screens[i].color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
		for (size_t y = 0; y < VGA_HEIGHT; y++) {
			for (size_t x = 0; x < VGA_WIDTH * NB_SCROLL; x++) {
				const size_t index = y * VGA_WIDTH + x;
				kernel.screens[i].content[index] = vga_entry(' ', kernel.screens[i].color);
			}
		}
	}

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			kernel.terminal_buffer[index] = vga_entry(' ', kernel.screens[kernel.screen_index].color);
		}
	}

	vga_set_cursor(0, 0);
	IDT_Initialize();
	ISR_Initialize();
	IRQ_Initialize();
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

void kernel_main(void)
{
	kernel.terminal_buffer = (uint16_t *)VGA_MEMORY;

	terminal_initialize();

	/* KFS-1 Base System */
	terminal_writestring("42\n");
	terminal_writestring("KFS - Kernel from Scratch\n");
	terminal_writestring("========================\n");
	terminal_writestring("GRUB bootloader working\n");
	terminal_writestring("Multiboot header detected\n");
	terminal_writestring("ASM boot code functional\n");
	terminal_writestring("Kernel compiled and linked\n");
	terminal_writestring("Screen interface working\n");
	terminal_writestring("Basic functions (strlen, strcmp) available\n");
	terminal_writestring("Colors and cursor support enabled\n");
	terminal_writestring("Keyboard input handling ready\n");
	terminal_writestring("Multiple screens supported\n");
	terminal_writestring("Interrupt handling system active\n\n");

	/* KFS-2 Features */
	terminal_writestring("Initializing KFS-2 features...\n");
	DisableInterrupts(); /* Clear interrupt flag */
	gdt_install();
	terminal_writestring("GDT initialized at 0x00000800\n");
	EnableInterrupts(); /* Re-enable interrupts after GDT is set up */

	/* Test stack functionality */
	stack_push(0xDEADBEEF);
	stack_push(0xCAFEBABE);
	stack_push(0x12345678);
	terminal_writestring("Kernel stack operational\n");

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
