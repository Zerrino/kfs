/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irq.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerrino <zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:34:40 by zerrino           #+#    #+#             */
/*   Updated: 2025/06/27 00:38:45 by zerrino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"


void	IRQ_Handler(t_registers* regs)
{
	int	irq;

	irq = regs->interrupt - PIC1_OFFSET;
	if (kernel.IRQHandlers[irq] != NULL)
	{
		kernel.IRQHandlers[irq](regs);
	}
	else
	{
		terminal_writestring("Unhandled IRQ : ");
		printnbr(irq, 10);
		terminal_writestring("\n");
	}
	PIC_SendEOF(irq);
}

void		IRQ_Initialize()
{
	int	i;

	PIC_Configure(PIC1_OFFSET, PIC2_OFFSET);
	i = 0;
	while (i < 16)
	{
		ISR_RegisterHandler(PIC1_OFFSET + i, IRQ_Handler);
		i++;
	}

	EnableInterrupts();

	IRQ_RegisterHandler(0, timer);
	IRQ_RegisterHandler(1, keyboard_handler);
}





void		IRQ_RegisterHandler(int irq, IRQHandler handler)
{
	kernel.IRQHandlers[irq] = handler;
}
