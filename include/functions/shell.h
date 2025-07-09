/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/09 16:40:14 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

#include "../core/enums.h"
#include "../core/libs.h"
#include "../core/structs.h"

/* ──────────── Shell Function Prototypes ──────────── */

/**
 * @brief Handle input character from keyboard
 * @param c Character received from keyboard
 */
void shell_handle_input(char c);

/**
 * @brief Process a complete command string
 * @param command Command string to execute
 */
void shell_process_command(const char *command);

/* ──────────── Shell Command Handler Functions ──────────── */
/* These are the actual functions implemented in shell.c */

/**
 * @brief Display help information
 */
void handle_help(void);

/**
 * @brief Clear the screen
 */
void handle_clear(void);

/**
 * @brief Display system information
 */
void handle_sysinfo(void);

/**
 * @brief Display memory information
 */
void handle_memory(void);

/**
 * @brief Reboot the system
 */
void handle_reboot(void);

/**
 * @brief Halt the system
 */
void handle_halt(void);

/**
 * @brief Shutdown the system
 */
void handle_shutdown(void);

/**
 * @brief Display stack contents
 */
void handle_stack(void);

/**
 * @brief Display GDT information
 */
void handle_gdt(void);

/**
 * @brief Display segment information
 */
void handle_segments(void);

/**
 * @brief Test GDT operations
 */
void handle_gdttest(void);

/**
 * @brief Test stack operations
 */
void handle_stacktest(void);

/**
 * @brief Display interrupt status
 */
void handle_interrupts(void);

/**
 * @brief Push value onto stack
 * @param arg String representation of value to push
 */
void handle_push(const char *arg);

/**
 * @brief Pop value from stack
 */
void handle_pop(void);

/**
 * @brief Handle unknown command
 * @param command Unknown command string
 */
void handle_unknown(const char *command);

/* ──────────── Utility Functions ──────────── */

/**
 * @brief Get command type from command string
 * @param command Command string
 * @return Command type enumeration
 */
command_type_t get_command_type(const char *command);

#endif