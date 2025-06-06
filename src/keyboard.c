/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:33:23 by alexafer          #+#    #+#             */
/*   Updated: 2025/06/06 14:34:58 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

void	update_cursor(int scancode)
{
	if (scancode == 75)
	{
		if (kernel.terminal_column > 0)
			kernel.terminal_column--;
		else
		{
			if (kernel.terminal_row > 0)
			{
				kernel.terminal_row--;
				kernel.terminal_column = VGA_WIDTH;
			}
		}
	}
	else if (scancode == 77)
	{
		if (kernel.terminal_column < VGA_WIDTH)
			kernel.terminal_column++;
		else
		{
			if (kernel.terminal_row < VGA_HEIGHT)
			{
				kernel.terminal_row++;
				kernel.terminal_column = 0;
			}
		}
	}
	else if (scancode == 80)
	{
		if (kernel.terminal_row < VGA_HEIGHT)
			kernel.terminal_row++;
		else
			kernel.terminal_row = 0;
	}
	else if (scancode == 72)
	{
		if (kernel.terminal_row > 0)
			kernel.terminal_row--;
		else
			kernel.terminal_row = VGA_HEIGHT;
	}
	vga_set_cursor(kernel.terminal_row, kernel.terminal_column);
}

void set_idt_gate(int n, uint32_t handler)
{
    kernel.idt[n].offset_1  = handler & 0xFFFF;
    kernel.idt[n].selector    = 0x08;          // Code segment selector
    kernel.idt[n].zero        = 0;
    kernel.idt[n].type_attributes   = 0x8E;          // Present, Ring 0, 32-bit interrupt gate
    kernel.idt[n].offset_2 = (handler >> 16) & 0xFFFF;
}

void init_idt()
{
    t_idt_descryptor idtp = {
        .size = sizeof(kernel.idt) - 1,
        .offset  = (uint32_t)&(*kernel.idt)
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

void keyboard_handler()
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
	{
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
