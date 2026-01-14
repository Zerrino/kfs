/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:44:42 by zerrino           #+#    #+#             */
/*   Updated: 2025/07/09 12:12:19 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

void	timer(t_registers* regs)
{
	(void)regs;
	static int timer_count = 0;
	timer_count++;
}

void	signal_timer_handler(uint8_t signal, const t_registers *regs)
{
	(void)regs;
	kernel.signal_counts[signal]++;
	kernel.signal_last = signal;
}

void	signal_keyboard_handler(uint8_t signal, const t_registers *regs)
{
	(void)regs;
	kernel.signal_counts[signal]++;
	kernel.signal_last = signal;
}

void	signal_syscall_handler(uint8_t signal, const t_registers *regs)
{
	(void)regs;
	kernel.signal_counts[signal]++;
	kernel.signal_last = signal;
}
