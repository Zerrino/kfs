/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:20 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/01 20:24:52 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

#define COMMAND_BUFFER_SIZE 256
static char command_buffer[COMMAND_BUFFER_SIZE];
static int buffer_pos = 0;

typedef enum {
    CMD_HELP,
    CMD_STACK,
    CMD_PUSH,
    CMD_POP,
    CMD_CLEAR,
    CMD_REBOOT,
    CMD_HALT,
    CMD_SHUTDOWN,
    CMD_MEMSTATS,
    CMD_MEMTEST,
    CMD_VMEMSTATS,
    CMD_MEMCHECK,
    CMD_UNKNOWN
} command_type_t;

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
    terminal_writestring("  memtest      - Run memory allocation tests\n");
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
    terminal_initialize();
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
    else if (strcmp(command, "memstats") == 0)
		cmd_type = CMD_MEMSTATS;
    else if (strcmp(command, "memtest") == 0)
		cmd_type = CMD_MEMTEST;
    else if (strcmp(command, "vmemstats") == 0)
		cmd_type = CMD_VMEMSTATS;
    else if (strcmp(command, "memcheck") == 0)
		cmd_type = CMD_MEMCHECK;
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
        case CMD_MEMSTATS:
            handle_memstats();
            break;
        case CMD_MEMTEST:
            handle_memtest();
            break;
        case CMD_VMEMSTATS:
            handle_vmemstats();
            break;
        case CMD_MEMCHECK:
            handle_memcheck();
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
        command_buffer[buffer_pos] = '\0';
        shell_process_command(command_buffer);
        buffer_pos = 0;
    } else if (c == '\b' && buffer_pos > 0) {
        buffer_pos--;
        terminal_putchar('\b');
        terminal_putchar(' ');
        terminal_putchar('\b');
    } else if (c >= ' ' && c <= '~' && buffer_pos < COMMAND_BUFFER_SIZE - 1) {
        command_buffer[buffer_pos++] = c;
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

/* Memory Management Command Handlers */

void handle_memstats() {
    terminal_writestring("\n=== Physical Memory Statistics ===\n");
    terminal_writestring("Total pages: ");
    printnbr(g_phys_mem_manager.total_pages, 10);
    terminal_writestring("\nUsed pages: ");
    printnbr(g_phys_mem_manager.used_pages, 10);
    terminal_writestring("\nFree pages: ");
    printnbr(g_phys_mem_manager.total_pages - g_phys_mem_manager.used_pages, 10);
    terminal_writestring("\n\n=== Kernel Heap Statistics ===\n");
    terminal_writestring("Heap start: 0x");
    printnbr(g_kernel_heap.start_addr, 16);
    terminal_writestring("\nHeap end: 0x");
    printnbr(g_kernel_heap.current_end, 16);
    terminal_writestring("\nHeap size: ");
    printnbr((g_kernel_heap.current_end - g_kernel_heap.start_addr) / 1024, 10);
    terminal_writestring(" KB\n");

    if (is_paging_enabled()) {
        terminal_writestring("\nPaging: ENABLED\n");
        terminal_writestring("Page directory: 0x");
        printnbr(get_cr3(), 16);
        terminal_writestring("\n");
    } else {
        terminal_writestring("\nPaging: DISABLED\n");
    }
}

void handle_memtest() {
    void *ptr1, *ptr2, *ptr3;

    terminal_writestring("\n=== Memory Allocation Test ===\n");

    /* Test kmalloc */
    terminal_writestring("Testing kmalloc...\n");
    ptr1 = kmalloc(1024);
    if (ptr1) {
        terminal_writestring("kmalloc(1024): SUCCESS - 0x");
        printnbr((uint32_t)ptr1, 16);
        terminal_writestring("\n");
    } else {
        terminal_writestring("kmalloc(1024): FAILED\n");
        return;
    }

    ptr2 = kmalloc(2048);
    if (ptr2) {
        terminal_writestring("kmalloc(2048): SUCCESS - 0x");
        printnbr((uint32_t)ptr2, 16);
        terminal_writestring("\n");
    } else {
        terminal_writestring("kmalloc(2048): FAILED\n");
        kfree(ptr1);
        return;
    }

    /* Test ksize */
    terminal_writestring("Size of first allocation: ");
    printnbr(ksize(ptr1), 10);
    terminal_writestring(" bytes\n");

    /* Test kfree */
    terminal_writestring("Testing kfree...\n");
    kfree(ptr1);
    terminal_writestring("kfree(ptr1): SUCCESS\n");

    /* Test vmalloc */
    terminal_writestring("Testing vmalloc...\n");
    ptr3 = vmalloc(4096);
    if (ptr3) {
        terminal_writestring("vmalloc(4096): SUCCESS - 0x");
        printnbr((uint32_t)ptr3, 16);
        terminal_writestring("\n");
    } else {
        terminal_writestring("vmalloc(4096): FAILED\n");
        kfree(ptr2);
        return;
    }

    /* Test vsize */
    terminal_writestring("Size of virtual allocation: ");
    printnbr(vsize(ptr3), 10);
    terminal_writestring(" bytes\n");

    /* Clean up */
    kfree(ptr2);
    vfree(ptr3);
    terminal_writestring("Memory test completed successfully!\n");
}

void handle_vmemstats() {
    vmem_print_stats();
}

void handle_memcheck() {
    terminal_writestring("\n=== Memory Integrity Check ===\n");

    /* Check if memory management is initialized */
    if (g_phys_mem_manager.total_pages == 0) {
        terminal_writestring("ERROR: Physical memory manager not initialized\n");
        return;
    }

    if (g_kernel_heap.start_addr == 0) {
        terminal_writestring("ERROR: Kernel heap not initialized\n");
        return;
    }

    if (!g_current_directory) {
        terminal_writestring("ERROR: Page directory not set\n");
        return;
    }

    terminal_writestring("Physical memory manager: OK\n");
    terminal_writestring("Kernel heap: OK\n");
    terminal_writestring("Paging system: OK\n");

    /* Check for memory leaks by comparing allocated vs used pages */
    uint32_t used_pages = g_phys_mem_manager.used_pages;
    uint32_t expected_pages = (g_kernel_heap.current_end - g_kernel_heap.start_addr) / PAGE_SIZE;
    expected_pages += 1; /* Page directory */

    terminal_writestring("Used pages: ");
    printnbr(used_pages, 10);
    terminal_writestring("\nExpected minimum: ");
    printnbr(expected_pages, 10);
    terminal_writestring("\n");

    if (used_pages >= expected_pages) {
        terminal_writestring("Memory usage: NORMAL\n");
    } else {
        terminal_writestring("WARNING: Unexpected memory usage pattern\n");
    }

    terminal_writestring("Memory integrity check completed\n");
}
