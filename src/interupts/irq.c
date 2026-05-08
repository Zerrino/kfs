/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irq.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reborn <reborn@42belgium.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:34:40 by zerrino           #+#    #+#             */
/*   Updated: 2026/05/08 10:46:47 by reborn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"


void	IRQ_Handler(t_registers* regs)
{
	DisableInterrupts();
	int	irq;

	irq = regs->interrupt - PIC1_OFFSET;
	if (kernel.IRQHandlers[irq] != NULL)
	{

		if (irq == 1)
		{
			regs->edi = inb(KEYBOARD_DATA_PORT);
		}

		if (kernel.signal_ptr < SIGNAL_QUEUE_SIZE)
		{
			kernel.signal_queue[kernel.signal_ptr] = kernel.IRQHandlers[irq];
			ft_memcpy(&kernel.signal_regs[kernel.signal_ptr], regs, sizeof(t_registers));
			kernel.signal_ptr++;
		}
	}
	else
	{
		terminal_writestring("Unhandled IRQ : ");
		printnbr(irq, 10);
		terminal_writestring("\n");
	}

	PIC_SendEOF(irq);
	EnableInterrupts();
}

void	IRQ_Initialize()
{

	PIC_Configure(PIC1_OFFSET, PIC2_OFFSET);
	for (int j = 0; j < 16; j++)
		PIC_Mask(j);

	for (size_t i = 0; i < 16; i++)
		ISR_RegisterHandler(PIC1_OFFSET + i, IRQ_Handler);

	IRQ_RegisterHandler(0, timer);
	IRQ_RegisterHandler(1, keyboard_handler);
	PIC_Unmask(0); // Timer
	PIC_Unmask(1); // Keyboard
}

void		IRQ_RegisterHandler(int irq, IRQHandler handler)
{
	kernel.IRQHandlers[irq] = handler;
}
