/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:20 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/05 16:06:13 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

#define COMMAND_BUFFER_SIZE 256
static char command_buffer[COMMAND_BUFFER_SIZE];
static int buffer_pos = 0;

command_type_t get_command_type(const char* command) {
    command_type_t cmd_type;

    cmd_type = get_kfs2_command_type(command);
    if (cmd_type != CMD_UNKNOWN)
        return cmd_type;

    cmd_type = get_kfs3_memory_command_type(command);
    if (cmd_type != CMD_UNKNOWN)
        return cmd_type;

    cmd_type = get_kfs3_crash_command_type(command);
    if (cmd_type != CMD_UNKNOWN)
        return cmd_type;

    return CMD_UNKNOWN;
}

void shell_process_command(const char* cmd) {
    char command[32] = {0};
    char arg[32] = {0};
    int i = 0, j = 0;

    while (cmd[i] && cmd[i] != ' ' && i < 31) {
        command[i] = cmd[i];
        i++;
    }
    command[i] = '\0';

    while (cmd[i] && cmd[i] == ' ')
        i++;

    while (cmd[i] && j < 31) {
        arg[j] = cmd[i];
        i++;
        j++;
    }
    arg[j] = '\0';

    command_type_t cmd_type = get_command_type(command);
    if (handle_kfs2_commands(cmd_type, arg)) {
        /* KFS2 command was handled */
    } else if (handle_kfs3_memory_commands(cmd_type, arg)) {
        /* KFS3 memory command was handled */
    } else if (handle_kfs3_crash_commands(cmd_type, arg)) {
        /* KFS3 crash command was handled */
    } else if (cmd_type == CMD_UNKNOWN) {
        if (command[0] != '\0')
            handle_unknown(command);
    }

    terminal_writestring("> ");
}

void shell_handle_input(char c) {
    if (c == '\n') {
        terminal_putchar('\n');
        command_buffer[buffer_pos] = '\0';
        shell_process_command(command_buffer);
        buffer_pos = 0;
    } else if (c == '\b' && buffer_pos > 0) {
        buffer_pos--;
        terminal_putchar('\b');
        terminal_putchar(' ');
        terminal_putchar('\b');
    } else if (c >= ' ' && c <= '~' && buffer_pos < COMMAND_BUFFER_SIZE - 1) {
        command_buffer[buffer_pos++] = c;
        terminal_putchar(c);
    }
}
