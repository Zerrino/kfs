/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:14:06 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/09 20:43:38 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kernel.h"

void handle_clear() {
	for (size_t y = 0; y < VGA_HEIGHT * NB_SCROLL; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			kernel.screens[kernel.screen_index].content[index] = vga_entry(' ', kernel.screens[kernel.screen_index].color);
		}
	}
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			kernel.terminal_buffer[index] = vga_entry(' ', kernel.screens[kernel.screen_index].color);
		}
	}

	kernel.screens[kernel.screen_index].row = 0;
	kernel.screens[kernel.screen_index].column = 0;
}

void handle_reboot() {
    terminal_writestring("Rebooting...\n");
    outb(REBOOT_PORT, REBOOT_CMD);
}

void handle_halt() {
    terminal_writestring("System halted\n");
    __asm__ volatile("hlt");
}

void handle_shutdown() {
    terminal_writestring("Shutting down...\n");

    outw(ACPI_SHUTDOWN_PORT, SHUTDOWN_CMD); /* ACPI shutdown */
    outw(APM_SHUTDOWN_PORT, SHUTDOWN_CMD);  /* APM shutdown */

    terminal_writestring("Shutdown failed, halting CPU\n");
    __asm__ volatile("cli; hlt");
}
