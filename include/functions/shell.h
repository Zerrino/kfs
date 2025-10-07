/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:30:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/10/07 12:40:40 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

#include "../core/enums.h"
#include "../core/libs.h"
#include "../core/structs.h"

#define CEIL_DIV(a,b)   (((a) + (b) - 1) / (b))
#define ALIGN_UP(x, align)   ( ((x) + ((align) - 1)) & ~((align) - 1) )
#define ALIGN_DOWN(x, align) ( (x) & ~((align) - 1) )


#define KERNEL_START 0xC0000000


#define PAGE_FLAG_PRESENT	(1 << 0)
#define PAGE_FLAG_WRITE		(1 << 1)
#define PAGE_FLAG_OWNER		(1 << 9)
#define KERNEL_MALLOC 0xD0000000
#define REC_PAGEDIR ((uint32_t*)0xfffff000)
#define REC_PAGETABLE(i) ((uint32_t*) (0xFFC00000 + ((i) << 12)))

uint32_t	*memGetCurrentPageDir();
void		memChangePageDir(uint32_t* pd);
void		syncPageDirs();
void		memMapPage(uint32_t virtualAddr, uint32_t physicalAddr, uint32_t flags);
uint32_t	pmmAllocPageFrame();
void		kmallocInit(uint32_t initialHeapSize);
void		changeHeapSize(int newSize);
void		memMapPage(uint32_t virtualAddr, uint32_t physicalAddr, uint32_t flags);

void		initMemory(uint32_t memHigh, uint32_t physicalAllocStart);

/* ──────────── Functions from src/shell/keyboard.c ──────────── */

void keyboard_init(void);
void keyboard_handler(t_registers *regs);
void update_cursor(int scancode);

/* ──────────── Functions from src/shell/shell_write.c ──────────── */

size_t strlen(const char* str);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_writestring(const char* data);
void terminal_scroll_up(void);
void terminal_newline(void);
void	printnbr(uint32_t nbr, uint32_t base);

/* ──────────── Functions from src/shell/vga_utils.c ──────────── */

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
uint16_t vga_entry(unsigned char uc, uint8_t color);
void vga_set_cursor(size_t row, size_t col);
void vga_cursor_restore(void);

/* ──────────── Functions from src/shell/mem_utils.c ──────────── */

void *ft_memcpy(void *dest, const void *src, size_t n);
void *ft_memset(void *s, int c, size_t n);
void *ft_memmove(void *dest, const void *src, size_t n);
void *ft_memchr(const void *s, int c, size_t n);
int ft_memcmp(const void *s1, const void *s2, size_t n);
void ft_bzero(void *s, size_t n);
int ft_strcmp(const char* s1, const char* s2);

/* ──────────── Functions from src/shell/kfs2.c ──────────── */

void display_kfs2_help(void);
command_type_t get_kfs2_command_type(const char* command);
bool handle_kfs2_commands(command_type_t cmd_type, const char* arg);

/* ──────────── Functions from src/shell/kfs2/shell_core.c ──────────── */

void handle_clear(void);
void handle_reboot(void);
void handle_halt(void);
void handle_shutdown(void);

/* ──────────── Functions from src/shell/kfs2/shell_gdt.c ──────────── */

void handle_gdt(void);
void handle_segments(void);
void handle_memory(void);
void handle_gdttest(void);

/* ──────────── Functions from src/shell/kfs2/shell_stack.c ──────────── */

void handle_stack(void);
void handle_userstack(void);
void handle_push(const char* arg);
void handle_pop(void);
void handle_stacktest(void);

/* ──────────── Functions from src/shell/kfs2/shell_sysinfo.c ──────────── */

void handle_sysinfo(void);
void handle_interrupts(void);

/* ──────────── Functions from src/shell/kfs2/kfs2_handle.c ──────────── */

void display_core_system_help(void);
void display_gdt_memory_help(void);
void display_stack_operations_help(void);
void display_system_info_help(void);
command_type_t get_core_system_command_type(const char* command);
command_type_t get_gdt_memory_command_type(const char* command);
command_type_t get_stack_operations_command_type(const char* command);
command_type_t get_system_info_command_type(const char* command);
bool handle_core_system_commands(command_type_t cmd_type, const char* arg);
bool handle_gdt_memory_commands(command_type_t cmd_type, const char* arg);
bool handle_stack_operations_commands(command_type_t cmd_type, const char* arg);
bool handle_system_info_commands(command_type_t cmd_type, const char* arg);

#endif
