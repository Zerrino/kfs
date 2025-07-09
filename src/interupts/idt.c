/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 22:24:08 by zerrino           #+#    #+#             */
/*   Updated: 2025/07/09 15:05:46 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

//	uint16_t offset_1;        // offset bits 0..15
//	uint16_t selector;        // a code segment selector in GDT or LDT
//	uint8_t  zero;            // unused, set to 0
//	uint8_t  type_attributes; // gate type, dpl, and p fields
//	uint16_t offset_2;        // offset bits 16..31

void	IDT_SetGate(int interrupt, void *base, uint16_t segmentDescriptor, uint8_t flags)
{
	kernel.idt[interrupt].base_low = (uint32_t)base & 0xFFFF;
	kernel.idt[interrupt].selector = segmentDescriptor;
	kernel.idt[interrupt].always0 = 0;
	kernel.idt[interrupt].flags = flags;
	kernel.idt[interrupt].base_high = ((uint32_t)base >> 16) & 0xFFFF;
}

void	IDT_EnableGate(int interrupt)
{
	FLAG_SET(kernel.idt[interrupt].flags, IDT_FLAG_PRESENT);
}

void	IDT_DisableGate(int interrupt)
{
	FLAG_UNSET(kernel.idt[interrupt].flags, IDT_FLAG_PRESENT);
}

void	IDT_Initialize()
{
	kernel.idt_descriptor.limit = sizeof(kernel.idt) - 1;
	kernel.idt_descriptor.base = (uint32_t)kernel.idt;

	IDT_Load(&kernel.idt_descriptor);
}
