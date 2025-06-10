/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:33:23 by alexafer          #+#    #+#             */
/*   Updated: 2025/06/10 12:52:13 by alexafer         ###   ########.fr       */
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
		else if (kernel.screens[kernel.screen_index].column < VGA_WIDTH - 1)
			kernel.screens[kernel.screen_index].column++;
		else
		{
			if (kernel.screens[kernel.screen_index].row < VGA_HEIGHT - 1)
			{
				kernel.screens[kernel.screen_index].row++;
				kernel.screens[kernel.screen_index].column = 0;
			}
		}
	}
	else if (scancode == 80)
	{
		if (kernel.screens[kernel.screen_index].row < VGA_HEIGHT - 1)
		{
			kernel.screens[kernel.screen_index].row++;
		}
		else
		{
			if (kernel.screens[kernel.screen_index].offset < (VGA_HEIGHT * (NB_SCREEN - 1)) - 1)
				terminal_offset(++kernel.screens[kernel.screen_index].offset);
		}
	}
	else if (scancode == 72)
	{
		if (kernel.screens[kernel.screen_index].row > 0)
			kernel.screens[kernel.screen_index].row--;
		else
		{
			if (kernel.screens[kernel.screen_index].offset > 0)
				terminal_offset(--kernel.screens[kernel.screen_index].offset);
		}
	}
	vga_set_cursor(kernel.screens[kernel.screen_index].row, kernel.screens[kernel.screen_index].column);
}

void set_idt_gate(int n, uint32_t handler)
{
    kernel.idt[n].offset_1			= handler & 0xFFFF;
    kernel.idt[n].selector			= 0x08;          // Code segment selector
    kernel.idt[n].zero				= 0;
    kernel.idt[n].type_attributes	= 0x8E;          // Present, Ring 0, 32-bit interrupt gate
    kernel.idt[n].offset_2			= (handler >> 16) & 0xFFFF;
}

void init_idt()
{
    t_idt_descryptor idtp = {
        .size = sizeof(kernel.idt) - 1,
        .offset  = (uint32_t)&(*kernel.idt)
    };
    set_idt_gate(PIC1_DATA, (uint32_t)irq1_handler);
	extern void irq0_handler();
	set_idt_gate(PIC1_CMD, (uint32_t)irq0_handler);
    load_idt(&idtp);
}

void pic_remap(void)
{
    outb(PIC1_CMD,  ICW1);
    outb(PIC2_CMD,  ICW1);

    outb(PIC1_DATA, PIC1_OFFSET);
    outb(PIC2_DATA, PIC2_OFFSET);

    outb(PIC1_DATA, PIC1_CASCADE_IRQ2);
    outb(PIC2_DATA, PIC2_CASCADE_ID);

    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, PIC_MASK_NONE);
    outb(PIC2_DATA, PIC_MASK_NONE);
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
		//else
		//	;
			//printnbr(scancode, 10);
	}
}
