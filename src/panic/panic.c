/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+ +:+         +:+     */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 10:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/08/24 10:30:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

static uint32_t panic_stack_snapshot[PANIC_STACK_WORDS];
static uint32_t panic_stack_base;
static size_t panic_stack_words;

static void panic_print_registers(const t_registers *regs)
{
	if (regs == NULL)
		return;
	terminal_writestring("Registers:\n");
	terminal_writestring("  EAX=0x"); printnbr(regs->eax, 16);
	terminal_writestring(" EBX=0x"); printnbr(regs->ebx, 16);
	terminal_writestring(" ECX=0x"); printnbr(regs->ecx, 16);
	terminal_writestring(" EDX=0x"); printnbr(regs->edx, 16);
	terminal_writestring("\n");
	terminal_writestring("  ESI=0x"); printnbr(regs->esi, 16);
	terminal_writestring(" EDI=0x"); printnbr(regs->edi, 16);
	terminal_writestring(" EBP=0x"); printnbr(regs->ebp, 16);
	terminal_writestring(" ESP=0x"); printnbr(regs->esp, 16);
	terminal_writestring("\n");
	terminal_writestring("  EIP=0x"); printnbr(regs->eip, 16);
	terminal_writestring(" CS=0x"); printnbr(regs->cs, 16);
	terminal_writestring(" EFLAGS=0x"); printnbr(regs->eflags, 16);
	terminal_writestring("\n");
	terminal_writestring("  INT=0x"); printnbr(regs->interrupt, 16);
	terminal_writestring(" ERR=0x"); printnbr(regs->error, 16);
	terminal_writestring("\n");
}

void panic_save_stack(void)
{
	uint32_t esp;
	uint32_t stack_top;

	__asm__ volatile ("mov %%esp, %0" : "=r"(esp));
	stack_top = (uint32_t)kernel_stack_top;
	panic_stack_base = esp;
	panic_stack_words = 0;

	for (uint32_t addr = esp;
		addr < stack_top && panic_stack_words < PANIC_STACK_WORDS;
		addr += sizeof(uint32_t))
	{
		panic_stack_snapshot[panic_stack_words++] = *(uint32_t *)addr;
	}
}

void panic_dump_stack(void)
{
	if (panic_stack_words == 0)
	{
		terminal_writestring("No stack snapshot captured.\n");
		return;
	}

	terminal_writestring("Stack snapshot:\n");
	for (size_t i = 0; i < panic_stack_words; i++)
	{
		uint32_t addr = panic_stack_base + (uint32_t)(i * sizeof(uint32_t));
		terminal_writestring("  0x");
		printnbr(addr, 16);
		terminal_writestring(": 0x");
		printnbr(panic_stack_snapshot[i], 16);
		terminal_writestring("\n");
	}
}

void panic_clean_registers(void)
{
	clear_registers();
}

void kernel_halt(void)
{
	DisableInterrupts();
	panic_clean_registers();
	kernelPanic();
}

void kernel_panic(const char *message, t_registers *regs)
{
	DisableInterrupts();
	terminal_writestring("KERNEL PANIC: ");
	if (message != NULL)
		terminal_writestring(message);
	terminal_writestring("\n");
	panic_print_registers(regs);
	panic_save_stack();
	panic_dump_stack();
	panic_clean_registers();
	kernelPanic();
}
