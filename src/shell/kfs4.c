/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kfs4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 14:40:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/27 14:40:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

static int	parse_int(const char *s, int *out)
{
	int	value;
	int	sign;

	if (!s || *s == '\0')
		return (-1);
	value = 0;
	sign = 1;
	if (*s == '-')
	{
		sign = -1;
		s++;
	}
	while (*s)
	{
		if (*s < '0' || '9' < *s)
			return (-1);
		value = value * 10 + (*s - '0');
		s++;
	}
	*out = value * sign;
	return (0);
}

void	display_kfs4_help(void)
{
	terminal_writestring("\n=== KFS4 Interrupts & Signals ===\n");
	terminal_writestring("  kfs4         - Show this help menu\n");
	terminal_writestring("  interrupts   - Show interrupt/PIC status\n");
	terminal_writestring("  signals      - Show signal handlers and queue state\n");
	terminal_writestring("  sigraise <n> - Schedule software signal <n> (default 0)\n");
	terminal_writestring("  panicinfo    - Dump last panic snapshot\n");
}

void	handle_signals_status(void)
{
	size_t	pending;
	size_t	registered;

	pending = SIG_PendingCount();
	registered = 0;
	for (int i = 0; i < MAX_SIGNALS; i++)
	{
		if (kernel.signal_handlers[i] != NULL)
			registered++;
	}
	terminal_writestring("Signal status:\n  registered: ");
	printnbr((uint32_t)registered, 10);
	terminal_writestring("\n  pending:    ");
	printnbr((uint32_t)pending, 10);
	terminal_writestring("\n  last:       ");
	if (kernel.last_signal_id >= 0)
		printnbr((uint32_t)kernel.last_signal_id, 10);
	else
		terminal_writestring("none");
	terminal_writestring("\n");

	if (pending > 0)
	{
		terminal_writestring("Pending queue: ");
		size_t idx = kernel.signal_queue_head;
		for (size_t i = 0; i < pending; i++)
		{
			printnbr((uint32_t)kernel.signal_queue[idx].id, 10);
			terminal_writestring(i + 1 < pending ? ", " : "\n");
			idx = (idx + 1) % MAX_SIGNAL_QUEUE;
		}
	}
}

void	handle_sigraise(const char *arg)
{
	int	signal;

	if (arg && *arg)
	{
		if (parse_int(arg, &signal) != 0)
		{
			terminal_writestring("sigraise: invalid id\n");
			return ;
		}
	}
	else
		signal = SIGNAL_USER_TEST;
	if (!SIG_Schedule(signal, NULL))
	{
		terminal_writestring("sigraise: queue full or invalid id\n");
		return ;
	}
	SIG_ProcessPending();
	terminal_writestring("signal scheduled\n");
}

void	handle_panicinfo(void)
{
	terminal_writestring("Last panic reason: ");
	terminal_writestring(kernel.panic_context.reason ? kernel.panic_context.reason : "none");
	terminal_writestring("\nRegisters snapshot:\n");
	terminal_writestring("  EIP: 0x"); printnbr(kernel.panic_context.regs.eip, 16);
	terminal_writestring("  ESP: 0x"); printnbr(kernel.panic_context.regs.esp, 16);
	terminal_writestring("  EBP: 0x"); printnbr(kernel.panic_context.regs.ebp, 16);
	terminal_writestring("\n");
	terminal_writestring("Stack snapshot (first 4 words): ");
	for (size_t i = 0; i < kernel.panic_context.stack_words && i < 4; i++)
	{
		terminal_writestring("0x");
		printnbr(kernel.panic_context.stack[i], 16);
		terminal_writestring(i + 1 < kernel.panic_context.stack_words && i < 3 ? " " : "\n");
	}
	if (kernel.panic_context.stack_words == 0)
		terminal_writestring("empty\n");
}

command_type_t	get_kfs4_command_type(const char *command)
{
	if (ft_strcmp(command, "kfs4") == 0)
		return (CMD_KFS4);
	if (ft_strcmp(command, "signals") == 0)
		return (CMD_SIGNALS);
	if (ft_strcmp(command, "sigraise") == 0)
		return (CMD_SIGRAISE);
	if (ft_strcmp(command, "panicinfo") == 0)
		return (CMD_PANICINFO);
	return (CMD_UNKNOWN);
}

bool	handle_kfs4_commands(command_type_t cmd_type, const char *arg)
{
	switch (cmd_type)
	{
		case CMD_KFS4:
			display_kfs4_help();
			return (true);
		case CMD_SIGNALS:
			handle_signals_status();
			return (true);
		case CMD_SIGRAISE:
			handle_sigraise(arg);
			return (true);
		case CMD_PANICINFO:
			handle_panicinfo();
			return (true);
		default:
			return (false);
	}
	(void)arg;
}
