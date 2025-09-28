/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_sysinfo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:14:06 by rperez-t          #+#    #+#             */
/*   Updated: 2025/07/10 16:01:54 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kfs.h"

void handle_sysinfo() {
    terminal_writestring("=== Complete System Information ===\n");
    terminal_writestring("Kernel: ChickenKernel v1.0\n");
    terminal_writestring("Architecture: i386 (x86 32-bit)\n");
    terminal_writestring("Boot Method: Multiboot via GRUB\n");

    // GDT Info
    terminal_writestring("--- Global Descriptor Table ---\n");
    terminal_writestring("Location: 0x");
    printnbr(GDT_BASE_ADDRESS, 16);
    terminal_writestring("\n");
    terminal_writestring("Entries: 7 (Null, K-Code, K-Data, K-Stack, U-Code, U-Data, U-Stack)\n");

    // Current segments
    uint16_t cs, ds, ss;
    __asm__ volatile ("mov %%cs, %0" : "=r" (cs));
    __asm__ volatile ("mov %%ds, %0" : "=r" (ds));
    __asm__ volatile ("mov %%ss, %0" : "=r" (ss));
    terminal_writestring("Active: CS=0x");
    printnbr(cs, 16);
    terminal_writestring(" DS=0x");
    printnbr(ds, 16);
    terminal_writestring(" SS=0x");
    printnbr(ss, 16);
    terminal_writestring("\n\n");

    // Stack info
    terminal_writestring("--- Kernel Stack ---\n");
    terminal_writestring("Size: 16384 bytes (16 KB)\n");
    terminal_writestring("Current entries: ");
    printnbr(stack_size(), 10);
    terminal_writestring("\n");

    uint32_t esp;
    __asm__ volatile ("mov %%esp, %0" : "=r" (esp));
    extern char stack_top;
    terminal_writestring("Usage: ");
    printnbr((uint32_t)&stack_top - esp, 10);
    terminal_writestring(" bytes\n\n");

    // Memory info
    extern char _start, _end;
    uint32_t kernel_size = (uint32_t)&_end - (uint32_t)&_start;
    terminal_writestring("--- Memory Layout ---\n");
    terminal_writestring("Kernel size: ");
    printnbr(kernel_size / 1024, 10);
    terminal_writestring(" KB\n");
    terminal_writestring("Load address: 0x");
    printnbr((uint32_t)&_start, 16);
    terminal_writestring("\n\n");

    terminal_writestring("--- Features ---\n");
    terminal_writestring("✓ Custom GDT with 7 segments\n");
    terminal_writestring("✓ Interrupt handling (IDT + ISR + IRQ)\n");
    terminal_writestring("✓ Keyboard input support\n");
    terminal_writestring("✓ VGA text mode display\n");
    terminal_writestring("✓ Kernel stack operations\n");
    terminal_writestring("✓ Interactive shell\n");
}

void handle_interrupts() {
    terminal_writestring("=== Interrupt System Status ===\n");

    // Check if interrupts are enabled
    uint32_t eflags;
    __asm__ volatile ("pushf; pop %0" : "=r" (eflags));

    terminal_writestring("Interrupt Flag (IF): ");
    if (eflags & EFLAGS_INTERRUPT_FLAG) {
        terminal_writestring("ENABLED\n");
    } else {
        terminal_writestring("DISABLED\n");
    }

    terminal_writestring("EFLAGS Register: 0x");
    printnbr(eflags, 16);
    terminal_writestring("\n");

    terminal_writestring("\nConfigured Interrupts:\n");
    terminal_writestring("  IRQ 0 (Timer):    Enabled\n");
    terminal_writestring("  IRQ 1 (Keyboard): Enabled\n");
    terminal_writestring("  IRQ 2-15:         Masked\n");

    terminal_writestring("\nPIC Configuration:\n");
    terminal_writestring("  Master PIC:       IRQ 32-39\n");
    terminal_writestring("  Slave PIC:        IRQ 40-47\n");

    terminal_writestring("\nIDT Status:\n");
    terminal_writestring("  Total gates:      256\n");
    terminal_writestring("  ISR handlers:     0-31 (CPU exceptions)\n");
    terminal_writestring("  IRQ handlers:     32-47 (Hardware interrupts)\n");
}
