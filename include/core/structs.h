/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2026/05/14 01:51:49 by alexafer         ###   ########.fr       */
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

typedef struct s_registers
{
	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;

	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;

	uint32_t interrupt;
	uint32_t error;

	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t useresp;
	uint32_t ss;
}	__attribute__((packed)) t_registers;

typedef void		(*ISRHandler)(t_registers *regs);
typedef void		(*IRQHandler)(t_registers *regs);
typedef uint32_t	(*SYSCALLHandler)(t_registers *regs);

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


# define SIGNAL_QUEUE_SIZE 128

typedef struct s_signal {
	ISRHandler			ISRsignals;
	t_registers			regs;
	struct	s_signal	*next;
}	t_signal;

/* ──────────── Main Kernel Structure ──────────── */
typedef struct s_kernel
{
	uint32_t			tick;
	int					signal_ptr;
	int					signalSize;
	t_signal			*signalBegin;
	t_signal			*signalEnd;
	t_signal			signalQueue[SIGNAL_QUEUE_SIZE];
	ISRHandler			ISRSignalsQueue[SIGNAL_QUEUE_SIZE];
	t_registers			signal_regs[SIGNAL_QUEUE_SIZE];
	ISRHandler			ISRhandlers[256];
	SYSCALLHandler		SYSCALLHandlers[256];
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
} t_kernel;

#endif
