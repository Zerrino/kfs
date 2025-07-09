/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/09 16:36:12 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBS_H
# define LIBS_H

/* This tutorial will only work for the 32-bit ix86 targets. */
# if !defined(__i386__)
	#error "This tutorial needs to be compiled with a ix86-elf compiler"
	# endif
	
#include <stdbool.h>

/* Kernel-compatible type definitions */
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;
typedef uint32_t           size_t;
typedef int32_t            ssize_t;

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define INT_MIN (-2147483648)
#define INT_MAX (2147483647)

#endif