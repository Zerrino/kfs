/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enums.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/11 11:54:38 by rperez-t         ###   ########.fr       */
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

/* ──────────── GDT Access Byte Flags ──────────── */
enum gdt_access {
	GDT_ACCESS_ACCESSED     = 0x01,  /* Accessed bit */
	GDT_ACCESS_RW           = 0x02,  /* Readable/Writable bit */
	GDT_ACCESS_DC           = 0x04,  /* Direction/Conforming bit */
	GDT_ACCESS_EXEC         = 0x08,  /* Executable bit */
	GDT_ACCESS_DESCRIPTOR   = 0x10,  /* Descriptor type (1 = code/data, 0 = system) */
	GDT_ACCESS_RING0        = 0x00,  /* Ring 0 privilege */
	GDT_ACCESS_RING1        = 0x20,  /* Ring 1 privilege */
	GDT_ACCESS_RING2        = 0x40,  /* Ring 2 privilege */
	GDT_ACCESS_RING3        = 0x60,  /* Ring 3 privilege */
	GDT_ACCESS_PRESENT      = 0x80,  /* Present bit */

	/* Common combinations */
	GDT_ACCESS_KERNEL_CODE  = 0x9A,  /* Present | Ring0 | Descriptor | Exec | RW */
	GDT_ACCESS_KERNEL_DATA  = 0x92,  /* Present | Ring0 | Descriptor | RW */
	GDT_ACCESS_USER_CODE    = 0xFA,  /* Present | Ring3 | Descriptor | Exec | RW */
	GDT_ACCESS_USER_DATA    = 0xF2,  /* Present | Ring3 | Descriptor | RW */
};

/* ──────────── GDT Granularity Flags ──────────── */
enum gdt_granularity {
	GDT_GRAN_BYTE           = 0x00,  /* Byte granularity */
	GDT_GRAN_4K             = 0x80,  /* 4KB granularity */
	GDT_GRAN_32BIT          = 0x40,  /* 32-bit protected mode */
	GDT_GRAN_16BIT          = 0x00,  /* 16-bit protected mode */

	/* Common combination */
	GDT_GRAN_STANDARD       = 0xC0,  /* 4KB granularity | 32-bit */
};

/* ──────────── GDT Entry Indices ──────────── */
enum gdt_entry_index {
	GDT_NULL_INDEX          = 0,  /* Null descriptor (required by CPU) */
	GDT_KERNEL_CODE_INDEX   = 1,  /* Kernel code segment */
	GDT_KERNEL_DATA_INDEX   = 2,  /* Kernel data segment */
	GDT_KERNEL_STACK_INDEX  = 3,  /* Kernel stack segment */
	GDT_USER_CODE_INDEX     = 4,  /* User code segment */
	GDT_USER_DATA_INDEX     = 5,  /* User data segment */
	GDT_USER_STACK_INDEX    = 6,  /* User stack segment */
};

/* ──────────── GDT Segment Properties ──────────── */
enum gdt_segment_properties {
	GDT_NULL_BASE           = 0x00000000,  /* Base address for null descriptor */
	GDT_NULL_LIMIT          = 0x00000000,  /* Limit for null descriptor */
	GDT_SEGMENT_BASE        = 0x00000000,  /* Standard base address (0 for flat memory) */
	GDT_SEGMENT_LIMIT       = 0xFFFFFFFF,  /* Full 4GB segment limit */
	GDT_NULL_ACCESS         = 0x00,        /* Access byte for null descriptor */
	GDT_NULL_GRANULARITY    = 0x00,        /* Granularity for null descriptor */
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
} IDT_FLAGS;

/* ──────────── PIC Command Enumeration ──────────── */
typedef enum {
	PIC_CMD_EOF		= 0x20,
	PIC_CMD_READ_IRR		= 0x0A,
	PIC_CMD_READ_ISR		= 0x0B,
} PIC_CMD;

/* ──────────── PIC ICW1 Enumeration ──────────── */
typedef enum {
	PIC_ICW1_ICW4		= 0x01,
	PIC_ICW1_SINGLE		= 0x02,
	PIC_ICW1_INTERVAL4	= 0x04,
	PIC_ICW1_LEVEL		= 0x08,
	PIC_ICW1_INIT		= 0x10,
} PIC_ICW1;

