/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:33:23 by alexafer          #+#    #+#             */
/*   Updated: 2025/06/06 21:57:18 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

void	update_cursor(int scancode)
{
	if (scancode == 29)
	{
		kernel.terminal_ctrl = 1;
	}
	if (scancode == 42 || scancode == 54)
	{
		kernel.terminal_shift = 1;
	}
	else if (scancode == 75)
	{

		if (kernel.terminal_ctrl && kernel.terminal_shift)
		{
			if (kernel.screen_index > 0)
			{
				kernel.screen_index--;
				terminal_restore();
				vga_cursor_restore();
				return ;
			}
		}
		else if (kernel.screens[kernel.screen_index].column > 0)
			kernel.screens[kernel.screen_index].column--;
		else
		{
			if (kernel.screens[kernel.screen_index].row > 0)
			{
				kernel.screens[kernel.screen_index].row--;
				kernel.screens[kernel.screen_index].column = VGA_WIDTH;
			}
		}
	}
	else if (scancode == 77)
	{
		if (kernel.terminal_ctrl && kernel.terminal_shift)
		{
			if (kernel.screen_index < NB_SCREEN - 1)
			{
				kernel.screen_index++;
				terminal_restore();
				vga_cursor_restore();
				return;
			}
		}
		else if (kernel.screens[kernel.screen_index].column < VGA_WIDTH)
		kernel.screens[kernel.screen_index].column++;
		else
		{
			if (kernel.screens[kernel.screen_index].row < VGA_HEIGHT)
			{
				kernel.screens[kernel.screen_index].row++;
				kernel.screens[kernel.screen_index].column = 0;
			}
		}
	}
	else if (scancode == 80)
	{
		if (kernel.screens[kernel.screen_index].row < VGA_HEIGHT)
			kernel.screens[kernel.screen_index].row++;
		else
			kernel.screens[kernel.screen_index].row = 0;
	}
	else if (scancode == 72)
	{
		if (kernel.screens[kernel.screen_index].row > 0)
			kernel.screens[kernel.screen_index].row--;
		else
			kernel.screens[kernel.screen_index].row = VGA_HEIGHT;
	}
	vga_set_cursor(kernel.screens[kernel.screen_index].row, kernel.screens[kernel.screen_index].column);
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
			if (kernel.terminal_ctrl == 0)
			{
				terminal_putchar(c);
			}
		}
		else
		{
			if (scancode == 75 || scancode == 77 || scancode == 80 || scancode == 72|| scancode == 29
				|| scancode == 42 || scancode == 54)
				update_cursor(scancode);
			else
			{
				printnbr(scancode, 10);
			}
		}
    }
	else
	{ // 29 - 157
		if (scancode == 157)
		{
			kernel.terminal_ctrl = 0;
		}
		else if (scancode == 170 || scancode == 182)
		{
			kernel.terminal_shift = 0;
		}
		else if (scancode == 142)
		{

		}
		else
			;
			//printnbr(scancode, 10);
	}
    outb(0x20, 0x20);                       /* EOI au PIC */
}
