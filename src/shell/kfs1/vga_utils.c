/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:23:39 by alexafer          #+#    #+#             */
/*   Updated: 2025/07/10 16:01:54 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kfs.h"

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

void outb(uint16_t port, uint8_t val)
{
    __asm__ __volatile__("outb %0, %1" : : "a"(val), "Nd"(port));
}

void outw(uint16_t port, uint16_t val)
{
    __asm__ __volatile__("outw %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port)
{
	uint8_t	ret;
	__asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

void vga_set_cursor(size_t row, size_t col)
{

    uint16_t pos = (uint16_t)(row * VGA_WIDTH + col);
    kernel.screens[kernel.screen_index].pos_cursor = pos;
    outb(VGA_PORT_INDEX, VGA_CRSR_LOW);
    outb(VGA_PORT_DATA,  pos & BYTE_MASK);
    outb(VGA_PORT_INDEX, VGA_CRSR_HIGH);
    outb(VGA_PORT_DATA,  pos >> BITS_PER_BYTE);
}

void vga_cursor_restore()
{
    outb(VGA_PORT_INDEX, VGA_CRSR_LOW);
    outb(VGA_PORT_DATA,  kernel.screens[kernel.screen_index].pos_cursor & BYTE_MASK);
    outb(VGA_PORT_INDEX, VGA_CRSR_HIGH);
    outb(VGA_PORT_DATA,  kernel.screens[kernel.screen_index].pos_cursor >> BITS_PER_BYTE);
}
