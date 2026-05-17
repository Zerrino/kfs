/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:33:23 by alexafer          #+#    #+#             */
/*   Updated: 2026/05/17 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

static const char g_qwerty[] = {
	[1] = 27, [2] = '1', [3] = '2', [4] = '3', [5] = '4', [6] = '5',
	[7] = '6', [8] = '7', [9] = '8', [10] = '9', [11] = '0', [12] = '-',
	[13] = '=', [14] = '\b', [15] = '\t', [16] = 'q', [17] = 'w',
	[18] = 'e', [19] = 'r', [20] = 't', [21] = 'y', [22] = 'u',
	[23] = 'i', [24] = 'o', [25] = 'p', [26] = '[', [27] = ']',
	[28] = '\n', [30] = 'a', [31] = 's', [32] = 'd', [33] = 'f',
	[34] = 'g', [35] = 'h', [36] = 'j', [37] = 'k', [38] = 'l',
	[39] = ';', [40] = '\'', [41] = '`', [43] = '\\', [44] = 'z',
	[45] = 'x', [46] = 'c', [47] = 'v', [48] = 'b', [49] = 'n',
	[50] = 'm', [51] = ',', [52] = '.', [53] = '/', [55] = '*',
	[57] = ' '
};

static const char g_qwerty_shift[] = {
	[1] = 27, [2] = '!', [3] = '@', [4] = '#', [5] = '$', [6] = '%',
	[7] = '^', [8] = '&', [9] = '*', [10] = '(', [11] = ')', [12] = '_',
	[13] = '+', [14] = '\b', [15] = '\t', [16] = 'Q', [17] = 'W',
	[18] = 'E', [19] = 'R', [20] = 'T', [21] = 'Y', [22] = 'U',
	[23] = 'I', [24] = 'O', [25] = 'P', [26] = '{', [27] = '}',
	[28] = '\n', [30] = 'A', [31] = 'S', [32] = 'D', [33] = 'F',
	[34] = 'G', [35] = 'H', [36] = 'J', [37] = 'K', [38] = 'L',
	[39] = ':', [40] = '"', [41] = '~', [43] = '|', [44] = 'Z',
	[45] = 'X', [46] = 'C', [47] = 'V', [48] = 'B', [49] = 'N',
	[50] = 'M', [51] = '<', [52] = '>', [53] = '?', [55] = '*',
	[57] = ' '
};

static const char g_azerty[] = {
	[1] = 27, [2] = '&', [3] = 'e', [4] = '"', [5] = '\'', [6] = '(',
	[7] = '-', [8] = 'e', [9] = '_', [10] = 'c', [11] = 'a', [12] = ')',
	[13] = '=', [14] = '\b', [15] = '\t', [16] = 'a', [17] = 'z',
	[18] = 'e', [19] = 'r', [20] = 't', [21] = 'y', [22] = 'u',
	[23] = 'i', [24] = 'o', [25] = 'p', [26] = '^', [27] = '$',
	[28] = '\n', [30] = 'q', [31] = 's', [32] = 'd', [33] = 'f',
	[34] = 'g', [35] = 'h', [36] = 'j', [37] = 'k', [38] = 'l',
	[39] = 'm', [40] = 'u', [41] = '2', [43] = '*', [44] = 'w',
	[45] = 'x', [46] = 'c', [47] = 'v', [48] = 'b', [49] = 'n',
	[50] = ',', [51] = ';', [52] = ':', [53] = '!', [55] = '*',
	[57] = ' '
};

static const char g_azerty_shift[] = {
	[1] = 27, [2] = '1', [3] = '2', [4] = '3', [5] = '4', [6] = '5',
	[7] = '6', [8] = '7', [9] = '8', [10] = '9', [11] = '0', [12] = 'o',
	[13] = '+', [14] = '\b', [15] = '\t', [16] = 'A', [17] = 'Z',
	[18] = 'E', [19] = 'R', [20] = 'T', [21] = 'Y', [22] = 'U',
	[23] = 'I', [24] = 'O', [25] = 'P', [26] = '"', [27] = '#',
	[28] = '\n', [30] = 'Q', [31] = 'S', [32] = 'D', [33] = 'F',
	[34] = 'G', [35] = 'H', [36] = 'J', [37] = 'K', [38] = 'L',
	[39] = 'M', [40] = '%', [41] = '~', [43] = 'u', [44] = 'W',
	[45] = 'X', [46] = 'C', [47] = 'V', [48] = 'B', [49] = 'N',
	[50] = '?', [51] = '.', [52] = '/', [53] = 's', [55] = '*',
	[57] = ' '
};

void keyboard_init()
{
	kernel.keyboard_read = 0;
	kernel.keyboard_write = 0;
	kernel.keyboard_count = 0;
	kernel.keyboard_line_len = 0;
	kernel.keyboard_line_cursor = 0;
	kernel.keyboard_line_ready = 0;
	kernel.keyboard_extended = 0;
	kernel.keyboard_caps = 0;
	kernel.keyboard_layout = KEYBOARD_LAYOUT_QWERTY;
	while (inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_READY)
		;
	outb(KEYBOARD_DATA_PORT, KEYBOARD_CMD_RESET);
	while (!(inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_DATA))
		;
	if (inb(KEYBOARD_DATA_PORT) != KEYBOARD_ACK)
		terminal_writestring("Keyboard reset failed\n");
	while (inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_READY)
		;
	outb(KEYBOARD_STATUS_PORT, KEYBOARD_CMD_ENABLE);
}

void keyboard_set_layout(keyboard_layout_t layout)
{
	kernel.keyboard_layout = layout;
}

static void keyboard_cursor_left(void)
{
	t_screens *screen;

	screen = &kernel.screens[kernel.screen_index];
	if (screen->column > 0)
		screen->column--;
	else if (screen->row > 0)
	{
		screen->row--;
		screen->column = VGA_WIDTH - 1;
	}
	vga_set_cursor(screen->row, screen->column);
}

static void keyboard_cursor_right(void)
{
	t_screens *screen;

	screen = &kernel.screens[kernel.screen_index];
	if (screen->column < VGA_WIDTH - 1)
		screen->column++;
	else if (screen->row < VGA_HEIGHT - 1)
	{
		screen->row++;
		screen->column = 0;
	}
	vga_set_cursor(screen->row, screen->column);
}

static void keyboard_complete_line(void)
{
	if (kernel.keyboard_line_ready)
		return ;
	kernel.keyboard_line[kernel.keyboard_line_len] = '\0';
	while (kernel.keyboard_line_cursor < kernel.keyboard_line_len)
	{
		kernel.keyboard_line_cursor++;
		keyboard_cursor_right();
	}
	kernel.keyboard_line_ready = 1;
	terminal_putchar('\n');
}

static void keyboard_put_printable(char c)
{
	if (kernel.keyboard_line_ready)
		return ;
	if (kernel.keyboard_line_cursor == kernel.keyboard_line_len)
	{
		if (kernel.keyboard_line_len >= KEYBOARD_LINE_SIZE - 1)
			return ;
		kernel.keyboard_line_len++;
	}
	kernel.keyboard_line[kernel.keyboard_line_cursor] = c;
	terminal_putchar(c);
	kernel.keyboard_line_cursor++;
}

static void keyboard_redraw_from_cursor(void)
{
	uint32_t cursor;
	uint32_t moves;

	cursor = kernel.keyboard_line_cursor;
	for (uint32_t i = cursor; i < kernel.keyboard_line_len; i++)
		terminal_putchar(kernel.keyboard_line[i]);
	terminal_putchar(' ');
	moves = kernel.keyboard_line_len - cursor + 1;
	while (moves-- > 0)
		keyboard_cursor_left();
}

static void keyboard_backspace(void)
{
	uint32_t cursor;

	if (kernel.keyboard_line_ready || kernel.keyboard_line_cursor == 0)
		return ;
	keyboard_cursor_left();
	kernel.keyboard_line_cursor--;
	cursor = kernel.keyboard_line_cursor;
	while (cursor + 1 < kernel.keyboard_line_len)
	{
		kernel.keyboard_line[cursor] = kernel.keyboard_line[cursor + 1];
		cursor++;
	}
	kernel.keyboard_line_len--;
	keyboard_redraw_from_cursor();
}

static void keyboard_move_left(void)
{
	if (kernel.keyboard_line_ready || kernel.keyboard_line_cursor == 0)
		return ;
	kernel.keyboard_line_cursor--;
	keyboard_cursor_left();
}

static void keyboard_move_right(void)
{
	if (kernel.keyboard_line_ready || kernel.keyboard_line_cursor >= kernel.keyboard_line_len)
		return ;
	kernel.keyboard_line_cursor++;
	keyboard_cursor_right();
}

static void keyboard_push_char(char c)
{
	if (kernel.keyboard_count == KEYBOARD_BUFFER_SIZE)
	{
		kernel.keyboard_read = (kernel.keyboard_read + 1) % KEYBOARD_BUFFER_SIZE;
		kernel.keyboard_count--;
	}
	kernel.keyboard_buffer[kernel.keyboard_write] = c;
	kernel.keyboard_write = (kernel.keyboard_write + 1) % KEYBOARD_BUFFER_SIZE;
	kernel.keyboard_count++;
	if (c == '\n')
		keyboard_complete_line();
	else if (c == '\b')
		keyboard_backspace();
	else if (' ' <= c && c <= '~')
		keyboard_put_printable(c);
}

int keyboard_read_char(char *c)
{
	if (kernel.keyboard_count == 0 || c == NULL)
		return (0);
	*c = kernel.keyboard_buffer[kernel.keyboard_read];
	kernel.keyboard_read = (kernel.keyboard_read + 1) % KEYBOARD_BUFFER_SIZE;
	kernel.keyboard_count--;
	return (1);
}

int keyboard_readline(char *buffer, size_t size)
{
	uint32_t i;

	if (!kernel.keyboard_line_ready || buffer == NULL || size == 0)
		return (0);
	i = 0;
	while (i + 1 < size && i < kernel.keyboard_line_len)
	{
		buffer[i] = kernel.keyboard_line[i];
		i++;
	}
	buffer[i] = '\0';
	kernel.keyboard_line_len = 0;
	kernel.keyboard_line_cursor = 0;
	kernel.keyboard_line_ready = 0;
	return (1);
}

size_t keyboard_getline(char *buffer, size_t size)
{
	while (!keyboard_readline(buffer, size))
		__asm__ volatile ("hlt");
	return (strlen(buffer));
}

void update_cursor(int scancode)
{
	switch (scancode)
	{
		case SCANCODE_LEFT_ARROW:
			keyboard_move_left();
			break;
		case SCANCODE_RIGHT_ARROW:
			keyboard_move_right();
			break;
		default:
			break;
	}
}

static char keyboard_translate(uint8_t scancode)
{
	const char *map;
	uint8_t shifted;
	char c;

	if (scancode >= 128)
		return (0);
	shifted = kernel.terminal_shift;
	if (kernel.keyboard_layout == KEYBOARD_LAYOUT_AZERTY)
		map = shifted ? g_azerty_shift : g_azerty;
	else
		map = shifted ? g_qwerty_shift : g_qwerty;
	c = map[scancode];
	if (kernel.keyboard_caps && c >= 'a' && c <= 'z')
		c -= 'a' - 'A';
	else if (kernel.keyboard_caps && c >= 'A' && c <= 'Z')
		c += 'a' - 'A';
	return (c);
}

void keyboard_handler(t_registers* regs)
{
	uint8_t scancode;
	char c;

	scancode = regs->edi & 0xFF;
	if (scancode == SCANCODE_E0_PREFIX)
	{
		kernel.keyboard_extended = 1;
		return ;
	}
	if (scancode & 0x80)
	{
		if (!kernel.keyboard_extended && scancode == SCANCODE_CTRL_RELEASE)
			kernel.terminal_ctrl = 0;
		else if (!kernel.keyboard_extended && (scancode == SCANCODE_LSHIFT_RELEASE ||
				scancode == SCANCODE_RSHIFT_RELEASE))
			kernel.terminal_shift = 0;
		kernel.keyboard_extended = 0;
		return ;
	}
	if (!kernel.keyboard_extended && scancode == SCANCODE_CTRL_PRESS)
		kernel.terminal_ctrl = 1;
	else if (!kernel.keyboard_extended && (scancode == SCANCODE_LSHIFT_PRESS ||
			scancode == SCANCODE_RSHIFT_PRESS))
		kernel.terminal_shift = 1;
	else if (!kernel.keyboard_extended && scancode == SCANCODE_CAPS_LOCK)
		kernel.keyboard_caps = !kernel.keyboard_caps;
	else if (scancode == SCANCODE_LEFT_ARROW || scancode == SCANCODE_RIGHT_ARROW ||
			scancode == SCANCODE_DOWN_ARROW || scancode == SCANCODE_UP_ARROW)
		update_cursor(scancode);
	else
	{
		c = keyboard_translate(scancode);
		if (c && c != 27)
			keyboard_push_char(c);
	}
	kernel.keyboard_extended = 0;
}
