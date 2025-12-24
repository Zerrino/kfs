/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irq.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:34:40 by zerrino           #+#    #+#             */
/*   Updated: 2025/08/02 16:00:22 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"


void	IRQ_Handler(t_registers* regs)
{
	int	irq;

	irq = regs->interrupt - PIC1_OFFSET;
	if (kernel.IRQHandlers[irq] != NULL)
		kernel.IRQHandlers[irq](regs);
	else
	{
		terminal_writestring("Unhandled IRQ : ");
		printnbr(irq, 10);
		terminal_writestring("\n");
	}

	PIC_SendEOF(irq);
	SIG_ProcessPending();
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
