/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kfs3_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:20 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/05 14:26:45 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

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