/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 12:40:20 by rperez-t          #+#    #+#             */
/*   Updated: 2025/06/15 12:40:21 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

// Define the kernel stack structure
#define STACK_SIZE 1024
static uint32_t kernel_stack[STACK_SIZE];
static int stack_pointer = -1;

// Push a value onto the stack
void stack_push(uint32_t value) {
    if (stack_pointer < STACK_SIZE - 1) {
        kernel_stack[++stack_pointer] = value;
    } else {
        terminal_writestring("Error: Stack overflow\n");
    }
}

// Pop a value from the stack
uint32_t stack_pop() {
    if (stack_pointer >= 0) {
        return kernel_stack[stack_pointer--];
    } else {
        terminal_writestring("Error: Stack underflow\n");
        return 0;
    }
}

// Peek at the top value without popping
uint32_t stack_peek() {
    if (stack_pointer >= 0) {
        return kernel_stack[stack_pointer];
    } else {
        terminal_writestring("Error: Stack is empty\n");
        return 0;
    }
}

// Check if stack is empty
int stack_is_empty() {
    return stack_pointer == -1;
}

// Get current stack size
int stack_size() {
    return stack_pointer + 1;
}

// Print the kernel stack (as required in the PDF)
void print_kernel_stack() {
    terminal_writestring("Kernel Stack Contents:\n");
    
    if (stack_is_empty()) {
        terminal_writestring("  Stack is empty\n");
        return;
    }
    
    for (int i = stack_pointer; i >= 0; i--) {
        terminal_writestring("  [");
        printnbr(i, 10);
        terminal_writestring("] 0x");
        
        // Convert value to hex string
        uint32_t value = kernel_stack[i];
        for (int j = 7; j >= 0; j--) {
            uint8_t nibble = (value >> (j * 4)) & 0xF;
            char hex_char = nibble < 10 ? '0' + nibble : 'A' + (nibble - 10);
            terminal_putchar(hex_char);
        }
        
        terminal_writestring("\n");
    }
}
