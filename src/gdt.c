/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:14 by rperez-t          #+#    #+#             */
/*   Updated: 2025/06/17 21:58:15 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct gdt_entry *gdt = (struct gdt_entry *)0x00000800;
struct gdt_ptr   gdt_pointer;

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);

    gdt[num].access      = access;
}

void gdt_install() {
    gdt_pointer.limit = (sizeof(struct gdt_entry) * 6) - 1;
    gdt_pointer.base = 0x00000800;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xC0); /* kernel code */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xC0); /* kernel data */
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0x92, 0xC0); /* kernel stack */
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xFA, 0xC0); /* user code */
    gdt_set_gate(5, 0, 0xFFFFFFFF, 0xF2, 0xC0); /* user data */

    gdt_flush(&gdt_pointer);
}
