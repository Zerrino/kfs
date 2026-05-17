/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inline_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:23:39 by alexafer          #+#    #+#             */
/*   Updated: 2025/07/09 14:58:54 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

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

static uint16_t vga_entry_invert(uint16_t entry)
{
	uint8_t color;
	uint8_t inverted;

	color = (entry >> 8) & BYTE_MASK;
	inverted = ((color & 0x0F) << 4) | ((color & 0xF0) >> 4);
	return ((entry & BYTE_MASK) | ((uint16_t)inverted << 8));
}

void vga_cursor_hide(void)
{
	if (!kernel.cursor_visible)
		return ;
	if (kernel.cursor_saved_pos < VGA_WIDTH * VGA_HEIGHT)
		kernel.terminal_buffer[kernel.cursor_saved_pos] = kernel.cursor_saved_entry;
	kernel.cursor_visible = 0;
}

void vga_cursor_show(void)
{
	uint16_t pos;

	if (kernel.cursor_visible || kernel.terminal_buffer == NULL)
		return ;
	pos = kernel.screens[kernel.screen_index].pos_cursor;
	if (pos >= VGA_WIDTH * VGA_HEIGHT)
		return ;
	kernel.cursor_saved_pos = pos;
	kernel.cursor_saved_entry = kernel.terminal_buffer[pos];
	kernel.terminal_buffer[pos] = vga_entry_invert(kernel.cursor_saved_entry);
	kernel.cursor_visible = 1;
}

void vga_set_cursor(size_t row, size_t col)
{
	uint16_t pos;

	if (row >= VGA_HEIGHT)
		row = VGA_HEIGHT - 1;
	if (col >= VGA_WIDTH)
		col = VGA_WIDTH - 1;
	vga_cursor_hide();
	pos = (uint16_t)(row * VGA_WIDTH + col);
	kernel.screens[kernel.screen_index].pos_cursor = pos;
	outb(VGA_PORT_INDEX, VGA_CRSR_LOW);
	outb(VGA_PORT_DATA, pos & BYTE_MASK);
	outb(VGA_PORT_INDEX, VGA_CRSR_HIGH);
	outb(VGA_PORT_DATA, pos >> BITS_PER_BYTE);
	vga_cursor_show();
}

void vga_cursor_restore()
{
	vga_set_cursor(kernel.screens[kernel.screen_index].row,
		kernel.screens[kernel.screen_index].column);
}

void vga_cursor_sync(void)
{
	vga_set_cursor(kernel.screens[kernel.screen_index].row,
		kernel.screens[kernel.screen_index].column);
}

void vga_cursor_blink(void)
{
	if (kernel.tick - kernel.cursor_last_tick < CURSOR_BLINK_TICKS)
		return ;
	kernel.cursor_last_tick = kernel.tick;
	if (kernel.cursor_visible)
		vga_cursor_hide();
	else
		vga_cursor_show();
}
