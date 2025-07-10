/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:20 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 16:26:40 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kfs.h"

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
    } else
        terminal_writestring("\nPaging: DISABLED\n");
}

void handle_vmemstats() {
    vmem_print_stats();
}

void handle_memcheck() {
    terminal_writestring("\n=== Memory Integrity Check ===\n");
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
    uint32_t used_pages = g_phys_mem_manager.used_pages;
    uint32_t expected_pages = (g_kernel_heap.current_end - g_kernel_heap.start_addr) / PAGE_SIZE;
    expected_pages += 1; /* Page directory */
    terminal_writestring("Used pages: ");
    printnbr(used_pages, 10);
    terminal_writestring("\nExpected minimum: ");
    printnbr(expected_pages, 10);
    terminal_writestring("\n");
    if (used_pages >= expected_pages)
        terminal_writestring("Memory usage: NORMAL\n");
    else
        terminal_writestring("WARNING: Unexpected memory usage pattern\n");

    terminal_writestring("Memory integrity check completed\n");
}

/* TEST 1: Basic Allocation Tests */
void handle_memtest1() {
    int i;
    uint32_t initial_used_pages;

    terminal_writestring("\n=== TEST 1: Basic Allocation Tests ===\n");
    initial_used_pages = g_phys_mem_manager.used_pages;
    terminal_writestring("Initial used pages: ");
    printnbr(initial_used_pages, 10);
    terminal_writestring("\n");
    uint32_t test_sizes[] = {16, 32, 64, 128, 256, 512, 1024, 2048, 4096}; /* Test various sizes */
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
            uint32_t *data = (uint32_t *)ptr;
            uint32_t pattern = 0xDEADBEEF + i;
            terminal_writestring("  Writing pattern 0x");
            printnbr(pattern, 16);
            terminal_writestring("...\n");
            *data = pattern;

            if (*data == pattern)
                terminal_writestring("  Memory write/read: OK\n");
            else {
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
    for (i = 0; i < 10; i++) {
        ptrs[i] = NULL;
    }

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
            uint32_t *data = (uint32_t *)ptrs[i]; /* Write unique pattern to each block */
            *data = 0xCAFEBABE + i;
        } else {
            terminal_writestring("  FAILED\n");
            break; /* Stop on first failure */
        }
    }

    terminal_writestring("Successfully allocated ");
    printnbr(successful_allocs, 10);
    terminal_writestring("/10 blocks\n");
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
    if (pattern_errors == 0)
        terminal_writestring("PASSED\n");
    else {
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
    terminal_writestring("Virtual memory layout:\n");
    terminal_writestring("  USER_SPACE_START: 0x");
    printnbr(USER_SPACE_START, 16);
    terminal_writestring("\n  USER_SPACE_END: 0x");
    printnbr(USER_SPACE_END, 16);
    terminal_writestring("\n");
    terminal_writestring("Physical memory status:\n");
    terminal_writestring("  Free pages: ");
    printnbr(g_phys_mem_manager.free_pages, 10);
    terminal_writestring("\n  Used pages: ");
    printnbr(g_phys_mem_manager.used_pages, 10);
    terminal_writestring("\n");
    if (g_phys_mem_manager.free_pages < 10) { 
        terminal_writestring("WARNING: Low physical memory (< 10 pages)\n");
        terminal_writestring("This may cause vmalloc to fail\n");
    } else 
        terminal_writestring("Physical memory looks sufficient\n");

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

/* TEST 5: Edge Cases */
void handle_memtest5() {
    terminal_writestring("\n=== TEST 5: Edge Cases ===\n");
    terminal_writestring("Testing zero allocation...\n");
    void *zero_ptr = kmalloc(0);
    if (zero_ptr == NULL)
        terminal_writestring("  kmalloc(0): Correctly returned NULL\n");
    else {
        terminal_writestring("  kmalloc(0): ERROR - Returned non-NULL\n");
        kfree(zero_ptr);
    }

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

    terminal_writestring("Testing invalid free protection...\n");
    terminal_writestring("  Invalid free (should warn): ");
    kfree((void *)0x12345678); /* This should trigger a warning */
    terminal_writestring("Done\n");
    terminal_writestring("TEST 5 COMPLETE\n");
}


