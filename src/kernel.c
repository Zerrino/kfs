/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:54:32 by alexafer          #+#    #+#             */
/*   Updated: 2025/06/05 19:05:23 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

size_t   terminal_row    = 0;
size_t   terminal_column = 0;
uint8_t  terminal_color  = 0;
uint16_t *const terminal_buffer = (uint16_t *)VGA_MEMORY;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ __volatile__("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	__asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline void vga_set_cursor(size_t row, size_t col)
{
    uint16_t pos = (uint16_t)(row * VGA_WIDTH + col);
    outb(VGA_PORT_INDEX, VGA_CRSR_LOW);
    outb(VGA_PORT_DATA,  pos & 0xFF);
    outb(VGA_PORT_INDEX, VGA_CRSR_HIGH);
    outb(VGA_PORT_DATA,  pos >> 8);
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	vga_set_cursor(0, 0);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
	if (c == '\0')
		return ;
	if (c == '\b')
	{
		if (terminal_column > 0)
			terminal_column--;
		else
		{
			if (terminal_row > 0)
			{
				terminal_row--;
				terminal_column = VGA_WIDTH;
			}
		}
		terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
	}
	else
	{
	if (c == '\n')
	{
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
		terminal_column = 0;
	}
	else
	{
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT)
				terminal_row = 0;
		}
	}
	}
	vga_set_cursor(terminal_row, terminal_column);
}

void	update_cursor(int scancode)
{
	if (scancode == 75)
	{
		if (terminal_column > 0)
			terminal_column--;
		else
		{
			if (terminal_row > 0)
			{
				terminal_row--;
				terminal_column = VGA_WIDTH;
			}
		}
	}
	else if (scancode == 77)
	{
		if (terminal_column < VGA_WIDTH)
			terminal_column++;
		else
		{
			if (terminal_row < VGA_HEIGHT)
			{
				terminal_row++;
				terminal_column = 0;
			}
		}
	}
	else if (scancode == 80)
	{
		if (terminal_row < VGA_HEIGHT)
			terminal_row++;
		else
			terminal_row = 0;
	}
	else if (scancode == 72)
	{
		if (terminal_row > 0)
			terminal_row--;
		else
			terminal_row = VGA_HEIGHT;
	}
	vga_set_cursor(terminal_row, terminal_column);
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

void	printnbr(int nbr, int base)
{
	terminal_putchar('-' * -(N(nbr)));
	if (nbr == INT_MIN || ABS(nbr) >= base)
		printnbr(ABS(nbr / base), base);
	terminal_putchar(ABS(nbr % base) + '0' + (7 * (ABS(nbr % base) > 9)));
}

#define IDT_ENTRIES 256

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern void load_idt(struct idt_ptr*);

struct idt_entry idt[IDT_ENTRIES];

void set_idt_gate(int n, uint32_t handler) {
    idt[n].offset_low  = handler & 0xFFFF;
    idt[n].selector    = 0x08;          // Code segment selector
    idt[n].zero        = 0;
    idt[n].type_attr   = 0x8E;          // Present, Ring 0, 32-bit interrupt gate
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

void init_idt(void) {
    struct idt_ptr idtp = {
        .limit = sizeof(idt) - 1,
        .base  = (uint32_t)&idt
    };

    // Exemple : handler clavier à l'entrée 0x21 (IRQ1)
    extern void irq1_handler();  // défini en ASM plus bas
    set_idt_gate(0x21, (uint32_t)irq1_handler);
	extern void irq0_handler();
	set_idt_gate(0x20, (uint32_t)irq0_handler); // IRQ0 remappée à 0x20
    load_idt(&idtp);
}

void pic_remap() {
    outb(0x20, 0x11); // init
    outb(0xA0, 0x11);
    outb(0x21, 0x20); // IRQ0–7 -> 0x20–0x27
    outb(0xA1, 0x28); // IRQ8–15 -> 0x28–0x2F
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void keyboard_handler(void)
{
    static const char scancode_to_ascii[] = {
        0, 27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
        '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
        0,'a','s','d','f','g','h','j','k','l',';','\'','`',
        0,'\\','z','x','c','v','b','n','m',',','.','/', 0,'*',
        0,' ' /* etc. */
    };

    uint8_t scancode = inb(0x60);
    if (!(scancode & 0x80))
	{               /* touche pressée */
        char c = scancode_to_ascii[scancode];
        if (c)
		{
			terminal_putchar(c);
		}
		else
		{
			if (scancode == 75 || scancode == 77 || scancode == 80 || scancode == 72)
				update_cursor(scancode);
			else
			{
				printnbr(scancode, 10);
			}
		}
    }
    outb(0x20, 0x20);                       /* EOI au PIC */
}

void kernel_main(void)
{
	/* Initialize terminal interface */
	terminal_initialize();

	/* Newline support is left as an exercise. */
	pic_remap();     // Remapper le PIC
    init_idt();      // Initialiser l’IDT
    __asm__ volatile ("sti"); // Activer les interruptions
	while(1)
	{
	}
}
