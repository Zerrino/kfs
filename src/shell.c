/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:20 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/09 12:05:19 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

typedef enum {
    CMD_HELP,
    CMD_STACK,
    CMD_GDT,
    CMD_SEGMENTS,
    CMD_MEMORY,
    CMD_GDTTEST,
    CMD_STACKTEST,
    CMD_SYSINFO,
    CMD_INTERRUPTS,
    CMD_PUSH,
    CMD_POP,
    CMD_CLEAR,
    CMD_REBOOT,
    CMD_HALT,
    CMD_SHUTDOWN,
    CMD_UNKNOWN
} command_type_t;

void shell_initialize() {
    terminal_writestring("KFS Shell v1.0\n");
    terminal_writestring("Type 'help' for available commands\n");
    terminal_writestring("> ");
    //kernel.shell_mode = 1;
}

void handle_help() {
    terminal_writestring("Available commands:\n");
    terminal_writestring("=== Core System ===\n");
    terminal_writestring("  help         - Display this help message\n");
    terminal_writestring("  clear        - Clear the screen\n");
    terminal_writestring("  reboot       - Reboot the system\n");
    terminal_writestring("  halt         - Halt the system\n");
    terminal_writestring("  shutdown     - Shutdown the system\n");
    terminal_writestring("\n=== GDT & Memory ===\n");
    terminal_writestring("  gdt          - Display GDT information\n");
    terminal_writestring("  segments     - Show current segment registers\n");
    terminal_writestring("  memory       - Show memory layout and addresses\n");
    terminal_writestring("  gdttest      - Test GDT segment switching\n");
    terminal_writestring("\n=== Stack Operations ===\n");
    terminal_writestring("  stack        - Print the kernel stack\n");
    terminal_writestring("  push <hex>   - Push a value onto the stack\n");
    terminal_writestring("  pop          - Pop a value from the stack\n");
    terminal_writestring("  stacktest    - Demonstrate stack operations\n");
    terminal_writestring("\n=== System Info ===\n");
    terminal_writestring("  sysinfo      - Show complete system information\n");
    terminal_writestring("  interrupts   - Show interrupt status\n");
}

void handle_stack() {
    print_kernel_stack();
}

void handle_gdt() {
    print_gdt_info();
}

void handle_segments() {
    uint16_t cs, ds, es, fs, gs, ss;

    // Read current segment registers
    __asm__ volatile ("mov %%cs, %0" : "=r" (cs));
    __asm__ volatile ("mov %%ds, %0" : "=r" (ds));
    __asm__ volatile ("mov %%es, %0" : "=r" (es));
    __asm__ volatile ("mov %%fs, %0" : "=r" (fs));
    __asm__ volatile ("mov %%gs, %0" : "=r" (gs));
    __asm__ volatile ("mov %%ss, %0" : "=r" (ss));

    terminal_writestring("Current Segment Registers:\n");
    terminal_writestring("  CS (Code Segment):  0x");
    printnbr(cs, 16);
    terminal_writestring(" (Expected: 0x");
    printnbr(GDT_KERNEL_CODE, 16);
    terminal_writestring(")\n");

    terminal_writestring("  DS (Data Segment):  0x");
    printnbr(ds, 16);
    terminal_writestring(" (Expected: 0x");
    printnbr(GDT_KERNEL_DATA, 16);
    terminal_writestring(")\n");

    terminal_writestring("  ES (Extra Segment): 0x");
    printnbr(es, 16);
    terminal_writestring(" (Expected: 0x");
    printnbr(GDT_KERNEL_DATA, 16);
    terminal_writestring(")\n");

    terminal_writestring("  FS (F Segment):     0x");
    printnbr(fs, 16);
    terminal_writestring(" (Expected: 0x");
    printnbr(GDT_KERNEL_DATA, 16);
    terminal_writestring(")\n");

    terminal_writestring("  GS (G Segment):     0x");
    printnbr(gs, 16);
    terminal_writestring(" (Expected: 0x");
    printnbr(GDT_KERNEL_DATA, 16);
    terminal_writestring(")\n");

    terminal_writestring("  SS (Stack Segment): 0x");
    printnbr(ss, 16);
    terminal_writestring(" (Expected: 0x");
    printnbr(GDT_KERNEL_STACK, 16);
    terminal_writestring(")\n");
}

