/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/11 11:53:14 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GDT_H
# define GDT_H

#include "../core/libs.h"
#include "../core/structs.h"

/* ──────────── Functions from src/gdt/gdt.c ──────────── */

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
void gdt_install(void);
void print_gdt_info(void);

/* ──────────── Functions from src/gdt/stack.c ──────────── */

void stack_push(uint32_t value);
uint32_t stack_pop(void);
uint32_t stack_peek(void);
int stack_is_empty(void);
int stack_size(void);
void print_kernel_stack(void);
void print_kernel_execution_stack(void);
void switch_to_kernel_stack(void);

#endif