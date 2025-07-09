/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/09 16:00:23 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOBALS_H
# define GLOBALS_H

#include "structs.h"

/* ──────────── Global Variables ──────────── */
/* Kernel instance */
extern t_kernel kernel;

/* GDT instance */
extern t_gdt_entry *gdt;

#endif