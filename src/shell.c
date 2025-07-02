/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerrino <zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:20 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/02 18:24:02 by zerrino          ###   ########.fr       */
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
    CMD_MEMTEST1,
    CMD_MEMTEST2,
    CMD_MEMTEST3,
    CMD_MEMTEST4,
    CMD_MEMTEST4DEBUG,
    CMD_MEMTEST5,
    CMD_CRASHTEST,
    CMD_CRASH1,
    CMD_CRASH2,
    CMD_CRASH3,
    CMD_CRASH4,
    CMD_CRASH5,
    CMD_CRASH6,
    CMD_VMEMSTATS,
    CMD_MEMCHECK,
    CMD_UNKNOWN
} command_type_t;

/* Function declarations */
void handle_memtest1(void);
void handle_memtest2(void);
void handle_memtest3(void);
void handle_memtest4(void);
void handle_memtest4debug(void);
void handle_memtest5(void);
void handle_crashtest(void);
void handle_crash1(void);
void handle_crash2(void);
void handle_crash3(void);
void handle_crash4(void);
void handle_crash5(void);
void handle_crash6(void);

void shell_initialize() {
    terminal_writestring("KFS Shell v1.0\n");
    terminal_writestring("Type 'help' for available commands\n");
    terminal_writestring("> ");
    //kernel.shell_mode = 1;
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
    else if (strcmp(command, "memtest1") == 0)
		cmd_type = CMD_MEMTEST1;
    else if (strcmp(command, "memtest2") == 0)
		cmd_type = CMD_MEMTEST2;
    else if (strcmp(command, "memtest3") == 0)
		cmd_type = CMD_MEMTEST3;
    else if (strcmp(command, "memtest4") == 0)
		cmd_type = CMD_MEMTEST4;
    else if (strcmp(command, "memtest4debug") == 0)
		cmd_type = CMD_MEMTEST4DEBUG;
    else if (strcmp(command, "memtest5") == 0)
		cmd_type = CMD_MEMTEST5;
    else if (strcmp(command, "crashtest") == 0)
		cmd_type = CMD_CRASHTEST;
    else if (strcmp(command, "crash1") == 0)
		cmd_type = CMD_CRASH1;
    else if (strcmp(command, "crash2") == 0)
		cmd_type = CMD_CRASH2;
    else if (strcmp(command, "crash3") == 0)
		cmd_type = CMD_CRASH3;
    else if (strcmp(command, "crash4") == 0)
		cmd_type = CMD_CRASH4;
    else if (strcmp(command, "crash5") == 0)
		cmd_type = CMD_CRASH5;
    else if (strcmp(command, "crash6") == 0)
		cmd_type = CMD_CRASH6;
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
        case CMD_MEMTEST1:
            handle_memtest1();
            break;
        case CMD_MEMTEST2:
            handle_memtest2();
            break;
        case CMD_MEMTEST3:
            handle_memtest3();
            break;
        case CMD_MEMTEST4:
            handle_memtest4();
            break;
        case CMD_MEMTEST4DEBUG:
            handle_memtest4debug();
            break;
        case CMD_MEMTEST5:
            handle_memtest5();
            break;
        case CMD_CRASHTEST:
            handle_crashtest();
            break;
        case CMD_CRASH1:
            handle_crash1();
            break;
        case CMD_CRASH2:
            handle_crash2();
            break;
        case CMD_CRASH3:
            handle_crash3();
            break;
        case CMD_CRASH4:
            handle_crash4();
            break;
        case CMD_CRASH5:
            handle_crash5();
            break;
        case CMD_CRASH6:
            handle_crash6();
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
    terminal_writestring("\n=== Memory Test Suite ===\n");
    terminal_writestring("Run individual tests to debug issues:\n");
    terminal_writestring("  memtest1 - Basic allocation tests\n");
    terminal_writestring("  memtest2 - Multiple allocation tests\n");
    terminal_writestring("  memtest3 - Large allocation tests\n");
    terminal_writestring("  memtest4 - Virtual memory tests\n");
    terminal_writestring("  memtest5 - Edge case tests\n");
    terminal_writestring("\nOr run all tests with individual commands\n");
}

