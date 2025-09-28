/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:17 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 16:01:54 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kfs.h"

void stack_push(uint32_t value) {
    if (kernel.stack_pointer < KERNEL_STACK_SIZE - 1)
        kernel.kernel_stack[++kernel.stack_pointer] = value;
    else
        terminal_writestring("Error: Stack overflow\n");
}

uint32_t stack_pop() {
    if (0 <= kernel.stack_pointer)
        return kernel.kernel_stack[kernel.stack_pointer--];

    terminal_writestring("Error: Stack underflow\n");
    return 0;
}

uint32_t stack_peek() {
    if (0 <= kernel.stack_pointer)
        return kernel.kernel_stack[kernel.stack_pointer];

    terminal_writestring("Error: Stack is empty\n");
    return 0;
}

int stack_is_empty() {
    return kernel.stack_pointer == -1;
}

int stack_size() {
    return kernel.stack_pointer + 1;
}

static void print_stack_header() {
    terminal_writestring("=== Kernel Stack Contents ===\n");
    terminal_writestring("Stack Size: ");
    printnbr(KERNEL_STACK_SIZE, 10);
    terminal_writestring(" entries\n");
    terminal_writestring("Current Stack Pointer: ");
    printnbr(kernel.stack_pointer, 10);
    terminal_writestring("\n");
    terminal_writestring("Used Stack Space: ");
    printnbr(kernel.stack_pointer + 1, 10);
    terminal_writestring(" / ");
    printnbr(KERNEL_STACK_SIZE, 10);
    terminal_writestring(" entries\n");
    terminal_writestring("Stack Base Address: 0x");
    printnbr((uint32_t)kernel.kernel_stack, 16);
    terminal_writestring("\n");
    terminal_writestring("Stack Top Address: 0x");
    printnbr((uint32_t)&kernel.kernel_stack[kernel.stack_pointer], 16);
    terminal_writestring("\n\n");
}

static void print_stack_table_header() {
    terminal_writestring("Stack Contents (Top to Bottom):\n");
    terminal_writestring("Index | Address    | Value      | ASCII\n");
    terminal_writestring("------|------------|------------|--------\n");
}

static void print_hex32(uint32_t value) {
    terminal_writestring("0x");
    for (int j = 7; 0 <= j; j--) {
        uint8_t nibble = (value >> (j * 4)) & 0xF;
        char hex_char = nibble < 10 ? '0' + nibble : 'A' + (nibble - 10);
        terminal_putchar(hex_char);
    }
}

static void print_ascii_representation(uint32_t value) {
    for (int byte_idx = 0; byte_idx < 4; byte_idx++) {
        uint8_t byte_val = (value >> (byte_idx * 8)) & 0xFF;
        if (' ' <= byte_val && byte_val <= '~')
            terminal_putchar(byte_val);
        else
            terminal_putchar('.');
    }
}

static void print_padded_index(int index) {
    if (index < 10)
        terminal_writestring("  ");
    else if (index < 100)
        terminal_writestring(" ");
        
    printnbr(index, 10);
}

static void print_stack_entry(int index) {
    uint32_t addr = (uint32_t)&kernel.kernel_stack[index];
    uint32_t value = kernel.kernel_stack[index];
    print_padded_index(index);
    terminal_writestring(" | ");
    print_hex32(addr);
    terminal_writestring(" | ");
    print_hex32(value);
    terminal_writestring(" | ");
    print_ascii_representation(value);
    if (index == kernel.stack_pointer)
        terminal_writestring(" <- TOP");

    terminal_writestring("\n");
}

void print_kernel_stack() {
    print_stack_header();
    if (stack_is_empty()) {
        terminal_writestring("  >>> Stack is empty <<<\n");
        return;
    }

    print_stack_table_header();
    for (int i = kernel.stack_pointer; 0 <= i; i--)
        print_stack_entry(i);

    terminal_writestring("=============================\n");
}

/* ──────────── Real Kernel Execution Stack Functions ──────────── */

