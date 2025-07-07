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

	//terminal_writestring("Entered IRQ_Initialize function\n");
	//terminal_writestring("Configuring PIC...\n");
	PIC_Configure(PIC1_OFFSET, PIC2_OFFSET);
	//terminal_writestring("PIC configured!\n");

	// Mask all IRQs initially to prevent spurious interrupts
	//terminal_writestring("Masking all IRQs...\n");
	for (int j = 0; j < 16; j++) {
		PIC_Mask(j);
	}
	//terminal_writestring("All IRQs masked!\n");

	// Register all IRQ handlers BEFORE enabling interrupts
	//terminal_writestring("Registering IRQ handlers...\n");
	i = 0;
	while (i < 16)
	{
		ISR_RegisterHandler(PIC1_OFFSET + i, IRQ_Handler);
		i++;
	}
	//terminal_writestring("IRQ handlers registered!\n");

	// Set up specific IRQ handlers
	//terminal_writestring("Setting up timer and keyboard handlers...\n");
	IRQ_RegisterHandler(0, timer);
	IRQ_RegisterHandler(1, keyboard_handler);
	//terminal_writestring("Specific handlers set up!\n");

	// DON'T enable interrupts yet - let the main kernel do it
	//terminal_writestring("IRQ setup complete, interrupts still disabled\n");

	// Now unmask specific IRQs we want to handle (but interrupts still disabled)
	//terminal_writestring("Unmasking timer and keyboard IRQs...\n");
	PIC_Unmask(0); // Timer
	PIC_Unmask(1); // Keyboard
	//terminal_writestring("IRQ initialization complete!\n");
}





void		IRQ_RegisterHandler(int irq, IRQHandler handler)
{
	kernel.IRQHandlers[irq] = handler;
}
