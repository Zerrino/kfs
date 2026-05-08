/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reborn <reborn@42belgium.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:44:42 by zerrino           #+#    #+#             */
/*   Updated: 2026/05/08 10:40:58 by reborn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

void	timer(t_registers* regs)
{
	(void)regs;
	static int timer_count = 0;
	timer_count++;
}

void	execute_signal()
{
	t_registers	regs;

	if (kernel.signal_ptr > 0)
	{
		DisableInterrupts();
		kernel.signal_ptr--;
		ft_memcpy(&regs, &kernel.signal_regs[kernel.signal_ptr], sizeof(t_registers));
		kernel.signal_queue[kernel.signal_ptr](&regs);
		EnableInterrupts();
	}
}