/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerrino <zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 22:24:08 by zerrino           #+#    #+#             */
/*   Updated: 2025/06/25 03:12:03 by zerrino          ###   ########.fr       */
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
	kernel.idt[interrupt].offset_1 = (uint32_t)base & 0xFFFF;
	kernel.idt[interrupt].selector = segmentDescriptor;
	kernel.idt[interrupt].zero = 0;
	kernel.idt[interrupt].type_attributes = flags;
	kernel.idt[interrupt].offset_2 = ((uint32_t)base >> 16) & 0xFFFF;
}

void	IDT_EnableGate(int interrupt)
{
	FLAG_SET(kernel.idt[interrupt].type_attributes, IDT_FLAG_PRESENT);
}

void	IDT_DisableGate(int interrupt)
{
	FLAG_UNSET(kernel.idt[interrupt].type_attributes, IDT_FLAG_PRESENT);
}

void	IDT_Initialize()
{
	kernel.idt_descriptor.size = sizeof(kernel.idt) - 1;
	kernel.idt_descriptor.offset = (uint32_t)kernel.idt;

	IDT_Load(&kernel.idt_descriptor);
}
