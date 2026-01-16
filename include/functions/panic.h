/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 13:10:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/24 13:10:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PANIC_H
# define PANIC_H

# include "../core/libs.h"

void	panic_save_stack(void);
void	panic_clean_registers(void);
void	kernel_panic(const char *message, const char *file, int line, int dump);
void	kernel_halt(const char *message);

# define KPANIC(msg) kernel_panic((msg), __FILE__, __LINE__, 0)
# define KPANIC_DUMP(msg) kernel_panic((msg), __FILE__, __LINE__, 1)

#endif