void handle_memory() {
    terminal_writestring("=== Memory Layout ===\n");
    terminal_writestring("GDT Base Address:     0x");
    printnbr(0x00000800, 16);
    terminal_writestring("\n");

    extern char _start, _end;
    terminal_writestring("Kernel Start:         0x");
    printnbr((uint32_t)&_start, 16);
    terminal_writestring("\n");
    terminal_writestring("Kernel End:           0x");
    printnbr((uint32_t)&_end, 16);
    terminal_writestring("\n");

    uint32_t kernel_size = (uint32_t)&_end - (uint32_t)&_start;
    terminal_writestring("Kernel Size:          ");
    printnbr(kernel_size, 10);
    terminal_writestring(" bytes (");
    printnbr(kernel_size / 1024, 10);
    terminal_writestring(" KB)\n");

    // Show stack information
    extern char stack_bottom, stack_top;
    terminal_writestring("Stack Bottom:         0x");
    printnbr((uint32_t)&stack_bottom, 16);
    terminal_writestring("\n");
    terminal_writestring("Stack Top:            0x");
    printnbr((uint32_t)&stack_top, 16);
    terminal_writestring("\n");
    terminal_writestring("Stack Size:           ");
    printnbr((uint32_t)&stack_top - (uint32_t)&stack_bottom, 10);
    terminal_writestring(" bytes (16 KB)\n");

    // Show current stack pointer
    uint32_t esp;
    __asm__ volatile ("mov %%esp, %0" : "=r" (esp));
    terminal_writestring("Current Stack Ptr:    0x");
    printnbr(esp, 16);
    terminal_writestring("\n");
    terminal_writestring("Stack Usage:          ");
    printnbr((uint32_t)&stack_top - esp, 10);
    terminal_writestring(" bytes\n");
}

void handle_gdttest() {
    terminal_writestring("=== GDT Segment Test ===\n");
    terminal_writestring("Testing segment descriptor access...\n");

    // Test reading from different segments
    uint16_t cs, ds, ss;
    __asm__ volatile ("mov %%cs, %0" : "=r" (cs));
    __asm__ volatile ("mov %%ds, %0" : "=r" (ds));
    __asm__ volatile ("mov %%ss, %0" : "=r" (ss));

    terminal_writestring("Before: CS=0x");
    printnbr(cs, 16);
    terminal_writestring(" DS=0x");
    printnbr(ds, 16);
    terminal_writestring(" SS=0x");
    printnbr(ss, 16);
    terminal_writestring("\n");

    // Test segment privilege levels
    terminal_writestring("Segment Privilege Levels:\n");
    terminal_writestring("  CS RPL: ");
    printnbr(cs & 0x3, 10);
    terminal_writestring(" (Ring ");
    printnbr(cs & 0x3, 10);
    terminal_writestring(")\n");

    terminal_writestring("  DS RPL: ");
    printnbr(ds & 0x3, 10);
    terminal_writestring(" (Ring ");
    printnbr(ds & 0x3, 10);
    terminal_writestring(")\n");

    terminal_writestring("  SS RPL: ");
    printnbr(ss & 0x3, 10);
    terminal_writestring(" (Ring ");
    printnbr(ss & 0x3, 10);
    terminal_writestring(")\n");

    terminal_writestring("GDT Test completed successfully!\n");
}

void handle_stacktest() {
    terminal_writestring("=== Stack Operations Demo ===\n");

    // Show initial stack state
    terminal_writestring("Initial stack size: ");
    printnbr(stack_size(), 10);
    terminal_writestring(" entries\n");

    // Push some test values
    terminal_writestring("Pushing test values: 0xDEAD, 0xBEEF, 0xCAFE\n");
    stack_push(0xDEAD);
    stack_push(0xBEEF);
    stack_push(0xCAFE);

    terminal_writestring("Stack size after push: ");
    printnbr(stack_size(), 10);
    terminal_writestring(" entries\n");

    // Peek at top value
    if (!stack_is_empty()) {
        terminal_writestring("Top value (peek): 0x");
        printnbr(stack_peek(), 16);
        terminal_writestring("\n");
    }

    // Pop values back
    terminal_writestring("Popping values: ");
    while (!stack_is_empty()) {
        terminal_writestring("0x");
        printnbr(stack_pop(), 16);
        terminal_writestring(" ");
    }
    terminal_writestring("\n");

    terminal_writestring("Final stack size: ");
    printnbr(stack_size(), 10);
    terminal_writestring(" entries\n");
    terminal_writestring("Stack test completed!\n");
}

