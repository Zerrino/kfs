/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 16:17:36 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

#include "libs.h"
#include "defines.h"
#include "enums.h"

/* ──────────── Interrupt Structures ──────────── */
typedef struct s_idt_entry {
	uint16_t	base_low;
	uint16_t	selector;
	uint8_t		always0;
	uint8_t		flags;
	uint16_t	base_high;
} __attribute__((packed)) t_idt_entry;

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

/* ──────────── Memory Management Structures ──────────── */

/* ──────────── Page Table Entry Structure ──────────── */
typedef struct s_page_table_entry {
	uint32_t present    : 1;   /* Page present in memory */
	uint32_t writable   : 1;   /* Read-only if clear, readwrite if set */
	uint32_t user       : 1;   /* Supervisor level only if clear */
	uint32_t pwt        : 1;   /* Page write through */
	uint32_t pcd        : 1;   /* Page cache disabled */
	uint32_t accessed   : 1;   /* Has the page been accessed since last refresh? */
	uint32_t dirty      : 1;   /* Has the page been written to since last refresh? */
	uint32_t pat        : 1;   /* Page attribute table */
	uint32_t global     : 1;   /* Global page */
	uint32_t ignored    : 3;   /* Amalgamation of unused and reserved bits */
	uint32_t frame      : 20;  /* Frame address (shifted right 12 bits) */
} __attribute__((packed)) t_page_table_entry;

/* ──────────── Page Directory Entry Structure ──────────── */
typedef struct s_page_dir_entry {
	uint32_t present    : 1;   /* Page table present in memory */
	uint32_t writable   : 1;   /* Read-only if clear, readwrite if set */
	uint32_t user       : 1;   /* Supervisor level only if clear */
	uint32_t pwt        : 1;   /* Page write through */
	uint32_t pcd        : 1;   /* Page cache disabled */
	uint32_t accessed   : 1;   /* Has the page table been accessed since last refresh? */
	uint32_t reserved   : 1;   /* Reserved bit */
	uint32_t page_size  : 1;   /* 0 = 4KB page, 1 = 4MB page */
	uint32_t ignored    : 4;   /* Amalgamation of unused and reserved bits */
	uint32_t frame      : 20;  /* Frame address (shifted right 12 bits) */
} __attribute__((packed)) t_page_dir_entry;

/* ──────────── Page Table Structure ──────────── */
typedef struct s_page_table {
	t_page_table_entry pages[PAGES_PER_TABLE];
} __attribute__((packed)) t_page_table;

/* ──────────── Page Directory Structure ──────────── */
typedef struct s_page_directory {
	t_page_dir_entry tables[TABLES_PER_DIR];
} __attribute__((packed)) t_page_directory;

/* ──────────── Physical Memory Manager Structure ──────────── */
typedef struct s_phys_mem_manager {
	uint8_t *bitmap;            /* Bitmap for tracking allocated pages */
	uint32_t total_pages;       /* Total number of pages */
	uint32_t used_pages;        /* Number of used pages */
	uint32_t free_pages;        /* Number of free pages */
	int first_free_page;        /* Index of first free page (optimization, -1 if none) */
} t_phys_mem_manager;

/* ──────────── Virtual Memory Area Structure ──────────── */
typedef struct s_vma {
	uint32_t start_addr;        /* Start virtual address */
	uint32_t end_addr;          /* End virtual address */
	uint32_t flags;             /* Memory flags */
	struct s_vma *next;         /* Next VMA in list */
} t_vma;

/* ──────────── Memory Block Structure ──────────── */
typedef struct s_mem_block {
	size_t size;                /* Size of the block */
	int is_free;                /* 1 if free, 0 if allocated */
	struct s_mem_block *next;   /* Next block in list */
	struct s_mem_block *prev;   /* Previous block in list */
} t_mem_block;

/* ──────────── Kernel Heap Structure ──────────── */
typedef struct s_kernel_heap {
	uint32_t start_addr;        /* Heap start address */
	uint32_t end_addr;          /* Heap end address */
	uint32_t current_end;       /* Current heap end */
	t_mem_block *first_block;   /* First memory block */
} t_kernel_heap;

#endif