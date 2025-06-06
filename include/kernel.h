/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:48:23 by alexafer          #+#    #+#             */
/*   Updated: 2025/06/06 15:31:48 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# if defined(__linux__)
	#error "You are not using a cross-compiler, you will most certainly run into trouble"
# endif

/* This tutorial will only work for the 32-bit ix86 targets. */
# if !defined(__i386__)
	#error "This tutorial needs to be compiled with a ix86-elf compiler"
# endif

#ifndef KERNEL_H
# define KERNEL_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <limits.h>

# define VGA_WIDTH   80
# define VGA_HEIGHT  25
# define VGA_MEMORY  0xB8000
# define VGA_CRSR_LOW   0x0F
# define VGA_CRSR_HIGH  0x0E
# define VGA_PORT_INDEX 0x3D4
# define VGA_PORT_DATA  0x3D5
# define IDT_ENTRIES 256

# define PIC1_CMD  0x20
# define PIC1_DATA 0x21
# define PIC2_CMD  0xA0
# define PIC2_DATA 0xA1

# define ICW1_INIT 0x10
# define ICW1_ICW4 0x01
# define ICW4_8086 0x01

# define N(x) (x >> ((sizeof(x) * 8) - 1))
# define ABS(x) ((x + (1 * N(x))) ^ N(x))

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

 // https://wiki.osdev.org/Interrupt_Descriptor_Table
 typedef struct s_idt_descriptor
 {
	uint16_t	size;	// Size: One less than the size of the IDT in bytes.
	uint32_t	offset;	// Offset: The linear address of the Interrupt Descriptor Table (not the physical address, paging applies).
 }	__attribute__((packed)) t_idt_descryptor;

typedef struct s_idt_entry
{
	uint16_t offset_1;        // offset bits 0..15
	uint16_t selector;        // a code segment selector in GDT or LDT
	uint8_t  zero;            // unused, set to 0
	uint8_t  type_attributes; // gate type, dpl, and p fields
	uint16_t offset_2;        // offset bits 16..31
}	__attribute__((packed)) t_idt_entry;

typedef struct s_kernel
{
	t_idt_entry idt[IDT_ENTRIES];
	size_t		terminal_row;
	size_t		terminal_column;
	uint8_t		terminal_color;
	uint8_t		terminal_ctrl;
	uint8_t		terminal_shift;
	uint16_t	*terminal_buffer;
}	t_kernel;

extern t_kernel	kernel;

/* src/keyboard.c */
void		update_cursor(int scancode);
void		set_idt_gate(int n, uint32_t handler);
void		init_idt();
void		pic_remap(void);
void		keyboard_handler();

/* src/utils.s */
void		load_idt(t_idt_descryptor *);


/* src/inlinie_utils.c */
uint8_t		vga_entry_color(enum vga_color fg, enum vga_color bg);
uint16_t	vga_entry(unsigned char uc, uint8_t color);
void		vga_set_cursor(size_t row, size_t col);
void		outb(uint16_t port, uint8_t val);
uint8_t		inb(uint16_t port);

/* src/string_utilitary.c */
size_t		strlen(const char* str);
void		terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void		terminal_putchar(char c);
void		terminal_write(const char* data, size_t size);
void		terminal_writestring(const char* data);
void		printnbr(int nbr, int base);


#endif
