/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:14 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 19:52:31 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kfs.h"

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;
    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[num].access      = access;
}

void gdt_install() {
    gdt_set_gate(GDT_NULL_INDEX,         GDT_NULL_BASE,    GDT_NULL_LIMIT,    GDT_NULL_ACCESS,         GDT_NULL_GRANULARITY);  /* null descriptor */
    gdt_set_gate(GDT_KERNEL_CODE_INDEX,  GDT_SEGMENT_BASE, GDT_SEGMENT_LIMIT, GDT_ACCESS_KERNEL_CODE,  GDT_GRAN_STANDARD);     /* kernel code */
    gdt_set_gate(GDT_KERNEL_DATA_INDEX,  GDT_SEGMENT_BASE, GDT_SEGMENT_LIMIT, GDT_ACCESS_KERNEL_DATA,  GDT_GRAN_STANDARD);     /* kernel data */
    gdt_set_gate(GDT_KERNEL_STACK_INDEX, GDT_SEGMENT_BASE, GDT_SEGMENT_LIMIT, GDT_ACCESS_KERNEL_DATA,  GDT_GRAN_STANDARD);     /* kernel stack segment (flat model) */
    gdt_set_gate(GDT_USER_CODE_INDEX,    GDT_SEGMENT_BASE, GDT_SEGMENT_LIMIT, GDT_ACCESS_USER_CODE,    GDT_GRAN_STANDARD);     /* user code */
    gdt_set_gate(GDT_USER_DATA_INDEX,    GDT_SEGMENT_BASE, GDT_SEGMENT_LIMIT, GDT_ACCESS_USER_DATA,    GDT_GRAN_STANDARD);     /* user data */
    gdt_set_gate(GDT_USER_STACK_INDEX,   GDT_SEGMENT_BASE, GDT_SEGMENT_LIMIT, GDT_ACCESS_USER_DATA,    GDT_GRAN_STANDARD);     /* user stack */
    kernel.gdt_pointer.limit = GDT_CALCULATE_LIMIT(GDT_MAX_ENTRIES);  /* GDT limit = (size * entries) - 1 */
    kernel.gdt_pointer.base = (uint32_t)gdt;                    /* GDT base address in memory */
    gdt_flush(&kernel.gdt_pointer);     /* Load the GDT into the processor */
}

void print_gdt_info() {
    static const t_gdt_segment_info segment_info[GDT_MAX_ENTRIES] = GDT_SEGMENT_INFO_INIT;

    terminal_writestring("GDT Information:\n");
    terminal_writestring("  GDT Base Address: 0x");
    printnbr(kernel.gdt_pointer.base, 16);
    terminal_writestring("\n  GDT Limit: ");
    printnbr(kernel.gdt_pointer.limit, 10);
    terminal_writestring(" bytes\n");
    terminal_writestring("  Number of entries: ");
    printnbr((kernel.gdt_pointer.limit + 1) / sizeof(t_gdt_entry), 10);
    terminal_writestring("\n");
    for (int i = 0; i < 7; i++) {
        terminal_writestring("  [");
        printnbr(i, 10);
        terminal_writestring("] ");
        terminal_writestring(segment_info[i].name);
        terminal_writestring(" (Selector: 0x");
        printnbr(segment_info[i].selector, 16);
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
