/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:33:23 by alexafer          #+#    #+#             */
/*   Updated: 2025/07/10 16:01:54 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kfs.h"

void keyboard_init() {
    while (inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_READY); // Wait for keyboard controller to be ready
    outb(KEYBOARD_DATA_PORT, KEYBOARD_CMD_RESET); // Reset keyboard
    while (!(inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_DATA)); // Wait for acknowledgment
    uint8_t response = inb(KEYBOARD_DATA_PORT);
    if (response != KEYBOARD_ACK)
        terminal_writestring("Keyboard reset failed\n");

    while (inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_READY); // Enable keyboard
    outb(KEYBOARD_STATUS_PORT, KEYBOARD_CMD_ENABLE);
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

    static const char scancode_to_ascii[] = {
        0, 27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',    /* 0-14 */
        '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',      /* 15-28 */
        0,'a','s','d','f','g','h','j','k','l',';','\'','`',             /* 29-41 */
        0,'\\','z','x','c','v','b','n','m',',','.','/', 0,'*',          /* 42-55 */
        0,' '                                                           /* 56 */
    };

    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    if (!(scancode & 0x80))
    {
        /* Handle special keys first (arrows, ctrl, shift, etc.) */
        if (scancode == SCANCODE_LEFT_ARROW || scancode == SCANCODE_RIGHT_ARROW ||
            scancode == SCANCODE_DOWN_ARROW || scancode == SCANCODE_UP_ARROW ||
            scancode == SCANCODE_CTRL_PRESS || scancode == SCANCODE_LSHIFT_PRESS ||
            scancode == SCANCODE_RSHIFT_PRESS)
        {
            update_cursor(scancode);
            return;
        }

        char c = scancode_to_ascii[scancode];
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
    }
}
