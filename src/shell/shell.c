/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:20 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/05 15:54:03 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

#define COMMAND_BUFFER_SIZE 256
static char command_buffer[COMMAND_BUFFER_SIZE];
static int buffer_pos = 0;

command_type_t get_command_type(const char* command) {
	command_type_t cmd_type;

	cmd_type = CMD_UNKNOWN;
    if (ft_strcmp(command, "help") == 0)
		cmd_type = CMD_HELP;
    else if (ft_strcmp(command, "stack") == 0)
		cmd_type = CMD_STACK;
    else if (ft_strcmp(command, "push") == 0)
		cmd_type = CMD_PUSH;
    else if (ft_strcmp(command, "pop") == 0)
		cmd_type = CMD_POP;
    else if (ft_strcmp(command, "clear") == 0)
		cmd_type = CMD_CLEAR;
    else if (ft_strcmp(command, "reboot") == 0)
		cmd_type = CMD_REBOOT;
    else if (ft_strcmp(command, "halt") == 0)
		cmd_type = CMD_HALT;
    else if (ft_strcmp(command, "shutdown") == 0)
		cmd_type = CMD_SHUTDOWN;
    else if (ft_strcmp(command, "memstats") == 0)
		cmd_type = CMD_MEMSTATS;
    else if (ft_strcmp(command, "memtest") == 0)
		cmd_type = CMD_MEMTEST;
    else if (ft_strcmp(command, "memtest1") == 0)
		cmd_type = CMD_MEMTEST1;
    else if (ft_strcmp(command, "memtest2") == 0)
		cmd_type = CMD_MEMTEST2;
    else if (ft_strcmp(command, "memtest3") == 0)
		cmd_type = CMD_MEMTEST3;
    else if (ft_strcmp(command, "memtest4") == 0)
		cmd_type = CMD_MEMTEST4;
    else if (ft_strcmp(command, "memtest5") == 0)
		cmd_type = CMD_MEMTEST5;
    else if (ft_strcmp(command, "crashtest") == 0)
		cmd_type = CMD_CRASHTEST;
    else if (ft_strcmp(command, "crash1") == 0)
		cmd_type = CMD_CRASH1;
    else if (ft_strcmp(command, "crash2") == 0)
		cmd_type = CMD_CRASH2;
    else if (ft_strcmp(command, "crash3") == 0)
		cmd_type = CMD_CRASH3;
    else if (ft_strcmp(command, "crash4") == 0)
		cmd_type = CMD_CRASH4;
    else if (ft_strcmp(command, "crash5") == 0)
		cmd_type = CMD_CRASH5;
    else if (ft_strcmp(command, "crash6") == 0)
		cmd_type = CMD_CRASH6;
    else if (ft_strcmp(command, "vmemstats") == 0)
		cmd_type = CMD_VMEMSTATS;
    else if (ft_strcmp(command, "memcheck") == 0)
		cmd_type = CMD_MEMCHECK;
	return cmd_type;
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

    switch (cmd_type) {
        case CMD_HELP:
            handle_help();
            break;
        case CMD_STACK:
            handle_stack();
            break;
        case CMD_PUSH:
            handle_push(arg);
            break;
        case CMD_POP:
            handle_pop();
            break;
        case CMD_CLEAR:
            handle_clear();
            break;
        case CMD_REBOOT:
            handle_reboot();
            break;
        case CMD_HALT:
            handle_halt();
            break;
        case CMD_SHUTDOWN:
            handle_shutdown();
            break;
        case CMD_MEMSTATS:
            handle_memstats();
            break;
        case CMD_MEMTEST:
            handle_memtest();
            break;
        case CMD_MEMTEST1:
            handle_memtest1();
            break;
        case CMD_MEMTEST2:
            handle_memtest2();
            break;
        case CMD_MEMTEST3:
            handle_memtest3();
            break;
        case CMD_MEMTEST4:
            handle_memtest4();
            break;
        case CMD_MEMTEST5:
            handle_memtest5();
            break;
        case CMD_CRASHTEST:
            handle_crashtest();
            break;
        case CMD_CRASH1:
            handle_crash1();
            break;
        case CMD_CRASH2:
            handle_crash2();
            break;
        case CMD_CRASH3:
            handle_crash3();
            break;
        case CMD_CRASH4:
            handle_crash4();
            break;
        case CMD_CRASH5:
            handle_crash5();
            break;
        case CMD_CRASH6:
            handle_crash6();
            break;
        case CMD_VMEMSTATS:
            handle_vmemstats();
            break;
        case CMD_MEMCHECK:
            handle_memcheck();
            break;
        case CMD_UNKNOWN:
            if (command[0] != '\0')
                handle_unknown(command);
            break;
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
