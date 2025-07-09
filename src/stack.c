/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:17 by rperez-t          #+#    #+#             */
/*   Updated: 2025/06/17 22:27:55 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

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
