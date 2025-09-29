/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_gdt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:14:06 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 16:01:54 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kfs.h"

void handle_gdt() {
    print_gdt_info();
}

void handle_segments() {
    uint16_t cs, ds, es, fs, gs, ss;

    // Read current segment registers
    __asm__ volatile ("mov %%cs, %0" : "=r" (cs));
    __asm__ volatile ("mov %%ds, %0" : "=r" (ds));
    __asm__ volatile ("mov %%es, %0" : "=r" (es));
    __asm__ volatile ("mov %%fs, %0" : "=r" (fs));
    __asm__ volatile ("mov %%gs, %0" : "=r" (gs));
    __asm__ volatile ("mov %%ss, %0" : "=r" (ss));

    terminal_writestring("Current Segment Registers:\n");
    terminal_writestring("  CS (Code Segment):  0x");
    printnbr(cs, 16);
    terminal_writestring(" (Expected: 0x");
    printnbr(GDT_KERNEL_CODE, 16);
    terminal_writestring(")\n");

    terminal_writestring("  DS (Data Segment):  0x");
    printnbr(ds, 16);
    terminal_writestring(" (Expected: 0x");
    printnbr(GDT_KERNEL_DATA, 16);
    terminal_writestring(")\n");

    terminal_writestring("  ES (Extra Segment): 0x");
    printnbr(es, 16);
    terminal_writestring(" (Expected: 0x");
    printnbr(GDT_KERNEL_DATA, 16);
    terminal_writestring(")\n");

    terminal_writestring("  FS (F Segment):     0x");
    printnbr(fs, 16);
    terminal_writestring(" (Expected: 0x");
    printnbr(GDT_KERNEL_DATA, 16);
    terminal_writestring(")\n");

    terminal_writestring("  GS (G Segment):     0x");
    printnbr(gs, 16);
    terminal_writestring(" (Expected: 0x");
    printnbr(GDT_KERNEL_DATA, 16);
    terminal_writestring(")\n");

    terminal_writestring("  SS (Stack Segment): 0x");
    printnbr(ss, 16);
    terminal_writestring(" (Expected: 0x");
    printnbr(GDT_KERNEL_STACK, 16);
    terminal_writestring(")\n");
}

void handle_memory() {
    terminal_writestring("=== Memory Layout ===\n");
    terminal_writestring("GDT Base Address:     0x");
    printnbr((uint32_t)gdt, 16);
    terminal_writestring("\n");

    extern char _start, _end;
    terminal_writestring("Kernel Start:         0x");
    printnbr((uint32_t)&_start, 16);
    terminal_writestring("\n");
    terminal_writestring("Kernel End:           0x");
    printnbr((uint32_t)&_end, 16);
    terminal_writestring("\n");

    uint32_t kernel_size = (uint32_t)&_end - (uint32_t)&_start;
    terminal_writestring("Kernel Size:          ");
    printnbr(kernel_size, 10);
    terminal_writestring(" bytes (");
    printnbr(kernel_size / 1024, 10);
    terminal_writestring(" KB)\n");

    // Show stack information
    extern char stack_bottom, stack_top;
    terminal_writestring("Stack Bottom:         0x");
    printnbr((uint32_t)&stack_bottom, 16);
    terminal_writestring("\n");
    terminal_writestring("Stack Top:            0x");
    printnbr((uint32_t)&stack_top, 16);
    terminal_writestring("\n");
    terminal_writestring("Stack Size:           ");
    printnbr((uint32_t)&stack_top - (uint32_t)&stack_bottom, 10);
    terminal_writestring(" bytes (16 KB)\n");

    // Show current stack pointer
    uint32_t esp;
    __asm__ volatile ("mov %%esp, %0" : "=r" (esp));
    terminal_writestring("Current Stack Ptr:    0x");
    printnbr(esp, 16);
    terminal_writestring("\n");
    terminal_writestring("Stack Usage:          ");
    printnbr((uint32_t)&stack_top - esp, 10);
    terminal_writestring(" bytes\n");
}

void handle_gdttest() {
    terminal_writestring("=== GDT Segment Test ===\n");
    terminal_writestring("Testing segment descriptor access...\n");

    // Test reading from different segments
    uint16_t cs, ds, ss;
    __asm__ volatile ("mov %%cs, %0" : "=r" (cs));
    __asm__ volatile ("mov %%ds, %0" : "=r" (ds));
    __asm__ volatile ("mov %%ss, %0" : "=r" (ss));

    terminal_writestring("Before: CS=0x");
    printnbr(cs, 16);
    terminal_writestring(" DS=0x");
    printnbr(ds, 16);
    terminal_writestring(" SS=0x");
    printnbr(ss, 16);
    terminal_writestring("\n");

    // Test segment privilege levels
    terminal_writestring("Segment Privilege Levels:\n");
    terminal_writestring("  CS RPL: ");
    printnbr(cs & RPL_MASK, 10);
    terminal_writestring(" (Ring ");
    printnbr(cs & RPL_MASK, 10);
    terminal_writestring(")\n");

    terminal_writestring("  DS RPL: ");
    printnbr(ds & RPL_MASK, 10);
    terminal_writestring(" (Ring ");
    printnbr(ds & RPL_MASK, 10);
    terminal_writestring(")\n");

    terminal_writestring("  SS RPL: ");
    printnbr(ss & RPL_MASK, 10);
    terminal_writestring(" (Ring ");
    printnbr(ss & RPL_MASK, 10);
    terminal_writestring(")\n");

    terminal_writestring("GDT Test completed successfully!\n");
}
