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
    kernel_halt("System halted");
}

void handle_shutdown() {
    terminal_writestring("Shutting down...\n");

    outw(ACPI_SHUTDOWN_PORT, SHUTDOWN_CMD); /* ACPI shutdown */
    outw(APM_SHUTDOWN_PORT, SHUTDOWN_CMD);  /* APM shutdown */

    terminal_writestring("Shutdown failed, halting CPU\n");
    kernel_halt("Shutdown failed");
}

static uint32_t parse_uint(const char *arg)
{
	uint32_t value = 0;
	int i = 0;

	if (arg == NULL)
		return 0;
	while (arg[i] == ' ')
		i++;
	while (arg[i] >= '0' && arg[i] <= '9')
	{
		value = value * 10 + (uint32_t)(arg[i] - '0');
		i++;
	}
	return value;
}

void handle_layout(const char *arg)
{
	if (arg == NULL || arg[0] == '\0')
	{
		terminal_writestring("Usage: layout qwerty|azerty\n");
		return;
	}
	if (ft_strcmp(arg, "qwerty") == 0)
	{
		keyboard_set_layout(KEYBOARD_LAYOUT_QWERTY);
		terminal_writestring("Layout set to qwerty\n");
	}
	else if (ft_strcmp(arg, "azerty") == 0)
	{
		keyboard_set_layout(KEYBOARD_LAYOUT_AZERTY);
		terminal_writestring("Layout set to azerty\n");
	}
	else
		terminal_writestring("Unknown layout\n");
}

void handle_getline(void)
{
	if (kernel.line_capture_active)
	{
		terminal_writestring("getline already active\n");
		return;
	}
	kernel.line_capture_active = 1;
	kernel.line_pos = 0;
	kernel.skip_next_prompt = 1;
	terminal_writestring("Enter line: ");
}

void handle_syscall(const char *arg)
{
	uint32_t num = parse_uint(arg);
	uint32_t result = 0;

	__asm__ volatile ("int $0x80" : "=a"(result) : "a"(num) : "memory");
	terminal_writestring("syscall result: 0x");
	printnbr(result, 16);
	terminal_writestring("\n");
}
