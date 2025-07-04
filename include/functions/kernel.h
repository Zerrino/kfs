/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:11:54 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/04 15:29:52 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KERNEL_FUNCTIONS_H
# define KERNEL_FUNCTIONS_H

#include "../core/structs.h"

/* ──────────── Kernel Core Functions ──────────── */
void		kernelPanic();

/* ──────────── Additional Utility Functions ──────────── */
void		crash_me();
size_t		strlen(const char* str);

/* ──────────── Memory Utility Functions (C implementations) ──────────── */
void		*ft_memset(void *s, int c, size_t n);
void		ft_bzero(void *s, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		*ft_memmove(void *dst, const void *src, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);

#endif /* KERNEL_FUNCTIONS_H */
