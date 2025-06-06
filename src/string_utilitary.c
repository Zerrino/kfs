/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utilitary.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:26:11 by alexafer          #+#    #+#             */
/*   Updated: 2025/06/06 15:36:32 by alexafer         ###   ########.fr       */
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
}

void terminal_putchar(char c)
{
	if (c == '\0')
		return ;
	if (c == '\b')
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
		terminal_putentryat(' ', kernel.terminal_color, kernel.terminal_column, kernel.terminal_row);
	}
	else
	{
		if (c == '\n')
		{
			if (++kernel.terminal_row == VGA_HEIGHT)
				kernel.terminal_row = 0;
			kernel.terminal_column = 0;
		}
		else
		{
			terminal_putentryat(c, kernel.terminal_color, kernel.terminal_column, kernel.terminal_row);
			if (++kernel.terminal_column == VGA_WIDTH) {
				kernel.terminal_column = 0;
				if (++kernel.terminal_row == VGA_HEIGHT)
					kernel.terminal_row = 0;
			}
		}
	}
	vga_set_cursor(kernel.terminal_row, kernel.terminal_column);
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
