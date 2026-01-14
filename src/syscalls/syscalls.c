/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscalls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 14:20:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/24 14:20:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

#define SYSCALL_PING 0
#define SYSCALL_COUNT 1

static void syscall_handler(t_registers *regs)
{
	uint32_t id = regs->eax;

	signal_schedule(SIGNAL_SYSCALL, regs);
	if (id == SYSCALL_PING)
		regs->eax = 0xC0DE;
	else if (id == SYSCALL_COUNT)
		regs->eax = kernel.signal_counts[SIGNAL_SYSCALL];
	else
		regs->eax = (uint32_t)-1;
}

void syscalls_init(void)
{
	ISR_RegisterHandler(0x80, syscall_handler);
}
