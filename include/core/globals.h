/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 12:02:34 by rperez-t         ###   ########.fr       */
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

/* ──────────── Global Memory Management Variables ──────────── */
extern t_phys_mem_manager g_phys_mem_manager;
extern t_page_directory *g_kernel_directory;
extern t_page_directory *g_current_directory;
extern t_kernel_heap g_kernel_heap;

#endif