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

        uint32_t value = kernel_stack[i];
        for (int j = 7; j >= 0; j--) {
            uint8_t nibble = (value >> (j * 4)) & 0xF;
            char hex_char = nibble < 10 ? '0' + nibble : 'A' + (nibble - 10);
            terminal_putchar(hex_char);
        }

        terminal_writestring("\n");
    }
}
