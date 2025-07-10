/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 21:58:14 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 16:01:11 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KERNEL_H
# define KERNEL_H

/* ──────────── Main Kernel Header ──────────── */
/* This header includes all necessary components for the kernel */

#include "core/libs.h"
#include "core/defines.h"
#include "core/enums.h"
#include "core/structs.h"
#include "core/globals.h"
#include "core/functions.h"

/* ──────────── Main Kernel Function ──────────── */
void kernel_main(void);

#endif
