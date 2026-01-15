/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 13:10:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/24 13:10:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

static int	interrupts_enabled(void)
{
	uint32_t eflags;

	__asm__ volatile ("pushf; pop %0" : "=r"(eflags));
	return (eflags & EFLAGS_INTERRUPT_FLAG) != 0;
}

void	signal_init(void)
{
	for (int i = 0; i < SIGNAL_MAX; i++)
	{
		kernel.signal_handlers[i] = NULL;
		kernel.signal_counts[i] = 0;
	}
	kernel.signal_queue.head = 0;
	kernel.signal_queue.tail = 0;
	kernel.signal_queue.count = 0;
	kernel.signal_last = 0;
}

void	signal_register(uint16_t signal, SignalHandler handler)
{
	if (signal < SIGNAL_MAX)
		kernel.signal_handlers[signal] = handler;
}

int	signal_schedule(uint16_t signal, const t_registers *regs)
{
	t_signal_event *event;
	int				restore_interrupts;

	if (signal >= SIGNAL_MAX)
		return -1;
	restore_interrupts = interrupts_enabled();
	DisableInterrupts();
	if (kernel.signal_queue.count >= SIGNAL_QUEUE_SIZE)
	{
		if (restore_interrupts)
			EnableInterrupts();
		return -1;
	}
	event = &kernel.signal_queue.events[kernel.signal_queue.tail];
	event->signal = (uint8_t)signal;
	event->has_regs = (regs != NULL);
	if (regs != NULL)
		event->regs = *regs;
	kernel.signal_queue.tail = (kernel.signal_queue.tail + 1) % SIGNAL_QUEUE_SIZE;
	kernel.signal_queue.count++;
	if (restore_interrupts)
		EnableInterrupts();
	return 0;
}

static int	signal_pop(t_signal_event *out_event)
{
	int	restore_interrupts;

	if (out_event == NULL)
		return 0;
	restore_interrupts = interrupts_enabled();
	DisableInterrupts();
	if (kernel.signal_queue.count == 0)
	{
		if (restore_interrupts)
			EnableInterrupts();
		return 0;
	}
	*out_event = kernel.signal_queue.events[kernel.signal_queue.head];
	kernel.signal_queue.head = (kernel.signal_queue.head + 1) % SIGNAL_QUEUE_SIZE;
	kernel.signal_queue.count--;
	if (restore_interrupts)
		EnableInterrupts();
	return 1;
}

void	signal_dispatch(void)
{
	t_signal_event event;
	SignalHandler	handler;

	while (signal_pop(&event))
	{
		handler = kernel.signal_handlers[event.signal];
		if (handler != NULL)
		{
			if (event.has_regs)
				handler(event.signal, &event.regs);
			else
				handler(event.signal, NULL);
		}
	}
}
