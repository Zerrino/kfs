/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_write.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:26:11 by alexafer          #+#    #+#             */
/*   Updated: 2025/08/02 19:58:49 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;

	return len;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;

	if (x >= VGA_WIDTH || y >= VGA_HEIGHT)
		return ;
	vga_cursor_hide();
	kernel.terminal_buffer[index] = vga_entry(c, color);
	kernel.screens[kernel.screen_index].content[index +
		(kernel.screens[kernel.screen_index].offset * VGA_WIDTH)] = vga_entry(c, color);
}

void terminal_scroll_up(void)
{
	size_t		index;
	size_t		last_line;
	t_screens	*screen;

	vga_cursor_hide();
	screen = &kernel.screens[kernel.screen_index];
	for (size_t y = 1; y < VGA_HEIGHT * NB_SCROLL; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			index = y * VGA_WIDTH + x;
			screen->content[index - VGA_WIDTH] = screen->content[index];
		}
	}
	last_line = (VGA_HEIGHT * NB_SCROLL - 1) * VGA_WIDTH;
	for (size_t x = 0; x < VGA_WIDTH; x++)
		screen->content[last_line + x] = vga_entry(' ', screen->color);
	screen->offset = 0;
	terminal_restore();
	vga_cursor_sync();
}

void terminal_putchar(char c)
{
	if (c == '\0')
		return ;
	vga_cursor_hide();

	if (c == '\b')
	{
		if (0 < kernel.screens[kernel.screen_index].column)
			kernel.screens[kernel.screen_index].column--;
		else if (0 < kernel.screens[kernel.screen_index].row)
		{
			kernel.screens[kernel.screen_index].row--;
			kernel.screens[kernel.screen_index].column = VGA_WIDTH - 1;
		}
		terminal_putentryat(' ', kernel.screens[kernel.screen_index].color, kernel.screens[kernel.screen_index].column, kernel.screens[kernel.screen_index].row);
	}
	else
	{
		if (c == '\n')
		{
			kernel.screens[kernel.screen_index].column = 0;
			if (++kernel.screens[kernel.screen_index].row == VGA_HEIGHT)
			{
				terminal_scroll_up();
				kernel.screens[kernel.screen_index].row = VGA_HEIGHT - 1;
			}
		}
		else
		{
			terminal_putentryat(c, kernel.screens[kernel.screen_index].color, kernel.screens[kernel.screen_index].column, kernel.screens[kernel.screen_index].row);
			if (++kernel.screens[kernel.screen_index].column == VGA_WIDTH) {
				kernel.screens[kernel.screen_index].column = 0;
				if (++kernel.screens[kernel.screen_index].row == VGA_HEIGHT)
				{
					terminal_scroll_up();
					kernel.screens[kernel.screen_index].row = VGA_HEIGHT - 1;
				}
			}
		}
	}
	vga_set_cursor(kernel.screens[kernel.screen_index].row, kernel.screens[kernel.screen_index].column);
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

void	printnbr(uint32_t nbr, uint32_t base)
{
	//terminal_putchar('-' * -(N(nbr)));
	if (base <= nbr)
		printnbr(nbr / base, base);

	terminal_putchar(nbr % base + '0' + (7 * (9 < nbr % base)));
}
