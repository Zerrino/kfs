/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kfs2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 21:19:52 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 16:01:54 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kfs.h"

void display_kfs2_help(void) {
    display_core_system_help();
    display_gdt_memory_help();
    display_stack_operations_help();
    display_system_info_help();
}

command_type_t get_kfs2_command_type(const char* command) {
    command_type_t cmd_type;

    cmd_type = get_core_system_command_type(command);
    if (cmd_type != CMD_UNKNOWN)
        return cmd_type;

    cmd_type = get_gdt_memory_command_type(command);
    if (cmd_type != CMD_UNKNOWN)
        return cmd_type;

    cmd_type = get_stack_operations_command_type(command);
    if (cmd_type != CMD_UNKNOWN)
        return cmd_type;

    cmd_type = get_system_info_command_type(command);
    if (cmd_type != CMD_UNKNOWN)
        return cmd_type;

    return CMD_UNKNOWN;
}

bool handle_kfs2_commands(command_type_t cmd_type, const char* arg) {
    if (handle_core_system_commands(cmd_type, arg))
        return true;

    if (handle_gdt_memory_commands(cmd_type, arg))
        return true;

    if (handle_stack_operations_commands(cmd_type, arg))
        return true;

    if (handle_system_info_commands(cmd_type, arg))
        return true;
        
    return false;
}
