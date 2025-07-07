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

#define STACK_SIZE 1024
static uint32_t kernel_stack[STACK_SIZE];
static int stack_pointer = -1;

void stack_push(uint32_t value) {
    if (stack_pointer < STACK_SIZE - 1) 
        kernel_stack[++stack_pointer] = value;
    else 
        terminal_writestring("Error: Stack overflow\n");
}

uint32_t stack_pop() {
    if (stack_pointer >= 0) 
        return kernel_stack[stack_pointer--];
    else {
        terminal_writestring("Error: Stack underflow\n");
        return 0;
    }
}

uint32_t stack_peek() {
    if (stack_pointer >= 0) 
        return kernel_stack[stack_pointer];
    else{
		
		terminal_writestring("Error: Stack is empty\n");
        return 0;
	}
}

int stack_is_empty() {
    return stack_pointer == -1;
}

int stack_size() {
    return stack_pointer + 1;
}


void print_kernel_stack() {
    terminal_writestring("=== Kernel Stack Contents ===\n");
    terminal_writestring("Stack Size: ");
    printnbr(STACK_SIZE, 10);
    terminal_writestring(" entries\n");
    terminal_writestring("Current Stack Pointer: ");
    printnbr(stack_pointer, 10);
    terminal_writestring("\n");
    terminal_writestring("Used Stack Space: ");
    printnbr(stack_pointer + 1, 10);
    terminal_writestring(" / ");
    printnbr(STACK_SIZE, 10);
    terminal_writestring(" entries\n");
    terminal_writestring("Stack Base Address: 0x");
    printnbr((uint32_t)kernel_stack, 16);
    terminal_writestring("\n");
    terminal_writestring("Stack Top Address: 0x");
    printnbr((uint32_t)&kernel_stack[stack_pointer], 16);
    terminal_writestring("\n\n");

    if (stack_is_empty()) {
        terminal_writestring("  >>> Stack is empty <<<\n");
        return;
    }

    terminal_writestring("Stack Contents (Top to Bottom):\n");
    terminal_writestring("Index | Address    | Value      | ASCII\n");
    terminal_writestring("------|------------|------------|--------\n");

    for (int i = stack_pointer; i >= 0; i--) {
        // Print index with padding
        if (i < 10) terminal_writestring("  ");
        else if (i < 100) terminal_writestring(" ");
        printnbr(i, 10);
        terminal_writestring(" | ");

        // Print address
        terminal_writestring("0x");
        uint32_t addr = (uint32_t)&kernel_stack[i];
        for (int j = 7; j >= 0; j--) {
            uint8_t nibble = (addr >> (j * 4)) & 0xF;
            char hex_char = nibble < 10 ? '0' + nibble : 'A' + (nibble - 10);
            terminal_putchar(hex_char);
        }
        terminal_writestring(" | ");

        // Print value
        terminal_writestring("0x");
        uint32_t value = kernel_stack[i];
        for (int j = 7; j >= 0; j--) {
            uint8_t nibble = (value >> (j * 4)) & 0xF;
            char hex_char = nibble < 10 ? '0' + nibble : 'A' + (nibble - 10);
            terminal_putchar(hex_char);
        }
        terminal_writestring(" | ");

        // Print ASCII representation
        for (int byte_idx = 0; byte_idx < 4; byte_idx++) {
            uint8_t byte_val = (value >> (byte_idx * 8)) & 0xFF;
            if (byte_val >= 32 && byte_val <= 126) {
                terminal_putchar(byte_val);
            } else {
                terminal_putchar('.');
            }
        }

        // Mark top of stack
        if (i == stack_pointer) {
            terminal_writestring(" <- TOP");
        }

        terminal_writestring("\n");
    }
    terminal_writestring("=============================\n");
}
