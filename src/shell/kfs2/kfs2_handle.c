/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kfs2_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:14:06 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/11 11:54:15 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kernel.h"

void display_core_system_help(void) {
    terminal_writestring("=== Core System ===\n");
    terminal_writestring("  clear        - Clear the screen\n");
    terminal_writestring("  reboot       - Reboot the system\n");
    terminal_writestring("  halt         - Halt the system\n");
    terminal_writestring("  shutdown     - Shutdown the system\n");
    terminal_writestring("  syscall      - Trigger a demo syscall\n");
}

void display_gdt_memory_help(void) {
    terminal_writestring("\n=== GDT & Memory ===\n");
    terminal_writestring("  gdt          - Display GDT information\n");
    terminal_writestring("  segments     - Show current segment registers\n");
    terminal_writestring("  memory       - Show memory layout and addresses\n");
    terminal_writestring("  gdttest      - Test GDT segment switching\n");
}

void display_stack_operations_help(void) {
    terminal_writestring("\n=== Stack Operations ===\n");
    terminal_writestring("  stack        - Print the kernel data stack\n");
    terminal_writestring("  userstack    - Print a stack that lives in the userspace\n");
    terminal_writestring("  push <hex>   - Push a value onto the user stack\n");
    terminal_writestring("  pop          - Pop a value from the pop stack\n");
    terminal_writestring("  stacktest    - Demonstrate user stack operations\n");
}

void display_system_info_help(void) {
    terminal_writestring("\n=== System Info ===\n");
    terminal_writestring("  sysinfo      - Show complete system information\n");
    terminal_writestring("  interrupts   - Show interrupt status\n");
}

command_type_t get_core_system_command_type(const char* command) {
    if (ft_strcmp(command, "clear") == 0)
        return CMD_CLEAR;
    else if (ft_strcmp(command, "reboot") == 0)
        return CMD_REBOOT;
    else if (ft_strcmp(command, "halt") == 0)
        return CMD_HALT;
    else if (ft_strcmp(command, "shutdown") == 0)
        return CMD_SHUTDOWN;
    else if (ft_strcmp(command, "syscall") == 0)
        return CMD_SYSCALL;

    return CMD_UNKNOWN;
}

command_type_t get_gdt_memory_command_type(const char* command) {
    if (ft_strcmp(command, "gdt") == 0)
        return CMD_GDT;
    else if (ft_strcmp(command, "segments") == 0)
        return CMD_SEGMENTS;
    else if (ft_strcmp(command, "memory") == 0)
        return CMD_MEMORY;
    else if (ft_strcmp(command, "gdttest") == 0)
        return CMD_GDTTEST;

    return CMD_UNKNOWN;
}

command_type_t get_stack_operations_command_type(const char* command) {
    if (ft_strcmp(command, "stack") == 0)
        return CMD_STACK;
    else if (ft_strcmp(command, "userstack") == 0)
        return CMD_USERSTACK;
    else if (ft_strcmp(command, "push") == 0)
        return CMD_PUSH;
    else if (ft_strcmp(command, "pop") == 0)
        return CMD_POP;
    else if (ft_strcmp(command, "stacktest") == 0)
        return CMD_STACKTEST;

    return CMD_UNKNOWN;
}

command_type_t get_system_info_command_type(const char* command) {
    if (ft_strcmp(command, "sysinfo") == 0)
        return CMD_SYSINFO;
    else if (ft_strcmp(command, "interrupts") == 0)
        return CMD_INTERRUPTS;

    return CMD_UNKNOWN;
}

bool handle_core_system_commands(command_type_t cmd_type, const char* arg) {
    switch (cmd_type) {
        case CMD_CLEAR:
            handle_clear();
            return true;
        case CMD_REBOOT:
            handle_reboot();
            return true;
        case CMD_HALT:
            handle_halt();
            return true;
        case CMD_SHUTDOWN:
            handle_shutdown();
            return true;
        case CMD_SYSCALL:
            handle_syscall(arg);
            return true;
        default:
            return false;
    }
    (void)arg; // Suppress unused parameter warning
}

bool handle_gdt_memory_commands(command_type_t cmd_type, const char* arg) {
    switch (cmd_type) {
        case CMD_GDT:
            handle_gdt();
            return true;
        case CMD_SEGMENTS:
            handle_segments();
            return true;
        case CMD_MEMORY:
            handle_memory();
            return true;
        case CMD_GDTTEST:
            handle_gdttest();
            return true;
        default:
            return false;
    }
    (void)arg; // Suppress unused parameter warning
}

bool handle_stack_operations_commands(command_type_t cmd_type, const char* arg) {
    switch (cmd_type) {
        case CMD_STACK:
            handle_stack();
            return true;
        case CMD_USERSTACK:
            handle_userstack();
            return true;
        case CMD_PUSH:
            handle_push(arg);
            return true;
        case CMD_POP:
            handle_pop();
            return true;
        case CMD_STACKTEST:
            handle_stacktest();
            return true;
        default:
            return false;
    }
}

bool handle_system_info_commands(command_type_t cmd_type, const char* arg) {
    switch (cmd_type) {
        case CMD_SYSINFO:
            handle_sysinfo();
            return true;
        case CMD_INTERRUPTS:
            handle_interrupts();
            return true;
        default:
            return false;
    }
    (void)arg; // Suppress unused parameter warning
}
