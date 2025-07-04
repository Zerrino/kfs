/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enums.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:10:55 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/04 15:11:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUMS_H
# define ENUMS_H

/* ──────────── VGA Color Enumeration ──────────── */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

/* ──────────── IDT Flags Enumeration ──────────── */
typedef enum {
	IDT_FLAG_GATE_TASK			= 0x5,
	IDT_FLAG_GATE_16BIT_INT		= 0x6,
	IDT_FLAG_GATE_16BIT_TRAP	= 0x7,
	IDT_FLAG_GATE_32BIT_INT		= 0xE,
	IDT_FLAG_GATE_32BIT_TRAP	= 0xF,

	IDT_FLAG_RIN0				= (0 << 5),
	IDT_FLAG_RIN1				= (1 << 5),
	IDT_FLAG_RIN2				= (2 << 5),
	IDT_FLAG_RIN3				= (3 << 5),

	IDT_FLAG_PRESENT			= 0x80
}	IDT_FLAGS;

/* ──────────── PIC Command Enumeration ──────────── */
typedef enum {
	PIC_CMD_EOF		= 0x20,
	PIC_CMD_READ_IRR		= 0x0A,
	PIC_CMD_READ_ISR		= 0x0B,
}	PIC_CMD;

/* ──────────── PIC ICW1 Enumeration ──────────── */
typedef enum {
	PIC_ICW1_ICW4		= 0x01,
	PIC_ICW1_SINGLE		= 0x02,
	PIC_ICW1_INTERVAL4	= 0x04,
	PIC_ICW1_LEVEL		= 0x08,
	PIC_ICW1_INIT		= 0x10,
}	PIC_ICW1;

/* ──────────── PIC ICW4 Enumeration ──────────── */
typedef enum {
	PIC_ICW4_8086			= 0x01,
	PIC_ICW4_AUTO_EOI		= 0x02,
	PIC_ICW4_BUFFER_MASTER	= 0x04,
	PIC_ICW4_BUFFER_SLAVE	= 0x00,
	PIC_ICW4_BUFFERED		= 0x08,
	PIC_ICW4_SFNM			= 0x10,
}	PIC_ICW4;

/* ──────────── Memory Panic Levels ──────────── */
typedef enum e_panic_level {
	PANIC_LEVEL_WARNING = 0,    /* Warning - system can continue */
	PANIC_LEVEL_ERROR = 1,      /* Error - system should stop */
	PANIC_LEVEL_FATAL = 2       /* Fatal - system must halt immediately */
} t_panic_level;

/* ──────────── Shell Command Types ──────────── */
typedef enum {
	CMD_HELP,
	CMD_STACK,
	CMD_PUSH,
	CMD_POP,
	CMD_CLEAR,
	CMD_REBOOT,
	CMD_HALT,
	CMD_SHUTDOWN,
	CMD_MEMSTATS,
	CMD_MEMTEST,
	CMD_MEMTEST1,
	CMD_MEMTEST2,
	CMD_MEMTEST3,
	CMD_MEMTEST4,
	CMD_MEMTEST4DEBUG,
	CMD_MEMTEST5,
	CMD_CRASHTEST,
	CMD_CRASH1,
	CMD_CRASH2,
	CMD_CRASH3,
	CMD_CRASH4,
	CMD_CRASH5,
	CMD_CRASH6,
	CMD_VMEMSTATS,
	CMD_MEMCHECK,
	CMD_UNKNOWN
} command_type_t;

#endif /* ENUMS_H */
