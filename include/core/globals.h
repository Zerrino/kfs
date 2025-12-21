/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/21 04:57:20 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBALS_H
# define GLOBALS_H

#include "structs.h"

/* ──────────── Global Variables ──────────── */
/* Kernel instance */
extern t_kernel kernel;

/* GDT instance */
extern t_gdt_entry *gdt;

/* ──────────── Kernel Execution Stack (GDT-integrated) ──────────── */
extern uint8_t kernel_execution_stack[KERNEL_EXECUTION_STACK_SIZE];
extern uint32_t *kernel_stack_top;

extern uint32_t g_memory_limit;

#endif
