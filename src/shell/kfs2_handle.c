/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kfs2_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:20 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/05 14:10:56 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

void shell_initialize() {
    terminal_writestring("KFS Shell v1.0\n");
    terminal_writestring("Type 'help' for available commands\n");
    terminal_writestring("> ");

}

void handle_help() {
    terminal_writestring("Available commands:\n");
    terminal_writestring("  help         - Display this help message\n");
    terminal_writestring("  stack        - Print the kernel stack\n");
    terminal_writestring("  push <hex>   - Push a value onto the stack\n");
    terminal_writestring("  pop          - Pop a value from the stack\n");
    terminal_writestring("  clear        - Clear the screen\n");
    terminal_writestring("  reboot       - Reboot the system\n");
    terminal_writestring("  halt         - Halt the system\n");
    terminal_writestring("  shutdown     - Shutdown the system\n");
    terminal_writestring("\nMemory Management Commands:\n");
    terminal_writestring("  memstats     - Display memory statistics\n");
    terminal_writestring("  memtest      - Run comprehensive memory tests\n");
    terminal_writestring("  memtest1     - Basic allocation tests\n");
    terminal_writestring("  memtest2     - Multiple allocation tests\n");
    terminal_writestring("  memtest3     - Large allocation tests\n");
    terminal_writestring("  memtest4     - Virtual memory analysis\n");
    terminal_writestring("  memtest4debug- Debug virtual memory issues\n");
    terminal_writestring("  memtest5     - Edge case tests\n");
    terminal_writestring("  crashtest    - Test kernel panic levels\n");
    terminal_writestring("  vmemstats    - Display virtual memory statistics\n");
    terminal_writestring("  memcheck     - Check memory integrity\n");
}

void handle_stack() {
    print_kernel_stack();
}

void handle_push(const char* arg) {
    if (arg[0] == '\0') {
        terminal_writestring("Error: push requires a hexadecimal value\n");
        return;
    }

    uint32_t value = 0;
    int i = 0;

    if (arg[0] == '0' && (arg[1] == 'x' || arg[1] == 'X'))
        i = 2;

    while (arg[i] != '\0') {
        value = value * 16;

        if (arg[i] >= '0' && arg[i] <= '9')
            value += arg[i] - '0';
        else if (arg[i] >= 'a' && arg[i] <= 'f')
            value += arg[i] - 'a' + 10;
        else if (arg[i] >= 'A' && arg[i] <= 'F')
            value += arg[i] - 'A' + 10;
        else {
            terminal_writestring("Error: Invalid hex value\n");
            return;
        }
        i++;
    }

    if (value != 0 || (arg[0] == '0' && arg[1] == '\0')) {
        stack_push(value);
        terminal_writestring("Pushed 0x");

        for (int j = 7; j >= 0; j--) {
            uint8_t nibble = (value >> (j * 4)) & 0xF;
            char hex_char;
            if (nibble < 10)
                hex_char = '0' + nibble;
            else
                hex_char = 'A' + (nibble - 10);
            terminal_putchar(hex_char);
        }

        terminal_writestring(" onto the stack\n");
    }
}

void handle_pop() {
    if (stack_is_empty()) {
        terminal_writestring("Error: Stack is empty\n");
        return;
    }

    uint32_t value = stack_pop();
    terminal_writestring("Popped 0x");

    for (int j = 7; j >= 0; j--) {
        uint8_t nibble = (value >> (j * 4)) & 0xF;
        char hex_char;
        if (nibble < 10)
            hex_char = '0' + nibble;
        else
            hex_char = 'A' + (nibble - 10);
        terminal_putchar(hex_char);
    }

    terminal_writestring(" from the stack\n");
}

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
    outb(0x64, 0xFE);
}

void handle_halt() {
    terminal_writestring("System halted\n");
    __asm__ volatile("hlt");
}

void handle_shutdown() {
    terminal_writestring("Shutting down...\n");

    outw(0xB004, 0x2000); /* ACPI shutdown */
    outw(0x604, 0x2000);  /* APM shutdown */

    terminal_writestring("Shutdown failed, halting CPU\n");
    __asm__ volatile("cli; hlt");
}

void handle_unknown(const char* command) {
    terminal_writestring("Unknown command: ");
    terminal_writestring(command);
    terminal_writestring("\n");
}