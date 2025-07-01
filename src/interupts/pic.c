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
	i686_outb(PIC1_CMD, PIC_ICW1_ICW4 | PIC_ICW1_INIT);
	iowait();
	i686_outb(PIC2_CMD, PIC_ICW1_ICW4 | PIC_ICW1_INIT);
	iowait();
	i686_outb(PIC1_DATA, offsetPic1);
	iowait();
	i686_outb(PIC2_DATA, offsetPic2);
	iowait();
	i686_outb(PIC1_DATA, 0x4);
	iowait();
	i686_outb(PIC2_DATA, 0x2);
	iowait();

	i686_outb(PIC1_DATA, PIC_ICW4_8086);
	iowait();
	i686_outb(PIC2_DATA, PIC_ICW4_8086);
	iowait();

	i686_outb(PIC1_DATA, 0);
	iowait();
	i686_outb(PIC2_DATA, 0);
	iowait();
}

void	PIC_SendEOF(int irq)
{
	if (irq >= 8)
		i686_outb(PIC2_CMD, PIC_CMD_EOF);
	i686_outb(PIC1_CMD, PIC_CMD_EOF);
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
	mask = i686_inb(port);
	i686_outb(port, mask | (1 << irq));
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
	mask = i686_inb(port);
	i686_outb(port, mask & ~(1 << irq));
}

void	PIC_Disable()
{
	i686_outb(PIC1_DATA, 0xFF);
	iowait();
	i686_outb(PIC2_DATA, 0xFF);
	iowait();
}

uint16_t	PIC_ReadIRQRequestReg()
{
	i686_outb(PIC1_CMD, PIC_CMD_READ_IRR);
	i686_outb(PIC2_CMD, PIC_CMD_READ_IRR);
	return (i686_inb(PIC1_CMD) | (i686_inb(PIC2_CMD) << 8));
}

uint16_t	PIC_ReadInServiceReg()
{
	i686_outb(PIC1_CMD, PIC_CMD_READ_ISR);
	i686_outb(PIC2_CMD, PIC_CMD_READ_ISR);
	return (i686_inb(PIC1_CMD) | (i686_inb(PIC2_CMD) << 8));
}
