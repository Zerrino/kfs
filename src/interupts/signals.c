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
