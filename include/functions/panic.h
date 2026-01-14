/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 10:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/08/24 10:30:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PANIC_H
# define PANIC_H

#include "../core/libs.h"
#include "../core/structs.h"

void panic_save_stack(void);
void panic_dump_stack(void);
void panic_clean_registers(void);
void kernel_halt(void);
void kernel_panic(const char *message, t_registers *regs);

#endif
