/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/09 16:05:43 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GDT_H
# define GDT_H

#include "../core/libs.h"
#include "../core/structs.h"

/* ──────────── GDT Selector Definitions ──────────── */
#define GDT_NULL_SEGMENT    0x00
#define GDT_KERNEL_CODE     0x08
#define GDT_KERNEL_DATA     0x10
#define GDT_KERNEL_STACK    0x18
#define GDT_USER_CODE       0x20
#define GDT_USER_DATA       0x28
#define GDT_USER_STACK      0x30

/* ──────────── GDT Function Prototypes ──────────── */
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
void gdt_install(void);
void print_gdt_info(void);

#endif