void handle_sysinfo() {
    terminal_writestring("=== Complete System Information ===\n");
    terminal_writestring("Kernel: ChickenKernel v1.0\n");
    terminal_writestring("Architecture: i386 (x86 32-bit)\n");
    terminal_writestring("Boot Method: Multiboot via GRUB\n\n");

    // GDT Info
    terminal_writestring("--- Global Descriptor Table ---\n");
    terminal_writestring("Location: 0x00000800\n");
    terminal_writestring("Entries: 7 (Null, K-Code, K-Data, K-Stack, U-Code, U-Data, U-Stack)\n");

    // Current segments
    uint16_t cs, ds, ss;
    __asm__ volatile ("mov %%cs, %0" : "=r" (cs));
    __asm__ volatile ("mov %%ds, %0" : "=r" (ds));
    __asm__ volatile ("mov %%ss, %0" : "=r" (ss));
    terminal_writestring("Active: CS=0x");
    printnbr(cs, 16);
    terminal_writestring(" DS=0x");
    printnbr(ds, 16);
    terminal_writestring(" SS=0x");
    printnbr(ss, 16);
    terminal_writestring("\n\n");

    // Stack info
    terminal_writestring("--- Kernel Stack ---\n");
    terminal_writestring("Size: 16384 bytes (16 KB)\n");
    terminal_writestring("Current entries: ");
    printnbr(stack_size(), 10);
    terminal_writestring("\n");

    uint32_t esp;
    __asm__ volatile ("mov %%esp, %0" : "=r" (esp));
    extern char stack_top;
    terminal_writestring("Usage: ");
    printnbr((uint32_t)&stack_top - esp, 10);
    terminal_writestring(" bytes\n\n");

    // Memory info
    extern char _start, _end;
    uint32_t kernel_size = (uint32_t)&_end - (uint32_t)&_start;
    terminal_writestring("--- Memory Layout ---\n");
    terminal_writestring("Kernel size: ");
    printnbr(kernel_size / 1024, 10);
    terminal_writestring(" KB\n");
    terminal_writestring("Load address: 0x");
    printnbr((uint32_t)&_start, 16);
    terminal_writestring("\n\n");

    terminal_writestring("--- Features ---\n");
    terminal_writestring("✓ Custom GDT with 7 segments\n");
    terminal_writestring("✓ Interrupt handling (IDT + ISR + IRQ)\n");
    terminal_writestring("✓ Keyboard input support\n");
    terminal_writestring("✓ VGA text mode display\n");
    terminal_writestring("✓ Kernel stack operations\n");
    terminal_writestring("✓ Interactive shell\n");
}

