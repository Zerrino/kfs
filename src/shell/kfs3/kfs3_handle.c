/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kfs3_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:14:06 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 19:58:20 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kfs.h"

void handle_memtest() {
    terminal_writestring("\n=== Memory Test Suite ===\n");
    terminal_writestring("Run individual tests to debug issues:\n");
    terminal_writestring("  memtest1 - Basic allocation tests\n");
    terminal_writestring("  memtest2 - Multiple allocation tests\n");
    terminal_writestring("  memtest3 - Large allocation tests\n");
    terminal_writestring("  memtest4 - Virtual memory tests\n");
    terminal_writestring("  memtest5 - Edge case tests\n");
    terminal_writestring("\nOr run all tests with individual commands\n");
}
void display_memory_management_help(void) {
    terminal_writestring("\n=== KFS3 Memory Management Commands ===\n");
    terminal_writestring("  memstats     - Display memory statistics\n");
    terminal_writestring("  memtest      - Show memory test suite help\n");
    terminal_writestring("  vmemstats    - Virtual memory statistics\n");
    terminal_writestring("  memcheck     - Memory integrity check\n");
}

void display_kernel_testing_help(void) {
    terminal_writestring("\n=== KFS3 Kernel Testing Commands ===\n");
    terminal_writestring("  crashtest    - Show crash test suite help\n");
}

command_type_t get_memory_management_command_type(const char* command) {
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

command_type_t get_kernel_testing_command_type(const char* command) {
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

bool handle_memory_management_commands(command_type_t cmd_type, const char* arg) {
    (void)arg;
    switch (cmd_type) {
        case CMD_MEMSTATS:
            handle_memstats();
            return true;
        case CMD_MEMTEST:
            handle_memtest();
            return true;
        case CMD_MEMTEST1:
            handle_memtest1();
            return true;
        case CMD_MEMTEST2:
            handle_memtest2();
            return true;
        case CMD_MEMTEST3:
            handle_memtest3();
            return true;
        case CMD_MEMTEST4:
            handle_memtest4();
            return true;
        case CMD_MEMTEST5:
            handle_memtest5();
            return true;
        case CMD_VMEMSTATS:
            handle_vmemstats();
            return true;
        case CMD_MEMCHECK:
            handle_memcheck();
            return true;
        default:
            return false;
    }
}

bool handle_kernel_testing_commands(command_type_t cmd_type, const char* arg) {
    (void)arg;
    switch (cmd_type) {
        case CMD_CRASHTEST:
            handle_crashtest();
            return true;
        case CMD_CRASH1:
            handle_crash1();
            return true;
        case CMD_CRASH2:
            handle_crash2();
            return true;
        case CMD_CRASH3:
            handle_crash3();
            return true;
        case CMD_CRASH4:
            handle_crash4();
            return true;
        case CMD_CRASH5:
            handle_crash5();
            return true;
        case CMD_CRASH6:
            handle_crash6();
            return true;
        default:
            return false;
    }
}
