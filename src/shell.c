/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:20 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/09 21:18:44 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

void shell_initialize() {
    terminal_writestring("KFS Shell v1.0\n");
    terminal_writestring("Type 'help' for available commands\n");
    if (kernel.skip_next_prompt)
        kernel.skip_next_prompt = 0;
    else
        terminal_writestring("> ");
}

void handle_help() {
    terminal_writestring("Available commands:\n");
    terminal_writestring("  help         - Display this help message\n");
    terminal_writestring("  kfs1         - Show KFS1 menu\n");
    terminal_writestring("  kfs2         - Show KFS2 menu\n");
    terminal_writestring("  kfs3         - Show KFS3 menu\n");
    terminal_writestring("  kfs4         - Show KFS4 menu\n");
    terminal_writestring("  memtest      - Run memory allocation demo\n");
}

void handle_unknown(const char* command) {
    terminal_writestring("Unknown command: ");
    terminal_writestring(command);
    terminal_writestring("\n");
}

command_type_t get_general_command_type(const char* command) {
    if (ft_strcmp(command, "help") == 0)
        return CMD_HELP;
    if (ft_strcmp(command, "kfs1") == 0)
        return CMD_KFS1;
    if (ft_strcmp(command, "kfs2") == 0)
        return CMD_KFS2MENU;
    if (ft_strcmp(command, "kfs3") == 0)
        return CMD_KFS3;
    if (ft_strcmp(command, "kfs4") == 0)
        return CMD_KFS4;
    if (ft_strcmp(command, "memtest") == 0)
        return CMD_MEMTEST;

    return CMD_UNKNOWN;
}

bool handle_general_commands(command_type_t cmd_type, const char* arg) {
    (void)arg;
    switch (cmd_type) {
        case CMD_HELP:
            handle_help();
            return true;
        case CMD_KFS1:
            display_kfs1_help();
            return true;
        case CMD_KFS2MENU:
            display_kfs2_help();
            return true;
        case CMD_KFS3:
            display_kfs3_help();
            return true;
        case CMD_KFS4:
            display_kfs4_help();
            return true;
        case CMD_MEMTEST:
            handle_memtest();
            return true;
        default:
            return false;
    }
}


command_type_t get_command_type(const char* command) {
    command_type_t cmd_type;

    cmd_type = get_general_command_type(command);
    if (cmd_type != CMD_UNKNOWN)
        return cmd_type;

    cmd_type = get_kfs2_command_type(command);
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
    if (handle_general_commands(cmd_type, arg)) {
        /* General command was handled */
    } else if (handle_kfs2_commands(cmd_type, arg)) {
        /* KFS2 command was handled */
    } else if (cmd_type == CMD_UNKNOWN) {
        if (command[0] != '\0')
            handle_unknown(command);
    }

    terminal_writestring("> ");
}

void shell_handle_input(char c) {
    if (c == '\n') {
        terminal_putchar('\n');
        kernel.command_buffer[kernel.buffer_pos] = '\0';
        shell_process_command(kernel.command_buffer);
        kernel.buffer_pos = 0;
    } else if (c == '\b' && 0 < kernel.buffer_pos) {
        kernel.buffer_pos--;
        terminal_putchar('\b');
        terminal_putchar(' ');
        terminal_putchar('\b');
    } else if (' ' <= c && c <= '~' && kernel.buffer_pos < COMMAND_BUFFER_SIZE - 1) {
        kernel.command_buffer[kernel.buffer_pos++] = c;
        terminal_putchar(c);
    }
}