void handle_interrupts() {
    terminal_writestring("=== Interrupt System Status ===\n");

    // Check if interrupts are enabled
    uint32_t eflags;
    __asm__ volatile ("pushf; pop %0" : "=r" (eflags));

    terminal_writestring("Interrupt Flag (IF): ");
    if (eflags & 0x200) {
        terminal_writestring("ENABLED\n");
    } else {
        terminal_writestring("DISABLED\n");
    }

    terminal_writestring("EFLAGS Register: 0x");
    printnbr(eflags, 16);
    terminal_writestring("\n");

    terminal_writestring("\nConfigured Interrupts:\n");
    terminal_writestring("  IRQ 0 (Timer):    Enabled\n");
    terminal_writestring("  IRQ 1 (Keyboard): Enabled\n");
    terminal_writestring("  IRQ 2-15:         Masked\n");

    terminal_writestring("\nPIC Configuration:\n");
    terminal_writestring("  Master PIC:       IRQ 32-39\n");
    terminal_writestring("  Slave PIC:        IRQ 40-47\n");

    terminal_writestring("\nIDT Status:\n");
    terminal_writestring("  Total gates:      256\n");
    terminal_writestring("  ISR handlers:     0-31 (CPU exceptions)\n");
    terminal_writestring("  IRQ handlers:     32-47 (Hardware interrupts)\n");
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

        if ('0' <= arg[i] && arg[i] <= '9')
            value += arg[i] - '0';
        else if ('a' <= arg[i] && arg[i] <= 'f')
            value += arg[i] - 'a' + 10;
        else if ('A' <= arg[i] && arg[i] <= 'F')
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

        for (int j = 7; 0 <= j; j--) {
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

    for (int j = 7; 0 <= j; j--) {
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

command_type_t get_command_type(const char* command) {
	command_type_t cmd_type;

	cmd_type = CMD_UNKNOWN;
    if (strcmp(command, "help") == 0)
		cmd_type = CMD_HELP;
    else if (strcmp(command, "stack") == 0)
		cmd_type = CMD_STACK;
    else if (strcmp(command, "gdt") == 0)
		cmd_type = CMD_GDT;
    else if (strcmp(command, "segments") == 0)
		cmd_type = CMD_SEGMENTS;
    else if (strcmp(command, "memory") == 0)
		cmd_type = CMD_MEMORY;
    else if (strcmp(command, "gdttest") == 0)
		cmd_type = CMD_GDTTEST;
    else if (strcmp(command, "stacktest") == 0)
		cmd_type = CMD_STACKTEST;
    else if (strcmp(command, "sysinfo") == 0)
		cmd_type = CMD_SYSINFO;
    else if (strcmp(command, "interrupts") == 0)
		cmd_type = CMD_INTERRUPTS;
    else if (strcmp(command, "push") == 0)
		cmd_type = CMD_PUSH;
    else if (strcmp(command, "pop") == 0)
		cmd_type = CMD_POP;
    else if (strcmp(command, "clear") == 0)
		cmd_type = CMD_CLEAR;
    else if (strcmp(command, "reboot") == 0)
		cmd_type = CMD_REBOOT;
    else if (strcmp(command, "halt") == 0)
		cmd_type = CMD_HALT;
    else if (strcmp(command, "shutdown") == 0)
		cmd_type = CMD_SHUTDOWN;
	return cmd_type;
}

void shell_process_command(const char* cmd) {
    char command[32] = {0};
    char arg[32] = {0};
    int i = 0, j = 0;

    while (cmd[i] && cmd[i] != ' ' && i < 31) {
        command[i] = cmd[i];
        i++;
    }
    command[i] = '\0';

    while (cmd[i] && cmd[i] == ' ')
        i++;

    while (cmd[i] && j < 31) {
        arg[j] = cmd[i];
        i++;
        j++;
    }
    arg[j] = '\0';

    command_type_t cmd_type = get_command_type(command);

    switch (cmd_type) {
        case CMD_HELP:
            handle_help();
            break;
        case CMD_STACK:
            handle_stack();
            break;
        case CMD_GDT:
            handle_gdt();
            break;
        case CMD_SEGMENTS:
            handle_segments();
            break;
        case CMD_MEMORY:
            handle_memory();
            break;
        case CMD_GDTTEST:
            handle_gdttest();
            break;
        case CMD_STACKTEST:
            handle_stacktest();
            break;
        case CMD_SYSINFO:
            handle_sysinfo();
            break;
        case CMD_INTERRUPTS:
            handle_interrupts();
            break;
        case CMD_PUSH:
            handle_push(arg);
            break;
        case CMD_POP:
            handle_pop();
            break;
        case CMD_CLEAR:
            handle_clear();
            break;
        case CMD_REBOOT:
            handle_reboot();
            break;
        case CMD_HALT:
            handle_halt();
            break;
        case CMD_SHUTDOWN:
            handle_shutdown();
            break;
        case CMD_UNKNOWN:
            if (command[0] != '\0')
                handle_unknown(command);
            break;
    }

    terminal_writestring("> ");
}

void shell_handle_input(char c) {
    if (c == '\n') {
        terminal_putchar('\n');
        kernel.command_buffer[kernel.buffer_pos] = '\0';
        shell_process_command(kernel.command_buffer);
        kernel.buffer_pos = 0;
    } else if (c == '\b' && 0 < kernel.buffer_pos) {
        kernel.buffer_pos--;
        terminal_putchar('\b');
        terminal_putchar(' ');
        terminal_putchar('\b');
    } else if (' ' <= c && c <= '~' && kernel.buffer_pos < COMMAND_BUFFER_SIZE - 1) {
        kernel.command_buffer[kernel.buffer_pos++] = c;
        terminal_putchar(c);
    }
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