/* ──────────── PIC ICW4 Enumeration ──────────── */
typedef enum {
	PIC_ICW4_8086			= 0x01,
	PIC_ICW4_AUTO_EOI		= 0x02,
	PIC_ICW4_BUFFER_MASTER	= 0x04,
	PIC_ICW4_BUFFER_SLAVE	= 0x00,
	PIC_ICW4_BUFFERED		= 0x08,
	PIC_ICW4_SFNM			= 0x10,
} PIC_ICW4;

/* ──────────── Shell Command Enumeration ──────────── */
typedef enum {
    CMD_HELP,
    CMD_STACK,
    CMD_USERSTACK,
    CMD_GDT,
    CMD_SEGMENTS,
    CMD_MEMORY,
    CMD_GDTTEST,
    CMD_STACKTEST,
    CMD_SYSINFO,
    CMD_INTERRUPTS,
    CMD_KFS4,
    CMD_SIGNALS,
    CMD_SIGRAISE,
    CMD_PANICINFO,
    CMD_PUSH,
    CMD_POP,
    CMD_CLEAR,
    CMD_REBOOT,
    CMD_HALT,
    CMD_SHUTDOWN,
    CMD_UNKNOWN
} command_type_t;

/* ──────────── CPU Exception Enumeration ──────────── */
typedef enum {
    EXCEPTION_DIVIDE_BY_ZERO = 0,          /* Divide by zero error */
    EXCEPTION_DEBUG = 1,                   /* Debug */
    EXCEPTION_NMI = 2,                     /* Non-maskable Interrupt */
    EXCEPTION_BREAKPOINT = 3,              /* Breakpoint */
    EXCEPTION_OVERFLOW = 4,                /* Overflow */
    EXCEPTION_BOUND_RANGE = 5,             /* Bound Range Exceeded */
    EXCEPTION_INVALID_OPCODE = 6,          /* Invalid Opcode */
    EXCEPTION_DEVICE_NOT_AVAILABLE = 7,    /* Device Not Available */
    EXCEPTION_DOUBLE_FAULT = 8,            /* Double Fault */
    EXCEPTION_COPROCESSOR_OVERRUN = 9,     /* Coprocessor Segment Overrun */
    EXCEPTION_INVALID_TSS = 10,            /* Invalid TSS */
    EXCEPTION_SEGMENT_NOT_PRESENT = 11,    /* Segment Not Present */
    EXCEPTION_STACK_SEGMENT_FAULT = 12,    /* Stack-Segment Fault */
    EXCEPTION_GENERAL_PROTECTION = 13,     /* General Protection Fault */
    EXCEPTION_PAGE_FAULT = 14,             /* Page Fault */
    EXCEPTION_RESERVED_15 = 15,            /* Reserved */
    EXCEPTION_X87_FPU = 16,                /* x87 Floating-Point Exception */
    EXCEPTION_ALIGNMENT_CHECK = 17,        /* Alignment Check */
    EXCEPTION_MACHINE_CHECK = 18,          /* Machine Check */
    EXCEPTION_SIMD_FPU = 19,               /* SIMD Floating-Point Exception */
    EXCEPTION_VIRTUALIZATION = 20,         /* Virtualization Exception */
    EXCEPTION_CONTROL_PROTECTION = 21,     /* Control Protection Exception */
    EXCEPTION_RESERVED_22 = 22,            /* Reserved */
    EXCEPTION_RESERVED_23 = 23,            /* Reserved */
    EXCEPTION_RESERVED_24 = 24,            /* Reserved */
    EXCEPTION_RESERVED_25 = 25,            /* Reserved */
    EXCEPTION_RESERVED_26 = 26,            /* Reserved */
    EXCEPTION_RESERVED_27 = 27,            /* Reserved */
    EXCEPTION_HYPERVISOR_INJECTION = 28,   /* Hypervisor Injection Exception */
    EXCEPTION_VMM_COMMUNICATION = 29,      /* VMM Communication Exception */
    EXCEPTION_SECURITY = 30,               /* Security Exception */
    EXCEPTION_RESERVED_31 = 31,            /* Reserved */
    EXCEPTION_MAX = 32                     /* Maximum exception number */
} cpu_exception_t;

#endif
