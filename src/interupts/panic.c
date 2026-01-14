/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 13:10:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/24 13:10:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

void	panic_clean_registers(void)
{
	clear_registers();
}

void	panic_save_stack(void)
{
	uint32_t esp;
	uint32_t ebp;
	uint32_t stack_base;
	uint32_t stack_top;
	uint32_t max_words;

	__asm__ volatile ("mov %%esp, %0" : "=r"(esp));
	__asm__ volatile ("mov %%ebp, %0" : "=r"(ebp));

	kernel.panic_stack_esp = esp;
	kernel.panic_stack_ebp = ebp;
	kernel.panic_stack_count = 0;
	kernel.panic_stack_valid = 0;

	stack_base = (uint32_t)kernel_execution_stack;
	stack_top = (uint32_t)kernel_stack_top;
	if (esp < stack_base || esp > stack_top)
		return;

	max_words = (stack_top - esp) / sizeof(uint32_t);
	if (max_words > PANIC_STACK_SNAPSHOT_WORDS)
		max_words = PANIC_STACK_SNAPSHOT_WORDS;
	for (uint32_t i = 0; i < max_words; i++)
		kernel.panic_stack_snapshot[i] = *(uint32_t *)(esp + (i * 4));
	kernel.panic_stack_count = max_words;
	kernel.panic_stack_valid = 1;
}

static void	panic_dump_stack_snapshot(void)
{
	if (!kernel.panic_stack_valid || kernel.panic_stack_count == 0)
	{
		terminal_writestring("No panic stack snapshot available.\n");
		return;
	}
	terminal_writestring("Saved stack snapshot:\n");
	terminal_writestring("ESP=0x");
	printnbr(kernel.panic_stack_esp, 16);
	terminal_writestring(" EBP=0x");
	printnbr(kernel.panic_stack_ebp, 16);
	terminal_writestring("\n");
	for (uint32_t i = 0; i < kernel.panic_stack_count; i++)
	{
		terminal_writestring("  [");
		printnbr(i, 10);
		terminal_writestring("] 0x");
		printnbr(kernel.panic_stack_snapshot[i], 16);
		terminal_writestring("\n");
	}
}

void	kernel_panic(const char *message, const char *file, int line, int dump)
{
	DisableInterrupts();
	terminal_writestring("[ KERNEL PANIC at (");
	terminal_writestring(file);
	terminal_writestring(":");
	printnbr((uint32_t)line, 10);
	terminal_writestring(") ] ");
	if (message != NULL)
		terminal_writestring(message);
	terminal_writestring("\n");
	panic_save_stack();
	if (dump)
		panic_dump_stack_snapshot();
	panic_clean_registers();
	kernelPanic();
}

void	kernel_halt(const char *message)
{
	DisableInterrupts();
	if (message != NULL)
	{
		terminal_writestring(message);
		terminal_writestring("\n");
	}
	panic_save_stack();
	panic_clean_registers();
	kernelPanic();
}
