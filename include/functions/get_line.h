/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_line.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 15:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/27 15:30:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_LINE_H
# define GET_LINE_H

#include "../core/libs.h"

/* Blocking read from keyboard until newline; returns number of chars stored (excluding terminator). */
size_t	kgetline(char *buffer, size_t max_len);

#endif
