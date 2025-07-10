/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_stack.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:14:06 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 16:01:54 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kfs.h"

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

        if ('0' <= arg[i] && arg[i] <= '9')
            value += arg[i] - '0';
        else if ('a' <= arg[i] && arg[i] <= 'f')
            value += arg[i] - 'a' + 10;
        else if ('A' <= arg[i] && arg[i] <= 'F')
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

        for (int j = 7; 0 <= j; j--) {
            uint8_t nibble = (value >> (j * BITS_PER_NIBBLE)) & NIBBLE_MASK;
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

    for (int j = 7; 0 <= j; j--) {
        uint8_t nibble = (value >> (j * BITS_PER_NIBBLE)) & NIBBLE_MASK;
        char hex_char;
        if (nibble < 10)
            hex_char = '0' + nibble;
        else
            hex_char = 'A' + (nibble - 10);
        terminal_putchar(hex_char);
    }

    terminal_writestring(" from the stack\n");
}

void handle_stacktest() {
    terminal_writestring("=== Stack Operations Demo ===\n");

    // Show initial stack state
    terminal_writestring("Initial stack size: ");
    printnbr(stack_size(), 10);
    terminal_writestring(" entries\n");

    // Push some test values
    terminal_writestring("Pushing test values: 0xDEAD, 0xBEEF, 0xCAFE\n");
    stack_push(TEST_VALUE_DEAD);
    stack_push(TEST_VALUE_BEEF);
    stack_push(TEST_VALUE_CAFE);

    terminal_writestring("Stack size after push: ");
    printnbr(stack_size(), 10);
    terminal_writestring(" entries\n");

    // Peek at top value
    if (!stack_is_empty()) {
        terminal_writestring("Top value (peek): 0x");
        printnbr(stack_peek(), 16);
        terminal_writestring("\n");
    }

    // Pop values back
    terminal_writestring("Popping values: ");
    while (!stack_is_empty()) {
        terminal_writestring("0x");
        printnbr(stack_pop(), 16);
        terminal_writestring(" ");
    }
    terminal_writestring("\n");

    terminal_writestring("Final stack size: ");
    printnbr(stack_size(), 10);
    terminal_writestring(" entries\n");
    terminal_writestring("Stack test completed!\n");
}
