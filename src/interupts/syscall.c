/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 16:06:12 by alexafer          #+#    #+#             */
/*   Updated: 2026/05/13 16:16:26 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

uint32_t SYS_write(t_registers *regs)
{
	uint32_t	i;

	i = 0;
	while (i < regs->edx)
	{
		terminal_putchar(((char *)regs->esi)[i]);
		i++;
	}
	return (i);
}

void	init_syscall()
{
	int	i;

	i = 0;
	while (i < 256)
	{
		kernel.SYSCALLHandlers[i] = 0;
		i++;
	}
	kernel.SYSCALLHandlers[1] = &SYS_write;
}

void syscall_handler(t_registers *regs)
{
	int	eax;

	eax = regs->eax;
	if (eax < 0 || eax >= 256)
		return ;
	if (kernel.SYSCALLHandlers[eax] != NULL)
		eax = kernel.SYSCALLHandlers[eax](regs);
}
