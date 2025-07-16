/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_write.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerrino <zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:26:11 by alexafer          #+#    #+#             */
/*   Updated: 2025/07/15 17:56:07 by zerrino          ###   ########.fr       */
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
	kernel.terminal_buffer[index] = vga_entry(c, color);
	kernel.screens[kernel.screen_index].content[index + (kernel.screens[kernel.screen_index].offset * VGA_WIDTH)] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
	if (c == '\0')
		return ;

	if (c == '\b')
	{
		if (0 < kernel.screens[kernel.screen_index].column)
			kernel.screens[kernel.screen_index].column--;
		else
		{
			if (0 < kernel.screens[kernel.screen_index].row)
			{
				kernel.screens[kernel.screen_index].row--;
				kernel.screens[kernel.screen_index].column = VGA_WIDTH;
			}
		}
		terminal_putentryat(' ', kernel.screens[kernel.screen_index].color, kernel.screens[kernel.screen_index].column, kernel.screens[kernel.screen_index].row);
	}
	else
	{
		if (c == '\n')
		{
			if (++kernel.screens[kernel.screen_index].row == VGA_HEIGHT)
				kernel.screens[kernel.screen_index].row = 0;
			kernel.screens[kernel.screen_index].column = 0;
		}
		else
		{
			if (kernel.terminal_shift && 'a' <= c && c <= 'z')
				c -= 32;
			terminal_putentryat(c, kernel.screens[kernel.screen_index].color, kernel.screens[kernel.screen_index].column, kernel.screens[kernel.screen_index].row);
			if (++kernel.screens[kernel.screen_index].column == VGA_WIDTH) {
				kernel.screens[kernel.screen_index].column = 0;
				if (++kernel.screens[kernel.screen_index].row == VGA_HEIGHT)
					kernel.screens[kernel.screen_index].row = 0;
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

void	printnbr(int nbr, int base)
{
	terminal_putchar('-' * -(N(nbr)));
	if (nbr == INT_MIN || base <= ABS(nbr))
		printnbr(ABS(nbr / base), base);

	terminal_putchar(ABS(nbr % base) + '0' + (7 * (9 < ABS(nbr % base))));
}
