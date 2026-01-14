/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 14:40:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/01/14 13:00:15 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

static void	snapshot_stack(t_registers *regs)
{
	uint32_t	*esp;

	if (regs == NULL || regs->esp == 0)
	{
		kernel.panic_context.stack_words = 0;
		return ;
	}
	esp = (uint32_t *)regs->esp;
	for (size_t i = 0; i < PANIC_STACK_SNAPSHOT_WORDS; i++)
		kernel.panic_context.stack[i] = esp[i];
	kernel.panic_context.stack_words = PANIC_STACK_SNAPSHOT_WORDS;
}

void	panic_init(void)
{
	ft_memset(&kernel.panic_context, 0, sizeof(kernel.panic_context));
	kernel.panic_context.reason = "none";
}

void	panic_capture_state(t_registers *regs, const char *reason)
{
	if (regs != NULL)
		kernel.panic_context.regs = *regs;
	else
		ft_memset(&kernel.panic_context.regs, 0, sizeof(kernel.panic_context.regs));
	kernel.panic_context.reason = (reason != NULL) ? reason : "unknown";
	snapshot_stack(regs);
}

void	panic_clean_registers(void)
{
	__asm__ volatile (
		"xor %%eax, %%eax\n"
		"xor %%ebx, %%ebx\n"
		"xor %%ecx, %%ecx\n"
		"xor %%edx, %%edx\n"
		"xor %%esi, %%esi\n"
		"xor %%edi, %%edi\n"
		:
		:
		: "eax", "ebx", "ecx", "edx", "esi", "edi"
	);
}

void	panic_halt(t_registers *regs, const char *reason)
{
	panic_capture_state(regs, reason);
	panic_clean_registers();
	kernelPanic();
}
