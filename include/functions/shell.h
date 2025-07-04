/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:11:31 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/04 15:29:44 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_FUNCTIONS_H
# define SHELL_FUNCTIONS_H

#include "../core/structs.h"
#include "../core/enums.h"

/* ──────────── Terminal Management Functions ──────────── */
void		terminal_offset(uint16_t offset);
void		terminal_restore();
void		terminal_initialize();
void		terminal_writestring(const char* data);
void		terminal_putchar(char c);
void		terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void		terminal_write(const char* data, size_t size);

/* ──────────── Keyboard Functions ──────────── */
void		update_cursor(int scancode);
void		keyboard_handler();

/* ──────────── VGA and Hardware Functions ──────────── */
uint8_t		vga_entry_color(enum vga_color fg, enum vga_color bg);
uint16_t	vga_entry(unsigned char uc, uint8_t color);
void		vga_set_cursor(size_t row, size_t col);
void		outb(uint16_t port, uint8_t val);
void		outw(uint16_t port, uint16_t val);
uint8_t		inb(uint16_t port);
void		vga_cursor_restore();

/* ──────────── Number Printing Functions ──────────── */
void		printnbr(int n, int base);

/* ──────────── Shell Functions ──────────── */
void		shell_initialize();
void		shell_process_command(const char* cmd);
void		shell_handle_input(char c);
int			strcmp(const char* s1, const char* s2);
void		handle_memstats();
void		handle_memtest();
void		handle_vmemstats();
void		handle_memcheck();

/* ──────────── Individual Memory Test Functions ──────────── */
void		handle_memtest1();
void		handle_memtest2();
void		handle_memtest3();
void		handle_memtest4();
void		handle_memtest4debug();
void		handle_memtest5();

/* ──────────── Crash Test Functions ──────────── */
void		handle_crashtest();
void		handle_crash1();
void		handle_crash2();
void		handle_crash3();
void		handle_crash4();
void		handle_crash5();
void		handle_crash6();

/* ──────────── Shell Helper Functions ──────────── */
void		handle_help();
void		handle_stack();
void		handle_push(const char* arg);
void		handle_pop();
void		handle_clear();
void		handle_reboot();
void		handle_halt();
void		handle_shutdown();
void		handle_unknown(const char* command);



#endif /* SHELL_FUNCTIONS_H */
