/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 15:40:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/27 15:40:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

static uint32_t	syscall_test(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx, uint32_t esi)
{
	(void)eax;
	terminal_writestring("[syscall] test handler\n");
	terminal_writestring("  ebx: 0x"); printnbr(ebx, 16); terminal_writestring("\n");
	terminal_writestring("  ecx: 0x"); printnbr(ecx, 16); terminal_writestring("\n");
	terminal_writestring("  edx: 0x"); printnbr(edx, 16); terminal_writestring("\n");
	terminal_writestring("  esi: 0x"); printnbr(esi, 16); terminal_writestring("\n");
	return (0xBEEF0000 | (ebx & 0xFFFF));
}

int	syscall_register(int id, syscall_handler_t handler)
{
	if (id < 0 || MAX_SYSCALLS <= id)
		return (-1);
	kernel.syscall_table[id] = handler;
	return (0);
}

uint32_t syscall_invoke(int id, uint32_t ebx, uint32_t ecx, uint32_t edx, uint32_t esi)
{
	uint32_t ret;
	__asm__ volatile(
		"int $0x80"
		: "=a"(ret)
		: "a"(id), "b"(ebx), "c"(ecx), "d"(edx), "S"(esi)
		: "memory"
	);
	return ret;
}

static void	syscall_handler(t_registers *regs)
{
	int					id;
	syscall_handler_t	handler;
	uint32_t			ret;

	id = (int)regs->eax;
	if (id < 0 || MAX_SYSCALLS <= id)
	{
		terminal_writestring("Unhandled syscall id ");
		printnbr((uint32_t)id, 10);
		terminal_writestring("\n");
		return ;
	}
	handler = kernel.syscall_table[id];
	if (handler == NULL)
	{
		terminal_writestring("Syscall not registered ");
		printnbr((uint32_t)id, 10);
		terminal_writestring("\n");
		return ;
	}
	ret = handler(regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi);
	kernel.last_syscall_ret = ret;
	kernel.last_syscall_id = id;
	regs->eax = ret;
}

void	syscall_init(void)
{
	for (int i = 0; i < MAX_SYSCALLS; i++)
		kernel.syscall_table[i] = NULL;
	kernel.last_syscall_id = -1;
	kernel.last_syscall_ret = 0;
	/* Allow userland (future) to trigger int 0x80 */
	IDT_SetGate(128, ISR128, GDT_KERNEL_CODE, IDT_FLAG_RIN3 | IDT_FLAG_GATE_32BIT_INT);
	ISR_RegisterHandler(128, syscall_handler);
	syscall_register(SYSCALL_TEST_ID, syscall_test);
}