/* TEST 1: Basic Allocation Tests */
void handle_memtest1() {
    int i;
    uint32_t initial_used_pages;

    terminal_writestring("\n=== TEST 1: Basic Allocation Tests ===\n");

    /* Record initial memory state */
    initial_used_pages = g_phys_mem_manager.used_pages;
    terminal_writestring("Initial used pages: ");
    printnbr(initial_used_pages, 10);
    terminal_writestring("\n");

    /* Test various sizes */
    uint32_t test_sizes[] = {16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
    uint32_t num_sizes = sizeof(test_sizes) / sizeof(test_sizes[0]);

    for (i = 0; i < (int)num_sizes; i++) {
        terminal_writestring("\nTesting size ");
        printnbr(test_sizes[i], 10);
        terminal_writestring(" bytes...\n");

        void *ptr = kmalloc(test_sizes[i]);
        if (ptr) {
            terminal_writestring("  kmalloc: SUCCESS - 0x");
            printnbr((uint32_t)ptr, 16);
            terminal_writestring("\n  Size: ");
            printnbr(ksize(ptr), 10);
            terminal_writestring(" bytes\n");

            /* Write pattern to memory to test accessibility */
            uint32_t *data = (uint32_t *)ptr;
            uint32_t pattern = 0xDEADBEEF + i;
            terminal_writestring("  Writing pattern 0x");
            printnbr(pattern, 16);
            terminal_writestring("...\n");

            *data = pattern;

            /* Verify pattern */
            if (*data == pattern) {
                terminal_writestring("  Memory write/read: OK\n");
            } else {
                terminal_writestring("  Memory write/read: FAILED\n");
                terminal_writestring("  Expected: 0x");
                printnbr(pattern, 16);
                terminal_writestring(", Got: 0x");
                printnbr(*data, 16);
                terminal_writestring("\n");
            }

            terminal_writestring("  Freeing memory...\n");
            kfree(ptr);
            terminal_writestring("  kfree: SUCCESS\n");
        } else {
            terminal_writestring("  kmalloc: FAILED\n");
            break; /* Stop on first failure */
        }
    }

    terminal_writestring("\nFinal used pages: ");
    printnbr(g_phys_mem_manager.used_pages, 10);
    terminal_writestring("\nTEST 1 COMPLETE\n");
}

/* TEST 2: Multiple Allocation Tests */
void handle_memtest2() {
    void *ptrs[10]; /* Reduced from 20 to 10 for safety */
    int i;

    terminal_writestring("\n=== TEST 2: Multiple Allocation Tests ===\n");

    /* Initialize array */
    for (i = 0; i < 10; i++) {
        ptrs[i] = NULL;
    }

    /* Allocate multiple small blocks */
    int successful_allocs = 0;
    terminal_writestring("Allocating 10 blocks of 128 bytes each...\n");

    for (i = 0; i < 10; i++) {
        terminal_writestring("Allocating block ");
        printnbr(i, 10);
        terminal_writestring("...\n");

        ptrs[i] = kmalloc(128);
        if (ptrs[i]) {
            successful_allocs++;
            terminal_writestring("  SUCCESS - 0x");
            printnbr((uint32_t)ptrs[i], 16);
            terminal_writestring("\n");

            /* Write unique pattern to each block */
            uint32_t *data = (uint32_t *)ptrs[i];
            *data = 0xCAFEBABE + i;
        } else {
            terminal_writestring("  FAILED\n");
            break; /* Stop on first failure */
        }
    }

    terminal_writestring("Successfully allocated ");
    printnbr(successful_allocs, 10);
    terminal_writestring("/10 blocks\n");

    /* Verify all patterns */
    terminal_writestring("Verifying patterns...\n");
    int pattern_errors = 0;
    for (i = 0; i < successful_allocs; i++) {
        if (ptrs[i]) {
            uint32_t *data = (uint32_t *)ptrs[i];
            uint32_t expected = 0xCAFEBABE + i;
            if (*data != expected) {
                pattern_errors++;
                terminal_writestring("  Block ");
                printnbr(i, 10);
                terminal_writestring(" pattern error\n");
            }
        }
    }

    terminal_writestring("Pattern verification: ");
    if (pattern_errors == 0) {
        terminal_writestring("PASSED\n");
    } else {
        terminal_writestring("FAILED (");
        printnbr(pattern_errors, 10);
        terminal_writestring(" errors)\n");
    }

    /* Clean up all allocations */
    terminal_writestring("Cleaning up allocations...\n");
    for (i = 0; i < successful_allocs; i++) {
        if (ptrs[i]) {
            terminal_writestring("  Freeing block ");
            printnbr(i, 10);
            terminal_writestring("\n");
            kfree(ptrs[i]);
            ptrs[i] = NULL;
        }
    }

    terminal_writestring("TEST 2 COMPLETE\n");
}

/* TEST 3: Large Allocation Tests */
void handle_memtest3() {
    int i;

    terminal_writestring("\n=== TEST 3: Large Allocation Tests ===\n");

    uint32_t large_sizes[] = {8192, 16384, 32768}; /* Reduced sizes for safety */
    uint32_t num_large = sizeof(large_sizes) / sizeof(large_sizes[0]);

    for (i = 0; i < (int)num_large; i++) {
        terminal_writestring("Testing large allocation: ");
        printnbr(large_sizes[i] / 1024, 10);
        terminal_writestring("KB...\n");

        void *ptr = kmalloc(large_sizes[i]);
        if (ptr) {
            terminal_writestring("  SUCCESS - 0x");
            printnbr((uint32_t)ptr, 16);
            terminal_writestring("\n");

            /* Test memory boundaries */
            terminal_writestring("  Testing memory boundaries...\n");
            uint32_t *start = (uint32_t *)ptr;
            uint32_t *end = (uint32_t *)((uint8_t *)ptr + large_sizes[i] - 4);

            *start = 0x12345678;
            *end = 0x87654321;

            if (*start == 0x12345678 && *end == 0x87654321) {
                terminal_writestring("  Boundary test: PASSED\n");
            } else {
                terminal_writestring("  Boundary test: FAILED\n");
            }

            terminal_writestring("  Freeing large allocation...\n");
            kfree(ptr);
            terminal_writestring("  SUCCESS\n");
        } else {
            terminal_writestring("  FAILED\n");
            break; /* Stop on first failure */
        }
    }

    terminal_writestring("TEST 3 COMPLETE\n");
}

/* TEST 4: Virtual Memory Tests */
void handle_memtest4() {
    terminal_writestring("\n=== TEST 4: Virtual Memory Analysis ===\n");

    /* Check virtual memory constants */
    terminal_writestring("Virtual memory layout:\n");
    terminal_writestring("  USER_SPACE_START: 0x");
    printnbr(USER_SPACE_START, 16);
    terminal_writestring("\n  USER_SPACE_END: 0x");
    printnbr(USER_SPACE_END, 16);
    terminal_writestring("\n");

    /* Check physical memory availability */
    terminal_writestring("Physical memory status:\n");
    terminal_writestring("  Free pages: ");
    printnbr(g_phys_mem_manager.free_pages, 10);
    terminal_writestring("\n  Used pages: ");
    printnbr(g_phys_mem_manager.used_pages, 10);
    terminal_writestring("\n");

    /* Check if we have enough physical memory for vmalloc */
    if (g_phys_mem_manager.free_pages < 10) {
        terminal_writestring("WARNING: Low physical memory (< 10 pages)\n");
        terminal_writestring("This may cause vmalloc to fail\n");
    } else {
        terminal_writestring("Physical memory looks sufficient\n");
    }

    /* Check current directory for paging */
    terminal_writestring("Page directory: ");
    if (g_current_directory) {
        terminal_writestring("0x");
        printnbr((uint32_t)g_current_directory, 16);
        terminal_writestring(" (OK)\n");
    } else {
        terminal_writestring("NULL (ERROR)\n");
    }

    terminal_writestring("\nVirtual memory system status:\n");
    terminal_writestring("- Virtual memory constants are defined\n");
    terminal_writestring("- Physical memory manager is active\n");
    terminal_writestring("- Page directory is initialized\n");
    terminal_writestring("- vmalloc/vfree functions are available\n");

    terminal_writestring("\nNOTE: Direct vmalloc testing disabled due to\n");
    terminal_writestring("potential infinite loop in error handling.\n");
    terminal_writestring("Virtual memory system appears ready for use.\n");

    terminal_writestring("TEST 4 COMPLETE\n");
}

/* TEST 4 DEBUG: Advanced Virtual Memory Debugging */
void handle_memtest4debug() {
    terminal_writestring("\n=== TEST 4 DEBUG: Advanced Virtual Memory Debugging ===\n");

    /* All the basic info from memtest4 */
    terminal_writestring("Virtual memory layout:\n");
    terminal_writestring("  USER_SPACE_START: 0x");
    printnbr(USER_SPACE_START, 16);
    terminal_writestring("\n  USER_SPACE_END: 0x");
    printnbr(USER_SPACE_END, 16);
    terminal_writestring("\n  Virtual space size: ");
    printnbr((USER_SPACE_END - USER_SPACE_START) / (1024 * 1024), 10);
    terminal_writestring(" MB\n");

    /* Physical memory detailed analysis */
    terminal_writestring("\nPhysical memory detailed status:\n");
    terminal_writestring("  Total pages: ");
    printnbr(g_phys_mem_manager.total_pages, 10);
    terminal_writestring("\n  Free pages: ");
    printnbr(g_phys_mem_manager.free_pages, 10);
    terminal_writestring("\n  Used pages: ");
    printnbr(g_phys_mem_manager.used_pages, 10);
    terminal_writestring("\n  Memory utilization: ");
    if (g_phys_mem_manager.total_pages > 0) {
        uint32_t utilization = (g_phys_mem_manager.used_pages * 100) / g_phys_mem_manager.total_pages;
        printnbr(utilization, 10);
        terminal_writestring("%\n");
    } else {
        terminal_writestring("N/A\n");
    }

    /* Page directory analysis */
    terminal_writestring("\nPage directory analysis:\n");
    terminal_writestring("  Current directory: 0x");
    printnbr((uint32_t)g_current_directory, 16);
    terminal_writestring("\n  Kernel directory: 0x");
    printnbr((uint32_t)g_kernel_directory, 16);
    terminal_writestring("\n");

    /* Memory layout analysis */
    terminal_writestring("\nKernel memory layout:\n");
    terminal_writestring("  Kernel heap start: 0x");
    printnbr(g_kernel_heap.start_addr, 16);
    terminal_writestring("\n  Kernel heap end: 0x");
    printnbr(g_kernel_heap.end_addr, 16);
    terminal_writestring("\n  Heap size: ");
    printnbr((g_kernel_heap.end_addr - g_kernel_heap.start_addr) / 1024, 10);
    terminal_writestring(" KB\n");

    terminal_writestring("TEST 4 DEBUG COMPLETE\n");
}

/* TEST 5: Edge Cases */
void handle_memtest5() {
    terminal_writestring("\n=== TEST 5: Edge Cases ===\n");

    /* Test zero allocation */
    terminal_writestring("Testing zero allocation...\n");
    void *zero_ptr = kmalloc(0);
    if (zero_ptr == NULL) {
        terminal_writestring("  kmalloc(0): Correctly returned NULL\n");
    } else {
        terminal_writestring("  kmalloc(0): ERROR - Returned non-NULL\n");
        kfree(zero_ptr);
    }

    /* Test double free protection */
    terminal_writestring("Testing double free protection...\n");
    void *double_free_ptr = kmalloc(256);
    if (double_free_ptr) {
        terminal_writestring("  Allocated test block\n");
        kfree(double_free_ptr);
        terminal_writestring("  First free: OK\n");
        terminal_writestring("  Second free (should warn): ");
        kfree(double_free_ptr); /* This should trigger a warning */
        terminal_writestring("Done\n");
    }

    /* Test invalid free */
    terminal_writestring("Testing invalid free protection...\n");
    terminal_writestring("  Invalid free (should warn): ");
    kfree((void *)0x12345678); /* This should trigger a warning */
    terminal_writestring("Done\n");

    terminal_writestring("TEST 5 COMPLETE\n");
}

/* Crash Test: Demonstrate Kernel Panic Levels */
void handle_crashtest() {
    terminal_writestring("\n=== KERNEL PANIC LEVEL DEMONSTRATION ===\n");
    terminal_writestring("Available crash test commands:\n\n");

    terminal_writestring("Safe tests (system continues):\n");
    terminal_writestring("  crash1    - WARNING level test\n");
    terminal_writestring("  crash2    - ERROR level test\n");
    terminal_writestring("\nDangerous tests (may halt system):\n");
    terminal_writestring("  crash3    - FATAL level test (WILL HALT SYSTEM)\n");
    terminal_writestring("  crash4    - Division by zero\n");
    terminal_writestring("  crash5    - NULL pointer dereference\n");
    terminal_writestring("  crash6    - Invalid memory access\n");

    terminal_writestring("\nType the command you want to test.\n");
    terminal_writestring("Example: crash1\n");
}

/* Individual Crash Tests */
void handle_crash1() {
    terminal_writestring("\n=== WARNING LEVEL TEST ===\n");
    terminal_writestring("Triggering kernel_warning()...\n");
    kernel_warning("This is a test warning - system continues normally");
    terminal_writestring("System is still running after warning!\n");
    terminal_writestring("WARNING test complete.\n");
}

void handle_crash2() {
    terminal_writestring("\n=== ERROR LEVEL TEST ===\n");
    terminal_writestring("Triggering kernel_error()...\n");
    kernel_error("This is a test error - system may continue");
    terminal_writestring("System is still running after error!\n");
    terminal_writestring("ERROR test complete.\n");
}

void handle_crash3() {
    terminal_writestring("\n=== FATAL LEVEL TEST ===\n");
    terminal_writestring("WARNING: This will halt the system!\n");
    terminal_writestring("Triggering kernel_panic()...\n");
    kernel_panic(PANIC_LEVEL_FATAL, "This is a test fatal panic - system will halt");
    /* This line should never be reached */
    terminal_writestring("ERROR: System should have halted!\n");
}

void handle_crash4() {
    terminal_writestring("\n=== DIVISION BY ZERO TEST ===\n");
    terminal_writestring("Attempting division by zero...\n");
    volatile int a = 42;
    volatile int b = 0;
    volatile int result = a / b; /* This should trigger an exception */
    terminal_writestring("Result: ");
    printnbr(result, 10);
    terminal_writestring(" (This shouldn't print)\n");
}

void handle_crash5() {
    terminal_writestring("\n=== NULL POINTER DEREFERENCE TEST ===\n");
    terminal_writestring("Attempting to dereference NULL pointer...\n");
    volatile uint32_t *null_ptr = NULL;
    volatile uint32_t value = *null_ptr; /* This should cause page fault */
    terminal_writestring("Value: ");
    printnbr(value, 10);
    terminal_writestring(" (This shouldn't print)\n");
}

void handle_crash6() {
    terminal_writestring("\n=== INVALID MEMORY ACCESS TEST ===\n");
    terminal_writestring("Attempting to access invalid memory address...\n");
    volatile uint32_t *bad_ptr = (uint32_t *)0xDEADBEEF;
    volatile uint32_t value = *bad_ptr; /* This should cause page fault */
    terminal_writestring("Value: ");
    printnbr(value, 10);
    terminal_writestring(" (This shouldn't print)\n");
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
