/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 21:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 19:30:37 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

#include "../core/enums.h"
#include "../core/libs.h"
#include "../core/structs.h"

/* ──────────── Functions from src/globals.c ──────────── */

void kernel_globals_init(void);

/* ──────────── Functions from src/kernel/init.c ──────────── */

void kfs2_init(void);
void kfs3_init(void);
void kernel_initialize(void);

/* ──────────── Functions from src/kernel/kernel.c ──────────── */

void kernel_initialize(void);
void terminal_initialize(void);
void terminal_offset(uint16_t offset);
void terminal_restore(void);

/* ──────────── Functions from src/kernel/shell.c ──────────── */

void shell_initialize(void);
void handle_help(void);
void handle_unknown(const char* command);
command_type_t get_general_command_type(const char* command);
bool handle_general_commands(command_type_t cmd_type, const char* arg);
command_type_t get_command_type(const char* command);
void shell_process_command(const char* cmd);
void shell_handle_input(char c);

#endif
