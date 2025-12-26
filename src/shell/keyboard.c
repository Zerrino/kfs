/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:33:23 by alexafer          #+#    #+#             */
/*   Updated: 2025/07/11 11:20:44 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

uint16_t	keyboard_translate_scancode(uint8_t scancode, bool shifted, bool altgr)
{
	if (kernel.keyboard_layout == NULL)
		return (0);
	if (altgr && shifted && kernel.keyboard_layout->map_altgr_shift[scancode] != 0)
		return (kernel.keyboard_layout->map_altgr_shift[scancode]);
	if (altgr && kernel.keyboard_layout->map_altgr[scancode] != 0)
		return (kernel.keyboard_layout->map_altgr[scancode]);
	if (shifted && kernel.keyboard_layout->map_shift[scancode] != 0)
		return (kernel.keyboard_layout->map_shift[scancode]);
	return (kernel.keyboard_layout->map[scancode]);
}

void keyboard_init() {
    while (inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_READY); // Wait for keyboard controller to be ready
    outb(KEYBOARD_DATA_PORT, KEYBOARD_CMD_RESET); // Reset keyboard
    while (!(inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_DATA)); // Wait for acknowledgment
    uint8_t response = inb(KEYBOARD_DATA_PORT);
    if (response != KEYBOARD_ACK)
        terminal_writestring("Keyboard reset failed\n");

    while (inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_READY); // Enable keyboard
    outb(KEYBOARD_STATUS_PORT, KEYBOARD_CMD_ENABLE);
    keyboard_layouts_init();
    kernel.terminal_altgr = 0;
}

static void	left_arrow()
{
	if (kernel.terminal_ctrl && kernel.terminal_shift)
	{
		if (0 < kernel.screen_index)
		{
			kernel.screen_index--;
			terminal_restore();
			vga_cursor_restore();
			return ;
		}
	}
	else if (0 < kernel.screens[kernel.screen_index].column)
		kernel.screens[kernel.screen_index].column--;
	else
	{
		if (0 < kernel.screens[kernel.screen_index].row)
		{
			kernel.screens[kernel.screen_index].row--;
			kernel.screens[kernel.screen_index].column = VGA_WIDTH;
		}
	}
}

static void	right_arrow()
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

static void	down_arrow()
{
	if (kernel.screens[kernel.screen_index].row < VGA_HEIGHT - 1)
		kernel.screens[kernel.screen_index].row++;
	else
	{
		if (kernel.screens[kernel.screen_index].offset < (VGA_HEIGHT * (NB_SCROLL - 1)) - 1)
			terminal_offset(++kernel.screens[kernel.screen_index].offset);
	}
}

static void	up_arrow()
{
	if (0 < kernel.screens[kernel.screen_index].row)
		kernel.screens[kernel.screen_index].row--;
	else
	{
		if (0 < kernel.screens[kernel.screen_index].offset)
			terminal_offset(--kernel.screens[kernel.screen_index].offset);
	}
}

void	update_cursor(int scancode)
{
	switch (scancode)
	{
		case SCANCODE_CTRL_PRESS:
			kernel.terminal_ctrl = 1;
			break;
		case SCANCODE_LSHIFT_PRESS:
		case SCANCODE_RSHIFT_PRESS:
			kernel.terminal_shift = 1;
			break;
		case SCANCODE_LEFT_ARROW:
			left_arrow();
			break;
		case SCANCODE_RIGHT_ARROW:
			right_arrow();
			break;
		case SCANCODE_DOWN_ARROW:
			down_arrow();
			break;
		case SCANCODE_UP_ARROW:
			up_arrow();
			break;
		default:
			break;
	}
	vga_set_cursor(kernel.screens[kernel.screen_index].row, kernel.screens[kernel.screen_index].column);
}

void keyboard_handler(t_registers* regs)
{
    (void)regs; // Suppress unused parameter warning

    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    if (!(scancode & 0x80))
    {
        /* Handle special keys first (arrows, ctrl, shift, etc.) */
        if (scancode == SCANCODE_LEFT_ARROW || scancode == SCANCODE_RIGHT_ARROW ||
            scancode == SCANCODE_DOWN_ARROW || scancode == SCANCODE_UP_ARROW ||
            scancode == SCANCODE_CTRL_PRESS || scancode == SCANCODE_LSHIFT_PRESS ||
            scancode == SCANCODE_RSHIFT_PRESS || scancode == SCANCODE_ALT_PRESS)
        {
            if (scancode == SCANCODE_ALT_PRESS)
                kernel.terminal_altgr = 1;
            update_cursor(scancode);
            return;
        }

        uint16_t cp = keyboard_translate_scancode(scancode, kernel.terminal_shift, kernel.terminal_altgr);
        char c = (cp <= 0x7F) ? (char)cp : '?';
        if (c)
        {
            /* Handle ESC key to exit shell mode */
            if (c == SCANCODE_ESC) /* ESC key */
            {
                if (kernel.screens[kernel.screen_index].shell_mode) {
                    kernel.screens[kernel.screen_index].shell_mode = 0;
                    terminal_writestring("\nNavigation mode. Use arrow keys to move, type to enter shell.\n");
                }
                return;
            }

            /* If not in shell mode and user types, enter shell mode */
            if (!kernel.screens[kernel.screen_index].shell_mode && c != SCANCODE_ESC) {
                kernel.screens[kernel.screen_index].shell_mode = 1;
                terminal_writestring("\n");
            }

            /* Send to shell if in shell mode and not in control mode */
            if (kernel.screens[kernel.screen_index].shell_mode && kernel.terminal_ctrl == 0)
                shell_handle_input(c);
            /* In navigation mode, just display the character */
            else if (!kernel.screens[kernel.screen_index].shell_mode)
                terminal_putchar(c);
        }
    }
    else
    {
        /* Handle key releases */
        if (scancode == SCANCODE_CTRL_RELEASE) /* Ctrl release */
            kernel.terminal_ctrl = 0;
        else if (scancode == SCANCODE_LSHIFT_RELEASE || scancode == SCANCODE_RSHIFT_RELEASE) /* Shift release */
            kernel.terminal_shift = 0;
        else if (scancode == SCANCODE_ALT_RELEASE)
            kernel.terminal_altgr = 0;
    }
}
