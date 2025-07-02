/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerrino <zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:33:23 by alexafer          #+#    #+#             */
/*   Updated: 2025/07/02 18:26:07 by zerrino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

static void	left_arrow()
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
	{
		kernel.screens[kernel.screen_index].row++;
	}
	else
	{
		if (kernel.screens[kernel.screen_index].offset < (VGA_HEIGHT * (NB_SCREEN - 1)) - 1)
			terminal_offset(++kernel.screens[kernel.screen_index].offset);
	}
}

static void	up_arrow()
{
	if (kernel.screens[kernel.screen_index].row > 0)
		kernel.screens[kernel.screen_index].row--;
	else
	{
		if (kernel.screens[kernel.screen_index].offset > 0)
			terminal_offset(--kernel.screens[kernel.screen_index].offset);
	}
}

void	update_cursor(int scancode)
{
	switch (scancode)
	{
		case 29:
			kernel.terminal_ctrl = 1;
			break;
		case 42:
		case 54:
			kernel.terminal_shift = 1;
			break;
		case 75:
			left_arrow();
			break;
		case 77:
			right_arrow();
			break;
		case 80:
			down_arrow();
			break;
		case 72:
			up_arrow();
			break;
		default:
			break;
	}
	vga_set_cursor(kernel.screens[kernel.screen_index].row, kernel.screens[kernel.screen_index].column);
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
        /* Handle special keys first (arrows, ctrl, shift, etc.) */
        if (scancode == 75 || scancode == 77 || scancode == 80 || scancode == 72|| scancode == 29
            || scancode == 42 || scancode == 54)
        {
            update_cursor(scancode);
            return;
        }

        char c = scancode_to_ascii[scancode];
        if (c)
        {
            /* Handle ESC key to exit shell mode */
            if (c == 27) /* ESC key */
            {
                if (kernel.screens[kernel.screen_index].shell_mode) {
                    kernel.screens[kernel.screen_index].shell_mode = 0;
                    terminal_writestring("\nNavigation mode. Use arrow keys to move, type to enter shell.\n");
                }
                return;
            }

            /* If not in shell mode and user types, enter shell mode */
            if (!kernel.screens[kernel.screen_index].shell_mode && c != 27) {
                kernel.screens[kernel.screen_index].shell_mode = 1;
                terminal_writestring("\n");
                //shell_initialize();
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
        if (scancode == 157) /* Ctrl release */
            kernel.terminal_ctrl = 0;
        else if (scancode == 170 || scancode == 182) /* Shift release */
            kernel.terminal_shift = 0;
    }
}
