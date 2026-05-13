/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:44:42 by zerrino           #+#    #+#             */
/*   Updated: 2026/05/13 16:56:12 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

void	timer(t_registers* regs)
{
	(void)regs;
	static int timer_count = 0;
	timer_count++;
	if (kernel.signal_ptr > 0)
	{
		kernel.signal_ptr--;
		t_registers	regs_ptr;
		ft_memcpy(&regs_ptr, &kernel.signal_regs[kernel.signal_ptr], sizeof(t_registers));
		kernel.ISRSignalsQueue[kernel.signal_ptr](&regs_ptr);
	}
}
