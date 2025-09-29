/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:40:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 19:53:49 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kfs.h"

/* Static storage for GDT entries */
static t_gdt_entry gdt_entries[GDT_MAX_ENTRIES] __attribute__((aligned(8)));

/* ──────────── Main Kernel Structure ──────────── */
#undef gdt
t_kernel kernel = {
    .gdt = gdt_entries,
    .gdt_pointer = {
        .limit = (uint16_t)(sizeof(t_gdt_entry) * GDT_MAX_ENTRIES - GDT_LIMIT_OFFSET),
        .base = (uint32_t)gdt_entries,
    },
};
#define gdt (kernel.gdt)

/* ──────────── Kernel Initialization Function ──────────── */
void kernel_globals_init(void) {
    #undef gdt /* Temporarily undefine the macro to access the field directly */
    kernel.gdt = gdt_entries; /* Point to static GDT storage */
    kernel.gdt_pointer.base = (uint32_t)gdt_entries;
    kernel.gdt_pointer.limit = GDT_CALCULATE_LIMIT(GDT_MAX_ENTRIES);
    #define gdt (kernel.gdt) /* Redefine the macro for compatibility */
    kernel.stack_pointer = -1; /* Initialize stack pointer to -1 to indicate it's not in use */
}
