/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 15:55:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/27 15:55:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

size_t	kgetline(char *buffer, size_t max_len)
{
	size_t	len;
	bool	shift;
	bool	altgr;

	if (buffer == NULL || max_len == 0)
		return (0);
	len = 0;
	shift = false;
	altgr = false;
	DisableInterrupts();
	while (len + 1 < max_len)
	{
		/* Wait for key data */
		while (!(inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_DATA))
			;
		uint8_t scancode = inb(KEYBOARD_DATA_PORT);
		if (scancode & 0x80)
		{
			/* Key release */
			if (scancode == SCANCODE_LSHIFT_RELEASE || scancode == SCANCODE_RSHIFT_RELEASE)
				shift = false;
			else if (scancode == SCANCODE_ALT_RELEASE)
				altgr = false;
			continue;
		}
		if (scancode == SCANCODE_LSHIFT_PRESS || scancode == SCANCODE_RSHIFT_PRESS)
		{
			shift = true;
			continue;
		}
		if (scancode == SCANCODE_ALT_PRESS)
		{
			altgr = true;
			continue;
		}
		uint16_t cp = keyboard_translate_scancode(scancode, shift, altgr);
		char c = (cp <= 0x7F) ? (char)cp : '?';
		if (c == '\b')
		{
			if (len > 0)
			{
				len--;
				terminal_putchar('\b');
				terminal_putchar(' ');
				terminal_putchar('\b');
			}
			continue;
		}
		if (c == '\n')
		{
			terminal_putchar('\n');
			break;
		}
		if (c == 0)
			continue;
		buffer[len++] = c;
		terminal_putchar(c);
	}
	buffer[len] = '\0';
	EnableInterrupts();
	return (len);
}