void print_kernel_execution_stack() {
    uint32_t current_esp;
    uint32_t stack_base = (uint32_t)kernel_execution_stack;
    uint32_t stack_top = (uint32_t)kernel_stack_top;

    /* Get current ESP */
    __asm__ volatile ("mov %%esp, %0" : "=r" (current_esp));

    terminal_writestring("=== Kernel Execution Stack (GDT-Integrated) ===\n");
    terminal_writestring("GDT Kernel Stack Segment: 0x");
    printnbr(GDT_KERNEL_STACK, 16);
    terminal_writestring(" (Selector)\n");
    terminal_writestring("Dedicated Stack Base:     0x");
    printnbr(stack_base, 16);
    terminal_writestring("\n");
    terminal_writestring("Dedicated Stack Top:      0x");
    printnbr(stack_top, 16);
    terminal_writestring("\n");
    terminal_writestring("Current ESP:              0x");
    printnbr(current_esp, 16);
    terminal_writestring("\n");
    terminal_writestring("Stack Size:               ");
    printnbr(KERNEL_EXECUTION_STACK_SIZE, 10);
    terminal_writestring(" bytes (");
    printnbr(KERNEL_EXECUTION_STACK_SIZE / 1024, 10);
    terminal_writestring(" KB)\n");

    /* Check if we're using the dedicated kernel stack */
    if (current_esp <= stack_top && current_esp >= stack_base) {
        terminal_writestring("Status:                   USING DEDICATED KERNEL STACK\n");
        terminal_writestring("Integration:              LINKED TO GDT KERNEL STACK SEGMENT\n");
        terminal_writestring("Implementation:           REPLACED BOOT STACK\n");
        uint32_t used = stack_top - current_esp;
        terminal_writestring("Stack Usage:              ");
        printnbr(used, 10);
        terminal_writestring(" bytes\n");
        terminal_writestring("Stack Free:               ");
        printnbr(KERNEL_EXECUTION_STACK_SIZE - used, 10);
        terminal_writestring(" bytes\n");

        /* Show some stack contents */
        terminal_writestring("\n--- Kernel Call Stack Contents ---\n");
        terminal_writestring("Address    | Value      | ASCII\n");
        terminal_writestring("-----------|------------|--------\n");

        for (int i = 0; i < 8 && (current_esp + (i * 4)) < stack_top; i++) {
            uint32_t addr = current_esp + (i * 4);
            uint32_t value = *(uint32_t*)addr;

            terminal_writestring("0x");
            printnbr(addr, 16);
            terminal_writestring(" | 0x");
            printnbr(value, 16);
            terminal_writestring(" | ");
            print_ascii_representation(value);
            if (i == 0)
                terminal_writestring(" <- ESP");
            terminal_writestring("\n");
        }
    } else {
        terminal_writestring("Status:                   USING BOOT STACK\n");
        terminal_writestring("Integration:              BOOT STACK NOT REPLACED\n");
        terminal_writestring("Note: The kernel should be using the dedicated execution stack.\n");
        terminal_writestring("Check boot.s configuration.\n");
    }
    terminal_writestring("===============================================\n");
}

void switch_to_kernel_stack() {
    /* Complete stack replacement: Copy boot stack content to kernel stack and switch */
    uint32_t old_esp, old_ebp;
    uint32_t boot_stack_top;

    /* Get current stack pointers */
    __asm__ volatile (
        "mov %%esp, %0\n\t"
        "mov %%ebp, %1"
        : "=r" (old_esp), "=r" (old_ebp)
    );

    /* Define boot stack boundaries (from boot.s) */
    extern uint8_t stack_top[];
    boot_stack_top = (uint32_t)stack_top;

    /* Calculate how much of the boot stack is actually used */
    uint32_t used_stack_size = boot_stack_top - old_esp;

    /* Make sure we don't exceed our kernel stack size */
    if (used_stack_size > KERNEL_EXECUTION_STACK_SIZE - 512) {
        used_stack_size = KERNEL_EXECUTION_STACK_SIZE - 512; /* Leave safety margin */
    }

    /* Calculate new stack positions - ENSURE we use our kernel_execution_stack */
    uint32_t kernel_stack_base = (uint32_t)kernel_execution_stack;
    uint32_t kernel_stack_top_addr = (uint32_t)kernel_stack_top;
    uint32_t new_esp = kernel_stack_top_addr - used_stack_size;

    /* Debug output */
    terminal_writestring("  Old ESP: 0x");
    printnbr(old_esp, 16);
    terminal_writestring(", New ESP: 0x");
    printnbr(new_esp, 16);
    terminal_writestring("\n");
    terminal_writestring("  Kernel stack range: 0x");
    printnbr(kernel_stack_base, 16);
    terminal_writestring(" - 0x");
    printnbr(kernel_stack_top_addr, 16);
    terminal_writestring("\n");
    terminal_writestring("  Copying ");
    printnbr(used_stack_size, 10);
    terminal_writestring(" bytes of stack data\n");

    /* Ensure new_esp is within our kernel stack bounds */
    if (new_esp < kernel_stack_base) {
        new_esp = kernel_stack_base + 256; /* Minimum safe position */
        used_stack_size = kernel_stack_top_addr - new_esp;
    }

    /* Calculate new EBP relative to the new stack */
    uint32_t ebp_offset = old_ebp - old_esp;
    uint32_t new_ebp = new_esp + ebp_offset;

    /* Copy the used portion of the boot stack to our kernel execution stack */
    uint8_t *old_stack_data = (uint8_t*)old_esp;
    uint8_t *new_stack_data = (uint8_t*)new_esp;

    for (uint32_t i = 0; i < used_stack_size; i++) {
        new_stack_data[i] = old_stack_data[i];
    }

    /* Atomic switch to our dedicated kernel execution stack */
    __asm__ volatile (
        "mov %0, %%esp\n\t"
        "mov %1, %%ebp"
        :
        : "r" (new_esp), "r" (new_ebp)
        : "memory"
    );
}