/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kfs3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:20:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 16:36:14 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kfs.h"

void display_kfs3_help(void) {
    display_memory_management_help();
    display_kernel_testing_help();
}

command_type_t get_kfs3_command_type(const char* command) {
    command_type_t cmd_type;

    cmd_type = get_memory_management_command_type(command);
    if (cmd_type != CMD_UNKNOWN)
        return cmd_type;

    cmd_type = get_kernel_testing_command_type(command);
    if (cmd_type != CMD_UNKNOWN)
        return cmd_type;
	
    return CMD_UNKNOWN;
}

bool handle_kfs3_commands(command_type_t cmd_type, const char* arg) {
    if (handle_memory_management_commands(cmd_type, arg))
        return true;
	
    if (handle_kernel_testing_commands(cmd_type, arg))
        return true;
        
    return false;
}
