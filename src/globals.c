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

/* ──────────── Main Kernel Structure ──────────── */
t_kernel kernel = {0};

/* ──────────── Kernel Initialization Function ──────────── */
void kernel_globals_init(void) {
    #undef gdt /* Temporarily undefine the macro to access the field directly */
    kernel.gdt = (t_gdt_entry *)GDT_BASE_ADDRESS; /* Initialize GDT pointer to the fixed memory location */
    #define gdt (kernel.gdt) /* Redefine the macro for compatibility */
	kernel.stack_pointer = -1; /* Initialize stack pointer to -1 to indicate it's not in use */
}
