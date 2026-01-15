/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:54:32 by alexafer          #+#    #+#             */
/*   Updated: 2025/12/23 18:40:21 by alexafer         ###   ########.fr       */
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
	signal_init();
	signal_register(SIGNAL_TIMER_TICK, signal_timer_handler);
	signal_register(SIGNAL_KEYBOARD, signal_keyboard_handler);
	signal_register(SIGNAL_SYSCALL, signal_syscall_handler);
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

extern uint32_t _kernel_start;
extern uint32_t _kernel_end;

void kernel_main(uint32_t magic, uint32_t multiboot_info_ptr)
{
	kernel.terminal_buffer = (uint16_t *)(0x000B8000);
	(void)multiboot_info_ptr;
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		while (1)
			__asm__ volatile ("hlt");
	}
	multiboot_info_t *mbi = (multiboot_info_t *)multiboot_info_ptr;
	if (mbi->flags & (1 << 0))
	{
		uint32_t lower_kb = mbi->mem_lower;
		uint32_t upper_kb = mbi->mem_upper;

		g_memory_limit = (uint32_t)((lower_kb + upper_kb) * 1024ULL);
	}
	else
	{
		g_memory_limit = 0;
	}
	terminal_initialize();








	//initMemory();

	//multiboot_info_t *mbi = (multiboot_info_t *)multiboot_info_ptr;

	//initMemory(mbi->mem_upper * 1024, physicalAllocStart);
	//kmallocInit(0x1000);
	//parse_multiboot1(mbi);
	//gdt_install();

	while (1)
	{
		__asm__ volatile ("hlt");
		signal_dispatch();
	}
}
