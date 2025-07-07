/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pic.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerrino <zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 19:59:42 by zerrino           #+#    #+#             */
/*   Updated: 2025/06/25 21:34:01 by zerrino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

void	PIC_Configure(uint8_t offsetPic1, uint8_t offsetPic2)
{
	outb(PIC1_CMD, PIC_ICW1_ICW4 | PIC_ICW1_INIT);
	iowait();
	outb(PIC2_CMD, PIC_ICW1_ICW4 | PIC_ICW1_INIT);
	iowait();
	outb(PIC1_DATA, offsetPic1);
	iowait();
	outb(PIC2_DATA, offsetPic2);
	iowait();
	outb(PIC1_DATA, 0x4);
	iowait();
	outb(PIC2_DATA, 0x2);
	iowait();

	outb(PIC1_DATA, PIC_ICW4_8086);
	iowait();
	outb(PIC2_DATA, PIC_ICW4_8086);
	iowait();

	outb(PIC1_DATA, 0);
	iowait();
	outb(PIC2_DATA, 0);
	iowait();
}

void	PIC_SendEOF(int irq)
{
	if (irq >= 8)
		outb(PIC2_CMD, PIC_CMD_EOF);
	outb(PIC1_CMD, PIC_CMD_EOF);
}

void	PIC_Mask(int irq)
{
	uint8_t	mask;
	uint8_t	port;

	if (irq < 8)
		port = PIC1_DATA;
	else
	{
		irq -= 8;
		port = PIC2_DATA;
	}
	mask = inb(port);
	outb(port, mask | (1 << irq));
}

void	PIC_Unmask(int irq)
{
	uint8_t	mask;
	uint8_t	port;

	if (irq < 8)
		port = PIC1_DATA;
	else
	{
		irq -= 8;
		port = PIC2_DATA;
	}
	mask = inb(port);
	outb(port, mask & ~(1 << irq));
}

void	PIC_Disable()
{
	outb(PIC1_DATA, 0xFF);
	iowait();
	outb(PIC2_DATA, 0xFF);
	iowait();
}

uint16_t	PIC_ReadIRQRequestReg()
{
	outb(PIC1_CMD, PIC_CMD_READ_IRR);
	outb(PIC2_CMD, PIC_CMD_READ_IRR);
	return (inb(PIC1_CMD) | (inb(PIC2_CMD) << 8));
}

uint16_t	PIC_ReadInServiceReg()
{
	outb(PIC1_CMD, PIC_CMD_READ_ISR);
	outb(PIC2_CMD, PIC_CMD_READ_ISR);
	return (inb(PIC1_CMD) | (inb(PIC2_CMD) << 8));
}
