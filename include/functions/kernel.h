/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/09 16:19:26 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KERNEL_FUNCTIONS_H
# define KERNEL_FUNCTIONS_H

#include "../core/libs.h"
#include "../core/structs.h"

/* ──────────── Terminal/VGA Functions ──────────── */

/**
 * @brief Initialize the terminal system
 */
void terminal_initialize(void);

/**
 * @brief Set terminal color
 * @param color VGA color value
 */
void terminal_setcolor(uint8_t color);

/**
 * @brief Put a character at specific position
 * @param c Character to display
 * @param color Color of the character
 * @param x X coordinate
 * @param y Y coordinate
 */
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);

/**
 * @brief Put a character at current cursor position
 * @param c Character to display
 */
void terminal_putchar(char c);

/**
 * @brief Write a string to terminal
 * @param data String to write
 */
void terminal_writestring(const char *data);

/**
 * @brief Set terminal offset for scrolling
 * @param offset Offset value
 */
void terminal_offset(uint16_t offset);

/**
 * @brief Restore terminal to default state
 */
void terminal_restore(void);

/* ──────────── Utility Functions ──────────── */

/**
 * @brief Print a number in specified base
 * @param nbr Number to print
 * @param base Base for number representation (2-16)
 */
void printnbr(int nbr, int base);

/**
 * @brief Calculate string length
 * @param str String to measure
 * @return Length of the string
 */
size_t ft_strlen(const char *str);

/**
 * @brief Compare two strings
 * @param s1 First string
 * @param s2 Second string
 * @return 0 if equal, non-zero otherwise
 */
int ft_strcmp(const char *s1, const char *s2);

/**
 * @brief Copy memory from source to destination
 * @param dest Destination buffer
 * @param src Source buffer
 * @param n Number of bytes to copy
 * @return Pointer to destination
 */
void *ft_memcpy(void *dest, const void *src, size_t n);

/**
 * @brief Set memory to a specific value
 * @param s Memory area to set
 * @param c Value to set
 * @param n Number of bytes to set
 * @return Pointer to memory area
 */
void *ft_memset(void *s, int c, size_t n);

/* ──────────── Keyboard Functions ──────────── */

/**
 * @brief Initialize keyboard system
 */
void keyboard_init(void);

/**
 * @brief Handle keyboard interrupt
 * @param regs Register state at interrupt
 */
void keyboard_handler(t_registers *regs);

/**
 * @brief Update cursor based on scancode
 * @param scancode Keyboard scancode
 */
void update_cursor(int scancode);

/* ──────────── Stack Functions ──────────── */

/**
 * @brief Push a value onto the kernel stack
 * @param value Value to push
 */
void stack_push(uint32_t value);

/**
 * @brief Pop a value from the kernel stack
 * @return Popped value
 */
uint32_t stack_pop(void);

/**
 * @brief Peek at top of stack without removing
 * @return Top stack value
 */
uint32_t stack_peek(void);

/**
 * @brief Check if stack is empty
 * @return 1 if empty, 0 otherwise
 */
int stack_is_empty(void);

/**
 * @brief Get current stack size
 * @return Number of elements on stack
 */
int stack_size(void);

/**
 * @brief Print detailed stack contents
 */
void print_kernel_stack(void);

/* ──────────── VGA/Hardware Functions ──────────── */

/**
 * @brief Create VGA color entry
 * @param fg Foreground color
 * @param bg Background color
 * @return Combined color value
 */
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);

/**
 * @brief Create VGA character entry
 * @param uc Character
 * @param color Color value
 * @return VGA entry
 */
uint16_t vga_entry(unsigned char uc, uint8_t color);

/**
 * @brief Set VGA cursor position
 * @param row Row position
 * @param col Column position
 */
void vga_set_cursor(size_t row, size_t col);

/**
 * @brief Restore VGA cursor to saved position
 */
void vga_cursor_restore(void);

/* ──────────── Additional Utility Functions ──────────── */

/**
 * @brief Move memory from source to destination
 * @param dest Destination buffer
 * @param src Source buffer
 * @param n Number of bytes to move
 * @return Pointer to destination
 */
void *ft_memmove(void *dest, const void *src, size_t n);

/**
 * @brief Find character in memory
 * @param s Memory area to search
 * @param c Character to find
 * @param n Number of bytes to search
 * @return Pointer to found character or NULL
 */
void *ft_memchr(const void *s, int c, size_t n);

/**
 * @brief Zero out memory area
 * @param s Memory area to zero
 * @param n Number of bytes to zero
 */
void ft_bzero(void *s, size_t n);

/**
 * @brief Compare memory areas
 * @param s1 First memory area
 * @param s2 Second memory area
 * @param n Number of bytes to compare
 * @return Comparison result
 */
int ft_memcmp(const void *s1, const void *s2, size_t n);

/**
 * @brief String comparison (implemented in shell.c)
 * @param s1 First string
 * @param s2 Second string
 * @return Comparison result
 */
int strcmp(const char *s1, const char *s2);

/**
 * @brief Initialize shell system
 */
void shell_initialize(void);

/* ──────────── I/O Functions ──────────── */

/**
 * @brief Read byte from I/O port
 * @param port Port to read from
 * @return Byte value
 */
uint8_t inb(uint16_t port);

/**
 * @brief Write byte to I/O port
 * @param port Port to write to
 * @param value Byte value to write
 */
void outb(uint16_t port, uint8_t value);

/**
 * @brief Write word to I/O port
 * @param port Port to write to
 * @param value Word value to write
 */
void outw(uint16_t port, uint16_t value);

/* ──────────── Exception Handling Functions ──────────── */

/**
 * @brief Get exception message string by exception number
 * @param exception_num Exception number (0-31)
 * @return Pointer to exception message string
 */
const char *get_exception_message(uint32_t exception_num);

#endif