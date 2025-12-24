/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 14:40:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/27 14:40:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

static void	signal_demo_handler(t_registers *regs, int signal)
{
	terminal_writestring("[signal] handled id ");
	printnbr((uint32_t)signal, 10);
	terminal_writestring(" eip=0x");
	printnbr(regs->eip, 16);
	terminal_writestring("\n");
}

static size_t	signal_queue_count(void)
{
	if (kernel.signal_queue_full)
		return (MAX_SIGNAL_QUEUE);
	if (kernel.signal_queue_tail >= kernel.signal_queue_head)
		return (kernel.signal_queue_tail - kernel.signal_queue_head);
	return (MAX_SIGNAL_QUEUE - (kernel.signal_queue_head - kernel.signal_queue_tail));
}

void	SIG_Init(void)
{
	for (int i = 0; i < MAX_SIGNALS; i++)
		kernel.signal_handlers[i] = NULL;
	ft_memset(kernel.signal_queue, 0, sizeof(kernel.signal_queue));
	kernel.signal_queue_head = 0;
	kernel.signal_queue_tail = 0;
	kernel.signal_queue_full = false;
	kernel.processing_signals = false;
	kernel.last_signal_id = -1;

	/* Demo handler to showcase the callback system */
	SIG_RegisterHandler(SIGNAL_USER_TEST, signal_demo_handler);
}

int	SIG_RegisterHandler(int signal, SignalHandler handler)
{
	if (signal < 0 || MAX_SIGNALS <= signal)
		return (-1);
	kernel.signal_handlers[signal] = handler;
	return (0);
}

static bool	queue_push(const t_signal_event *evt)
{
	if (kernel.signal_queue_full)
		return (false);
	kernel.signal_queue[kernel.signal_queue_tail] = *evt;
	kernel.signal_queue_tail = (kernel.signal_queue_tail + 1) % MAX_SIGNAL_QUEUE;
	if (kernel.signal_queue_tail == kernel.signal_queue_head)
		kernel.signal_queue_full = true;
	return (true);
}

bool	SIG_Schedule(int signal, t_registers *regs)
{
	t_signal_event evt;

	if (signal < 0 || MAX_SIGNALS <= signal)
		return (false);
	evt.id = signal;
	if (regs != NULL)
		evt.context = *regs;
	else
		ft_memset(&evt.context, 0, sizeof(evt.context));
	return (queue_push(&evt));
}

void	SIG_ProcessPending(void)
{
	if (kernel.processing_signals)
		return;
	kernel.processing_signals = true;
	while (kernel.signal_queue_head != kernel.signal_queue_tail || kernel.signal_queue_full)
	{
		t_signal_event evt;
		SignalHandler	handler;

		evt = kernel.signal_queue[kernel.signal_queue_head];
		kernel.signal_queue_head = (kernel.signal_queue_head + 1) % MAX_SIGNAL_QUEUE;
		kernel.signal_queue_full = false;

		handler = kernel.signal_handlers[evt.id];
		kernel.last_signal_id = evt.id;
		if (handler)
			handler(&evt.context, evt.id);
		else
		{
			terminal_writestring("Unhandled signal : ");
			printnbr((uint32_t)evt.id, 10);
			terminal_writestring("\n");
		}
	}
	kernel.processing_signals = false;
}

size_t	SIG_PendingCount(void)
{
	return (signal_queue_count());
}
