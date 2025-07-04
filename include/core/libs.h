/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:10:10 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/04 15:10:28 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBS_H
# define LIBS_H

/* ──────────── Standard C Libraries ──────────── */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

/* ──────────── Cross-compiler Validation ──────────── */
# if defined(__linux__)
	#error "You are not using a cross-compiler, you will most certainly run into trouble"
# endif

/* This tutorial will only work for the 32-bit ix86 targets. */
# if !defined(__i386__)
	#error "This tutorial needs to be compiled with a ix86-elf compiler"
# endif

#endif /* LIBS_H */
