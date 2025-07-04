/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:11:05 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/04 15:11:08 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBALS_H
# define GLOBALS_H

#include "structs.h"

/* ──────────── Global Kernel Structure ──────────── */
extern t_kernel	kernel;

/* ──────────── Global Memory Management Variables ──────────── */
extern t_phys_mem_manager g_phys_mem_manager;
extern t_page_directory *g_kernel_directory;
extern t_page_directory *g_current_directory;
extern t_kernel_heap g_kernel_heap;

/* ──────────── Global Virtual Memory Variables ──────────── */
/* Note: vma_list is static in vmalloc.c */

/* ──────────── Global GDT Variables ──────────── */
extern t_gdt_entry *gdt;
extern t_gdt_ptr gdt_pointer;

#endif /* GLOBALS_H */
