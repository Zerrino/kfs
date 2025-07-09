/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:14 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/01 20:23:23 by rperez-t         ###   ########.fr       */
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
    /* Initialize GDT pointer using constants */
    gdt_pointer.limit = (sizeof(struct gdt_entry) * GDT_MAX_ENTRIES) - 1;
    gdt_pointer.base = GDT_BASE_ADDRESS;

    /* Set up GDT entries using descriptive enums and constants */
    gdt_set_gate(GDT_NULL_INDEX,         GDT_NULL_BASE,    GDT_NULL_LIMIT,    GDT_NULL_ACCESS,         GDT_NULL_GRANULARITY);  /* null descriptor */
    gdt_set_gate(GDT_KERNEL_CODE_INDEX,  GDT_SEGMENT_BASE, GDT_SEGMENT_LIMIT, GDT_ACCESS_KERNEL_CODE,  GDT_GRAN_STANDARD);     /* kernel code */
    gdt_set_gate(GDT_KERNEL_DATA_INDEX,  GDT_SEGMENT_BASE, GDT_SEGMENT_LIMIT, GDT_ACCESS_KERNEL_DATA,  GDT_GRAN_STANDARD);     /* kernel data */
    gdt_set_gate(GDT_KERNEL_STACK_INDEX, GDT_SEGMENT_BASE, GDT_SEGMENT_LIMIT, GDT_ACCESS_KERNEL_DATA,  GDT_GRAN_STANDARD);     /* kernel stack */
    gdt_set_gate(GDT_USER_CODE_INDEX,    GDT_SEGMENT_BASE, GDT_SEGMENT_LIMIT, GDT_ACCESS_USER_CODE,    GDT_GRAN_STANDARD);     /* user code */
    gdt_set_gate(GDT_USER_DATA_INDEX,    GDT_SEGMENT_BASE, GDT_SEGMENT_LIMIT, GDT_ACCESS_USER_DATA,    GDT_GRAN_STANDARD);     /* user data */
    gdt_set_gate(GDT_USER_STACK_INDEX,   GDT_SEGMENT_BASE, GDT_SEGMENT_LIMIT, GDT_ACCESS_USER_DATA,    GDT_GRAN_STANDARD);     /* user stack */

    gdt_flush(&gdt_pointer);
}

void print_gdt_info() {
    terminal_writestring("GDT Information:\n");
    terminal_writestring("  GDT Base Address: 0x");
    printnbr(gdt_pointer.base, 16);
    terminal_writestring("\n  GDT Limit: ");
    printnbr(gdt_pointer.limit, 10);
    terminal_writestring(" bytes\n");
    terminal_writestring("  Number of entries: ");
    printnbr((gdt_pointer.limit + 1) / sizeof(struct gdt_entry), 10);
    terminal_writestring("\n");

    const char* segment_names[] = {
        "Null Descriptor",
        "Kernel Code",
        "Kernel Data",
        "Kernel Stack",
        "User Code",
        "User Data",
        "User Stack"
    };

    uint16_t selectors[] = {
        GDT_NULL_SEGMENT, GDT_KERNEL_CODE, GDT_KERNEL_DATA,
        GDT_KERNEL_STACK, GDT_USER_CODE, GDT_USER_DATA, GDT_USER_STACK
    };

    for (int i = 0; i < 7; i++) {
        terminal_writestring("  [");
        printnbr(i, 10);
        terminal_writestring("] ");
        terminal_writestring(segment_names[i]);
        terminal_writestring(" (Selector: 0x");
        printnbr(selectors[i], 16);
        terminal_writestring(")\n");
        terminal_writestring("      Base: 0x");

        uint32_t base = gdt[i].base_low | (gdt[i].base_middle << 16) | (gdt[i].base_high << 24);
        printnbr(base, 16);

        terminal_writestring(", Limit: 0x");
        uint32_t limit = gdt[i].limit_low | ((gdt[i].granularity & 0x0F) << 16);
        printnbr(limit, 16);

        terminal_writestring(", Access: 0x");
        printnbr(gdt[i].access, 16);
        terminal_writestring("\n");
    }
}
