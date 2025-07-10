/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_crash.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:20 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 16:27:11 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kfs.h"

/* Crash Test: Demonstrate Kernel Panic Levels */
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
}
