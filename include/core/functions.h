/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:33:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/04 15:34:37 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUNCTIONS_H
# define FUNCTIONS_H

/* ──────────── Master Functions Include ──────────── */
/* Import all function declarations by module */

#include "../functions/gdt.h"          /* GDT and stack functions */
#include "../functions/interupts.h"    /* Interrupt management functions */
#include "../functions/memory.h"       /* Memory management functions */
#include "../functions/shell.h"        /* Shell and terminal functions */
#include "../functions/kernel.h"       /* Kernel core functions */

#endif /* FUNCTIONS_H */
