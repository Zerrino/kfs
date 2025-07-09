/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zerrino <zerrino@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:48:23 by alexafer          #+#    #+#             */
/*   Updated: 2025/07/02 18:24:31 by zerrino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



/* This tutorial will only work for the 32-bit ix86 targets. */
# if !defined(__i386__)
	#error "This tutorial needs to be compiled with a ix86-elf compiler"
# endif

#ifndef KERNEL_H
# define KERNEL_H

/* ──────────── Ports du PIC 8259 ──────────── */
#define PIC1_CMD    0x20    /* Command register (Master)  */
#define PIC1_DATA   0x21    /* Data register    (Master)  */
#define PIC2_CMD    0xA0    /* Command register (Slave)   */
#define PIC2_DATA   0xA1    /* Data register    (Slave)   */

/* ──────────── ICW1 (Initialization Cmd Word 1) ──────────── */
#define ICW1_ICW4   0x01    /* ICW4 suivra               */
#define ICW1_INIT   0x10    /* Lancer la séquence d’init */
#define ICW1        (ICW1_INIT | ICW1_ICW4)       /* 0x11 */

/* ──────────── Offsets après remap (ICW2) ──────────── */
#define PIC1_OFFSET 0x20    /* IRQ0-7  → vecteurs 0x20-27 */
#define PIC2_OFFSET 0x28    /* IRQ8-15 → vecteurs 0x28-2F */

/* ──────────── Cascade configuration (ICW3) ──────────── */
#define PIC1_CASCADE_IRQ2 0x04   /* Le Slave est branché sur IRQ2 */
#define PIC2_CASCADE_ID   0x02   /* ID du Slave dans la chaîne    */

/* ──────────── ICW4 ──────────── */
#define ICW4_8086  0x01     /* Mode 8086/88 */

/* ──────────── Masques (OCW1) ──────────── */
#define PIC_MASK_NONE 0x00  /* Dé-masquer toutes les lignes */

/* ──────────── Commande EOI (End-Of-Interrupt) ──────────── */
#define PIC_EOI    0x20




/* Kernel-compatible type definitions */
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;
typedef uint32_t           size_t;
typedef int32_t            ssize_t;

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#define INT_MIN (-2147483648)
#define INT_MAX (2147483647)

typedef int bool;

# include "idt.h"


# define VGA_WIDTH   80
# define VGA_HEIGHT  25
# define VGA_MEMORY  0xB8000
# define VGA_CRSR_LOW   0x0F
# define VGA_CRSR_HIGH  0x0E
# define VGA_PORT_INDEX 0x3D4
# define VGA_PORT_DATA  0x3D5
# define IDT_ENTRIES 256

# define PIC1_CMD  0x20
# define PIC1_DATA 0x21
# define PIC2_CMD  0xA0
# define PIC2_DATA 0xA1

# define ICW1_INIT 0x10
# define ICW1_ICW4 0x01
# define ICW4_8086 0x01

# define NB_SCREEN 3
# define NB_SCROLL 3

# define N(x) (x >> ((sizeof(x) * 8) - 1))
# define ABS(x) ((x + (1 * N(x))) ^ N(x))

/* Hardware text mode color constants. */
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

/* GDT Access Byte flags */
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

/* GDT Granularity flags */
enum gdt_granularity {
	GDT_GRAN_BYTE           = 0x00,  /* Byte granularity */
	GDT_GRAN_4K             = 0x80,  /* 4KB granularity */
	GDT_GRAN_32BIT          = 0x40,  /* 32-bit protected mode */
	GDT_GRAN_16BIT          = 0x00,  /* 16-bit protected mode */

	/* Common combination */
	GDT_GRAN_STANDARD       = 0xC0,  /* 4KB granularity | 32-bit */
};

/* Shell constants */
#define COMMAND_BUFFER_SIZE     256

/* Stack constants */
#define KERNEL_STACK_SIZE       1024

typedef struct s_screens
{
	int			shell_mode;
	size_t		row;
	size_t		column;
	uint8_t		color;
	uint16_t	content[VGA_WIDTH * (VGA_HEIGHT * NB_SCROLL)];
	uint16_t	pos_cursor;
	uint16_t	offset;
}	t_screens;

