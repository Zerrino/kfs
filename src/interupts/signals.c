/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 01:32:05 by alexafer          #+#    #+#             */
/*   Updated: 2026/05/14 01:37:08 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

void	init_signals()
{
	int	i;

	i = 0;
	while (i < SIGNAL_QUEUE_SIZE - 1)
	{
		kernel.signalQueue[i].next = &kernel.signalQueue[i + 1];
		i++;
	}
	kernel.signalQueue[i].next = &kernel.signalQueue[0];
	kernel.signalBegin = &kernel.signalQueue[0];
	kernel.signalEnd = &kernel.signalQueue[0];
	kernel.signalSize = 0;
}

int	signal_schedule(ISRHandler handler, t_registers *regs)
{
	if (handler == NULL || regs == NULL)
		return (-1);
	if (kernel.signalSize >= SIGNAL_QUEUE_SIZE)
		return (-1);
	ft_memcpy(&kernel.signalEnd->regs, regs, sizeof(t_registers));
	kernel.signalEnd->ISRsignals = handler;
	kernel.signalEnd = kernel.signalEnd->next;
	kernel.signalSize++;
	return (0);
}

int	signal_dispatch_next(void)
{
	t_signal	signal;

	if (kernel.signalSize <= 0)
		return (0);
	ft_memcpy(&signal, kernel.signalBegin, sizeof(t_signal));
	kernel.signalBegin = kernel.signalBegin->next;
	kernel.signalSize--;
	if (signal.ISRsignals == NULL)
		return (-1);
	signal.ISRsignals(&signal.regs);
	return (1);
}
