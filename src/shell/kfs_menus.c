/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kfs_menus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 18:40:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/24 18:40:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

void display_kfs1_help(void)
{
	terminal_writestring("=== KFS1 (Boot/Screen) ===\n");
	terminal_writestring("  Mandatory:\n");
	terminal_writestring("    - GRUB boot + multiboot (boot.s)\n");
	terminal_writestring("    - VGA output (shell banner)\n");
	terminal_writestring("  Bonus demos:\n");
	terminal_writestring("    - Colors/cursor/scroll (VGA)\n");
	terminal_writestring("    - Keyboard input (type in shell)\n");
	terminal_writestring("    - Multi-screen: Ctrl+Shift+Left/Right\n");
}

void display_kfs3_help(void)
{
	terminal_writestring("=== KFS3 (Memory) ===\n");
	terminal_writestring("  Mandatory demos:\n");
	terminal_writestring("    - Paging enabled (kernel init)\n");
	terminal_writestring("    - kmalloc/vmalloc APIs\n");
	terminal_writestring("    - memtest command\n");
	terminal_writestring("  Bonus ideas:\n");
	terminal_writestring("    - heap debug/dump (not enabled)\n");
}

void display_kfs4_help(void)
{
	terminal_writestring("=== KFS4 (Interrupts) ===\n");
	terminal_writestring("  Mandatory demos:\n");
	terminal_writestring("    - interrupts (status + signals)\n");
	terminal_writestring("    - keyboard IRQs\n");
	terminal_writestring("    - panic path (page fault)\n");
	terminal_writestring("  Bonus demos:\n");
	terminal_writestring("    - syscall <n> (int 0x80)\n");
	terminal_writestring("    - layout qwerty|azerty\n");
	terminal_writestring("    - getline\n");
}

void handle_memtest(void)
{
	void *a;
	void *b;
	uint32_t size_a;
	uint32_t size_b;

	terminal_writestring("memtest: kmalloc/vmalloc demo\n");
	a = kmalloc(128);
	b = vmalloc(256);
	size_a = ksize(a);
	size_b = vsize(b);
	terminal_writestring("  kmalloc size: ");
	printnbr(size_a, 10);
	terminal_writestring("\n");
	terminal_writestring("  vmalloc size: ");
	printnbr(size_b, 10);
	terminal_writestring("\n");
	kfree(a);
	vfree(b);
	terminal_writestring("  freed ok\n");
}
