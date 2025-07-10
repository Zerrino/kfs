/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:40:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 16:01:54 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kfs.h"

/* ──────────── Main Kernel Structure ──────────── */
t_kernel        kernel = {0};

/* ──────────── Memory Management Globals ──────────── */
t_kernel_heap   g_kernel_heap = {0};
t_page_directory *g_kernel_directory = 0;
t_page_directory *g_current_directory = 0;
t_phys_mem_manager g_phys_mem_manager = {0};

/* ──────────── GDT Globals ──────────── */
t_gdt_entry     *gdt = (t_gdt_entry *)0x00000800;
t_gdt_ptr       gdt_pointer;
