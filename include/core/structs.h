/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/22 19:00:41 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

#include "libs.h"
#include "defines.h"

/* ──────────── Interrupt Structures ──────────── */
typedef struct s_idt_entry {
	uint16_t	base_low;
	uint16_t	selector;
	uint8_t		always0;
	uint8_t		flags;
	uint16_t	base_high;
} __attribute__((packed)) t_idt_entry;

typedef struct s_khdr
{
    uint32_t size;
    uint32_t magic;
}	khdr_t;

typedef struct s_idt_descryptor {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) t_idt_descryptor;

typedef struct s_registers {
	uint32_t	ds;
	uint32_t	edi, esi, ebp, kern_esp, ebx, edx, ecx, eax;
	uint32_t	interrupt, error;
	uint32_t	eip, cs, eflags, esp, ss;
} __attribute__((packed)) t_registers;

typedef void (*ISRHandler)(t_registers *regs);
typedef void (*IRQHandler)(t_registers *regs);
typedef void (*SignalHandler)(t_registers *regs, int signal);

typedef struct s_signal_event {
	int				id;
	t_registers		context;
} t_signal_event;

typedef struct s_panic_context {
	t_registers		regs;
	uint32_t		stack[PANIC_STACK_SNAPSHOT_WORDS];
	size_t			stack_words;
	const char		*reason;
} t_panic_context;

/* ──────────── GDT Structures ──────────── */
typedef struct s_gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_middle;
	uint8_t  access;
	uint8_t  granularity;
	uint8_t  base_high;
} __attribute__((packed)) t_gdt_entry;

typedef struct s_gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) t_gdt_ptr;

/* ──────────── GDT Segment Information Structure ──────────── */
typedef struct s_gdt_segment_info {
	const char *name;
	uint16_t selector;
} t_gdt_segment_info;

/* ──────────── Screen Management Structures ──────────── */
typedef struct s_screens {
	int			shell_mode;
	size_t		row;
	size_t		column;
	uint8_t		color;
	uint16_t	content[VGA_WIDTH * (VGA_HEIGHT * NB_SCROLL)];
	uint16_t	pos_cursor;
	uint16_t	offset;
} t_screens;

/* ──────────── Main Kernel Structure ──────────── */
typedef struct s_kernel {
	ISRHandler			ISRhandlers[256];
	IRQHandler			IRQHandlers[16];
	SignalHandler		signal_handlers[MAX_SIGNALS];
	t_signal_event		signal_queue[MAX_SIGNAL_QUEUE];
	size_t				signal_queue_head;
	size_t				signal_queue_tail;
	bool				signal_queue_full;
	bool				processing_signals;
	int					last_signal_id;
	t_idt_entry			idt[IDT_ENTRIES];
	t_idt_descryptor	idt_descriptor;
	t_screens			screens[NB_SCREEN];
	uint8_t				terminal_ctrl;
	uint8_t				terminal_shift;
	uint8_t				screen_index;
	uint16_t			*terminal_buffer;
	char				command_buffer[COMMAND_BUFFER_SIZE];
	int					buffer_pos;
	uint32_t			kernel_stack[KERNEL_STACK_SIZE];
	int					stack_pointer;
	t_gdt_ptr			gdt_pointer;
	t_panic_context		panic_context;
} t_kernel;

#endif
