/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:20 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/05 16:04:53 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

command_type_t get_kfs2_command_type(const char* command) {
    if (ft_strcmp(command, "help") == 0)
        return CMD_HELP;
    else if (ft_strcmp(command, "stack") == 0)
        return CMD_STACK;
    else if (ft_strcmp(command, "push") == 0)
        return CMD_PUSH;
    else if (ft_strcmp(command, "pop") == 0)
        return CMD_POP;
    else if (ft_strcmp(command, "clear") == 0)
        return CMD_CLEAR;
    else if (ft_strcmp(command, "reboot") == 0)
        return CMD_REBOOT;
    else if (ft_strcmp(command, "halt") == 0)
        return CMD_HALT;
    else if (ft_strcmp(command, "shutdown") == 0)
        return CMD_SHUTDOWN;
    return CMD_UNKNOWN;
}

command_type_t get_kfs3_memory_command_type(const char* command) {
    if (ft_strcmp(command, "memstats") == 0)
        return CMD_MEMSTATS;
    else if (ft_strcmp(command, "memtest") == 0)
        return CMD_MEMTEST;
    else if (ft_strcmp(command, "memtest1") == 0)
        return CMD_MEMTEST1;
    else if (ft_strcmp(command, "memtest2") == 0)
        return CMD_MEMTEST2;
    else if (ft_strcmp(command, "memtest3") == 0)
        return CMD_MEMTEST3;
    else if (ft_strcmp(command, "memtest4") == 0)
        return CMD_MEMTEST4;
    else if (ft_strcmp(command, "memtest5") == 0)
        return CMD_MEMTEST5;
    else if (ft_strcmp(command, "vmemstats") == 0)
        return CMD_VMEMSTATS;
    else if (ft_strcmp(command, "memcheck") == 0)
        return CMD_MEMCHECK;
    return CMD_UNKNOWN;
}

command_type_t get_kfs3_crash_command_type(const char* command) {
    if (ft_strcmp(command, "crashtest") == 0)
        return CMD_CRASHTEST;
    else if (ft_strcmp(command, "crash1") == 0)
        return CMD_CRASH1;
    else if (ft_strcmp(command, "crash2") == 0)
        return CMD_CRASH2;
    else if (ft_strcmp(command, "crash3") == 0)
        return CMD_CRASH3;
    else if (ft_strcmp(command, "crash4") == 0)
        return CMD_CRASH4;
    else if (ft_strcmp(command, "crash5") == 0)
        return CMD_CRASH5;
    else if (ft_strcmp(command, "crash6") == 0)
        return CMD_CRASH6;
    return CMD_UNKNOWN;
}


int handle_kfs2_commands(command_type_t cmd_type, const char* arg) {
    switch (cmd_type) {
        case CMD_HELP:
            handle_help();
            return 1;
        case CMD_STACK:
            handle_stack();
            return 1;
        case CMD_PUSH:
            handle_push(arg);
            return 1;
        case CMD_POP:
            handle_pop();
            return 1;
        case CMD_CLEAR:
            handle_clear();
            return 1;
        case CMD_REBOOT:
            handle_reboot();
            return 1;
        case CMD_HALT:
            handle_halt();
            return 1;
        case CMD_SHUTDOWN:
            handle_shutdown();
            return 1;
        default:
            return 0;
    }
}

int handle_kfs3_memory_commands(command_type_t cmd_type, const char* arg) {
    (void)arg;
    switch (cmd_type) {
        case CMD_MEMSTATS:
            handle_memstats();
            return 1;
        case CMD_MEMTEST:
            handle_memtest();
            return 1;
        case CMD_MEMTEST1:
            handle_memtest1();
            return 1;
        case CMD_MEMTEST2:
            handle_memtest2();
            return 1;
        case CMD_MEMTEST3:
            handle_memtest3();
            return 1;
        case CMD_MEMTEST4:
            handle_memtest4();
            return 1;
        case CMD_MEMTEST5:
            handle_memtest5();
            return 1;
        case CMD_VMEMSTATS:
            handle_vmemstats();
            return 1;
        case CMD_MEMCHECK:
            handle_memcheck();
            return 1;
        default:
            return 0;
    }
}

int handle_kfs3_crash_commands(command_type_t cmd_type, const char* arg) {
    (void)arg;
    switch (cmd_type) {
        case CMD_CRASHTEST:
            handle_crashtest();
            return 1;
        case CMD_CRASH1:
            handle_crash1();
            return 1;
        case CMD_CRASH2:
            handle_crash2();
            return 1;
        case CMD_CRASH3:
            handle_crash3();
            return 1;
        case CMD_CRASH4:
            handle_crash4();
            return 1;
        case CMD_CRASH5:
            handle_crash5();
            return 1;
        case CMD_CRASH6:
            handle_crash6();
            return 1;
        default:
            return 0;
    }
}
