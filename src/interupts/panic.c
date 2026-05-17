/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codex <codex@openai.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/05/17 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

static void	panic_print_hex(const char *label, uint32_t value)
{
	terminal_writestring(label);
	terminal_writestring("0x");
	printnbr(value, 16);
	terminal_writestring("\n");
}

static void	panic_print_registers(t_registers *regs)
{
	if (regs == NULL)
	{
		terminal_writestring("Registers: unavailable\n");
		return ;
	}
	terminal_writestring("\n--- Registers ---\n");
	panic_print_hex("interrupt: ", regs->interrupt);
	panic_print_hex("error:     ", regs->error);
	panic_print_hex("eip:       ", regs->eip);
	panic_print_hex("esp:       ", regs->esp);
	panic_print_hex("ebp:       ", regs->ebp);
	panic_print_hex("eflags:    ", regs->eflags);
	panic_print_hex("eax:       ", regs->eax);
	panic_print_hex("ebx:       ", regs->ebx);
	panic_print_hex("ecx:       ", regs->ecx);
	panic_print_hex("edx:       ", regs->edx);
	panic_print_hex("esi:       ", regs->esi);
	panic_print_hex("edi:       ", regs->edi);
	panic_print_hex("cs:        ", regs->cs);
	panic_print_hex("ds:        ", regs->ds);
	panic_print_hex("ss:        ", regs->ss);
}

static uint32_t	panic_stack_pointer(t_registers *regs)
{
	uint32_t esp;

	if (regs != NULL && regs->esp != 0)
		return (regs->esp);
	__asm__ volatile ("mov %%esp, %0" : "=r" (esp));
	return (esp);
}

void	panic_save_stack(t_registers *regs)
{
	uint32_t	*stack;

	kernel.panic.has_regs = regs != NULL;
	if (regs != NULL)
		ft_memcpy(&kernel.panic.regs, regs, sizeof(t_registers));
	kernel.panic.stack_base = panic_stack_pointer(regs);
	kernel.panic.stack_count = 0;
	if (kernel.panic.stack_base == 0)
		return ;
	stack = (uint32_t *)kernel.panic.stack_base;
	while (kernel.panic.stack_count < PANIC_STACK_WORDS)
	{
		kernel.panic.stack_words[kernel.panic.stack_count] =
			stack[kernel.panic.stack_count];
		kernel.panic.stack_count++;
	}
}

void	panic_dump_stack(void)
{
	uint32_t	addr;
	uint32_t	i;

	terminal_writestring("\n--- Stack Snapshot ---\n");
	if (kernel.panic.stack_count == 0)
	{
		terminal_writestring("Stack snapshot unavailable\n");
		return ;
	}
	i = 0;
	while (i < kernel.panic.stack_count)
	{
		addr = kernel.panic.stack_base + (i * sizeof(uint32_t));
		terminal_writestring("0x");
		printnbr(addr, 16);
		terminal_writestring(": 0x");
		printnbr(kernel.panic.stack_words[i], 16);
		if (i == 0)
			terminal_writestring(" <- ESP");
		terminal_writestring("\n");
		i++;
	}
}

void	kernel_panic(const char *reason, t_registers *regs)
{
	DisableInterrupts();
	kernel.panic.reason = reason;
	terminal_writestring("\nKERNEL PANIC");
	if (reason != NULL)
	{
		terminal_writestring(": ");
		terminal_writestring(reason);
	}
	terminal_writestring("\n");
	panic_print_registers(regs);
	panic_save_stack(regs);
	panic_dump_stack();
	terminal_writestring("\nSystem halted.\n");
	kernelPanic();
	while (1)
		__asm__ volatile ("hlt");
}