typedef struct s_kernel
{
	ISRHandler			ISRhandlers[256];
	IRQHandler			IRQHandlers[16];
	t_idt_entry			idt[IDT_ENTRIES];
	t_idt_descryptor	idt_descriptor;
	t_screens			screens[NB_SCREEN];
	uint8_t				terminal_ctrl;
	uint8_t				terminal_shift;
	uint8_t				screen_index;
	uint16_t			*terminal_buffer;

	/* Shell state */
	char				command_buffer[COMMAND_BUFFER_SIZE];
	int					buffer_pos;

	/* Kernel stack */
	uint32_t			kernel_stack[KERNEL_STACK_SIZE];
	int					stack_pointer;
}	t_kernel;

/* GDT Entry Indices - makes gdt_install() calls self-documenting */
enum gdt_entry_index {
	GDT_NULL_INDEX          = 0,  /* Null descriptor (required by CPU) */
	GDT_KERNEL_CODE_INDEX   = 1,  /* Kernel code segment */
	GDT_KERNEL_DATA_INDEX   = 2,  /* Kernel data segment */
	GDT_KERNEL_STACK_INDEX  = 3,  /* Kernel stack segment */
	GDT_USER_CODE_INDEX     = 4,  /* User code segment */
	GDT_USER_DATA_INDEX     = 5,  /* User data segment */
	GDT_USER_STACK_INDEX    = 6,  /* User stack segment */
};

/* GDT Segment Properties - common values used in GDT setup */
enum gdt_segment_properties {
	GDT_NULL_BASE           = 0x00000000,  /* Base address for null descriptor */
	GDT_NULL_LIMIT          = 0x00000000,  /* Limit for null descriptor */
	GDT_SEGMENT_BASE        = 0x00000000,  /* Standard base address (0 for flat memory) */
	GDT_SEGMENT_LIMIT       = 0xFFFFFFFF,  /* Full 4GB segment limit */
	GDT_NULL_ACCESS         = 0x00,        /* Access byte for null descriptor */
	GDT_NULL_GRANULARITY    = 0x00,        /* Granularity for null descriptor */
};

/* GDT Constants */
#define GDT_BASE_ADDRESS        0x00000800  /* GDT base address */
#define GDT_MAX_ENTRIES         7           /* Number of GDT entries */

extern t_kernel	kernel;
void		terminal_offset(uint16_t offset);
void		terminal_restore();

/* src/keyboard.c */
void		update_cursor(int scancode);
void		set_idt_gate(int n, uint32_t handler);
void		keyboard_handler(t_registers* regs);
void		keyboard_init();

/* src/utils.s */
int		ft_strcmp(const char *, const char *);
char	*ft_strcpy(char *, const char *);

/* src/inlinie_utils.c */
uint8_t		vga_entry_color(enum vga_color fg, enum vga_color bg);
uint16_t	vga_entry(unsigned char uc, uint8_t color);
void		vga_set_cursor(size_t row, size_t col);
void		outb(uint16_t port, uint8_t val);
void		outw(uint16_t port, uint16_t val);
uint8_t		inb(uint16_t port);
void		vga_cursor_restore();

/* src/string_utilitary.c */
size_t		strlen(const char* str);
void		terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void		terminal_putchar(char c);
void		terminal_write(const char* data, size_t size);
void		terminal_writestring(const char* data);
void		printnbr(int nbr, int base);


void		crash_me();

/* src/gdt.c */
void		gdt_install();
void		gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
void		print_gdt_info();

/* src/asm/gdt_asm.s */
void		gdt_flush(void *);

/* src/stack.c */
void		stack_push(uint32_t value);
uint32_t	stack_pop();
uint32_t	stack_peek();
int			stack_is_empty();
int			stack_size();
void		print_kernel_stack();

/* src/shell.c */
void		shell_initialize();
void		shell_process_command(const char* cmd);
void		shell_handle_input(char c);
int			strcmp(const char* s1, const char* s2);

/* src/kernel.c */
void		terminal_initialize();

/* src/mem_utils.c*/
void		*ft_memset(void *s, int c, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		*ft_memmove(void *dst, const void *src, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
void		ft_bzero(void *s, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);

#endif
