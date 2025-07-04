/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:12:16 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/04 15:35:42 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GDT_FUNCTIONS_H
# define GDT_FUNCTIONS_H

#include "../core/structs.h"

/* ──────────── GDT Functions ──────────── */
void		gdt_install();
void		gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
void		gdt_flush(void *);

/* ──────────── Stack Functions ──────────── */
void		stack_push(uint32_t value);
uint32_t	stack_pop();
uint32_t	stack_peek();
int			stack_is_empty();
void		print_kernel_stack();

#endif /* GDT_FUNCTIONS_H */
