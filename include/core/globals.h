/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 19:29:15 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBALS_H
# define GLOBALS_H

#include "structs.h"

/* ──────────── Global Variables ──────────── */
/* Main Kernel Structure - contains all kernel state */
extern t_kernel kernel;

/* ──────────── Legacy Compatibility Macros ──────────── */
/* These macros provide backward compatibility for existing code */
#define gdt                     (kernel.gdt)
#define g_phys_mem_manager      (kernel.phys_mem_manager)
#define g_kernel_directory      (kernel.kernel_directory)
#define g_current_directory     (kernel.current_directory)
#define g_kernel_heap           (kernel.kernel_heap)

/* ──────────── Kernel Execution Stack (GDT-integrated) ──────────── */
extern uint8_t kernel_execution_stack[KERNEL_EXECUTION_STACK_SIZE];
extern uint32_t *kernel_stack_top;

#endif